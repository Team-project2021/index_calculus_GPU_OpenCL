__kernel void VectorMult(__global unsigned long* c, __global unsigned long* a, __global unsigned long* b)
{
	unsigned long long n = get_global_id(0);
	c[n] = a[n] * b[n];
}