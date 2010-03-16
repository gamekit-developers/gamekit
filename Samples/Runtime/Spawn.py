## ----------------------------------------------------------------------------
## This file is part of OgreKit.
## http://gamekit.googlecode.com/
## 
## The contents of this file are placed in the public domain. Feel
## free to make use of it in any way you like.
## 
## ----------------------------------------------------------------------------
## 
## This is a test for executing OgreKit from inside Blender. 
## Looks for the OgreKit executable by searching the 'PATH' environment variable.
## ----------------------------------------------------------------------------
import subprocess, Blender


def findOgreKit():
	import os, sys
	
	if (os.environ.has_key("PATH")):

		spath = []
		if sys.platform == 'win32':
			spath = os.environ['PATH'].split(';')
		else:
			# unix
			spath = os.environ['PATH'].split(':')


		for i in spath:
			if os.path.isfile(i + os.sep + "OgreKit.exe"):
				return i + os.sep + "OgreKit.exe"
			elif os.path.isfile(i + os.sep + "OgreKit.bat"):
				return i + os.sep + "OgreKit.bat"
			elif os.path.isfile(i + os.sep + "OgreKit"):
				return i + os.sep + "OgreKit"
	return None	

found = findOgreKit()
if found != None:
	subprocess.Popen([found, Blender.Get('filename')])

