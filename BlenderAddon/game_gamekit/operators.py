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

import bpy
from bpy.props import *
import os
import subprocess
import shlex

# Declare gamekit as a render engine
class GamekitRender(bpy.types.RenderEngine):
    bl_idname = 'GAMEKIT_RENDER'
    bl_label = "Gamekit"
    #We do not implement the render function because Gamekit 
    #is not really a render engine.

# Export startup config fo gamekit
class GamekitExportStartupFileOperator(bpy.types.Operator):
    ''''''
    bl_idname = "scene.gamekit_export_startup_config"
    bl_label = "Export Gamekit Startup.cfg file"
    filepath = StringProperty(name="File Path", description="File path used for exporting the CFG file", maxlen= 1024, default= "")
    
    @classmethod
    def poll(self, context):
        return context.scene.render.engine == 'GAMEKIT_RENDER'

    def execute(self, context):
        gdata = context.scene.game_settings
        gks = context.scene.gamekit
        scene = context.scene

        try:
            file = open(self.properties.filepath, 'w')
        except IOError as er:
            self.report('ERROR', str(er))
            return {'CANCELLED'}
        
        file.write("rendersystem        = ")
        file.write(gks.gk_render_system.lower())
        file.write("\n")
        
        file.write("log                 = ")
        file.write(gks.gk_log_file)
        file.write("\n")
        
        file.write("debugfps            = ")
        file.write( str(gdata.show_framerate_profile))
        file.write("\n")
        
        file.write("debugphysics        = ")
        file.write( str(gdata.show_physics_visualization))
        file.write("\n")
        
        file.write("debugphysicsaabb    = ")
        file.write( str(gks.gk_debug_physicsaabb))
        file.write("\n")
        
        file.write("grabinput           = ")
        file.write( str(gks.gk_grab_input))
        file.write("\n")
        
        file.write("verbose             = ")
        file.write(str(gks.gk_verbose))
        file.write("\n")

        file.write("winsize             = ")
        file.write( str(gdata.resolution_x) + " " + str(gdata.resolution_y) )
        file.write("\n")
        
        file.write("wintitle            = ")
        file.write(gks.gk_window_title)
        file.write("\n")
        
        file.write("fullscreen          = ")
        file.write( str(gdata.show_fullscreen))  
        file.write("\n")      
      
        file.write("animspeed           = ")
        file.write( str(scene.render.fps))
        file.write("\n")
        
        file.write("startframe          = ")
        file.write( str(gks.gk_start_frame))
        file.write("\n")
        
        file.write("blendermat          = ")
        if gdata.material_mode == 'MULTITEXTURE':
            file.write("True")
        else:
            file.write("False")
        file.write("\n")
        
        file.write("buildinstances      = ")
        file.write( str(gks.gk_build_instances))
        file.write("\n")
        
        file.write("frustrumculling     = ")
        file.write( str(gks.gk_frustrum_culling))
        file.write("\n")
        
        file.write("showdebugprops      = ")
        file.write( str(gdata.show_debug_properties))
        file.write("\n")
        
        file.write("debugsounds         = ")
        file.write( str(gks.gk_debug_sound))
        file.write("\n")
        
        file.write("enableshadows       = ")
        file.write( str(gks.gk_use_shadows))
        file.write("\n")
        
        file.write("shadowtechnique     = ")
        file.write(gks.gk_shadow_type.replace('_','').lower())
        file.write("\n")
        
        file.write("colourshadow        = ")
        file.write( str(gks.gk_shadow_color.r) + " " + str(gks.gk_shadow_color.g) + " " + str(gks.gk_shadow_color.b))
        file.write("\n")
        
        file.write("fardistanceshadow   = ")
        file.write( str(gks.gk_far_dist_shadow))
        file.write("\n")
        
        file.close()
        return {'FINISHED'}
    
    def invoke(self, context, event):
        context.manager.add_fileselect(self)
        return {'RUNNING_MODAL'}
    

# Start gamekit
class GamekitStartGameOperator(bpy.types.Operator):
    ''''''
    bl_idname = "scene.gamekit_start_game"
    bl_label = "Start Game"
    
    @classmethod
    def poll(self, context):
        return context.scene.render.engine == 'GAMEKIT_RENDER'

    def execute(self, context):
        scene = context.scene
        gks = scene.gamekit
        
        if os.sys.platform == 'darwin' and gks.gk_runtime_exec_path[0] != '/':
            blend_path = os.path.dirname(bpy.app.binary_path)
            gks.gk_runtime_exec_path = blend_path + '/' + gks.gk_runtime_exec_path
        else:
            # make sure path are absolute 
            gks.gk_runtime_exec_path = bpy.path.abspath(gks.gk_runtime_exec_path)
            #gks.gk_runtime_working_dir = bpy.utils.expandpath(gks.gk_runtime_working_dir)
            if not gks.gk_runtime_working_dir.endswith(os.sep):
                gks.gk_runtime_working_dir += os.sep
            #gks.gk_export_tmp_dir = bpy.utils.expandpath(gks.gk_export_tmp_dir)
            if not gks.gk_export_tmp_dir.endswith(os.sep):
                gks.gk_export_tmp_dir += os.sep

        gamefile = gks.gk_export_tmp_dir + "BOgreKitGame.blend"
        cfgfile = gks.gk_export_tmp_dir + "BOgreKitStartup.cfg"
        cmdline = gks.gk_runtime_exec_path + " -c " + cfgfile + " " + gamefile
        
        args =  shlex.split(cmdline.replace(os.sep, '/'))        

        # save temporary game .blend         
        returnv = bpy.ops.wm.save_as_mainfile(filepath=gamefile, compress= False, relative_remap=True, check_existing=True, copy=True)
        if returnv != {'FINISHED'}:
            self.report('ERROR',"Could not save temporary blend file: " + gamefile)
            return {'CANCELLED'}

        # save temporary game .cfg
        returnv = bpy.ops.scene.gamekit_export_startup_config(filepath=cfgfile)
        if returnv != {'FINISHED'}:
            self.report('ERROR',"Could not save temporary cfg file: " + cfgfile)
            return {'CANCELLED'}

        # launch game
        try:
            subp = subprocess.Popen(args, cwd=gks.gk_runtime_working_dir, shell=False)
            subp.communicate() #like wait() but without the risk of deadlock with verbose output
            returnv = subp.returncode

            if returnv != 0:
                self.report('ERROR',"Gamekit exited anormally.")
                return {'CANCELLED'}

            self.report('INFO',"Gamekit exited normally.")

        except OSError as er:
            self.report('ERROR', "Could not launch: " + cmdline + " in " + gks.gk_runtime_working_dir+ " : " + str(er))
            return {'CANCELLED'}
            
        return {'FINISHED'}
     
    def invoke(self, context, event):
        return self.execute(context)
