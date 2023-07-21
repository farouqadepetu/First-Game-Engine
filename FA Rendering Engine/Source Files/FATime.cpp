#include "FATime.h"


namespace FATime
{
	Time::Time() : mPrevTime{ 0 }, mCurrentTime{ 0 }, mDeltaTime{ 0 }, mSecondsPerCount{ 0.0 }, mStopped{ false }
	{
		//get counts per second
		__int64 countsPerSec{ 0 };
		QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);

		//convert to seconds per count
		mSecondsPerCount = 1.0 / (double)countsPerSec;
	}

	float Time::GetPrevTime() const
	{
		return (float)mPrevTime;
	}

	float Time::GetDeltaTime() const
	{
		return (float)mDeltaTime;
	}

	bool Time::GetIsTimeStopped() const
	{
		return mStopped;
	}

	void Time::Reset()
	{
		//get current time
		__int64 resetTime{ 0 };
		QueryPerformanceCounter((LARGE_INTEGER*)&resetTime);

		mPrevTime = resetTime;
	}

	void Time::Tick()
	{
		if (mStopped)
		{
			mDeltaTime = 0.0f;
			return;
		}

		//get current time
		QueryPerformanceCounter((LARGE_INTEGER*)&mCurrentTime);

		mDeltaTime = (mCurrentTime - mPrevTime) * mSecondsPerCount;

		mPrevTime = mCurrentTime;

		// Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
		// processor goes into a power save mode or we get shuffled to another
		// processor, then mDeltaTime can be negative.
		if (mDeltaTime < 0.0)
		{
			mDeltaTime = 0.0;
		}
	}

	void Time::Start()
	{
		if (mStopped)
		{
			__int64 startTime{ 0 };
			QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

			mPrevTime = startTime;
			mStopped = false;
		}
	}

	void Time::Stop()
	{
		if (!mStopped)
		{
			mStopped = true;
		}
	}
}