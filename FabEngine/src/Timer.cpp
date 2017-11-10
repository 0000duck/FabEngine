#include <DirectXComponentsPCH.h>
#include "Timer.h"

namespace Fab
{
	Timer::Timer()
		: _secondsPerCount(0.0), _deltaTime(-1.0), _baseTime(0),
		_pausedTime(0), _previousTime(0), _currentTime(0), _stopped(false)
	{
		__int64 countsPerSec;
		QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
		_secondsPerCount = 1.0 / (double)countsPerSec;
	}

	Timer::~Timer()
	{
	}

	// Returns the total time elapsed since Reset() was called, NOT counting any
	// time when the clock is stopped.
	float Timer::TotalTime() const
	{
		// If we are stopped, do not count the time that has passed since we stopped.
		// Moreover, if we previously already had a pause, the distance 
		// mStopTime - mBaseTime includes paused time, which we do not want to count.
		// To correct this, we can subtract the paused time from mStopTime:  
		//
		//                     |<--paused time-->|
		// ----*---------------*-----------------*------------*------------*------> time
		//  mBaseTime       mStopTime        startTime     mStopTime    mCurrTime

		if (_stopped)
		{
			return (float)(((_stopTime - _pausedTime) - _baseTime) * _secondsPerCount);
		}

		// The distance mCurrTime - mBaseTime includes paused time,
		// which we do not want to count.  To correct this, we can subtract 
		// the paused time from mCurrTime:  
		//
		//  (mCurrTime - mPausedTime) - mBaseTime 
		//
		//                     |<--paused time-->|
		// ----*---------------*-----------------*------------*------> time
		//  mBaseTime       mStopTime        startTime     mCurrTime

		else
		{
			return (float)(((_currentTime - _pausedTime) - _baseTime) * _secondsPerCount);
		}
	}

	float Timer::DeltaTime()const
	{
		return (float)_deltaTime;
	}

	void Timer::Reset()
	{
		__int64 currentTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

		_baseTime = currentTime;
		_previousTime = currentTime;
		_stopTime = 0;
		_stopped = false;
	}

	void Timer::Start()
	{
		__int64 startTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

		// Accumulate the time elapsed between stop and start pairs.
		//
		//                     |<-------d------->|
		// ----*---------------*-----------------*------------> time
		//  mBaseTime       mStopTime        startTime     

		if (_stopped)
		{
			_pausedTime += (startTime - _stopTime);

			_previousTime = startTime;
			_stopTime = 0;
			_stopped = false;
		}
	}

	void Timer::Stop()
	{
		if (!_stopped)
		{
			__int64 currentTime;
			QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

			_stopTime = currentTime;
			_stopped = true;
		}
	}

	void Timer::Tick()
	{
		if (_stopped)
		{
			_deltaTime = 0.0;
			return;
		}

		__int64 currentTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
		_currentTime = currentTime;

		// Time difference between this frame and the previous.
		_deltaTime = (_currentTime - _previousTime) * _secondsPerCount;

		// Prepare for next frame.
		_previousTime = _currentTime;

		// Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
		// processor goes into a power save mode or we get shuffled to another
		// processor, then mDeltaTime can be negative.
		if (_deltaTime < 0.0)
		{
			_deltaTime = 0.0;
		}
	}
}