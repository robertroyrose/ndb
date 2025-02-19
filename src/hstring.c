uint32_t large_prime(const uint8_t *s, uint32_t hash)
{
    // Better alternative to x33: multiply by a large co-prime instead of a small one
    while (*s != '\0')
        hash = 17000069*hash + *s++;
    return hash;
}
