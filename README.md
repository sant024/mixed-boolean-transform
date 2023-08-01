
# mixed-boolean-transform
A proof-of-concept program that generates mixed boolean arithmetic expressions capable of hiding information, such as constants within source code.


## Dependencies 
This project requires `z3` , `gmp` , and `eigen3` libraries to be installed on your system. Use `vcpkg` to install these dependencies as it simplifies the process. 

## Installation  

1. Install vcpkg If you don't have `vcpkg` installed on your system, follow the instructions in the official vcpkg repository: [vcpkg](https://github.com/microsoft/vcpkg)
2.  Open a terminal or command prompt and run the following commands using `vcpkg`: 
  
```bash 
# Install z3 
vcpkg install z3 
# Install gmp 
vcpkg install gmp
# Install eigen3
vcpkg install eigen3
```
## Example Use
```cpp
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
	return 0; // Will print 1080
}
```
In this C++ file, we can easily trace that the value will be printed is 1080.

```cpp
#include <iostream>

int fun_a(int x, int y){
    return x + ((346944530715156030497453)*((-2*(~(x | y))+-1*(x & ~y)+1*(~y)+1*(~(x | y)))+(308924813101144738138186))+(285590800713089855787518))%1<<79 + y;
}

int fun_b(int x, int y){
    int z = fun_a(x, y);
    return z + ((312646088269814805846141)*((-2*(x & y)+-2*(x ^ y)+1*(x | y)+1*(x | y))+(223327974010260293607636064))+(539947916355369829107996))%1<<79; 
}

int fun_c(int x, int y){
    int n = x + ((3174376919201385605733)*((-1*(x | ~y)+1*(y)+-1*(~(x & y))+2*(~y))+(151660436265643080081342))+(589456583812029285273098))%1<<79 + y;
    return fun_b(x, y);
    
}

int main() {
    std::cout << "Value is: " << fun_c(40,20) << std::endl;  
    return 0;
```
After, it is now difficult to see what will be printed after passing `40, 20`

## Limitations
It is possible to apply this source-to-source constant transformation on C++ files; however, parsing and handling large integer types would need to be taken into consideration.




