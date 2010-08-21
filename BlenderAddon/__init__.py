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



bl_addon_info = {
    'name': 'Gamekit Engine',
    'author': 'Xavier Thomas (xat)',
    'version': '0.1.525',
    'blender': (2, 5, 4),
    'location': 'Info Window > Render Engine > Gamekit',
    'description': 'Launch game using the fine gamekit engine',
    'wiki_url': 'http://wiki.blender.org/index.php/Extensions:2.5/Py/Scripts/Gamekit_Engine',
    'tracker_url': 'https://projects.blender.org/tracker/index.php?func=detail&aid=22933&group_id=153&atid=467',
    'category': 'Game Engine'}

try:
    init_data

    reload(operators)
    reload(ui)
except:
    from game_gamekit import operators
    from game_gamekit import ui

init_data = True

def register():
    ui.addProperties()

def unregister():
    import bpy
    bpy.types.Scene.RemoveProperty("gamekit")

if __name__ == "__main__":
    register()
