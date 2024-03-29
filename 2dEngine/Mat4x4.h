#pragma once

#include "Vector2.h"
#include "Utils.h"
#include <math.h>
#include "Vector4.h"

namespace eg
{
	struct Mat4x4
	{
		//					column			row
		//NOTE: Column major. 0    *   4  +   0 and column vectors, therefore post multiplication or pre multiplication with transpose
		float matrix[4 * 4] = { 0 };
	public:
		Mat4x4() = default;
		//col * 4 + row
		inline Mat4x4& operator*=(const Mat4x4& rhs)
		{
			Mat4x4 result;

			result.matrix[0 * 4 + 0] = matrix[0 * 4 + 0] * rhs.matrix[0 * 4 + 0] + matrix[1 * 4 + 0] * rhs.matrix[0 * 4 + 1] + matrix[2 * 4 + 0] * rhs.matrix[0 * 4 + 2] + matrix[3 * 4 + 0] * rhs.matrix[0 * 4 + 3];

			result.matrix[1 * 4 + 0] = matrix[0 * 4 + 0] * rhs.matrix[1 * 4 + 0] + matrix[1 * 4 + 0] * rhs.matrix[1 * 4 + 1] + matrix[2 * 4 + 0] * rhs.matrix[1 * 4 + 2] + matrix[3 * 4 + 0] * rhs.matrix[1 * 4 + 3];
			result.matrix[2 * 4 + 0] = matrix[0 * 4 + 0] * rhs.matrix[2 * 4 + 0] + matrix[1 * 4 + 0] * rhs.matrix[2 * 4 + 1] + matrix[2 * 4 + 0] * rhs.matrix[2 * 4 + 2] + matrix[3 * 4 + 0] * rhs.matrix[2 * 4 + 3];
			result.matrix[3 * 4 + 0] = matrix[0 * 4 + 0] * rhs.matrix[3 * 4 + 0] + matrix[1 * 4 + 0] * rhs.matrix[3 * 4 + 1] + matrix[2 * 4 + 0] * rhs.matrix[3 * 4 + 2] + matrix[3 * 4 + 0] * rhs.matrix[3 * 4 + 3];

			result.matrix[0 * 4 + 1] = matrix[0 * 4 + 1] * rhs.matrix[0 * 4 + 0] + matrix[1 * 4 + 1] * rhs.matrix[0 * 4 + 1] + matrix[2 * 4 + 1] * rhs.matrix[0 * 4 + 2] + matrix[3 * 4 + 1] * rhs.matrix[0 * 4 + 3];
			result.matrix[0 * 4 + 2] = matrix[0 * 4 + 2] * rhs.matrix[0 * 4 + 0] + matrix[1 * 4 + 2] * rhs.matrix[0 * 4 + 1] + matrix[2 * 4 + 2] * rhs.matrix[0 * 4 + 2] + matrix[3 * 4 + 2] * rhs.matrix[0 * 4 + 3];
			result.matrix[0 * 4 + 3] = matrix[0 * 4 + 3] * rhs.matrix[0 * 4 + 0] + matrix[1 * 4 + 3] * rhs.matrix[0 * 4 + 1] + matrix[2 * 4 + 3] * rhs.matrix[0 * 4 + 2] + matrix[3 * 4 + 3] * rhs.matrix[0 * 4 + 3];

			result.matrix[1 * 4 + 1] = matrix[0 * 4 + 1] * rhs.matrix[1 * 4 + 0] + matrix[1 * 4 + 1] * rhs.matrix[1 * 4 + 1] + matrix[2 * 4 + 1] * rhs.matrix[1 * 4 + 2] + matrix[3 * 4 + 1] * rhs.matrix[1 * 4 + 3];
			result.matrix[1 * 4 + 2] = matrix[0 * 4 + 2] * rhs.matrix[1 * 4 + 0] + matrix[1 * 4 + 2] * rhs.matrix[1 * 4 + 1] + matrix[2 * 4 + 2] * rhs.matrix[1 * 4 + 2] + matrix[3 * 4 + 2] * rhs.matrix[1 * 4 + 3];
			result.matrix[1 * 4 + 3] = matrix[0 * 4 + 3] * rhs.matrix[1 * 4 + 0] + matrix[1 * 4 + 3] * rhs.matrix[1 * 4 + 1] + matrix[2 * 4 + 3] * rhs.matrix[1 * 4 + 2] + matrix[3 * 4 + 3] * rhs.matrix[1 * 4 + 3];

			result.matrix[2 * 4 + 1] = matrix[0 * 4 + 1] * rhs.matrix[2 * 4 + 0] + matrix[1 * 4 + 1] * rhs.matrix[2 * 4 + 1] + matrix[2 * 4 + 1] * rhs.matrix[2 * 4 + 2] + matrix[3 * 4 + 1] * rhs.matrix[2 * 4 + 3];
			result.matrix[2 * 4 + 2] = matrix[0 * 4 + 2] * rhs.matrix[2 * 4 + 0] + matrix[1 * 4 + 2] * rhs.matrix[2 * 4 + 1] + matrix[2 * 4 + 2] * rhs.matrix[2 * 4 + 2] + matrix[3 * 4 + 2] * rhs.matrix[2 * 4 + 3];
			result.matrix[2 * 4 + 3] = matrix[0 * 4 + 3] * rhs.matrix[2 * 4 + 0] + matrix[1 * 4 + 3] * rhs.matrix[2 * 4 + 1] + matrix[2 * 4 + 3] * rhs.matrix[2 * 4 + 2] + matrix[3 * 4 + 3] * rhs.matrix[2 * 4 + 3];

			result.matrix[3 * 4 + 1] = matrix[0 * 4 + 1] * rhs.matrix[3 * 4 + 0] + matrix[1 * 4 + 1] * rhs.matrix[3 * 4 + 1] + matrix[2 * 4 + 1] * rhs.matrix[3 * 4 + 2] + matrix[3 * 4 + 1] * rhs.matrix[3 * 4 + 3];
			result.matrix[3 * 4 + 2] = matrix[0 * 4 + 2] * rhs.matrix[3 * 4 + 0] + matrix[1 * 4 + 2] * rhs.matrix[3 * 4 + 1] + matrix[2 * 4 + 2] * rhs.matrix[3 * 4 + 2] + matrix[3 * 4 + 2] * rhs.matrix[3 * 4 + 3];
			result.matrix[3 * 4 + 3] = matrix[0 * 4 + 3] * rhs.matrix[3 * 4 + 0] + matrix[1 * 4 + 3] * rhs.matrix[3 * 4 + 1] + matrix[2 * 4 + 3] * rhs.matrix[3 * 4 + 2] + matrix[3 * 4 + 3] * rhs.matrix[3 * 4 + 3];

			*this = result;

			return *this;
		}

		inline Mat4x4 operator*(const Mat4x4& rhs) const
		{
			Mat4x4 lhs = *this;
			lhs *= rhs;
			return lhs;
		}

		inline Vector2f operator*(const Vector2f& rhs) const
		{
			Vector2f result;

			result.x = matrix[0 * 4 + 0] * rhs.x + matrix[1 * 4 + 0] * rhs.y + matrix[3 * 4 + 0] * 1.0f;
			result.y = matrix[0 * 4 + 1] * rhs.x + matrix[1 * 4 + 1] * rhs.y + matrix[3 * 4 + 1] * 1.0f;

			return result;
		}

		inline Vector3f operator*(const Vector3f& rhs) const
		{
			Vector3f result;

			result.x = matrix[0 * 4 + 0] * rhs.x + matrix[1 * 4 + 0] * rhs.y + matrix[2 * 4 + 0] * rhs.z + matrix[3 * 4 + 0] * 1.0f;
			result.y = matrix[0 * 4 + 1] * rhs.x + matrix[1 * 4 + 1] * rhs.y + matrix[2 * 4 + 1] * rhs.z + matrix[3 * 4 + 1] * 1.0f;
			result.z = matrix[0 * 4 + 2] * rhs.x + matrix[1 * 4 + 2] * rhs.y + matrix[2 * 4 + 2] * rhs.z + matrix[3 * 4 + 2] * 1.0f;

			return result;
		}

		inline Vector4f operator*(const Vector4f& rhs) const
		{
			Vector4f result;

			result.x = getColumnVector(0).dotProduct(rhs);
			result.y = getColumnVector(1).dotProduct(rhs);
			result.z = getColumnVector(2).dotProduct(rhs);
			result.w = getColumnVector(3).dotProduct(rhs);

			return result;
		}

		Vector4f getColumnVector(uint32_t row) const
		{
			Vector4f result;

			result.x = matrix[0 * 4 + row];
			result.y = matrix[1 * 4 + row];
			result.z = matrix[2 * 4 + row];
			result.w = matrix[3 * 4 + row];

			return result;
		}

		void transpose()
		{
			float temp = matrix[0 * 4 + 1];
			matrix[0 * 4 + 1] = matrix[1 * 4 + 0];
			matrix[1 * 4 + 0] = temp;

			temp = matrix[0 * 4 + 2];
			matrix[0 * 4 + 2] = matrix[2 * 4 + 0];
			matrix[2 * 4 + 0] = temp;

			temp = matrix[0 * 4 + 3];
			matrix[0 * 4 + 3] = matrix[3 * 4 + 0];
			matrix[3 * 4 + 0] = temp;

			temp = matrix[1 * 4 + 2];
			matrix[1 * 4 + 2] = matrix[2 * 4 + 1];
			matrix[2 * 4 + 1] = temp;

			temp = matrix[1 * 4 + 3];
			matrix[1 * 4 + 3] = matrix[3 * 4 + 1];
			matrix[3 * 4 + 1] = temp;

			temp = matrix[2 * 4 + 3];
			matrix[2 * 4 + 3] = matrix[3 * 4 + 2];
			matrix[3 * 4 + 2] = temp;
		}

		Mat4x4 getTranspose() const
		{
			Mat4x4 result = *this;
			result.transpose();
			return result;
		}

		static Mat4x4 identity()
		{
			Mat4x4 result;

			result.matrix[0 * 4 + 0] = 1;
			result.matrix[1 * 4 + 1] = 1;
			result.matrix[2 * 4 + 2] = 1;
			result.matrix[3 * 4 + 3] = 1;

			return result;
		}

		static Mat4x4 translate(const Vector2f& trans)
		{
			Mat4x4 result = identity();

			result.matrix[3 * 4 + 0] = trans.x;
			result.matrix[3 * 4 + 1] = trans.y;

			return result;
		}

		static Mat4x4 translate(const Vector3f& trans)
		{
			Mat4x4 result = identity();

			result.matrix[3 * 4 + 0] = trans.x;
			result.matrix[3 * 4 + 1] = trans.y;
			result.matrix[3 * 4 + 2] = trans.z;

			return result;
		}

		static Mat4x4 scale(const Vector2f& scl)
		{
			Mat4x4 result = identity();

			result.matrix[0 * 4 + 0] = scl.x;
			result.matrix[1 * 4 + 1] = scl.y;

			return result;
		}

		static Mat4x4 scale(const Vector3f& scl)
		{
			Mat4x4 result = identity();

			result.matrix[0 * 4 + 0] = scl.x;
			result.matrix[1 * 4 + 1] = scl.y;
			result.matrix[2 * 4 + 2] = scl.z;

			return result;
		}

		static Mat4x4 rotate(const float rot)
		{
			Mat4x4 result = identity();

			if (rot != 0.0f)
			{
				float cosA = cosf(utils::degreesToRadians(rot));
				float sinA = sinf(utils::degreesToRadians(rot));

				Mat4x4 rot = identity();
				rot.matrix[0 * 4 + 0] = cosA;
				rot.matrix[1 * 4 + 0] = sinA;
				rot.matrix[0 * 4 + 1] = -sinA;
				rot.matrix[1 * 4 + 1] = cosA;

				result *= rot;
			}

			return result;
		}

		static Mat4x4 rotateX(float rot)
		{
			Mat4x4 result = identity();

			if (rot != 0.0f)
			{
				float cosA = cosf(utils::degreesToRadians(rot));
				float sinA = sinf(utils::degreesToRadians(rot));

				Mat4x4 rot = identity();
				rot.matrix[1 * 4 + 1] = cosA;
				rot.matrix[2 * 4 + 1] = sinA;
				rot.matrix[1 * 4 + 2] = -sinA;
				rot.matrix[2 * 4 + 2] = cosA;

				result *= rot;
			}

			return result;
		}

		static Mat4x4 rotateY(float rot)
		{
			Mat4x4 result = identity();

			if (rot != 0.0f)
			{
				float cosA = cosf(utils::degreesToRadians(rot));
				float sinA = sinf(utils::degreesToRadians(rot));

				Mat4x4 rot = identity();
				rot.matrix[0 * 4 + 0] = cosA;
				rot.matrix[0 * 4 + 2] = sinA;
				rot.matrix[2 * 4 + 0] = -sinA;
				rot.matrix[2 * 4 + 2] = cosA;

				result *= rot;
			}

			return result;
		}

		static Mat4x4 rotateZ(float rot)
		{
			return rotate(rot);
		}

		static Mat4x4 rot(const Vector3f& axis, float angle)
		{
			Mat4x4 result = identity();

			/*
			Vector3f v;
			Vector3f proj = v.proj(axis);

			Vector3f perp = v.perp(axis);
			Vector3f w = v.crossProduct(perp);

			Vector3f rotationPerp = perp * cosf(angle) + w * sinf(angle);
			Vector3f rotation = proj + rotationPerp;

			After multipling out and rearranging and creating a matrix based on that:
			*/
			float cosA = cosf(utils::degreesToRadians(angle));
			float sinA = cosf(utils::degreesToRadians(angle));

			result.matrix[0 * 4 + 0] = cosA + (1 - cosA) * (axis.x * axis.x);
			result.matrix[0 * 4 + 1] = (1 - cosA) * (axis.y * axis.x) + sinA * axis.z;
			result.matrix[0 * 4 + 2] = (1 - cosA) * (axis.z * axis.x) + sinA * (-axis.y);
			result.matrix[1 * 4 + 0] = (1 - cosA) * (axis.x * axis.y) + sinA * (-axis.z);
			result.matrix[1 * 4 + 1] = cosA + (1 - cosA) * (axis.y * axis.y);
			result.matrix[1 * 4 + 2] = (1 - cosA) * (axis.z * axis.y) + sinA * axis.x;
			result.matrix[2 * 4 + 0] = (1 - cosA) * (axis.x * axis.z) + sinA * axis.y;
			result.matrix[2 * 4 + 1] = (1 - cosA) * (axis.y * axis.z) + sinA * (-axis.x);
			result.matrix[2 * 4 + 2] = cosA + (1 - cosA) * (axis.z * axis.z);

			return result;
		}

		static Mat4x4 orthoProj(float nearPlane, float farPlane, float left, float bottom, float right, float top)
		{
			Mat4x4 result = identity();

			//NOTE: Aspect ratio in here for free (because right = top * aspectRatio is what one passes in here)

			//NOTE: scl
			result.matrix[0 * 4 + 0] = 2 / (right - left);
			result.matrix[1 * 4 + 1] = 2 / (top - bottom);
			result.matrix[2 * 4 + 2] = -(2 / (farPlane - nearPlane));
			//NOTE: translation
			result.matrix[3 * 4 + 0] = -(right + left) / (right - left);
			result.matrix[3 * 4 + 1] = -(top + bottom) / (top - bottom);
			result.matrix[3 * 4 + 2] = -((farPlane + nearPlane) / (farPlane - nearPlane));

			return result;
		}

		static Mat4x4 persProj(float nearPlane, float farPlane, float aspectRatio, float fov)
		{
			//distance to view window
			float d = tanf(utils::degreesToRadians(fov / 2.0f));

			Mat4x4 result = identity();

			result.matrix[0 * 4 + 0] = d;
			result.matrix[1 * 4 + 1] = d * aspectRatio;
			// or:
			// result.matrix[0 * 4 + 0] = d / aspectRatio;
			// result.matrix[1 * 4 + 1] = d;

			//to normalize z between -1 and 1
			//to get to these formulars, one has to solve the following equations:
			// an + b = -n and af + b = f, where n = near and f = far and a and b are the coefficients to solve for
			//really we want it to be = -1 and = 1, but we have to take into account that the perspective divide comes later, and then -n/n = -1 and f/f = 1
			result.matrix[2 * 4 + 2] = -(2 / (farPlane - nearPlane));
			//Other solved equation
			result.matrix[3 * 4 + 2] = -((farPlane + nearPlane) / (farPlane - nearPlane));

			//to get z in w for perspective divide
			result.matrix[2 * 4 + 3] = 1;

			return result;
		}
	};
}