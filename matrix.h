#pragma once
#include <vector>
#include <stdexcept>
#include <iostream>
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
	int gcd = gcdExtended(b % a, a, &x1, &y1);

	*x = y1 - (b / a) * x1;
	*y = x1;

	return gcd;
}

uint64_t gcd(uint64_t a, uint64_t b) {
	uint64_t x, y;
	return  gcdExtended(a, b, &x, &y);
}

class Matrix
{
private:
	const size_t cols;
	std::vector<uint64_t> matrix;
public:
	

	Matrix(size_t c): cols(c) {
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
	
	int find_inv(size_t c, uint64_t n) {
		uint64_t rows = n_rows();
		
		for (int i = 0; i < rows; i++) {
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
	void add_row(std::vector<uint64_t> row) {
		
		if (row.size() != cols) {
			throw std::invalid_argument("row.size() != cols");
		}

		for (int i = 0; i < cols; i++) {
			matrix.push_back(row[i]);
		}
	}
	void print_matrix() {
		for (int i = 0; i < n_rows(); i++) {
			for (int j = 0; j < n_cols(); j++) {
				std::cout << "\t" << at(i, j);

			}
			std::cout << "\n";
		}

	}
	size_t size() {
		return matrix.size();
	}

	vector<uint64_t> slicing(vector<uint64_t>& arr,
		uint64_t X, uint64_t Y)
	{

		// Starting and Ending iterators
		auto start = arr.begin() + X;
		auto end = arr.begin() + Y + 1;

		// To store the sliced vector
		vector<uint64_t> result(Y - X + 1);

		// Copy vector using copy function()
		copy(start, end, result.begin());

		// Return the final sliced vector
		return result;
	}

	vector<uint64_t> get_row(uint64_t i)
	{
		vector<uint64_t> temp = vector<uint64_t>(this->matrix.begin() + (i * this->n_cols()), this->matrix.begin() + ((i + 1) * (this->n_cols())));
		return temp;
	}

	void swap_rows(uint64_t i, uint64_t j) {

		vector<uint64_t> temp = get_row(j);
		for (int k = 0; k < this->n_cols(); k++) {
			this->matrix[k + j * this->n_cols()] = this->operator()(i, k);
			this->matrix[k + i * this->n_cols()] = temp[k];
		}
	}
};
