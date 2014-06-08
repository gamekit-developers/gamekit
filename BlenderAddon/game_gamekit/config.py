#Copyright (c) 2010 harkon.kr
#
# ***** BEGIN MIT LICENSE BLOCK *****
#
#Permission is hereby granted, free of charge, to any person obtaining a copy
#of this software and associated documentation files (the "Software"), to deal
#in the Software without restriction, including without limitation the rights
#to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#copies of the Software, and to permit persons to whom the Software is
#furnished to do so, subject to the following conditions:
#
#The above copyright notice and this permission notice shall be included in
#all copies or substantial portions of the Software.
#
#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#THE SOFTWARE.
#
# ***** END MIT LICENCE BLOCK *****


import bpy
from bpy.types import Operator, AddonPreferences
from bpy.props import StringProperty, IntProperty, BoolProperty
import os, os.path

class GamekitAddonPreferences(AddonPreferences):
    # this must match the addon name, use '__package__'
    # when defining this in a submodule of a python package.
    bl_idname = __package__

    runtime_path = StringProperty(
            name="Runtime File Path",
            subtype='FILE_PATH',
            )
    working_dir = StringProperty(
            name="Working Directory",
            subtype='FILE_PATH',
            )

    def draw(self, context):
        layout = self.layout
        layout.label(text="Gamekit Runtime options")
        layout.prop(self, "runtime_path")
        layout.prop(self, "working_dir")

class GamekitConfig:
    cfg = dict()
    
    defaultswin= {
                'runtime':'./OgreKit/OgreKit-NoDX.exe',
                'workingdir':'//'
                }

    defaultsmac= {
                'runtime':'./OgreKit/AppOgreKit',
                'workingdir':'//'
                }

    defaultslinux= {
                'runtime':'./OgreKit/AppOgreKit',
                'workingdir':'//'
                }

    def load_defaults(self):
        if os.name == "nt":
            self.cfg.update(self.defaultswin)
        elif os.name == "mac":
            self.cfg.update(self.defaultsmac)
        else:
            self.cfg.update(self.defaultslinux)
        return True  

    def read_config(self, fn, clear_cfg = True):
        if clear_cfg: self.cfg = {}
        try:
            f = open(fn)
            lines = f.readlines()
            for s in lines:
                s = s.strip()
                if len(s) > 0 and s[0] != '#':
                    kv = s.split('=', 1)
                    self.cfg[kv[0].strip()] = kv[1].strip()    
        except:
            return False
        
        return True
    
    def write_config(self, fn):
        try:
            file = open(fn, 'w')
        except IOError as er:
            print(str(er))
            return False
    
        for k,v in self.cfg.items():
            file.write(k + " = " +  v + "\n")
            
        file.close()
        
        return True
    
    def get(self, key, defvalue = ""):
        try:
            v = self.cfg[str(key)]
            if not v: return defvalue
            return v
        except:
            return defvalue
        
    def set(self, key, value):
        self.cfg[str(key)] = str(value)
        
    def get_bool(self, key, defvalue = "False"):
        v = self.get(key, defvalue)        
        if v == "" or v.lower() == "false" or v == "0": return False
        return bool(v)
        
    def get_int(self, key, defvalue = "0"):
        try:
            return int(self.get(key, defvalue))
        except:
            return 0
            
    def get_float(self, key, defvalue = "0.0"):
        try:
            return float(self.get(key, defvalue))
        except:
            return 0.0
      
    def get_color(self, key, defvalue = "(0.0, 0.0, 0.0)"):
        try:
            return eval(self.get(key, defvalue))
        except:
            return (0,0, 0.0, 0.0)
