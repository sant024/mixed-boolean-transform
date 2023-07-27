#include <iostream>
#include <vector>
#include <random>
#include <Eigen/Dense>
#include <z3++.h>
#include "table.h"
#include "engine.h"
#include "evaluate.h"
#include "util.h"
#include "exprtk.hpp"

std::pair<std::pair<int, int>, std::pair<int, int>> get_invertible_affine(int n) {

    std::mt19937 rng(std::time(0));

    std::uniform_int_distribution<int> a_dist(1, (1 << n) - 1);
    int a = a_dist(rng) | 1;

    std::uniform_int_distribution<int> b_dist(0, (1 << n) - 1);
    int b = b_dist(rng);

    std::cout << "affine n: " << n << std::endl;
    std::cout << "A: " << a << " B:" << b << std::endl;
    system("PAUSE");
    // Calculate the modular inverse of 'a'
    int modulus = 1 << n;
    int a_inv = static_cast<int>(std::pow(a, -1)) % modulus;

    int b_inv = (-(static_cast<int64_t>(a_inv) * b) % modulus + modulus) % modulus;

    return std::make_pair(std::make_pair(a, b), std::make_pair(a_inv, b_inv));
}

int main() {
    std::string z;
    // to obfuscate constants and expressions
    int var_count = 4; // 4 only , fix line 27 in table << << matrix
    bool satsfied = false;
    while (!satsfied) {
        Table table = Table(var_count);
        Engine engine = Engine(var_count);
        Evaluate eval = Evaluate(var_count);

        auto F = table.get_matrix(); // by column already
        std::cout << "F: \n" << F << std::endl;
        engine.add_matrix(F);

        //std::cout << "debug this: \n";
        std::cout << "Use to eval:\n" << eval.solution << std::endl;
        satsfied = engine.execute(eval.solution);
        if (satsfied) {
            std::cout << " eval is :\n" << eval.solution << std::endl;
            std::cout << "final: " << eval.get_expression(table) << std::endl;
            z = eval.get_expression(table);
        }
    }

    int k = 129;
    int k_bit_len = 0;

    while (k > 0) {
        k >>= 1; 
        k_bit_len++;
    }

    std::random_device rd;
    std::mt19937 generator(rd());

    int minVal = k_bit_len+1;
    std::cout << "k bit len + 1 : " << minVal << std::endl;
    int maxVal = 100;

    std::uniform_int_distribution<int> distribution(minVal, maxVal);
    int n = distribution(generator);
    std::cout << "N value argument: " << n << std::endl;
    n = 79;
    std::cout << "Should be 79 " << n << std::endl;

    // wrong function for some reaosn (evaluate is inaccurate)
    auto coeffs = get_invertible_affine(n);
    
    
    std::cout << "coeffs: " << coeffs.first.first << " " << coeffs.first.second << " " << coeffs.second.first << " " << coeffs.second.second << std::endl;
    std::string p_arg = generate_random_string(10);
    std::string q_arg = generate_random_string(10);

    std::string p = std::to_string(coeffs.first.first) + "*(" + p_arg + ")+(" + std::to_string(coeffs.first.second) + ")";
    std::string q = std::to_string(coeffs.second.first) + "*(" + q_arg + ")+(" + std::to_string(coeffs.second.second) + ")";

    size_t pos = p.find(p_arg);
    if (pos != std::string::npos)
        p.replace(pos, p_arg.length(), std::to_string(k));
    // p val
    std::cout << "p_val no eval " << p << std::endl;
    typedef exprtk::expression<double> expression_t;
    typedef exprtk::parser<double> parser_t;


    expression_t expression;
    parser_t parser;

    if (!parser.compile(p, expression)) {
        std::cerr << "Unable to parse the expression" << std::endl;
        return 1;

    }

    int p_val = expression.value();
    std::cout << "p_val: " << p_val << std::endl;


    // Step 3
    pos = q.find(q_arg);
    if (pos != std::string::npos)
        q.replace(pos, q_arg.length(), "(" + z + ")+(" + std::to_string(p_val) + ")");
    // q val
  

    std::string out = "(" + q + ")" + "%2**" + std::to_string(n);
    
    std::cout << " go: " << out << std::endl;

    //auto xx = get_invertible_affine(4);
    //std::cout << "see here: " << xx.first.first << std::endl;
    std::cout << "complete\n";
    return 0;
}

