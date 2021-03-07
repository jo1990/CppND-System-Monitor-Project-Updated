#include <string>
#include <chrono>
#include <iomanip>
#include <iostream>

#include "format.h"

using std::string;
using std::chrono::duration_cast;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) 
{ 

    std::chrono::seconds sec(seconds);
    std::stringstream str_time;

	str_time << std::setfill('0') <<  std::setw(2) << std::to_string(duration_cast<std::chrono::hours>(sec).count())
                <<":"<<std::setfill('0') <<  std::setw(2)<<std::to_string(duration_cast<std::chrono::minutes>(sec).count() % 60)
                << ":" << std::setfill('0') <<  std::setw(2)<<std::to_string(sec.count() % 60);
	return str_time.str();
}