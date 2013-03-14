__kernel void integrate_F4(
		__constant float* a, 
		__constant float* b, 
		__global float* out,
		__constant int* n,
		__constant float* params
		)
{
	short ggs1 = get_global_size(1);
	short index = get_global_id(0) + (get_global_id(1)*ggs1) + (get_global_id(2)*ggs1*get_global_size(2));
	//short index = get_global_id(0) + (get_global_id(1)*ggs1) + (get_global_id(2)*get_global_size(1)*get_global_size(2));

	int i0 = fmod((float)index, (float)n[0]);
	int i1 = fmod((float)floor(native_divide((float)index, (float)n[1])), n[1]);
	int i2 = floor(native_divide((float)index, ((float)n[1] * (float)n[2])));

	float x[3] = { 0.0f };

	x[2]=a[2]+(b[2]-a[2]) * native_divide((i2+0.5f), n[2]);
	x[1]=a[1]+(b[1]-a[1]) * native_divide((i1+0.5f), n[1]);
	x[0]=a[0]+(b[0]-a[0]) * native_divide((i0+0.5f), n[0]);

	float acc = 0;

	acc+= (params[0]*x[0] + params[1]*x[1] + params[2]*x[2]) * x[0];
	acc+= (params[3]*x[0] + params[4]*x[1] + params[5]*x[2]) * x[1];
	acc+= (params[6]*x[0] + params[7]*x[1] + params[8]*x[2]) * x[2];

	float brian = native_exp(native_divide(-acc, 2));
	float dave = params[9] * brian;

	out[index] = dave;

}	