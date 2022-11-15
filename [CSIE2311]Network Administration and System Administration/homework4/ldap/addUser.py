import ldap
from passlib.hash import ldap_md5_crypt

FIRST_UID = 100
LAST_UID = 8000

LDAP_HOST = "ldapi:///"
LDAP_BASE_DN = "ou=people,dc=nasa,dc=csie,dc=ntu"
LDAP_ADMIN_DN = "cn=nasa,dc=nasa,dc=csie,dc=ntu"

def generate_uid():
    check = [0 for i in range(0, LAST_UID)]
    l = ldap.initialize("ldapi:///")
    l.simple_bind_s("cn=nasa,dc=nasa,dc=csie,dc=ntu", "nasa2022")
    res = l.search_s("dc=nasa,dc=csie,dc=ntu", ldap.SCOPE_SUBTREE, 'objectClass=posixaccount', ['uidNumber'])
    for a in res:
        if(int(a[1].get('uidNumber')[0]) >= LAST_UID):
            continue
        check[int(a[1].get('uidNumber')[0])] = 1
    for uid in range(FIRST_UID, LAST_UID):
        if check[uid] == 0:
            return uid

def input_data():
    user = {}
    user['username'] = str(input("Username: "))
    user['uidNumber'] = generate_uid()
    user['gidNumber'] = 2500
    user['homeDirectory'] = "/home" + str(user['username'])
    passwd = str(input("Password: "))
    user['userPassword'] = ldap_md5_crypt.encrypt(passwd)
    user['loginShell'] = '/bin/bash'
    return user

def create_user(user):
    dn = 'uid=' + user['username'] + ',' + LDAP_BASE_DN
    entry = []
    entry.extend([
            ('objectClass', ["top".encode("utf-8"), "inetOrgPerson".encode("utf-8"), "posixAccount".encode("utf-8"), "shadowAccount".encode("utf-8")]),
            ('cn', user['username'].encode("utf-8")),
            ('uid', user['username'].encode("utf-8")),
            ('uidNumber', str(user['uidNumber']).encode("utf-8")),
            ('sn',  user['username'].encode("utf-8")),
            ('givenName', "default".encode("utf-8")),
            ('gidNumber', str(user['gidNumber']).encode("utf-8")),
            ('homeDirectory', user['homeDirectory'].encode("utf-8")),
            ('loginShell', user['loginShell'].encode("utf-8")),
            ('userPassword', user['userPassword'].encode("utf-8"))
    ])
    connection = ldap.initialize(LDAP_HOST)
    connection.simple_bind_s("cn=nasa,dc=nasa,dc=csie,dc=ntu", "nasa2022")
    connection.add_s(dn, entry)

if __name__ == '__main__':
    user_info = input_data()
    create_user(user_info)