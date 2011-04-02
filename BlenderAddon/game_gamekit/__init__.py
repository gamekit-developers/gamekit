#Copyright (c) 2010 Xavier Thomas (xat)
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



bl_info = {
    'name': 'Gamekit Engine',
    'author': 'Xavier Thomas (xat)',
    'version': (0,0,601),
    'api': 35899,
    'blender': (2, 5, 7),
    'location': 'Info Window > Render Engine > Gamekit',
    'description': 'Launch game using the fine gamekit engine',
    'wiki_url': 'http://wiki.blender.org/index.php/Extensions:2.5/Py/Scripts/Gamekit_Engine',
    'tracker_url': 'https://projects.blender.org/tracker/index.php?func=detail&aid=23378&group_id=153&atid=514',
    'category': 'Game Engine'}

if "bpy" in locals():
    import imp
    #if "operators" in locals():
    imp.reload(operators)
    imp.reload(ui)
else:
    from . import operators
    from . import ui


#init_data = True

import bpy

def register():
    #bpy.utils.register_module(__name__)
    
    bpy.utils.register_class(operators.GamekitExportStartupFileOperator)
    bpy.utils.register_class(operators.GamekitStartGameOperator)

    ui.addProperties()

def unregister():   
    #bpy.utils.unregister_module(__name__)
    
    bpy.utils.unregister_class(operators.GamekitExportStartupFileOperator)
    bpy.utils.unregister_class(operators.GamekitStartGameOperator)
    
    ui.remProperties()

if __name__ == "__main__":
    register()
