#include "table.h"
#include <iostream>


Table::Table(int t= 4) : rng(std::time(0)), dist(0, ExpressionTruthTableVec.size() - 1), column_matrix(4, 4) { //5 
    this->t = t;
    
     // k =t
    for (int i = 0; i < t; ++i) {
        std::size_t idx = dist(rng);
        std::cout << "random index: " << idx << std::endl;
        initial_truth_table.push_back(ExpressionTruthTableVec[idx].truth_table);
    }

    //    std::vector<int> l1 = { 1, 0, 1, 1 };
    //    std::vector<int> l2 = { 0, 1, 1, 0 };
    //    std::vector<int> l3 = { 0, 1, 0, 0 };
    //    std::vector<int> l4 = { 1, 0, 0, 1 };
    //initial_truth_table.push_back(l1);
    //initial_truth_table.push_back(l2);
    //initial_truth_table.push_back(l3);
    //initial_truth_table.push_back(l4); 

    // fix to add variables
    for (const auto& vec : initial_truth_table) {
        Eigen::VectorXd _vec(4); // truth vec 
        std::cout << "Push row as VectorXd:";
        for (int i = 0; i < 4; ++i) {
            std::cout << " " << vec[i];
            _vec[i] = vec[i];
        }
        std::cout << " <" << std::endl;

        initial_truth_table_eig.push_back(_vec);
    }


    // Stack the column vectors horizontally to create a matrix
    //for (const auto& vecxd : initial_truth_table_eig) {
    //    column_matrix << vecxd;
    //} doesn't work for some reason

    for (size_t i = 0; i < initial_truth_table_eig.size(); ++i) {
        column_matrix.col(i) = initial_truth_table_eig[i];
    }


    //column_matrix << initial_truth_table_eig[0], initial_truth_table_eig[1], initial_truth_table_eig[2], initial_truth_table_eig[3];
    
   
    
    std::cout << "Column Matrix: (stacked by column) \n" << column_matrix << std::endl;
 
}

Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> Table::get_matrix()
{
    std::cout << "Get Matrix\n";
    Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> F(r, c);
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            F(i, j) = column_matrix(i, j);
        }
    }

    //for (int i = 0; i < r; ++i) {
    //    for (int j = 0; j < c; ++j) {
    //        std::cout << F(i, j) << " ";
    //    }
    //    std::cout << std::endl;
    //}
        return F;
}

