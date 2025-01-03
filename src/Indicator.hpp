#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <cmath>
#include <pagmo/utils/hv_algos/hv_hvwfg.hpp>
using namespace std;

class Indicator {
  public:
    int num_of_call_indicator = 0;
    vector<vector<double>> ref_point_set;
    vector<vector<double>> weight_point_set;
    vector<double> ref_point;
    string q_indicator;
    static constexpr double INF = 1e300;

    Indicator();
    Indicator(string& q_indicator, vector<vector<double>>& ref_point_set, vector<vector<double>>& weight_point_set, vector<double>& ref_point);
    double eval(vector<vector<double>>& point_set);
    double eval_binary(const vector<int>& x, const vector<vector<double>>& point_set);
    double hv(vector<vector<double>>& point_set);
    double igd(const vector<vector<double>>& point_set);
    double igd_plus(const vector<vector<double>>& point_set);
    double r2(const vector<vector<double>>& point_set);
    double nr2(const vector<vector<double>>& point_set);
    double senergy(const vector<vector<double>>& point_set);
    double epsilon(const vector<vector<double>>& point_set);
};