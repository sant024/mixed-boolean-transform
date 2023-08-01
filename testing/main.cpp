#include <iostream>

// insert zero mba in between x + mba + y
int fun_a(int x, int y){
	return x + y;  
}

int fun_b(int x, int y){
	int z = fun_a(x, y);
	int q = 1020; // constant mba 
	return z + q; 
}

int fun_c(int x, int y){
	int n = x + y; // insert zero mba in between x + mba + y
	return fun_b(x, y);
	
}

int main() {
	std::cout << "Value is: " << fun_c(40,20) << std::endl;  
	return 0;
}
