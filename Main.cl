uchar Rand(ulong seed)
{
	return seed >> 13213721 ^ seed << 9128321;
}

kernel void Draw(global int *pixels)
{
	size_t id = get_global_id(0);
	uchar r, g, b;
	r = Rand(id);
	g = Rand(id);
	b = Rand(id);
	
	pixels[id] = (r << 16) | (g << 8) | (b << 0);
}