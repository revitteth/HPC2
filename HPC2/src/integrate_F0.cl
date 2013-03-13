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
	int i1 = floor((float)index/(float)n[1]);
	int i2 = floor((float)index/(float)n[2]);

	float x[3] = { 0.0f };

	x[1]=a[1]+(b[1]-a[1]) * (i1+0.5f)/n[1];

	x[0]=a[0]+(b[0]-a[0]) * (i0+0.5f)/n[0];

	out[i0] = exp(x[0]);

}