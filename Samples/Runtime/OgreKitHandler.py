# SPACEHANDLER.VIEW3D.EVENT
import Blender
from Blender import Draw
import subprocess
evt = Blender.event
return_it = False



if evt == Draw.PKEY:
    print "Starting GameKit Test"

    def findOgreKit():
        import os, sys
        home = Blender.Get('homedir')
        if home != None:
            if os.path.isfile(home + os.sep + "OgreKit.exe"):
                return home + os.sep + "OgreKit.exe"
            elif os.path.isfile(home + os.sep + "OgreKit.bat"):
                return home + os.sep + "OgreKit.bat"
            elif os.path.isfile(home + os.sep + "OgreKit"):
                return home + os.sep + "OgreKit"
        return None	

    found = findOgreKit()
    if found != None:
        subprocess.Popen([found, Blender.Get('filename')])
else:
    return_it = True

if not return_it: Blender.event = None
