#pragma once

#include <cmath>

class Vector2
{
public:
	float x = 0;
	float y = 0;
public:

	Vector2() = default;
	~Vector2() = default;

	Vector2(float x, float y)
		:x(x), y(y) {
	}
	Vector2(int x, int y)
		:x(static_cast<float>(x)), y(static_cast<float>(y)) {
	}

	Vector2 operator+(const Vector2& vec) const
	{
		return Vector2(x + vec.x, y + vec.y);
	}

	Vector2 operator-(const Vector2& vec) const
	{
		return Vector2(x - vec.x, y - vec.y);
	}

	float operator*(const Vector2& vec) const
	{
		return x * vec.x + y * vec.y;
	}

	Vector2 operator*(float val) const
	{
		return Vector2(x * val, y * val);
	}

	void operator+=(const Vector2& vec)
	{
		x += vec.x, y += vec.y;
	}

	bool operator==(const Vector2& vec)
	{
		return (x == vec.x) && (y == vec.y);
	}

	void operator-=(const Vector2& vec)
	{
		x -= vec.x, y -= vec.y;
	}

	void operator*=(int val)
	{
		x *= val, y *= val;
	}

	float length()
	{
		return sqrt(x * x + y * y);
	}

	Vector2 normalize()
	{
		float len = length();

		if (len == 0)
			return Vector2(0, 0);

		return Vector2(x / len, y / len);
	}
};