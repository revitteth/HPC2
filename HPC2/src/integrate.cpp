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

	/*std::cout << "		0" << std::endl;
	std::cout << "GPU " << Test0()->totalTime() << std::endl;
	std::cout << "CPU " << Test0_dumb()->totalTime() << std::endl;
	std::cout << "		1" << std::endl;
	std::cout << "GPU " << Test1()->totalTime() << std::endl;
	std::cout << "CPU " << Test1_dumb()->totalTime() << std::endl;
	std::cout << "		2" << std::endl;
	std::cout << "GPU " << Test2()->totalTime() << std::endl;
	std::cout << "CPU " << Test2_dumb()->totalTime() << std::endl;
	std::cout << "		3" << std::endl;
	std::cout << "GPU " << Test3()->totalTime() << std::endl;
	std::cout << "CPU " << Test3_dumb()->totalTime() << std::endl;*/
	//Timer* t3 = Test3();
	////Timer* t3_dumb = Test3_dumb();
	//for(int i = 0; i<t3->count();i++)
	//{
	//	std::cout << i << ": CPU(3) " << t3_dumb->timeAt(i) << "		GPU(3) " << t3->timeAt(i) << std::endl;
	//}
	//std::cout << "-----------------------------------------------------------" << std::endl;
	//std::cout << "T: CPU(3) " << t3_dumb->totalTime() << "		GPU(3) " << t3->totalTime() << std::endl;
	//std::cout << "-----------------------------------------------------------" << std::endl;
	//std::cout << "-----------------------------------------------------------" << std::endl << std::endl;
	Test4();
	//Test4_dumb();
	//Test5();
	//Test5_dumb();
	//Test6();
	//Test6_dumb();
	

	std::cin.get();
	return 0;
}
