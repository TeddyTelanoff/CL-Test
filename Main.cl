kernel void Sqrt(global float *values, size_t id)
{
	values[id] = sqrt(values[id]);
}

kernel void ISqrt(global float *values)
{
	// size_t id = get_global_id(get_work_dim() - 1);
	size_t id = get_global_id(0);
	values[id] = 1 / sqrt(values[id]);
}

kernel void FastISqrt(global float *values)
{
	size_t id = get_global_id(0);
	float f = values[id];
	int i = *(int *)&f;
	i = 0x5f3759df - (i >> 1);
	f = *(float *)&i;
	// f = f * (1.5f - f * f * f * 0.5f);
	values[id] = f;
}