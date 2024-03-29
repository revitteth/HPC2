__kernel void integrate_F1(
		__constant float* a, 
		__constant float* b, 
		__global float* out,
		__constant int* n,
		__constant float* params,
		__constant int* pchunks
		)
{
	// get local id of work item
	int index = get_global_id(0);

	int i0 = get_global_id(0);
	int i1 = get_global_id(1);

	__private float x[3] = { 0.0f };

	x[1]=a[1]+(b[1]-a[1]) * native_divide((i1+0.5f), n[1]);
	x[0]=a[0]+(b[0]-a[0]) * native_divide((i0+0.5f), n[0]);

	out[index] = native_sin(params[0] + x[0]*x[1]) * native_exp(params[1]+x[0]);
}