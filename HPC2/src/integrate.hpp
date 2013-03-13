#ifndef INTEGRATE_HPP_
#define INTEGRATE_HPP_

#define __CL_ENABLE_EXCEPTIONS
#define __NO_STD_VECTOR // Use cl::vector instead of STL version


#include "functions.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>

#include <CL/cl.hpp>
#include <tbb/tbb.h>

template <typename T>
	std::string NumberToString ( T Number )
	{
		std::ostringstream ss;
		ss << Number;
		return ss.str();
	}



double IntegrateExample(
  int functionCode,
  int n,	// How many points on each dimension
  const float *a, // An array of k lower bounds
  const float *b, // An array of k upper bounds
  const float *params // Parameters to function
){
	int k=-1, total=-1, i0, i1, i2, j;

	switch(functionCode)
	{
		case 0:	k=1;	break;
		case 1:	k=2;	break;
		case 2:	k=3;	break;
		case 3:	k=3;	break;
		case 4:	k=3;	break;
		case 5:	k=3;	break;
		case 6:	k=3;	break;
		default:
			fprintf(stderr, "Invalid function code.");
			exit(1);
	}

	float acc=0;
	float* x = new float[k];

	int n0=n, n1=n, n2=n;	// By default use n points in each dimension

	std::string func = "integrate_F" + NumberToString(functionCode);

	// Get available platforms
	cl::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
 
	// Select the default platform and create a context using this platform and the GPU
	cl_context_properties cps[3] = {
		CL_CONTEXT_PLATFORM,
		(cl_context_properties)(platforms[0])(),
		0
	};
	cl::Context context(CL_DEVICE_TYPE_GPU, cps);
 
	// Get a list of devices on this platform
	cl::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
 
	// Create a command queue and use the first device
	cl::CommandQueue queue = cl::CommandQueue(context, devices[0]);
 
	// Read source file
	std::ifstream sourceFile("..\\HPC2\\src\\" + func + ".cl");
	std::string sourceCode(
		std::istreambuf_iterator<char>(sourceFile),
		(std::istreambuf_iterator<char>()));
	cl::Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length()+1));
 
	// Make program of the source code in the context
	cl::Program program = cl::Program(context, source);
 
	// Build program for these specific devices
	program.build(devices);
 
	// Make kernel
	cl::Kernel kernel(program, func.c_str());

	// Create memory buffers - all sizes just copied from above :)
	cl::Buffer bufferA = cl::Buffer(context, CL_MEM_READ_ONLY, n * sizeof(float));
	cl::Buffer bufferB = cl::Buffer(context, CL_MEM_READ_ONLY, n * sizeof(float));
	cl::Buffer bufferX = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_HOST_PTR, k * sizeof(float));
		
	// Copy lists A and B to the memory buffers
	queue.enqueueWriteBuffer(bufferA, CL_TRUE, 0, n * sizeof(float), a);
	queue.enqueueWriteBuffer(bufferB, CL_TRUE, 0, n * sizeof(float), b);
	queue.enqueueWriteBuffer(bufferX, CL_TRUE, 0, k * sizeof(float), x);
 
	// Set arguments to kernel
	kernel.setArg(0, bufferA);
	kernel.setArg(1, bufferB);
	kernel.setArg(3, bufferX);

	// Run the kernel on specific ND range
	queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(n1), cl::NDRange(1));

	// pass another buffer (write only array)
	//queue.enqueueReadBuffer(, CL_TRUE, 0, sizeof(float), );

	
	return acc;
}

void Test0()
{
	double exact=(exp(1)-1);	// Exact result
	float a[1]={0};
	float b[1]={1};
	int n;
	
	for(n=2;n<=512;n*=2){		
		double res=IntegrateExample(
		  0, // functionCode,
		  n,	// How many points on each dimension
		  a, // An array of k lower bounds
		  b, // An array of k upper bounds
		  NULL // Parameters to function (no parameters for this function)
		);
		fprintf(stderr, "F0, n=%d, value=%lf, error=%lg\n", n, res, res-exact);
	}
}

void Test1()
{
	double exact=1.95683793560212f;	// Correct to about 10 digits
	float a[2]={0,0};
	float b[2]={1,1};
	float params[2]={0.5,0.5};
	int n;
	
	for(n=2;n<=1024;n*=2){		
		double res=IntegrateExample(
		  1, // functionCode,
		  n,	// How many points on each dimension
		  a, // An array of k lower bounds
		  b, // An array of k upper bounds
		  params // Parameters to function
		);
		fprintf(stderr, "F1, n=%d, value=%lf, error=%lg\n", n, res, res-exact);
	}
}

void Test2()
{
	double exact=9.48557252267795;	// Correct to about 6 digits
	float a[3]={-1,-1,-1};
	float b[3]={1,1,1};
	int n;
	
	for(n=2;n<=256;n*=2){		
		double res=IntegrateExample(
		  2, // functionCode,
		  n,	// How many points on each dimension
		  a, // An array of k lower bounds
		  b, // An array of k upper bounds
		  NULL // Parameters to function (no parameters for this function)
		);
		fprintf(stderr, "F2, n=%d, value=%lf, error=%lg\n", n, res, res-exact);
	}
}

void Test3()
{
	double exact=-7.18387139942142f;	// Correct to about 6 digits
	float a[3]={0,0,0};
	float b[3]={5,5,5};
	float params[1]={2};
	int n;
	
	for(n=2;n<=256;n*=2){		
		double res=IntegrateExample(
		  3, // functionCode,
		  n,	// How many points on each dimension
		  a, // An array of k lower bounds
		  b, // An array of k upper bounds
		  params // Parameters to function (no parameters for this function)
		);
		fprintf(stderr, "F3, n=%d, value=%lf, error=%lg\n", n, res, res-exact);
	}
}

void Test4()
{
	double exact=0.677779532970409f;	// Correct to about 8 digits
	float a[3]={-16,-16,-16};	// We're going to cheat, and assume -16=-infinity.
	float b[3]={1,1,1};
	// We're going to use the covariance matrix with ones on the diagonal, and
	// 0.5 off the diagonal.
	const float PI=3.1415926535897932384626433832795f;
	float params[10]={
		1.5, -0.5, -0.5,
		-0.5, 1.5, -0.5,
		-0.5, -0.5, 1.5,
		(float)pow(2*PI,-3.0/2.0)*pow(0.5,-0.5) // This is the scale factor
	};
	int n;
	
	for(n=2;n<=512;n*=2){		
		double res=IntegrateExample(
		  4, // functionCode,
		  n,	// How many points on each dimension
		  a, // An array of k lower bounds
		  b, // An array of k upper bounds
		  params // Parameters to function (no parameters for this function)
		);
		fprintf(stderr, "F4, n=%d, value=%lf, error=%lg	\n", n, res, res-exact);
	}
}

void Test5()
{
	double exact=13.4249394627056;	// Correct to about 6 digits
	float a[3]={0,0,0};
	float b[3]={3,3,3};
	int n;
	
	for(n=2;n<=512;n*=2){		
		double res=IntegrateExample(
		  5, // functionCode,
		  n,	// How many points on each dimension
		  a, // An array of k lower bounds
		  b, // An array of k upper bounds
		  NULL
		);
		fprintf(stderr, "F5, n=%d, value=%lf, error=%lg	\n", n, res, res-exact);
	}
}

void Test6()
{
	// Integrate over a shell with radius 3 and width 0.02
	//  = volume of a sphere of 3.01 minus a sphere of 2.99
	double exact=   2.261955088165;
	float a[3]={-4,-4,-4};
	float b[3]={4,4,4};
	float params[2]={3,0.01};
	int n;
	
	for(n=2;n<=2048;n*=2){		
		double res=IntegrateExample(
		  6, // functionCode,
		  n,	// How many points on each dimension
		  a, // An array of k lower bounds
		  b, // An array of k upper bounds
		  params
		);
		fprintf(stderr, "F6, n=%d, value=%lf, error=%lg	\n", n, res, res-exact);
	}
}


#endif