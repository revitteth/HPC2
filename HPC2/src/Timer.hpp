#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <tbb/tbb.h>

// Time units of code easily with this class
class Timer
{
	private: 
		tbb::tick_count start_time, end_time;
		std::vector<double> times;

	public:
		Timer() {}

		Timer(tbb::tick_count start_time) : start_time(start_time) {}

		void Start(tbb::tick_count now) { start_time = now; }

		double Stop(tbb::tick_count now) 
		{
			end_time = now;
			double time = (end_time - start_time).seconds();
			times.push_back(time);
			start_time = end_time;
			return time;
		}

		double getLastTime() { return times.back(); }

		double getTimeAt(unsigned index) { return times.at(index); }

		double getTotalTime()
		{
			double sum;
			for(unsigned i = 0; i < times.size(); i++)
			{
				sum += times.at(i);
			}
			return sum;
		}
};


#endif