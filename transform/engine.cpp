#include "engine.h"
#include <iostream>

Engine::Engine(int t) : V(ctx), solver(ctx)
{
    this->t = t;
    b << 0, 0, 0, 0;

    for (int i = 0; i < t; ++i) {
        std::string var_name = "x" + std::to_string(i);
        V.push_back(ctx.int_const(var_name.c_str()));
    }

    for (unsigned i = 0; i < V.size(); ++i) {
        std::cout << "Solve for: " << V[i] << std::endl;
    }

    // constraint: var != 0 
    for (unsigned i = 0; i < V.size(); ++i) {
        solver.add(V[i] != 0); //  can't be zero vector 
    }



    // Check if the constraints are satisfiable
    //if (solver.check() == z3::sat) {
}

void Engine::add_matrix(Eigen::MatrixXi F)
{
    for (int i = 0; i < r; ++i) {
        z3::expr_vector row_expr(ctx);
        for (int j = 0; j < t; ++j) {
            row_expr.push_back(F(i, j) * V[j]);
        }
        std::cout << "b(i) " << b(i) << std::endl;
        solver.add(z3::sum(row_expr) == b(i));
    }
    matrix_present = true;
}

bool Engine::execute(Eigen::VectorXd &sorted_sol)
{
    if (!matrix_present) {
        std::cout << "Add matrix first!\n";
        return false;
    }
	
    if (solver.check() == z3::sat) {
        std::cout << "Constraint satisfied\n";


        z3::model model = solver.get_model();
        //Eigen::VectorXd sorted_sol(t);
        for (int i = 0; i < t; ++i) {
            std::string var_name = "v" + std::to_string(i);
            int value = model.eval(V[i]).get_numeral_int();
            std::cout << value << ", ";
            sorted_sol(i) = value;
        }
        std::cout << "<- Engine Solved\n";
        return true;
    }
    else {
        std::cout << "Constraint not satisfied\n";
    }
    return false;
}
