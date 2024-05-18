#include <CL/cl.h>
#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>

int main() {
    // Initialize OpenCL environment
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;

    // Get OpenCL platform and device
    cl_uint num_platforms;
    clGetPlatformIDs(1, NULL, &num_platforms);
    if (num_platforms == 0) {
        std::cerr << "No OpenCL platforms found." << std::endl;
        return 1;
    }
    clGetPlatformIDs(1, &platform, NULL);
    cl_uint num_devices;
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, NULL, &num_devices);
    if (num_devices == 0) {
        std::cerr << "No OpenCL devices found." << std::endl;
        return 1;
    }
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

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
    cl_int status;
    program = clCreateProgramWithSource(context, 1, (const char **)&kernel_source, NULL, &status);
    if (status != CL_SUCCESS) {
        std::cerr << "Error creating program: " << clGetErrorString(status) << std::endl;
        return 1;
    }
    status = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (status != CL_SUCCESS) {
        std::cerr << "Error building program: " << clGetErrorString(status) << std::endl;
        return 1;
    }

    // Create kernel function object
    kernel = clCreateKernel(program, "base64_encode", &status);
    if (status != CL_SUCCESS) {
        std::cerr << "Error creating kernel: " << clGetErrorString(status) << std::endl;
        return 1;
    }

    // Create input and output buffers
    std::vector<unsigned char> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
    std::vector<unsigned char> output(input.size() * 4 / 3 + 3);

    // Create buffer objects for input and output data
    cl_mem input_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY, input.size() * sizeof(unsigned char), NULL, NULL);
    cl_mem output_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, output.size() * sizeof(unsigned char), NULL, NULL);

    // Write input data to input buffer
    status = clEnqueueWriteBuffer(queue, input_buffer, CL_TRUE, 0, input.size() * sizeof(unsigned char), input.data(), 0, NULL, NULL);
    if (status != CL_SUCCESS) {
        std::cerr << "Error writing input buffer: " << clGetErrorString(status) << std::endl;
        return 1;
    }

    // Set kernel arguments
    status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_buffer);
    if (status != CL_SUCCESS) {
        std::cerr << "Error setting kernel argument 0: " << clGetErrorString(status) << std::endl;
        return 1;
    }
    status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &output_buffer);
    if (status != CL_SUCCESS) {
        std::cerr << "Error setting kernel argument 1: " << clGetErrorString(status) << std::endl;
        return 1;
    }
    status = clSetKernelArg(kernel, 2, sizeof(uint32_t), &input.size());
    if (status != CL_SUCCESS) {
        std::cerr << "Error setting kernel argument 2: " << clGetErrorString(status) << std::endl;
        return 1;
    }
    status = clSetKernelArg(kernel, 3, sizeof(uint32_t), &output.size() * sizeof(unsigned char));
    if (status != CL_SUCCESS) {
        std::cerr << "Error setting kernel argument 3: " << clGetErrorString(status) << std::endl;
        return 1;
    }

    // Execute kernel function
    size_t global_work_size = input.size() / 3;
    size_t local_work_size = 256;
    status = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_work_size, &local_work_size, 0, NULL, NULL);
    if (status != CL_SUCCESS) {
        std::cerr << "Error executing kernel: " << clGetErrorString(status) << std::endl;
        return 1;
    }

    // Read output data from output buffer
    status = clEnqueueReadBuffer(queue, output_buffer, CL_TRUE, 0, output.size() * sizeof(unsigned char), output.data(), 0, NULL, NULL);
    if (status != CL_SUCCESS) {
        std::cerr << "Error reading output buffer: " << clGetErrorString(status) << std::endl;
        return 1;
    }

    // Print output data
    std::for_each(output.begin(), output.end(), [](unsigned char c) {
        std::cout << c;
    });
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