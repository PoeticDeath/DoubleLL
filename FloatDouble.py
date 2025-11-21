"""
sign = (f & 0x8000000000000000) >> 63
ex   = (f & 0x7ff0000000000000) >> 52
frac =  f & 0x000fffffffffffff
"""

def lltodouble(ll):
    ull = abs(ll)
    if not ull:
        return False, 0, 0
    mask = 1 << 52
    for i in range(53):
        if ull & mask:
            break
        else:
            mask >>= 1
    oex = 52 - i + 1023
    ex = mask
    mask = 1 << 52
    frac = ull
    frac &= ~ex
    frac <<= 22
    return ll < 0, oex, frac

def doubletoll(double):
    sign, oex, frac = double
    if not oex and not frac:
        return 0
    oex -= 1023
    ex = 1 << oex
    frac >>= 22
    return (1 - 2 * sign) * (ex + frac)
