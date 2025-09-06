#include "Timer.h"

Timer& Timer::getInstance()
{
    static Timer instance;
    return instance;
}

//Calls the millisecond version of the funcion
void Timer::setDelay(std::string name, std::chrono::seconds duration)
{
    setDelay(name, std::chrono::duration_cast<std::chrono::milliseconds>(duration));
}

//Registers a delay, it automatically sets the timepoint to 0 so that the firt time delayFinised is called it is always true
void Timer::setDelay(std::string name, std::chrono::milliseconds duration)
{
    timeStamps.insert({ name.data(), {
        std::chrono::steady_clock::time_point(),
        duration} });
}

bool Timer::delayFinished(const std::string& name)
{
    if (timeStamps.find(name) == timeStamps.end())
    {
        std::println("Delay {} is not registered call setDelay()", name);
        return false;
    }

    if (std::chrono::steady_clock::now() - timeStamps[name].timeStamp > timeStamps[name].delayTime)
        return true;
    
    return false;
}

void Timer::resetDelay(const std::string& name)
{
    if (timeStamps.find(name) == timeStamps.end())
    {
        std::println("Delay \"{}\" is not registered call setDelay()", name);
        return;
    }
        
    timeStamps[name].timeStamp = std::chrono::steady_clock::now();
}

std::chrono::year_month_day Timer::getDate()
{
    auto now = std::chrono::system_clock::now();
    auto days = std::chrono::floor<std::chrono::days>(now);

    return std::chrono::year_month_day(days);
}

Time Timer::getTime()
{
    auto zt = std::chrono::zoned_time{ std::chrono::current_zone(), std::chrono::system_clock::now()};
    
    // 2. Get the local time from the zoned time
    auto local_tp = zt.get_local_time();

    // 3. Get the duration since the beginning of the day (midnight)
    auto time_since_midnight = local_tp - std::chrono::floor<std::chrono::days>(local_tp);

    // 4. Create an hh_mm_ss object to break down the duration
    //    auto hms = std::chrono::hh_mm_ss{ time_since_midnight };
    Time t = Time(std::chrono::hh_mm_ss{ time_since_midnight });
    return t;
}
