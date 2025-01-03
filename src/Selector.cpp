#include "Selector.hpp"

Selector::Selector(vector<vector<double>>& point_set, int point_subset_size, Indicator& I, mt19937 rnd_engine, int nlist_size, int rlist_size) {
    this->point_set = point_set;
    this->point_set_size = point_set.size();
    this->point_subset_size = point_subset_size;
    this->d = point_set[0].size();
    this->I = I;
    this->rnd_engine = rnd_engine;
    this->nlist_size = nlist_size;
    this->rlist_size = rlist_size;
}

vector<vector<double>> Selector::select(string algorithm) {
    if(algorithm == "fils") return LS();
    if(algorithm == "fils-nlist") return LS_N();
    if(algorithm == "fils-rlist") return LS_R();
    if(algorithm == "fils-rlist-nlist") return LS_RN();
    cerr << "Error. " << algorithm << " is not defined" << endl;
    exit(1);
}

vector<vector<double>> Selector::LS() {
    vector<vector<double>> point_subset;

    vector<int> shuffled_ids(point_set_size);
    iota(shuffled_ids.begin(), shuffled_ids.end(), 0);

    vector<int> x(point_set_size, 0);
    vector<int> sel_ids(point_subset_size);
    vector<int> not_sel_ids(point_set_size - point_subset_size);

    shuffle(shuffled_ids.begin(), shuffled_ids.end(), rnd_engine);
    for(int i = 0; i < point_subset_size; i++) {
        x[shuffled_ids[i]] = 1;
        sel_ids[i] = shuffled_ids[i];
    }
    for(int i = point_subset_size; i < point_set_size; i++) {
        x[shuffled_ids[i]] = 0;
        not_sel_ids[i - point_subset_size] = shuffled_ids[i];
    }

    double bsf_qi_value = I.eval_binary(x, point_set);

    double qi_value;
    bool improvement_exist = true;

    while(improvement_exist == true) {
        improvement_exist = false;

        for(int i = 0; i < point_subset_size; i++) {
            for(int j = 0; j < point_set_size - point_subset_size; j++) {
                swap(x[sel_ids[i]], x[not_sel_ids[j]]);
                qi_value = I.eval_binary(x, point_set);

                if(qi_value < bsf_qi_value) {
                    bsf_qi_value = qi_value;
                    swap(sel_ids[i], not_sel_ids[j]);
                    improvement_exist = true;
                    break;
                } else {
                    swap(x[sel_ids[i]], x[not_sel_ids[j]]);
                }
            }
        }
    }

    for(int i = 0; i < point_subset_size; i++) {
        point_subset.push_back(point_set[sel_ids[i]]);
    }
    return point_subset;
}

vector<vector<double>> Selector::LS_N() {
    vector<vector<double>> point_subset;

    vector<int> shuffled_ids(point_set_size);
    iota(shuffled_ids.begin(), shuffled_ids.end(), 0);

    vector<int> x(point_set_size, 0);
    vector<int> sel_ids(point_subset_size);
    shuffle(shuffled_ids.begin(), shuffled_ids.end(), rnd_engine);
    for(int i = 0; i < point_subset_size; i++) {
        x[shuffled_ids[i]] = 1;
        sel_ids[i] = shuffled_ids[i];
    }

    vector<vector<double>> dist_matrix(point_set_size, vector<double>(point_set_size, 1e20));
    for(int i = 0; i < point_set_size; i++) {
        for(int j = 0; j < i; j++) {
            double dist = 0;
            for(int k = 0; k < d; k++) {
                double x = point_set[i][k] - point_set[j][k];
                dist += x * x;
            }
            dist = sqrt(dist);
            dist_matrix[i][j] = dist_matrix[j][i] = dist;
        }
    }

    auto argsort = [&](const vector<double> &v) {
        int n = v.size();
        vector<int> ids(n);
        iota(ids.begin(), ids.end(), 0);
        sort(ids.begin(), ids.end(), [&](int i, int j){ return v[i] < v[j]; });
        return ids;
    };

    vector<vector<int>> nlist(point_set_size, vector<int>(nlist_size));
    for(int i = 0; i < point_set_size; i++) {
        vector<int> sorted_ids = argsort(dist_matrix[i]);
        for(int j = 0; j < nlist_size; j++) nlist[i][j] = sorted_ids[j];
    }

    double bsf_qi_value = I.eval_binary(x, point_set);
    double qi_value;
    bool improvement_exist = true;

    while(improvement_exist == true) {
        improvement_exist = false;

        for(int i = 0; i < point_subset_size; i++) {
            for(int k = 0; k < nlist_size; k++) {
                int j = nlist[sel_ids[i]][k];
                if(x[j] == 0) {
                    swap(x[sel_ids[i]], x[j]);
                    qi_value = I.eval_binary(x, point_set);

                    if(qi_value < bsf_qi_value) {
                        bsf_qi_value = qi_value;
                        sel_ids[i] = j;
                        improvement_exist = true;
                        break;
                    } else {
                        swap(x[sel_ids[i]], x[j]);
                    }
                }
            }
        }
    }

    for(int i = 0; i < point_subset_size; i++) {
        point_subset.push_back(point_set[sel_ids[i]]);
    }
    return point_subset;
}

vector<vector<double>> Selector::LS_R() {
    vector<vector<double>> point_subset;

    vector<int> shuffled_ids(point_set_size);
    iota(shuffled_ids.begin(), shuffled_ids.end(), 0);

    vector<int> x(point_set_size, 0);
    vector<int> sel_ids(point_subset_size);
    shuffle(shuffled_ids.begin(), shuffled_ids.end(), rnd_engine);
    for(int i = 0; i < point_subset_size; i++) {
        x[shuffled_ids[i]] = 1;
        sel_ids[i] = shuffled_ids[i];
    }

    vector<vector<int>> rlist(point_set_size, vector<int>(rlist_size));
    for(int i = 0; i < point_set_size; i++) {
        shuffle(shuffled_ids.begin(), shuffled_ids.end(), rnd_engine);
        for(int j = 0; j < rlist_size; j++) rlist[i][j] = shuffled_ids[j];
    }

    double bsf_qi_value = I.eval_binary(x, point_set);
    double qi_value;
    bool improvement_exist = true;

    while(improvement_exist == true) {
        improvement_exist = false;

        for(int i = 0; i < point_subset_size; i++) {
            for(int k = 0; k < (int)rlist[sel_ids[i]].size(); k++) {
                int j = rlist[sel_ids[i]][k];
                if(x[j] == 0) {
                    swap(x[sel_ids[i]], x[j]);
                    qi_value = I.eval_binary(x, point_set);

                    if(qi_value < bsf_qi_value) {
                        bsf_qi_value = qi_value;
                        sel_ids[i] = j;
                        improvement_exist = true;
                        break;
                    } else {
                        swap(x[sel_ids[i]], x[j]);
                    }
                }
            }
        }
    }

    for(int i = 0; i < point_subset_size; i++) {
        point_subset.push_back(point_set[sel_ids[i]]);
    }
    return point_subset;
}

vector<vector<double>> Selector::LS_RN() {
    vector<vector<double>> point_subset;

    vector<int> shuffled_ids(point_set_size);
    iota(shuffled_ids.begin(), shuffled_ids.end(), 0);

    vector<int> x(point_set_size, 0);
    vector<int> sel_ids(point_subset_size);
    shuffle(shuffled_ids.begin(), shuffled_ids.end(), rnd_engine);
    for(int i = 0; i < point_subset_size; i++) {
        x[shuffled_ids[i]] = 1;
        sel_ids[i] = shuffled_ids[i];
    }

    vector<vector<double>> dist_matrix(point_set_size, vector<double>(point_set_size, 1e20));
    for(int i = 0; i < point_set_size; i++) {
        for(int j = 0; j < i; j++) {
            double dist = 0;
            for(int k = 0; k < d; k++) {
                double x = point_set[i][k] - point_set[j][k];
                dist += x * x;
            }
            dist = sqrt(dist);
            dist_matrix[i][j] = dist_matrix[j][i] = dist;
        }
    }

    auto argsort = [&](const vector<double> &v) {
        int n = v.size();
        vector<int> ids(n);
        iota(ids.begin(), ids.end(), 0);
        sort(ids.begin(), ids.end(), [&](int i, int j){ return v[i] < v[j]; });
        return ids;
    };

    vector<vector<int>> nlist(point_set_size, vector<int>(nlist_size));
    for(int i = 0; i < point_set_size; i++) {
        vector<int> sorted_ids = argsort(dist_matrix[i]);
        for(int j = 0; j < nlist_size; j++) nlist[i][j] = sorted_ids[j];
    }

    vector<vector<int>> rlist(point_set_size, vector<int>(rlist_size));
    for(int i = 0; i < point_set_size; i++) {
        shuffle(shuffled_ids.begin(), shuffled_ids.end(), rnd_engine);
        for(int j = 0; j < rlist_size; j++) rlist[i][j] = shuffled_ids[j];
    }

    double bsf_qi_value = I.eval_binary(x, point_set);
    double qi_value;
    bool improvement_exist = true;

    while(improvement_exist == true) {
        improvement_exist = false;

        for(int i = 0; i < point_subset_size; i++) {
            for(int k = 0; k < (int)rlist[sel_ids[i]].size(); k++) {
                int j = rlist[sel_ids[i]][k];
                if(x[j] == 0) {
                    swap(x[sel_ids[i]], x[j]);
                    qi_value = I.eval_binary(x, point_set);

                    if(qi_value < bsf_qi_value) {
                        bsf_qi_value = qi_value;
                        sel_ids[i] = j;
                        improvement_exist = true;
                        break;
                    } else {
                        swap(x[sel_ids[i]], x[j]);
                    }
                }
            }
        }
    }

    improvement_exist = true;
    while(improvement_exist == true) {
        improvement_exist = false;

        for(int i = 0; i < point_subset_size; i++) {
            for(int k = 0; k < (int)nlist[sel_ids[i]].size(); k++) {
                int j = nlist[sel_ids[i]][k];
                if(x[j] == 0) {
                    swap(x[sel_ids[i]], x[j]);
                    qi_value = I.eval_binary(x, point_set);

                    if(qi_value < bsf_qi_value) {
                        bsf_qi_value = qi_value;
                        sel_ids[i] = j;
                        improvement_exist = true;
                        break;
                    } else {
                        swap(x[sel_ids[i]], x[j]);
                    }
                }
            }
        }
    }

    for(int i = 0; i < point_subset_size; i++) {
        point_subset.push_back(point_set[sel_ids[i]]);
    }
    return point_subset;
}
