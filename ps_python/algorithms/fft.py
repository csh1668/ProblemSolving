import math


def fft(a, w):
    n = len(a)
    if n == 1:
        return a
    even = []
    odd = []
    for i in range(n):
        if i & 1:
            odd.append(a[i])
        else:
            even.append(a[i])
    fft(even, w * w)
    fft(odd, w * w)
    wp = 1 + 0j
    for i in range(n // 2):
        a[i] = even[i] + wp * odd[i]
        a[i + n // 2] = even[i] - wp * odd[i]
        wp *= w
    return a



def multiply_fft(a, b):
    n = 1
    while n < len(a):
        n <<= 1
    while n < len(b):
        n <<= 1
    n <<= 1
    while len(a) < n:
        a.append(0)
    while len(b) < n:
        b.append(0)

    w = math.cos(2 * math.pi / n) + 1j * (math.sin(2 * math.pi / n))
    fft(a, w)
    fft(b, w)
    c = []
    for i in range(n):
        c.append(a[i] * b[i])
    fft(c, 1 / w)
    for i in range(n):
        c[i] /= n + 0j
        c[i] = int(round(c[i].real))
    return c