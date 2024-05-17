#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CL/cl.h>
#include <iostream>

// Funkcja do sprawdzania błędów
#define CHECK_ERROR(err) if (err != CL_SUCCESS) { printf("Error: %d\n", err); exit(EXIT_FAILURE); }

const char* CezarkernelSource = R"(
__kernel void caesar_cipher(__global const char* input, __global char* output, int shift, int input_size) {
    int gid = get_global_id(0);

    if (gid < input_size) {
        char ch = input[gid];
        if (ch >= 'a' && ch <= 'z') {
            output[gid] = ((ch - 'a' + shift) % 26) + 'a';
        } else if (ch >= 'A' && ch <= 'Z') {
            output[gid] = ((ch - 'A' + shift) % 26) + 'A';
        } else {
            output[gid] = ch;
        }
    }
}
)";

std::string caesarEncrypt(const std::string& inputed, int shift) {
    const char* input = inputed.c_str();
    size_t input_size = strlen(input);  
    
    char* output = (char*)malloc(input_size + 1);
    output[input_size] = '\0';

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
    queue = clCreateCommandQueueWithProperties(context, device, 0, &err); CHECK_ERROR(err);

    program = clCreateProgramWithSource(context, 1, &CezarkernelSource, NULL, &err); CHECK_ERROR(err);
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL); 
    if (err != CL_SUCCESS) {
        size_t log_size;
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        char* log = (char*)malloc(log_size);
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
        printf("Build log:\n%s\n", log);
        free(log);
        exit(EXIT_FAILURE);
    }

    kernel = clCreateKernel(program, "caesar_cipher", &err); CHECK_ERROR(err);

    cl_mem input_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, input_size, (void*)input, &err); CHECK_ERROR(err);
    cl_mem output_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, input_size, NULL, &err); CHECK_ERROR(err);

    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_buffer); CHECK_ERROR(err);
    err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &output_buffer); CHECK_ERROR(err);
    err = clSetKernelArg(kernel, 2, sizeof(int), &shift); CHECK_ERROR(err);
    err = clSetKernelArg(kernel, 3, sizeof(int), &input_size); CHECK_ERROR(err);

    size_t global_size = input_size;

    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_size, NULL, 0, NULL, NULL); CHECK_ERROR(err);

    err = clEnqueueReadBuffer(queue, output_buffer, CL_TRUE, 0, input_size, output, 0, NULL, NULL); CHECK_ERROR(err);

    clReleaseMemObject(input_buffer);
    clReleaseMemObject(output_buffer);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    std::string result(output);
    free(output);
    return result;
}