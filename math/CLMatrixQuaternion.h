//
//  CLMatrixQuaternion.h
//  Chloe
//
//  Copyright 2015 by Überpixel. All rights reserved.
//  Unauthorized use is punishable by torture, mutilation, and vivisection.
//

#ifndef __CHLOE_MATRIXQUATERNION_H__
#define __CHLOE_MATRIXQUATERNION_H__

#include "CLVector.h"
#include "CLSIMD.h"

namespace CL
{
	class Quaternion;
	class alignas(16) Matrix
	{
	public:
		inline Matrix();

		inline bool operator== (const Matrix &other) const;
		inline bool operator!= (const Matrix &other) const;

		inline Matrix &operator*= (const Matrix &other);
		inline Matrix operator* (const Matrix &other) const;
		inline Vector3 operator* (const Vector3 &other) const;
		inline Vector4 operator* (const Vector4 &other) const;

		inline static Matrix WithIdentity();
		inline static Matrix WithTranslation(const Vector3 &translation);
		inline static Matrix WithTranslation(const Vector4 &translation);
		inline static Matrix WithScaling(const Vector3 &scaling);
		inline static Matrix WithScaling(const Vector4 &scaling);
		inline static Matrix WithRotation(const Vector3 &rotation);
		inline static Matrix WithRotation(const Vector4 &rotation);
		inline static Matrix WithRotation(const Quaternion &rotation);

		inline static Matrix WithProjectionOrthogonal(float left, float right, float bottom, float top, float clipnear, float clipfar);
		inline static Matrix WithProjectionPerspective(float arc, float aspect, float clipnear, float clipfar);
		inline static Matrix WithInverseProjectionPerspective(float arc, float aspect, float clipnear, float clipfar);

		inline float GetDeterminant() const;

		inline Vector3 GetEulerAngle() const;
		inline Vector4 GetAxisAngle() const;
		inline Quaternion GetQuaternion() const;

		inline void Translate(const Vector3 &translation);
		inline void Translate(const Vector4 &translation);
		inline void Scale(const Vector3 &scaling);
		inline void Scale(const Vector4 &scaling);
		inline void Rotate(const Vector3 &rotation);
		inline void Rotate(const Vector4 &rotation);
		inline void Rotate(const Quaternion &rotation);

		inline void Transpose();
		inline Matrix GetTransposed() const;
		inline void Inverse();
		inline Matrix GetInverse() const;

		inline bool IsEqual(const Matrix &other, float epsilon) const;
	
#if RN_SIMD
		void *operator new[](size_t size) { return Memory::AllocateSIMD(size); }
		void operator delete[](void *ptr) { if(ptr) Memory::FreeSIMD(ptr); }
		
		union
		{
			float m[16];
			SIMD::VecFloat vec[4];
		};
#else
		float m[16];
#endif
	private:
		inline float GetSubmatrixDeterminant(const int k) const;
		
	};

	class Quaternion
	{
	public:
		inline Quaternion();
		inline Quaternion(float x, float y, float z, float w);
		inline Quaternion(const Vector3 &euler);
		inline Quaternion(const Vector4 &axis);

		inline bool operator== (const Quaternion &other) const;
		inline bool operator!= (const Quaternion &other) const;

		inline Quaternion &operator+= (const Quaternion &other);
		inline Quaternion &operator-= (const Quaternion &other);
		inline Quaternion &operator*= (const Quaternion &other);
		inline Quaternion &operator/= (const Quaternion &other);
		inline Quaternion &operator*= (float scalar);
		inline Quaternion &operator/= (float scalar);

		inline Quaternion operator+ (const Quaternion &other) const;
		inline Quaternion operator- (const Quaternion &other) const;
		inline Quaternion operator* (const Quaternion &other) const;
		inline Quaternion operator/ (const Quaternion &other) const;
		inline Quaternion operator* (float scalar) const;
		inline Quaternion operator/ (float scalar) const;

		inline Quaternion &operator+= (const Vector3 &other);
		inline Quaternion &operator-= (const Vector3 &other);
		inline Quaternion operator+ (const Vector3 &other) const;
		inline Quaternion operator- (const Vector3 &other) const;

		inline static Quaternion WithIdentity();
		inline static Quaternion WithEulerAngle(const Vector3 &euler);
		inline static Quaternion WithAxisAngle(const Vector4 &euler);
		inline static Quaternion WithLerpSpherical(const Quaternion &start, const Quaternion &end, float factor);
		inline static Quaternion WithLerpLinear(const Quaternion &start, const Quaternion &end, float factor);
		inline static Quaternion WithLookAt(const Vector3 &dir, const Vector3 &up=Vector3(0.0f, 1.0f, 0.0f), bool forceup=false);

		inline Quaternion &Normalize();
		inline Quaternion GetNormalized() const;
		inline Quaternion &Conjugate();
		inline Quaternion GetConjugated() const;

		inline Quaternion GetLerpSpherical(const Quaternion &other, float factor) const;
		inline Quaternion GetLerpLinear(const Quaternion &other, float factor) const;

		inline Vector3 GetRotatedVector(const Vector3 &vec) const;
		inline Vector4 GetRotatedVector(const Vector4 &vec) const;

		inline Matrix GetRotationMatrix() const;

		inline Vector3 GetEulerAngle() const;
		inline Vector4 GetAxisAngle() const;

		inline float GetLength() const;
		inline float GetDotProduct(const Quaternion &other) const;

		inline bool IsEqual(const Quaternion &other, float epsilon) const;

		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
	};
}

#endif /* __CHLOE_MATRIXQUATERNION_H__ */
