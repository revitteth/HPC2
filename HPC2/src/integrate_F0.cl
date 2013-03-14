__kernel void integrate_F0(
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

	float x[3] = { 0.0f };

	x[0]= a[0]+(b[0]-a[0]) * native_divide((i0+0.5f),n[0]);

	out[i0] = native_exp(x[0]);

}