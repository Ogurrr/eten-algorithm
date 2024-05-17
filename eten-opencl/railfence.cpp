#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CL/cl.h>
#include <iostream>
// Funkcja do sprawdzania błędów
#define CHECK_ERROR(err) if (err != CL_SUCCESS) { printf("Error: %d\n", err); exit(EXIT_FAILURE); }

const char* kernelSource = R"(
__kernel void rail_fence_cipher(__global const char* input, __global char* output, int num_rails, int input_length) {
    int gid = get_global_id(0);

    if (gid < input_length) {
        int period = 2 * (num_rails - 1);
        int cycle_pos = gid % period;
        int rail;
        
        if (cycle_pos < num_rails) {
            rail = cycle_pos;
        } else {
            rail = period - cycle_pos;
        }

        int pos = (cycle_pos < num_rails) ? cycle_pos : period - cycle_pos;
        int index = (input_length / period) * rail + (input_length % period > cycle_pos ? rail + 1 : rail);

        for (int i = 0; i < gid / period; i++) {
            index += period;
        }

        output[index] = input[gid];
    }
}
)";

std::string encryptRailFence(string text, int num_rails) {
    // Przykładowe dane wejściowe
    const char* input = text.c_str();
    size_t input_length = strlen(input);
    
    // Alokacja pamięci na dane wyjściowe
    char* output = (char*)malloc(input_length + 1);
    output[input_length] = '\0';

    // OpenCL setup
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

    program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, &err); CHECK_ERROR(err);
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

    kernel = clCreateKernel(program, "rail_fence_cipher", &err); CHECK_ERROR(err);

    // Alokacja i inicjalizacja pamięci na urządzeniu
    cl_mem input_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, input_length, (void*)input, &err); CHECK_ERROR(err);
    cl_mem output_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, input_length, NULL, &err); CHECK_ERROR(err);

    // Ustawianie argumentów kernela
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_buffer); CHECK_ERROR(err);
    err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &output_buffer); CHECK_ERROR(err);
    err = clSetKernelArg(kernel, 2, sizeof(int), &num_rails); CHECK_ERROR(err);
    err = clSetKernelArg(kernel, 3, sizeof(int), &input_length); CHECK_ERROR(err);

    size_t global_size = input_length;

    // Uruchomienie kernela
    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_size, NULL, 0, NULL, NULL); CHECK_ERROR(err);

    // Odczyt wyników
    err = clEnqueueReadBuffer(queue, output_buffer, CL_TRUE, 0, input_length, output, 0, NULL, NULL); CHECK_ERROR(err);

    // Wyświetlenie wyników
    printf("Input: %s\n", input);
    printf("Output: %s\n", output);

    // Zwolnienie zasobów
    clReleaseMemObject(input_buffer);
    clReleaseMemObject(output_buffer);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    free(output);

    return 0;
}
