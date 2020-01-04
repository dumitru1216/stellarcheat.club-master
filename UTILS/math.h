#pragma once
#include "../UTILS/qangle.h"
#include <random>
#include <iostream>
#ifndef M_PI
#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h
#endif

#define M_PI_F		((float)(M_PI))	// Shouldn't collide with anything.
#ifndef RAD2DEG
#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / M_PI_F) )
#endif

#ifndef DEG2RAD
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) )
#endif

namespace MATH
{
	//inline Vector CalcAngle(const Vector& src, const Vector& dst);
	void angle_vectors_cus(const Vector& angles, Vector& forward);
	void AngleMatrix(const Vector angles, matrix3x4_t & matrix);
	void MatrixSetColumn(const Vector & in, int column, matrix3x4_t & out);
//	void AngleMatrix(const QAngle & angles, const Vector & position, matrix3x4_t & matrix_out);
	void VectorAngles(const Vector& forward, Vector &angles);
	void VectorAngles2(const Vector& forward, Vector& angles);
	Vector NormalizeAngle(Vector angle);
	Vector CalcAngle(const Vector& vecSource, const Vector& vecDestination);
	float NormalizeAngle(float angle);
	void NormalizeNum(Vector &vIn, Vector &vOut);
	void VectorSubtract(const Vector& a, const Vector& b, Vector& c);
	void AngleVectorsA4(const Vector &angles, Vector *forward, Vector *right, Vector *up);
	float NormalizeYaw(float yaw);
	//void VectorAngles3(const Vector& forward, QAngle &angles);
	float NormalizeYawInPlace(float f);
	float YawDistance(float firstangle, float secondangle);
	void inline SinCos(float radians, float *sine, float *cosine);
	void AngleVectors(const Vector &angles, Vector *forward);
	void AngleVectors3(const Vector &angles, Vector forward);
	__forceinline float DotProduct(const float *a, const float *b);
	void calculate_angle(Vector src, Vector dst, Vector & angles);
	void VectorTransform(const float *in1, const matrix3x4_t& in2, float *out);
	void VectorTransform(const Vector& in1, const matrix3x4_t& in2, Vector& out);
	void VectorTransform(const float *in1, const VMatrix& in2, float *out);
	QAngle CalcAngleQ(Vector src, Vector dst);
	Vector VectorTransformTest(const Vector& in1, const matrix3x4_t& in2);
	float NormalizeFloat(float angle);
	void VectorTransform(const Vector& in1, const VMatrix& in2, Vector& out);
	void rotate_point(Vector2D& point, Vector2D origin, bool clockwise, float angle);
	Vector RotateVectorYaw(Vector origin, float angle, Vector point);
	float CalcAngle2D(Vector2D src, Vector2D dst);
	//inline void VectorCrossProduct(const Vector& a, const Vector& b, Vector& result);
	void VectorAngle2D(const Vector2D &direction, float &angle);
	float GRD_TO_BOG(float GRD);
	void AngleVectors2D(float angle, Vector2D &forward);
	Vector inline  ExtrapolateTick(Vector p0, Vector v0);
	void AngleVectors2(const Vector &angles, Vector *forward, Vector *right, Vector *up);
	inline float RandomNumber(float min, float max)
	{
		static std::random_device rand_device;
		static std::mt19937 rand_gen(rand_device());

		std::uniform_real_distribution<float> rand_distributer(min, max);
		return rand_distributer(rand_gen);
	}

	inline float clamp_yaw(float yaw) {

		while (yaw > 180.f)
			yaw -= 360.f;

		while (yaw < -180.f)
			yaw += 360.f;

		return yaw;
	}
}