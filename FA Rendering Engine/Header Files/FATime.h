#pragma once

/** @file FATime.h
*	@brief File that has namespace FATime. Withn the namespace is the class Time.
*/

#include <Windows.h>

/** @namespace FATime
*	@brief Has Time class.
*/
namespace FATime
{
	class Time
	{
	public:
		Time();

		float GetPrevTime() const;
		float GetDeltaTime() const;
		bool GetIsTimeStopped()const ;

		void Reset();
		void Tick();

		void Start();
		void Stop();

	private:
		__int64 mPrevTime;				//t0
		__int64 mCurrentTime;			//t1
		double mDeltaTime;				//t1 - t0
		double mSecondsPerCount;
		bool mStopped;
	};
}