#pragma once

#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <Eigen/Dense>
#include <Eigen/Core>
struct ExpressionTruthTable {
    std::string expression;
    std::vector<int> truth_table;
};

inline std::vector<ExpressionTruthTable> ExpressionTruthTableVec = {
   {"(x & y)",    {0, 0, 0, 1}},
   {"(x & ~y)",   {0, 0, 1, 0}},
   {"(x)",        {0, 0, 1, 1}},
   {"(~x & y)",   {0, 1, 0, 0}},
   {"(y)",        {0, 1, 0, 1}},
   {"(x ^ y)",    {0, 1, 1, 0}},
   {"(x | y)",    {0, 1, 1, 1}},
   {"(~(x | y))", {1, 0, 0, 0}},
   {"(~(x ^ y))", {1, 0, 0, 1}},
   {"(~y)",       {1, 0, 1, 0}},
   {"(x | ~y)",   {1, 0, 1, 1}},
   {"(~x)",       {1, 1, 0, 0}},
   {"(~x | y)",   {1, 1, 0, 1}},
   {"(~(x & y))", {1, 1, 1, 0}},
   {"(-1)",       {1, 1, 1, 1}}
};

class Table {
public:
    int t; // number of expressions (truth table)
    Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> get_matrix();
    std::vector<std::vector<int>> initial_truth_table{}; // first 4 random ->  use for eval
    Table(int t);
private:
    std::mt19937 rng;
    std::uniform_int_distribution<std::size_t> dist;
    std::vector< Eigen::VectorXd> initial_truth_table_eig{};
    Eigen::MatrixXd column_matrix;
    const int r = 4;
    const int c = 4;
};