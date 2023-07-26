#pragma once

#include <cmath>

#if defined(_DEBUG)
#include <iostream>
#endif


#define EPSILON 1e-6f
#define PI 3.14159f
#define PI2 6.28319f

namespace MathEngine
{

	struct Vector2D;
	struct Vector3D;
	struct Vector4D;
	class Matrix2x2;
	class Matrix3x3;
	class Matrix4x4;

//------------------------------------------------------------------------------------------------------------------------------------
//COMPARISON FUNCTIONS

	/**@brief Returns true if \a x and \a y are equal.
	*
	* Uses exact \a epsilion and adaptive \a epsilion to compare.
	*/
	inline bool CompareFloats(float x, float y, float epsilon)
	{
		float diff = fabs(x - y);
		//exact epsilon
		if (diff < epsilon)
		{
			return true;
		}

		//adapative epsilon
		return diff <= epsilon * (((fabs(x)) > (fabs(y))) ? (fabs(x)) : (fabs(y)));
	}

	/**@brief Returns true if \a x and \a y are equal.
	*
	* Uses exact \a epsilion and adaptive \a epsilion to compare.
	*/
	inline bool CompareDoubles(double x, double y, double epsilon)
	{
		double diff = fabs(x - y);
		//exact epsilon
		if (diff < epsilon)
		{
			return true;
		}

		//adapative epsilon
		return  diff <= epsilon * (((fabs(x)) > (fabs(y))) ? (fabs(x)) : (fabs(y)));
	}

	/**@brief Returns a clamped value.
	*
	* Returns a if value < a. Returns b if value > b. Returns value if it is between a and b.
	*/
	inline float Clamp(float value, float a, float b)
	{
		if (value < a)
			return a;

		if (value > b)
			return b;

		return value;
	}

//------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------
//2D VECTOR

	/** @struct Vector2D ""
	*	@brief A vector stuct used for 2D vectors/points.
	*
	*	The datatype for the components is float. When an object of this struct is made the components are intialized to 0.0f.
	*/
	struct Vector2D
	{
		float x = 0.0f;
		float y = 0.0f;
	};

	/**@brief Adds the 2D vector \a v1 to the 2D vector \a v2 and stores the result in \a v1.
	*/
	inline void operator+=(Vector2D& v1, const Vector2D& v2)
	{
		v1.x += v2.x;
		v1.y += v2.y;
	}

	/**@brief Subtracts the the 2D vector \a v2 from the 2D vector \a v1 and stores the result in \a v1.
	* 
	* v1 - v2.
	*/
	inline void operator-=(Vector2D& v1, const Vector2D& v2)
	{
		v1.x -= v2.x;
		v1.y -= v2.y;
	}

	/**@brief Multiplies the 2D vector \a v by the scalar (float) \a k and stores the result in \a v.
	*/
	inline void operator*=(Vector2D& v, float k)
	{
		v.x *= k;
		v.y *= k;
	}

	/**@brief Adds the two 2D vectors and returns the result.
	*/
	inline Vector2D operator+(const Vector2D& v1, const Vector2D& v2)
	{
		return Vector2D{ v1.x + v2.x, v1.y + v2.y };
	}

	/**@brief Negates the 3D vector \a v1 and returns the result.
	*/
	inline Vector2D operator-(const Vector2D& v)
	{
		return Vector2D{ -v.x, -v.y };
	}

	/**@brief Subtracts the 2D vector \a v2 from the 2D vector \a v1 and returns the result.
	* 
	* Returns v1 - v2.
	*/
	inline Vector2D operator-(const Vector2D& v1, const Vector2D& v2)
	{
		return Vector2D{ v1.x - v2.x, v1.y - v2.y };
	}

	/**@brief Multiplies the 2D vector \a v by the scalar (float) \a k and returns the result.
	*/
	inline Vector2D operator*(const Vector2D& v, float k)
	{
		return Vector2D{ v.x * k, v.y * k };
	}

	/**@brief Multiplies the scalar (float) \a k by the 2D vector \a v and returns the result.
	*/
	inline Vector2D operator*(float k, const Vector2D& v)
	{
		return Vector2D{ k * v.x, k * v.y };
	}

	/**@brief Returns true if the 2D vector \a v1 equals to the 2D vector\a v2, false otherwise.
	*/
	inline bool operator==(const Vector2D& v1, const Vector2D& v2)
	{
		return CompareFloats(v1.x, v2.x, EPSILON) && CompareFloats(v1.y, v2.y, EPSILON);
	}

	/**@brief Returns true if the 2D vector \a v1 does not equal to the 2D vector \a v2, false otherwise.
	*/
	inline bool operator!=(const Vector2D& v1, const Vector2D& v2)
	{
		return !(v1 == v2);
	}

	/**@brief Returns true if the 2D vector \a v is equal to the zero vector, false otherwise.
	*/
	inline bool ZeroVector(const Vector2D& v)
	{
		return CompareFloats(v.x, 0.0f, EPSILON) && CompareFloats(v.y, 0.0f, EPSILON);
	}

	/**@brief Returns the dot product between the 2D vectors \a v1 and \a v2.
	*/
	inline float DotProduct(const Vector2D& v1, const Vector2D& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	/**@brief Returns the length (magnitude) of the the 2D vector \a v.
	*/
	inline float Length(const Vector2D& v)
	{
		return sqrt(v.x * v.x + v.y * v.y);
	}

	/**@brief Normalizes (makes it unit length) the 2D vector \a v and returns the result.
	* 
	* If v is the zero vector, v is returned.
	*/
	inline Vector2D Normalize(const Vector2D& v)
	{
		if (ZeroVector(v))
			return v;

		float inverseLength{ 1.0f / Length(v) };

		return v * inverseLength;
	}

	/**@brief Linear interpolate between the two vectors \a start and \a end.
	*
	* If t is not between 0 and 1, it gets clamped.
	*/
	inline Vector2D Lerp(const Vector2D& start, const Vector2D& end, float t)
	{
		if (t < 0.0f)
			return start;
		else if (t > 1.0f)
			return end;

		return (1.0f - t) * start + t * end;
	}

#if defined(_DEBUG)
	inline void print(const Vector2D& v)
	{
		std::cout << "(" << v.x << ", " << v.y << ")";
	}
#endif

//------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------
//3D VECTOR

	/** @struct Vector3D ""
	*	@brief A vector stuct used for 3D vectors/points.
	*
	*	The datatype for the components is float. When an object of this struct is made the components are intialized to 0.0f.
	*/
	struct Vector3D
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};


	/**@brief Adds the 3D vector \a v1 to the 3D vector \a v2 and stores the result in \a v1.
	*/
	inline void operator+=(Vector3D& v1, const Vector3D& v2)
	{
		v1.x += v2.x;
		v1.y += v2.y;
		v1.z += v2.z;
	}

	/**@brief Subtracts the the 3D vector \a v2 from the 3D vector \a v1 and stores the result in \a v1.
	*
	* v1 - v2.
	*/
	inline void operator-=(Vector3D& v1, const Vector3D& v2)
	{
		v1.x -= v2.x;
		v1.y -= v2.y;
		v1.z -= v2.z;
	}

	/**@brief Multiplies the 3D vector \a v by the scalar (float) \a k and stores the result in \a v.
	*/
	inline void operator*=(Vector3D& v, float k)
	{
		v.x *= k;
		v.y *= k;
		v.z *= k;
	}

	/**@brief Adds the two 3D vectors and returns the result.
	*/
	inline Vector3D operator+(const Vector3D& v1, const Vector3D& v2)
	{
		return Vector3D{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	}

	/**@brief Negates the 3D vector \a v and returns the result.
	*/
	inline Vector3D operator-(const Vector3D& v)
	{
		return Vector3D{ -v.x, -v.y, -v.z };
	}

	/**@brief Subtracts the 3D vector \a v2 from the 3D vector \a v1 and returns the result.
	*
	* Returns v1 - v2.
	*/
	inline Vector3D operator-(const Vector3D& v1, const Vector3D& v2)
	{
		return Vector3D{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
	}

	/**@brief Multiplies the 3D vector \a v by the scalar (float) \a k and returns the result.
	*/
	inline Vector3D operator*(const Vector3D& v, float k)
	{
		return Vector3D{ v.x * k, v.y * k, v.z * k };
	}

	/**@brief Multiplies the scalar (float) \a k by the 3D vector \a v and returns the result.
	*/
	inline Vector3D operator*(float k, const Vector3D& v)
	{
		return Vector3D{ k * v.x, k * v.y, k * v.z};
	}

	/**@brief Returns true if the 3D vector \a v1 equals to the 3D vector\a v2, false otherwise.
	*/
	inline bool operator==(const Vector3D& v1, const Vector3D& v2)
	{
		return CompareFloats(v1.x, v2.x, EPSILON) && CompareFloats(v1.y, v2.y, EPSILON) && CompareFloats(v1.z, v2.z, EPSILON);
	}

	/**@brief Returns true if the 3D vector \a v1 does not equal to the 3D vector \a v2, false otherwise.
	*/
	inline bool operator!=(const Vector3D& v1, const Vector3D& v2)
	{
		return !(v1 == v2);
	}

	/**@brief Returns true if the 3D vector \a v is equal to the zero vector, false otherwise.
	*/
	inline bool ZeroVector(const Vector3D& v)
	{
		return CompareFloats(v.x, 0.0f, EPSILON) && CompareFloats(v.y, 0.0f, EPSILON) && CompareFloats(v.z, 0.0f, EPSILON);
	}

	/**@brief Returns the dot product between the 3D vectors \a v1 and \a v2.
	*/
	inline float DotProduct(const Vector3D& v1, const Vector3D& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	/**@brief Returns the cross product between the 3D vectors \a v1 and \a v2.
	*/
	inline Vector3D CrossProduct(const Vector3D& v1, const Vector3D& v2)
	{
		return Vector3D{ v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
	}

	/**@brief Returns the length (magnitude) of the the 3D vector \a v.
	*/
	inline float Length(const Vector3D& v)
	{
		return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}

	/**@brief Normalizes (makes it unit length) the 3D vector \a v and returns the result.
	* 
	* If v is the zero vector, v is returned.
	*/
	inline Vector3D Normalize(const Vector3D& v)
	{
		if (ZeroVector(v))
			return v;

		float inverseLength{ 1.0f / Length(v) };

		return v * inverseLength;
	}

	/**@brief Orthonormalizes the specified vectors.
	*/
	inline void Orthonormalize(Vector3D& x, Vector3D& y, Vector3D& z)
	{
		x = Normalize(x);
		y = Normalize(CrossProduct(z, x));
		z = Normalize(CrossProduct(x, y));
	}

	/**@brief Linear interpolate between the two vectors \a start and \a end.
	*
	* If t is not between 0 and 1, it gets clamped.
	*/
	inline Vector3D Lerp(const Vector3D& start, const Vector3D& end, float t)
	{
		if (t < 0.0f)
			return start;
		else if (t > 1.0f)
			return end;

		return (1.0f - t) * start + t * end;
	}

#if defined(_DEBUG)
	inline void print(const Vector3D& v)
	{
		std::cout << "(" << v.x << ", " << v.y << ")";
	}
#endif

//------------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------------
//4D VECTOR

	/** @struct Vector4D ""
	*	@brief A vector stuct used for 4D vectors/points.
	*
	*	The datatype for the components is float. When an object of this struct is made the components are intialized to 0.0f.
	*/
	struct Vector4D
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float w = 0.0f;
	};


	/**@brief Adds the 4D vector \a v1 to the 4D vector \a v2 and stores the result in \a v1.
	*/
	inline void operator+=(Vector4D& v1, const Vector4D& v2)
	{
		v1.x += v2.x;
		v1.y += v2.y;
		v1.z += v2.z;
		v1.w += v2.w;
	}

	/**@brief Subtracts the the 4D vector \a v2 from the 4D vector \a v1 and stores the result in \a v1.
	*
	* v1 - v2.
	*/
	inline void operator-=(Vector4D& v1, const Vector4D& v2)
	{
		v1.x -= v2.x;
		v1.y -= v2.y;
		v1.z -= v2.z;
		v1.w -= v2.w;
	}

	/**@brief Multiplies the 4D vector \a v by the scalar (float) \a k and stores the result in \a v.
	*/
	inline void operator*=(Vector4D& v, float k)
	{
		v.x *= k;
		v.y *= k;
		v.z *= k;
		v.w *= k;
	}

	/**@brief Adds the two 4D vectors and returns the result.
	*/
	inline Vector4D operator+(const Vector4D& v1, const Vector4D& v2)
	{
		return Vector4D{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w };
	}

	/**@brief Negates the 4D vector \a v and returns the result.
	*/
	inline Vector4D operator-(const Vector4D& v)
	{
		return Vector4D{ -v.x, -v.y, -v.z, - v.w };
	}

	/**@brief Subtracts the 4D vector \a v2 from the 4D vector \a v1 and returns the result.
	*
	* Returns v1 - v2.
	*/
	inline Vector4D operator-(const Vector4D& v1, const Vector4D& v2)
	{
		return Vector4D{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w };
	}

	/**@brief Multiplies the 4D vector \a v by the scalar (float) \a k and returns the result.
	*/
	inline Vector4D operator*(const Vector4D& v, float k)
	{
		return Vector4D{ v.x * k, v.y * k, v.z * k, v.w * k };
	}

	/**@brief Multiplies the scalar (float) \a k by the 4D vector \a v and returns the result.
	*/
	inline Vector4D operator*(float k, const Vector4D& v)
	{
		return Vector4D{ k * v.x, k * v.y, k * v.z, k * v.w };
	}

	/**@brief Returns true if the 4D vector \a v1 equals to the 4D vector\a v2, false otherwise.
	*/
	inline bool operator==(const Vector4D& v1, const Vector4D& v2)
	{
		return CompareFloats(v1.x, v2.x, EPSILON) && CompareFloats(v1.y, v2.y, EPSILON) && CompareFloats(v1.z, v2.z, EPSILON)
			&& CompareFloats(v1.w, v2.w, EPSILON);
	}

	/**@brief Returns true if the 4D vector \a v1 does not equal to the 4D vector \a v2, false otherwise.
	*/
	inline bool operator!=(const Vector4D& v1, const Vector4D& v2)
	{
		return !(v1 == v2);
	}

	/**@brief Returns true if the 4D vector \a v is equal to the zero vector, false otherwise.
	*/
	inline bool ZeroVector(const Vector4D& v)
	{
		return CompareFloats(v.x, 0.0f, EPSILON) && CompareFloats(v.y, 0.0f, EPSILON) && CompareFloats(v.z, 0.0f, EPSILON)
			&& CompareFloats(v.w, 0.0f, EPSILON);
	}

	/**@brief Returns the dot product between the 4D vectors \a v1 and \a v2.
	*/
	inline float DotProduct(const Vector4D& v1, const Vector4D& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
	}

	/**@brief Returns the length (magnitude) of the the 4D vector \a v.
	*/
	inline float Length(const Vector4D& v)
	{
		return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
	}

	/**@brief Normalizes (makes it unit length) the 4D vector \a v and returns the result.
	* 
	* If v is the zero vector, v is returned.
	*/
	inline Vector4D Normalize(const Vector4D& v)
	{
		if (ZeroVector(v))
			return v;

		float inverseLength{ 1.0f / Length(v) };

		return v * inverseLength;
	}

	/**@brief Linear interpolate between the two vectors \a start and \a end.
	*
	* If t is not between 0 and 1, it gets clamped.
	*/
	inline Vector4D Lerp(const Vector4D& start, const Vector4D& end, float t)
	{
		if (t < 0.0f)
			return start;
		else if (t > 1.0f)
			return end;

		return (1.0f - t) * start + t * end;
	}

#if defined(_DEBUG)
	inline void print(const Vector4D& v)
	{
		std::cout << "(" << v.x << ", " << v.y << ")";
	}
#endif

//------------------------------------------------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------------------------------------------------
	/** @class Matrix2x2 ""
	*	@brief A matrix class used for 2x2 matrices and their manipulations.
	*
	*	The datatype for the components is float.\n
	*	The 2x2 matrix is treated as a row-major matrix.
	*
	*/
	class Matrix2x2
	{
	public:

		/**@brief Creates a new 2x2 identity matrix.
		*/
		Matrix2x2();

		/**@brief Creates a new 2x2 matrix with elements initialized to the given 2D array.
		*
		*	If \a a isn't a 2x2 matrix, the behavior is undefined.
		*/
		Matrix2x2(float a[][2]);

		/**@brief Creates a new 2x2 matrix with each row being set to the specified rows.
		*/
		Matrix2x2(const Vector2D& r1, const Vector2D& r2);

		/**@brief Creates a new 2x2 matrix with each row being set to the first two values of the respective rows of the 3x3 matrix.
		*/
		Matrix2x2(const Matrix3x3& m);

		/**@brief Creates a new 2x2 matrix with each row being set to the first two values of the respective rows of the 4x4 matrix.
		*/
		Matrix2x2(const Matrix4x4& m);

		/**@brief Returns a pointer to the first element in the matrix.
		*/
		float* Data();

		/**@brief Returns a constant pointer to the first element in the matrix.
		*/
		const float* Data() const;

		/**@brief Returns a constant reference to the element at the given (\a row, \a col).
		*
		* The \a row and \a col values should be between [0,1]. If any of them are out of that range, the first element will be returned.
		*/
		const float& operator()(unsigned int row, unsigned int col) const;

		/**@brief Returns a reference to the element at the given (\a row, \a col).
		*
		* The \a row and \a col values should be between [0,1]. If any of them are out of that range, the first element will be returned.
		*/
		float& operator()(unsigned int row, unsigned int col);

		/**@brief Returns the specified \a row.
		*
		*  \a Row should be between [0,1]. If it is out of range the first row will be returned.
		*/
		Vector2D GetRow(unsigned int row) const;

		/**@brief Returns the specified \a col.
		*
		* \a Col should be between [0,1]. If it is out of range the first col will be returned.
		*/
		Vector2D GetCol(unsigned int col) const;

		/**@brief Sets each element in the given \a row to the components of vector \a v.
		*
		* \a Row should be between [0,1]. If it is out of range the first row will be set.
		*/
		void SetRow(unsigned int row, Vector2D v);

		/**@brief Sets each element in the given \a col to the components of vector \a v.
		*
		* \a Col should be between [0,1]. If it is out of range the first col will be set.
		*/
		void SetCol(unsigned int col, Vector2D v);

		/**@brief Sets the values each row to the first two values of the respective rows of the 3x3 matrix.
		*/
		Matrix2x2& operator=(const Matrix3x3& m);

		/**@brief Sets the values each row to the first two values of the respective rows of the 4x4 matrix.
		*/
		Matrix2x2& operator=(const Matrix4x4& m);

		/**@brief Adds this 2x2 matrix with given matrix \a m and stores the result in this 2x2 matrix.
		*/
		Matrix2x2& operator+=(const Matrix2x2& m);

		/**@brief Subtracts \a m from this 2x2 matrix stores the result in this 2x2 matrix.
		*/
		Matrix2x2& operator-=(const Matrix2x2& m);

		/**@brief Multiplies this 2x2 matrix with \a k and stores the result in this 2x2 matrix.
		*/
		Matrix2x2& operator*=(float k);

		/**@brief Multiplies this 2x2 matrix with given matrix \a m and stores the result in this 2x2 matrix.
		*/
		Matrix2x2& operator*=(const Matrix2x2& m);

	private:

		float mMat[2][2];
	};

	//-------------------------------------------------------------------------------------
	inline Matrix2x2::Matrix2x2()
	{
		//1st row
		mMat[0][0] = 1.0f;
		mMat[0][1] = 0.0f;

		//2nd
		mMat[1][0] = 0.0f;
		mMat[1][1] = 1.0f;
	}

	inline Matrix2x2::Matrix2x2(float a[][2])
	{
		//1st row
		mMat[0][0] = a[0][0];
		mMat[0][1] = a[0][1];

		//2nd row
		mMat[1][0] = a[1][0];
		mMat[1][1] = a[1][1];
	}

	inline Matrix2x2::Matrix2x2(const Vector2D& r1, const Vector2D& r2)
	{
		SetRow(0, r1);
		SetRow(1, r2);
	}

	inline float* Matrix2x2::Data()
	{
		return mMat[0];
	}

	inline const float* Matrix2x2::Data() const
	{
		return mMat[0];
	}

	inline const float& Matrix2x2::operator()(unsigned int row, unsigned int col) const
	{
		if (row > 1 || col > 1)
		{
			return mMat[0][0];
		}
		else
		{
			return mMat[row][col];
		}
	}

	inline float& Matrix2x2::operator()(unsigned int row, unsigned int col)
	{
		if (row > 1 || col > 1)
		{
			return mMat[0][0];
		}
		else
		{
			return mMat[row][col];
		}
	}

	inline Vector2D Matrix2x2::GetRow(unsigned int row) const
	{
		if (row < 0 || row > 1)
			return Vector2D{ mMat[0][0], mMat[0][1] };
		else
			return Vector2D{ mMat[row][0], mMat[row][1] };

	}

	inline Vector2D Matrix2x2::GetCol(unsigned int col) const
	{
		if (col < 0 || col > 1)
			return Vector2D{ mMat[0][0], mMat[1][0] };
		else
			return Vector2D{ mMat[0][col], mMat[1][col] };
	}

	inline void Matrix2x2::SetRow(unsigned int row, Vector2D v)
	{
		if (row > 1)
		{
			mMat[0][0] = v.x;
			mMat[0][1] = v.y;
		}
		else
		{
			mMat[row][0] = v.x;
			mMat[row][1] = v.y;
		}
	}

	inline void Matrix2x2::SetCol(unsigned int col, Vector2D v)
	{
		if (col > 1)
		{
			mMat[0][0] = v.x;
			mMat[1][0] = v.y;
		}
		else
		{
			mMat[0][col] = v.x;
			mMat[1][col] = v.y;
		}
	}

	inline Matrix2x2& Matrix2x2::operator+=(const Matrix2x2& m)
	{
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				this->mMat[i][j] += m.mMat[i][j];
			}
		}

		return *this;
	}

	inline Matrix2x2& Matrix2x2::operator-=(const Matrix2x2& m)
	{
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				this->mMat[i][j] -= m.mMat[i][j];
			}
		}

		return *this;
	}

	inline Matrix2x2& Matrix2x2::operator*=(float k)
	{
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				this->mMat[i][j] *= k;
			}
		}

		return *this;
	}

	inline Matrix2x2& Matrix2x2::operator*=(const Matrix2x2& m)
	{
		Matrix2x2 res;

		for (int i = 0; i < 2; ++i)
		{
			res.mMat[i][0] =
				(mMat[i][0] * m.mMat[0][0]) +
				(mMat[i][1] * m.mMat[1][0]);

			res.mMat[i][1] =
				(mMat[i][0] * m.mMat[0][1]) +
				(mMat[i][1] * m.mMat[1][1]);
		}

		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				mMat[i][j] = res.mMat[i][j];
			}
		}

		return *this;
	}

	/**@brief Adds \a m1 with \a m2 and returns the result.
	*/
	inline Matrix2x2 operator+(const Matrix2x2& m1, const Matrix2x2& m2)
	{
		Matrix2x2 res;
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				res(i, j) = m1(i, j) + m2(i, j);
			}
		}

		return res;
	}

	/**@brief Negates the 2x2 matrix \a m.
	*/
	inline Matrix2x2 operator-(const Matrix2x2& m)
	{
		Matrix2x2 res;
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				res(i, j) = -m(i, j);
			}
		}

		return res;
	}

	/**@brief Subtracts \a m2 from \a m1 and returns the result.
	*/
	inline Matrix2x2 operator-(const Matrix2x2& m1, const Matrix2x2& m2)
	{
		Matrix2x2 res;
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				res(i, j) = m1(i, j) - m2(i, j);
			}
		}

		return res;
	}

	/**@brief Multiplies \a m with \a k and returns the result.
	*/
	inline Matrix2x2 operator*(const Matrix2x2& m, const float& k)
	{
		Matrix2x2 res;
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				res(i, j) = m(i, j) * k;
			}
		}

		return res;
	}

	/**@brief Multiplies \a k with \m and returns the result.
	*/
	inline Matrix2x2 operator*(const float& k, const Matrix2x2& m)
	{
		Matrix2x2 res;
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				res(i, j) = k * m(i, j);
			}
		}

		return res;
	}

	/**@brief Multiplies \a m1 with \m2 and returns the result.
	*
	* Does \a m1 * \a m2 in that order.
	*/
	inline Matrix2x2 operator*(const Matrix2x2& m1, const Matrix2x2& m2)
	{
		Matrix2x2 res;

		for (int i = 0; i < 4; ++i)
		{
			res(i, 0) =
				(m1(i, 0) * m2(0, 0)) +
				(m1(i, 1) * m2(1, 0));

			res(i, 1) =
				(m1(i, 0) * m2(0, 1)) +
				(m1(i, 1) * m2(1, 1));

			res(i, 2) =
				(m1(i, 0) * m2(0, 2)) +
				(m1(i, 1) * m2(1, 2));

			res(i, 3) =
				(m1(i, 0) * m2(0, 3)) +
				(m1(i, 1) * m2(1, 3));
		}

		return res;
	}

	/**@brief Multiplies \a m with \a v and returns the result.
	*
	* The vector \a v is a column vector.
	*/
	inline Vector2D operator*(const Matrix2x2& m, const Vector2D& v)
	{
		Vector2D res;

		res.x = m(0, 0) * v.x + m(0, 1) * v.y;

		res.y = m(1, 0) * v.x + m(1, 1) * v.y;

		return res;
	}

	/**@brief Multiplies \a v with \a m and returns the result.
	*
	* The vector \a v is a row vector.
	*/
	inline Vector2D operator*(const Vector2D& v, const Matrix2x2& m)
	{
		Vector2D res;

		res.x = v.x * m(0, 0) + v.y * m(1, 0);

		res.y = v.x * m(0, 1) + v.y * m(1, 1);

		return res;
	}

	/**@brief Sets \a m to the identity matrix.
	*/
	inline void SetToIdentity(Matrix2x2& m)
	{
		//set to identity matrix by setting the diagonals to 1.0f and all other elements to 0.0f

		//1st row
		m(0, 0) = 1.0f;
		m(0, 1) = 0.0f;

		//2nd row
		m(1, 0) = 0.0f;
		m(1, 1) = 1.0f;
	}

	/**@brief Returns true if \a m is the identity matrix, false otherwise.
	*/
	inline bool Identity(const Matrix2x2& m)
	{
		//Is the identity matrix if the diagonals are equal to 1.0f and all other elements equals to 0.0f

		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				if (i == j)
				{
					if (!CompareFloats(m(i, j), 1.0f, EPSILON))
						return false;

				}
				else
				{
					if (!CompareFloats(m(i, j), 0.0f, EPSILON))
						return false;

				}
			}
		}
	}

	/**@brief Returns the tranpose of the given matrix \a m.
	*/
	inline Matrix2x2 Transpose(const Matrix2x2& m)
	{
		//make the rows into cols

		Matrix2x2 res;

		//1st col = 1st row
		res(0, 0) = m(0, 0);
		res(1, 0) = m(0, 1);

		//2nd col = 2nd row
		res(0, 1) = m(1, 0);
		res(1, 1) = m(1, 1);

		return res;
	}

	/**@brief Returns a 2x2 scaling matrix.
	*/
	inline Matrix2x2 Scale(float x, float y)
	{
		//x 0
		//0 y

		Matrix2x2 scale;
		scale(0, 0) = x;
		scale(1, 1) = y;

		return scale;
	}

	/**@brief Returns a 2x2 scaling matrix.
	*/
	inline Matrix2x2 Scale(const Vector2D& scaleVector)
	{
		//x 0
		//0 y

		Matrix2x2 scale;
		scale(0, 0) = scaleVector.x;
		scale(1, 1) = scaleVector.y;

		return scale;
	}

	/**@brief Returns a 2x2 rotation matrix that rotates a point/vector about the origin.
	*/
	inline Matrix2x2 Rotate(float angle)
	{
		//c		s
		//-s	c
		//c = cos(angle)
		//s = sin(angle)

		float c = cos(angle * PI / 180.0f);
		float s = sin(angle * PI / 180.0f);

		Matrix2x2 result;

		//1st row
		result(0, 0) = c;
		result(0, 1) = s;

		//2nd row
		result(1, 0) = -s;
		result(1, 1) = c;

		return result;
	}

	/**@brief Returns the determinant of \a m.
	*/
	inline double Determinant(const Matrix2x2& m)
	{
		return (double)m(0, 0) * m(1, 1) - (double)m(0, 1) * m(1, 0);
	}

	/**@brief Returns the cofactor of the \a row and \a col in \a m.
	*/
	inline double Cofactor(const Matrix2x2& m, unsigned int row, unsigned int col)
	{
		//cij = ((-1)^i + j) * det of minor(i, j);
		double minor{ 0.0 };

		if (row == 0 && col == 0)
			minor = m(1, 1);
		else if (row == 0 && col == 1)
			minor = m(1, 0);
		else if (row == 1 && col == 0)
			minor = m(0, 1);
		else if (row == 1 && col == 1)
			minor = m(0, 0);

		return pow(-1, row + col) * minor;
	}

	/**@brief Returns the adjoint of \a m.
	*/
	inline Matrix2x2 Adjoint(const Matrix2x2& m)
	{
		//Cofactor of each ijth position put into matrix cA.
		//Adjoint is the tranposed matrix of cA.
		Matrix2x2 cofactorMatrix;
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				cofactorMatrix(i, j) = static_cast<float>(Cofactor(m, i, j));
			}
		}

		return Transpose(cofactorMatrix);
	}

	/**@brief Returns the inverse of \a m.
	*
	* If \a m is noninvertible/singular, the identity matrix is returned.
	*/
	inline Matrix2x2 Inverse(const Matrix2x2& m)
	{
		//Inverse of m = adjoint of m / det of m
		double det = Determinant(m);
		if (CompareDoubles(det, 0.0, EPSILON))
			return Matrix2x2();

		return Adjoint(m) * (1.0f / static_cast<float>(det));
	}


#if defined(_DEBUG)
	inline void print(const Matrix2x2& m)
	{
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				std::cout << m(i, j) << " ";
			}

			std::cout << std::endl;
		}
	}
#endif

//------------------------------------------------------------------------------------------------------------------------------------





//------------------------------------------------------------------------------------------------------------------------------------
	/** @class Matrix3x3 ""
	*	@brief A matrix class used for 3x3 matrices and their manipulations.
	*
	*	The datatype for the components is float.\n
	*	The 3x3 matrix is treated as a row-major matrix.
	*
	*/
	class Matrix3x3
	{
	public:

		/**@brief Creates a new 3x3 identity matrix.
		*/
		Matrix3x3();

		/**@brief Creates a new 3x3 matrix with elements initialized to the given 2D array.
		*
		*	If \a a isn't a 3x3 matrix, the behavior is undefined.
		*/
		Matrix3x3(float a[][3]);

		/**@brief Creates a new 3x3 matrix with each row being set to the specified rows.
		*/
		Matrix3x3(const Vector3D& r1, const Vector3D& r2, const Vector3D& r3);

		/**@brief Creates a new 3x3 matrix with the first two values of the first two rows being set to the values of the 2x2 matrix.
		*
		* The last value of the first two rows is set to 0.
		* The last row is set to (0, 0, 1);.
		*/
		Matrix3x3(const Matrix2x2& m);

		/**@brief Creates a new 3x3 matrix with each row being set to the first three values of the respective rows of the 4x4 matrix.
		*/
		Matrix3x3(const Matrix4x4& m);

		/**@brief Returns a pointer to the first element in the matrix.
		*/
		float* Data();

		/**@brief Returns a constant pointer to the first element in the matrix.
		*/
		const float* Data() const;

		/**@brief Returns a constant reference to the element at the given (\a row, \a col).
		*
		* The \a row and \a col values should be between [0,2]. If any of them are out of that range, the first element will be returned.
		*/
		const float& operator()(unsigned int row, unsigned int col) const;

		/**@brief Returns a reference to the element at the given (\a row, \a col).
		*
		* The \a row and \a col values should be between [0,2]. If any of them are out of that range, the first element will be returned.
		*/
		float& operator()(unsigned int row, unsigned int col);

		/**@brief Returns the specified \a row.
		*
		*  \a Row should be between [0,2]. If it is out of range the first row will be returned.
		*/
		Vector3D GetRow(unsigned int row) const;

		/**@brief Returns the specified \a col.
		*
		* \a Col should be between [0,2]. If it is out of range the first col will be returned.
		*/
		Vector3D GetCol(unsigned int col) const;

		/**@brief Sets each element in the given \a row to the components of vector \a v.
		*
		* \a Row should be between [0,2]. If it is out of range the first row will be set.
		*/
		void SetRow(unsigned int row, Vector3D v);

		/**@brief Sets each element in the given \a col to the components of vector \a v.
		*
		* \a Col should be between [0,2]. If it is out of range the first col will be set.
		*/
		void SetCol(unsigned int col, Vector3D v);

		/**@brief Sets the first two values of the first two rows to the values of the 2x2 matrix.
		*
		* The last value of the first two rows is set to 0.
		* The last row is set to (0, 0, 1);.
		*/
		Matrix3x3& operator=(const Matrix2x2& m);

		/**@brief Sets the values of each row to the first three values of the respective rows of the 4x4 matrix.
		*/
		Matrix3x3& operator=(const Matrix4x4& m);

		/**@brief Adds this 3x3 matrix with given matrix \a m and stores the result in this 3x3 matrix.
		*/
		Matrix3x3& operator+=(const Matrix3x3& m);

		/**@brief Subtracts \a m from this 3x3 matrix stores the result in this 3x3 matrix.
		*/
		Matrix3x3& operator-=(const Matrix3x3& m);

		/**@brief Multiplies this 3x3 matrix with \a k and stores the result in this 3x3 matrix.
		*/
		Matrix3x3& operator*=(float k);

		/**@brief Multiplies this 3x3 matrix with given matrix \a m and stores the result in this 3x3 matrix.
		*/
		Matrix3x3& operator*=(const Matrix3x3& m);

	private:

		float mMat[3][3];
	};

	//-------------------------------------------------------------------------------------
	inline Matrix3x3::Matrix3x3()
	{
		//1st row
		mMat[0][0] = 1.0f;
		mMat[0][1] = 0.0f;
		mMat[0][2] = 0.0f;

		//2nd
		mMat[1][0] = 0.0f;
		mMat[1][1] = 1.0f;
		mMat[1][2] = 0.0f;

		//3rd row
		mMat[2][0] = 0.0f;
		mMat[2][1] = 0.0f;
		mMat[2][2] = 1.0f;
	}

	inline Matrix3x3::Matrix3x3(float a[][3])
	{
		//1st row
		mMat[0][0] = a[0][0];
		mMat[0][1] = a[0][1];
		mMat[0][2] = a[0][2];

		//2nd
		mMat[1][0] = a[1][0];
		mMat[1][1] = a[1][1];
		mMat[1][2] = a[1][2];

		//3rd row
		mMat[2][0] = a[2][0];
		mMat[2][1] = a[2][1];
		mMat[2][2] = a[2][2];
	}

	inline Matrix3x3::Matrix3x3(const Vector3D& r1, const Vector3D& r2, const Vector3D& r3)
	{
		SetRow(0, r1);
		SetRow(1, r2);
		SetRow(2, r3);
	}

	inline float* Matrix3x3::Data()
	{
		return mMat[0];
	}

	inline const float* Matrix3x3::Data() const
	{
		return mMat[0];
	}

	inline const float& Matrix3x3::operator()(unsigned int row, unsigned int col) const
	{
		if (row > 2 || col > 2)
		{
			return mMat[0][0];
		}
		else
		{
			return mMat[row][col];
		}
	}

	inline float& Matrix3x3::operator()(unsigned int row, unsigned int col)
	{
		if (row > 2 || col > 2)
		{
			return mMat[0][0];
		}
		else
		{
			return mMat[row][col];
		}
	}

	inline Vector3D Matrix3x3::GetRow(unsigned int row) const
	{
		if (row < 0 || row > 2)
			return Vector3D{ mMat[0][0], mMat[0][1], mMat[0][2] };
		else
			return Vector3D{ mMat[row][0], mMat[row][1], mMat[row][2] };

	}

	inline Vector3D Matrix3x3::GetCol(unsigned int col) const
	{
		if (col < 0 || col > 2)
			return Vector3D{ mMat[0][0], mMat[1][0], mMat[2][0] };
		else
			return Vector3D{ mMat[0][col], mMat[1][col], mMat[2][col] };
	}

	inline void Matrix3x3::SetRow(unsigned int row, Vector3D v)
	{
		if (row > 2)
		{
			mMat[0][0] = v.x;
			mMat[0][1] = v.y;
			mMat[0][2] = v.z;
		}
		else
		{
			mMat[row][0] = v.x;
			mMat[row][1] = v.y;
			mMat[row][2] = v.z;
		}
	}

	inline void Matrix3x3::SetCol(unsigned int col, Vector3D v)
	{
		if (col > 2)
		{
			mMat[0][0] = v.x;
			mMat[1][0] = v.y;
			mMat[2][0] = v.z;
		}
		else
		{
			mMat[0][col] = v.x;
			mMat[1][col] = v.y;
			mMat[2][col] = v.z;
		}
	}

	inline Matrix3x3& Matrix3x3::operator+=(const Matrix3x3& m)
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				this->mMat[i][j] += m.mMat[i][j];
			}
		}

		return *this;
	}

	inline Matrix3x3& Matrix3x3::operator-=(const Matrix3x3& m)
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				this->mMat[i][j] -= m.mMat[i][j];
			}
		}

		return *this;
	}

	inline Matrix3x3& Matrix3x3::operator*=(float k)
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				this->mMat[i][j] *= k;
			}
		}

		return *this;
	}

	inline Matrix3x3& Matrix3x3::operator*=(const Matrix3x3& m)
	{
		Matrix3x3 result;

		for (int i = 0; i < 3; ++i)
		{
			result.mMat[i][0] =
				(mMat[i][0] * m.mMat[0][0]) +
				(mMat[i][1] * m.mMat[1][0]) +
				(mMat[i][2] * m.mMat[2][0]);

			result.mMat[i][1] =
				(mMat[i][0] * m.mMat[0][1]) +
				(mMat[i][1] * m.mMat[1][1]) +
				(mMat[i][2] * m.mMat[2][1]);

			result.mMat[i][2] =
				(mMat[i][0] * m.mMat[0][2]) +
				(mMat[i][1] * m.mMat[1][2]) +
				(mMat[i][2] * m.mMat[2][2]);
		}

		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				mMat[i][j] = result.mMat[i][j];
			}
		}

		return *this;
	}

	/**@brief Adds \a m1 with \a m2 and returns the result.
	*/
	inline Matrix3x3 operator+(const Matrix3x3& m1, const Matrix3x3& m2)
	{
		Matrix3x3 result;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				result(i, j) = m1(i, j) + m2(i, j);
			}
		}

		return result;
	}

	/**@brief Negates the 3x3 matrix \a m.
	*/
	inline Matrix3x3 operator-(const Matrix3x3& m)
	{
		Matrix3x3 result;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				result(i, j) = -m(i, j);
			}
		}

		return result;
	}

	/**@brief Subtracts \a m2 from \a m1 and returns the result.
	*/
	inline Matrix3x3 operator-(const Matrix3x3& m1, const Matrix3x3& m2)
	{
		Matrix3x3 result;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				result(i, j) = m1(i, j) - m2(i, j);
			}
		}

		return result;
	}

	/**@brief Multiplies \a m with \a k and returns the result.
	*/
	inline Matrix3x3 operator*(const Matrix3x3& m, const float& k)
	{
		Matrix3x3 result;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				result(i, j) = m(i, j) * k;
			}
		}

		return result;
	}

	/**@brief Multiplies \a k with \m and returns the result.
	*/
	inline Matrix3x3 operator*(const float& k, const Matrix3x3& m)
	{
		Matrix3x3 result;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				result(i, j) = k * m(i, j);
			}
		}

		return result;
	}

	/**@brief Multiplies \a m1 with \m2 and returns the result.
	*
	* Does \a m1 * \a m2 in that order.
	*/
	inline Matrix3x3 operator*(const Matrix3x3& m1, const Matrix3x3& m2)
	{
		Matrix3x3 result;

		for (int i = 0; i < 4; ++i)
		{
			result(i, 0) =
				(m1(i, 0) * m2(0, 0)) +
				(m1(i, 1) * m2(1, 0)) +
				(m1(i, 2) * m2(2, 0));

			result(i, 1) =
				(m1(i, 0) * m2(0, 1)) +
				(m1(i, 1) * m2(1, 1)) +
				(m1(i, 2) * m2(2, 1));

			result(i, 2) =
				(m1(i, 0) * m2(0, 2)) +
				(m1(i, 1) * m2(1, 2)) +
				(m1(i, 2) * m2(2, 2));

			result(i, 3) =
				(m1(i, 0) * m2(0, 3)) +
				(m1(i, 1) * m2(1, 3)) +
				(m1(i, 2) * m2(2, 3));
		}

		return result;
	}

	/**@brief Multiplies \a m with \a v and returns the result.
	*
	* The vector \a v is a column vector.
	*/
	inline Vector3D operator*(const Matrix3x3& m, const Vector3D& v)
	{
		Vector3D result;

		result.x = m(0, 0) * v.x + m(0, 1) * v.y + m(0, 2) * v.z;

		result.y = m(1, 0) * v.x + m(1, 1) * v.y + m(1, 2) * v.z;

		result.z = m(2, 0) * v.x + m(2, 1) * v.y + m(2, 2) * v.z;

		return result;
	}

	/**@brief Multiplies \a v with \a m and returns the result.
	*
	* The vector \a v is a row vector.
	*/
	inline Vector3D operator*(const Vector3D& v, const Matrix3x3& m)
	{
		Vector3D result;

		result.x = v.x * m(0, 0) + v.y * m(1, 0) + v.z * m(2, 0);

		result.y = v.x * m(0, 1) + v.y * m(1, 1) + v.z * m(2, 1);

		result.z = v.x * m(0, 2) + v.y * m(1, 2) + v.z * m(2, 2);

		return result;
	}

	/**@brief Sets \a m to the identity matrix.
	*/
	inline void SetToIdentity(Matrix3x3& m)
	{
		//set to identity matrix by setting the diagonals to 1.0f and all other elements to 0.0f

		//1st row
		m(0, 0) = 1.0f;
		m(0, 1) = 0.0f;
		m(0, 2) = 0.0f;

		//2nd row
		m(1, 0) = 0.0f;
		m(1, 1) = 1.0f;
		m(1, 2) = 0.0f;

		//3rd row
		m(2, 0) = 0.0f;
		m(2, 1) = 0.0f;
		m(2, 2) = 1.0f;
	}

	/**@brief Returns true if \a m is the identity matrix, false otherwise.
	*/
	inline bool Identity(const Matrix3x3& m)
	{
		//Is the identity matrix if the diagonals are equal to 1.0f and all other elements equals to 0.0f

		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if (i == j)
				{
					if (!CompareFloats(m(i, j), 1.0f, EPSILON))
						return false;

				}
				else
				{
					if (!CompareFloats(m(i, j), 0.0f, EPSILON))
						return false;

				}
			}
		}
	}

	/**@brief Returns the tranpose of the given matrix \a m.
	*/
	inline Matrix3x3 Transpose(const Matrix3x3& m)
	{
		//make the rows into cols

		Matrix3x3 result;

		//1st col = 1st row
		result(0, 0) = m(0, 0);
		result(1, 0) = m(0, 1);
		result(2, 0) = m(0, 2);

		//2nd col = 2nd row
		result(0, 1) = m(1, 0);
		result(1, 1) = m(1, 1);
		result(2, 1) = m(1, 2);

		//3rd col = 3rd row
		result(0, 2) = m(2, 0);
		result(1, 2) = m(2, 1);
		result(2, 2) = m(2, 2);

		return result;
	}

	/**brief Returns a 3x3 scale matrix.
	*/
	inline Matrix3x3 Scale(float x, float y, float z)
	{
		//x 0 0
		//0 y 0
		//0 0 z

		Matrix3x3 scale;
		scale(0, 0) = x;
		scale(1, 1) = y;
		scale(2, 2) = z;

		return scale;
	}

	/**brief Returns a 3x3 scale matrix.
	*/
	inline Matrix3x3 Scale(const Vector3D& scaleVector)
	{
		//x 0 0
		//0 y 0
		//0 0 z

		Matrix3x3 scale;
		scale(0, 0) = scaleVector.x;
		scale(1, 1) = scaleVector.y;
		scale(2, 2) = scaleVector.z;

		return scale;
	}

	/**brief Returns a 3x3 rotation matrix about the given axis.
	*/
	inline Matrix3x3 Rotate(float angle, float x, float y, float z)
	{

		//c + (1 - c)x^2	(1 - c)xy + sz	(1 - c)xz - sy
		//(1 - c)xy - sz	c + (1 - c)y^2	(1 - c)yz + sx
		//(1 - c)xz + sy	(1 - c)yz - sx	c + (1 - c)z^2
		//c = cos(angle)
		//s = sin(angle)

		Vector3D axis{ x, y, z };
		axis = Normalize(axis);
		x = axis.x;
		y = axis.y;
		z = axis.z;

		float c = cos(angle * PI / 180.0f);
		float s = sin(angle * PI / 180.0f);
		float oneMinusC = 1.0f - c;

		Matrix3x3 result;

		//1st row
		result(0, 0) = c + (oneMinusC * (x * x));
		result(0, 1) = (oneMinusC * (x * y)) + (s * z);
		result(0, 2) = (oneMinusC * (x * z)) - (s * y);

		//2nd row
		result(1, 0) = (oneMinusC * (x * y)) - (s * z);
		result(1, 1) = c + (oneMinusC * (y * y));
		result(1, 2) = (oneMinusC * (y * z)) + (s * x);

		//3rd row
		result(2, 0) = (oneMinusC * (x * z)) + (s * y);
		result(2, 1) = (oneMinusC * (y * z)) - (s * x);
		result(2, 2) = c + (oneMinusC * (z * z));

		return result;
	}

	/**brief Returns a 3x3 rotation matrix about the given axis.
	*/
	inline Matrix3x3 Rotate(float angle, const Vector3D& axis)
	{

		//c + (1 - c)x^2	(1 - c)xy + sz	(1 - c)xz - sy
		//(1 - c)xy - sz	c + (1 - c)y^2	(1 - c)yz + sx
		//(1 - c)xz + sy	(1 - c)yz - sx	c + (1 - c)z^2
		//c = cos(angle)
		//s = sin(angle)

		Vector3D nAxis(Normalize(axis));
		float x = nAxis.x;
		float y = nAxis.y;
		float z = nAxis.z;

		float c = cos(angle * PI / 180.0f);
		float s = sin(angle * PI / 180.0f);
		float oneMinusC = 1.0f - c;

		Matrix3x3 result;

		//1st row
		result(0, 0) = c + (oneMinusC * (x * x));
		result(0, 1) = (oneMinusC * (x * y)) + (s * z);
		result(0, 2) = (oneMinusC * (x * z)) - (s * y);

		//2nd row
		result(1, 0) = (oneMinusC * (x * y)) - (s * z);
		result(1, 1) = c + (oneMinusC * (y * y));
		result(1, 2) = (oneMinusC * (y * z)) + (s * x);

		//3rd row
		result(2, 0) = (oneMinusC * (x * z)) + (s * y);
		result(2, 1) = (oneMinusC * (y * z)) - (s * x);
		result(2, 2) = c + (oneMinusC * (z * z));

		return result;
	}

	/**@brief Returns the determinant of \a m.
	*/
	inline double Determinant(const Matrix3x3& m)
	{
		//m00m11m22 - m00m12m21
		double c1 = (double)m(0, 0) * m(1, 1) * m(2, 2) - (double)m(0, 0) * m(1, 2) * m(2, 1);

		//m01m12m20 - m01m10m22
		double c2 = (double)m(0, 1) * m(1, 2) * m(2, 0) - (double)m(0, 1) * m(1, 0) * m(2, 2);

		//m02m10m21 - m02m11m20
		double c3 = (double)m(0, 2) * m(1, 0) * m(2, 1) - (double)m(0, 2) * m(1, 1) * m(2, 0);

		return c1 + c2 + c3;
	}

	/**@brief Returns the cofactor of the \a row and \a col in \a m.
	*/
	inline double Cofactor(const Matrix3x3& m, unsigned int row, unsigned int col)
	{
		//cij = ((-1)^i + j) * det of minor(i, j);
		Matrix2x2 minor;
		int r{ 0 };
		int c{ 0 };

		//minor(i, j)
		for (int i = 0; i < 3; ++i)
		{
			if (i == row)
				continue;

			for (int j = 0; j < 3; ++j)
			{
				if (j == col)
					continue;

				minor(r, c) = m(i, j);
				++c;

			}
			c = 0;
			++r;
		}

		return pow(-1, row + col) * Determinant(minor);
	}

	/**@brief Returns the adjoint of \a m.
	*/
	inline Matrix3x3 Adjoint(const Matrix3x3& m)
	{
		//Cofactor of each ijth position put into matrix cA.
		//Adjoint is the tranposed matrix of cA.
		Matrix3x3 cofactorMatrix;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				cofactorMatrix(i, j) = static_cast<float>(Cofactor(m, i, j));
			}
		}

		return Transpose(cofactorMatrix);
	}

	/**@brief Returns the inverse of \a m.
	*
	* If \a m is noninvertible/singular, the identity matrix is returned.
	*/
	inline Matrix3x3 Inverse(const Matrix3x3& m)
	{
		//Inverse of m = adjoint of m / det of m
		double det = Determinant(m);
		if (CompareDoubles(det, 0.0, EPSILON))
			return Matrix3x3();

		return Adjoint(m) * (1.0f / static_cast<float>(det));
	}


#if defined(_DEBUG)
	inline void print(const Matrix3x3& m)
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				std::cout << m(i, j) << "\t";
			}

			std::cout << std::endl;
		}
	}
#endif

//------------------------------------------------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------------------------------------------------
	/** @class Matrix4x4 ""
	*	@brief A matrix class used for 4x4 matrices and their manipulations.
	*
	*	The datatype for the components is float.\n
	*	The 4x4 matrix is treated as a row-major matrix.
	*
	*/
	class Matrix4x4
	{
	public:

		/**@brief Creates a new 4x4 identity matrix.
		*/
		Matrix4x4();

		/**@brief Creates a new 4x4 matrix with elements initialized to the given 2D array.
		*
		*	If \a a isn't a 4x4 matrix, the behavior is undefined.
		*/
		Matrix4x4(float a[][4]);

		/**@brief Creates a new 4x4 matrix with each row being set to the specified rows.
		*/
		Matrix4x4(const Vector4D& r1, const Vector4D& r2, const Vector4D& r3, const Vector4D& r4);

		/**@brief Creates a new 4x4 matrix with the first two values of the first two rows being set to the values of the 2x2 matrix.
		*
		* The last two values of the first two rows are set to (0, 0).
		* The values of the 3rd row is set to (0, 0, 1, 0).
		* The values of the 4th row is set to (0, 0, 0, 1).
		*/
		Matrix4x4(const Matrix2x2& m);

		/**@brief Creates a new 4x4 matrix with the first three values of the first three rows being set to the values of the 3x3 matrix.
		*
		* The last values of the first three rows are set to 0.
		* The values of the 4th row is set to (0, 0, 0, 1).
		*/
		Matrix4x4(const Matrix3x3& m);

		/**@brief Sets the first two values of the first two rows to the values of the 2x2 matrix.
		*
		* The last two values of the first two rows are set to (0, 0).
		* The values of the 3rd row is set to (0, 0, 1, 0).
		* The values of the 4th row is set to (0, 0, 0, 1).
		*/
		Matrix4x4& operator=(const Matrix2x2& m);

		/**@brief Sets the first three values of the first three rows to the values of the 3x3 matrix.
		*
		* The last values of the first three rows are set to 0.
		* The values of the 4th row is set to (0, 0, 0, 1).
		*/
		Matrix4x4& operator=(const Matrix3x3& m);

		/**@brief Returns a pointer to the first element in the matrix.
		*/
		float* Data();

		/**@brief Returns a constant pointer to the first element in the matrix.
		*/
		const float* Data() const;

		/**@brief Returns a constant reference to the element at the given (\a row, \a col).
		*
		* The \a row and \a col values should be between [0,3]. If any of them are out of that range, the first element will be returned.
		*/
		const float& operator()(unsigned int row, unsigned int col) const;

		/**@brief Returns a reference to the element at the given (\a row, \a col).
		*
		* The \a row and \a col values should be between [0,3]. If any of them are out of that range, the first element will be returned.
		*/
		float& operator()(unsigned int row, unsigned int col);

		/**@brief Returns the specified \a row.
		*
		*  \a Row should be between [0,3]. If it is out of range the first row will be returned.
		*/
		Vector4D GetRow(unsigned int row) const;

		/**@brief Returns the specified \a col.
		*
		* \a Col should be between [0,3]. If it is out of range the first col will be returned.
		*/
		Vector4D GetCol(unsigned int col) const;

		/**@brief Sets each element in the given \a row to the components of vector \a v.
		*
		* \a Row should be between [0,3]. If it is out of range the first row will be set.
		*/
		void SetRow(unsigned int row, Vector4D v);

		/**@brief Sets each element in the given \a col to the components of vector \a v.
		*
		* \a Col should be between [0,3]. If it is out of range the first col will be set.
		*/
		void SetCol(unsigned int col, Vector4D v);

		/**@brief Adds this 4x4 matrix with given matrix \a m and stores the result in this 4x4 matrix.
		*/
		Matrix4x4& operator+=(const Matrix4x4& m);

		/**@brief Subtracts \a m from this 4x4 matrix stores the result in this 4x4 matrix.
		*/
		Matrix4x4& operator-=(const Matrix4x4& m);

		/**@brief Multiplies this 4x4 matrix with \a k and stores the result in this 4x4 matrix.
		*/
		Matrix4x4& operator*=(float k);

		/**@brief Multiplies this 4x4 matrix with given matrix \a m and stores the result in this 4x4 matrix.
		*/
		Matrix4x4& operator*=(const Matrix4x4& m);

	private:

		float mMat[4][4];
	};

	//-------------------------------------------------------------------------------------
	inline Matrix4x4::Matrix4x4()
	{
		//1st row
		mMat[0][0] = 1.0f;
		mMat[0][1] = 0.0f;
		mMat[0][2] = 0.0f;
		mMat[0][3] = 0.0f;

		//2nd
		mMat[1][0] = 0.0f;
		mMat[1][1] = 1.0f;
		mMat[1][2] = 0.0f;
		mMat[1][3] = 0.0f;

		//3rd row
		mMat[2][0] = 0.0f;
		mMat[2][1] = 0.0f;
		mMat[2][2] = 1.0f;
		mMat[2][3] = 0.0f;

		//4th row
		mMat[3][0] = 0.0f;
		mMat[3][1] = 0.0f;
		mMat[3][2] = 0.0f;
		mMat[3][3] = 1.0f;
	}



	inline Matrix4x4::Matrix4x4(float a[][4])
	{
		//1st row
		mMat[0][0] = a[0][0];
		mMat[0][1] = a[0][1];
		mMat[0][2] = a[0][2];
		mMat[0][3] = a[0][3];

		//2nd
		mMat[1][0] = a[1][0];
		mMat[1][1] = a[1][1];
		mMat[1][2] = a[1][2];
		mMat[1][3] = a[1][3];

		//3rd row
		mMat[2][0] = a[2][0];
		mMat[2][1] = a[2][1];
		mMat[2][2] = a[2][2];
		mMat[2][3] = a[2][3];

		//4th row
		mMat[3][0] = a[3][0];
		mMat[3][1] = a[3][1];
		mMat[3][2] = a[3][2];
		mMat[3][3] = a[3][3];
	}

	inline Matrix4x4::Matrix4x4(const Vector4D& r1, const Vector4D& r2, const Vector4D& r3, const Vector4D& r4)
	{
		SetRow(0, r1);
		SetRow(1, r2);
		SetRow(2, r3);
		SetRow(3, r4);
	}

	inline float* Matrix4x4::Data()
	{
		return mMat[0];
	}

	inline const float* Matrix4x4::Data() const
	{
		return mMat[0];
	}

	inline const float& Matrix4x4::operator()(unsigned int row, unsigned int col) const
	{
		if (row > 3 || col > 3)
		{
			return mMat[0][0];
		}
		else
		{
			return mMat[row][col];
		}
	}

	inline float& Matrix4x4::operator()(unsigned int row, unsigned int col)
	{
		if (row > 3 || col > 3)
		{
			return mMat[0][0];
		}
		else
		{
			return mMat[row][col];
		}
	}

	inline Vector4D Matrix4x4::GetRow(unsigned int row) const
	{
		if (row < 0 || row > 3)
			return Vector4D{ mMat[0][0], mMat[0][1], mMat[0][2], mMat[0][3] };
		else
			return Vector4D{ mMat[row][0], mMat[row][1], mMat[row][2], mMat[row][3] };

	}

	inline Vector4D Matrix4x4::GetCol(unsigned int col) const
	{
		if (col < 0 || col > 3)
			return Vector4D{ mMat[0][0], mMat[1][0], mMat[2][0], mMat[3][0] };
		else
			return Vector4D{ mMat[0][col], mMat[1][col], mMat[2][col], mMat[3][col] };
	}

	inline void Matrix4x4::SetRow(unsigned int row, Vector4D v)
	{
		if (row > 3)
		{
			mMat[0][0] = v.x;
			mMat[0][1] = v.y;
			mMat[0][2] = v.z;
			mMat[0][3] = v.w;
		}
		else
		{
			mMat[row][0] = v.x;
			mMat[row][1] = v.y;
			mMat[row][2] = v.z;
			mMat[row][3] = v.w;
		}
	}

	inline void Matrix4x4::SetCol(unsigned int col, Vector4D v)
	{
		if (col > 3)
		{
			mMat[0][0] = v.x;
			mMat[1][0] = v.y;
			mMat[2][0] = v.z;
			mMat[3][0] = v.w;
		}
		else
		{
			mMat[0][col] = v.x;
			mMat[1][col] = v.y;
			mMat[2][col] = v.z;
			mMat[3][col] = v.w;
		}
	}

	inline Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& m)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				this->mMat[i][j] += m.mMat[i][j];
			}
		}

		return *this;
	}

	inline Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& m)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				this->mMat[i][j] -= m.mMat[i][j];
			}
		}

		return *this;
	}

	inline Matrix4x4& Matrix4x4::operator*=(float k)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				this->mMat[i][j] *= k;
			}
		}

		return *this;
	}

	inline Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& m)
	{
		Matrix4x4 result;

		for (int i = 0; i < 4; ++i)
		{
			result.mMat[i][0] =
				(mMat[i][0] * m.mMat[0][0]) +
				(mMat[i][1] * m.mMat[1][0]) +
				(mMat[i][2] * m.mMat[2][0]) +
				(mMat[i][3] * m.mMat[3][0]);

			result.mMat[i][1] =
				(mMat[i][0] * m.mMat[0][1]) +
				(mMat[i][1] * m.mMat[1][1]) +
				(mMat[i][2] * m.mMat[2][1]) +
				(mMat[i][3] * m.mMat[3][1]);

			result.mMat[i][2] =
				(mMat[i][0] * m.mMat[0][2]) +
				(mMat[i][1] * m.mMat[1][2]) +
				(mMat[i][2] * m.mMat[2][2]) +
				(mMat[i][3] * m.mMat[3][2]);

			result.mMat[i][3] =
				(mMat[i][0] * m.mMat[0][3]) +
				(mMat[i][1] * m.mMat[1][3]) +
				(mMat[i][2] * m.mMat[2][3]) +
				(mMat[i][3] * m.mMat[3][3]);
		}

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				mMat[i][j] = result.mMat[i][j];
			}
		}

		return *this;
	}

	/**@brief Adds \a m1 with \a m2 and returns the result.
	*/
	inline Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2)
	{
		Matrix4x4 result;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				result(i, j) = m1(i, j) + m2(i, j);
			}
		}

		return result;
	}

	/**@brief Negates the 4x4 matrix \a m.
	*/
	inline Matrix4x4 operator-(const Matrix4x4& m)
	{
		Matrix4x4 result;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				result(i, j) = -m(i, j);
			}
		}

		return result;
	}

	/**@brief Subtracts \a m2 from \a m1 and returns the result.
	*/
	inline Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2)
	{
		Matrix4x4 result;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				result(i, j) = m1(i, j) - m2(i, j);
			}
		}

		return result;
	}

	/**@brief Multiplies \a m with \a k and returns the result.
	*/
	inline Matrix4x4 operator*(const Matrix4x4& m, const float& k)
	{
		Matrix4x4 result;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				result(i, j) = m(i, j) * k;
			}
		}

		return result;
	}

	/**@brief Multiplies \a k with \m and returns the result.
	*/
	inline Matrix4x4 operator*(const float& k, const Matrix4x4& m)
	{
		Matrix4x4 result;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				result(i, j) = k * m(i, j);
			}
		}

		return result;
	}

	/**@brief Multiplies \a m1 with \m2 and returns the result.
	*
	* Does \a m1 * \a m2 in that order.
	*/
	inline Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2)
	{
		Matrix4x4 result;

		for (int i = 0; i < 4; ++i)
		{
			result(i, 0) =
				(m1(i, 0) * m2(0, 0)) +
				(m1(i, 1) * m2(1, 0)) +
				(m1(i, 2) * m2(2, 0)) +
				(m1(i, 3) * m2(3, 0));

			result(i, 1) =
				(m1(i, 0) * m2(0, 1)) +
				(m1(i, 1) * m2(1, 1)) +
				(m1(i, 2) * m2(2, 1)) +
				(m1(i, 3) * m2(3, 1));

			result(i, 2) =
				(m1(i, 0) * m2(0, 2)) +
				(m1(i, 1) * m2(1, 2)) +
				(m1(i, 2) * m2(2, 2)) +
				(m1(i, 3) * m2(3, 2));

			result(i, 3) =
				(m1(i, 0) * m2(0, 3)) +
				(m1(i, 1) * m2(1, 3)) +
				(m1(i, 2) * m2(2, 3)) +
				(m1(i, 3) * m2(3, 3));
		}

		return result;
	}

	/**@brief Multiplies \a m with \a v and returns the result.
	*
	* The vector \a v is a column vector.
	*/
	inline Vector4D operator*(const Matrix4x4& m, const Vector4D& v)
	{
		Vector4D result;

		result.x = m(0, 0) * v.x + m(0, 1) * v.y + m(0, 2) * v.z + m(0, 3) * v.w;

		result.y = m(1, 0) * v.x + m(1, 1) * v.y + m(1, 2) * v.z + m(1, 3) * v.w;

		result.z = m(2, 0) * v.x + m(2, 1) * v.y + m(2, 2) * v.z + m(2, 3) * v.w;

		result.w = m(3, 0) * v.x + m(3, 1) * v.y + m(3, 2) * v.z + m(3, 3) * v.w;

		return result;
	}

	/**@brief Multiplies \a v with \a m and returns the result.
	*
	* The vector \a v is a row vector.
	*/
	inline Vector4D operator*(const Vector4D& v, const Matrix4x4& m)
	{
		Vector4D result;

		result.x = v.x * m(0, 0) + v.y * m(1, 0) + v.z * m(2, 0) + v.w * m(3, 0);

		result.y = v.x * m(0, 1) + v.y * m(1, 1) + v.z * m(2, 1) + v.w * m(3, 1);

		result.z = v.x * m(0, 2) + v.y * m(1, 2) + v.z * m(2, 2) + v.w * m(3, 2);

		result.w = v.x * m(0, 3) + v.y * m(1, 3) + v.z * m(2, 3) + v.w * m(3, 3);

		return result;
	}

	/**@brief Sets \a m to the identity matrix.
	*/
	inline void SetToIdentity(Matrix4x4& m)
	{
		//set to identity matrix by setting the diagonals to 1.0f and all other elements to 0.0f

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

	/**@brief Returns true if \a m is the identity matrix, false otherwise.
	*/
	inline bool Identity(const Matrix4x4& m)
	{
		//Is the identity matrix if the diagonals are equal to 1.0f and all other elements equals to 0.0f

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				if (i == j)
				{
					if (!CompareFloats(m(i, j), 1.0f, EPSILON))
						return false;

				}
				else
				{
					if (!CompareFloats(m(i, j), 0.0f, EPSILON))
						return false;

				}
			}
		}
	}

	/**@brief Returns the tranpose of the given matrix \a m.
	*/
	inline Matrix4x4 Transpose(const Matrix4x4& m)
	{
		//make the rows into cols

		Matrix4x4 result;

		//1st col = 1st row
		result(0, 0) = m(0, 0);
		result(1, 0) = m(0, 1);
		result(2, 0) = m(0, 2);
		result(3, 0) = m(0, 3);

		//2nd col = 2nd row
		result(0, 1) = m(1, 0);
		result(1, 1) = m(1, 1);
		result(2, 1) = m(1, 2);
		result(3, 1) = m(1, 3);

		//3rd col = 3rd row
		result(0, 2) = m(2, 0);
		result(1, 2) = m(2, 1);
		result(2, 2) = m(2, 2);
		result(3, 2) = m(2, 3);

		//4th col = 4th row
		result(0, 3) = m(3, 0);
		result(1, 3) = m(3, 1);
		result(2, 3) = m(3, 2);
		result(3, 3) = m(3, 3);

		return result;
	}

	/**@brief Returns a 4x4 translation matrix.
	*/
	inline Matrix4x4 Translate(float x, float y, float z)
	{
		//1 0 0 0
		//0 1 0 0
		//0 0 1 0
		//x y z 1

		Matrix4x4 translate;
		translate(3, 0) = x;
		translate(3, 1) = y;
		translate(3, 2) = z;

		return translate;
	}

	/**@brief Returns a 4x4 translation matrix.
	*/
	inline Matrix4x4 Translate(const Vector3D& translateVector)
	{
		//1 0 0 0
		//0 1 0 0
		//0 0 1 0
		//x y z 1

		Matrix4x4 translate;
		translate(3, 0) = translateVector.x;
		translate(3, 1) = translateVector.y;
		translate(3, 2) = translateVector.z;

		return translate;
	}

	/**@brief Returns a 4x4 scale matrix.
	*/
	inline Matrix4x4 Scale4x4(float x, float y, float z)
	{
		//x 0 0 0
		//0 y 0 0
		//0 0 z 0
		//0 0 0 1

		Matrix4x4 scale;
		scale(0, 0) = x;
		scale(1, 1) = y;
		scale(2, 2) = z;

		return scale;
	}

	/**@brief Returns a 4x4 scale matrix.
	*/
	inline Matrix4x4 Scale4x4(const Vector3D& scaleVector)
	{
		//x 0 0 0
		//0 y 0 0
		//0 0 z 0
		//0 0 0 1

		Matrix4x4 scale;
		scale(0, 0) = scaleVector.x;
		scale(1, 1) = scaleVector.y;
		scale(2, 2) = scaleVector.z;

		return scale;
	}

	/**@brief Returns a 4x4 rotation matrix about the given axis.
	*/
	inline Matrix4x4 Rotate4x4(float angle, float x, float y, float z)
	{
		//c + (1 - c)x^2	(1 - c)xy + sz	(1 - c)xz - sy	0
		//(1 - c)xy - sz	c + (1 - c)y^2	(1 - c)yz + sx	0
		//(1 - c)xz + sy	(1 - c)yz - sx	c + (1 - c)z^2	0
		//0					0				0				1
		//c = cos(angle)
		//s = sin(angle)

		Vector3D axis{ x, y, z };

		axis = Normalize(axis);

		x = axis.x;
		y = axis.y;
		z = axis.z;

		float c = cos(angle * PI / 180.0f);
		float s = sin(angle * PI / 180.0f);
		float oneMinusC = 1 - c;

		Matrix4x4 result;

		//1st row
		result(0, 0) = c + (oneMinusC * (x * x));
		result(0, 1) = (oneMinusC * (x * y)) + (s * z);
		result(0, 2) = (oneMinusC * (x * z)) - (s * y);

		//2nd row
		result(1, 0) = (oneMinusC * (x * y)) - (s * z);
		result(1, 1) = c + (oneMinusC * (y * y));
		result(1, 2) = (oneMinusC * (y * z)) + (s * x);

		//3rd row
		result(2, 0) = (oneMinusC * (x * z)) + (s * y);
		result(2, 1) = (oneMinusC * (y * z)) - (s * x);
		result(2, 2) = c + (oneMinusC * (z * z));

		return result;
	}

	/**@brief Returns a 4x4 rotation matrix about the given axis.
	*/
	inline Matrix4x4 Rotate4x4(float angle, const Vector3D& axis)
	{
		//c + (1 - c)x^2	(1 - c)xy + sz	(1 - c)xz - sy	0
		//(1 - c)xy - sz	c + (1 - c)y^2	(1 - c)yz + sx	0
		//(1 - c)xz + sy	(1 - c)yz - sx	c + (1 - c)z^2	0
		//0					0				0				1
		//c = cos(angle)
		//s = sin(angle)

		Vector3D nAxis(Normalize(axis));

		float x = nAxis.x;
		float y = nAxis.y;
		float z = nAxis.z;

		float c = cos(angle * PI / 180.0f);
		float s = sin(angle * PI / 180.0f);
		float oneMinusC = 1 - c;

		Matrix4x4 result;

		//1st row
		result(0, 0) = c + (oneMinusC * (x * x));
		result(0, 1) = (oneMinusC * (x * y)) + (s * z);
		result(0, 2) = (oneMinusC * (x * z)) - (s * y);

		//2nd row
		result(1, 0) = (oneMinusC * (x * y)) - (s * z);
		result(1, 1) = c + (oneMinusC * (y * y));
		result(1, 2) = (oneMinusC * (y * z)) + (s * x);

		//3rd row
		result(2, 0) = (oneMinusC * (x * z)) + (s * y);
		result(2, 1) = (oneMinusC * (y * z)) - (s * x);
		result(2, 2) = c + (oneMinusC * (z * z));

		return result;
	}

	/**@brief Returns the determinant \a m.
	*/
	inline double Determinant(const Matrix4x4& m)
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

	/**@brief Returns the cofactor of the \a row and \a col in \a m.
	*/
	inline double Cofactor(const Matrix4x4& m, unsigned int row, unsigned int col)
	{
		//cij = (-1)^i + j * det of minor(i, j);
		Matrix3x3 minor;
		int r{ 0 };
		int c{ 0 };

		//minor(i, j)
		for (int i = 0; i < 4; ++i)
		{
			if (i == row)
				continue;

			for (int j = 0; j < 4; ++j)
			{
				if (j == col)
					continue;

				minor(r, c) = m(i, j);
				++c;

			}
			c = 0;
			++r;
		}


		return pow(-1, row + col) * Determinant(minor);
	}

	/**@brief Returns the adjoint of \a m.
	*/
	inline Matrix4x4 Adjoint(const Matrix4x4& m)
	{
		//Cofactor of each ijth position put into matrix cA.
		//Adjoint is the tranposed matrix of cA.
		Matrix4x4 cofactorMatrix;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				cofactorMatrix(i, j) = static_cast<float>(Cofactor(m, i, j));
			}
		}

		return Transpose(cofactorMatrix);
	}

	/**@brief Returns the inverse of \a m.
	*
	* If \a m is noninvertible/singular, the identity matrix is returned.
	*/
	inline Matrix4x4 Inverse(const Matrix4x4& m)
	{
		//Inverse of m = adjoint of m / det of m
		double det = Determinant(m);
		if (CompareDoubles(det, 0.0, EPSILON))
			return Matrix4x4();

		return Adjoint(m) * (1.0f / static_cast<float>(det));
	}


#if defined(_DEBUG)
	inline void print(const Matrix4x4& m)
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

//------------------------------------------------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------------------------------------------------
//QUATERNION

	/** @struct Quaternion ""
	*	@brief A quaternion struct used for quaternions and their manipulations.
	*
	*	The datatype for the components is float. When making an object of this struct, the quaternion is initialized to the identity quaternion.
	*/
	struct Quaternion
	{
		float scalar = 1.0f;
		Vector3D vector;
	};

	/**@brief Adds the quaternion \a q1 to the quaternion \a q2 and stores the result in \a q1.
	*/
	inline void operator+=(Quaternion& q1, const Quaternion& q2)
	{
		q1.scalar += q2.scalar;
		q1.vector += q2.vector;
	}

	/**@brief Subtracts the quaternion \a q2 from the quaternion \a q1 and stores the result in \a q1.
	* 
	* q1 - q2
	*/
	inline void operator-=(Quaternion& q1, const Quaternion& q2)
	{
		q1.scalar -= q2.scalar;
		q1.vector -= q2.vector;
	}

	/**@brief Multiplies the quaternion \a q1 by the scalar (float) k and stores the result in \a q1.
	*/
	inline void operator*=(Quaternion& q1, float k)
	{
		q1.scalar *= k;
		q1.vector *= k;
	}

	/**@brief Multiplies the quaternion \a q1 by the quaternion \a q1 and stores the result in \a q1.
	*/
	inline void operator*=(Quaternion& q1, const Quaternion& q2)
	{
		//q1q2 = [w1, v1][w2, v2] = [w1w2 - v1 dot v2, w1v2 + w2v1 + v1 x v2]
		//w is the scalar component and v is the vector component

		q1.scalar = q1.scalar * q2.scalar - DotProduct(q1.vector, q2.vector);
		q1.vector = q1.scalar * q2.vector + q2.scalar * q1.vector + CrossProduct(q1.vector, q2.vector);
	}

	/**@brief Adds the quaternion \a q1 to the quaternion \a q2 and returns the result.
	*/
	inline Quaternion operator+(const Quaternion& q1, const Quaternion& q2)
	{
		return Quaternion{ q1.scalar + q2.scalar, q1.vector + q2.vector };
	}

	/**@brief Subtracts the quaternion \a q2 from the quaternion \a q1 and returns the result.
	* 
	* Returns q1 - q2.
	*/
	inline Quaternion operator-(const Quaternion& q1, const Quaternion& q2)
	{
		return Quaternion{ q1.scalar - q2.scalar, q1.vector - q2.vector };
	}

	/**@brief Negates the quaternion \a q1 and returns the result.
	*/
	inline Quaternion operator-(const Quaternion& q)
	{
		return Quaternion{ -q.scalar, -q.vector};
	}

	/**@brief Multiplies the quaternion \a q by the scalar (float) k and returns the result.
	*/
	inline Quaternion operator*(const Quaternion& q, float k)
	{
		return Quaternion{ q.scalar * k, q.vector * k };
	}

	/**@brief Multiplies the scalar (float) k by the quaternion \a q and returns the result.
	*/
	inline Quaternion operator*(float k, const Quaternion& q)
	{
		return Quaternion{ k * q.scalar, k * q.vector };
	}

	/**@brief Multiplies the quaternion \a q1 by the quaternion \a q2 and returns the result.
	*/
	inline Quaternion operator*(const Quaternion& q1, const Quaternion& q2)
	{
		//q1q2 = [w1, v1][w2, v2] = [w1w2 - v1 dot v2, w1v2 + w2v1 + v1 x v2]
		//w is the scalar component and v is the vector component

		float scalarResult{ 0.0f };
		Vector3D vectorResult;

		scalarResult = q1.scalar * q2.scalar - DotProduct(q1.vector, q2.vector);
		vectorResult = q1.scalar * q2.vector + q2.scalar * q1.vector + CrossProduct(q1.vector, q2.vector);

		return Quaternion{ scalarResult, vectorResult };
	}

	/**@brief Returns true if the quaternion \a q1 equals to the quaternion \a q2, false otherwise.
	*/
	inline bool operator==(const Quaternion& q1, const Quaternion& q2)
	{
		return CompareFloats(q1.scalar, q2.scalar, EPSILON) && (q1.vector == q2.vector);
	}

	/**@brief Returns true if the quaternion \a q1 does not equal to the quaternion \a q2, false otherwise.
	*/
	inline bool operator!=(const Quaternion& q1, const Quaternion& q2)
	{
		return !(q1 == q2);
	}

	/**@brief Returns true if quaternion \a q is a zero quaternion, false otherwise.
	*/
	inline bool ZeroQuaternion(const Quaternion& q)
	{
		//zero quaternion = (0, 0, 0, 0)
		return CompareFloats(q.scalar, 0.0f, EPSILON) && ZeroVector(q.vector);
	}

	/**@brief Returns true if quaternion \a q is an identity quaternion, false otherwise.
	*/
	inline bool Identity(const Quaternion& q)
	{
		//identity quaternion = (1, 0, 0, 0)
		return CompareFloats(q.scalar, 1.0f, EPSILON) && CompareFloats(q.vector.x, 0.0f, EPSILON) &&
			CompareFloats(q.vector.y, 0.0f, EPSILON) && CompareFloats(q.vector.z, 0.0f, EPSILON);
	}

	/**@brief Returns the conjugate of quaternion \a q.
	*/
	inline Quaternion Conjugate(const Quaternion& q)
	{
		//conjugate of a quaternion is the quaternion with its vector part negated
		return Quaternion{ q.scalar, -q.vector };
	}

	/**@brief Returns the length of quaternion \a q.
	*/
	inline float Length(const Quaternion& q)
	{
		//length of a quaternion = sqrt(scalar^2 + x^2 + y^2 + z^2)
		return sqrt(q.scalar * q.scalar + q.vector.x * q.vector.x + q.vector.y * q.vector.y + q.vector.z * q.vector.z);
	}

	/**@brief Normalizes the quaternion \a q and returns the normalized quaternion.
	* 
	* If q is the zero quaternion, q is returned.
	*/
	inline Quaternion Normalize(const Quaternion& q)
	{
		//to normalize a quaternion you do q / |q|

		if (ZeroQuaternion(q))
			return q;

		float inverseMagnitdue{ 1.0f / Length(q) };

		return Quaternion{ q.scalar * inverseMagnitdue, q.vector * inverseMagnitdue };
	}

	/**@brief Returns the invese of the quaternion \a q.
	*
	*  If \a q is the zero quaternion then \a q is returned.
	*/
	inline Quaternion Inverse(const Quaternion& q)
	{
		//inverse = conjugate of q / |q|

		if (ZeroQuaternion(q))
			return q;

		float inverseMagnitdue{ 1.0f / Length(q) };

		return Quaternion{ Conjugate(q) * inverseMagnitdue };
	}

	/**@brief Returns a rotation quaternion from the axis-angle representation.
	*
	*  The \a angle should be given in degrees.
	*/
	inline Quaternion RotationQuaternion(float angle, float x, float y, float z)
	{
		//A roatation quaternion is a quaternion where the
		//scalar part = cos(theta / 2)
		//vector part = sin(theta / 2) * axis
		//the axis needs to be normalized

		float ang{ angle / 2.0f };
		float c{ cos(ang * PI / 180.0f) };
		float s{ sin(ang * PI / 180.0f) };

		Vector3D axis{ x, y, z };
		axis = Normalize(axis);

		return Quaternion{ c, s * axis };
	}

	/**@brief Returns a quaternion from the axis-angle representation.
	*
	* The \a angle should be given in degrees.
	*/
	inline Quaternion RotationQuaternion(float angle, const Vector3D& axis)
	{
		//A roatation quaternion is a quaternion where the
		//scalar part = cos(theta / 2)
		//vector part = sin(theta / 2) * axis
		//the axis needs to be normalized

		float ang{ angle / 2.0f };
		float c{ cos(ang * PI / 180.0f) };
		float s{ sin(ang * PI / 180.0f) };

		Vector3D axisN(Normalize(axis));

		return Quaternion{ c, s * axisN };
	}

	/**@brief Returns a quaternion from the axis-angle  representation.
	*
	* The x value in the 4D vector \a v should be the angle(in degrees).\n
	* The y, z and w values in the 4D vector \a v should be the axis.
	*/
	inline Quaternion RotationQuaternion(const Vector4D& angAxis)
	{
		//A roatation quaternion is a quaternion where the
		//scalar part = cos(theta / 2)
		//vector part = sin(theta / 2) * axis
		//the axis needs to be normalized

		float angle{ angAxis.x / 2.0f };
		float c{ cos(angle * PI / 180.0f) };
		float s{ sin(angle * PI / 180.0f) };

		Vector3D axis{ angAxis.y, angAxis.z, angAxis.w };
		axis = Normalize(axis);

		return Quaternion{ c, s * axis };
	}

	/**@brief Rotates the specified point/vector \a p using the quaternion \a q
	*
	* \a q should be a rotation quaternion.
	*/
	inline Vector3D Rotate(const Quaternion& q, const Vector3D& p)
	{
		//To rotate a point/vector using quaternions you do qpq*, where p = (0, x, y, z) is the point/vector in quaternion from, q is a rotation quaternion
		//and q* is its conjugate.

		Quaternion point{ 0.0f, p };

		Quaternion result(q * point * Conjugate(q));

		//The rotated vector/point is in the vector component of the quaternion.
		return result.vector;
	}

	/**@brief Rotates the specified point/vector \a p using the quaternion \a q
	*
	* \a q should be a rotation quaternion.
	*/
	inline Vector4D Rotate(const Quaternion& q, const Vector4D& p)
	{
		//To rotate a point/vector using quaternions you do qpq*, where p = (0, x, y, z) is the point/vector, q is a rotation quaternion
		//and q* is its conjugate.

		Quaternion point{ 0.0f, p.x, p.y, p.z };

		Quaternion result(q * point * Conjugate(q));

		//The rotated vector/point is in the vector component of the quaternion.
		return Vector4D{ result.vector.x, result.vector.y, result.vector.z, p.w };
	}

	/**@brief Transforms \a q into a column-major matrix.
	*
	* \a q should be a rotation quaternion.
	*/
	inline Matrix3x3 QuaternionToRotationMatrixCol3x3(const Quaternion& q)
	{
		//1 - 2q3^2 - 2q4^2		2q2q3 - 2q1q4		2q2q4 + 2q1q3
		//2q2q3 + 2q1q4			1 - 2q2^2 - 2q4^2	2q3q4 - 2q1q2
		//2q2q4 - 2q1q3			2q3q4 + 2q1q2		1 - 2q2^2 - 2q3^2
		//q1 = scalar
		//q2 = x
		//q3 = y
		//q4 = z

		Matrix3x3 colMat;

		colMat(0, 0) = 1.0f - 2.0f * q.vector.y * q.vector.y - 2.0f * q.vector.z * q.vector.z;
		colMat(0, 1) = 2.0f * q.vector.x * q.vector.y - 2.0f * q.scalar * q.vector.z;
		colMat(0, 2) = 2.0f * q.vector.x * q.vector.z + 2.0f * q.scalar * q.vector.y;

		colMat(1, 0) = 2.0f * q.vector.x * q.vector.y + 2.0f * q.scalar * q.vector.z;
		colMat(1, 1) = 1.0f - 2.0f * q.vector.x * q.vector.x - 2.0f * q.vector.z * q.vector.z;
		colMat(1, 2) = 2.0f * q.vector.y * q.vector.z - 2.0f * q.scalar * q.vector.x;

		colMat(2, 0) = 2.0f * q.vector.x * q.vector.z - 2.0f * q.scalar * q.vector.y;
		colMat(2, 1) = 2.0f * q.vector.y * q.vector.z + 2.0f * q.scalar * q.vector.x;
		colMat(2, 2) = 1.0f - 2.0f * q.vector.x * q.vector.x - 2.0f * q.vector.y * q.vector.y;

		return colMat;
	}

	/**@brief Transforms \a q into a row-major matrix.
	*
	* \a q should be a unit quaternion.
	*/
	inline Matrix3x3 QuaternionToRotationMatrixRow3x3(const Quaternion& q)
	{
		//1 - 2q3^2 - 2q4^2		2q2q3 + 2q1q4		2q2q4 - 2q1q3
		//2q2q3 - 2q1q4			1 - 2q2^2 - 2q4^2	2q3q4 + 2q1q2
		//2q2q4 + 2q1q3			2q3q4 - 2q1q2		1 - 2q2^2 - 2q3^2
		//q1 = scalar
		//q2 = x
		//q3 = y
		//q4 = z

		Matrix3x3 rowMat;

		rowMat(0, 0) = 1.0f - 2.0f * q.vector.y * q.vector.y - 2.0f * q.vector.z * q.vector.z;
		rowMat(0, 1) = 2.0f * q.vector.x * q.vector.y + 2.0f * q.scalar * q.vector.z;
		rowMat(0, 2) = 2.0f * q.vector.x * q.vector.z - 2.0f * q.scalar * q.vector.y;

		rowMat(1, 0) = 2.0f * q.vector.x * q.vector.y - 2.0f * q.scalar * q.vector.z;
		rowMat(1, 1) = 1.0f - 2.0f * q.vector.x * q.vector.x - 2.0f * q.vector.z * q.vector.z;
		rowMat(1, 2) = 2.0f * q.vector.y * q.vector.z + 2.0f * q.scalar * q.vector.x;

		rowMat(2, 0) = 2.0f * q.vector.x * q.vector.z + 2.0f * q.scalar * q.vector.y;
		rowMat(2, 1) = 2.0f * q.vector.y * q.vector.z - 2.0f * q.scalar * q.vector.x;
		rowMat(2, 2) = 1.0f - 2.0f * q.vector.x * q.vector.x - 2.0f * q.vector.y * q.vector.y;

		return rowMat;
	}

	/**@brief Transforms \a q into a column-major matrix.
	*
	* \a q should be a rotation quaternion.
	*/
	inline Matrix4x4 QuaternionToRotationMatrixCol4x4(const Quaternion& q)
	{
		//1 - 2q3^2 - 2q4^2		2q2q3 - 2q1q4		2q2q4 + 2q1q3		0
		//2q2q3 + 2q1q4			1 - 2q2^2 - 2q4^2	2q3q4 - 2q1q2		0
		//2q2q4 - 2q1q3			2q3q4 + 2q1q2		1 - 2q2^2 - 2q3^2	0
		//0						0					0					1
		//q1 = scalar
		//q2 = x
		//q3 = y
		//q4 = z

		Matrix4x4 colMat;

		colMat(0, 0) = 1.0f - 2.0f * q.vector.y * q.vector.y - 2.0f * q.vector.z * q.vector.z;
		colMat(0, 1) = 2.0f * q.vector.x * q.vector.y - 2.0f * q.scalar * q.vector.z;
		colMat(0, 2) = 2.0f * q.vector.x * q.vector.z + 2.0f * q.scalar * q.vector.y;

		colMat(1, 0) = 2.0f * q.vector.x * q.vector.y + 2.0f * q.scalar * q.vector.z;
		colMat(1, 1) = 1.0f - 2.0f * q.vector.x * q.vector.x - 2.0f * q.vector.z * q.vector.z;
		colMat(1, 2) = 2.0f * q.vector.y * q.vector.z - 2.0f * q.scalar * q.vector.x;

		colMat(2, 0) = 2.0f * q.vector.x * q.vector.z - 2.0f * q.scalar * q.vector.y;
		colMat(2, 1) = 2.0f * q.vector.y * q.vector.z + 2.0f * q.scalar * q.vector.x;
		colMat(2, 2) = 1.0f - 2.0f * q.vector.x * q.vector.x - 2.0f * q.vector.y * q.vector.y;

		return colMat;
	}

	/**@brief Transforms \a q into a row-major matrix.
	*
	* \a q should be a unit quaternion.
	*/
	inline Matrix4x4 QuaternionToRotationMatrixRow4x4(const Quaternion& q)
	{
		//1 - 2q3^2 - 2q4^2		2q2q3 + 2q1q4		2q2q4 - 2q1q3		0
		//2q2q3 - 2q1q4			1 - 2q2^2 - 2q4^2	2q3q4 + 2q1q2		0
		//2q2q4 + 2q1q3			2q3q4 - 2q1q2		1 - 2q2^2 - 2q3^2	0
		//0						0					0					1
		//q1 = scalar
		//q2 = x
		//q3 = y
		//q4 = z

		Matrix4x4 rowMat;

		rowMat(0, 0) = 1.0f - 2.0f * q.vector.y * q.vector.y - 2.0f * q.vector.z * q.vector.z;
		rowMat(0, 1) = 2.0f * q.vector.x * q.vector.y + 2.0f * q.scalar * q.vector.z;
		rowMat(0, 2) = 2.0f * q.vector.x * q.vector.z - 2.0f * q.scalar * q.vector.y;

		rowMat(1, 0) = 2.0f * q.vector.x * q.vector.y - 2.0f * q.scalar * q.vector.z;
		rowMat(1, 1) = 1.0f - 2.0f * q.vector.x * q.vector.x - 2.0f * q.vector.z * q.vector.z;
		rowMat(1, 2) = 2.0f * q.vector.y * q.vector.z + 2.0f * q.scalar * q.vector.x;

		rowMat(2, 0) = 2.0f * q.vector.x * q.vector.z + 2.0f * q.scalar * q.vector.y;
		rowMat(2, 1) = 2.0f * q.vector.y * q.vector.z - 2.0f * q.scalar * q.vector.x;
		rowMat(2, 2) = 1.0f - 2.0f * q.vector.x * q.vector.x - 2.0f * q.vector.y * q.vector.y;

		return rowMat;
	}

	/**@brief Returns the dot product of the quaternions \a q1 and \a q2.
	*/
	inline float DotProduct(const Quaternion& q1, const Quaternion& q2)
	{
		//q1 dot q2 = [w1, v1] dot [w2, v2] = w1w2 + v1 dot v2
		//w is the scalar component and v is the vector component.

		return q1.scalar * q2.scalar + DotProduct(q1.vector, q2.vector);
	}

	/**@brief Linear Interpolates between quaternions \a q1 and \a q2.
	*
	*  If t is not between 0 and 1, it gets clamped.
	*/
	inline Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t)
	{
		if (t < 0.0f)
			return q1;
		else if (t > 1.0f)
			return q2;

		//Compute the cosine of the angle between the quaternions
		float cosOmega = DotProduct(q1, q2);

		Quaternion newQ2;
		//If the dot product is negative, negate q2 to so we take the shorter arc
		if (cosOmega < 0.0f)
		{
			newQ2 = -q2;
			cosOmega = -cosOmega;
		}
		else
		{
			newQ2 = q2;
		}

		return (1.0f - t) * q1 + t * newQ2;
	}

	/**@brief Linear Interpolates between quaternions \a q1 and \a q2 and normalizes the result.
	*
	*  If t is not between 0 and 1, it gets clamped.
	*/
	inline Quaternion NLerp(const Quaternion& q1, const Quaternion& q2, float t)
	{
		if (t < 0.0f)
			return q1;
		else if (t > 1.0f)
			return q2;

		//Compute the cosine of the angle between the quaternions
		float cosOmega = DotProduct(q1, q2);

		Quaternion newQ2;
		//If the dot product is negative, negate q2 to so we take the shorter arc
		if (cosOmega < 0.0f)
		{
			newQ2 = -q2;
			cosOmega = -cosOmega;
		}
		else
		{
			newQ2 = q2;
		}

		return Normalize((1.0f - t) * q1 + t * newQ2);
	}

	/**@brief Spherical Linear Interpolates between quaternions \a q1 and \a q2.
	*
	*  If t is not between 0 and 1, it gets clamped.
	*/
	inline Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t)
	{
		//Formula used is
		//k1 = sin((1 - t)omega) * omega) / sin(omega);
		//k2 = (sin(tomega) * omega) / sin(omega)
		//newQ = k1q1 * k2q2
		//Omega is the angle between the q0 and q1.

		if (t < 0.0f)
			return q1;
		else if (t > 1.0f)
			return q2;

		//Compute the cosine of the angle between the quaternions
		float cosOmega = DotProduct(q1, q2);

		Quaternion newQ2;
		//If the dot product is negative, negate q2 to so we take the shorter arc
		if (cosOmega < 0.0f)
		{
			newQ2 = -q2;
			cosOmega = -cosOmega;
		}
		else
		{
			newQ2 = q2;
		}

		float k1{ 0.0f };
		float k2{ 0.0f };

		//Linear interpolate if the quaternions are very close to protect dividing by zero.
		if (cosOmega > 0.9999f)
		{
			k1 = 1.0f - t;
			k2 = t;
		}
		else
		{
			//sin of the angle between the quaternions is
			//sin(omega) = 1 - cos^2(omega) from the trig identity
			//sin^2(omega) + cos^2(omega) = 1.
			float sinOmega{ sqrt(1.0f - cosOmega * cosOmega) };

			//retrieve the angle
			float omega{ atan2(sinOmega, cosOmega) };

			//Compute inverse to avoid dividng multiple times
			float oneOverSinOmega{ 1.0f / sinOmega };

			k1 = sin((1.0f - t) * omega) * oneOverSinOmega;
			k2 = sin(t * omega) * oneOverSinOmega;
		}

		return k1 * q1 + k2 * newQ2;
	}

#if defined(_DEBUG)
	inline void print(const Quaternion& q)
	{
		std::cout << "(" << q.scalar << ", " << q.vector.x << ", " << q.vector.y << ", " << q.vector.z << ")";
	}
#endif

//------------------------------------------------------------------------------------------------------------------------------------
}

typedef MathEngine::Vector2D vec2;
typedef MathEngine::Vector3D vec3;
typedef MathEngine::Vector4D vec4;
typedef MathEngine::Matrix2x2 mat2;
typedef MathEngine::Matrix3x3 mat3;
typedef MathEngine::Matrix4x4 mat4;
typedef MathEngine::Quaternion quaternion;
