#include <iostream>
#include <vector>
#include <random>
#include <Eigen/Dense>
#include <z3++.h>
#include "table.h"
#include "engine.h"
#include "evaluate.h"
#include "util.h"

int main() {
    std::string z;
    // to obfuscate constants and expressions
    int var_count = 4; 
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

            eval.constant(z);
        }
    }



    std::cout << "complete\n";
    return 0;
}


