#pragma once
#include <math.h>

namespace math
{
	struct vector3d
	{
		float x;
		float y;
		float z;

		explicit vector3d() {}
		explicit vector3d(float new_x, float new_y, float new_z) : x(new_x), y(new_y), z(new_z) {}

		inline bool operator==(const vector3d& other) const
		{
			return (this->x == other.x && this->y == other.y && this->z == other.z);
		}
		inline bool operator!=(const vector3d& other) const
		{
			return !(*this == other);
		}

		inline vector3d operator+(const vector3d& other) const
		{
			return vector3d(this->x + other.x, this->y + other.y, this->z + other.z);
		}
		inline vector3d operator-(const vector3d& other) const
		{
			return vector3d(this->x - other.x, this->y - other.y, this->z - other.z);
		}
		inline vector3d operator-() const
		{
			return vector3d(-this->x, -this->y, -this->z);
		}
		inline vector3d operator*(float val) const
		{
			return vector3d(this->x * val, this->y * val, this->z * val);

		}
		inline vector3d operator/(float val) const
		{
			return vector3d(this->x / val, this->y / val, this->z / val);

		}

		inline float dot(const vector3d& other) const
		{
			return	x * other.x +
				y * other.y +
				z * other.z;
		}

		inline float distance(const vector3d& other) const
		{
			return sqrt((x - other.x) * (x - other.x) +
				(y - other.y) * (y - other.y) +
				(z - other.z) * (z - other.z)
			);
		}

		inline bool negative() const
		{
			return this->x < 0.f || this->y < 0.f || this->z < 0.f;
		}
	};

	struct vector2d
	{
		float x;
		float y;

		explicit vector2d() {}
		explicit vector2d(float new_x, float new_y) : x(new_x), y(new_y) {}

		inline bool operator==(const vector2d& other) const
		{
			return (this->x == other.x && this->y == other.y);
		}
		inline bool operator!=(const vector2d& other) const
		{
			return !(*this == other);
		}

		inline vector2d operator+(const vector2d& other) const
		{
			return vector2d(this->x + other.x, this->y + other.y);
		}
		inline vector2d operator-(const vector2d& other) const
		{
			return vector2d(this->x - other.x, this->y - other.y);
		}
		inline vector2d operator-() const
		{
			return vector2d(-this->x, -this->y);
		}
		inline vector2d operator*(float val) const
		{
			return vector2d(this->x * val, this->y * val);

		}
		inline vector2d operator/(float val) const
		{
			return vector2d(this->x / val, this->y / val);

		}

		inline float dot(const vector2d& other) const
		{
			return	x * other.x +
				y * other.y;
		}

		inline float distance(const vector2d& other) const
		{
			return sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
		}

		inline bool negative() const
		{
			return this->x < 0.f || this->y < 0.f;
		}
	};
}