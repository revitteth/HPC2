#define CL_USE_DEPRECATED_OPENCL_1_1_APIS  // My gfx card only supports 1.1
#define __NO_STD_VECTOR // Use cl::vector instead of STL version
#define __CL_ENABLE_EXCEPTIONS

#include <tbb/tbb.h>
#include <CL/cl.hpp>
#include <utility>
#include <iostream>
#include <fstream>
#include <string>

// I think the idea is we rewrite these
#include "functions.hpp"
// In here, making use of OpenCL
#include "integrate.hpp"

#include "dumb_impl.hpp"

#include "Timer.hpp"

int main(int argc, char **argv){ 

	//std::cout << "		0" << std::endl;
	//std::cout << "GPU " << Test0()->totalTime() << std::endl;
	//std::cout << "CPU " << Test0_dumb()->totalTime() << std::endl;
	//std::cout << "		1" << std::endl;
	//std::cout << "GPU " << Test1()->totalTime() << std::endl;
	//std::cout << "CPU " << Test1_dumb()->totalTime() << std::endl;
	//std::cout << "		2" << std::endl;
	//std::cout << "GPU " << Test2()->totalTime() << std::endl;
	//std::cout << "CPU " << Test2_dumb()->totalTime() << std::endl;
	std::cout << "		3" << std::endl;
	std::cout << "GPU " << Test3()->totalTime() << std::endl;
	std::cout << "CPU " << Test3_dumb()->totalTime() << std::endl;
	//Test4();
	//Test4_dumb();
	//Test5();
	//Test6();

	//Test0();
	//Test1();
	//Test2();
	//Test3();
	

	std::cin.get();
	return 0;
}
