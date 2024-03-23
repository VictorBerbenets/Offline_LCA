# Solving RMQ problem
### About 
This program solves RMQ problem in <O(n), O(1)> complexity. Where preprocessing
takes O(n) and answering each querry - O(1).
The key role here is played by reducing the RMQ problem to LCA and back LCA to
RMQ.
1) First we build a Cartesian tree using an implicit key.
2) Afterwards we solve the problem RMQ +-1 (Eulerian tree traversal)
3) At the end we use the Farah-Colton and Bender algorithm

## Requirements
**cmake** version must be 3.15 or higher  
**gtest** must be installed
## How to build
```bash
git clone git@github.com:VictorBerbenets/Offline_LCA.git
cd Offline_LCA/
cmake -S ./ -B build/ -DCMAKE_BUILD_TYPE=Release .
cd build/
cmake --build .
```
## To Run the program do
```bash
./offline_lca
```
The program will be waiting for input data in stdin in such way:
```bash
<arr_size> num1 num2 ... <queries_num> l1 r1  l2 r2 ...
```
Here `arr_size` - the number of elements of the array, and `queries_num` - the number of queries.  
At the end the program displays answers to all queries. Example:  
input:  
5 1 -1 2 0 5 4 0 0 0 1 0 3 4 4  
output:  
1 -1 -1 5
## How to run tests:
### You can run unit tests:
```bash
./tests/unit
```
### Or you can run end2end tests:
#### In that case you can generate end2end tests do:
```bash
./tests/end2end <tests_number> <min> <max> <arr_sz> <queries_sz>
```
***tests_number*** - the number of tests you want to generate.  
***min*** - minimum number in the array.  
***max*** - maximum number in the array.  
***arr_sz*** - number of elements in the array.  
***queries_sz*** - total number of queries.  
After running you can see a generated directory - `tests/end2end/resources/`.
There will be the tests themselves and the answers to them.
#### To run end2end tests do:
```bash
bash ../tests/end2end/test_runner.sh <test_nu> <tests_directory> <answers_directory>
```
***test_num*** - number of tests you want to run(must be correct number)  
***tests_directory*** - directory with tests.  
***answers_directory*** - directory with the answers.  
By default tests and answers will be launched from the `tests/end2end/resources` directory.
> Note: tests should be named like this: test[i], and the answers to them:
        answer[i], where i is the test number

