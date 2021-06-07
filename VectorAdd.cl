__kernel void VectorAdd(__global ulong* c, __global ulong* a, __global ulong* b)
{
	ulong p = 9223372036854775783;
	unsigned int n = get_global_id(0);
	c[n] = (a[n] * b[n]) % p;
}
