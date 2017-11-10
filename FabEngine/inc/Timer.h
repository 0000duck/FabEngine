#pragma once

namespace Fab
{
	class Timer
	{
	public:
		Timer();
		~Timer();

		float TotalTime() const;
		float DeltaTime() const;

		void  Reset();
		void  Start();
		void  Stop();
		void  Tick();

	private:
		double  _secondsPerCount;
		double  _deltaTime;

		__int64 _baseTime;
		__int64 _pausedTime;
		__int64 _stopTime;
		__int64 _previousTime;
		__int64 _currentTime;

		bool    _stopped;
	};
}