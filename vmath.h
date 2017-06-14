#pragma once

namespace vmath
{
	struct vec2d
	{
		double x, y;
	};

	vec2d operator+(const vec2d &a, const vec2d &b);
	vec2d operator-(const vec2d &a, const vec2d &b);
	double operator*(const vec2d &a, const vec2d &b);
	double magnitude(vec2d a);
	double angle(vec2d a, vec2d b);
	double angle_raw(vec2d a, vec2d b);
	vec2d normalize(vec2d a);
};
