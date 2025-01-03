#include <iostream>
#include <string>
#include <vector>
#include "File.hpp"
#include "Selector.hpp"
#include "Indicator.hpp"
#include "Time.hpp"
using namespace std;

int main(int argc, char **argv) {
    string point_set_file_path = "";
    string ref_point_set_file_path = "";
    string weight_point_set_file_path = "";
    string out_point_subset_file_path = "";
    string out_qi_file_path = "";
    int point_subset_size = 0;
    string q_indicator = "";
    double hv_ref_point_val = numeric_limits<double>::min();
    string selector_alg = "";
    int rnd_seed = 0;
    string out_time_file_path = "";
    int nlist_size = 0;
    int rlist_size = 0;
    string out_n_qi_calls_file_path = "";

    for(int i = 1; i < argc; i += 2) {
        string key = argv[i], value = argv[i + 1];
             if(key == "-point_set_file_path") point_set_file_path = value;
        else if(key == "-ref_point_set_file_path") ref_point_set_file_path = value;
        else if(key == "-weight_point_set_file_path") weight_point_set_file_path = value;
        else if(key == "-out_point_subset_file_path") out_point_subset_file_path = value;
        else if(key == "-out_qi_file_path") out_qi_file_path = value;
        else if(key == "-out_time_file_path") out_time_file_path = value;
        else if(key == "-out_n_qi_calls_file_path") out_n_qi_calls_file_path = value;
        else if(key == "-point_subset_size") point_subset_size = stoi(value);
        else if(key == "-q_indicator") q_indicator = value;
        else if(key == "-hv_ref_point_val") hv_ref_point_val = stod(value);
        else if(key == "-selector_alg") selector_alg = value;
        else if(key == "-seed") rnd_seed = stoi(value);
        else if(key == "-nlist_size") nlist_size = stoi(value);
        else if(key == "-rlist_size") rlist_size = stoi(value);
        else {
            cerr << "Error. Unkown argument " << key << endl;
            exit(1);
        }
    }

    mt19937 rnd_engine(rnd_seed);
    vector<vector<double>> point_set = File::read_point_set(point_set_file_path);
    vector<vector<double>> ref_point_set;
    if(!ref_point_set_file_path.empty()) ref_point_set = File::read_point_set(ref_point_set_file_path);
    vector<vector<double>> weight_point_set;
    if(!weight_point_set_file_path.empty()) weight_point_set = File::read_point_set(weight_point_set_file_path);
    vector<double> ref_point;
    if(hv_ref_point_val != numeric_limits<double>::min()) ref_point = vector<double>(point_set[0].size(), hv_ref_point_val);
        
    Indicator I(q_indicator, ref_point_set, weight_point_set, ref_point);
    Selector selector(point_set, point_subset_size, I, rnd_engine, nlist_size, rlist_size);
    
    Time timer; timer.start();
    vector<vector<double>> point_subset = selector.select(selector_alg);
    assert(point_subset.size() == point_subset_size);
    auto TIME = timer.get(Time::MILLISECONDS); timer.end();

    File::write_value(TIME, out_time_file_path);
    File::write_point_set(point_subset, out_point_subset_file_path);
    File::write_value(selector.I.num_of_call_indicator, out_n_qi_calls_file_path);
    File::write_value(I.eval(point_subset), out_qi_file_path);
}