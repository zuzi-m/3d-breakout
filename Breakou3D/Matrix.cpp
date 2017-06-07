#include "Matrix.h"

namespace MathLib {

	Matrix::Matrix()
	{
		setAll(0);
	}

	Matrix::Matrix(Matrix &other)
	{
		for (int i = 0; i < MATRIX_DIM; i++) {
			for (int j = 0; j < MATRIX_DIM; j++) {
				data[i][j] = other.data[i][j];
			}
		}
	}

	Matrix::Matrix(double x)
	{
		setAll(x);
	}

	Matrix::Matrix(Vector &diagonal)
	{
		setAll(0.0);
		data[0][0] = diagonal.U();
		data[1][1] = diagonal.V();
		data[2][2] = diagonal.W();
	}


	Matrix::~Matrix()
	{
	}

	Matrix Matrix::diag(double x)
	{
		Matrix m(0);
		for (int i = 0; i < MATRIX_DIM; i++) {
			m.data[i][i] = x;
		}
		return m;
	}

	Matrix Matrix::fromVectors(const Vector &colVector, const Vector &rowVector)
	{
		Matrix result(0);
		double col[MATRIX_DIM];
		double row[MATRIX_DIM];
		for (int i = 0; i < MATRIX_DIM; i++) {
			for (int j = 0; j < MATRIX_DIM; j++) {
				result.data[i][j] = col[j] * row[i];
			}
		}
		return result;
	}

	Matrix Matrix::fromVectors(const Vector &row1, const Vector &row2, const Vector &row3)
	{
		Matrix m(0.0);
		for (int i = 0; i < MATRIX_DIM; i++) {
			m.data[i][0] = row1[i];
			m.data[i][1] = row2[i];
			m.data[i][2] = row3[i];
		}
		return m;
	}

	Matrix Matrix::rotation(int axis, double rads)
	{
		double c = cos(rads);
		double s = sin(rads);

		Matrix result(0);

		switch (axis % MATRIX_DIM) {
		case 0:
			result[0][0] = 1;
			result[1][1] = c;
			result[1][2] = -s;
			result[2][1] = s;
			result[2][2] = c;
			break;
		case 1:
			result[1][1] = 1;
			result[0][0] = c;
			result[0][2] = s;
			result[2][0] = -s;
			result[2][2] = c;
			break;
		case 2:
			result[2][2] = 1;
			result[0][0] = c;
			result[0][1] = -s;
			result[1][0] = s;
			result[1][1] = c;
			break;
		}

		return result;
	}

	double Matrix::det() const
	{
		return (data[0][0] * data[1][1] * data[2][2])
			+ (data[0][1] * data[1][2] * data[2][0])
			+ (data[0][2] * data[1][0] * data[2][1])
			+ (data[0][2] * data[1][1] * data[2][0])
			+ (data[0][1] * data[1][0] * data[2][2])
			+ (data[0][0] * data[1][2] * data[2][1]);
	}

	Matrix Matrix::add(const Matrix &other) const
	{
		Matrix result;
		for (int i = 0; i < MATRIX_DIM; i++) {
			for (int j = 0; j < MATRIX_DIM; j++) {
				result.data[i][j] = data[i][j] + other.data[i][j];
			}
		}
		return result;
	}

	Matrix Matrix::sub(const Matrix &other) const
	{
		return this->add(other.minus());
	}

	Matrix Matrix::minus() const
	{
		Matrix result;
		for (int i = 0; i < MATRIX_DIM; i++) {
			for (int j = 0; j < MATRIX_DIM; j++) {
				result.data[i][j] = -data[i][j];
			}
		}
		return result;
	}

	Matrix Matrix::mult(const Matrix &other) const
	{
		Matrix result;
		for (int i = 0; i < MATRIX_DIM; i++) {
			for (int j = 0; j < MATRIX_DIM; j++) {
				double sum = 0;
				for (int k = 0; k < MATRIX_DIM; k++) {
					sum += data[i][k] * other.data[k][j];
				}
				result.data[i][j] = sum;
			}
		}
		return result;
	}

	Matrix Matrix::mult(const double x) const
	{
		Matrix result;
		for (int i = 0; i < MATRIX_DIM; i++) {
			for (int j = 0; j < MATRIX_DIM; j++) {
				result.data[i][j] = data[i][j] * x;
			}
		}
		return result;
	}

	Vector Matrix::mult(const Vector &vec) const
	{
		double u = 0, v = 0, w = 0;
		for (int i = 0; i < MATRIX_DIM; i++) {
			u += data[0][i] * vec[i];
			v += data[1][i] * vec[i];
			w += data[2][i] * vec[i];
		}
		return Vector(u, v, w);
	}

	Point Matrix::mult(const Point &p) const
	{
		double u = 0, v = 0, w = 0;
		for (int i = 0; i < MATRIX_DIM; i++) {
			u += data[0][i] * p[i];
			v += data[1][i] * p[i];
			w += data[2][i] * p[i];
		}
		return Point(u, v, w);
	}

	Matrix Matrix::transpose() const
	{
		Matrix result;
		for (int i = 0; i < MATRIX_DIM; i++) {
			for (int j = 0; j < MATRIX_DIM; j++) {
				result.data[i][j] = data[j][i];
			}
		}
		return result;
	}

	void getMinorIndices(int row, int col, int* above, int* below, int* left, int* right) {
		for (int i = 0; i < MATRIX_DIM; i++) {
			for (int j = 0; j < MATRIX_DIM; j++) {
				if (i != row && j != col) {
					if (*above == -1) {
						*above = i;
					}
					else {
						*below = i;
					}
					if (*left == -1) {
						*left = j;
					}
					else {
						*right = j;
					}
				}
			}
		}
	}

	Matrix Matrix::inverse() const
	{
		double d = det();
		if (d == 0.0) {
			return Matrix(0);
		}
		// get the matrix of minors
		Matrix conjugates(0);
		for (int i = 0; i < MATRIX_DIM; i++) {
			for (int j = 0; j < MATRIX_DIM; j++) {
				int above = -1, below = -1, left = -1, right = -1;
				getMinorIndices(i, j, &above, &below, &left, &right);
				double ad = data[above][left] * data[below][right];
				double bc = data[above][right] * data[below][left];
				conjugates[i][j] = ad - bc;
				if (i % 2 == 0) {
					if (j % 2 == 1) {
						conjugates[i][j] = -conjugates[i][j];
					}
				} else {
					if (j % 2 == 0) {
						conjugates[i][j] = -conjugates[i][j];
					}
				}
			}
		}
		// transpose it and multiply it by 1/determinant
		return conjugates.transpose().mult(1.0 / d);
	}

	Vector Matrix::getDiagonal() const
	{
		return Vector(data[0][0], data[1][1], data[2][2]);
	}

	std::ostream & operator<<(std::ostream & os, const Matrix& m)
	{
		for (int i = 0; i < MATRIX_DIM; i++) {
			for (int j = 0; j < MATRIX_DIM; j++) {
				if (j != 0) {
					os << " ";
				}
				os << m.data[i][j];
				if (j == MATRIX_DIM - 1) {
					os << std::endl;
				}
			}
		}
		return os;
	}

	void Matrix::setAll(double x)
	{
		for (int i = 0; i < MATRIX_DIM; i++) {
			for (int j = 0; j < MATRIX_DIM; j++) {
				data[i][j] = x;
			}
		}
	}

}
