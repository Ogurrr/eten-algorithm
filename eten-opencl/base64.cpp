#include <CL/cl.h>
#include <iostream>
#include <vector>

int main() {
    // Initialize OpenCL environment
    cl_platform_t platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;

    // Get OpenCL platform and device
    cl_uint num_platforms;
    clGetPlatformIDs(1, &platform, &num_platforms);
    cl_uint num_devices;
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, &num_devices);

    // Create OpenCL context and command queue
    context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    queue = clCreateCommandQueue(context, device, 0, NULL);

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
    size_t kernel_source_size = kernel_source.size();
    program = clCreateProgramWithSource(context, 1, (const char **)&kernel_source, &kernel_source_size, NULL);
    clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

    // Create kernel function object
    kernel = clCreateKernel(program, "base64_encode", NULL);

    // Create input and output buffers
    std::vector<uchar> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
    std::vector<uchar> output(input.size() * 4 / 3 + 3);

    // Create buffer objects for input and output data
    cl_mem input_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY, input.size() * sizeof(uchar), NULL, NULL);
    cl_mem output_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, output.size() * sizeof(uchar), NULL, NULL);

    // Write input data to input buffer
    clEnqueueWriteBuffer(queue, input_buffer, CL_TRUE, 0, input.size() * sizeof(uchar), input.data(), 0, NULL, NULL);

    // Set kernel arguments
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_buffer);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &output_buffer);
    clSetKernelArg(kernel, 2, sizeof(uint), &input.size());
    clSetKernelArg(kernel, 3, sizeof(uint), &output.size());

    // Execute kernel function
    size_t global_work_size = input.size() / 3;
    size_t local_work_size = 256;
    clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_work_size, &local_work_size, 0, NULL, NULL);

    // Read output data from output buffer
    clEnqueueReadBuffer(queue, output_buffer, CL_TRUE, 0, output.size() * sizeof(uchar), output.data(), 0, NULL, NULL);

    // Print output data
    std::cout << "Output data:" << std::endl;
    for (uchar c : output) {
        std::cout << c;
    }
    std::cout << std::endl;

    // Release OpenCL resources
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseMemObject(input_buffer);
    clReleaseMemObject(output_buffer);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return 0;
}