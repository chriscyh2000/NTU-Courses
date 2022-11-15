from secret import flag
import os
import string

def bytes_to_int(x: bytes):
    return int.from_bytes(x, 'little')

def encrypt(plaintext):
    assert all(c not in string.ascii_uppercase for c in plaintext), f'The plaintext contains uppercase letter'
    assert all(c in string.printable for c in plaintext), f'The plaintext contains non-printable character'
    assert len(plaintext) % 8 == 0 and len(plaintext) > 0, f'Length of plaintext: {len(plaintext)} % 8 != 0'

    # Generate a random key
    key = bytes_to_int(os.urandom(1)) % 26

    # Map A-Z to  0-25 for convenience
    plaintext = [ ord(c) - ord('a') for c in plaintext ]
    ciphertext = []

    # Encrypt block by block with block size = 8
    for i in range(0, len(plaintext), 8):
        l, r = (i, i + 8)
        ciphertext.extend([ (c + key) % 26 if 0 <= c < 26 else c for c in plaintext[l:r] ])
        # Update the key
        key = (key + ciphertext[r - 1]) % 26

    ciphertext = ''.join( chr(c + ord('a')) for c in ciphertext )

    return ciphertext

if __name__ == '__main__':
    print(f'HW4{{{encrypt(flag[4:-1])}}}')