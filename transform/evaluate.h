#pragma once
#include <Eigen/Dense>
#include "table.h"
class Evaluate {
public:
	Evaluate(int t);
	Eigen::VectorXd solution;
	std::string get_expression(Table& table);
private:

};