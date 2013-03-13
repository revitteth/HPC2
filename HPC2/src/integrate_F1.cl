__kernel void integrate_F1(
		__constant float* a, 
		__constant float* b, 
		__global float* out,
		__constant int* n,
		__constant float* params,
		__constant int* p_size
		)
{
	// get local id of work item 
	int index = get_global_id(0);

	int i0 = fmod((float)index, (float)n[0]);
	int i1 = floor((float)index / (float)n[1]);
	int i2 = floor((float)index / (float)n[2]);

	float x[3] = { 0.0f };

	x[2]=a[2]+(b[2]-a[2]) * (i2+0.5f)/n[2];

	x[1]=a[1]+(b[1]-a[1]) * (i1+0.5f)/n[1];

	x[0]=a[0]+(b[0]-a[0]) * (i0+0.5f)/n[0];

	//out[i0] = exp(x[0]);

	out[i0] = sin(params[0] + x[0]*x[1]) * exp(params[1]+x[0]);

}