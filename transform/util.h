
#pragma once
#include <vector>
#include <iostream>
#include "table.h"

inline void print_vec(std::vector<int>& tt) {
    for (int val : tt) {
        std::cout << val << ", ";
    }
    std::cout << "\n";

}

inline std::string generate_random_string(int k) {
    std::string chars = "abcdefghijklmnop";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(chars.begin(), chars.end(), gen);
    return chars.substr(0, k);
}

inline std::string get_expr_from_vec(const std::vector<int>& inputVector) {

    std::cout << "[=]Truth table to converted to expression: " << std::endl;
    for (int value : inputVector) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    for (const auto& match : ExpressionTruthTableVec) {

        if (match.truth_table == inputVector) {
            std::cout << "[=] Converted to: " << match.expression << std::endl;
            return match.expression;
        }
    }


    return "";
}

