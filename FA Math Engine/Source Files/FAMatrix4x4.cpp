#include "FAMatrix4x4.h"
#include "FAVector4D.h"

namespace FAMath
{
	//-------------------------------------------------------------------------------------
	Matrix4x4::Matrix4x4()
	{
		//1st row
		m_mat[0][0] = 1.0f;
		m_mat[0][1] = 0.0f;
		m_mat[0][2] = 0.0f;
		m_mat[0][3] = 0.0f;

		//2nd
		m_mat[1][0] = 0.0f;
		m_mat[1][1] = 1.0f;
		m_mat[1][2] = 0.0f;
		m_mat[1][3] = 0.0f;

		//3rd row
		m_mat[2][0] = 0.0f;
		m_mat[2][1] = 0.0f;
		m_mat[2][2] = 1.0f;
		m_mat[2][3] = 0.0f;

		//4th row
		m_mat[3][0] = 0.0f;
		m_mat[3][1] = 0.0f;
		m_mat[3][2] = 0.0f;
		m_mat[3][3] = 1.0f;
	}



	Matrix4x4::Matrix4x4(float a[][4])
	{
		//1st row
		m_mat[0][0] = a[0][0];
		m_mat[0][1] = a[0][1];
		m_mat[0][2] = a[0][2];
		m_mat[0][3] = a[0][3];

		//2nd
		m_mat[1][0] = a[1][0];
		m_mat[1][1] = a[1][1];
		m_mat[1][2] = a[1][2];
		m_mat[1][3] = a[1][3];

		//3rd row
		m_mat[2][0] = a[2][0];
		m_mat[2][1] = a[2][1];
		m_mat[2][2] = a[2][2];
		m_mat[2][3] = a[2][3];

		//4th row
		m_mat[3][0] = a[3][0];
		m_mat[3][1] = a[3][1];
		m_mat[3][2] = a[3][2];
		m_mat[3][3] = a[3][3];
	}

	float* Matrix4x4::data()
	{
		return m_mat[0];
	}

	const float* Matrix4x4::data() const
	{
		return m_mat[0];
	}

	const float& Matrix4x4::operator()(unsigned int row, unsigned int col) const
	{
		if (row > 3 || col > 3)
		{
			return m_mat[0][0];
		}
		else
		{
			return m_mat[row][col];
		}
	}

	float& Matrix4x4::operator()(unsigned int row, unsigned int col)
	{
		if (row > 3 || col > 3)
		{
			return m_mat[0][0];
		}
		else
		{
			return m_mat[row][col];
		}
	}

	void Matrix4x4::setRow(unsigned int row, Vector4D v)
	{
		if (row < 0 || row > 3)
		{
			m_mat[0][0] = v.x();
			m_mat[0][1] = v.y();
			m_mat[0][2] = v.z();
			m_mat[0][3] = v.w();
		}
		else
		{
			m_mat[row][0] = v.x();
			m_mat[row][1] = v.y();
			m_mat[row][2] = v.z();
			m_mat[row][3] = v.w();
		}
	}

	void Matrix4x4::setCol(unsigned int col, Vector4D v)
	{
		if (col < 0 || col > 3)
		{
			m_mat[0][0] = v.x();
			m_mat[1][0] = v.y();
			m_mat[2][0] = v.z();
			m_mat[3][0] = v.w();
		}
		else
		{
			m_mat[0][col] = v.x();
			m_mat[1][col] = v.y();
			m_mat[2][col] = v.z();
			m_mat[3][col] = v.w();
		}
	}

	Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& m)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				this->m_mat[i][j] += (double)m.m_mat[i][j];
			}
		}

		return *this;
	}

	Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& m)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				this->m_mat[i][j] -= (double)m.m_mat[i][j];
			}
		}

		return *this;
	}

	Matrix4x4& Matrix4x4::operator*=(const float& k)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				this->m_mat[i][j] *= (double)k;
			}
		}

		return *this;
	}

	Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& m)
	{
		Matrix4x4 res;

		for (int i = 0; i < 4; ++i)
		{
			res.m_mat[i][0] = ((double)m_mat[i][0] * m.m_mat[0][0]) +
				((double)m_mat[i][1] * m.m_mat[1][0]) +
				((double)m_mat[i][2] * m.m_mat[2][0]) +
				((double)m_mat[i][3] * m.m_mat[3][0]);

			res.m_mat[i][1] = ((double)m_mat[i][0] * m.m_mat[0][1]) +
				((double)m_mat[i][1] * m.m_mat[1][1]) +
				((double)m_mat[i][2] * m.m_mat[2][1]) +
				((double)m_mat[i][3] * m.m_mat[3][1]);

			res.m_mat[i][2] = ((double)m_mat[i][0] * m.m_mat[0][2]) +
				((double)m_mat[i][1] * m.m_mat[1][2]) +
				((double)m_mat[i][2] * m.m_mat[2][2]) +
				((double)m_mat[i][3] * m.m_mat[3][2]);

			res.m_mat[i][3] = ((double)m_mat[i][0] * m.m_mat[0][3]) +
				((double)m_mat[i][1] * m.m_mat[1][3]) +
				((double)m_mat[i][2] * m.m_mat[2][3]) +
				((double)m_mat[i][3] * m.m_mat[3][3]);
		}

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				m_mat[i][j] = res.m_mat[i][j];
			}
		}

		return *this;
	}

	Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2)
	{
		Matrix4x4 res;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				res(i, j) = (double)m1(i, j) + m2(i, j);
			}
		}

		return res;
	}

	Matrix4x4 operator-(const Matrix4x4& m)
	{
		Matrix4x4 res;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				res(i, j) = -m(i, j);
			}
		}

		return res;
	}

	Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2)
	{
		Matrix4x4 res;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				res(i, j) = (double)m1(i, j) - m2(i, j);
			}
		}

		return res;
	}

	Matrix4x4 operator*(const Matrix4x4& m, const float& k)
	{
		Matrix4x4 res;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				res(i, j) = (double)m(i, j) * k;
			}
		}

		return res;
	}

	Matrix4x4 operator*(const float& k, const Matrix4x4& m)
	{
		Matrix4x4 res;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				res(i, j) = k * (double)m(i, j);
			}
		}

		return res;
	}

	Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2)
	{
		Matrix4x4 res;

		for (int i = 0; i < 4; ++i)
		{
			res(i, 0) = ((double)m1(i, 0) * m2(0, 0)) +
				((double)m1(i, 1) * m2(1, 0)) +
				((double)m1(i, 2) * m2(2, 0)) +
				((double)m1(i, 3) * m2(3, 0));

			res(i, 1) = ((double)m1(i, 0) * m2(0, 1)) +
				((double)m1(i, 1) * m2(1, 1)) +
				((double)m1(i, 2) * m2(2, 1)) +
				((double)m1(i, 3) * m2(3, 1));

			res(i, 2) = ((double)m1(i, 0) * m2(0, 2)) +
				((double)m1(i, 1) * m2(1, 2)) +
				((double)m1(i, 2) * m2(2, 2)) +
				((double)m1(i, 3) * m2(3, 2));

			res(i, 3) = ((double)m1(i, 0) * m2(0, 3)) +
				((double)m1(i, 1) * m2(1, 3)) +
				((double)m1(i, 2) * m2(2, 3)) +
				((double)m1(i, 3) * m2(3, 3));
		}

		return res;
	}

	Vector4D operator*(const Matrix4x4& m, const Vector4D& v)
	{
		Vector4D res;

		res.x() = ((double)m(0, 0) * v.x() + (double)m(0, 1) * v.y() + (double)m(0, 2) * v.z() + (double)m(0, 3) * v.w());
		res.y() = ((double)m(1, 0) * v.x() + (double)m(1, 1) * v.y() + (double)m(1, 2) * v.z() + (double)m(1, 3) * v.w());
		res.z() = ((double)m(2, 0) * v.x() + (double)m(2, 1) * v.y() + (double)m(2, 2) * v.z() + (double)m(2, 3) * v.w());
		res.w() = ((double)m(3, 0) * v.x() + (double)m(3, 1) * v.y() + (double)m(3, 2) * v.z() + (double)m(3, 3) * v.w());

		return res;
	}

	Vector4D operator*(const Vector4D& v, const Matrix4x4& m)
	{
		Vector4D res;

		res.x() = ((double)v.x() * m(0, 0) + (double)v.y() * m(1, 0) + (double)v.z() * m(2, 0) + (double)v.w() * m(3, 0));
		res.y() = ((double)v.x() * m(0, 1) + (double)v.y() * m(1, 1) + (double)v.z() * m(2, 1) + (double)v.w() * m(3, 1));
		res.z() = ((double)v.x() * m(0, 2) + (double)v.y() * m(1, 2) + (double)v.z() * m(2, 2) + (double)v.w() * m(3, 2));
		res.w() = ((double)v.x() * m(0, 3) + (double)v.y() * m(1, 3) + (double)v.z() * m(2, 3) + (double)v.w() * m(3, 3));

		return res;
	}

	//set to identity matrix by setting the diagonals to 1.0f and all other elements to 0.0f
	void setToIdentity(Matrix4x4& m)
	{
		//1st row
		m(0, 0) = 1.0f;
		m(0, 1) = 0.0f;
		m(0, 2) = 0.0f;
		m(0, 3) = 0.0f;

		//2nd row
		m(1, 0) = 0.0f;
		m(1, 1) = 1.0f;
		m(1, 2) = 0.0f;
		m(1, 3) = 0.0f;

		//3rd row
		m(2, 0) = 0.0f;
		m(2, 1) = 0.0f;
		m(2, 2) = 1.0f;
		m(2, 3) = 0.0f;

		//4th row
		m(3, 0) = 0.0f;
		m(3, 1) = 0.0f;
		m(3, 2) = 0.0f;
		m(3, 3) = 1.0f;
	}

	//Is the identity matrix if the diagonals are equal to 1.0f and all other elements equals to 0.0f
	bool isIdentity(const Matrix4x4& m)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				if (i == j)
				{
					if (!compareFloats(m(i, j), 1.0f, EPSILON))
					{
						return false;
					}
				}
				else
				{
					if (!compareFloats(m(i, j), 0.0f, EPSILON))
					{
						return false;
					}
				}
			}
		}
	}

	//make the rows into cols
	Matrix4x4 transpose(const Matrix4x4& m)
	{
		Matrix4x4 res;

		//1st col = 1st row
		res(0, 0) = m(0, 0);
		res(1, 0) = m(0, 1);
		res(2, 0) = m(0, 2);
		res(3, 0) = m(0, 3);

		//2nd col = 2nd row
		res(0, 1) = m(1, 0);
		res(1, 1) = m(1, 1);
		res(2, 1) = m(1, 2);
		res(3, 1) = m(1, 3);

		//3rd col = 3rd row
		res(0, 2) = m(2, 0);
		res(1, 2) = m(2, 1);
		res(2, 2) = m(2, 2);
		res(3, 2) = m(2, 3);

		//4th col = 4th row
		res(0, 3) = m(3, 0);
		res(1, 3) = m(3, 1);
		res(2, 3) = m(3, 2);
		res(3, 3) = m(3, 3);

		return res;
	}

	//1 0 0 0
	//0 1 0 0
	//0 0 1 0
	//x y z 1
	Matrix4x4 translate(const Matrix4x4& cm, float x, float y, float z)
	{
		Matrix4x4 t;
		t(3, 0) = x;
		t(3, 1) = y;
		t(3, 2) = z;

		return cm * t;
	}

	//x 0 0 0
	//0 y 0 0
	//0 0 z 0
	//0 0 0 1
	Matrix4x4 scale(const Matrix4x4& cm, float x, float y, float z)
	{
		Matrix4x4 s;
		s(0, 0) = x;
		s(1, 1) = y;
		s(2, 2) = z;

		return cm * s;
	}

	//c + (1 - c)x^2	(1 - c)xy + sz	(1 - c)xz - sy	0
	//(1 - c)xy - sz	c + (1 - c)y^2	(1 - c)yz + sx	0
	//(1 - c)xz + sy	(1 - c)yz - sx	c + (1 - c)z^2	0
	//0					0				0				1
	//c = cos(angle)
	//s = sin(angle)
	Matrix4x4 rotate(const Matrix4x4& cm, float angle, float x, float y, float z)
	{
		double c = cos(angle * PI / 180.0);
		double s = sin(angle * PI / 180.0);

		Matrix4x4 r;

		//1st row
		r(0, 0) = c + (1.0 - c) * ((double)x * x);
		r(0, 1) = (1.0 - c) * ((double)x * y) + (s * z);
		r(0, 2) = (1.0 - c) * ((double)x * z) - (s * y);

		//2nd row
		r(1, 0) = (1.0 - c) * ((double)x * y) - (s * z);
		r(1, 1) = c + (1.0 - c) * ((double)y * y);
		r(1, 2) = (1.0 - c) * ((double)y * z) + (s * x);

		//3rd row
		r(2, 0) = (1.0 - c) * ((double)x * z) + (s * y);
		r(2, 1) = (1.0 - c) * ((double)y * z) - (s * x);
		r(2, 2) = c + (1.0 - c) * ((double)z * z);

		return cm * r;
	}

	double det(const Matrix4x4& m)
	{
		//m00m11(m22m33 - m23m32)
		double c1 = (double)m(0, 0) * m(1, 1) * m(2, 2) * m(3, 3) - (double)m(0, 0) * m(1, 1) * m(2, 3) * m(3, 2);

		//m00m12(m23m31 - m21m33)
		double c2 = (double)m(0, 0) * m(1, 2) * m(2, 3) * m(3, 1) - (double)m(0, 0) * m(1, 2) * m(2, 1) * m(3, 3);

		//m00m13(m21m32 - m22m31)
		double c3 = (double)m(0, 0) * m(1, 3) * m(2, 1) * m(3, 2) - (double)m(0, 0) * m(1, 3) * m(2, 2) * m(3, 1);

		//m01m10(m22m33 - m23m32)
		double c4 = (double)m(0, 1) * m(1, 0) * m(2, 2) * m(3, 3) - (double)m(0, 1) * m(1, 0) * m(2, 3) * m(3, 2);

		//m01m12(m23m30 - m20m33)
		double c5 = (double)m(0, 1) * m(1, 2) * m(2, 3) * m(3, 0) - (double)m(0, 1) * m(1, 2) * m(2, 0) * m(3, 3);

		//m01m13(m20m32 - m22m30)
		double c6 = (double)m(0, 1) * m(1, 3) * m(2, 0) * m(3, 2) - (double)m(0, 1) * m(1, 3) * m(2, 2) * m(3, 0);

		//m02m10(m21m33 - m23m31)
		double c7 = (double)m(0, 2) * m(1, 0) * m(2, 1) * m(3, 3) - (double)m(0, 2) * m(1, 0) * m(2, 3) * m(3, 1);

		//m02m11(m23m30 - m20m33)
		double c8 = (double)m(0, 2) * m(1, 1) * m(2, 3) * m(3, 0) - (double)m(0, 2) * m(1, 1) * m(2, 0) * m(3, 3);

		//m02m13(m20m31 - m21m30)
		double c9 = (double)m(0, 2) * m(1, 3) * m(2, 0) * m(3, 1) - (double)m(0, 2) * m(1, 3) * m(2, 1) * m(3, 0);

		//m03m10(m21m32 - m22m21)
		double c10 = (double)m(0, 3) * m(1, 0) * m(2, 1) * m(3, 2) - (double)m(0, 3) * m(1, 0) * m(2, 2) * m(3, 1);

		//m03m11(m22m30 - m20m32)
		double c11 = (double)m(0, 3) * m(1, 1) * m(2, 2) * m(3, 0) - (double)m(0, 3) * m(1, 1) * m(2, 0) * m(3, 2);

		//m03m12(m20m31 - m21m30)
		double c12 = (double)m(0, 3) * m(1, 2) * m(2, 0) * m(3, 1) - (double)m(0, 3) * m(1, 2) * m(2, 1) * m(3, 0);

		return (c1 + c2 + c3) - (c4 + c5 + c6) + (c7 + c8 + c9) - (c10 + c11 + c12);
	}

	//cij = (-1)^i + j * det of minor(i, j);
	double cofactor(const Matrix4x4& m, unsigned int row, unsigned int col)
	{
		double tempMat[3][3];
		int tr{ 0 };
		int tc{ 0 };

		//minor(i, j)
		for (int i = 0; i < 4; ++i)
		{
			if (i == row)
				continue;

			for (int j = 0; j < 4; ++j)
			{
				if (j == col)
					continue;

				tempMat[tr][tc] = m(i, j);
				++tc;

			}
			tc = 0;
			++tr;
		}

		//determinant of minor(i, j)
		double det3x3 = (tempMat[0][0] * tempMat[1][1] * tempMat[2][2]) + (tempMat[0][1] * tempMat[1][2] * tempMat[2][0]) +
			(tempMat[0][2] * tempMat[1][0] * tempMat[2][1]) - (tempMat[0][2] * tempMat[1][1] * tempMat[2][0]) -
			(tempMat[0][1] * tempMat[1][0] * tempMat[2][2]) - (tempMat[0][0] * tempMat[1][2] * tempMat[2][1]);

		return pow(-1, row + col) * det3x3;
	}

	//Cofactor of each ijth position put into matrix cA.
	//Adjoint is the tranposed matrix of cA.
	Matrix4x4 adjoint(const Matrix4x4& m)
	{
		Matrix4x4 cA;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				cA(i, j) = cofactor(m, i, j);
			}
		}

		return transpose(cA);
	}

	//Inverse of m = adjoint of m / det of m
	Matrix4x4 inverse(const Matrix4x4& m)
	{
		double determinant = det(m);
		if (compareDoubles(determinant, 0.0, EPSILON))
			return Matrix4x4();

		return adjoint(m) * (1.0 / determinant);
	}


#if defined(_DEBUG)
	void print(const Matrix4x4& m)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				std::cout << m(i, j) << " ";
			}

			std::cout << std::endl;
		}
	}
#endif
	//-------------------------------------------------------------------------------------
}