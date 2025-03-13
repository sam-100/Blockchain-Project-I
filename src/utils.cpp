#include "utils.hh"
#include <random>
#include <string.h>

using namespace std;

static random_device rd;
static mt19937 gen(rd());  // Declare generator once

int random_int(int start, int end) {
    uniform_int_distribution<int> dist(start, end - 1);
    return dist(gen);
}

float random_float(float start, float end) {
    uniform_real_distribution<float> dist(start, end);
    return dist(gen);
}

float random_exp_float(float mean) {
    exponential_distribution<float> dist(1.0 / mean);
    return dist(gen);
}
