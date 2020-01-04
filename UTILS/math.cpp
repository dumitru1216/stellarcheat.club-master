#include "../includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/CGlobalVars.h"

#include "qangle.h"
#include "math.h"

namespace MATH
{
	void angle_vectors_cus(const Vector& angles, Vector& forward)
	{
		Assert(s_bMathlibInitialized);
		Assert(forward);

		float sp, sy, cp, cy;

		sy = sin(DEG2RAD(angles[1]));
		cy = cos(DEG2RAD(angles[1]));

		sp = sin(DEG2RAD(angles[0]));
		cp = cos(DEG2RAD(angles[0]));

		forward.x = cp * cy;
		forward.y = cp * sy;
		forward.z = -sp;
	}

	/*inline Vector CalcAngle(const Vector& src, const Vector& dst)
	{
		Vector ret;
		VectorAngles(dst - src, ret);
		return ret;
	}*/
	void VectorAngles(const Vector& forward, QAngle& angles)
	{
		/*if (forward[1] == 0.0f && forward[0] == 0.0f)
		{
			angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f;
			angles[1] = 0.0f;
		}
		else
		{
			angles[0] = atan2(-forward[2], forward.Length2D()) * -180 / PI;
			angles[1] = atan2(forward[1], forward[0]) * 180 / PI;

			if (angles[1] > 90) angles[1] -= 180;
			else if (angles[1] < 90) angles[1] += 180;
			else if (angles[1] == 90) angles[1] = 0;
		}

		angles[2] = 0.0f;*/

		if (forward.y == 0 && forward.x == 0) {
			angles.pitch = forward.z > 0.f ? 270.f : 90.f;
			angles.yaw = 0;
		}

		else {
			angles.pitch = (atan2(-forward.z, forward.Length2D()) * 180 / M_PI);
			if (angles.pitch < 0)
				angles.pitch += 360;

			angles.yaw = (atan2(forward.y, forward.x) * 180 / M_PI);
			if (angles.yaw < 0)
				angles.yaw += 360;
		}

		angles.roll = 0;
	}
	float NormalizeFloat(float angle)
	{
		auto revolutions = angle / 360.f;
		if (angle > 180.f || angle < -180.f) {
			revolutions = round(abs(revolutions));
			if (angle < 0.f)
				angle = (angle + 360.f * revolutions);
			else
				angle = (angle - 360.f * revolutions);
			return angle;
		}
		return angle;
	}

	QAngle CalcAngleQ(Vector src, Vector dst)
	{
		QAngle angles;
		Vector delta = dst - src;

		VectorAngles(delta, angles);

		angles.Normalize();

		return angles;
	}
	void calculate_angle(Vector src, Vector dst, Vector &angles)
	{
		Vector delta = src - dst;
		double hyp = delta.Length2D();
		angles.y = (atan(delta.y / delta.x) * 57.295779513082f);
		angles.x = (atan(delta.z / hyp) * 57.295779513082f);
		angles[2] = 0.0f;
		if (delta.x >= 0.0) angles.y += 180.0f;
	}
	void AngleVectorsQ(QAngle q, Vector &vec)
	{
		float sp, sy, cp, cy;

		SinCos(DEG2RAD(q[1]), &sy, &cy);
		SinCos(DEG2RAD(q[0]), &sp, &cp);

		vec.x = cp * cy;
		vec.y = cp * sy;
		vec.z = -sp;
	}
	void VectorAngles(const Vector& forward, Vector &angles)
	{
		float tmp, yaw, pitch;

		if (forward[1] == 0 && forward[0] == 0)
		{
			yaw = 0;
			if (forward[2] > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else
		{
			yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
			if (yaw < 0)
				yaw += 360;

			tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
			pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
			if (pitch < 0)
				pitch += 360;
		}

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0;
	}
	void VectorAngles2(const Vector& forward, Vector &angles)
	{
		if (forward.x == 0.f && forward.y == 0.f)
		{
			angles.x = forward.z > 0.f ? -90.f : 90.f;
			angles.y = 0.f;
		}
		else
		{
			angles.x = RAD2DEG(atan2(-forward.z, forward.Length2D()));
			angles.y = RAD2DEG(atan2(forward.y, forward.x));
		}

		angles.z = 0.f;
	}
	float NormalizeYawInPlace(float f)
	{
		if (std::isnan(f) || std::isinf(f))
			f = 0;

		if (f > 9999999)
			f = 0;

		if (f < -9999999)
			f = 0;

		while (f < -180.0f) f += 360.0f;
		while (f > 180.0f) f -= 360.0f;

		return f;
	}
	void VectorAngles3(const Vector& forward, QAngle &angles)
	{
		float	tmp, yaw, pitch;

		if (forward[1] == 0 && forward[0] == 0)
		{
			yaw = 0;
			if (forward[2] > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else
		{
			yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
			if (yaw < 0)
				yaw += 360;

			tmp = sqrtf(forward[0] * forward[0] + forward[1] * forward[1]);
			pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
			if (pitch < 0)
				pitch += 360;
		}

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0;
	}
	Vector MATH::CalcAngle(const Vector& vecSource, const Vector& vecDestination)
	{
		Vector qAngles;
		Vector delta = Vector((vecSource[0] - vecDestination[0]), (vecSource[1] - vecDestination[1]), (vecSource[2] - vecDestination[2]));
		double hyp = sqrtf(delta[0] * delta[0] + delta[1] * delta[1]);
		qAngles[0] = (float)(atan(delta[2] / hyp) * (180.0 / M_PI));
		qAngles[1] = (float)(atan(delta[1] / delta[0]) * (180.0 / M_PI));
		qAngles[2] = 0.f;
		if (delta[0] >= 0.f)
			qAngles[1] += 180.f;

		return qAngles;
	}
	float MATH::GRD_TO_BOG(float GRD) {
		return (M_PI / 180) * GRD;
	}
	void rotate_point(Vector2D& point, Vector2D origin, bool clockwise, float angle)
	{
		Vector2D delta = point - origin;
		Vector2D rotated(delta.x * cos(angle) - delta.y * sin(angle), delta.x * sin(angle) + delta.y * cos(angle));

		if (clockwise)
		{
			rotated = Vector2D(delta.x * cos(angle) - delta.y * sin(angle), delta.x * sin(angle) + delta.y * cos(angle));
		}
		else
		{
			rotated = Vector2D(delta.x * sin(angle) - delta.y * cos(angle), delta.x * cos(angle) + delta.y * sin(angle));
		}

		point = rotated + origin;
	}
	/*inline void VectorCrossProduct(const Vector& a, const Vector& b, Vector& result)
	{
		CHECK_VALID(a);
		CHECK_VALID(b);

		Assert(&a != &result);
		Assert(&b != &result);

		result.x = a.y*b.z - a.z*b.y;
		result.y = a.z*b.x - a.x*b.z;
		result.z = a.x*b.y - a.y*b.x;
	}*/
	void CrossProduct(const float* v1, const float* v2, float* cross)
	{
		cross[0] = v1[1] * v2[2] - v1[2] * v2[1];
		cross[1] = v1[2] * v2[0] - v1[0] * v2[2];
		cross[2] = v1[0] * v2[1] - v1[1] * v2[0];
	}
	Vector NormalizeAngle(Vector angle)
	{
		while (angle.x > 89.f)
		{
			angle.x -= 180.f;
		}
		while (angle.x < -89.f)
		{
			angle.x += 180.f;
		}
		if (angle.y > 180)
		{
			angle.y -= (round(angle.y / 360) * 360.f);
		}
		else if (angle.y < -180)
		{
			angle.y += (round(angle.y / 360) * -360.f);
		}
		if ((angle.z > 50) || (angle.z < 50))
		{
			angle.z = 0;
		}
		return angle;
	}
	float NormalizeYaw(float yaw)
	{
		if (yaw > 180)
		{
			yaw -= (round(yaw / 360) * 360.f);
		}
		else if (yaw < -180)
		{
			yaw += (round(yaw / 360) * -360.f);
		}
		return yaw;
	}
	float YawDistance(float firstangle, float secondangle)
	{
		if (firstangle == secondangle)
			return 0.f;

		bool oppositeSides = false;

		if (firstangle > 0 && secondangle < 0)
			oppositeSides = true;
		else if (firstangle < 0 && secondangle > 0)
			oppositeSides = true;

		if (!oppositeSides)
			return fabs(firstangle - secondangle);

		bool past90 = false;

		if (firstangle > 90 && secondangle < -90)
		{
			firstangle -= (firstangle - 90);
			secondangle += (secondangle + 90);
		}
		else if (firstangle < -90 && secondangle > 90)
		{
			firstangle += (firstangle + 90);
			secondangle -= (secondangle - 90);
		}

		float oneTwo;

		oneTwo = fabs(firstangle - secondangle);

		return oneTwo;
	}
	void NormalizeNum(Vector &vIn, Vector &vOut)
	{
		float flLen = vIn.Length();
		if (flLen == 0) {
			vOut.Init(0, 0, 1);
			return;
		}
		flLen = 1 / flLen;
		vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
	}
	void inline SinCos(float radians, float *sine, float *cosine)
	{
		*sine = sin(radians);
		*cosine = cos(radians);

	}
	void VectorSubtract(const Vector& a, const Vector& b, Vector& c)
	{
		c.x = a.x - b.x;
		c.y = a.y - b.y;
		c.z = a.z - b.z;
	}
	inline void VectorCrossProduct(const Vector& a, const Vector& b, Vector& result)

	{

		CHECK_VALID(a);

		CHECK_VALID(b);

		Assert(&a != &result);

		Assert(&b != &result);

		result.x = a.y*b.z - a.z*b.y;

		result.y = a.z*b.x - a.x*b.z;

		result.z = a.x*b.y - a.y*b.x;

	}
	void AngleVectors(const Vector &angles, Vector *forward)
	{
		float	sp, sy, cp, cy;

		SinCos(DEG2RAD(angles[1]), &sy, &cy);
		SinCos(DEG2RAD(angles[0]), &sp, &cp);

		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}
	void AngleVectors3(const Vector &angles, Vector forward)
	{
		float sp, sy, cp, cy;

		sy = sin(DEG2RAD(angles[1]));
		cy = cos(DEG2RAD(angles[1]));

		sp = sin(DEG2RAD(angles[0]));
		cp = cos(DEG2RAD(angles[0]));

		forward.x = cp * cy;
		forward.y = cp * sy;
		forward.z = -sp;
	}
	__forceinline float DotProduct(const float *a, const float *b)
	{
		return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	}
	Vector RotateVectorYaw(Vector origin, float angle, Vector point)
	{
		float s;
		float c;
		SinCos(DEG2RAD(angle), &s, &c);

		// translate point back to origin:
		point.x -= origin.x;
		point.y -= origin.y;

		// rotate point
		float xnew = point.x * c - point.y * s;
		float ynew = point.x * s + point.y * c;

		// translate point back:
		point.x = xnew + origin.x;
		point.y = ynew + origin.y;
		return point;
	}
	void VectorTransform(const float *in1, const matrix3x4_t& in2, float *out)
	{
		out[0] = DotProduct(in1, in2[0]) + in2[0][3];
		out[1] = DotProduct(in1, in2[1]) + in2[1][3];
		out[2] = DotProduct(in1, in2[2]) + in2[2][3];
	}
	void VectorTransform(const Vector& in1, const matrix3x4_t& in2, Vector& out)
	{
		VectorTransform(&in1.x, in2, &out.x);
	}
	void VectorTransform(const float *in1, const VMatrix& in2, float *out)
	{
		out[0] = DotProduct(in1, in2[0]) + in2[0][3];
		out[1] = DotProduct(in1, in2[1]) + in2[1][3];
		out[2] = DotProduct(in1, in2[2]) + in2[2][3];
	}
	void VectorTransform(const Vector& in1, const VMatrix& in2, Vector& out)
	{
		VectorTransform(&in1.x, in2, &out.x);
	}
	Vector VectorTransformTest(const Vector& in1, const matrix3x4_t& in2)
	{
		return Vector(in1.Dot(in2.m_flMatVal[0]) + in2.m_flMatVal[0][3],
			in1.Dot(in2.m_flMatVal[1]) + in2.m_flMatVal[1][3],
			in1.Dot(in2.m_flMatVal[2]) + in2.m_flMatVal[2][3]);
	}
	void AngleVectorsA4(const Vector &angles, Vector *forward, Vector *right, Vector *up)
	{
		float sr, sp, sy, cr, cp, cy;

		SinCos(DEG2RAD(angles[1]), &sy, &cy);
		SinCos(DEG2RAD(angles[0]), &sp, &cp);
		SinCos(DEG2RAD(angles[2]), &sr, &cr);

		if (forward)
		{
			forward->x = cp*cy;
			forward->y = cp*sy;
			forward->z = -sp;
		}

		if (right)
		{
			right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
			right->y = (-1 * sr*sp*sy + -1 * cr*cy);
			right->z = -1 * sr*cp;
		}

		if (up)
		{
			up->x = (cr*sp*cy + -sr*-sy);
			up->y = (cr*sp*sy + -sr*cy);
			up->z = cr*cp;
		}
	}
	float CalcAngle2D(Vector2D src, Vector2D dst)
	{
		float angle;
		VectorAngle2D(dst - src, angle);
		return angle;
	}
	void VectorAngle2D(const Vector2D &direction, float &angle)
	{
		angle = RAD2DEG(std::atan2(direction.y, direction.x)) + 90.f;
	}
	void AngleVectors2D(float angle, Vector2D &forward)
	{
		angle = DEG2RAD(angle);
		Vector2D slope(sin(angle), -cos(angle));
		forward = slope;
	}
	inline Vector ExtrapolateTick(Vector p0, Vector v0)
	{
		return p0 + (v0 * INTERFACES::Globals->interval_per_tick);
	}
	/*
	void draw_angle_line2d(const Vector2D center, const float yaw, const float
		line_distance_from_center, const float line_length, const CColor line_color)
	{
		const auto start_position = get_rotated_screen_position(center, yaw, line_distance_from_center);
		const auto end_position = get_rotated_screen_position(center, yaw, line_distance_from_center + line_length);

		IRender->DrawLine(start_position.x, start_position.y, end_position.x, end_position.y, line_color);
	}
	Vector2D get_rotated_screen_position(Vector2D center, float yaw, float distance)
	{
		Vector2D slope = { sin(DEG2RAD(yaw)), -cos(DEG2RAD(yaw)) }; // x = run, y = rise
		Vector2D direction = slope;
		return center + (slope * distance);
	}
	*/
	void AngleVectors2(const Vector &angles, Vector *forward, Vector *right, Vector *up)
	{
		static const constexpr auto PIRAD = 0.01745329251f;
		float sr, sp, sy, cr, cp, cy;

		sp = static_cast<float>(sin(double(angles.x) * PIRAD));
		cp = static_cast<float>(cos(double(angles.x) * PIRAD));
		sy = static_cast<float>(sin(double(angles.y) * PIRAD));
		cy = static_cast<float>(cos(double(angles.y) * PIRAD));
		sr = static_cast<float>(sin(double(angles.z) * PIRAD));
		cr = static_cast<float>(cos(double(angles.z) * PIRAD));

		if (forward)
		{
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}

		if (right)
		{
			right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
			right->y = (-1 * sr*sp*sy + -1 * cr*cy);
			right->z = -1 * sr*cp;
		}

		if (up)
		{
			up->x = (cr*sp*cy + -sr * -sy);
			up->y = (cr*sp*sy + -sr * cy);
			up->z = cr * cp;
		}
	}
	float NormalizeAngle(float angle)
	{
		if (angle > 180.f || angle < -180.f)
		{
			auto revolutions = round(abs(angle / 360.f));

			if (angle < 0.f)
				angle = angle + 360.f * revolutions;
			else
				angle = angle - 360.f * revolutions;
		} return angle;
	}
}