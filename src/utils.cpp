#include "utils.hh"
#include <random>

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

std::string btoa(bool val) {
    return (val == true) ? "True" : "False";
}

void plot_graph(string filename, string outputfilename) {
    string cmd = "dot -Tpng -o " + outputfilename + " " + filename;
    system(cmd.c_str());
}