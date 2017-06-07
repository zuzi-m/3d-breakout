#pragma once
#include "MathCommon.h"
#include "Vector.h"
#include "Point.h"

#include <iostream>

namespace MathLib {

	class Matrix
	{
	public:
		Matrix();
		Matrix(Matrix&);
		Matrix(double);
		Matrix(Vector& diagonal);
		~Matrix();
		static Matrix diag(const double);
		static Matrix fromVectors(const Vector&, const Vector&);
		static Matrix fromVectors(const Vector&, const Vector&, const Vector&);
		static Matrix rotation(int axis, double rads);

		double det() const;

		Matrix add(const Matrix&) const;
		Matrix sub(const Matrix&) const;
		Matrix minus() const;
		Matrix mult(const Matrix&) const;
		Matrix mult(const double) const;
		Vector mult(const Vector&) const;
		Point mult(const Point&) const;

		Matrix transpose() const;
		Matrix inverse() const;
		Vector getDiagonal() const;

		class _MatrixIndexer
		{
		public:
			_MatrixIndexer(Matrix* m, const int index1) { this->m = m; this->index1 = index1; };
			double& operator[](const int index) { return this->m->data[this->index1][index]; };
		private:
			int index1;
			Matrix* m;
		};
		_MatrixIndexer operator[](const int index) { return _MatrixIndexer(this, index); }
		friend std::ostream& operator<<(std::ostream& os, const Matrix& m);
	private:
		double data[MATRIX_DIM][MATRIX_DIM];

		void setAll(double);
	};
}
