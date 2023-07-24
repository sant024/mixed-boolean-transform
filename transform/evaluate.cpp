#include "evaluate.h"
#include <iostream>
#include "util.h"
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
