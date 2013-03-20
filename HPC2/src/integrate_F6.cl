__kernel void integrate_F6(
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
		int i2 = gid2 * chunks + z;
		x_ptr[2] = a[2]+(b[2]-a[2]) * native_divide((i2+0.5f), n[2]);

		for(int y = 0; y < chunks; y++)
		{
			int i1 = gid1 * chunks + y;
			x_ptr[1] = a[1]+(b[1]-a[1]) * native_divide((i1+0.5f), n[1]);

			for(int x = 0; x < chunks; x++)
			{
				int i0 = gid0 * chunks + x;
				x_ptr[0] = a[0]+(b[0]-a[0]) * native_divide((i0+0.5f), n[0]);

				float a2 = pow(pow(x_ptr[0],2) + pow(x_ptr[1],2) + pow(x_ptr[2],2),0.5); // distance from origin
				float d = (a2 - params[0]);	// How far from surface of sphere

				d = fabs(d);

				if (d < params[1])
				{
					group_acc += 1;
				}
			}
		}
	}

	out[gid2*gs1*gs1 + gid1*gs1 + gid0] = group_acc;
}	