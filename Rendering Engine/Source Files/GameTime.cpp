#include "GameTime.h"

namespace RenderingEngine
{
	void InitializeTime(Time& time)
	{
		//get counts per second
		__int64 countsPerSec{ 0 };
		QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);

		//convert to seconds per count
		time.secondsPerCount = 1.0 / (double)countsPerSec;
	}

	void Reset(Time& time)
	{
		//get current time
		__int64 resetTime{ 0 };
		QueryPerformanceCounter((LARGE_INTEGER*)&resetTime);

		time.previousTime = resetTime;
	}

	void Tick(Time& time)
	{
		if (time.stopped)
		{
			time.deltaTime = 0.0f;
			return;
		}

		//get current time
		QueryPerformanceCounter((LARGE_INTEGER*)&time.currentTime);

		time.deltaTime = (time.currentTime - time.previousTime) * time.secondsPerCount;

		time.previousTime = time.currentTime;

		// Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
		// processor goes into a power save mode or we get shuffled to another
		// processor, then mDeltaTime can be negative.
		if (time.deltaTime < 0.0)
		{
			time.deltaTime = 0.0;
		}
	}

	void Start(Time& time)
	{
		if (time.stopped)
		{
			__int64 startTime{ 0 };
			QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

			time.previousTime = startTime;
			time.stopped = false;
		}
	}

	void Stop(Time& time)
	{
		if (!time.stopped)
		{
			time.stopped = true;
		}
	}
}