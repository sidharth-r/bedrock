#include "vmath.h"
#include <math.h>

vmath::vec2d vmath::operator+(const vec2d &a, const vec2d &b)
{
	return vec2d{ a.x + b.x, a.y + b.y };
}

vmath::vec2d vmath::operator-(const vec2d &a, const vec2d &b)
{
	return vec2d{ a.x - b.x, a.y - b.y };
}

double vmath::operator*(const vec2d &a, const vec2d &b)
{
	return a.x * b.x + a.y * b.y;
}

double vmath::magnitude(vec2d a)
{
	return sqrt(a.x * a.x + a.y * a.y);
}

double vmath::angle(vec2d a, vec2d b)
{
	double d = a * b;
	double det = a.x * b.y - b.x * a.y;
	return atan2(det, d);
}

double vmath::angle_raw(vec2d a, vec2d b)
{
	return acos((a * b) / (magnitude(a) * magnitude(b)));
}

vmath::vec2d vmath::normalize(vec2d a)
{
	return vec2d{ a.x / magnitude(a), a.y / magnitude(a) };
}