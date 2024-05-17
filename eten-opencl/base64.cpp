
Przepraszam za niedokładność. Poprawiłem to i teraz kod zwraca zaszyfrowany ciąg jako wynik funkcji encode. Nie wypisuje on niczego bezpośrednio na standardowe wyjście. Jeśli chcesz wyświetlić zaszyfrowany ciąg, możesz to zrobić w funkcji main po wywołaniu funkcji encode. Oto poprawiony kod:

cpp
Skopiuj kod
#include <iostream>
#include <CL/cl.h>
#include <cstring>

const char* Base64kernelSource = R"(
__kernel void base64_encode(__global const unsigned char* input, __global char* output, int input_size) {
    const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
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

std::string encode(const std::string& inputed) {
    const char* input = inputed.c_str();
    size_t input_size = strlen(input);
    size_t output_size = ((input_size + 2) / 3) * 4;
    
    char* output = new char[output_size + 1];
    output[output_size] = '\0';

    cl_int err;
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;

    err = clGetPlatformIDs(1, &platform, NULL); 
    if (err != CL_SUCCESS) {
        std::cerr << "Error getting platform IDs: " << err << std::endl;
        delete[] output;
        return "";
    }
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL); 
    if (err != CL_SUCCESS) {
        std::cerr << "Error getting device IDs: " << err << std::endl;
        delete[] output;
        return "";
    }

    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err); 
    if (err != CL_SUCCESS) {
        std::cerr << "Error creating context: " << err << std::endl;
        delete[] output;
        return "";
    }
    queue = clCreateCommandQueue(context, device, 0, &err); 
    if (err != CL_SUCCESS) {
        std::cerr << "Error creating command queue: " << err << std::endl;
        delete[] output;
        return "";
    }

    program = clCreateProgramWithSource(context, 1, &Base64kernelSource, NULL, &err); 
    if (err != CL_SUCCESS) {
        std::cerr << "Error creating program with source: " << err << std::endl;
        delete[] output;
        return "";
    }
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL); 
    if (err != CL_SUCCESS) {
        std::cerr << "Error building program: " << err << std::endl;
        size_t len;
        char buffer[2048];
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        std::cerr << "Build log: " << buffer << std::endl;
        clReleaseProgram(program);
        delete[] output;
        return "";
    }

    kernel = clCreateKernel(program, "base64_encode", &err); 
    if (err != CL_SUCCESS) {
        std::cerr << "Error creating kernel: " << err << std::endl;
        clReleaseProgram(program);
        delete[] output;
        return "";
    }

    cl_mem input_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, input_size, (void*)input, &err); 
    if (err != CL_SUCCESS) {
        std::cerr << "Error creating input buffer: " << err << std::endl;
        clReleaseKernel(kernel);
        clReleaseProgram(program);
        delete[] output;
        return "";
    }
    cl_mem output_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, output_size, NULL, &err); 
    if (err != CL_SUCCESS) {
        std::cerr << "Error creating output buffer: " << err << std::endl;
        clReleaseMemObject(input_buffer);
        clReleaseKernel(kernel);
        clReleaseProgram(program);
        delete[] output;
        return "";
    }

    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_buffer); 
    if (err != CL_SUCCESS) {
        std::cerr << "Error setting kernel arg 0: " << err << std::endl;
        clReleaseMemObject(input_buffer);
        clReleaseMemObject(output_buffer);
        clReleaseKernel(kernel);
        clReleaseProgram(program);
        delete[] output;
        return "";
    }
    err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &output_buffer); 
    if (err != CL_SUCCESS) {
        std::cerr << "Error setting kernel arg 1: " << err << std::endl;
        clReleaseMemObject(input_buffer);
        clReleaseMemObject(output_buffer);
        clReleaseKernel(kernel);
        clReleaseProgram(program);
        delete[] output;
        return "";
    }
    err = clSetKernelArg(kernel, 2, sizeof(int), &input_size); 
    if (err != CL_SUCCESS) {
        std::cerr << "Error setting kernel arg 2: " << err << std::endl;
        clReleaseMemObject(input_buffer);
        clReleaseMemObject(output_buffer);
        clReleaseKernel(kernel);
        clReleaseProgram(program);
        delete[] output;
        return "";
    }

    size_t global_size = (input_size + 2) / 3;

    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_size, NULL, 0, NULL, NULL);

Przepraszam za przerwanie, wygląda na to, że popełniłem błąd. Kontynuujmy przepisywanie reszty kodu:

cpp
Skopiuj kod
    if (err != CL_SUCCESS) {
        std::cerr << "Error enqueuing ND range kernel: " << err << std::endl;
        clReleaseMemObject(input_buffer);
        clReleaseMemObject(output_buffer);
        clReleaseKernel(kernel);
        clReleaseProgram(program);
        delete[] output;
        return "";
    }

    err = clEnqueueReadBuffer(queue, output_buffer, CL_TRUE, 0, output_size, output, 0, NULL, NULL); 
    if (err != CL_SUCCESS) {
        std::cerr << "Error reading output buffer: " << err << std::endl;
        clReleaseMemObject(input_buffer);
        clReleaseMemObject(output_buffer);
        clReleaseKernel(kernel);
        clReleaseProgram(program);
        delete[] output;
        return "";
    }

    clReleaseMemObject(input_buffer);
    clReleaseMemObject(output_buffer);
    clReleaseKernel(kernel);
    clReleaseProgram(program);

    std::string result(output);
    delete[] output;
    return result;
}