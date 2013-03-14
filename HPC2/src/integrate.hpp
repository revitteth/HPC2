#ifndef INTEGRATE_HPP_
#define INTEGRATE_HPP_

#define __CL_ENABLE_EXCEPTIONS
#define __NO_STD_VECTOR // Use cl::vector instead of STL version

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>

#include <CL/cl.hpp>
#include <tbb/tbb.h>

#include "functions.hpp"
#include "Timer.hpp"

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
	int k=-1, total=-1, i0, i1, i2, j, p_size;
	const int p_size_max = 10;

	int n0=n, n1=n, n2=n;	// By default use n points in each dimension

	switch(functionCode)
	{
		case 0:	k=1; n2 = 1; n1 = 1; p_size = 0; break;
		case 1:	k=2; n2 = 1; p_size = 2; break;
		case 2:	k=3; p_size = 0; break;
		case 3:	k=3; p_size = 1; break;
		case 4:	k=3; p_size = 10; break;
		case 5:	k=3; p_size = 0; break;
		case 6:	k=3; p_size = 2; break;
		default:
			fprintf(stderr, "Invalid function code.");
			exit(1);
	}

	int total_points = n0*n1*n2;
	int n_array[3] = {n0, n1, n2};

	float* new_params = new float[p_size_max];
	if (p_size > 0)
	{
		for(int i = 0; i < p_size; i++)
		{
			new_params[i] = params[i];
		}
	}

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
	cl::Context context(CL_DEVICE_TYPE_CPU, cps);
 
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

	// Create memory buffers
	cl::Buffer buf_a = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_ALLOC_HOST_PTR, k * sizeof(float));
	cl::Buffer buf_b = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_ALLOC_HOST_PTR, k * sizeof(float));
	cl::Buffer buf_out = cl::Buffer(context, CL_MEM_WRITE_ONLY | CL_MEM_ALLOC_HOST_PTR, total_points * sizeof(float));
	cl::Buffer buf_n = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_ALLOC_HOST_PTR, 3 * sizeof(int));
	cl::Buffer buf_params = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_ALLOC_HOST_PTR, p_size_max * sizeof(float));
		
	// Copy data to memory buffers
	queue.enqueueWriteBuffer(buf_a, CL_TRUE, 0, k * sizeof(float), &a[0]);
	queue.enqueueWriteBuffer(buf_b, CL_TRUE, 0, k * sizeof(float), &b[0]);
	queue.enqueueWriteBuffer(buf_n, CL_TRUE, 0, 3 * sizeof(int), &n_array[0]);
	queue.enqueueWriteBuffer(buf_params, CL_TRUE, 0, p_size_max * sizeof(float), &new_params[0]);
 
	// Set arguments to kernel
	kernel.setArg(0, buf_a);
	kernel.setArg(1, buf_b);
	kernel.setArg(2, buf_out);
	kernel.setArg(3, buf_n);
	kernel.setArg(4, buf_params);
	
	cl::NDRange global(n0, n1, n2);
	cl::NDRange local(2, 2, 2);	

	// Run the kernel on specific ND range
	float* out = new float[total_points];
	if (n2 == 1 | n1 == 1)
		queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(total_points), cl::NDRange(1));
	else if (n <= 8)
		queue.enqueueNDRangeKernel(kernel, cl::NullRange, global, local);
	else
		queue.enqueueNDRangeKernel(kernel, cl::NullRange, global, cl::NDRange(8, 8, 8));

	queue.enqueueReadBuffer(buf_out, CL_TRUE, 0, total_points * sizeof(float), out);
	queue.flush();
	queue.finish();
	double acc = 0.0;

	for(int i = 0; i < total_points; i++)
	{
		acc += out[i];	
	}

	for(j=0;j<k;j++){
		acc = acc*(b[j]-a[j]);
	}

	return acc/(total_points);
}

Timer* Test0()
{
	double exact=(exp(1)-1);	// Exact result
	float a[1]={0};
	float b[1]={1};
	int n;
	
	Timer* t0 = new Timer();

	for(n=2;n<=512;n*=2){
		t0->Start(tbb::tick_count::now());
		double res=IntegrateExample(
		  0, // functionCode,
		  n,	// How many points on each dimension
		  a, // An array of k lower bounds
		  b, // An array of k upper bounds
		  NULL // Parameters to function (no parameters for this function)
		);
		t0->Stop(tbb::tick_count::now());
		fprintf(stderr, "F0, n=%d, value=%lf, error=%lg\n", n, res, res-exact);
	}
	return t0;
}

Timer* Test1()
{ 
	double exact=1.95683793560212f;	// Correct to about 10 digits
	float a[2]={0,0};
	float b[2]={1,1};
	float params[2]={0.5,0.5};
	int n;

	Timer* t1 = new Timer();
	
	for(n=2;n<=1024;n*=2){
		t1->Start(tbb::tick_count::now());
		double res=IntegrateExample(
		  1, // functionCode,
		  n,	// How many points on each dimension
		  a, // An array of k lower bounds
		  b, // An array of k upper bounds
		  params // Parameters to function
		);
		t1->Stop(tbb::tick_count::now());
		fprintf(stderr, "F1, n=%d, value=%lf, error=%lg\n", n, res, res-exact);
	}

	return t1;
}

Timer* Test2()
{
	double exact=9.48557252267795;	// Correct to about 6 digits
	float a[3]={-1,-1,-1};
	float b[3]={1,1,1};
	int n;

	Timer* t2 = new Timer();
	
	for(n=2;n<=256;n*=2){
		t2->Start(tbb::tick_count::now());
		double res=IntegrateExample(
		  2, // functionCode,
		  n,	// How many points on each dimension
		  a, // An array of k lower bounds
		  b, // An array of k upper bounds
		  NULL // Parameters to function (no parameters for this function)
		);
		t2->Stop(tbb::tick_count::now());
		fprintf(stderr, "F2, n=%d, value=%lf, error=%lg\n", n, res, res-exact);
	}

	return t2;
}

Timer* Test3()
{
	double exact=-7.18387139942142f;	// Correct to about 6 digits
	float a[3]={0,0,0};
	float b[3]={5,5,5};
	float params[1]={2};
	int n;

	Timer* t3 = new Timer();

	for(n=2;n<=256;n*=2){
		t3->Start(tbb::tick_count::now());
		double res=IntegrateExample(
		  3, // functionCode,
		  n,	// How many points on each dimension
		  a, // An array of k lower bounds
		  b, // An array of k upper bounds
		  params // Parameters to function (no parameters for this function)
		);
		t3->Stop(tbb::tick_count::now());
		fprintf(stderr, "F3, n=%d, value=%lf, error=%lg\n", n, res, res-exact);
	}
	return t3;
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
	//for(n=2;n<=256;n*=2){
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
	
	//for(n=2;n<=512;n*=2){		
	for(n=2;n<=256;n*=2){
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