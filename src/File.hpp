#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <boost/tokenizer.hpp>
using namespace std;

namespace File {

vector<vector<double>> read_point_set(string path);
void write_point_set(vector<vector<double>>& point_set, string path);
void write_value(double value, string path);

}