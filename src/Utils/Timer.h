#pragma once

#include <print>
#include <chrono>
#include <unordered_map>
#include <iomanip>
#include <ctime>

using namespace std::literals::chrono_literals;

struct timeData
{
	std::chrono::steady_clock::time_point timeStamp;
	std::chrono::milliseconds delayTime;
};

class Time
{
public:
	using currentTime = std::chrono::hh_mm_ss<std::chrono::system_clock::duration>;

	Time(currentTime hms) : m_time(hms) {}
	

	std::string toString() const
	{
		std::string_view time_of_day = m_time.hours().count() >= 12 ? "pm" : "am";
		int hours = m_time.hours().count() >= 13 ? (m_time.hours().count() % 13 + 1) : m_time.hours().count();
		return std::format("{}:{} {}", m_time.hours().count() % 13 + 1, m_time.minutes().count(), time_of_day);
	};

private:
	currentTime m_time;
};

template <>
struct std::formatter<Time> {
	// 3a. Implement parse (for simple cases, just return the iterator)
	constexpr auto parse(std::format_parse_context& ctx) {
		return ctx.begin();
	}

	// 3b. Implement format
	auto format(const Time& t, std::format_context& ctx) const {
		// Format the Point as "(x, y)"
		return std::format_to(ctx.out(), "{}", t.toString());
	}
};

class Timer
{
public:

	//using Time = std::chrono::hh_mm_ss<std::chrono::system_clock::duration>;

	Timer(const Timer&) = delete;
	Timer& operator=(const Timer& other) = delete;

	static Timer& getInstance();

	void setDelay(std::string name, std::chrono::seconds duration);
	void setDelay(std::string name, std::chrono::milliseconds duration);

	bool delayFinished(const std::string& name);
	void resetDelay(const std::string& name);

	std::chrono::year_month_day getDate();
	Time getTime();

private:
	//Default constructor... not used just required
	Timer() {};

	std::unordered_map<std::string, timeData> timeStamps;
};
