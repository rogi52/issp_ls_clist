#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include "Indicator.hpp"
using namespace std;


class Selector {
  private:
    vector<vector<double>> point_set;
    int point_set_size;
    int point_subset_size;
    int d;
    mt19937 rnd_engine;
    int nlist_size;
    int rlist_size;
    string path;

  public:
    Indicator I;
    Selector(vector<vector<double>>& point_set, int point_subset_size, Indicator& I, mt19937 rnd_engine, int nlist_size, int rlist_size);
    vector<vector<double>> select(string algorithm);
    vector<vector<double>> LS();
    vector<vector<double>> LS_N();
    vector<vector<double>> LS_R();
    vector<vector<double>> LS_RN();
};