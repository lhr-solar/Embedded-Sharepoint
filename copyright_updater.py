# Copyright (c) 2018-2022 UT Longhorn Racing Solar
# Python script to change copyright information
import os
from pathlib import Path
directory =  os.path.dirname(os.getcwd()) #Obtains directory above current
headers = ['/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */\n', '# Copyright (c) 2018-2023 UT Longhorn Racing Solar\n']


for root, subdirectories, files in os.walk(directory):
    for file in files:
        if file.endswith(".c") or file.endswith('.h') or file.endswith('.py'):  #finds all .c .h .py files
            filepath = os.path.join(root, file)
            f = open(filepath,"r",encoding = "ISO-8859-1")  #meant to fix bug with not recognizing byte 0x92
            text = f.readlines()
            if "Copyright (c)" in text[0] and "Longhorn Racing Solar" in text[0]:
                if (text[0][0] == '/' and text[0][1] == '*') or (text[0][0] == '/' and text[0][1] == '/'): # Checks if first line is comment in c file
                    text[0] = headers[0]
                    out = open(filepath, 'w')
                    out.writelines(text)
                    out.close
                if (text[0][0] == '#'): # Checks if first line is comment in python file
                    text[0] = headers[1]
                    out = open(filepath, 'w')
                    out.writelines(text)
                    out.close
            elif ("STM32F4" not in filepath) and ("CMSIS" not in filepath) and ("uCOS-III" not in filepath):#meant to ignore files which are not ours
                print(filepath)     
                print('\n')
            f.close()
        else:
            continue