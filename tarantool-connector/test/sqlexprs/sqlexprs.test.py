import os
import sys
import subprocess

os.environ['LISTEN'] = str(iproto.py_con.port)

proc = subprocess.Popen("sqlexprs/main", env=os.environ.copy(), stdout=subprocess.PIPE);

sys.stdout.write(proc.communicate()[0])

if proc.returncode != 0:
	raise Exception("Failed executing test")
