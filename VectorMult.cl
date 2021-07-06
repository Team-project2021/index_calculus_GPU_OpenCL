const ulong p = 11;

ulong mult32_mod_p(ulong a)
{
    a = a % p;
    ulong a1 = a >> 32;
    ulong b1 = 0xffffffff & a;
    ulong residue = ((b1 << 32) % p) + a1 * 50;
    return residue % p;
}

ulong mult64_mod_p(ulong a)
{
    a = a % p;
    ulong a1 = a >> 32;
    ulong b1 = 0xffffffff & a;
    ulong residue = mult32_mod_p(a1 * 50) + b1 * 50;
    return residue % p;
}


ulong mult(ulong a, ulong b)
{

    ulong a1 = a;
    a1 = a1 >> 32;
    ulong b1 = 0xffffffff & a;
    ulong a2 = b;
    a2 = a2 >> 32;
    ulong b2 = 0xffffffff & b;
    ulong A = mult64_mod_p(a1 * a2);
    ulong B = mult32_mod_p(a1 * b2);
    ulong C = mult32_mod_p(a2 * b1);
    ulong D = (b1 * b2) % p;

    return ((((A + B) % p + C) % p + D) % p);
}

__kernel void VectorMult(__global ulong* c, __global ulong* a, __global ulong* b)
{	
	unsigned long long n = get_global_id(0);
	c[n] = mult(a[n], b[n]);
}