import binascii

def hexstr2carray(s):
	s = s.replace(" ","")
	assert(len(s) % 2 == 0)
	numbers = []
	for i in range(0, len(s), 2):
		numbers.append(s[i:i+2])

	return '{%s}' % (', '.join("0x%s" % part for part in numbers), )

def parseBytesAsBigInt(s):
    result = 0L
    for c in s:
        result <<= 8
        result |= ord(c)

    return result

def bigIntToBytes(n):
    result = bytearray()
    while n > 0:
        result.append(chr(n & 0xff))
        n >>= 8
    result.reverse()
    return str(result)

def bytestring2carray(s):
    return hexstr2carray(binascii.b2a_hex(s))

def generateSpeckTestCases(ofile, repeat = 10):
    import speck
    import os
    sizes = (
            (32, 64),
            (48, 72),
            (48, 96),
            (64, 96),
            (64, 128),
            (96, 96),
            (96, 144),
            (128, 128),
            (128, 192),
            )

    for size in sizes:
        for i in range(0, repeat):
            key = os.urandom(size[1] / 8)
            plaintext = os.urandom(size[0] / 8)
            s = speck.SPECK(size[0], size[1])
            s.change_key(parseBytesAsBigInt(key))
            ciphertext = bigIntToBytes(s.encrypt(parseBytesAsBigInt(plaintext)))
            ofile.write("\t{%d, %d, %s, %s, %s}\n" % (size[0], size[1], bytestring2carray(key), bytestring2carray(plaintext), bytestring2carray(ciphertext)))
