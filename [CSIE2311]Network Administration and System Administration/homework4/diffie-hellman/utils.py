from Crypto.Util.number import getPrime, long_to_bytes, bytes_to_long

# Object For Diffie-Hellman
class DHBase:
    def __init__(self, g, p):
        self.g = g          # Generator
        self.p = p          # Prime for mod
        self.a = None       # a:    private key (the secret only you know)
        self.g_a = None     # g^a:  public key that send to others 
        self.g_ab = None    # g^ab: the shared key between two people

    # Generate a, g^a and return g^a
    def getPublicKey(self):
        raise NotImplementedError

    # Calculate the shared key from g^b of the other person you want 
    # to communicate with
    def genSharedKey(self, g_b):
        self.g_ab = pow(g_b, self.a, self.p)

    # Return encrypted message
    def encrypt(self, m: bytes):
        return long_to_bytes(bytes_to_long(m) ^ self.g_ab)

    # Return decrypted message
    def decrypt(self, c: bytes):
        return long_to_bytes(bytes_to_long(c) ^ self.g_ab)