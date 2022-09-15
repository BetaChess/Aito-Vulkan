#ifndef AITO_TIME_H
#define AITO_TIME_H
#include <chrono>

class Time
{
public:
	inline float deltaTime() { return Time::deltaTime_; };

	void updateTime(); // SHOULD ONLY BE RUN ONCE PER FRAME!

private:
	std::chrono::steady_clock clock_;

	float deltaTime_ = 0;
	std::chrono::steady_clock::time_point current_time_ = Time::clock_.now();
};


#endif /* AITO_TIME_H */