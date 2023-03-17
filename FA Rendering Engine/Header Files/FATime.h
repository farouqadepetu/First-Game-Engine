#pragma once

/** @file FATime.h
*	@brief File that has namespace FATime. Withn the namespace is the class Time.
*/

#include <Windows.h>

/** @class Time ""
*	@brief This class is used to get the time between each frame. You can stop start, reset and get the total time.
*/
namespace FATime
{
	class Time
	{
	public:
		/**@brief Default Constructor.
		* Gets and stores the seconds per count.
		*/
		Time();

		/**@brief Stores the difference between the current time and the previous time.
		*/
		void Tick();

		/**@brief Returns the difference between the current time and the previous time.
		*/
		float DeltaTime() const;

		/**@brief Resets all time variables.
		*/
		void Reset();

		/**@brief Stops the timer.
		*/
		void Stop();

		/**@brief Starts the timer.
		*/
		void Start();

		/**@brief Returns how much time has passed since Reset() was called. Does not count any pause time.
		*/
		float TotalTime() const;

	private:
		__int64 mCurrTime; //holds current time stamp ti
		__int64 mPrevTime; //holds previous time stamp ti-1
		__int64 mStopTime; //holds the time we stopped the game/animation
		__int64 mPausedTime; //holds how long the game/animation was paused for
		__int64 mBaseTime; //holds the time we started / resetted

		double mSecondsPerCount;
		double mDeltaTime; //time elapsed btw frames change in t = ti - ti-1

		bool mStopped; //flag to indicate if the game/animation is paused or not

	};
}