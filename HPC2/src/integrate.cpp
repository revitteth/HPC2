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

	Test0();	

	std::cin.get();
	return 0;
}
