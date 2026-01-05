#ifndef _CPP_STANDARD_LIB_HINT_
#define _CPP_STANDARD_LIB_HINT_
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <chrono>
#include <thread>
#include <algorithm>
#define smart_use_second_sleep(seconds__) std::this_thread::sleep_for(std::chrono::seconds(seconds__))
#endif //_CPP_STANDARD_LIB_HINT_