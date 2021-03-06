#include <string>
#include <chrono>

#include "format.h"

using std::string;
using std::chrono::duration_cast;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) 
{ 

    std::chrono::seconds sec(seconds);
    string str_time;

	str_time =std::to_string(duration_cast<std::chrono::hours>(sec).count())
                +":"+ std::to_string(duration_cast<std::chrono::minutes>(sec).count() % 60)
                + ":" +std::to_string(sec.count() % 60);
	return str_time;
}