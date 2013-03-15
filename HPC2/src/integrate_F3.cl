__kernel void integrate_F3(
		__constant float* a, 
		__constant float* b, 
		__global float* out,
		__constant int* n,
		__constant float* params,
		__constant int* pchunks
		)
{
	int chunks = *pchunks;
	int gid0 = get_global_id(0);
	int gid1 = get_global_id(1);
	int gid2 = get_global_id(2);
	int gs1 = get_global_size(1);

	__private float group_acc = 0;
	__private float x_ptr[3] = { 0.0f };

	for (int z = 0; z < chunks; z++)
	{
		for(int y = 0; y < chunks; y++)
		{
			for(int x = 0; x < chunks; x++)
			{
				int i2 = gid2 * chunks + z;
				int i1 = gid1 * chunks + y;
				int i0 = gid0 * chunks + x;

				x_ptr[2] = a[2]+(b[2]-a[2]) * native_divide((i2+0.5f), n[2]);
				x_ptr[1] = a[1]+(b[1]-a[1]) * native_divide((i1+0.5f), n[1]);
				x_ptr[0] = a[0]+(b[0]-a[0]) * native_divide((i0+0.5f), n[0]);

				float v = native_powr(x_ptr[0], params[0]) + native_powr(x_ptr[1], params[0]) + native_powr(x_ptr[2],params[0]);
				float dave = native_powr(v, native_recip(params[0]));
				float geoff = native_divide(sin(dave), dave);
				group_acc += geoff;
			}
		}
	}

	out[gid2*gs1*gs1 + gid1*gs1 + gid0] = group_acc;

}