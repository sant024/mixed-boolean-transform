#pragma once
#include <z3++.h>
#include <Eigen/Dense>
// Backend supported : Z3 , more soon
class Engine {
public:
	Engine(int t);
	void add_matrix(Eigen::MatrixXi matrix);
	bool execute(Eigen::VectorXd& sorted_sol);
private:
	int t;  // desired number of variables that must evaluate to 0
	z3::context ctx;
	z3::expr_vector V; // xpr_vector to store the integer variables
	z3::solver solver;
	Eigen::Vector4i b;
	bool matrix_present = false;
	const int r = 4;
};