#include <fstream>
#include <vector>
#include <iostream>
#include <random>
#include <set>
#include <algorithm>
#include <functional>
#include <chrono>
#include <tuple>
#include "zonemaps.h"
#include "zonemaps.cpp"
#include <filesystem>

// Specify your path of workload file here
std::string kInputDataPath = "./workload.dat";
std::string kPointQueriesPath = "./point_queries.txt";
std::string kRangeQueriesPath = "./range_queries.txt";
const uint32_t kRuns = 3;
using namespace std;

void loadPointQueries(std::string & input_queries_path, std::vector<int> & queries) {

  queries.clear();
  std::__fs::filesystem::current_path("/Users/lucas/Desktop/Research/CS561/cs561_templatezonemaps2");

  std::ifstream infile(input_queries_path, ios::in);
   
  int tmp;
  while (infile >> tmp) {
	  queries.push_back(tmp);
  }
  // shuffle indexes
  std::default_random_engine rng(std::random_device{}());
  std::shuffle(queries.begin(), queries.end(), rng);

}

void loadRangeQueries(std::string & input_queries_path, std::vector<std::tuple<int, int>> & queries_range) {

  queries_range.clear();
  std::__fs::filesystem::current_path("/Users/lucas/Desktop/Research/CS561/cs561_templatezonemaps2");

  std::ifstream infile(input_queries_path, ios::in); 
  string tmp;
  int lineCount = 0;

  while (infile >> tmp) {
    size_t spacePos = tmp.find(',');
    int num1 = std::stoi(tmp.substr(0, spacePos));
    int num2 = std::stoi(tmp.substr(spacePos + 1));
    queries_range.push_back(std::make_tuple(num1, num2));
  }
  auto closeStart = std::chrono::high_resolution_clock::now();
  infile.close();
  
  // shuffle indexes
  std::default_random_engine rng(std::random_device{}());
  auto closeEnd = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> closeElapsed = closeEnd - closeStart;
  std::shuffle(queries_range.begin(), queries_range.end(), rng);
}

int main(int argc, char **argv)
{ 
  // read data

  std::ifstream ifs;
  std::vector<int> data;
  ifs.open(kInputDataPath, std::ios::binary);
  ifs.seekg(0, std::ios::end);
  size_t filesize = ifs.tellg();
  ifs.seekg(0, std::ios::beg);
  data.resize(filesize / sizeof(int));
  ifs.read((char *)data.data(), filesize);

  //1. ----------------------------- initialize zonemap and build -----------------------------
  //build zonemap
  zonemap<int> zones(data, (uint)data.size() / 100);

  //2. ----------------------------- point queries -----------------------------
  std::vector<int> queries;
  loadPointQueries(kPointQueriesPath, queries); 
  auto start_pq = std::chrono::high_resolution_clock::now();
  for (size_t r = 0; r < kRuns; r++) {
    for (int pq: queries) {
      // query from zonemaps here 
      zones.query(pq);
    }

    // shuffle indexes
    std::default_random_engine rng(std::random_device{}());
    std::shuffle(queries.begin(), queries.end(), rng);
  }
  auto stop_pq = std::chrono::high_resolution_clock::now();
  auto duration_pq = std::chrono::duration_cast<std::chrono::microseconds>(stop_pq - start_pq);
  unsigned long long point_query_time = duration_pq.count();
  std::cout << "Time taken to perform point queries from zonemap = " << point_query_time*1.0/kRuns << " microseconds" << endl;
    
  //3. ----------------------------- range queries -----------------------------

  std::vector<std::tuple<int, int>> queries_range;
  loadRangeQueries(kRangeQueriesPath, queries_range); 
  auto start_rq = std::chrono::high_resolution_clock::now();
  for (size_t r = 0; r < kRuns; r++) {
    for (std::tuple<int, int> rq: queries_range) {
      // query from zonemaps here 
      zones.query(std::get<0>(rq), std::get<1>(rq));
    }
    // shuffle indexes
    std::default_random_engine rng(std::random_device{}());
    std::shuffle(queries.begin(), queries.end(), rng);
  }

  auto stop_rq = std::chrono::high_resolution_clock::now();
  auto duration_rq = std::chrono::duration_cast<std::chrono::microseconds>(stop_rq - start_rq);
  unsigned long long range_query_time = duration_rq.count();
  std::cout << "Time taken to perform range query from zonemap = " << range_query_time*1.0/kRuns << " microseconds" << endl;


  return 0;
}
