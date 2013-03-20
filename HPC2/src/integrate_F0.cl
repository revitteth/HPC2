__kernel void integrate_F0(
		__constant float* a, 
		__constant float* b, 
		__global float* out,
		__constant int* n,
		__constant float* params,
		__constant int* pchunks
		)
{
	// get local id of work item 
	int i0 = get_global_id(0);

	__private float x[3] = { 0.0f };

	x[0]= mad((b[0]-a[0]), native_divide((i0+0.5f),n[0]), a[0]);

	out[i0] = native_exp(x[0]);

}