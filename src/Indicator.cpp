#include "Indicator.hpp"

Indicator::Indicator(){}

Indicator::Indicator(string& q_indicator, vector<vector<double>>& ref_point_set, vector<vector<double>>& weight_point_set, vector<double>& ref_point) {
    this->q_indicator = q_indicator;
    this->ref_point_set = ref_point_set;
    this->weight_point_set = weight_point_set;
    this->ref_point = ref_point;
}

double Indicator::eval(vector<vector<double>>& point_set) {
    num_of_call_indicator++;
    if(q_indicator == "hv") return -hv(point_set);
    if(q_indicator == "igd") return igd(point_set);
    if(q_indicator == "igd+") return igd_plus(point_set);
    if(q_indicator == "r2") return r2(point_set);
    if(q_indicator == "nr2") return -nr2(point_set);
    if(q_indicator == "senergy") return senergy(point_set);
    if(q_indicator == "epsilon") return epsilon(point_set);
    cerr << q_indicator << " is not defined." << endl;
    exit(1);
}

double Indicator::eval_binary(const vector<int>& x, const vector<vector<double>>& point_set) {
    vector<vector<double>> point_subset;
    for(int i = 0; i < x.size(); i++) {
        if(x[i] == 1) {
            point_subset.push_back(point_set[i]);
        }
    }
    return eval(point_subset);
}

double Indicator::hv(vector<vector<double>>& point_set) {
    pagmo::hvwfg hvwfg(10);
    auto x = hvwfg.compute(point_set, this->ref_point);
    return x;
}

double Indicator::igd(const vector<vector<double>>& point_set) {
    assert(!ref_point_set.empty());
    int d = point_set[0].size();
    double total_sum_dist = 0;
    for(int i = 0; i < ref_point_set.size(); i++) {
        double min_dist = INF;
        for(int j = 0; j < point_set.size(); j++) {
            double sum = 0;
            for(int k = 0; k < d; k++) {
                double x = ref_point_set[i][k] - point_set[j][k];
                sum += x * x;
            }
            double dist = sqrt(sum);
            min_dist = min(min_dist, dist);
        }
        total_sum_dist += min_dist;
    }
    return total_sum_dist / (double)ref_point_set.size();
}

double Indicator::igd_plus(const vector<vector<double>>& point_set) {
    assert(!ref_point_set.empty());
    int d = point_set[0].size();
    double total_sum_dist = 0;
    for(int i = 0; i < ref_point_set.size(); i++) {
        double min_dist = INF;
        for(int j = 0; j < point_set.size(); j++) {
            double sum = 0;
            for(int k = 0; k < d; k++) {
                double x = max(point_set[j][k] - ref_point_set[i][k], 0.0);
                sum += x * x;
            }
            double dist = sqrt(sum);
            min_dist = min(min_dist, dist);
        }
        total_sum_dist += min_dist;
    }
    return total_sum_dist / (double)ref_point_set.size();
}

double Indicator::r2(const vector<vector<double>>& point_set) {
    assert(!weight_point_set.empty());
    int d = point_set[0].size();
    double sum_tch = 0;
    for(int i = 0; i < weight_point_set.size(); i++) {
        double min_tch = +INF;
        for(int j = 0; j < point_set.size(); j++) {
            double tch = -INF;
            for(int k = 0; k < d; k++) {
                double tmp = weight_point_set[i][k] * abs(point_set[j][k]);
                tch = max(tch, tmp);
            }
            min_tch = min(min_tch, tch);
        }
        sum_tch += min_tch;
    }
    return sum_tch / double(weight_point_set.size());
} 

double Indicator::nr2(const vector<vector<double>>& point_set) {
    assert(!weight_point_set.empty());
    assert(!ref_point.empty());
    double sum_mtch = 0;
    int d = point_set[0].size();
    for(int i = 0; i < weight_point_set.size(); i++) {
        double max_mtch = -INF;
        for(int j = 0; j < point_set.size(); j++) {
            double min_mtch = +INF;
            for(int k = 0; k < d; k++) {
                double w = weight_point_set[i][k];
                if(w == 0) w = 1e-6;
                double tmp = abs(ref_point[k] - point_set[j][k]) / w;
                min_mtch = min(min_mtch, tmp);
            }
            max_mtch = max(max_mtch, min_mtch);
        }
        max_mtch = pow(max_mtch, d);
        sum_mtch += max_mtch;
    }
    return sum_mtch / double(weight_point_set.size());
}

double Indicator::senergy(const vector<vector<double>>& point_set) {
    int d = point_set[0].size();
    double s_energy_value = 0;
    int s_control_value = d - 1;
    for(int i = 0; i < point_set.size(); i++) {
        for(int j = 0; j < point_set.size(); j++) {
            if(j != i) {
                double sum = 0;
                for(int k = 0; k < d; k++) {
                    double x = point_set[i][k] - point_set[j][k];
                    sum += x * x;
                }
                sum = sqrt(sum);
                if(sum > 0)
                    s_energy_value += pow(sum, -s_control_value);
                else
                    return 1e30;
            }
        }
    }
    return s_energy_value;
}

double Indicator::epsilon(const vector<vector<double>>& point_set) {
    assert(!ref_point_set.empty());
    double max_eps_i = -INF;
    int d = point_set[0].size();
    for(int i = 0; i < ref_point_set.size(); i++) {
        double min_eps_j = +INF;
        for(int j = 0; j < point_set.size(); j++) {
            double max_eps_k = -INF;
            for(int k = 0; k < d; k++) {
                double eps_k = point_set[j][k] - ref_point_set[i][k];
                max_eps_k = max(max_eps_k, eps_k);
            }
            min_eps_j = min(min_eps_j, max_eps_k);
        }
        max_eps_i = max(max_eps_i, min_eps_j);
    }
    return max_eps_i;
}