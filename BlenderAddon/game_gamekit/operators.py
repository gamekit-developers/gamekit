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
from . import config

def touch(fname):
    if os.path.exists(fname):
        os.utime(fname, None)
    else:
        open(fname, 'w').close()

def is_dir_writable(dir):
    fname = dir + "temporarytestfile.touch"
    try:
        touch(fname)
        os.remove(fname)
    except:
        return False
    return True
    
def assure_temp_dir(dir):
    tmpdir = dir
    if not tmpdir.endswith(os.sep):
        tmpdir += os.sep
    if not os.path.isdir(tmpdir) or not is_dir_writable(tmpdir):
        tmpdir = os.getenv('TEMP')
    if not tmpdir.endswith(os.sep):
        tmpdir += os.sep
    return tmpdir
    
# Declare gamekit as a render engine
class GamekitRender(bpy.types.RenderEngine):
    #bl_idname = 'GAMEKIT_RENDER'
    #bl_label = "Gamekit"
    #We do not implement the render function because Gamekit 
    #is not really a render engine.
    pass

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
                
        cfg = config.GamekitConfig()
        
        #cfg.set("runtime", gks.gk_runtime_exec_path)
        #cfg.set("workingdir", gks.gk_runtime_working_dir)
        cfg.set("rendersystem", gks.gk_render_system.lower())
        cfg.set('log', gks.gk_log_file)
        cfg.set('debugfps', str(gdata.show_framerate_profile))
        cfg.set('debugphysics', str(gdata.show_physics_visualization))
        cfg.set('debugphysicsaabb', str(gks.gk_debug_physicsaabb))
        cfg.set('grabinput', str(gks.gk_grab_input))
        cfg.set('verbose', str(gks.gk_verbose))
        cfg.set('winsize', str(gdata.resolution_x) + " " + str(gdata.resolution_y) )
        cfg.set('wintitle', gks.gk_window_title)
        cfg.set('fullscreen', str(gdata.show_fullscreen))
        cfg.set('animspeed', str(scene.render.fps))
        cfg.set('startframe', str(gks.gk_start_frame))
        if gdata.material_mode != 'TEXTURE_FACE':
            cfg.set('blendermat', "True")
        else:
            cfg.set('blendermat', "False")
        cfg.set('matblending', str(gks.gk_matblending))
        cfg.set('buildinstances', str(gks.gk_build_instances))
        cfg.set('frustrumculling', str(gks.gk_frustrum_culling))
        cfg.set('showdebugprops', str(gdata.show_debug_properties))
        cfg.set('debugsounds', str(gks.gk_debug_sound))
        cfg.set('enableshadows', str(gks.gk_use_shadows))
        cfg.set('shadowtechnique', gks.gk_shadow_type.replace('_','').lower())
        cfg.set('colourshadow', str(gks.gk_shadow_color.r) + " " + str(gks.gk_shadow_color.g) + " " + str(gks.gk_shadow_color.b))
        cfg.set('fardistanceshadow', str(gks.gk_far_dist_shadow))
        
        if cfg.write_config(self.properties.filepath) is False:
            self.report({'ERROR'}, "Error saving gamekit comfig to: " + self.properties.filepath)
            return {'CANCELLED'}
        
        return {'FINISHED'}
        
    
    def invoke(self, context, event):
        self.properties.filepath = "//OgreKitStartup.cfg"
        context.window_manager.fileselect_add(self)
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
        
        user_preferences = context.user_preferences
        addon_prefs = user_preferences.addons[__package__].preferences
        
        execpath = bpy.path.abspath(addon_prefs.runtime_path)
        working_dir = bpy.path.abspath(addon_prefs.working_dir)
        
        #execpath = bpy.path.abspath(gks.gk_runtime_exec_path)
        if execpath[0:2] in { "./", ".\\" }:
            pwd = os.path.dirname(bpy.app.binary_path)
            execpath = pwd + os.sep + execpath
        
        tmpdir = assure_temp_dir(bpy.path.abspath(bpy.context.user_preferences.filepaths.temporary_directory))
        gamefile = tmpdir + "gamekit_tmp.blend"
        cfgfile = tmpdir + "gamekit_startup.cfg"
        cmdline = execpath + " -c " + cfgfile + " " + gamefile
        #workingdir = assure_temp_dir(bpy.path.abspath(gks.gk_runtime_working_dir))
        workingdir = assure_temp_dir(working_dir)
        
        args =  shlex.split(cmdline.replace(os.sep, '/'))        

        # save temporary game .blend         
        returnv = bpy.ops.wm.save_as_mainfile(filepath=gamefile, compress= False, relative_remap=True, check_existing=True, copy=True)
        if returnv != {'FINISHED'}:
            self.report({'ERROR'},"Could not save temporary blend file: " + gamefile)
            return {'CANCELLED'}

        # save temporary game .cfg
        returnv = bpy.ops.scene.gamekit_export_startup_config(filepath=cfgfile)
        if returnv != {'FINISHED'}:
            self.report({'ERROR'},"Could not save temporary cfg file: " + cfgfile)
            return {'CANCELLED'}

        # launch game
        try:
            subp = subprocess.Popen(args, cwd=workingdir, shell=False)
            subp.communicate() #like wait() but without the risk of deadlock with verbose output
            returnv = subp.returncode

            if returnv != 0:
                self.report({'ERROR'},"Gamekit exited anormally.")
                return {'CANCELLED'}

            self.report({'INFO'},"Gamekit exited normally.")

        except OSError as er:
            self.report({'ERROR'}, "Could not launch: " + cmdline + ". In directory:" + workingdir+ ". Error: " + str(er))
            return {'CANCELLED'}
            
        return {'FINISHED'}
     
    def invoke(self, context, event):
        return self.execute(context)
