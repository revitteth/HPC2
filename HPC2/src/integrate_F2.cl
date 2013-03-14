__kernel void integrate_F2(
		__constant float* a, 
		__constant float* b, 
		__global float* out,
		__constant int* n,
		__constant float* params,
		__constant int* p_size
		)
{
	// get local id of work item 
	int index = get_global_id(0) + (get_global_id(1)*get_global_size(1)) + (get_global_id(2)*get_global_size(1)*get_global_size(2));

	int i0 = fmod((float)index, (float)n[0]);
	int i1 = fmod((float)floor(native_divide((float)index, (float)n[1])), n[1]);
	int i2 = floor(native_divide((float)index, ((float)n[1] * (float)n[2])));

	float x[3] = { 0.0f };

	x[2]=a[2]+(b[2]-a[2]) * native_divide((i2+0.5f), n[2]);
	x[1]=a[1]+(b[1]-a[1]) * native_divide((i1+0.5f), n[1]);
	x[0]=a[0]+(b[0]-a[0]) * native_divide((i0+0.5f), n[0]);

	out[index] = floor(native_exp(-x[0])) - floor(native_exp(x[1]))*native_sin(x[2]);

}