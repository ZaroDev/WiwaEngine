#pragma once

#include "Vector2i.h"
#include "Vector2f.h"

#include "Vector3i.h"
#include "Vector3f.h"

#include "Vector4i.h"
#include "Vector4f.h"

#include "Rectangle.h"


namespace Wiwa {
	//Quake3's fast inverse sqrt algo
	//float Q_rsqrt(float number)
	//{
	//	long i;
	//	float x2, y;
	//	const float threehalfs = 1.5F;

	//	x2 = number * 0.5F;
	//	y = number;
	//	i = *(long*)&y;                       // evil floating point bit level hacking
	//	i = 0x5f3759df - (i >> 1);               // what the fuck? 
	//	y = *(float*)&i;
	//	y = y * (threehalfs - (x2 * y * y));   // 1st iteration
	////	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	//	return y;
	//}
}