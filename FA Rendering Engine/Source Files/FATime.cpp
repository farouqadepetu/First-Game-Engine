#include "FATime.h"


namespace FATime
{
	Time::Time() : mCurrTime{ 0 }, mPrevTime{ 0 }, mDeltaTime{ 0.0 }, mStopTime{ 0 }, mPausedTime{ 0 }, mBaseTime{ 0 }, mStopped{ false }
	{
		//get counts per second
		__int64 countsPerSec{ 0 };
		QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);

		//convert to seconds per count
		mSecondsPerCount = 1.0 / (double)countsPerSec;

	}

	void Time::Tick()
	{
		if (mStopped)
		{
			mDeltaTime = 0.0;
			return;

		}

		//get current time
		QueryPerformanceCounter((LARGE_INTEGER*)&mCurrTime);

		//time difference between this frame and the previous frame
		mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPerCount;

		//Save current time for the next frame
		mPrevTime = mCurrTime;

		// Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
		// processor goes into a power save mode or we get shuffled to another
		// processor, then mDeltaTime can be negative.
		if (mDeltaTime < 0.0)
		{
			mDeltaTime = 0.0;
		}
	}

	void Time::Reset()
	{
		//get current time
		__int64 resetTime{ 0 };
		QueryPerformanceCounter((LARGE_INTEGER*)&resetTime);

		//For the first frame there is no pervious frame so no previous time stamp.
		//Need to initalize mPrevTime
		mPrevTime = resetTime;

		mBaseTime = resetTime;

		mStopTime = 0;
		mStopped = false;
	}

	float Time::DeltaTime() const
	{
		return (float)mDeltaTime;
	}

	void Time::Stop()
	{
		//if not paused already
		if (!mStopped)
		{
			//get the time the game/animation was paused
			__int64 stopTime{ 0 };
			QueryPerformanceCounter((LARGE_INTEGER*)&stopTime);

			mStopTime = stopTime;
			mStopped = true;
		}
	}

	void Time::Start()
	{
		//if paused
		if (mStopped)
		{
			//get the time the game/animation was unpaused
			__int64 startTime{ 0 };
			QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

			//get how much time the game/animation was paused for
			mPausedTime += static_cast<__int64>((startTime - mStopTime) * mSecondsPerCount);

			//reset stop time and mStopped
			mStopTime = 0;
			mStopped = false;
		}
	}

	//Returns how much time has passed since Reset() was called.
	//Does not count any pause time
	float Time::TotalTime() const
	{
		//if paused
		if (mStopped)
		{
			//if we take the total time when the game is paused, we do not count the time that has passed since we paused
			//If there were previous pauses we need to not include that in the total time
			//                     |<--paused time-->|
			// ----*---------------*-----------------*------------*------------*------> time
			//  mBaseTime       mStopTime        startTime     mStopTime    mCurrTime
			return static_cast<float>(((mStopTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
		}
		else //not paused
		{
			//If there were previous pauses we need to not include that in the total time
			//                     |<--paused time-->|
			// ----*---------------*-----------------*------------*------> time
			//  mBaseTime       mStopTime        startTime     mCurrTime
			return static_cast<float>(((mCurrTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
		}
	}
}