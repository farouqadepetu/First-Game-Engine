#include "FAMathUtility.h"
#include <cmath>

namespace FAMath
{
	bool compareFloats(float x, float y, float epsilon)
	{
		float diff = fabs(x - y);
		//exact epsilon
		if (diff < epsilon)
		{
			return true;
		}

		//adapative epsilon
		return diff <= epsilon * std::max(fabs(x), fabs(y));
	}

	bool compareDoubles(double x, double y, double epsilon)
	{
		double diff = fabs(x - y);
		//exact epsilon
		if (diff < epsilon)
		{
			return true;
		}

		//adapative epsilon
		return  diff <= epsilon * std::max(fabs(x), fabs(y));
	}
}