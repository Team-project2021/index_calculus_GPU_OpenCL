#pragma once
#include <vector>
#include <stdexcept>
#include <iostream>
#include <tuple>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include "Header.h"

using namespace std;


uint64_t gcdExtended(uint64_t a, uint64_t b, uint64_t* x, uint64_t* y)
{
	if (a == 0)
	{
		*x = 0;
		*y = 1;
		return b;
	}

	uint64_t x1, y1;
	uint64_t gcd = gcdExtended(b % a, a, &x1, &y1);

	*x = y1 - (b / a) * x1;
	*y = x1;

	return gcd;
}

uint64_t gcd(uint64_t a, uint64_t b) {
	uint64_t x, y;
	return  gcdExtended(a, b, &x, &y);
}

std::string kernel_load(std::string path)
{
	std::ifstream file(path);
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

tuple<cl_platform_id, cl_device_id, cl_context_properties, cl_context, cl_command_queue > initialize_openCL()
{
	cl_platform_id cpPlatform;
	clGetPlatformIDs(1, &cpPlatform, NULL);

	// Get a GPU device
	cl_device_id cdDevice;
	clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 1, &cdDevice, NULL);

	char cBuffer[1024];
	clGetDeviceInfo(cdDevice, CL_DEVICE_NAME, sizeof(cBuffer), &cBuffer, NULL);
	//printf("CL_DEVICE_NAME: %s\n", cBuffer);
	clGetDeviceInfo(cdDevice, CL_DRIVER_VERSION, sizeof(cBuffer), &cBuffer, NULL);
	//printf("CL_DRIVER_VERSION: %s\n\n", cBuffer);
	cl_context_properties properties[] = { CL_CONTEXT_PLATFORM, (uint64_t)cpPlatform, 0 };
	// Create a context to run OpenCL enabled GPU

	cl_context GPUContext = clCreateContextFromType(properties, CL_DEVICE_TYPE_GPU, NULL, NULL, NULL);

	// Create a command-queue on the GPU device
	cl_command_queue cqCommandQueue = clCreateCommandQueueWithProperties(GPUContext, cdDevice, 0, NULL);
	tuple<cl_platform_id, cl_device_id, cl_context_properties, cl_context, cl_command_queue > output = make_tuple(cpPlatform, cdDevice, *properties, GPUContext, cqCommandQueue);
	return output;
}

class Matrix
{
private:
	const uint64_t p = 9223372036854775783;
	const size_t cols;
	vector<uint64_t> matrix;
	tuple<cl_platform_id, cl_device_id, cl_context_properties, cl_context, cl_command_queue > stuff = initialize_openCL();

public:
	vector<uint64_t> X;

	Matrix(vector<uint64_t> X, size_t c) : cols(c) {
		this->X = X;
	}

	uint64_t* data() {
		return matrix.data();
	}

	uint64_t& at(size_t r, size_t c) {
		return matrix[c + r * cols];
	}

	uint64_t& operator()(size_t r, size_t c) {
		return matrix[c + r * cols];
	}

	int find_inv(size_t c, uint64_t n, uint64_t start_row) {
		uint64_t rows = n_rows();
		for (int i = start_row; i < rows; i++) {
			uint64_t element = this->operator()(i, c);
			uint64_t res = gcd(element, n);
			if (res == 1) {
				return i;
			}
		}
		return -1;
	}
	size_t n_cols() {
		return cols;
	}
	size_t n_rows() {
		return matrix.size() / cols;
	}
	void add_row(vector<uint64_t> row) {

		if (row.size() != cols) {
			throw std::invalid_argument("row.size() != cols");
		}

		for (unsigned int i = 0; i < cols; i++) {
			matrix.push_back(row[i]);
		}
	}
	void print_matrix() {
		for (int i = 0; i < n_rows(); i++) {
			for (int j = 0; j < n_cols(); j++) {
				cout << "\t" << at(i, j);
			}
			cout << "\n";
		}
		cout << endl;
	}
	size_t size() {
		return matrix.size();
	}

	vector<uint64_t> get_row(int i)
	{
		vector<uint64_t> temp = vector<uint64_t>(this->matrix.begin() + (i * this->n_cols()), this->matrix.begin() + ((i + 1) * (this->n_cols())));
		return temp;
	}

	void swap_rows(int i, int j) {

		vector<uint64_t> temp = get_row(j);
		for (int k = 0; k < this->n_cols(); k++) {
			this->matrix[k + j * this->n_cols()] = this->operator()(i, k);
			this->matrix[k + i * this->n_cols()] = temp[k];
		}
	}

	void reassign_row(vector<uint64_t> new_vector, int row)
	{
		for (int i = 0; i < cols; i++)
		{
			matrix[(row * cols) + i] = new_vector[i];
		}
	}

	int gaussian_elimination(uint64_t q) {
		int invertable_row_index;


		vector<uint64_t> temp;
		vector<uint64_t> temp1;
		uint64_t tempX;
		uint64_t tempX1;

		uint64_t xgcd;
		uint64_t xgcd1;


		uint64_t x_inv, y_inv;
		uint64_t x_inv1, y_inv1;

		for (int i = 0; i < cols; i++) {
			invertable_row_index = find_inv(i, q, i);
			if (invertable_row_index == -1 or invertable_row_index < i)
			{
				return -1;
			}

			temp = get_row(invertable_row_index);
			tempX = X[invertable_row_index];

			swap_rows(i, invertable_row_index);
			swap(X[i], X[invertable_row_index]);

			xgcd = gcdExtended(this->operator()(i, i), q, &x_inv, &y_inv);

			x_inv = (x_inv + q) % q;

			vector_mult(stuff, temp, x_inv);
			tempX = mult(tempX, x_inv);

			reassign_row(temp, i);
			X[i] = tempX;

			for (int j = i + 1; j < n_rows(); j++)
			{
				temp = get_row(i);
				tempX = X[i];

				xgcd1 = gcdExtended(this->operator()(i, i), q - 1, &x_inv1, &y_inv1);
				vector_mult(stuff, temp, x_inv1);
				tempX = mult(tempX, x_inv1);

				temp1 = get_row(j);
				tempX1 = X[j];
				vector_mult(stuff, temp, at(j, i));
				tempX = mult(tempX, at(j, i));

				vector_sub(stuff, temp1, temp);
				tempX1 = (tempX1 - tempX + q) % q;
				reassign_row(temp1, j);
				X[j] = tempX1;
			}
		}
		return 0;
	}

	vector<uint64_t> solve_system(uint64_t q)
	{
		//int x = gaussian_elimination(q);

		cout << "Po Gaussie " << endl;
		print_matrix();
		for (int i = 0; i < X.size(); i++)
			cout << X[i] << " ";
		cout << endl;

		vector<uint64_t> temp;
		vector<uint64_t> temp1;
		vector<uint64_t> temp2;

		uint64_t tempX;
		uint64_t tempX1;
		uint64_t tempX2;

		for (int i = cols - 1; i >= 0; i--)
		{
			temp = get_row(i);
			tempX = X[i];

			for (int j = i; j >= 1; j--)
			{
				temp1 = temp;
				tempX1 = tempX;
				temp2 = get_row(j-1);
				tempX2 = X[j - 1];

				vector_mult(stuff, temp1, at(j-1, i));
				tempX1 = mult(tempX1, X[j - 1]);
				vector_sub(stuff, temp2, temp1);
				tempX2 = (tempX2 - tempX1 + q) % q;

				reassign_row(temp2, j - 1);
				X[j - 1] = tempX2;
			}
		}
		return X;
	}


	void vector_mult(tuple<cl_platform_id, cl_device_id, cl_context_properties, cl_context, cl_command_queue > stuff, vector<uint64_t>& input_vector, uint64_t a)
	{
		std::string S = kernel_load("VectorMult.cl");
		// Two integer source vectors in Host memory
		std::vector<uint64_t> HostVector1;
		std::vector<uint64_t> HostVector2;
		//Output Vector
		std::vector<uint64_t> HostOutputVector;

		cl_int err;

		HostVector1.resize(input_vector.size());
		HostVector2.resize(input_vector.size());
		HostOutputVector.resize(input_vector.size());
		uint64_t size = input_vector.size();

		HostVector2.assign(size, a);
		HostOutputVector.assign(size, 0);

		for (int c = 0; c < size; c++)
		{
			HostVector1[c] = input_vector[c];
		}
		
		cl_mem GPUVector1 = clCreateBuffer(get<3>(stuff), CL_MEM_READ_ONLY |
			CL_MEM_COPY_HOST_PTR, sizeof(uint64_t) * size, HostVector1.data(), NULL);

		cl_mem GPUVector2 = clCreateBuffer(get<3>(stuff), CL_MEM_READ_ONLY |
			CL_MEM_COPY_HOST_PTR, sizeof(uint64_t) * size, HostVector2.data(), NULL);

		// Allocate output memory on GPU
		cl_mem GPUOutputVector = clCreateBuffer(get<3>(stuff), CL_MEM_WRITE_ONLY,
			sizeof(uint64_t) * size, NULL, NULL);

		const char* cstr = S.c_str();

		// Create OpenCL program with source code
		cl_program OpenCLProgram = clCreateProgramWithSource(get<3>(stuff), 1, &cstr, NULL, NULL);

		// Build the program (OpenCL JIT compilation)
		err = clBuildProgram(OpenCLProgram, 0, NULL, NULL, NULL, NULL);
		if (err != CL_SUCCESS)
		{
			printf("Nie dziala\n");
			char str[8192];
			size_t len;
			clGetProgramBuildInfo(OpenCLProgram, get<1>(stuff), CL_PROGRAM_BUILD_LOG, 8192, str, &len);
			if (len > 8192)
			{
				printf("Nie miesci sie\n");
			}
			printf("%s", str);
		}
		// Create a handle to the compiled OpenCL function (Kernel)
		cl_kernel OpenCLVectorAdd = clCreateKernel(OpenCLProgram, "VectorMult", NULL);

		// In the next step we associate the GPU memory with the Kernel arguments

		clSetKernelArg(OpenCLVectorAdd, 0, sizeof(cl_mem), (void*)&GPUOutputVector);
		clSetKernelArg(OpenCLVectorAdd, 1, sizeof(cl_mem), (void*)&GPUVector1);
		clSetKernelArg(OpenCLVectorAdd, 2, sizeof(cl_mem), (void*)&GPUVector2);

		// Launch the Kernel on the GPU

		// This kernel only uses global data

		size_t WorkSize[1] = { size }; // one dimensional Range

		clEnqueueNDRangeKernel(get<4>(stuff), OpenCLVectorAdd, 1, NULL,
			WorkSize, NULL, 0, NULL, NULL);

		// Copy the output in GPU memory back to CPU memory
		clEnqueueReadBuffer(get<4>(stuff), GPUOutputVector, CL_TRUE, 0,
			size * sizeof(uint64_t), HostOutputVector.data(), 0, NULL, NULL);

		for (int i = 0; i < size; i++)
			input_vector[i] = HostOutputVector[i];

		// Cleanup
		clReleaseKernel(OpenCLVectorAdd);
		clReleaseProgram(OpenCLProgram);
		clReleaseMemObject(GPUVector1);
		clReleaseMemObject(GPUVector2);
		clReleaseMemObject(GPUOutputVector);

	}


	void vector_sub(tuple<cl_platform_id, cl_device_id, cl_context_properties, cl_context, cl_command_queue > stuff, vector<uint64_t>& input_vector1, vector<uint64_t>& input_vector2)
	{
		std::string S = kernel_load("VectorAdd.cl");

		// Two integer source vectors in Host memory
		std::vector<uint64_t> HostVector1;
		std::vector<uint64_t> HostVector2;
		//Output Vector
		std::vector<uint64_t> HostOutputVector;

		HostVector1.resize(input_vector1.size());
		HostVector2.resize(input_vector2.size());
		HostOutputVector.resize(input_vector1.size());
		uint64_t size = input_vector1.size();

		
		HostOutputVector.assign(size, 0);

		// Initialize with some interesting repeating data
		for (int c = 0; c < size; c++)
		{
			HostVector1[c] = input_vector1[c];
			HostVector2[c] = input_vector2[c];
		}

		//Get an OpenCL platform

		// Allocate GPU memory for source vectors AND initialize from CPU memory

		cl_mem GPUVector1 = clCreateBuffer(get<3>(stuff), CL_MEM_READ_ONLY |
			CL_MEM_COPY_HOST_PTR, sizeof(uint64_t) * size, HostVector1.data(), NULL);

		cl_mem GPUVector2 = clCreateBuffer(get<3>(stuff), CL_MEM_READ_ONLY |
			CL_MEM_COPY_HOST_PTR, sizeof(uint64_t) * size, HostVector2.data(), NULL);

		// Allocate output memory on GPU
		cl_mem GPUOutputVector = clCreateBuffer(get<3>(stuff), CL_MEM_WRITE_ONLY,
			sizeof(uint64_t) * size, NULL, NULL);

		const char* cstr = S.c_str();

		// Create OpenCL program with source code
		cl_program OpenCLProgram = clCreateProgramWithSource(get<3>(stuff), 1, &cstr, NULL, NULL);

		// Build the program (OpenCL JIT compilation)
		clBuildProgram(OpenCLProgram, 0, NULL, NULL, NULL, NULL);

		// Create a handle to the compiled OpenCL function (Kernel)
		cl_kernel OpenCLVectorAdd = clCreateKernel(OpenCLProgram, "VectorAdd", NULL);

		// In the next step we associate the GPU memory with the Kernel arguments

		clSetKernelArg(OpenCLVectorAdd, 0, sizeof(cl_mem), (void*)&GPUOutputVector);
		clSetKernelArg(OpenCLVectorAdd, 1, sizeof(cl_mem), (void*)&GPUVector1);
		clSetKernelArg(OpenCLVectorAdd, 2, sizeof(cl_mem), (void*)&GPUVector2);

		// Launch the Kernel on the GPU

		// This kernel only uses global data

		size_t WorkSize[1] = { size }; // one dimensional Range

		clEnqueueNDRangeKernel(get<4>(stuff), OpenCLVectorAdd, 1, NULL,
			WorkSize, NULL, 0, NULL, NULL);

		// Copy the output in GPU memory back to CPU memory
		clEnqueueReadBuffer(get<4>(stuff), GPUOutputVector, CL_FALSE, 0,
			size * sizeof(uint64_t), HostOutputVector.data(), 0, NULL, NULL);

		for (int i = 0; i < size; i++)
			input_vector1[i] = HostOutputVector[i];

		// Cleanup
		clReleaseKernel(OpenCLVectorAdd);
		clReleaseProgram(OpenCLProgram);
		clReleaseMemObject(GPUVector1);
		clReleaseMemObject(GPUVector2);
		clReleaseMemObject(GPUOutputVector);
	}

	
};
