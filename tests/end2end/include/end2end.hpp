#pragma once

#include <vector>
#include <concepts>
#include <random>
#include <fstream>
#include <filesystem>
#include <utility>
#include <string>

#include "sparse_table.hpp"

namespace testing {

namespace dirs {
  const std::string resource_dir = "../tests/end2end/resources";
  const std::string tests_dir    = "../tests/end2end/resources/tests/";
  const std::string ans_dir      = "../tests/end2end/resources/answers/";
}

template<std::integral T>
class generator final {
  using size_type         = std::size_t;
  using value_type        = T;
  using generator_type    = std::mt19937;
  using distribution_type = std::uniform_int_distribution<T>;
  
  static constexpr int64_t MIN_VALUE                = -1000000;
  static constexpr int64_t MAX_VALUE                = 1000000;
  static constexpr size_type DEFAULT_SIZE           = 1000000;
  static constexpr size_type DEFAULT_QUERIES_NUMBER = 1000000;

  template <std::integral U>
  U random_value(U min_val, U max_val) {
      distribution_type distr(min_val, max_val);
      return distr(generator_);
  }

  void create_source_directory() {
      using namespace std::filesystem;
      directory_entry resource_dir(dirs::resource_dir);

      if (!resource_dir.is_directory()) {
          create_directory(dirs::resource_dir);
          create_directory(dirs::tests_dir);
          create_directory(dirs::ans_dir);
      } else {
          const path tests_path{dirs::tests_dir};
          const path answs_path{dirs::ans_dir};

          for (auto& dir_iter : directory_iterator{tests_path}) {
              remove(dir_iter.path());
          }
          for (auto& dir_iter : directory_iterator{answs_path}) {
              remove(dir_iter.path());
          }
      }
  }

  void generate_array_and_queries(size_type test_number) { 
    std::string test_name = "test" + std::to_string(test_number) + ".txt";
    std::string ans_name  = "answer" + std::to_string(test_number) + ".txt";
    std::ofstream test_file {dirs::tests_dir + test_name};
    std::ofstream ans_file  {dirs::ans_dir + ans_name};
    
    // generating array
    std::vector<int64_t> array(array_size_);
    std::generate(array.begin(), array.end(), random_value(min_value_, max_value_));
    for (auto &&v : array) {
      test_file << "k " << v << ' ';
    }
    // generating queries
    std::vector<std::pair<size_type, size_type>> queries(queries_num_);
    std::generate(array.begin(), array.end(), [size = array.size()] {
                       auto l = random_value(0, size);
                       auto r = random_value(l, size);
                       return std::make_pair(l, r);
                  });
    // generating answers with sparse table
    yLAB::SparseTable<int64_t> sparse_table(array.begin(), array.end(), array.size());
    std::transform(queries.begin(), queries.end(), std::ostream_iterator<int64_t>(ans_file, " "),
                  [&sparse_table](auto &&pair) {
      return  sparse_table.min(pair.first, pair.second); 
    });
  }


 public:
  generator(size_type tests_number, int64_t min = MIN_VALUE, int64_t max = MAX_VALUE,
            size_type arr_sz = DEFAULT_SIZE, size_type queries = DEFAULT_QUERIES_NUMBER)
      : tests_number_ {tests_number},
        min_value_ {min},
        max_value_ {max},
        array_size_ {arr_sz},
        queries_num_ {queries} {
    generator_.seed(static_cast<size_type>(std::time(nullptr)));
  }

  void generate_tests() {
    create_source_directory();

    for (size_type counter = 1; counter <= tests_number_; ++counter) {
        generate_array_and_queries(counter);
    }
  }
 private:
  size_type tests_number_;
  int64_t min_value_;
  int64_t max_value_;
  size_type array_size_;
  size_type queries_num_;
  generator_type generator_;
};

} // <--- namespace testing

