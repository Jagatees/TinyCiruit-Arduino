import subprocess

# Path to the Python interpreter you want to use
# python_interpreter = "C:/Users/zfcro/AppData/Local/Programs/Python/Python311/python.exe"
python_interpreter = "/Library/Frameworks/Python.framework/Versions/3.11/bin/python3"

#python_interpreter = "C:/Users/user/AppData/Local/Programs/Python/Python311/python.exe"
# run application.py and telebotcircuit.py 


subprocess.Popen([python_interpreter, "application.py"])
subprocess.Popen([python_interpreter, "telebotcircuit.py"])

    