#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <tbb/tbb.h>
#include <string>

// Time units of code easily with this class
class Timer
{
	private: 
		tbb::tick_count start_time, end_time;
		std::vector<double> times;
		std::vector<std::string> strings;

	public:
		// default constructor
		Timer() {}

		// construct and initialise timer with start time
		Timer(tbb::tick_count start_time) : start_time(start_time) {}

		// start the timer at given time
		void Start(tbb::tick_count now) { start_time = now; }

		// stop the timer at given time and save interval to times
		double Stop(tbb::tick_count now) 
		{
			end_time = now;
			double time = (end_time - start_time).seconds();
			times.push_back(time);
			start_time = end_time;
			return time;
		}

		// number of times stored in vector times
		unsigned count() { return times.size(); }

		// the last recorded time interval
		double lastTime() { return times.back(); }

		// time at given index of times
		double timeAt(unsigned index) { return times.at(index); }
		
		// total time (sum of intervals) stored in times
		double totalTime()
		{
			double sum;
			for(unsigned i = 0; i < times.size(); i++)
			{
				sum += times.at(i);
			}
			return sum;
		}


		void addString(std::string str)
		{
			strings.push_back(str);
		}

		void printStrings()
		{
			for (int i = 0; i < strings.size(); i++)
			{
				std::cout << strings.at(i) << std::endl;
			}
			std::cout << std::endl;
		}
};


#endif