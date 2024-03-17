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
  
  static constexpr int64_t MIN_VALUE                = 0;
  static constexpr int64_t MAX_VALUE                = 100000000;
  static constexpr size_type DEFAULT_SIZE           = 10000000;
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
    array_size_ = (array_size_ == 0 ? 1 : array_size_);
    std::vector<int64_t> array(array_size_);
    std::generate(array.begin(), array.end(), [&] { return random_value(min_value_, max_value_); });
    for (auto &&v : array) {
      test_file << "k " << v << ' ';
    }
    // generating queries
    std::vector<std::pair<size_type, size_type>> queries(queries_num_);
    std::generate(queries.begin(), queries.end(), [&, size = array.size()] {
                       auto l = random_value(0ul, size - 1);
                       auto r = random_value(l, size - 1);
                       return std::make_pair(l, r);
                  });
    for (auto &&[l, r] : queries) {
      test_file << "q " << l << ' ' << r << ' '; 
    }
    // generating answers with sparse table
    yLAB::SparseTable<int64_t> sparse_table(array.begin(), array.end(), array.size());
    std::transform(queries.begin(), queries.end(), std::ostream_iterator<int64_t>(ans_file, " "),
                  [&sparse_table](auto &&pair) {
      return  sparse_table.min({pair.first, pair.second}); 
    });
  }

 public:
  generator(size_type tests_number)
      : tests_number_ {tests_number} {
    generator_.seed(static_cast<size_type>(std::time(nullptr)));
  }

  void generate_tests(int64_t min = MIN_VALUE, int64_t max = MAX_VALUE,
                      size_type arr_sz = DEFAULT_SIZE,
                      size_type queries = DEFAULT_QUERIES_NUMBER) {
    create_source_directory();
    min_value_ = min, max_value_ = max, array_size_ = arr_sz,
    queries_num_ = queries;
    for (size_type counter = 1; counter <= tests_number_; ++counter) {
      std::cout << counter << std::endl;
        generate_array_and_queries(counter);
    }
  }

 private:
  size_type tests_number_;
  int64_t min_value_     = MIN_VALUE;
  int64_t max_value_     = MAX_VALUE;
  size_type array_size_  = DEFAULT_SIZE;
  size_type queries_num_ = DEFAULT_QUERIES_NUMBER;
  generator_type generator_;
};

} // <--- namespace testing

