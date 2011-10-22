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
from . import config

# Use some of the existing buttons.
#import properties_game
from bl_ui import properties_game
properties_game.RENDER_PT_game_player.COMPAT_ENGINES.add('GAMEKIT_RENDER')
#properties_game.RENDER_PT_game_performance.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_game.RENDER_PT_game_shading.COMPAT_ENGINES.add('GAMEKIT_RENDER')
#properties_game.RENDER_PT_game_sound.COMPAT_ENGINES.add('GAMEKIT_RENDER')
#properties_game.RENDER_PT_game_stereo.COMPAT_ENGINES.add('GAMEKIT_RENDER')
#properties_game.WORLD_PT_game_context_world.COMPAT_ENGINES.add('GAMEKIT_RENDER')
#properties_game.WORLD_PT_game_world.COMPAT_ENGINES.add('GAMEKIT_RENDER')
#properties_game.WORLD_PT_game_mist.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_game.WORLD_PT_game_physics.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_game.PHYSICS_PT_game_physics.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_game.PHYSICS_PT_game_collision_bounds.COMPAT_ENGINES.add('GAMEKIT_RENDER')
del properties_game

from bl_ui import properties_particle
properties_particle.PARTICLE_PT_context_particles.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_particle.PARTICLE_PT_emission.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_particle.PARTICLE_PT_velocity.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_particle.PARTICLE_PT_rotation.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_particle.PARTICLE_PT_physics.COMPAT_ENGINES.add('GAMEKIT_RENDER')
#properties_particle.PARTICLE_PT_boidbrain.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_particle.PARTICLE_PT_render.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_particle.PARTICLE_PT_draw.COMPAT_ENGINES.add('GAMEKIT_RENDER')
#properties_particle.PARTICLE_PT_children.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_particle.PARTICLE_PT_field_weights.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_particle.PARTICLE_PT_force_fields.COMPAT_ENGINES.add('GAMEKIT_RENDER')
#properties_particle.PARTICLE_PT_vertexgroups.COMPAT_ENGINES.add('GAMEKIT_RENDER')
del properties_particle
    
from bl_ui import properties_material
properties_material.MATERIAL_PT_context_material.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_material.MATERIAL_PT_custom_props.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_material.MATERIAL_PT_diffuse.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_material.MATERIAL_PT_flare.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_material.MATERIAL_PT_halo.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_material.MATERIAL_PT_mirror.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_material.MATERIAL_PT_options.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_material.MATERIAL_PT_physics.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_material.MATERIAL_PT_preview.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_material.MATERIAL_PT_shading.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_material.MATERIAL_PT_shadow.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_material.MATERIAL_PT_specular.COMPAT_ENGINES.add('GAMEKIT_RENDER')
#properties_material.MATERIAL_PT_sss.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_material.MATERIAL_PT_strand.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_material.MATERIAL_PT_transp.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_material.MATERIAL_PT_transp_game.COMPAT_ENGINES.add('GAMEKIT_RENDER')
#properties_material.MATERIAL_PT_volume_density.COMPAT_ENGINES.add('GAMEKIT_RENDER')
#properties_material.MATERIAL_PT_volume_integration.COMPAT_ENGINES.add('GAMEKIT_RENDER')
#properties_material.MATERIAL_PT_volume_lighting.COMPAT_ENGINES.add('GAMEKIT_RENDER')
#properties_material.MATERIAL_PT_volume_options.COMPAT_ENGINES.add('GAMEKIT_RENDER')
#properties_material.MATERIAL_PT_volume_shading.COMPAT_ENGINES.add('GAMEKIT_RENDER')
#properties_material.MATERIAL_PT_volume_transp.COMPAT_ENGINES.add('GAMEKIT_RENDER')
del properties_material

from bl_ui import properties_data_mesh
properties_data_mesh.DATA_PT_context_mesh.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_data_mesh.DATA_PT_custom_props_mesh.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_data_mesh.DATA_PT_normals.COMPAT_ENGINES.add('GAMEKIT_RENDER')
#properties_data_mesh.DATA_PT_settings.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_data_mesh.DATA_PT_shape_keys.COMPAT_ENGINES.add('GAMEKIT_RENDER')
#properties_data_mesh.DATA_PT_texface.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_data_mesh.DATA_PT_uv_texture.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_data_mesh.DATA_PT_vertex_colors.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_data_mesh.DATA_PT_vertex_groups.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_data_mesh.MESH_MT_shape_key_specials.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_data_mesh.MESH_MT_vertex_group_specials.COMPAT_ENGINES.add('GAMEKIT_RENDER')
del properties_data_mesh
 
from bl_ui import properties_texture
properties_texture.TEXTURE_MT_envmap_specials.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_texture.TEXTURE_MT_specials.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_texture.TEXTURE_PT_blend.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_texture.TEXTURE_PT_clouds.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_texture.TEXTURE_PT_colors.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_texture.TEXTURE_PT_context_texture.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_texture.TEXTURE_PT_custom_props.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_texture.TEXTURE_PT_distortednoise.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_texture.TEXTURE_PT_envmap.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_texture.TEXTURE_PT_envmap_sampling.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_texture.TEXTURE_PT_image.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_texture.TEXTURE_PT_image_mapping.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_texture.TEXTURE_PT_image_sampling.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_texture.TEXTURE_PT_influence.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_texture.TEXTURE_PT_magic.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_texture.TEXTURE_PT_mapping.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_texture.TEXTURE_PT_marble.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_texture.TEXTURE_PT_musgrave.COMPAT_ENGINES.add('GAMEKIT_RENDER')
#properties_texture.TEXTURE_PT_plugin.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_texture.TEXTURE_PT_pointdensity.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_texture.TEXTURE_PT_pointdensity_turbulence.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_texture.TEXTURE_PT_preview.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_texture.TEXTURE_PT_stucci.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_texture.TEXTURE_PT_voronoi.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_texture.TEXTURE_PT_voxeldata.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_texture.TEXTURE_PT_wood.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_texture.TextureSlotPanel.COMPAT_ENGINES.add('GAMEKIT_RENDER')
#properties_texture.TextureTypePanel.COMPAT_ENGINES.add('GAMEKIT_RENDER')
del properties_texture

from bl_ui import properties_data_camera
properties_data_camera.DATA_PT_camera.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_data_camera.DATA_PT_camera_display.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_data_camera.DATA_PT_context_camera.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_data_camera.DATA_PT_custom_props_camera.COMPAT_ENGINES.add('GAMEKIT_RENDER')
del properties_data_camera

from bl_ui import properties_data_lamp
properties_data_lamp.DATA_PT_area.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_data_lamp.DATA_PT_context_lamp.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_data_lamp.DATA_PT_custom_props_lamp.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_data_lamp.DATA_PT_falloff_curve.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_data_lamp.DATA_PT_lamp.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_data_lamp.DATA_PT_preview.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_data_lamp.DATA_PT_shadow.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_data_lamp.DATA_PT_spot.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_data_lamp.DATA_PT_sunsky.COMPAT_ENGINES.add('GAMEKIT_RENDER')
properties_data_lamp.LAMP_MT_sunsky_presets.COMPAT_ENGINES.add('GAMEKIT_RENDER')
del properties_data_lamp

from bl_ui import properties_world


class WORLD_PT_world(properties_world.WorldButtonsPanel, bpy.types.Panel):
    bl_label = "World"
    COMPAT_ENGINES = {'GAMEKIT_RENDER'}

    def draw(self, context):
        layout = self.layout
        world = context.world

        row = layout.row()
        row.prop(world, "use_sky_paper")
        row.prop(world, "use_sky_blend")
        row.prop(world, "use_sky_real")

        row = layout.row()
        row.column().prop(world, "horizon_color")
        col = row.column()
        col.prop(world, "zenith_color")
        col.active = world.use_sky_blend
        row.column().prop(world, "ambient_color")



class WORLD_PT_mist(properties_world.WorldButtonsPanel, bpy.types.Panel):
    bl_label = "Mist"
    bl_options = {'DEFAULT_CLOSED'}
    COMPAT_ENGINES = {'GAMEKIT_RENDER'}

    def draw_header(self, context):
        world = context.world

        self.layout.prop(world.mist_settings, "use_mist", text="")

    def draw(self, context):
        layout = self.layout
        world = context.world

        layout.active = world.mist_settings.use_mist

        split = layout.split()

        col = split.column()
        col.prop(world.mist_settings, "intensity", slider=True)
        col.prop(world.mist_settings, "start")

        col = split.column()
        col.prop(world.mist_settings, "depth")
        col.prop(world.mist_settings, "height")

        layout.prop(world.mist_settings, "falloff")

# Base class for ou panels
class RenderButtonsPanel():
    bl_space_type = 'PROPERTIES'
    bl_region_type = 'WINDOW'
    bl_context = "render"
    # COMPAT_ENGINES must be defined in each subclass, external engines can add themselves here
    
    @classmethod
    def poll(self, context):
        rd = context.scene.render
        return (rd.engine in self.COMPAT_ENGINES)

# Runtime panel
class RENDER_PT_gamekit_runtime(RenderButtonsPanel, bpy.types.Panel):
    bl_label = "Gamekit Runtime"
    COMPAT_ENGINES = {'GAMEKIT_RENDER'}

    def draw(self, context):
        layout = self.layout

        scene = context.scene
        gks = scene.gamekit

        col = layout.column()
        
        col.prop(gks, "gk_window_title")
        col.prop(gks, "gk_runtime_exec_path")
        col.prop(gks, "gk_runtime_working_dir")
        col.prop(gks, "gk_log_file")

# Main Gamekit panel
class RENDER_PT_gamekit(RenderButtonsPanel, bpy.types.Panel):
    bl_label = "Gamekit"
    COMPAT_ENGINES = {'GAMEKIT_RENDER'}

    def draw(self, context):
        gks = context.scene.gamekit
        render = context.scene.render
        layout = self.layout
        
        split = layout.split()
        
        col = split.column()
        col.operator("scene.gamekit_start_game", text="Start Game", icon='GAME')
        col.operator("scene.gamekit_export_startup_config", text="Save Config", icon='FILE')
        
        split = layout.split()
        
        col = split.column()
        col.label(text="General::")
        col.prop(gks, "gk_render_system", text="")
        col.prop(gks, "gk_grab_input")
        col.prop(gks, "gk_matblending")
        col.prop(gks, "gk_verbose")
        
        #col = split.column()
        col.label(text="Animation:")
        col.prop(render, "fps")
        col.prop(gks, "gk_start_frame")
        
        #split = layout.split()
        
        col = split.column()
        col.label(text="Shadows:")
        col.prop(gks, "gk_use_shadows")
        row = col.row()
        row.prop(gks, "gk_shadow_type", text="")
        row.active = gks.gk_use_shadows
        row = col.row()
        row.prop(gks, "gk_shadow_color", text="")
        row.active = gks.gk_use_shadows
        row = col.row()
        row.prop(gks, "gk_far_dist_shadow", text="Far")
        row.active = gks.gk_use_shadows


class RENDER_PT_gamekit_performance(RenderButtonsPanel, bpy.types.Panel):
    bl_label = "Performance"
    COMPAT_ENGINES = {'GAMEKIT_RENDER'}

    def draw(self, context):
        layout = self.layout

        gs = context.scene.game_settings
        gks = context.scene.gamekit
        
        split = layout.split()

        col = split.column()
        col.label(text="Debug:")
        col.prop(gs, "show_debug_properties", text="Properties")
        col.prop(gs, "show_framerate_profile", text="Framerate and Profile")
        col.prop(gs, "show_physics_visualization", text="Physics Visualization")
        col.prop(gks, "gk_debug_physicsaabb")
        col.prop(gks, "gk_debug_sound")

        col = split.column()

        col.label(text="Optimisation:")
        col.prop(gks, "gk_frustrum_culling")
        col.prop(gks, "gk_build_instances")


# Setting for this addon
class GamekitSettings(bpy.types.PropertyGroup):
    pass  
    

# Declare gamekit as a render engine
class GamekitRender(bpy.types.RenderEngine):
    bl_idname = 'GAMEKIT_RENDER'
    bl_label = "Gamekit"
    #We do not implement the render function because Gamekit 
    #is not really a render engine.
    

def remProperties():
    # bpy.types.Scene.RemoveProperty("gamekit")
    bpy.utils.unregister_class(GamekitRender)
    bpy.utils.unregister_class(GamekitSettings)    
    
def addProperties():
    bpy.utils.register_class(WORLD_PT_world)
    bpy.utils.register_class(WORLD_PT_mist)

    bpy.utils.register_class(RENDER_PT_gamekit)
    bpy.utils.register_class(RENDER_PT_gamekit_runtime)
    bpy.utils.register_class(RENDER_PT_gamekit_performance)

    bpy.utils.register_class(GamekitRender)
    bpy.utils.register_class(GamekitSettings)    
    
    #bpy.types.Scene.PointerProperty(attr="gamekit", type=GamekitSettings, name="Gamekit", description="Gamekit Settings")
    bpy.types.Scene.gamekit = bpy.props.PointerProperty(type=GamekitSettings, name="Gamekit", description="Gamekit Settings")
    
    cfg = config.GamekitConfig()
    cfg.load_defaults()
        
    GamekitSettings.gk_runtime_exec_path = bpy.props.StringProperty(
                    name="Runtime",
                    description="Path of the gamekit executable",
                    maxlen = 512,
                    default = cfg.get('runtime'),
                    subtype='FILE_PATH')
                    
                    
    GamekitSettings.gk_runtime_working_dir = bpy.props.StringProperty(
                    name="Working Directory",
                    description="Directory in which to launch the game",
                    maxlen = 512,
                    default = cfg.get('workingdir'),
                    subtype='FILE_PATH')

    GamekitSettings.gk_render_system = bpy.props.EnumProperty(
                    name="Render system",
                    items=(('GL', 'OpenGL', 'gl'),
                    ('D3D9', 'Direct 3D 9', 'd3d9'),
                    ('D3D10', 'Direct 3D 10', 'd3d10'),
                    ('D3D11', 'Direct 3D 11', 'd3d11')),
                    default = 'GL')

    GamekitSettings.gk_log_file = bpy.props.StringProperty(
                    name="Log",
                    description="Name of the log file that will be created in the working directory.",
                    maxlen=512,
                    default="OgreKit.log")

    GamekitSettings.gk_window_title = bpy.props.StringProperty(
                    name="Window title",
                    description="Name of the log file that will be created in the working directory.",
                    maxlen=512,
                    default="OgreKit")

    GamekitSettings.gk_grab_input = bpy.props.BoolProperty(
                    name="Grab input",
                    description="Grab mouse and keyboard event (cause other app to be unusable while game is runing).",
                    default=True)

    GamekitSettings.gk_matblending = bpy.props.BoolProperty(
                    name="Material blending",
                    description="Enable material blending mode by ramp blend.",
                    default=False)

    GamekitSettings.gk_verbose = bpy.props.BoolProperty(
                    name="Verbose",
                    description="Verbose output on the console.",
                    default=False)

    GamekitSettings.gk_start_frame = bpy.props.IntProperty(
                    name="Start frame",
                    min=1,
                    soft_min=1,
                    default =1)

    GamekitSettings.gk_frustrum_culling = bpy.props.BoolProperty(
                    name="Frustrum culling",
                    default=True)

    GamekitSettings.gk_debug_sound = bpy.props.BoolProperty(
                    name="Debug sound",
                    default=False)

    GamekitSettings.gk_debug_physicsaabb = bpy.props.BoolProperty(
                    name="Physics AABB",
                    description="Show physics axis aligned bounding boxes",
                    default=False)

    GamekitSettings.gk_build_instances = bpy.props.BoolProperty(
                    name="Build instances",
                    default=False)

    GamekitSettings.gk_use_shadows = bpy.props.BoolProperty(
                    name="Enable shadows",
                    default=True)

    GamekitSettings.gk_shadow_type = bpy.props.EnumProperty(
                    name="Shadow type",
                    items=(('NONE', 'None', 'n'),
                    ('STENCIL_MODULATIVE', 'Stencil modulative', 'sm'),
                    ('STENCIL_ADDITIVE', 'Stencil additive', 'sa'),
                    ('TEXTURE_MODULATIVE', 'Texture modulative', 'tm'),
                    ('TEXTURE_ADDITIVE', 'Texture additive', 'ta'),
                    ('TEXTURE_MODULATIVE_INTEGRATED', 'Texture modulative integrated', 'tmi'),
                    ('TEXTURE_ADDITIVE_INTEGRATED', 'Texture additive integrated', 'tai')),
                    description = "Ogre shadow technique.",
                    default = 'STENCIL_ADDITIVE')

    GamekitSettings.gk_shadow_color = bpy.props.FloatVectorProperty(
                    name="Sadow color",
                    subtype='COLOR',
                    min=0.0,
                    max=1.0,
                    soft_min=0.0,
                    soft_max=1.0,
                    default = (0.0, 0.0, 0.0))

    GamekitSettings.gk_far_dist_shadow = bpy.props.FloatProperty(
                    name="Far distance shadow",
                    min=0.0,
                    max=1.0,
                    soft_min=0.0,
                    soft_max=1.0,
                    default =0.0)
