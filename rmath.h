#pragma once

#include <vector>

#include "vmath.h"

using namespace std;
using namespace vmath;

namespace rmath
{
	bool dda(vector<vector<int>> &wMap, vec2d origin, vec2d dest);
}