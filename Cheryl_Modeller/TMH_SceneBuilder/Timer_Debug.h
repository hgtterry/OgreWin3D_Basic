#pragma once
class Timer_Debug
{
public:

	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;

	Timer_Debug(void);
	~Timer_Debug(void);
};

