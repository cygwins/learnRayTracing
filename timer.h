#ifndef TIMER_H_
#define TIMER_H_
#include <chrono>
#include <ctime>
#include <iostream>
using std::chrono::system_clock;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::time_point;
using std::chrono::seconds;
using std::chrono::minutes;
using std::chrono::hours;
using std::cout;
using std::endl;

class timer {
public:
    timer(){}
    void begin() { begin_time = system_clock::now(); }
    void end() { end_time = system_clock::now(); }
    void show_summary() {
        duration<float> elapsed = end_time - begin_time;
        time_t end_t = system_clock::to_time_t(end_time);
        cout << endl << "Finished at " << ctime(&end_t) << "Time elapsed: ";
        auto s = duration_cast<seconds>(elapsed).count();
        auto h = s / 3600; if(h > 0) { cout << h << "h "; }
        auto m = (s / 60) % 60; if(m > 0) { cout << m << "min "; }
        cout << s % 60 << "s." << endl;
    }
    void show_elapsed() {
        cur_time = system_clock::now();
        duration<float> elapsed = cur_time - begin_time;
        cout << "Used: ";
        show_duration(elapsed);
    }
    void show_eta(float percentage) {
        cur_time = system_clock::now();
        duration<float> elapsed = cur_time - begin_time;
        auto estimate = elapsed * (1 - percentage) / percentage;
        cout << "ETA: ";
        show_duration(estimate);
    }
    void show_duration(duration<float> &elapsed) {
        auto s = duration_cast<seconds>(elapsed).count();
        auto h = s / 3600; if(h > 0) { cout << h << "h "; }
        auto m = (s / 60) % 60; if(m > 0) { cout << m << "min "; }
        cout << s % 60 << "s ";
    }

    time_point<system_clock> begin_time, end_time, cur_time;
};

#endif
