#include "File.hpp"

vector<vector<double>> File::read_point_set(string path) {
    vector<vector<double>> pset;
    ifstream in(path);
    if (!in.is_open()) {
        cerr << "Error. " << path << " cannot be opened." << endl;
        exit(1);
    }

    vector<string> str_vec;
    string line;
    while(getline(in, line)) {
        boost::tokenizer< boost::escaped_list_separator<char> > tok(line);
        str_vec.assign(tok.begin(), tok.end());
        vector<double> double_vec;
        for(int i = 0; i < str_vec.size(); i++) {
            double_vec.push_back(stod(str_vec[i]));
        }
        pset.push_back(double_vec);
    }
    return pset;
}

void File::write_point_set(vector<vector<double>>& point_set, string path) {
    ofstream out(path);
    if (!out.is_open()) {
        cerr << "Error. " << path << " cannot be opened." << endl;
        exit(1);
    }

    for(int i = 0; i < point_set.size(); i++) {
        for(int j = 0; j < point_set[i].size(); j++) {
            out << setprecision(20) << point_set[i][j] << ",\n"[j == (int)point_set[i].size() - 1];
        }
    }
    out.close();
}

void File::write_value(double value, string path) {
    ofstream out(path);
    if (!out.is_open()) {
        cerr << "Error. " << path << " cannot be opened." << endl;
        exit(1);
    }

    out << setprecision(20) << value << endl;
    out.close();
}