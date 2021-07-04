__kernel void VectorAdd(__global ulong* c, __global ulong* a, __global ulong* b)
{
	ulong p = 2147483647;
	unsigned int n = get_global_id(0);
	c[n] = (a[n] - b[n] + p) % p;
}
