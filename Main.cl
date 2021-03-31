uint Hash(uint n)
{
	n ^= 2747636419;
	n *= 2654435769;

	n ^= n >> 16;
	n *= 2654435769;

	n ^= n >> 16;
	n *= 2654435769;
	return n;
}

kernel void Draw(global int *pixels)
{
	size_t id = get_global_id(0);
	uint x = id % 512, y = id / 512;
	uchar r, g, b;
	r = g = b = Hash(x) ^ Hash(y);
	
	pixels[id] = (r << 16) | (g << 8) | (b << 0);
}