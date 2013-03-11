#include <tbb/tbb.h>
#include <iostream>

#include <CL/cl.hpp>

// I think the idea is we rewrite these
#include "functions.hpp";
// In here, making use of OpenCL
#include "integrate.hpp";

#include "Timer.hpp";

int main(int argc, char **argv){
	std::cout << "hello world" << std::endl;
	std::cin.get();
	return 0;
}
