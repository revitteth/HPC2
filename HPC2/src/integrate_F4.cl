__kernel void integrate_F4(
		__constant float* a, 
		__constant float* b, 
		__global float* out,
		__constant int* n,
		__constant float* params,
		__global float* acc,
		__constant int* pchunks
		)
{
	int chunks = *pchunks;

	for (int z = 0; z < chunks; z++)
	{
		for(int y = 0; y < chunks; y++)
		{
			for(int x = 0; x < chunks; y++)
			{
				float acc1 = 0;
				float x[3] = { 0.0f };
				int i2 = get_global_id(2) * chunks + z;
				int i1 = get_global_id(1) * chunks + y;
				int i0 = get_global_id(0) * chunks + x;

				x[2] = a[2]+(b[2]-a[2]) * native_divide((i2+0.5f), n[2]);
				x[1] = a[1]+(b[1]-a[1]) * native_divide((i1+0.5f), n[1]);
				x[0] = a[0]+(b[0]-a[0]) * native_divide((i0+0.5f), n[0]);

				acc1+= (params[0]*x[0] + params[1]*x[1] + params[2]*x[2]) * x[0];
				acc1+= (params[3]*x[0] + params[4]*x[1] + params[5]*x[2]) * x[1];
				acc1+= (params[6]*x[0] + params[7]*x[1] + params[8]*x[2]) * x[2];
				float brian = native_exp(native_divide(-acc1, 2));
				float dave = params[9] * brian;

				out[(get_global_id(1)*get_global_id(2)*get_global_size(1)*chunks+z) + (get_global_id(1)*get_global_size(1)*chunks+y) + (get_global_id(0)*chunks+x)] = dave;

			}
		}
	}

}	