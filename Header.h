#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <climits>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "matrix.h"
//using namespace std;



uint64_t mult32_mod_p(uint64_t a)
{
    const uint64_t p = 9223372036854775783;
    a = a % p;
    uint64_t a1 = a >> 32;
    uint64_t b1 = 0xffffffff & a;
    uint64_t residue = ((b1 << 32) % p) + a1 * 50;
    return residue % p;
}

uint64_t mult64_mod_p(uint64_t a)
{
    const uint64_t p = 9223372036854775783;
    a = a % p;
    uint64_t a1 = a >> 32;
    uint64_t b1 = 0xffffffff & a;
    uint64_t residue = mult32_mod_p(a1 * 50) + b1 * 50;
    return residue % p;
}


uint64_t mult(uint64_t a, uint64_t b)
{
    const uint64_t p = 9223372036854775783;
    uint64_t a1 = a;
    a1 = a1 >> 32;
    uint64_t b1 = 0xffffffff & a;
    uint64_t a2 = b;
    a2 = a2 >> 32;
    uint64_t b2 = 0xffffffff & b;
    uint64_t A = mult64_mod_p(a1 * a2);
    uint64_t B = mult32_mod_p(a1 * b2);
    uint64_t C = mult32_mod_p(a2 * b1);
    uint64_t D = (b1 * b2) % p;

    return ((((A + B) % p + C) % p + D) % p);
}

bool test_mult(uint64_t a, uint64_t b, uint64_t c)
{
    if (mult(a, b) == c)
    {
        return true;
    }
    else {
        return false;
    }
}

//std::string kernel_load(std::string path)
//{
//    std::ifstream file(path);
//    std::stringstream buffer;
//    buffer << file.rdbuf();
//    return buffer.str();
//}


//void vector_sub(std::vector<uint64_t>& input_vector, uint64_t a)
//{
//    std::string S = kernel_load("VectorAdd.cl");
//
//    // Two integer source vectors in Host memory
//    std::vector<uint64_t> HostVector1;
//    std::vector<uint64_t> HostVector2;
//    //Output Vector
//    std::vector<uint64_t> HostOutputVector;
//
//    HostVector1.resize(input_vector.size());
//    HostVector2.resize(input_vector.size());
//    HostOutputVector.resize(input_vector.size());
//    uint64_t size = input_vector.size();
//
//    // Initialize with some interesting repeating data
//    for (int c = 0; c < size; c++)
//    {
//        HostVector1[c] = input_vector[c];
//        HostVector2[c] = a;
//        HostOutputVector[c] = 0;
//    }
//
//    //Get an OpenCL platform
//
//    cl_platform_id cpPlatform;
//    clGetPlatformIDs(1, &cpPlatform, NULL);
//
//    // Get a GPU device
//    cl_device_id cdDevice;
//    clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 1, &cdDevice, NULL);
//
//    char cBuffer[1024];
//    clGetDeviceInfo(cdDevice, CL_DEVICE_NAME, sizeof(cBuffer), &cBuffer, NULL);
//    printf("CL_DEVICE_NAME: %s\n", cBuffer);
//    clGetDeviceInfo(cdDevice, CL_DRIVER_VERSION, sizeof(cBuffer), &cBuffer, NULL);
//    printf("CL_DRIVER_VERSION: %s\n\n", cBuffer);
//    cl_context_properties properties[] = { CL_CONTEXT_PLATFORM, (uint64_t)cpPlatform, 0 };
//    // Create a context to run OpenCL enabled GPU
//
//    cl_context GPUContext = clCreateContextFromType(properties, CL_DEVICE_TYPE_GPU, NULL, NULL, NULL);
//
//    // Create a command-queue on the GPU device
//    cl_command_queue cqCommandQueue = clCreateCommandQueueWithProperties(GPUContext, cdDevice, 0, NULL);
//
//    // Allocate GPU memory for source vectors AND initialize from CPU memory
//
//    cl_mem GPUVector1 = clCreateBuffer(GPUContext, CL_MEM_READ_ONLY |
//        CL_MEM_COPY_HOST_PTR, sizeof(uint64_t) * size, HostVector1.data(), NULL);
//
//    cl_mem GPUVector2 = clCreateBuffer(GPUContext, CL_MEM_READ_ONLY |
//        CL_MEM_COPY_HOST_PTR, sizeof(uint64_t) * size, HostVector2.data(), NULL);
//
//    // Allocate output memory on GPU
//    cl_mem GPUOutputVector = clCreateBuffer(GPUContext, CL_MEM_WRITE_ONLY,
//        sizeof(uint64_t) * size, NULL, NULL);
//
//    const char* cstr = S.c_str();
//
//    // Create OpenCL program with source code
//    cl_program OpenCLProgram = clCreateProgramWithSource(GPUContext, 1, &cstr, NULL, NULL);
//
//    // Build the program (OpenCL JIT compilation)
//    clBuildProgram(OpenCLProgram, 0, NULL, NULL, NULL, NULL);
//
//    // Create a handle to the compiled OpenCL function (Kernel)
//    cl_kernel OpenCLVectorAdd = clCreateKernel(OpenCLProgram, "VectorAdd", NULL);
//
//    // In the next step we associate the GPU memory with the Kernel arguments
//
//    clSetKernelArg(OpenCLVectorAdd, 0, sizeof(cl_mem), (void*)&GPUOutputVector);
//    clSetKernelArg(OpenCLVectorAdd, 1, sizeof(cl_mem), (void*)&GPUVector1);
//    clSetKernelArg(OpenCLVectorAdd, 2, sizeof(cl_mem), (void*)&GPUVector2);
//
//    // Launch the Kernel on the GPU
//
//    // This kernel only uses global data
//
//    size_t WorkSize[1] = { size }; // one dimensional Range
//
//    clEnqueueNDRangeKernel(cqCommandQueue, OpenCLVectorAdd, 1, NULL,
//        WorkSize, NULL, 0, NULL, NULL);
//
//    // Copy the output in GPU memory back to CPU memory
//    clEnqueueReadBuffer(cqCommandQueue, GPUOutputVector, CL_TRUE, 0,
//        size * sizeof(uint64_t), HostOutputVector.data(), 0, NULL, NULL);
//
//    // Cleanup
//    clReleaseKernel(OpenCLVectorAdd);
//    clReleaseProgram(OpenCLProgram);
//    clReleaseCommandQueue(cqCommandQueue);
//    clReleaseContext(GPUContext);
//    clReleaseMemObject(GPUVector1);
//    clReleaseMemObject(GPUVector2);
//    clReleaseMemObject(GPUOutputVector);
//
//    for (int i = 0; i < size; i++)
//        printf("[%llu + %llu = %llu]\n", HostVector1[i], HostVector2[i], HostOutputVector[i]);
//}



//void vector_mult(std::vector<uint64_t>& input_vector, uint64_t a)
//{
//    std::string S = kernel_load("VectorMult.cl");
//    // Two integer source vectors in Host memory
//    std::vector<uint64_t> HostVector1;
//    std::vector<uint64_t> HostVector2;
//    //Output Vector
//    std::vector<uint64_t> HostOutputVector;
//
//    cl_int err;
//
//    HostVector1.resize(input_vector.size());
//    HostVector2.resize(input_vector.size());
//    HostOutputVector.resize(input_vector.size());
//    uint64_t size = input_vector.size();
//
//    for (int c = 0; c < size; c++)
//    {
//        HostVector1[c] = input_vector[c];
//        HostVector2[c] = a;
//        HostOutputVector[c] = 0;
//    }
//
//    //Get an OpenCL platform
//
//    cl_platform_id cpPlatform;
//    clGetPlatformIDs(1, &cpPlatform, NULL);
//
//    // Get a GPU device
//    cl_device_id cdDevice;
//    clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 1, &cdDevice, NULL);
//
//    char cBuffer[1024];
//    clGetDeviceInfo(cdDevice, CL_DEVICE_NAME, sizeof(cBuffer), &cBuffer, NULL);
//    printf("CL_DEVICE_NAME: %s\n", cBuffer);
//    clGetDeviceInfo(cdDevice, CL_DRIVER_VERSION, sizeof(cBuffer), &cBuffer, NULL);
//    printf("CL_DRIVER_VERSION: %s\n\n", cBuffer);
//    cl_context_properties properties[] = { CL_CONTEXT_PLATFORM, (uint64_t)cpPlatform, 0 };
//    // Create a context to run OpenCL enabled GPU
//
//    cl_context GPUContext = clCreateContextFromType(properties, CL_DEVICE_TYPE_GPU, NULL, NULL, NULL);
//
//    // Create a command-queue on the GPU device
//    cl_command_queue cqCommandQueue = clCreateCommandQueueWithProperties(GPUContext, cdDevice, 0, NULL);
//
//    // Allocate GPU memory for source vectors AND initialize from CPU memory
//
//    cl_mem GPUVector1 = clCreateBuffer(GPUContext, CL_MEM_READ_ONLY |
//        CL_MEM_COPY_HOST_PTR, sizeof(uint64_t) * size, HostVector1.data(), NULL);
//
//    cl_mem GPUVector2 = clCreateBuffer(GPUContext, CL_MEM_READ_ONLY |
//        CL_MEM_COPY_HOST_PTR, sizeof(uint64_t) * size, HostVector2.data(), NULL);
//
//    // Allocate output memory on GPU
//    cl_mem GPUOutputVector = clCreateBuffer(GPUContext, CL_MEM_WRITE_ONLY,
//        sizeof(uint64_t) * size, NULL, NULL);
//
//    const char* cstr = S.c_str();
//
//    // Create OpenCL program with source code
//    cl_program OpenCLProgram = clCreateProgramWithSource(GPUContext, 1, &cstr, NULL, NULL);
//
//    // Build the program (OpenCL JIT compilation)
//    err = clBuildProgram(OpenCLProgram, 0, NULL, NULL, NULL, NULL);
//    if (err != CL_SUCCESS)
//    {
//        printf("Nie dziala\n");
//        char str[8192];
//        size_t len;
//        clGetProgramBuildInfo(OpenCLProgram, cdDevice, CL_PROGRAM_BUILD_LOG, 8192, str, &len);
//        if (len > 8192)
//        {
//            printf("Nie miesci sie\n");
//        }
//        printf("%s", str);
//    }
//    // Create a handle to the compiled OpenCL function (Kernel)
//    cl_kernel OpenCLVectorAdd = clCreateKernel(OpenCLProgram, "VectorMult", NULL);
//
//    // In the next step we associate the GPU memory with the Kernel arguments
//
//    clSetKernelArg(OpenCLVectorAdd, 0, sizeof(cl_mem), (void*)&GPUOutputVector);
//    clSetKernelArg(OpenCLVectorAdd, 1, sizeof(cl_mem), (void*)&GPUVector1);
//    clSetKernelArg(OpenCLVectorAdd, 2, sizeof(cl_mem), (void*)&GPUVector2);
//
//    // Launch the Kernel on the GPU
//
//    // This kernel only uses global data
//
//    size_t WorkSize[1] = { size }; // one dimensional Range
//
//    clEnqueueNDRangeKernel(cqCommandQueue, OpenCLVectorAdd, 1, NULL,
//        WorkSize, NULL, 0, NULL, NULL);
//
//    // Copy the output in GPU memory back to CPU memory
//    clEnqueueReadBuffer(cqCommandQueue, GPUOutputVector, CL_TRUE, 0,
//        size * sizeof(uint64_t), HostOutputVector.data(), 0, NULL, NULL);
//
//    // Cleanup
//    clReleaseKernel(OpenCLVectorAdd);
//    clReleaseProgram(OpenCLProgram);
//    clReleaseCommandQueue(cqCommandQueue);
//    clReleaseContext(GPUContext);
//    clReleaseMemObject(GPUVector1);
//    clReleaseMemObject(GPUVector2);
//    clReleaseMemObject(GPUOutputVector);
//
//    for (int i = 0; i < size; i++)
//        printf("[%llu * %llu = %llu]\n", HostVector1[i], HostVector2[i], HostOutputVector[i]);
//
//}



////std::vector<uint64_t>
//void gauss(Matrix A, std::vector<uint64_t> x) {
//    const uint64_t p = 9223372036854775783;
//    int invert_row_index;
//
//    uint64_t x_inv, y_inv;
//
//    for (int i = 0; i < A.n_cols(); i++) {
//        invert_row_index = A.find_inv(i, 7);
//        A.swap_rows(invert_row_index, i);
//        for (int j = 0; j < A.n_cols(); j++) {
//            uint64_t xgcd = gcdExtended(A.at(i, j), p - 1, &x_inv, &y_inv);
//        }
//    }
//
//}
