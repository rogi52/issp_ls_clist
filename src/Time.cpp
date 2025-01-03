#include "Time.hpp"

void Time::start() {
    start_time = chrono::system_clock::now();
    is_working = true;
}

void Time::end() { is_working = false; }

long long Time::get(int type) {
    assert(is_working);
    auto now = chrono::system_clock::now();
    switch(type) {
        case MILLISECONDS: return cast_milliseconds(now);
        default:
            cerr << type << " is not defined" << endl;
            exit(1);
    }
}

long long Time::cast_milliseconds(TP now) {
    return chrono::duration_cast<chrono::milliseconds>(now - start_time).count();
}