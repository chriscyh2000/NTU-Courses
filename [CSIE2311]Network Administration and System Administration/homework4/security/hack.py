from utils import DHBase

class Eve(DHBase):
    def __init__(self):
        self.p = 11388168327786972724700146437190443163708827575264440209064585024166842431170021153743806986759032927536175038878351515812006275962970367012655779439437133
        self.g = 2
        self.g_a = 8319860550917878207889021253554859053610353642755340499028865221011267710909792792608850011472277545377760550323403133459407076363557129064994492854944873
        self.g_b = 957980644638641185040454363991430051273061826902639475225383371061710902532335172747285111678187124590717341587849737760723153145916300712124793311197498
        self.encrypted_flag = b'=\x88t\x8f\xef:Sm18\xa5@G\xc6\xe0\x93]H\xa1W\x08\n\x96XkM:+\x99\xcb\xcel~M\x0c\x92U\xf5\xd7>/\xe0\xaeY1\x00\xcfQ\xda\xe0_\xf8\xb5\x02\xd0\xac\xd0\xcc:\x1bS8\xed\x95'
        self.g_ab = None

    # Try to obtain g^ab here?
    def hack(self):
        # HW4{THIS_IS_A_FAKE_FLAGGGGGGGGG}
        for i in range(0, 65536):
            if pow(self.g, int(i), self.p) == self.g_a or ((self.p - 1) * pow(self.g, int(i), self.p) % self.p) == self.g_a:
                self.a = i + self.p // 2
                self.genSharedKey(self.g_b)
                break


eve = Eve()
eve.hack()
print(eve.decrypt(eve.encrypted_flag))