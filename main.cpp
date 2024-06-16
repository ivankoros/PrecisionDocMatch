#include <rapidfuzz/fuzz.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <list>
#include <thread>
using namespace std::chrono;
using namespace std;

vector<vector<string>> load_csv(const string & path) {
    ifstream indata;
    indata.open(path);
    string line;
    vector<vector<string>> data;
    while (getline(indata, line)) {
        stringstream lineStream(line);
        string cell;
        vector<string> row;
        while (getline(lineStream, cell, ',')) {
            row.push_back(cell);
        }
        data.push_back(row);
    }
    return data;
}

void print_matrix(vector<vector<string>> data, const int limit = 500) {
    auto rows = data.size() - 1;
    auto cols = data[0].size();

    cout << "rows: " << rows << " cols: " << cols << "\n";
    for (int i = 0; i <= rows & i < limit; i++) {
        for (int j = 0; j < cols; j++) {
            cout << data[i][j];
            if (j < cols - 1) {
                cout << " ";
            }
        }
        cout << endl;
    }
}

auto find_best_fuzzy_match(const vector<vector<string>>* data,
                                             const string& first_name,
                                             const string& last_name) {
    std::tuple<double, int> best_match_index;
    for (int i = 1; i < (*data).size(); i++) {
        std::list<double> scores;
        double first_name_score = rapidfuzz::fuzz::ratio((*data)[i][11], first_name);
        double last_name_score = rapidfuzz::fuzz::ratio((*data)[i][14], last_name);
        scores.push_back(first_name_score);
        scores.push_back(last_name_score);

        double average_score = (first_name_score + last_name_score) / 2.0;

        if (average_score > get<0>(best_match_index)) {
            get<0>(best_match_index) = average_score;
            get<1>(best_match_index) = i;
        }
    }
    return best_match_index;
}

unordered_map<string, int> get_column_indicies(const vector<vector<string>>& dataframe,
                                               bool print = false) {
    unordered_map<string, int> column_indicies;
    for (int i = 0; i < dataframe[0].size(); i++) {
        column_indicies[dataframe[0][i]] = i;
        if (print) {
            cout << dataframe[0][i] << " at index " << i << "\n";
            }
    }
    return column_indicies;
}

void run_fuzzy_function(const vector<vector<string>>* data,
                        const string& first_name,
                        const string& last_name,
                        const int& run_amount) {

    for (int i = 0; i < run_amount; i++) {
        auto best_match = find_best_fuzzy_match(data, first_name, last_name);
    }

}

void speed_profile(const vector<vector<string>>* data) {
    int con_threads = static_cast<int>(thread::hardware_concurrency());

    vector <thread> threads;
    threads.reserve(con_threads);

    int runs_per_thread = 1000 / con_threads;

    auto start = high_resolution_clock::now();
    for (int i = 0; i < con_threads; i++) {
        threads.emplace_back(run_fuzzy_function, data, "Yolo", "Younman", runs_per_thread);
    }

    for (auto &thread : threads) {
        thread.join();
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << "Time taken by function: "
         << duration.count() << " microseconds "
         <<  "average time per run: "
         << duration.count() / 1000 << " microseconds\n"
         << endl;
}

int main()
{
    auto data_loaded = load_csv("/Users/koros/CLionProjects/PrecisionDocMatch/All Active Students.csv");
    auto& data = data_loaded;


    return 0;
}
