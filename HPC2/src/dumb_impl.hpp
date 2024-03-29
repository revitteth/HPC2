#include "functions.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.hpp>
#include <tbb/tbb.h>
#include "Timer.hpp"

/* This is a simple example of multi-dimensional integration
	using a simple (not necessarily optimal) spacing of points.
	Note that this doesn't perform any error estimation - it
	only calculates the value for a given grid size.
*/
double IntegrateExample_dumb(
  int functionCode,
  int n,	// How many points on each dimension
  const float *a, // An array of k lower bounds
  const float *b, // An array of k upper bounds
  const float *params // Parameters to function
){
	int k=-1, total=-1, i0, i1, i2, j;
	// Accumulate in double, as it avoids floating-point errors when adding large
	// numbers of values together. Note that using double in a GPU has implications,
	// as some GPUs cannot do doubles, and on others they are much slower than floats
	double acc=0;	
	//float *x=NULL;
	int n0=n, n1=n, n2=n;	// By default use n points in each dimension
	
	switch(functionCode){
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
	
	// Collapse any dimensions we don't use
	if(k<3){
		n2=1;
	}
	if(k<2){
		n1=1;
	}
	
	float* x = new float[k];

	// Loop over highest dimension on outside, as it might be collapsed to zero
	for(i2=0;i2<n2;i2++){
		if(k>2){
			x[2]=a[2]+(b[2]-a[2]) * (i2+0.5f)/n2;
		}
		
		for(i1=0;i1<n1;i1++){
			if(k>1){
				x[1]=a[1]+(b[1]-a[1]) * (i1+0.5f)/n1;
			}
			
			// Inner dimension is never collapsed to zero
			for(i0=0;i0<n0;i0++){
				x[0]=a[0]+(b[0]-a[0]) * (i0+0.5f)/n0;
				
				// Now call the function. Note that it is rather
				// inefficient to be choosing the function in the inner loop... 
				// MAKE A FUNCTION POINTER FURTHER UP THEN USE IT!!!!
				switch(functionCode){
				case 0:	acc+=F0(x,params);	break;
				case 1:	acc+=F1(x,params);  break;
				case 2:	acc+=F2(x,params);  break;
				case 3:	acc+=F3(x,params);	break;
				case 4:	acc+=F4(x,params);	break;
				case 5:	acc+=F5(x,params);	break;
				case 6:	acc+=F6(x,params);	break;				   
				}
			}
		}
	}
	
	// Do the final normalisation and return the results
	for(j=0;j<k;j++){
		acc=acc*(b[j]-a[j]);
	}
	return acc/(n0*n1*n2);
}

Timer* Test0_dumb()
{
	double exact=(exp(1)-1);	// Exact result
	float a[1]={0};
	float b[1]={1};
	int n;

	Timer* t0 = new Timer();
	
	for(n=2;n<=512;n*=2)
	{
		t0->Start(tbb::tick_count::now());
		double res=IntegrateExample_dumb(
		  0, // functionCode,
		  n,	// How many points on each dimension
		  a, // An array of k lower bounds
		  b, // An array of k upper bounds
		  NULL // Parameters to function (no parameters for this function)
		);
		t0->Stop(tbb::tick_count::now());
		//std::cout << t0->lastTime() << " seconds" << std::endl;
		fprintf(stderr, "F0, n=%d, value=%lf, error=%lg\n", n, res, res-exact);
	}

	return t0;
}

Timer* Test1_dumb()
{
	double exact=1.95683793560212f;	// Correct to about 10 digits
	float a[2]={0,0};
	float b[2]={1,1};
	float params[2]={0.5,0.5};
	int n;

	Timer* t1 = new Timer();
	
	for(n=2;n<=1024;n*=2)
	{
		t1->Start(tbb::tick_count::now());
		double res=IntegrateExample_dumb(
		  1, // functionCode,
		  n,	// How many points on each dimension
		  a, // An array of k lower bounds
		  b, // An array of k upper bounds
		  params // Parameters to function
		);
		fprintf(stderr, "F1, n=%d, value=%lf, error=%lg\n", n, res, res-exact);
		t1->Stop(tbb::tick_count::now());
		//std::cout << t1->lastTime() << " seconds" << std::endl;
	}

	return t1;
}

Timer* Test2_dumb()
{
	double exact=9.48557252267795;	// Correct to about 6 digits
	float a[3]={-1,-1,-1};
	float b[3]={1,1,1};
	int n;

	Timer* t2 = new Timer();
	
	for(n=2;n<=256;n*=2){
		t2->Start(tbb::tick_count::now());
		double res=IntegrateExample_dumb(
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

Timer* Test3_dumb()
{
	double exact=-7.18387139942142f;	// Correct to about 6 digits
	float a[3]={0,0,0};
	float b[3]={5,5,5};
	float params[1]={2};
	int n;

	Timer* t3 = new Timer();
	
	for(n=2;n<=256;n*=2){		
		t3->Start(tbb::tick_count::now());
		double res=IntegrateExample_dumb(
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

Timer* Test4_dumb()
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

	Timer* t4 = new Timer();
	
	for(n=2;n<=512;n*=2){		
		t4->Start(tbb::tick_count::now());
		double res=IntegrateExample_dumb(
		  4, // functionCode,
		  n,	// How many points on each dimension
		  a, // An array of k lower bounds
		  b, // An array of k upper bounds
		  params // Parameters to function (no parameters for this function)
		);
		t4->Stop(tbb::tick_count::now());
		fprintf(stderr, "F4, n=%d, value=%lf, error=%lg	\n", n, res, res-exact);
	}
	return t4;
}

Timer* Test5_dumb()
{
	double exact=13.4249394627056;	// Correct to about 6 digits
	float a[3]={0,0,0};
	float b[3]={3,3,3};
	int n;

	Timer* t5 = new Timer();
	
	for(n=2;n<=512;n*=2){
		t5->Start(tbb::tick_count::now());
		double res=IntegrateExample_dumb(
		  5, // functionCode,
		  n,	// How many points on each dimension
		  a, // An array of k lower bounds
		  b, // An array of k upper bounds
		  NULL
		);
		t5->Stop(tbb::tick_count::now());
		fprintf(stderr, "F5, n=%d, value=%lf, error=%lg	\n", n, res, res-exact);
	}

	return t5;
}

Timer* Test6_dumb()
{
	// Integrate over a shell with radius 3 and width 0.02
	//  = volume of a sphere of 3.01 minus a sphere of 2.99
	double exact=   2.261955088165;
	float a[3]={-4,-4,-4};
	float b[3]={4,4,4};
	float params[2]={3,0.01};
	int n;
	
	Timer* t6 = new Timer();

	for(n=2;n<=256;n*=2){	
		t6->Start(tbb::tick_count::now());
		double res=IntegrateExample_dumb(
		  6, // functionCode,
		  n,	// How many points on each dimension
		  a, // An array of k lower bounds
		  b, // An array of k upper bounds
		  params
		);
		t6->Stop(tbb::tick_count::now());
		fprintf(stderr, "F6, n=%d, value=%lf, error=%lg	\n", n, res, res-exact);
	}

	return t6;
}
