#pragma once
#include "Common.h"
#include "Vector.h"

namespace MathLib {

	class Matrix
	{
	public:
		Matrix();
		~Matrix();
		static Matrix zero();
		static Matrix diag(double);
		static Matrix all(double);

		Matrix mult(Matrix&);
		Vector mult(Vector&);
	private:
		double data[MATRIX_DIM][MATRIX_DIM];

		void setAll(double);
	};

}
