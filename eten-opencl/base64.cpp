#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#define CHECK_ERROR(err) if (err != CL_SUCCESS) { printf("Error: %d\n", err); exit(EXIT_FAILURE); }

const char* kernelSource = R"(
__constant char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

__kernel void base64_encode(__global const unsigned char* input, __global char* output, int input_size) {
    int gid = get_global_id(0);

    if (gid * 3 < input_size) {
        unsigned int in1 = input[gid * 3];
        unsigned int in2 = (gid * 3 + 1 < input_size) ? input[gid * 3 + 1] : 0;
        unsigned int in3 = (gid * 3 + 2 < input_size) ? input[gid * 3 + 2] : 0;

        output[gid * 4]     = base64_chars[in1 >> 2];
        output[gid * 4 + 1] = base64_chars[((in1 & 0x3) << 4) | (in2 >> 4)];
        output[gid * 4 + 2] = (gid * 3 + 1 < input_size) ? base64_chars[((in2 & 0xF) << 2) | (in3 >> 6)] : '=';
        output[gid * 4 + 3] = (gid * 3 + 2 < input_size) ? base64_chars[in3 & 0x3F] : '=';
    }
}
)";

std::string encode(std::string inputed) {
    const char* input = inputed.c_str();
    size_t input_size = strlen(input);
    size_t output_size = ((input_size + 2) / 3) * 4;
    
    char* output = (char*)malloc(output_size + 1);
    output[output_size] = '\0';

    cl_int err;
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;

    err = clGetPlatformIDs(1, &platform, NULL); CHECK_ERROR(err);
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL); CHECK_ERROR(err);

    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err); CHECK_ERROR(err);
    queue = clCreateCommandQueue(context, device, 0, &err); CHECK_ERROR(err);

    program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, &err); CHECK_ERROR(err);
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL); CHECK_ERROR(err);

    kernel = clCreateKernel(program, "base64_encode", &err); CHECK_ERROR(err);

    cl_mem input_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, input_size, (void*)input, &err); CHECK_ERROR(err);
    cl_mem output_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, output_size, NULL, &err); CHECK_ERROR(err);

    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_buffer); CHECK_ERROR(err);
    err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &output_buffer); CHECK_ERROR(err);
    err = clSetKernelArg(kernel, 2, sizeof(int), &input_size); CHECK_ERROR(err);

    size_t global_size = (input_size + 2) / 3;

    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_size, NULL, 0, NULL, NULL); CHECK_ERROR(err);


    err = clEnqueueReadBuffer(queue, output_buffer, CL_TRUE, 0, output_size, output, 0, NULL, NULL); CHECK_ERROR(err);

    clReleaseMemObject(input_buffer);
    clReleaseMemObject(output_buffer);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return output;
}
