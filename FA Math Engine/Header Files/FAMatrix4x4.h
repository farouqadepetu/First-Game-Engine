#pragma once

#include "FAMathUtility.h"

/** @file FAMatrix4x4.h
*	@brief File has a 4x4 matrix class under the namespace FAMath.
*/


/** @namespace FAMath
*	@brief The name space has utility functions, Vector2D, Vector3D, Vector4D, Matrix4x4, and Quaternion classes.
*/
namespace FAMath
{
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

		/**@brief Default Constructor.
		*
		*	Creates a new 4x4 identity matrix.
		*/
		Matrix4x4();

		/**@brief Overloaded Constructor.
		*
		*	Creates a new 4x4 matrix with elements initialized to the given 2D array.\n
		*	If the passed in 2D array isn't a 4x4 matrix, the behavior is undefined.
		*/
		Matrix4x4(float a[][4]);

		/**@brief Returns a pointer to the first element in the matrix.
		*/
		float* data();

		/**@brief Returns a constant pointer to the first element in the matrix.
		*/
		const float* data() const;

		/**@brief Returns a constant reference to the element at the given (row, col).
		* The row and col values should be between [0,3]. If any of them are out of that range, the first element will be returned.
		*/
		const float& operator()(unsigned int row, unsigned int col) const;

		/**@brief Returns a reference to the element at the given (row, col).
		* The row and col values should be between [0,3]. If any of them are out of that range, the first element will be returned.
		*/
		float& operator()(unsigned int row, unsigned int col);

		/**@brief Sets each element in the given row to the components of vector v.
		* Row should be between [0,3]. If it out of range the first row will be set.
		*/
		void setRow(unsigned int row, Vector4D v);

		/**@brief Sets each element in the given col to the components of vector v.
		* Col should be between [0,3]. If it out of range the first col will be set.
		*/
		void setCol(unsigned int col, Vector4D v);

		/**@brief Adds this 4x4 matrix with given matrix m and stores the result in this 4x4 matrix.
		*/
		Matrix4x4& operator+=(const Matrix4x4& m);

		/**@brief Subtracts this 4x4 matrix with given matrix m and stores the result in this 4x4 matrix.
		*/
		Matrix4x4& operator-=(const Matrix4x4& m);

		/**@brief Multiplies this 4x4 matrix with given scalar k and stores the result in this 4x4 matrix.
		*/
		Matrix4x4& operator*=(const float& k);


		/**@brief Multiplies this 4x4 matrix with given matrix m and stores the result in this 4x4 matrix.
		*/
		Matrix4x4& operator*=(const Matrix4x4& m);

	private:

		float m_mat[4][4];
	};

	/**@brief Adds the two given 4x4 matrices and returns a Matrix4x4 object with the result.
	*/
	Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2);

	/**@brief Negates the 4x4 matrix m.
	*/
	Matrix4x4 operator-(const Matrix4x4& m);

	/**@brief Subtracts the two given 4x4 matrices and returns a Matrix4x4 object with the result.
	*/
	Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2);

	/**@brief Multiplies the given 4x4 matrix with the given scalar and returns a Matrix4x4 object with the result.
	*/
	Matrix4x4 operator*(const Matrix4x4& m, const float& k);

	/**@brief Multiplies the the given scalar with the given 4x4 matrix and returns a Matrix4x4 object with the result.
	*/
	Matrix4x4 operator*(const float& k, const Matrix4x4& m);

	/**@brief Multiplies the two given 4x4 matrices and returns a Matrix4x4 object with the result.
	*/
	Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);

	/**@brief Multiplies the given 4x4 matrix with the given 4D vector and returns a Vector4D object with the result.
	* The vector is a column vector.
	*/
	Vector4D operator*(const Matrix4x4& m, const Vector4D& v);

	/**@brief Multiplies the given 4D vector with the given 4x4 matrix and returns a Vector4D object with the result.
	* The vector is a row vector.
	*/
	Vector4D operator*(const Vector4D& a, const Matrix4x4& m);

	/**@brief Sets the given matrix to the identity matrix.
	*/
	void setToIdentity(Matrix4x4& m);

	/**@brief Returns true if the given matrix is the identity matrix, false otherwise.
	*/
	bool isIdentity(const Matrix4x4& m);

	/**@brief Returns the tranpose of the given matrix m.
	*/
	Matrix4x4 transpose(const Matrix4x4& m);

	/**@brief Construct a 4x4 translation matrix with the given floats and post-multiply's it by the given matrix.
	* cm = cm * translate
	*/
	Matrix4x4 translate(const Matrix4x4& cm, float x, float y, float z);

	/**@brief Construct a 4x4 scaling matrix with the given floats and post-multiply's it by the given matrix.
	* cm = cm * scale
	*/
	Matrix4x4 scale(const Matrix4x4& cm, float x, float y, float z);

	/**@brief Construct a 4x4 rotation matrix with the given angle (in degrees) and axis (x, y, z) and post-multiply's it by the given matrix.
	* cm = cm * rotate.\n
	*/
	Matrix4x4 rotate(const Matrix4x4& cm, float angle, float x, float y, float z);

	/**@brief Returns the determinant of the given matrix.
	*/
	double det(const Matrix4x4& m);

	/**@brief Returns the cofactor of the given row and col using the given matrix.
	*/
	double cofactor(const Matrix4x4& m, unsigned int row, unsigned int col);

	/**@brief Returns the adjoint of the given matrix.
	*/
	Matrix4x4 adjoint(const Matrix4x4& m);

	/**@brief Returns the inverse of the given matrix.
	* If the matrix is noninvertible/singular, the identity matrix is returned.
	*/
	Matrix4x4 inverse(const Matrix4x4& m);



#if defined(_DEBUG)
	void print(const Matrix4x4& m);
#endif
}