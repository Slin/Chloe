//
//  CLMath.h
//  Chloe
//
//  Copyright 2015 by Überpixel. All rights reserved.
//  Unauthorized use is punishable by torture, mutilation, and vivisection.
//

#ifndef __CHLOE_MATH_H__
#define __CHLOE_MATH_H__

#include <algorithm>
#include "CLSIMD.h"
#include "CLConstants.h"

namespace CL
{
	namespace Math
	{
		static inline float FastAbs(float val)
		{
			union
			{
				float fval;
				int ival;
			} tval;
			
			tval.fval = val;
			tval.ival &= (1UL << 31) - 1;

			return tval.fval;
		}
		
		static inline double FastAbs(double val)
		{
			union
			{
				double dval;
				long long ival;
			} tval;
			
			tval.dval = val;
			tval.ival &= (1ULL << 63) - 1;
			
			return tval.dval;
		}
		
		static inline bool IsNegative(float val)
		{
			union
			{
				float fval;
				int ival;
			} tval;
			
			tval.fval = val;
			return (tval.ival & (1UL << 31));
		}
		
		static inline bool IsNegative(double val)
		{
			union
			{
				double dval;
				long long ival;
			} tval;
			
			tval.dval = val;
			return (tval.ival & (1ULL << 63));
		}
		
		static inline bool Compare(float x, float y, float delta = k::EpsilonFloat)
		{
			return (FastAbs(x - y) < delta);
		}
		
		static inline bool Compare(double x, double y, float delta = k::EpsilonFloat)
		{
			return (FastAbs(x - y) < delta);
		}
		
		static inline float RadiansToDegrees(float radians)
		{
			return radians * 57.2957795130823208768f;
		}
		
		static inline float DegreesToRadians(float degrees)
		{
			return degrees * 0.01745329251994329577f;
		}
		
		
		float Sqrt(float x);
		float InverseSqrt(float x);
		
		float Sin(float x);
		float Cos(float x);
	}
}

#endif
