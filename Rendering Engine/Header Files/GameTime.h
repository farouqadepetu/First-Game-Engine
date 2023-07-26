#pragma once

#include <Windows.h>

namespace RenderingEngine
{
	/** @struct Time ""
	*	@brief A struct that holds the properties for time.
	*/
	struct Time
	{
		__int64 previousTime = 0;
		__int64 currentTime = 0;
		double deltaTime = 0;
		double secondsPerCount = 0.0;
		bool stopped = false;
	};

	/**brief Initializes the specified Time object.
	*/
	void InitializeTime(Time& time);

	/**brief Resets the specified Time object.
	*/
	void Reset(Time& time);

	/**brief Computes the delta time (time between each frame) for the specified Time object.
	*/
	void Tick(Time& time);

	/**brief Starts the time for the specified Time object.
	*/
	void Start(Time& time);

	/**brief Stops the time for the specified Time object.
	*/
	void Stop(Time& time);
}