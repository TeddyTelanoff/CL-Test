kernel void Square(global float *values)
{
	// size_t id = get_global_id(get_work_dim() - 1);
	size_t id = get_global_id(0);
	values[id] *= values[id];
}