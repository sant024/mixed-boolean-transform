#include <iostream>

int fun_a(int x, int y){
    return x + ((346944530715156030497453)*((-2*(~(x | y))+-1*(x & ~y)+1*(~y)+1*(~(x | y)))+(308924813101144738138186))+(285590800713089855787518))%2**79 + y;
}

int fun_b(int x, int y){
    int z = fun_a(x, y);
    return z + ((312646088269814805846141)*((-2*(x & y)+-2*(x ^ y)+1*(x | y)+1*(x | y))+(223327974010260293607636064))+(539947916355369829107996))%2**79; 
}

int fun_c(int x, int y){
    int n = x + ((3174376919201385605733)*((-1*(x | ~y)+1*(y)+-1*(~(x & y))+2*(~y))+(151660436265643080081342))+(589456583812029285273098))%2**79 + y;
    return fun_b(x, y);
    
}

int main() {
    std::cout << "Value is: " << fun_c(40,20) << std::endl;  
    return 0;
}


// Must be rewritten because C++ doesn't have built-in big integer types by default
// 
