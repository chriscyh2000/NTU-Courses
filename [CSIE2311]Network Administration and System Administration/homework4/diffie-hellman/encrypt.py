from Crypto.Util.number import getPrime, long_to_bytes, bytes_to_long
from random import randrange

from utils import DHBase

# Alice!!!
class Alice(DHBase):
    def __init__(self, g, p):
        super().__init__(g, p)
        self.flag = b'HW4{THIS_IS_A_FAKE_FLAGGGGGGGGG}'     # Flag!!!

    def getPublicKey(self):
        self.a = self.p // 2 + randrange(0, 65536)
        self.g_a = pow(self.g, self.a, self.p)
        return self.g_a

# Bob!!!
class Bob(DHBase):
    def getPublicKey(self):
        self.a = randrange(2, p - 1)
        print(self.a)
        self.g_a = pow(self.g, self.a, self.p)
        return self.g_a

if __name__ == '__main__':
    # Generate prime with 1024 bits and generator
    p = getPrime(512)
    g = 2
    
    print(f'p: {p}')
    print(f'g: {g}')

    # Alice and Bob come
    alice = Alice(g, p)
    bob   = Bob(g, p)

    # Exchange public keys
    g_a = alice.getPublicKey()
    g_b = bob.getPublicKey()
    alice.genSharedKey(g_b)
    bob.genSharedKey(g_a)
    assert alice.g_ab == bob.g_ab

    print(f'g^a: {g_a}')
    print(f'g^b: {g_b}')

    # Alice send the flag to Bob
    ciphertext = alice.encrypt(alice.flag)
    assert bob.decrypt(ciphertext) == alice.flag

    print(f'encrypted flag: {ciphertext}')
    # print(bob.decrypt(ciphertext))