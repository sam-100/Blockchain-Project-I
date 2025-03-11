#include "utils.hh"
#include <random>
#include <string.h>

using namespace std;

int random_int(int start, int end) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<int> dist(start, end-1);
    return dist(gen);
}

float random_float(float start, float end) {
    return (float)random_int(start*100, end*100)/100.00;
}

float random_exp_float(float mean) {
    static random_device rd;
    static mt19937 gen(rd());
    exponential_distribution<float> dist(1.0/mean);
    return dist(gen);
}
