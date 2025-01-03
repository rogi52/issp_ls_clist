#pragma once
#include <vector>
#include <iostream>
#include <chrono>
#include <cassert>
using namespace std;

class Time {
  public:
    
    void start();
    void end();
    enum { MILLISECONDS, SECONDS, MINUTES, HOURS };
    long long get(int type);

  private:
    using TP = std::chrono::system_clock::time_point;
    TP start_time;
    bool is_working = false;
    long long cast_milliseconds(TP now);
};