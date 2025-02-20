#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "Utils.hpp"


class Matrix {
public:
	// Default constructor
	Matrix() : rows(0), cols(0) { 
		data.resize(rows, std::vector<double>(cols));
	}

	// Constructor
	Matrix(size_t numRows, size_t numCols, double initVal = 0.0) : rows(numRows), cols(numCols)
	{
		data.resize(rows, std::vector<double>(cols, initVal)); // initialize matrix of vals
	}

	// Resize
	void resize(size_t newRows, size_t newCols, double initVal = 0.0) {
		std::vector<std::vector<double>> newData(newRows, std::vector<double>(newCols, initVal));

		// copy existing data to the new data structure
		for (size_t i = 0; i < std::min(rows, newRows); i++) {
			for (size_t j = 0; j < std::min(cols, newCols); j++) {
				newData[i][j] = data[i][j];
			}
		}

		// update the data and dimensions
		data = std::move(newData);
		rows = newRows;
		cols = newCols;
	}

	// Accessors
	// used to modify the matrix
	std::vector<double>& operator[](size_t index) {
		return data[index];
	}

	// used to access but not modify, with the same code as the modifying function
	// note that the first const indicates the return matrix's structure cannot be modified,
	// although the elements inside can be (because they are not const)
	// also, the const at the end shows that this function will not change member vars of the class
	const std::vector<double>& operator[](size_t index) const {
		return data[index];
	}

	size_t numRows() const noexcept {
		return rows;
	}

	size_t numCols() const noexcept {
		return cols;
	}

	void setColumn(size_t colIdx, const std::vector<double>& colData) {
		if (colIdx >= cols) {
			throw std::invalid_argument("Invalid column index.");
		}

		else if (colData.size() != rows) {
			throw std::invalid_argument("Invalid column data size.");
		}

		for (size_t i = 0; i < rows; i++) {
			data[i][colIdx] = colData[i];
		}
	}

	Matrix getColumn(size_t colIdx) const {
		if (colIdx >= cols) {
			throw std::out_of_range("Invalid col index.");
		}

		Matrix result(rows, 1);
		for (size_t i = 0; i < rows; ++i) {
			result.data[i][0] = data[i][colIdx];
		}
		return result;
	}

	const std::vector<double>& getRow(size_t rowIdx) const {
		if (rowIdx >= rows) {
			throw std::out_of_range("Invalid row index.");
		}

		return data[rowIdx];
	}

	void shape() const noexcept {
		std::cout << rows << " x " << cols << std::endl;
	}

	std::pair<size_t, size_t> size() const noexcept {
		return {rows, cols};
	}

	void print() const {
		for (size_t i = 0; i < rows; ++i) {
			for (size_t j = 0; j < cols; ++j) {
				std::cout << data[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}

	// operations
	// elementwise addition
	Matrix operator+(const Matrix& other) const {
		if (size() != other.size()) {
			throw std::runtime_error("Matrix dimensions do not match for addition.");
		}

		Matrix result(rows, cols);

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				result.data[i][j] = data[i][j] + other.data[i][j];
			}
		}
		return result;
	}

	// elementwise subtraction
	Matrix operator-(const Matrix& other) const {
		if (size() != other.size()) {
			throw std::runtime_error("Matrix dimensions do not match for subtraction.");
		}

		Matrix result(rows, cols);

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				result.data[i][j] = data[i][j] - other.data[i][j];
			}
		}
		return result;
	}

	// scalar addition 
	Matrix operator+(const double scalar) const {
		Matrix result(rows, cols);

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				result.data[i][j] = data[i][j] + scalar;
			}
		}
		return result;
	}


	// scalar subtraction
	Matrix operator-(const double scalar) const {
		Matrix result(rows, cols);

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				result.data[i][j] = data[i][j] - scalar;
			}
		}
		return result;
	}

	// matrix multiplication
	Matrix operator*(const Matrix& other) const {
		if (cols != other.numRows()) {
			throw std::runtime_error("Matrix dimensions do not match for multiplication.");
		}

		Matrix result(rows, other.numCols());

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < other.numCols(); j++) {
				double sum = 0.0;
				for (int k = 0; k < cols; k++) {
					sum += data[i][k] * other.data[k][j];
				}
				result.data[i][j] = sum;
			}
		}
		return result;
	}

	// elementwise multiplication
	Matrix elementwiseMult(const Matrix& other) const {
		if (size() != other.size()) {
			this->shape();
			other.shape();
			throw std::runtime_error("Matrix dimensions do not match for elementwise multiplication.");
		}

		Matrix result(rows, cols);

		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < cols; j++) {
				result.data[i][j] = data[i][j] * other.data[i][j];
			}
		}
		return result;
	}

	// scalar multiplication
	Matrix operator*(const double scalar) const {
		Matrix result(rows, cols);

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				result.data[i][j] = data[i][j] * scalar;
			}
		}
		return result;
	}

	// transposition
	Matrix T() const {
		Matrix result(cols, rows);

		// reverse indices 
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				result.data[j][i] = data[i][j];
			}
		}
		return result;
	}

	// converting from vector to 1-dimensional matrix
	static Matrix toMatrix(const std::vector<int>& vec) noexcept{
		Matrix result(vec.size(), 1);

		for (size_t i = 0; i < vec.size(); i++) {
			result.data[i][0] = vec[i];
		}
		return result;
	}

	// flatten matrix to column vector
	Matrix flatten() const {
		Matrix result(numRows() * numCols(), 1);
		for (size_t i = 0; i < numRows(); i++) {
			for (size_t j = 0; j < numCols(); j++) {
				result.data[i * numCols() + j][0] = data[i][j];
			}
		}
		return result;
	}

private:
	size_t rows;
	size_t cols;

	std::vector<std::vector<double>> data;
};