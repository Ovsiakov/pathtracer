#pragma once
#include <array>
#include <cmath>
#include <iostream>

#define M_PI 3.14159265358979323846

float toRadians(float degrees);

class SphericalVector;

class Vector2 {
	public:
		float x = 0.0f;
		float y = 0.0f;
		Vector2() {
		}
		;
		Vector2(float x, float y) :
				x(x), y(y) {
		}
		;
};

class Vector3 {
	public:
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		Vector3& normalize(); //return this
		Vector3& negate();
		Vector3& scale(float);
		float length() const;

		Vector3();
		Vector3(float x, float y, float z);
		Vector3(const SphericalVector&);

		static float dot(Vector3, Vector3);
		static Vector3 cross(Vector3, Vector3);
		static float distance(Vector3, Vector3);
		Vector3 operator-(Vector3) const;
		Vector3 operator+(Vector3) const;
		float operator [](int i) const;
		float* operator [](int i);
		Vector3 copy() const;

		const Vector3 operator=(const Vector3&);
		bool isZero() const;
		friend std::ostream& operator<<(std::ostream&, const Vector3&);
};

class SphericalVector {
	public:
		float r = 0.0f;
		float phi = 0.0f;
		float fi = 0.0f;

		SphericalVector(const Vector3&);
		friend std::ostream& operator<<(std::ostream&, const SphericalVector&);
};

Vector3 operator*(float, Vector3);
Vector3 operator*(Vector3, float);

class Vector4 {
	public:
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float w = 0.0f;

		Vector4();
		Vector4(float x, float y, float z, float w);
		Vector4(Vector3 v, float w);

		const Vector4 operator=(const Vector4&);
		operator Vector3() const;

		Vector4 copy();
};

class Matrix4x4 {
	private:
		std::array<float, 16> data { };
		Matrix4x4() {
		}
		;

	public:
		static Matrix4x4 zeroMatrix();
		static Matrix4x4 identityMatrix();
		static Matrix4x4 translationMatrix(Vector3);
		static Matrix4x4 rotationMatrixXYZ(float x, float y, float z);
		static Matrix4x4 projection(float left, float right, float bottom, float top, float near, float far);
		static Matrix4x4 projection(float fovy, float aspectRatio, float near, float far);
		static Matrix4x4 lookAt(const Vector3& eye, const Vector3& center, const Vector3& upDir);

		static Matrix4x4 multiply(const Matrix4x4&, const Matrix4x4&);
		static Vector4 multiply(const Matrix4x4&, const Vector4&);
		static Vector4 multiply(const Matrix4x4&, const Vector3&, float w);
		static Vector4 multiply(const Matrix4x4&, float x, float y, float z, float w);

		float det();
		Matrix4x4 inverse();
		Matrix4x4 transpose();
		void scale(float factor);

		std::array<float, 16> getData();
		const std::array<float, 16> getData() const;
		Matrix4x4 operator*(const Matrix4x4&);
		Vector4 operator*(const Vector4&);

};

