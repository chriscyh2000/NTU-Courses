import os
import string

def decrypt_attempt(key, en_flag):
    de_flag = ""
    flag_len = len(en_flag)
    key_increment = 0
    for i in range(0, flag_len, 8):
        l, r = i, i + 8
        key = (key + key_increment) % 26
        for j in range(l, r):
            c = ord(en_flag[j]) - ord('a')
            if 0 <= c < 26:
                de_flag = de_flag + chr(((ord(en_flag[j]) - key) % 26 + ord('a'))) 
            else:
                de_flag = de_flag + en_flag[j]
        key_increment = ord(en_flag[r-1]) - ord('a')
    print("HW4{" + de_flag + "}")

if __name__ == '__main__':
    en_flag = "g_aykc_g_wea_m_gyxaeobon_sbkf_sffk_xkek!"
    for key in range(0, 26):
        decrypt_attempt(key, en_flag=en_flag)