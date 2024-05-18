#include <CL/cl.hpp>
#include <iostream>
#include <vector>

int main() {
    // Initialize OpenCL environment
    cl::Platform platform;
    cl::Device device;
    cl::Context context;
    cl::CommandQueue queue;
    cl::Program program;
    cl::Kernel kernel;

    // Get OpenCL platform and device
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    platform = platforms[0];
    std::vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
    device = devices[0];

    // Create OpenCL context and command queue
    context = cl::Context(device);
    queue = cl::CommandQueue(context, device);

    // Read kernel source code from file
    std::string kernel_source = R"(
__kernel void base64_encode(
    __global const uchar* input,
    __global uchar* output,
    const uint input_size,
    const uint output_size)
{
    uint gid = get_global_id(0);
    uint lid = get_local_id(0);
    uint gs = get_global_size(0);
    uint ls = get_local_size(0);

    uint start_index = gid * 3;
    uint end_index = start_index + 3;

    if (start_index >= input_size) {
        return;
    }

    uint index = start_index / 3 * 4 + lid;
    uint offset = start_index % 3;

    uchar3 data;
    data.x = input[start_index];
    data.y = offset == 1 ? 0 : input[start_index + 1];
    data.z = offset == 2 ? 0 : input[start_index + 2];

    uchar4 encoded;
    encoded.x = (data.x >> 2) & 0x3F;
    encoded.y = ((data.x & 0x03) << 4) | ((data.y >> 4) & 0x0F);
    encoded.z = ((data.y & 0x0F) << 2) | ((data.z >> 6) & 0x3F);
    encoded.w = data.z & 0x3F;

    if (index < output_size) {
        output[index] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(encoded.x >> (6 - 2 * offset)) & 0x3F];
        output[index + 1] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(encoded.y >> (6 - 2 * offset)) & 0x3F];
        output[index + 2] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(encoded.z >> (6 - 2 * offset)) & 0x3F];
        output[index + 3] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(encoded.w >> (6 - 2 * offset)) & 0x3F];
    }
}
)";
    // Compile kernel source code
    cl::Program::Sources sources;
    sources.push_back(std::make_pair(kernel_source.c_str(), kernel_source.length()));
    program = cl::Program(context, sources);
    program.build("-cl-std=CL1.2");

    // Create kernel function object
    kernel = cl::Kernel(program, "base64_encode");

    // Create input and output buffers
    std::vector<uchar> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
    std::vector<uchar> output(input.size() * 4 / 3 + 3);

    // Create buffer objects for input and output data
    cl::Buffer input_buffer(context, CL_MEM_READ_ONLY, input.size() * sizeof(uchar));
    cl::Buffer output_buffer(context, CL_MEM_WRITE_ONLY, output.size() * sizeof(uchar));

    // Write input data to input buffer
    queue.enqueueWriteBuffer(input_buffer, CL_TRUE, 0, input.size() * sizeof(uchar), input.data());

    // Set kernel arguments
    kernel.setArg(0, input_buffer);
    kernel.setArg(1, output_buffer);
    kernel.setArg(2, static_cast<uint>(input.size()));
    kernel.setArg(3, static_cast<uint>(output.size()));

    // Execute kernel function
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(input.size() / 3), cl::NullRange);

    // Read output data from output buffer
    queue.enqueueReadBuffer(output_buffer, CL_TRUE, 0, output.size() * sizeof(uchar), output.data());

    // Print output data
    std::cout << "Output data:" << std::endl;
    for (uchar c : output) {
        std::cout << c;
    }
    std::cout << std::endl;

    return 0;
}