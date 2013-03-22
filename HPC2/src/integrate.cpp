#define CL_USE_DEPRECATED_OPENCL_1_1_APIS  // My gfx card only supports 1.1
#define __NO_STD_VECTOR // Use cl::vector instead of STL version
#define __CL_ENABLE_EXCEPTIONS

#include <tbb/tbb.h>
#include <CL/cl.hpp>
#include <utility>
#include <iostream>
#include <fstream>
#include <string>

#include "functions.hpp"
#
// OpenCL implementation of function Integrate(...)
#include "integrate.hpp"
// CPU implementation of function IntegrateExample_dumb(...)
#include "dumb_impl.hpp"
// Useful timer class also collects strings to be output
#include "Timer.hpp"

int main(int argc, char **argv)
{ 
	// Call my test functions which evaluate each kernel
	// Console will print value, error, estimated error, time etc.
	Test0()->printStrings();	
	Test1()->printStrings();
	Test2()->printStrings();
	Test3()->printStrings();
	Test4()->printStrings();
	Test5()->printStrings();
	Test6()->printStrings();

	return 0;
}
