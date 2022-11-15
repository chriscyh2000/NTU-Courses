from ldap3 import Server, Connection, SAFE_SYNC, Tls, ALL, MODIFY_REPLACE
import getpass
tls = Tls(ciphers="ALL") 
server = Server('ldaps://nasa.csie.ntu',  tls=tls, get_info='ALL')
my_password = str(input("Password: "))
conn = Connection(server, user="uid="+getpass.getuser()+",ou=people,dc=nasa,dc=csie,dc=ntu", password=my_password, client_strategy=SAFE_SYNC, auto_bind=True)
conn.bind()
conn.modify("uid="+getpass.getuser()+",ou=people,dc=nasa,dc=csie,dc=ntu",
            {'givenName': [(MODIFY_REPLACE, [str(input('New givenName: '))])]})
print(conn.result)
conn.unbind()