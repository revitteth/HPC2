__kernel void integrate_F0(
		__constant float* a, 
		__constant float* b, 
		__global float* out
		)
{
	// get local id of work item 
	int i0 = get_global_id(0);

	out[i0] = 1;

}