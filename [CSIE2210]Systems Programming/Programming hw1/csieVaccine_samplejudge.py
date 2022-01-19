import  telnetlib
import subprocess
import time


class TelnetConnection:
	def __init__(self, host, port):
		self.host = host
		self.port = port
	def telnet(self):
		telnet_client = telnetlib.Telnet(self.host, self.port)
		print('Connection established')		
		return telnet_client

class Tasks:
	def __init__(self, dir_, port):
		self.read_port = port
		self.read_port2 = port+3

		self.write_port = port+1
		self.write_port2 = port+2
		self.dir_ = dir_
		self.str1 = "Please enter your id (to check your preference order):\n"
		self.str2_1 = "Your preference order is AZ > BNT > Moderna.\n"
		self.str2_2 = "Your preference order is Moderna > BNT > AZ.\n"
		self.str3 = "Please input your preference order respectively(AZ,BNT,Moderna):\n"
		self.str4 = "Locked.\n"
		self.str5 = "[Error] Operation failed. Please try again.\n"


	def start_server(self, path, port):
		print(path, port)
		server = subprocess.Popen([path, str(port)])
		time.sleep(0.2)
		#server = subprocess.Popen([path, str(port)], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
		return server
	
	def task1(self):
		try:
			cmd = subprocess.Popen(["make", "-f", "Makefile"])
			cmd.wait()
		except Exception as err:
			print('\033[91m'+'task1: make command failed.'+'\033[0m')
			print(f'Here is the original error msg: {err}')
		else:
			try:
				#print()
				read_proc = self.start_server("./read_server", self.read_port)
				write_proc = self.start_server("./write_server", self.write_port)
			except:
				print('\033[91m'+'task1: start read_server/write_server failed.'+'\033[0m')
				raise
			else:
				try:
					#print()
					read_proc.kill()
					read_proc.wait()
					write_proc.kill()
					write_proc.wait()		
					print('\33[32m'+"===task1 succeed!==="+"\33[0m")	
				except Exception as e:
					print(e)
					raise

	def task2(self):
		read_proc = self.start_server("./read_server", self.read_port)

		try:
			telnet_client = TelnetConnection("localhost", self.read_port)
			telnet_client = telnet_client.telnet()
		except Exception as e:
			print(e)
			raise
		else:
			try:
				#gets = telnet_client.read_eager().decode()
				gets = telnet_client.read_until(b"\n", timeout=0.2).decode()
				print(gets)
				assert (gets == self.str1), '\033[91m'+"task2-1 Failed. Should be\n"+'\033[0m'+self.str1+'\033[91m'+"\nbut have\n"+'\033[0m'+gets+'\033[91m'+"\ninstead."+'\033[0m'
				
				print("\33[3m"+"(Client side input) " +"\33[0m"+"902001")
				telnet_client.write(b"902001\n")
				gets = telnet_client.read_until(b"\n", timeout=0.2).decode()
				print(gets)
				#gets = telnet_client.read_until(b"\n", timeout=0.2).decode()
				#print(gets)
				assert (gets == self.str2_1), '\033[91m'+"task2-1 Failed. Should be\n"+'\033[0m'+self.str2_1+'\033[91m'+"\nbut have\n"+'\033[0m'+gets+'\033[91m'+"\ninstead."+'\033[0m'
				
			except Exception as e:
				print(e)
				#raise
			else:
				print("\33[32m"+"===task2-1 succeed!==="+"\33[0m")
			finally:
				read_proc.kill()
				read_proc.wait()

	def task2_2(self):
		read_proc = self.start_server("./read_server", self.read_port)

		try:
			telnet_client = TelnetConnection("localhost", self.read_port)
			telnet_client = telnet_client.telnet()
		except Exception as e:
			print(e)
			#raise
		else:
			try:
				#gets = telnet_client.read_until(b"\n").decode()
				gets = telnet_client.read_until(b"\n", timeout=0.2).decode()
				#print(gets)
				assert (gets == self.str1), '\033[91m'+"task2-2 Failed. Should be\n"+'\033[0m'+self.str1+'\033[91m'+"\nbut have\n"+'\033[0m'+gets+'\033[91m'+"\ninstead."+'\033[0m'
				print("\33[3m"+"(Client side input) " +"\33[0m"+"999999")
				telnet_client.write(b"999999\n")
				gets = telnet_client.read_until(b"\n", timeout=0.2).decode()
				#print(gets)
				assert (gets == self.str5), '\033[91m'+"task2-2 Failed. Should be\n"+'\033[0m'+self.str5+'\033[91m'+"\nbut have\n"+'\033[0m'+gets+'\033[91m'+"\ninstead."+'\033[0m'
				
			except Exception as e:
				print(e)
				#raise
			else:
				print("\33[32m"+"===task2-2 succeed!==="+"\33[0m")
			finally:
				read_proc.kill()
				read_proc.wait()


	def task3(self):
		str4 = "Preference order for 902001 modified successed, new preference order is BNT > AZ > Moderna.\n"
		# str4_1 = "Preference order for 902001 modified successed, new preference order is AZ > BNT > Moderna.\n"

		write_proc = self.start_server("./write_server", self.write_port)

		try:
			telnet_client = TelnetConnection("localhost", self.write_port)
			telnet_client = telnet_client.telnet()
		except Exception as e:
			print(e)

		try:
			gets = telnet_client.read_until(b"\n", timeout=0.2).decode()
			print("\33[3m"+"(Server output)"+gets)
			assert (gets == self.str1), '\033[91m'+"task3-1 Failed. Should be\n"+'\033[0m'+self.str1+'\033[91m'+"\nbut have\n"+'\033[0m'+gets+'\033[91m'+"\ninstead."+'\033[0m'
			print("\33[3m"+"(Client side input) " +"\33[0m"+"902001")
			telnet_client.write(b"902001\n")
			gets = telnet_client.read_until(b"\n", timeout=0.2).decode()
			print("\33[3m"+"(Server output)"+gets)
			assert (gets == self.str2_1), '\033[91m'+"task3-1 Failed. Should be\n"+'\033[0m'+self.str2_1+'\033[91m'+"\nbut have\n"+'\033[0m'+gets+'\033[91m'+"\ninstead."+'\033[0m'

			
			gets = telnet_client.read_until(b"\n", timeout=0.2).decode()
			print("\33[3m"+"(Server output)"+gets)
			assert (gets == self.str3), '\033[91m'+"task3-1 Failed. Should be\n"+'\033[0m'+self.str3+'\033[91m'+"\nbut have\n"+'\033[0m'+gets+'\033[91m'+"\ninstead."+'\033[0m'
			
			print("\33[3m"+"(Client side input) " +"\33[0m"+"2 1 3")
			telnet_client.write(b"2 1 3\n")
			gets = telnet_client.read_until(b"\n", timeout=0.2).decode()
			print("\33[3m"+"(Server output)"+gets)
			assert (gets == (str4 or str4)), '\033[91m'+"task3-1 Failed. Should be\n"+'\033[0m'+str4+'\033[91m' +"\nor\n" '\033[0m'+str4+'\033[91m'+"\nbut have\n"+'\033[0m'+gets+'\033[91m'+"\ninstead."+'\033[0m'
				
		except AssertionError as e:
			#raise
			print(e)
		else:
			print("\33[32m"+"===task3-1 succeed!==="+"\33[0m")
		finally:
			write_proc.kill()
			write_proc.wait()

	def task6(self):

		task6_str1 = "Your preference order is AZ > BNT > Moderna.\n"
		task6_str2 = "Preference order for 902001 modified successed, new preference order is Moderna > BNT > AZ.\n"

		write_proc = self.start_server("./write_server", self.write_port)
		write_proc2 = self.start_server("./write_server", self.write_port2)

		try:
			telnet_client = TelnetConnection("localhost", self.write_port)
			telnet_client = telnet_client.telnet()
			telnet_client2 = TelnetConnection("localhost", self.write_port2)
			telnet_client2 = telnet_client2.telnet()			
		except Exception as e:
			print(e)

		try:
			gets = telnet_client.read_until(b"\n", timeout=0.2).decode()
			print("\33[3m"+"(Server output)"+gets)
			assert (gets == self.str1), '\033[91m'+"task6-1 Failed. Should be\n"+'\033[0m'+self.str1+'\033[91m'+"\nbut have\n"+'\033[0m'+gets+'\033[91m'+"\ninstead."+'\033[0m'

			gets = telnet_client2.read_until(b"\n", timeout=0.2).decode()
			print("\33[3m"+"(Server2 output)"+gets)
			assert (gets == self.str1), '\033[91m'+"task6-1 Failed. Should be\n"+'\033[0m'+self.str1+'\033[91m'+"\nbut have\n"+'\033[0m'+gets+'\033[91m'+"\ninstead."+'\033[0m'
			
			print("\33[3m"+"(Client1 side input) " +"\33[0m"+"902001")
			telnet_client.write(b"902001\n")
			gets = telnet_client.read_until(b"\n", timeout=0.2).decode()
			print("\33[3m"+"(Server output)"+gets)
			assert (gets == task6_str1), '\033[91m'+"task6-1 Failed. Should be\n"+'\033[0m'+task6_str1+'\033[91m'+"\nbut have\n"+'\033[0m'+gets+'\033[91m'+"\ninstead."+'\033[0m'

			print("\33[3m"+"(Client2 side input) " +"\33[0m"+"902001")
			telnet_client2.write(b"902001\n")
			gets = telnet_client2.read_until(b"\n", timeout=0.2).decode()
			print("\33[3m"+"(Server2 output)"+gets)
			assert (gets == self.str4), '\033[91m'+"task6-1 Failed. Should be\n"+'\033[0m'+self.str4+'\033[91m'+"\nbut have\n"+'\033[0m'+gets+'\033[91m'+"\ninstead."+'\033[0m'

				
			gets = telnet_client.read_until(b"\n", timeout=0.2).decode()
			print("\33[3m"+"(Server output)"+gets)
			assert (gets == self.str3), '\033[91m'+"task6-1 Failed. Should be\n"+'\033[0m'+self.str3+'\033[91m'+"\nbut have\n"+'\033[0m'+gets+'\033[91m'+"\ninstead."+'\033[0m'
			
			print("\33[3m"+"(Client1 side input) " +"\33[0m"+"3 2 1")
			telnet_client.write(b"3 2 1\n")
			gets = telnet_client.read_until(b"\n", timeout=0.2).decode()
			print("\33[3m"+"(Server output)"+gets)
			assert (gets == task6_str2), '\033[91m'+"task6-1 Failed. Should be\n"+'\033[0m'+task6_str2+'\033[91m' +'\033[91m'+"\nbut have\n"+'\033[0m'+gets+'\033[91m'+"\ninstead."+'\033[0m'
				
		except AssertionError as e:
			#raise
			print(e)
		else:
			print("\33[32m"+"===task6-1 succeed!==="+"\33[0m")
		finally:
			write_proc.kill()
			write_proc.wait()	
			write_proc2.kill()
			write_proc2.wait()					

def main():
	scorer = Tasks(dir_='.', port=7772)
	print("=== Making copy of registerRecord...===")
	cmd = subprocess.Popen(["cp", "registerRecord", "registerRecord_copy"])
	cmd.wait()

	cmd = subprocess.Popen(["rm", "read_server"])
	cmd.wait()

	cmd = subprocess.Popen(["rm", "write_server"])
	cmd.wait()

	try:
		print("\33[1m"+"\n=== TASK1: Finish the Makefile. ==="+"\33[0m")
		scorer.task1()
		
	except Exception as e:
		raise
	else:
		try:
			print('\33[1m' + "\n=== TASK2-1: A read server handles requests correctly. ==="+"\33[0m")
			time.sleep(1)
			scorer.task2()
		except Exception as e:
			raise

		try:
			print('\33[1m'+"\n=== TASK3-1: A write server handles requests correctly. ==="+"\33[0m")
			scorer.task3()
		except Exception as e:
			raise
		finally:
			cmd = subprocess.Popen(["cp", "registerRecord_copy", "registerRecord"])
		try:
			print('\33[1m'+"\n=== TASK2-2: A read server handles requests correctly. ==="+"\33[0m")
			scorer.task2_2()
		except Exception as e:
			raise	
		try:
			print('\33[1m'+"\n=== TASK6-1: Requests issued to write servers. ==="+"\33[0m")
			scorer.task6()
		except Exception as e:
			raise			
	finally:
		print("=== Restoring copy of registerRecord...===")
		cmd = subprocess.Popen(["cp", "registerRecord_copy", "registerRecord"])
		cmd = subprocess.Popen(["rm", "registerRecord_copy"])
		cmd = subprocess.Popen(["rm", "read_server"])
		cmd = subprocess.Popen(["rm", "write_server"])

		print('\33[1m'+"Note that, this is a sample judge and it does not contain all testcases/sub-testcases,\nso passing the judge does not guarantee that you get full points of the task(s)."+"\33[0m")
		
	return

if __name__ == '__main__':
	main()

