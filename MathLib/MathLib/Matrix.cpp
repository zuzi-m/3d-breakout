#include "Matrix.h"

namespace MathLib {

	Matrix::Matrix()
	{
		setAll(0);
	}


	Matrix::~Matrix()
	{
	}

	Matrix Matrix::zero()
	{
		return Matrix();
	}

	Matrix Matrix::diag(double x)
	{
		Matrix m;
		for (int i = 0; i < MATRIX_DIM; i++) {
			m.data[i][i] = x;
		}
		return m;
	}

	Matrix Matrix::all(double x)
	{
		Matrix m;
		m.setAll(x);
	}

	Matrix Matrix::mult(Matrix &other)
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

	Vector Matrix::mult(Vector &vec)
	{
		double u, v, w;
		for (int i = 0; i < MATRIX_DIM; i++) {
			u = data[0][i];
			v = data[1][i];
			w = data[2][i];
		}
		Vector result(u, v, w);
		return result;
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
