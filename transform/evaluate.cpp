#pragma warning(disable:4146)
#include "evaluate.h"
#include <iostream>
#include "util.h"

#include <gmp.h>
#include <regex>


Evaluate::Evaluate(int t) : solution(t)
{

}

std::string Evaluate::get_expression(Table &table)
{
    std::cout << "Solution from Z3: \n" << solution << std::endl;
    std::vector<std::vector<int>> initial_table = table.initial_truth_table;
    auto current_matrix = table.get_matrix();

    std::cout << "current matrix size should be 4: " << current_matrix.innerSize() << std::endl;
    std::string result = "";
    for (int i = 0; i < current_matrix.innerSize(); i++) {

        auto truth_vec = initial_table[i]; // error here
        std::cout << "test:\n";
        print_vec(truth_vec);

        //std::vector<int> cur_temp = column_matrix[i];

        std::string to_expr = get_expr_from_vec(truth_vec);

        std::ostringstream ss;
        ss.precision(0);
        ss << std::fixed << solution(i);
        std::cout << ss.str() << std::endl;

        result.append(ss.str());
        result.append("*");
        result.append(to_expr);

        if (i != current_matrix.innerSize()- 1)
        result.append("+");
    }

    //std::cout << result << std::endl;
    return result;
}


std::pair<std::pair<std::string, std::string>, std::pair<std::string, std::string>> get_invertible_affine(int n) {
    mpz_t one, a_value, b_value;  // https://stackoverflow.com/questions/30942413/c-gmp-generating-random-number   
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
    // b start 
    mpz_urandomb(b_value, rstate, n);
    mpz_sub(b_value, b_value, one);
    const char* b_value_str = mpz_get_str(NULL, 10, b_value);
    std::cout << "(b) random number generated 0-2^n-1: " << b_value_str << std::endl;
    //    code 3 (0x3).
    mpz_t modulus;
    mpz_init(modulus);
    mpz_ui_pow_ui(modulus, 2, n);

    mpz_t a_inv, negative_one;
    mpz_init(a_inv);

    mpz_init(negative_one);
    mpz_set_si(negative_one, -1); // https://stackoverflow.com/questions/11181172/using-the-mpz-powm-functions-from-the-gmp-mpir-libraries-with-negative-exponents

    mpz_powm(a_inv, a_value, negative_one, modulus);

    const char* a_inv_str = mpz_get_str(NULL, 10, a_inv);
    std::cout << "(a)inverse: " << a_inv_str << std::endl;

    mpz_t b_inv;
    mpz_init(b_inv);

    mpz_t a_inv_neg;
    mpz_init(a_inv_neg);

    mpz_neg(a_inv_neg, a_inv);
    mpz_mul(b_inv, a_inv_neg, b_value);

    std::cout << " test -a_inv * b : " << mpz_get_str(NULL, 10, b_inv) << std::endl;

    mpz_mod(b_inv, b_inv, modulus);
    const char* b_inv_str = mpz_get_str(NULL, 10, b_inv);
    std::cout << "(b)inverse: " << b_inv_str << std::endl;

    return std::make_pair(std::make_pair(a_value_str, b_value_str), std::make_pair(a_inv_str, b_inv_str));
}

std::string solv(const std::string& input) {
    const std::regex pattern(R"(\((\d+)\)\s*\*\s*\((\d+)\)\s*\+\s*\((\d+)\))");
    std::string first_match, second_match, third_match;
    mpz_t x_value, y_value, z_value, result;
    std::smatch matches;
    mpz_init(x_value);
    mpz_init(y_value);
    mpz_init(z_value);
    mpz_init(result);
    std::string out = "";

    if (std::regex_match(input, matches, pattern)) {
        first_match = matches[1].str();
        second_match = matches[2].str();
        third_match = matches[3].str();
        std::cout << "found: " << first_match;
        std::cout << "\nfound: " << second_match;
        std::cout << "\nfound: " << third_match;
        mpz_set_str(x_value, first_match.c_str(), 10);
        mpz_set_str(y_value, second_match.c_str(), 10);
        mpz_set_str(z_value, third_match.c_str(), 10);

        mpz_mul(result, x_value, y_value);
        mpz_add(result, result, z_value);
        out = mpz_get_str(NULL, 10, result);
    }

    std::cout << "solved  p (eval) = " << out << std::endl;
    return out;
}

void Evaluate::constant(std::string& z) {
    int k = 1020;  // put the constant here, for example : 1020
    int k_shift = 1020;
    int k_bit_len = 0;

    while (k_shift > 0) {
        k_shift >>= 1;
        k_bit_len++;
    }

    std::random_device rd;
    std::mt19937 generator(rd());

    int minVal = k_bit_len + 1;
    std::cout << "k bit len + 1 : " << minVal << std::endl;
    int maxVal = 100;

    std::uniform_int_distribution<int> distribution(minVal, maxVal);
    int n = distribution(generator);
    std::cout << "N value argument: " << n << std::endl;

    auto coeffs = get_invertible_affine(n);

    std::cout << "coeffs: " << coeffs.first.first << " " << coeffs.first.second << " " << coeffs.second.first << " " << coeffs.second.second << std::endl;
    std::string to_replace = "replace";
    std::string p = "(" + coeffs.first.first + ")" + +"*(" + std::to_string(k) + ")+(" + coeffs.first.second + ")";
    std::string q = "(" + coeffs.second.first + ")" + "*(" + to_replace + ")+(" + coeffs.second.second + ")";

    std::cout << "P: " << p << std::endl;
    std::cout << "Q: " << q << std::endl;


    // p val
    std::cout << "p_val not eval " << p << std::endl;

    std::string p_val = solv(p);
    std::cout << "p_val evaluated: " << p_val << std::endl;

    // 114503850325760475253189*(129)+(313523368772846873043148)

    size_t pos = q.find(to_replace);
    if (pos != std::string::npos)
        q.replace(pos, to_replace.length(), "(" + z + ")+(" + p_val + ")");
    // q val

    std::string out = "(" + q + ")" + "%2**" + std::to_string(n);

    std::cout << " go: " << out << std::endl;
}
