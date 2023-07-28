#pragma warning(disable:4146)
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

#include <gmp.h>


std::pair<std::pair<int, int>, std::pair<int, int>> get_invertible_affine(int n) {
    int a = 1;
    int b = 3;

    // https://stackoverflow.com/questions/30942413/c-gmp-generating-random-number 
    ////////////////////////////////////////////////////////////////////////////////////////////////
    mpz_t one, a_value, b_value;
    mpz_init(one); 
    mpz_init(a_value);
    mpz_init(b_value);

    // setup random 
    gmp_randstate_t rstate;
    gmp_randinit_mt(rstate);
    gmp_randseed_ui(rstate, std::time(0));

    // a start
    mpz_set_ui(one, 1); // for calculating the random number floor
    mpz_urandomb(a_value, rstate, n);  // generate a random integer in the range [0, (2^n)-1]
    mpz_add(a_value, a_value, one); // low floor (case b)
    const char* a_value_str = mpz_get_str(NULL, 10, a_value); 

    std::cout << "(a) random number generated 0-2^n-1: " << a_value_str << std::endl;
    // a end
    // rand;
    // b start 
    mpz_urandomb(b_value, rstate, n);
    mpz_sub(b_value, b_value, one);
    const char* b_value_str = mpz_get_str(NULL, 10, b_value);
    std::cout << "(b) random number generated 0-2^n-1: " << b_value_str << std::endl;
    // b end

    // inverse  
    //mpf_t a_inv; // prepare float type for x/1;
    //mpz_t a_inv; // prepare float type for x/1; (old)
    //mpz_init2(a_inv, 256);
    //mpz_set(a_inv, a_value);
    //mpz_div(a_inv, one, a_inv);
    //std::cout << "a/1 =  " << mpz_get_str(NULL, 10, a_inv) << std::endl;
    //gmp_printf("a/1 = %.100Ff\n", a_inv);
    // https://stackoverflow.com/questions/12804362/gmp-division-precision-or-printing-issue
    // (new)
    //mpf_t a_inv;   (divide something by x/1 then see .0000 trncate 100)
    //mpf_t one_f;
   
    //mpf_init2(a_inv, 256);
    //mpf_init(one_f);

    //mpf_set_z(a_inv, a_value);
    //mpf_set_z(one_f, one);
    //mpf_div(a_inv, one_f, a_inv);
  
    //gmp_printf("%.100Ff\n", a_inv);
    ////divide by 1 (goal now).. Make it precise
    //// then mod  2 * *n
    mpz_t modulus;
    mpz_init(modulus);
    mpz_ui_pow_ui(modulus, 2, n);

    // step 2 inverse [NEW]
    mpz_t a_inv, negative_one;
    mpz_init(a_inv);

    mpz_init(negative_one);
    mpz_set_si(negative_one, -1); // https://stackoverflow.com/questions/11181172/using-the-mpz-powm-functions-from-the-gmp-mpir-libraries-with-negative-exponents

    mpz_powm(a_inv, a_value, negative_one, modulus);

    const char* a_inv_str = mpz_get_str(NULL, 10, a_inv);
    std::cout << "(a)inverse: " << a_inv_str << std::endl;

    // step 3 B inverse 
    mpz_t b_inv;
    mpz_init(b_inv);

    mpz_t a_inv_neg;
    mpz_init(a_inv_neg);
    // ... (code to initialize and set a_value)

    // Make a_value negative
    mpz_neg(a_inv_neg, a_inv);
    mpz_mul(b_inv, a_inv_neg, b_value);
    mpz_mod(b_inv, b_inv, modulus);
    const char* b_inv_str = mpz_get_str(NULL, 10, b_inv);
    std::cout << "(b)inverse: " << b_inv_str << std::endl;



   
    return std::make_pair(std::make_pair(a, b), std::make_pair(1, 3));
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

