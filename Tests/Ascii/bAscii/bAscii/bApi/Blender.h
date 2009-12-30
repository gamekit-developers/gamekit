/*
-------------------------------------------------------------------------------
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C & Erwin Coumans
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
	 claim that you wrote the original software. If you use this software
	 in a product, an acknowledgment in the product documentation would be
	 appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
	 misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#ifndef _Blender_h_
#define _Blender_h_


namespace Blender {
class Link;
class LinkData;
class ListBase;
class vec2s;
class vec2i;
class vec2f;
class vec2d;
class vec3i;
class vec3f;
class vec3d;
class vec4i;
class vec4f;
class vec4d;
class rcti;
class rctf;
class IDPropertyData;
class IDProperty;
class ID;
class Library;
class PreviewImage;
class IpoDriver;
class IpoCurve;
class Ipo;
class KeyBlock;
class Key;
class ScriptLink;
class TextLine;
class TextMarker;
class Text;
class PackedFile;
class Camera;
class ImageUser;
class Image;
class MTex;
class PluginTex;
class CBData;
class ColorBand;
class EnvMap;
class Tex;
class TexMapping;
class Lamp;
class Wave;
class Material;
class VFont;
class MetaElem;
class MetaBall;
class BezTriple;
class BPoint;
class Nurb;
class CharInfo;
class TextBox;
class Curve;
class Mesh;
class TFace;
class MFace;
class MEdge;
class MDeformWeight;
class MDeformVert;
class MVert;
class MCol;
class MTexPoly;
class MLoopUV;
class MLoopCol;
class MSticky;
class MSelect;
class MTFace;
class MFloatProperty;
class MIntProperty;
class MStringProperty;
class OrigSpaceFace;
class MultiresCol;
class MultiresColFace;
class MultiresFace;
class MultiresEdge;
class MultiresLevel;
class Multires;
class PartialVisibility;
class ModifierData;
class SubsurfModifierData;
class LatticeModifierData;
class CurveModifierData;
class BuildModifierData;
class MaskModifierData;
class ArrayModifierData;
class MirrorModifierData;
class EdgeSplitModifierData;
class BevelModifierData;
class BMeshModifierData;
class DisplaceModifierData;
class UVProjectModifierData;
class DecimateModifierData;
class SmoothModifierData;
class CastModifierData;
class WaveModifierData;
class ArmatureModifierData;
class HookModifierData;
class SoftbodyModifierData;
class ClothModifierData;
class CollisionModifierData;
class SurfaceModifierData;
class BooleanModifierData;
class MDefInfluence;
class MDefCell;
class MeshDeformModifierData;
class ParticleSystemModifierData;
class ParticleInstanceModifierData;
class ExplodeModifierData;
class FluidsimModifierData;
class ShrinkwrapModifierData;
class SimpleDeformModifierData;
class Lattice;
class bDeformGroup;
class BoundBox;
class Object;
class ObHook;
class PartDeflect;
class PointCache;
class SBVertex;
class BulletSoftBody;
class SoftBody;
class FluidsimSettings;
class World;
class Radio;
class Base;
class AviCodecData;
class QuicktimeCodecData;
class FFMpegCodecData;
class AudioData;
class SceneRenderLayer;
class RenderData;
class RenderProfile;
class GameFraming;
class TimeMarker;
class ImagePaintSettings;
class ParticleBrushData;
class ParticleEditSettings;
class TransformOrientation;
class ToolSettings;
class BrushData;
class SculptData;
class Scene;
class BGpic;
class View3D;
class View2D;
class SpaceLink;
class SpaceInfo;
class SpaceIpo;
class SpaceButs;
class SpaceSeq;
class SpaceFile;
class SpaceOops;
class SpaceImage;
class SpaceNla;
class SpaceText;
class Script;
class SpaceScript;
class SpaceTime;
class SpaceNode;
class SpaceImaSel;
class ThemeUI;
class ThemeSpace;
class ThemeWireColor;
class bTheme;
class SolidLight;
class UserDef;
class bScreen;
class ScrVert;
class ScrEdge;
class Panel;
class ScrArea;
class FileGlobal;
class StripElem;
class TStripElem;
class StripCrop;
class StripTransform;
class StripColorBalance;
class StripProxy;
class Strip;
class PluginSeq;
class Sequence;
class MetaStack;
class Editing;
class WipeVars;
class GlowVars;
class TransformVars;
class SolidColorVars;
class SpeedControlVars;
class Effect;
class BuildEff;
class PartEff;
class WaveEff;
class TreeStoreElem;
class TreeStore;
class Oops;
class bProperty;
class bNearSensor;
class bMouseSensor;
class bTouchSensor;
class bKeyboardSensor;
class bPropertySensor;
class bActuatorSensor;
class bDelaySensor;
class bCollisionSensor;
class bRadarSensor;
class bRandomSensor;
class bRaySensor;
class bMessageSensor;
class bSensor;
class bJoystickSensor;
class bExpressionCont;
class bPythonCont;
class bController;
class bAddObjectActuator;
class bActionActuator;
class bSoundActuator;
class bCDActuator;
class bEditObjectActuator;
class bSceneActuator;
class bPropertyActuator;
class bObjectActuator;
class bIpoActuator;
class bCameraActuator;
class bConstraintActuator;
class bGroupActuator;
class bRandomActuator;
class bMessageActuator;
class bGameActuator;
class bVisibilityActuator;
class bTwoDFilterActuator;
class bParentActuator;
class bStateActuator;
class bActuator;
class FreeCamera;
class bSound;
class bSoundListener;
class SpaceSound;
class GroupObject;
class Group;
class Bone;
class bArmature;
class bPoseChannel;
class bPose;
class bActionGroup;
class bActionChannel;
class bAction;
class SpaceAction;
class bConstraintChannel;
class bConstraint;
class bConstraintTarget;
class bPythonConstraint;
class bKinematicConstraint;
class bTrackToConstraint;
class bRotateLikeConstraint;
class bLocateLikeConstraint;
class bMinMaxConstraint;
class bSizeLikeConstraint;
class bActionConstraint;
class bLockTrackConstraint;
class bFollowPathConstraint;
class bStretchToConstraint;
class bRigidBodyJointConstraint;
class bClampToConstraint;
class bChildOfConstraint;
class bTransformConstraint;
class bLocLimitConstraint;
class bRotLimitConstraint;
class bSizeLimitConstraint;
class bDistLimitConstraint;
class bShrinkwrapConstraint;
class bActionModifier;
class bActionStrip;
class bNodeStack;
class bNodeSocket;
class bNode;
class bNodeLink;
class bNodeTree;
class NodeImageAnim;
class NodeBlurData;
class NodeDBlurData;
class NodeBilateralBlurData;
class NodeHueSat;
class NodeImageFile;
class NodeChroma;
class NodeTwoXYs;
class NodeTwoFloats;
class NodeGeometry;
class NodeVertexCol;
class NodeDefocus;
class NodeScriptDict;
class NodeGlare;
class NodeTonemap;
class NodeLensDist;
class TexNodeOutput;
class CurveMapPoint;
class CurveMap;
class CurveMapping;
class BrushClone;
class Brush;
class CustomDataLayer;
class CustomData;
class HairKey;
class ParticleKey;
class ChildParticle;
class ParticleData;
class ParticleSettings;
class ParticleSystem;
class ClothSimSettings;
class ClothCollSettings;
class bGPDspoint;
class bGPDstroke;
class bGPDframe;
class bGPDlayer;
class bGPdata;
class TreeStoreElem
{
public:
    short type;
    short nr;
    short flag;
    short used;
    ID *id;
};

class CustomData
{
public:
    CustomDataLayer *layers;
    int totlayer;
    int maxlayer;
    int totsize;
    int pad1;
    void *pool;
};

class Link
{
public:
    Link *next;
    Link *prev;
};

class LinkData
{
public:
    LinkData *next;
    LinkData *prev;
    void *data;
};

class ListBase
{
public:
    void *first;
    void *last;
};

class vec2s
{
public:
    short x;
    short y;
};

class vec2i
{
public:
    int x;
    int y;
};

class vec2f
{
public:
    float x;
    float y;
};

class vec2d
{
public:
    double x;
    double y;
};

class vec3i
{
public:
    int x;
    int y;
    int z;
};

class vec3f
{
public:
    float x;
    float y;
    float z;
};

class vec3d
{
public:
    double x;
    double y;
    double z;
};

class vec4i
{
public:
    int x;
    int y;
    int z;
    int w;
};

class vec4f
{
public:
    float x;
    float y;
    float z;
    float w;
};

class vec4d
{
public:
    double x;
    double y;
    double z;
    double w;
};

class rcti
{
public:
    int xmin;
    int xmax;
    int ymin;
    int ymax;
};

class rctf
{
public:
    float xmin;
    float xmax;
    float ymin;
    float ymax;
};

class IDPropertyData
{
public:
    void *pointer;
    ListBase group;
    int val;
    int val2;
};

class IDProperty
{
public:
    IDProperty *next;
    IDProperty *prev;
    char name[32];
    char type;
    char subtype;
    short flag;
    int saved;
    IDPropertyData data;
    int len;
    int totallen;
};

class ID
{
public:
    void *next;
    void *prev;
    ID *newid;
    Library *lib;
    char name[24];
    short us;
    short flag;
    int icon_id;
    IDProperty *properties;
};

class Library
{
public:
    ID id;
    ID *idblock;
    void *filedata;
    char name[240];
    char filename[240];
    int tot;
    int pad1;
    Library *parent;
};

class PreviewImage
{
public:
    int w[2];
    int h[2];
    short changed[2];
    short pad1;
    short pad2;
    int *rect[2];
};

class IpoDriver
{
public:
    Object *ob;
    short blocktype;
    short adrcode;
    short type;
    short flag;
    char name[128];
};

class IpoCurve
{
public:
    IpoCurve *next;
    IpoCurve *prev;
    BPoint *bp;
    BezTriple *bezt;
    rctf maxrct;
    rctf totrct;
    short blocktype;
    short adrcode;
    short vartype;
    short totvert;
    short ipo;
    short extrap;
    short flag;
    short rt;
    float ymin;
    float ymax;
    int bitmask;
    float slide_min;
    float slide_max;
    float curval;
    IpoDriver *driver;
};

class Ipo
{
public:
    ID id;
    ListBase curve;
    rctf cur;
    short blocktype;
    short showkey;
    short muteipo;
    short pad1;
};

class KeyBlock
{
public:
    KeyBlock *next;
    KeyBlock *prev;
    float pos;
    float curval;
    short type;
    short adrcode;
    short relative;
    short flag;
    int totelem;
    int pad1;
    void *data;
    float *weights;
    char name[32];
    char vgroup[32];
    float slidermin;
    float slidermax;
};

class Key
{
public:
    ID id;
    KeyBlock *refkey;
    char elemstr[32];
    int elemsize;
    float curval;
    ListBase block;
    Ipo *ipo;
    ID *from;
    short type;
    short totkey;
    short slurph;
    short flag;
};

class ScriptLink
{
public:
    ID **scripts;
    short *flag;
    short actscript;
    short totscript;
    int pad1;
};

class TextLine
{
public:
    TextLine *next;
    TextLine *prev;
    char *line;
    char *format;
    int len;
    int blen;
};

class TextMarker
{
public:
    TextMarker *next;
    TextMarker *prev;
    int lineno;
    int start;
    int end;
    int pad1;
    int group;
    int flags;
    char color[4];
    char pad2;
};

class Text
{
public:
    ID id;
    char *name;
    int flags;
    int nlines;
    ListBase lines;
    TextLine *curl;
    TextLine *sell;
    int curc;
    int selc;
    ListBase markers;
    char *undo_buf;
    int undo_pos;
    int undo_len;
    void *compiled;
    double mtime;
};

class PackedFile
{
public:
    int size;
    int seek;
    int flags;
    int pad1;
    void *data;
};

class Camera
{
public:
    ID id;
    short type;
    short flag;
    float passepartalpha;
    float angle;
    float clipsta;
    float clipend;
    float lens;
    float ortho_scale;
    float drawsize;
    float shiftx;
    float shifty;
    float YF_dofdist;
    float YF_aperture;
    short YF_bkhtype;
    short YF_bkhbias;
    float YF_bkhrot;
    Ipo *ipo;
    ScriptLink scriptlink;
    Object *dof_ob;
};

class ImageUser
{
public:
    int framenr;
    int frames;
    int offset;
    int sfra;
    short fie_ima;
    short cycl;
    short flag;
    short ok;
    short multi_index;
    short layer;
    short pass;
    short menunr;
};

class Image
{
public:
    ID id;
    char name[240];
    ListBase ibufs;
    void *gputexture;
    void *anim;
    void *rr;
    short ok;
    short flag;
    short source;
    short type;
    short pad1;
    short pad2;
    int lastframe;
    short tpageflag;
    short totbind;
    short xrep;
    short yrep;
    short twsta;
    short twend;
    int bindcode;
    int *repbind;
    PackedFile *packedfile;
    PreviewImage *preview;
    float lastupdate;
    int lastused;
    short animspeed;
    short gen_x;
    short gen_y;
    short gen_type;
    float aspx;
    float aspy;
    void *vnode;
};

class MTex
{
public:
    short texco;
    short mapto;
    short maptoneg;
    short blendtype;
    Object *object;
    Tex *tex;
    char uvname[32];
    char projx;
    char projy;
    char projz;
    char mapping;
    float ofs[3];
    float size[3];
    short texflag;
    short colormodel;
    short pmapto;
    short pmaptoneg;
    short normapspace;
    short which_output;
    short pad1;
    float r;
    float g;
    float b;
    float k;
    float def_var;
    float rt;
    float colfac;
    float norfac;
    float varfac;
    float dispfac;
    float warpfac;
};

class PluginTex
{
public:
    char name[160];
    void *handle;
    char *pname;
    char *stnames;
    int stypes;
    int vars;
    void *varstr;
    float *result;
    float *cfra;
    float data[32];
    int (*doit)();
    void (*instance_init)();
    void (*callback)();
    int version;
    int pad1;
};

class CBData
{
public:
    float r;
    float g;
    float b;
    float a;
    float pos;
    int cur;
};

class ColorBand
{
public:
    short flag;
    short tot;
    short cur;
    short ipotype;
    CBData data[32];
};

class EnvMap
{
public:
    Object *object;
    Image *ima;
    void *cube[6];
    float imat[4][4];
    float obimat[3][3];
    short type;
    short stype;
    float clipsta;
    float clipend;
    float viewscale;
    int notlay;
    short cuberes;
    short depth;
    int ok;
    int lastframe;
    short recalc;
    short lastsize;
};

class Tex
{
public:
    ID id;
    float noisesize;
    float turbul;
    float bright;
    float contrast;
    float rfac;
    float gfac;
    float bfac;
    float filtersize;
    float mg_H;
    float mg_lacunarity;
    float mg_octaves;
    float mg_offset;
    float mg_gain;
    float dist_amount;
    float ns_outscale;
    float vn_w1;
    float vn_w2;
    float vn_w3;
    float vn_w4;
    float vn_mexp;
    short vn_distm;
    short vn_coltype;
    short noisedepth;
    short noisetype;
    short noisebasis;
    short noisebasis2;
    short imaflag;
    short flag;
    short type;
    short stype;
    float cropxmin;
    float cropymin;
    float cropxmax;
    float cropymax;
    short xrepeat;
    short yrepeat;
    short extend;
    short fie_ima;
    int len;
    int frames;
    int offset;
    int sfra;
    float checkerdist;
    float nabla;
    float norfac;
    ImageUser iuser;
    bNodeTree *nodetree;
    Ipo *ipo;
    Image *ima;
    PluginTex *plugin;
    ColorBand *coba;
    EnvMap *env;
    PreviewImage *preview;
    char use_nodes;
    char pad1;
};

class TexMapping
{
public:
    float loc[3];
    float rot[3];
    float size[3];
    int flag;
    float mat[4][4];
    float min[3];
    float max[3];
    Object *ob;
};

class Lamp
{
public:
    ID id;
    short type;
    short pad1;
    int mode;
    short colormodel;
    short totex;
    float r;
    float g;
    float b;
    float k;
    float shdwr;
    float shdwg;
    float shdwb;
    float shdwpad;
    float energy;
    float dist;
    float spotsize;
    float spotblend;
    float haint;
    float att1;
    float att2;
    CurveMapping *curfalloff;
    short falloff_type;
    short pad2;
    float clipsta;
    float clipend;
    float shadspotsize;
    float bias;
    float soft;
    short bufsize;
    short samp;
    short buffers;
    short filtertype;
    char bufflag;
    char buftype;
    short ray_samp;
    short ray_sampy;
    short ray_sampz;
    short ray_samp_type;
    short area_shape;
    float area_size;
    float area_sizey;
    float area_sizez;
    float adapt_thresh;
    short ray_samp_method;
    short pad3;
    short texact;
    short shadhalostep;
    short sun_effect_type;
    short skyblendtype;
    float horizon_brightness;
    float spread;
    float sun_brightness;
    float sun_size;
    float backscattered_light;
    float sun_intensity;
    float atm_turbidity;
    float atm_inscattering_factor;
    float atm_extinction_factor;
    float atm_distance_factor;
    float skyblendfac;
    float sky_exposure;
    short sky_colorspace;
    short pad4;
    int YF_numphotons;
    int YF_numsearch;
    short YF_phdepth;
    short YF_useqmc;
    short YF_bufsize;
    short YF_pad;
    float YF_causticblur;
    float YF_ltradius;
    float YF_glowint;
    float YF_glowofs;
    short YF_glowtype;
    short YF_pad2;
    MTex *mtex[18];
    Ipo *ipo;
    PreviewImage *preview;
    ScriptLink scriptlink;
};

class Wave
{
public:
    ID id;
    Ipo *ipo;
};

class Material
{
public:
    ID id;
    short colormodel;
    short flag;
    float r;
    float g;
    float b;
    float specr;
    float specg;
    float specb;
    float mirr;
    float mirg;
    float mirb;
    float ambr;
    float ambb;
    float ambg;
    float amb;
    float emit;
    float ang;
    float spectra;
    float ray_mirror;
    float alpha;
    float ref;
    float spec;
    float zoffs;
    float add;
    float translucency;
    float fresnel_mir;
    float fresnel_mir_i;
    float fresnel_tra;
    float fresnel_tra_i;
    float filter;
    float tx_limit;
    float tx_falloff;
    short ray_depth;
    short ray_depth_tra;
    short har;
    char seed1;
    char seed2;
    float gloss_mir;
    float gloss_tra;
    short samp_gloss_mir;
    short samp_gloss_tra;
    float adapt_thresh_mir;
    float adapt_thresh_tra;
    float aniso_gloss_mir;
    float dist_mir;
    short fadeto_mir;
    short shade_flag;
    int mode;
    int mode_l;
    short flarec;
    short starc;
    short linec;
    short ringc;
    float hasize;
    float flaresize;
    float subsize;
    float flareboost;
    float strand_sta;
    float strand_end;
    float strand_ease;
    float strand_surfnor;
    float strand_min;
    float strand_widthfade;
    char strand_uvname[32];
    float sbias;
    float lbias;
    float shad_alpha;
    int septex;
    char rgbsel;
    char texact;
    char pr_type;
    char use_nodes;
    short pr_back;
    short pr_lamp;
    short pad1;
    short ml_flag;
    short diff_shader;
    short spec_shader;
    float roughness;
    float refrac;
    float param[4];
    float rms;
    float darkness;
    short texco;
    short mapto;
    ColorBand *ramp_col;
    ColorBand *ramp_spec;
    char rampin_col;
    char rampin_spec;
    char rampblend_col;
    char rampblend_spec;
    short ramp_show;
    short pad2;
    float rampfac_col;
    float rampfac_spec;
    MTex *mtex[18];
    bNodeTree *nodetree;
    Ipo *ipo;
    Group *group;
    PreviewImage *preview;
    float friction;
    float fh;
    float reflect;
    float fhdist;
    float xyfrict;
    short dynamode;
    short pad3;
    float sss_radius[3];
    float sss_col[3];
    float sss_error;
    float sss_scale;
    float sss_ior;
    float sss_colfac;
    float sss_texfac;
    float sss_front;
    float sss_back;
    short sss_flag;
    short sss_preset;
    float YF_ar;
    float YF_ag;
    float YF_ab;
    float YF_dscale;
    float YF_dpwr;
    int YF_dsmp;
    int YF_preset;
    int YF_djit;
    ScriptLink scriptlink;
    ListBase gpumaterial;
};

class VFont
{
public:
    ID id;
    char name[256];
    float scale;
    float pad1;
    void *data;
    PackedFile *packedfile;
};

class MetaElem
{
public:
    MetaElem *next;
    MetaElem *prev;
    BoundBox *bb;
    int i1;
    int j1;
    int k1;
    int i2;
    int j2;
    int k2;
    short type;
    short flag;
    short selcol1;
    short selcol2;
    float x;
    float y;
    float z;
    float quat[4];
    float expx;
    float expy;
    float expz;
    float rad;
    float rad2;
    float s;
    float len;
    float *mat;
    float *imat;
};

class MetaBall
{
public:
    ID id;
    BoundBox *bb;
    ListBase elems;
    ListBase disp;
    Ipo *ipo;
    Material **mat;
    short flag;
    short totcol;
    int texflag;
    float loc[3];
    float size[3];
    float rot[3];
    float wiresize;
    float rendersize;
    float thresh;
};

class BezTriple
{
public:
    float vec[3][3];
    float alfa;
    float weight;
    float radius;
    short h1;
    short h2;
    char f1;
    char f2;
    char f3;
    char hide;
};

class BPoint
{
public:
    float vec[4];
    float alfa;
    float weight;
    short f1;
    short hide;
    float radius;
    float pad1;
};

class Nurb
{
public:
    Nurb *next;
    Nurb *prev;
    short type;
    short mat_nr;
    short hide;
    short flag;
    short pntsu;
    short pntsv;
    short resolu;
    short resolv;
    short orderu;
    short orderv;
    short flagu;
    short flagv;
    float *knotsu;
    float *knotsv;
    BPoint *bp;
    BezTriple *bezt;
    short tilt_interp;
    short radius_interp;
    int charidx;
};

class CharInfo
{
public:
    short kern;
    short mat_nr;
    char flag;
    char pad1;
    short pad2;
};

class TextBox
{
public:
    float x;
    float y;
    float w;
    float h;
};

class Curve
{
public:
    ID id;
    BoundBox *bb;
    ListBase nurb;
    ListBase disp;
    Object *bevobj;
    Object *taperobj;
    Object *textoncurve;
    Ipo *ipo;
    void *path;
    Key *key;
    Material **mat;
    ListBase bev;
    float loc[3];
    float size[3];
    float rot[3];
    int texflag;
    short pathlen;
    short totcol;
    short flag;
    short bevresol;
    float width;
    float ext1;
    float ext2;
    short resolu;
    short resolv;
    short resolu_ren;
    short resolv_ren;
    int pad1;
    short len;
    short lines;
    short pos;
    short spacemode;
    float spacing;
    float linedist;
    float shear;
    float fsize;
    float wordspace;
    float ulpos;
    float ulheight;
    float xof;
    float yof;
    float linewidth;
    char *str;
    char family[24];
    VFont *vfont;
    VFont *vfontb;
    VFont *vfonti;
    VFont *vfontbi;
    int sepchar;
    int totbox;
    int actbox;
    int pad2;
    TextBox *tb;
    int selstart;
    int selend;
    CharInfo *strinfo;
    CharInfo curinfo;
};

class Mesh
{
public:
    ID id;
    BoundBox *bb;
    ListBase effect;
    Ipo *ipo;
    Key *key;
    Material **mat;
    MFace *mface;
    MTFace *mtface;
    TFace *tface;
    MVert *mvert;
    MEdge *medge;
    MDeformVert *dvert;
    MCol *mcol;
    MSticky *msticky;
    Mesh *texcomesh;
    MSelect *mselect;
    CustomData vdata;
    CustomData edata;
    CustomData fdata;
    int totvert;
    int totedge;
    int totface;
    int totselect;
    int act_face;
    int texflag;
    float loc[3];
    float size[3];
    float rot[3];
    float cubemapsize;
    float pad1;
    short smoothresh;
    short flag;
    short subdiv;
    short subdivr;
    short totcol;
    short subsurftype;
    Multires *mr;
    PartialVisibility *pv;
    void *vnode;
};

class TFace
{
public:
    void *tpage;
    float uv[4][2];
    int col[4];
    char flag;
    char transp;
    short mode;
    short tile;
    short unwrap;
};

class MFace
{
public:
    int v1;
    int v2;
    int v3;
    int v4;
    char pad1;
    char mat_nr;
    char edcode;
    char flag;
};

class MEdge
{
public:
    int v1;
    int v2;
    char crease;
    char bweight;
    short flag;
};

class MDeformWeight
{
public:
    int def_nr;
    float weight;
};

class MDeformVert
{
public:
    MDeformWeight *dw;
    int totweight;
    int flag;
};

class MVert
{
public:
    float co[3];
    short no[3];
    char flag;
    char mat_nr;
    char bweight;
    char pad1;
};

class MCol
{
public:
    char a;
    char r;
    char g;
    char b;
};

class MTexPoly
{
public:
    Image *tpage;
    char flag;
    char transp;
    short mode;
    short tile;
    short unwrap;
};

class MLoopUV
{
public:
    float uv[2];
};

class MLoopCol
{
public:
    char a;
    char r;
    char g;
    char b;
    int pad1;
};

class MSticky
{
public:
    float co[2];
};

class MSelect
{
public:
    int index;
    int type;
};

class MTFace
{
public:
    float uv[4][2];
    Image *tpage;
    char flag;
    char transp;
    short mode;
    short tile;
    short unwrap;
};

class MFloatProperty
{
public:
    float f;
};

class MIntProperty
{
public:
    int i;
};

class MStringProperty
{
public:
    char s[256];
};

class OrigSpaceFace
{
public:
    float uv[4][2];
};

class MultiresCol
{
public:
    float a;
    float r;
    float g;
    float b;
};

class MultiresColFace
{
public:
    MultiresCol col[4];
};

class MultiresFace
{
public:
    int v[4];
    int mid;
    char flag;
    char mat_nr;
    char pad1;
};

class MultiresEdge
{
public:
    int v[2];
    int mid;
};

class MultiresLevel
{
public:
    MultiresLevel *next;
    MultiresLevel *prev;
    MultiresFace *faces;
    MultiresColFace *colfaces;
    MultiresEdge *edges;
    char *edge_boundary_states;
    ListBase *vert_edge_map;
    ListBase *vert_face_map;
    void *map_mem;
    int totvert;
    int totface;
    int totedge;
    int pad1;
    MVert *verts;
};

class Multires
{
public:
    ListBase levels;
    MVert *verts;
    char level_count;
    char current;
    char newlvl;
    char edgelvl;
    char pinlvl;
    char renderlvl;
    char use_col;
    char flag;
    CustomData vdata;
    CustomData fdata;
    short *edge_flags;
    char *edge_creases;
};

class PartialVisibility
{
public:
    int *vert_map;
    int *edge_map;
    MFace *old_faces;
    MEdge *old_edges;
    int totface;
    int totedge;
    int totvert;
    int pad1;
};

class ModifierData
{
public:
    ModifierData *next;
    ModifierData *prev;
    int type;
    int mode;
    char name[32];
    char *error;
};

class SubsurfModifierData
{
public:
    ModifierData modifier;
    short subdivType;
    short levels;
    short renderLevels;
    short flags;
    void *emCache;
    void *mCache;
};

class LatticeModifierData
{
public:
    ModifierData modifier;
    Object *object;
    char name[32];
};

class CurveModifierData
{
public:
    ModifierData modifier;
    Object *object;
    char name[32];
    short defaxis;
    char pad1;
};

class BuildModifierData
{
public:
    ModifierData modifier;
    float start;
    float length;
    int randomize;
    int seed;
};

class MaskModifierData
{
public:
    ModifierData modifier;
    Object *ob_arm;
    char vgroup[32];
    int mode;
    int flag;
};

class ArrayModifierData
{
public:
    ModifierData modifier;
    Object *start_cap;
    Object *end_cap;
    Object *curve_ob;
    Object *offset_ob;
    float offset[3];
    float scale[3];
    float length;
    float merge_dist;
    int fit_type;
    int offset_type;
    int flags;
    int count;
};

class MirrorModifierData
{
public:
    ModifierData modifier;
    short axis;
    short flag;
    float tolerance;
    Object *mirror_ob;
};

class EdgeSplitModifierData
{
public:
    ModifierData modifier;
    float split_angle;
    int flags;
};

class BevelModifierData
{
public:
    ModifierData modifier;
    float value;
    int res;
    int pad1;
    short flags;
    short val_flags;
    short lim_flags;
    short e_flags;
    float bevel_angle;
    char defgrp_name[32];
};

class BMeshModifierData
{
public:
    ModifierData modifier;
    float pad1;
    int type;
};

class DisplaceModifierData
{
public:
    ModifierData modifier;
    Tex *texture;
    float strength;
    int direction;
    char defgrp_name[32];
    float midlevel;
    int texmapping;
    Object *map_object;
    char uvlayer_name[32];
    int uvlayer_tmp;
    int pad1;
};

class UVProjectModifierData
{
public:
    ModifierData modifier;
    Object *projectors[10];
    Image *image;
    int flags;
    int num_projectors;
    float aspectx;
    float aspecty;
    char uvlayer_name[32];
    int uvlayer_tmp;
    int pad1;
};

class DecimateModifierData
{
public:
    ModifierData modifier;
    float percent;
    int faceCount;
};

class SmoothModifierData
{
public:
    ModifierData modifier;
    float fac;
    char defgrp_name[32];
    short flag;
    short repeat;
};

class CastModifierData
{
public:
    ModifierData modifier;
    Object *object;
    float fac;
    float radius;
    float size;
    char defgrp_name[32];
    short flag;
    short type;
};

class WaveModifierData
{
public:
    ModifierData modifier;
    Object *objectcenter;
    char defgrp_name[32];
    Tex *texture;
    Object *map_object;
    short flag;
    short pad1;
    float startx;
    float starty;
    float height;
    float width;
    float narrow;
    float speed;
    float damp;
    float falloff;
    int texmapping;
    int uvlayer_tmp;
    char uvlayer_name[32];
    float timeoffs;
    float lifetime;
    float pad2;
};

class ArmatureModifierData
{
public:
    ModifierData modifier;
    short deformflag;
    short multi;
    int pad1;
    Object *object;
    float *prevCos;
    char defgrp_name[32];
};

class HookModifierData
{
public:
    ModifierData modifier;
    Object *object;
    float parentinv[4][4];
    float cent[3];
    float falloff;
    int *indexar;
    int totindex;
    float force;
    char name[32];
};

class SoftbodyModifierData
{
public:
    ModifierData modifier;
};

class ClothModifierData
{
public:
    ModifierData modifier;
    void *clothObject;
    ClothSimSettings *sim_parms;
    ClothCollSettings *coll_parms;
    PointCache *point_cache;
};

class CollisionModifierData
{
public:
    ModifierData modifier;
    MVert *x;
    MVert *xnew;
    MVert *xold;
    MVert *current_xnew;
    MVert *current_x;
    MVert *current_v;
    MFace *mfaces;
    int numverts;
    int numfaces;
    short absorption;
    short pad1;
    float time;
    void *bvhtree;
};

class SurfaceModifierData
{
public:
    ModifierData modifier;
    void *dm;
    void *bvhtree;
};

class BooleanModifierData
{
public:
    ModifierData modifier;
    Object *object;
    int operation;
    int pad1;
};

class MDefInfluence
{
public:
    int vertex;
    float weight;
};

class MDefCell
{
public:
    int offset;
    int totinfluence;
};

class MeshDeformModifierData
{
public:
    ModifierData modifier;
    Object *object;
    char defgrp_name[32];
    short gridsize;
    short needbind;
    short flag;
    short pad1;
    float *bindweights;
    float *bindcos;
    int totvert;
    int totcagevert;
    MDefCell *dyngrid;
    MDefInfluence *dyninfluences;
    int *dynverts;
    int *pad2;
    int dyngridsize;
    int totinfluence;
    float dyncellmin[3];
    float dyncellwidth;
    float bindmat[4][4];
};

class ParticleSystemModifierData
{
public:
    ModifierData modifier;
    ParticleSystem *psys;
    void *dm;
    int totdmvert;
    int totdmedge;
    int totdmface;
    short flag;
    short rt;
};

class ParticleInstanceModifierData
{
public:
    ModifierData modifier;
    Object *ob;
    short psys;
    short flag;
    short rt[2];
};

class ExplodeModifierData
{
public:
    ModifierData modifier;
    int *facepa;
    short flag;
    short vgroup;
    float protect;
};

class FluidsimModifierData
{
public:
    ModifierData modifier;
    FluidsimSettings *fss;
    PointCache *point_cache;
};

class ShrinkwrapModifierData
{
public:
    ModifierData modifier;
    Object *target;
    Object *auxTarget;
    char vgroup_name[32];
    float keepDist;
    short shrinkType;
    short shrinkOpts;
    char projAxis;
    char subsurfLevels;
    char pad1;
};

class SimpleDeformModifierData
{
public:
    ModifierData modifier;
    Object *origin;
    char vgroup_name[32];
    float factor;
    float limit[2];
    char mode;
    char axis;
    char originOpts;
    char pad1;
};

class Lattice
{
public:
    ID id;
    short pntsu;
    short pntsv;
    short pntsw;
    short flag;
    short opntsu;
    short opntsv;
    short opntsw;
    short pad1;
    char typeu;
    char typev;
    char typew;
    char type;
    int pad2;
    float fu;
    float fv;
    float fw;
    float du;
    float dv;
    float dw;
    BPoint *def;
    Ipo *ipo;
    Key *key;
    MDeformVert *dvert;
};

class bDeformGroup
{
public:
    bDeformGroup *next;
    bDeformGroup *prev;
    char name[32];
};

class BoundBox
{
public:
    float vec[8][3];
    int flag;
    int pad1;
};

class Object
{
public:
    ID id;
    short type;
    short partype;
    int par1;
    int par2;
    int par3;
    char parsubstr[32];
    Object *parent;
    Object *track;
    Object *proxy;
    Object *proxy_group;
    Object *proxy_from;
    Ipo *ipo;
    void *path;
    BoundBox *bb;
    bAction *action;
    bAction *poselib;
    bPose *pose;
    void *data;
    ListBase constraintChannels;
    ListBase effect;
    ListBase disp;
    ListBase defbase;
    ListBase modifiers;
    Material **mat;
    float loc[3];
    float dloc[3];
    float orig[3];
    float size[3];
    float dsize[3];
    float rot[3];
    float drot[3];
    float obmat[4][4];
    float parentinv[4][4];
    float constinv[4][4];
    float imat[4][4];
    int lay;
    short flag;
    short colbits;
    short transflag;
    short ipoflag;
    short trackflag;
    short upflag;
    short nlaflag;
    short protectflag;
    short ipowin;
    short scaflag;
    short scavisflag;
    short boundtype;
    int dupon;
    int dupoff;
    int dupsta;
    int dupend;
    float sf;
    float ctime;
    float mass;
    float damping;
    float inertia;
    float formfactor;
    float rdamping;
    float sizefac;
    float margin;
    float max_vel;
    float min_vel;
    float m_contactProcessingThreshold;
    char dt;
    char dtx;
    char totcol;
    char actcol;
    char empty_drawtype;
    char pad1;
    float empty_drawsize;
    float dupfacesca;
    ScriptLink scriptlink;
    ListBase prop;
    ListBase sensors;
    ListBase controllers;
    ListBase actuators;
    float bbsize[3];
    short index;
    short actdef;
    float col[4];
    int gameflag;
    int gameflag2;
    BulletSoftBody *bsoft;
    short softflag;
    short recalc;
    float anisotropicFriction[3];
    ListBase constraints;
    ListBase nlastrips;
    ListBase hooks;
    ListBase particlesystem;
    PartDeflect *pd;
    SoftBody *soft;
    Group *dup_group;
    short fluidsimFlag;
    short restrictflag;
    short shapenr;
    short shapeflag;
    float smoothresh;
    short recalco;
    short body_type;
    FluidsimSettings *fluidsimSettings;
    void *derivedDeform;
    void *derivedFinal;
    int lastDataMask;
    int state;
    int init_state;
    int pad2;
    void *vnode;
    ListBase gpulamp;
};

class ObHook
{
public:
    ObHook *next;
    ObHook *prev;
    Object *parent;
    float parentinv[4][4];
    float mat[4][4];
    float cent[3];
    float falloff;
    char name[32];
    int *indexar;
    int totindex;
    int curindex;
    short type;
    short active;
    float force;
};

class PartDeflect
{
public:
    short deflect;
    short forcefield;
    short flag;
    short falloff;
    float pdef_damp;
    float pdef_rdamp;
    float pdef_perm;
    float pdef_frict;
    float pdef_rfrict;
    float f_strength;
    float f_power;
    float f_dist;
    float f_damp;
    float maxdist;
    float mindist;
    float maxrad;
    float minrad;
    float f_power_r;
    float pdef_sbdamp;
    float pdef_sbift;
    float pdef_sboft;
    float clump_fac;
    float clump_pow;
    float kink_freq;
    float kink_shape;
    float kink_amp;
    float free_end;
    float tex_nabla;
    short tex_mode;
    short kink;
    short kink_axis;
    short rt2;
    Tex *tex;
    void *rng;
    float f_noise;
    int seed;
};

class PointCache
{
public:
    int flag;
    int simframe;
    int startframe;
    int endframe;
    int editframe;
};

class SBVertex
{
public:
    float vec[4];
};

class BulletSoftBody
{
public:
    int flag;
    float linStiff;
    float angStiff;
    float volume;
    int viterations;
    int piterations;
    int diterations;
    int citerations;
    float kSRHR_CL;
    float kSKHR_CL;
    float kSSHR_CL;
    float kSR_SPLT_CL;
    float kSK_SPLT_CL;
    float kSS_SPLT_CL;
    float kVCF;
    float kDP;
    float kDG;
    float kLF;
    float kPR;
    float kVC;
    float kDF;
    float kMT;
    float kCHR;
    float kKHR;
    float kSHR;
    float kAHR;
    int collisionflags;
    int numclusteriterations;
    float welding;
    float margin;
};

class SoftBody
{
public:
    ParticleSystem *particles;
    int totpoint;
    int totspring;
    void *bpoint;
    void *bspring;
    char pad1;
    char msg_lock;
    short msg_value;
    float nodemass;
    char namedVG_Mass[32];
    float grav;
    float mediafrict;
    float rklimit;
    float physics_speed;
    float goalspring;
    float goalfrict;
    float mingoal;
    float maxgoal;
    float defgoal;
    short vertgroup;
    char namedVG_Softgoal[32];
    short fuzzyness;
    float inspring;
    float infrict;
    char namedVG_Spring_K[32];
    int sfra;
    int efra;
    int interval;
    short local;
    short solverflags;
    SBVertex **keys;
    int totpointkey;
    int totkey;
    float secondspring;
    float colball;
    float balldamp;
    float ballstiff;
    short sbc_mode;
    short aeroedge;
    short minloops;
    short maxloops;
    short choke;
    short solver_ID;
    short plastic;
    short springpreload;
    void *scratch;
    float shearstiff;
    float inpush;
    PointCache *pointcache;
};

class FluidsimSettings
{
public:
    short type;
    short show_advancedoptions;
    short resolutionxyz;
    short previewresxyz;
    float realsize;
    short guiDisplayMode;
    short renderDisplayMode;
    float viscosityValue;
    short viscosityMode;
    short viscosityExponent;
    float gravx;
    float gravy;
    float gravz;
    float animStart;
    float animEnd;
    float gstar;
    int maxRefine;
    float iniVelx;
    float iniVely;
    float iniVelz;
    Mesh *orgMesh;
    Mesh *meshSurface;
    Mesh *meshBB;
    char surfdataPath[240];
    float bbStart[3];
    float bbSize[3];
    Ipo *ipo;
    short typeFlags;
    char domainNovecgen;
    char volumeInitType;
    float partSlipValue;
    int generateTracers;
    float generateParticles;
    float surfaceSmoothing;
    int surfaceSubdivs;
    int flag;
    float particleInfSize;
    float particleInfAlpha;
    float farFieldSize;
    MVert *meshSurfNormals;
    float cpsTimeStart;
    float cpsTimeEnd;
    float cpsQuality;
    float attractforceStrength;
    float attractforceRadius;
    float velocityforceStrength;
    float velocityforceRadius;
    int lastgoodframe;
};

class World
{
public:
    ID id;
    short colormodel;
    short totex;
    short texact;
    short mistype;
    float horr;
    float horg;
    float horb;
    float hork;
    float zenr;
    float zeng;
    float zenb;
    float zenk;
    float ambr;
    float ambg;
    float ambb;
    float ambk;
    int fastcol;
    float exposure;
    float exp;
    float range;
    float linfac;
    float logfac;
    float gravity;
    float activityBoxRadius;
    short skytype;
    short mode;
    short occlusionRes;
    short physicsEngine;
    short ticrate;
    short maxlogicstep;
    short physubstep;
    short maxphystep;
    float misi;
    float miststa;
    float mistdist;
    float misthi;
    float starr;
    float starg;
    float starb;
    float stark;
    float starsize;
    float starmindist;
    float stardist;
    float starcolnoise;
    short dofsta;
    short dofend;
    short dofmin;
    short dofmax;
    float aodist;
    float aodistfac;
    float aoenergy;
    float aobias;
    short aomode;
    short aosamp;
    short aomix;
    short aocolor;
    float ao_adapt_thresh;
    float ao_adapt_speed_fac;
    float ao_approx_error;
    float ao_approx_correction;
    short ao_samp_method;
    short ao_gather_method;
    short ao_approx_passes;
    short pad1;
    float *aosphere;
    float *aotables;
    Ipo *ipo;
    MTex *mtex[18];
    PreviewImage *preview;
    ScriptLink scriptlink;
};

class Radio
{
public:
    short hemires;
    short maxiter;
    short drawtype;
    short flag;
    short subshootp;
    short subshoote;
    short nodelim;
    short maxsublamp;
    short pama;
    short pami;
    short elma;
    short elmi;
    int maxnode;
    float convergence;
    float radfac;
    float gamma;
};

class Base
{
public:
    Base *next;
    Base *prev;
    int lay;
    int selcol;
    int flag;
    short sx;
    short sy;
    Object *object;
};

class AviCodecData
{
public:
    void *lpFormat;
    void *lpParms;
    int cbFormat;
    int cbParms;
    int fccType;
    int fccHandler;
    int dwKeyFrameEvery;
    int dwQuality;
    int dwBytesPerSecond;
    int dwFlags;
    int dwInterleaveEvery;
    int pad1;
    char avicodecname[128];
};

class QuicktimeCodecData
{
public:
    void *cdParms;
    void *pad1;
    int cdSize;
    int pad2;
    char qtcodecname[128];
};

class FFMpegCodecData
{
public:
    int type;
    int codec;
    int audio_codec;
    int video_bitrate;
    int audio_bitrate;
    int gop_size;
    int flags;
    int rc_min_rate;
    int rc_max_rate;
    int rc_buffer_size;
    int mux_packet_size;
    int mux_rate;
    IDProperty *properties;
};

class AudioData
{
public:
    int mixrate;
    float main;
    short flag;
    short pad1;
};

class SceneRenderLayer
{
public:
    SceneRenderLayer *next;
    SceneRenderLayer *prev;
    char name[32];
    Material *mat_override;
    Group *light_override;
    int lay;
    int lay_zmask;
    int layflag;
    int pad1;
    int passflag;
    int pass_xor;
};

class RenderData
{
public:
    AviCodecData *avicodecdata;
    QuicktimeCodecData *qtcodecdata;
    FFMpegCodecData ffcodecdata;
    int cfra;
    int sfra;
    int efra;
    int psfra;
    int pefra;
    int images;
    int framapto;
    short flag;
    short threads;
    float ctime;
    float framelen;
    float blurfac;
    float edgeR;
    float edgeG;
    float edgeB;
    short fullscreen;
    short xplay;
    short yplay;
    short freqplay;
    short depth;
    short attrib;
    short rt1;
    short rt2;
    short stereomode;
    short dimensionspreset;
    short filtertype;
    short size;
    short maximsize;
    short xsch;
    short ysch;
    short xparts;
    short yparts;
    short winpos;
    short planes;
    short imtype;
    short subimtype;
    short bufflag;
    short quality;
    short rpad;
    short rpad1;
    short rpad2;
    int scemode;
    int mode;
    short renderer;
    short ocres;
    short alphamode;
    short osa;
    short frs_sec;
    short edgeint;
    rctf safety;
    rctf border;
    rcti disprect;
    ListBase layers;
    short actlay;
    short pad1;
    float xasp;
    float yasp;
    float frs_sec_base;
    float gauss;
    float postmul;
    float postgamma;
    float posthue;
    float postsat;
    float dither_intensity;
    short bake_osa;
    short bake_filter;
    short bake_mode;
    short bake_flag;
    short bake_normal_space;
    short bake_quad_split;
    float bake_maxdist;
    float bake_biasdist;
    float bake_pad;
    short GIquality;
    short GIcache;
    short GImethod;
    short GIphotons;
    short GIdirect;
    short YF_AA;
    short YFexportxml;
    short YF_nobump;
    short YF_clamprgb;
    short yfpad1;
    int GIdepth;
    int GIcausdepth;
    int GIpixelspersample;
    int GIphotoncount;
    int GImixphotons;
    float GIphotonradius;
    int YF_raydepth;
    int YF_AApasses;
    int YF_AAsamples;
    int yfpad2;
    float GIshadowquality;
    float GIrefinement;
    float GIpower;
    float GIindirpower;
    float YF_gamma;
    float YF_exposure;
    float YF_raybias;
    float YF_AApixelsize;
    float YF_AAthreshold;
    char backbuf[160];
    char pic[160];
    int stamp;
    short stamp_font_id;
    short pad2;
    char stamp_udata[160];
    float fg_stamp[4];
    float bg_stamp[4];
    int simplify_subsurf;
    int simplify_shadowsamples;
    float simplify_particles;
    float simplify_aosss;
    short cineonwhite;
    short cineonblack;
    float cineongamma;
    short jp2_preset;
    short jp2_depth;
    int rpad3;
    short domeres;
    short domemode;
    short domeangle;
    short dometilt;
    float domeresbuf;
    float pad3;
    Text *dometext;
};

class RenderProfile
{
public:
    RenderProfile *next;
    RenderProfile *prev;
    char name[32];
    short particle_perc;
    short subsurf_max;
    short shadbufsample_max;
    short pad1;
    float ao_error;
    float pad2;
};

class GameFraming
{
public:
    float col[3];
    char type;
    char pad1;
    char pad2;
    char pad3;
};

class TimeMarker
{
public:
    TimeMarker *next;
    TimeMarker *prev;
    int frame;
    char name[64];
    int flag;
};

class ImagePaintSettings
{
public:
    Brush *brush;
    short flag;
    short tool;
    short seam_bleed;
    short normal_angle;
};

class ParticleBrushData
{
public:
    short size;
    short strength;
    short step;
    short invert;
};

class ParticleEditSettings
{
public:
    short flag;
    short totrekey;
    short totaddkey;
    short brushtype;
    ParticleBrushData brush[7];
    float emitterdist;
    int draw_timed;
};

class TransformOrientation
{
public:
    TransformOrientation *next;
    TransformOrientation *prev;
    char name[36];
    float mat[3][3];
};

class ToolSettings
{
public:
    short cornertype;
    short editbutflag;
    float jointrilimit;
    float degr;
    short step;
    short turn;
    float extr_offs;
    float doublimit;
    short segments;
    short rings;
    short vertices;
    short unwrapper;
    float uvcalc_radius;
    float uvcalc_cubesize;
    float uvcalc_margin;
    float pad1;
    short uvcalc_mapdir;
    short uvcalc_mapalign;
    short uvcalc_flag;
    short autoik_chainlen;
    ImagePaintSettings imapaint;
    ParticleEditSettings particle;
    float select_thresh;
    float clean_thresh;
    char retopo_mode;
    char retopo_paint_tool;
    char line_div;
    char ellipse_div;
    char retopo_hotspot;
    char multires_subdiv_type;
    short skgen_resolution;
    float skgen_threshold_internal;
    float skgen_threshold_external;
    float skgen_length_ratio;
    float skgen_length_limit;
    float skgen_angle_limit;
    float skgen_correlation_limit;
    float skgen_symmetry_limit;
    float skgen_retarget_angle_weight;
    float skgen_retarget_length_weight;
    float skgen_retarget_distance_weight;
    short skgen_options;
    char skgen_postpro;
    char skgen_postpro_passes;
    char skgen_subdivisions[3];
    char skgen_multi_level;
    Object *skgen_template;
    char bone_sketching;
    char bone_sketching_convert;
    char skgen_subdivision_number;
    char skgen_retarget_options;
    char skgen_retarget_roll;
    char skgen_side_string[8];
    char skgen_num_string[8];
    char edge_mode;
    char pad2;
};

class BrushData
{
public:
    short size;
    char strength;
    char dir;
    char view;
    char flag;
    char pad1;
};

class SculptData
{
public:
    void *session;
    MTex *mtex[18];
    CurveMapping *cumap;
    BrushData drawbrush;
    BrushData smoothbrush;
    BrushData pinchbrush;
    BrushData inflatebrush;
    BrushData grabbrush;
    BrushData layerbrush;
    BrushData flattenbrush;
    float pivot[3];
    short brush_type;
    short texact;
    short texnr;
    short spacing;
    char texrept;
    char texfade;
    char texsep;
    char averaging;
    char flags;
    char tablet_size;
    char tablet_strength;
    char symm;
    char rake;
    char axislock;
    char pad1;
};

class Scene
{
public:
    ID id;
    Object *camera;
    World *world;
    Scene *set;
    Image *ima;
    ListBase base;
    Base *basact;
    float cursor[3];
    float twcent[3];
    float twmin[3];
    float twmax[3];
    int lay;
    float editbutsize;
    short selectmode;
    short proportional;
    short prop_mode;
    short automerge;
    short pad1;
    short pad2;
    short autokey_mode;
    short use_nodes;
    bNodeTree *nodetree;
    void *ed;
    Radio *radio;
    GameFraming framing;
    ToolSettings *toolsettings;
    RenderData r;
    AudioData audio;
    ScriptLink scriptlink;
    ListBase markers;
    ListBase transform_spaces;
    short jumpframe;
    short snap_mode;
    short snap_flag;
    short snap_target;
    void *theDag;
    short dagisvalid;
    short dagflags;
    short pad3;
    short recalc;
    SculptData sculptdata;
    int frame_step;
    int pad4;
};

class BGpic
{
public:
    Image *ima;
    ImageUser iuser;
    float xof;
    float yof;
    float size;
    float zoom;
    float blend;
    short xim;
    short yim;
};

class View3D
{
public:
    SpaceLink *next;
    SpaceLink *prev;
    int spacetype;
    float blockscale;
    ScrArea *area;
    short blockhandler[8];
    float viewmat[4][4];
    float viewinv[4][4];
    float persmat[4][4];
    float persinv[4][4];
    float winmat1[4][4];
    float viewmat1[4][4];
    float viewquat[4];
    float dist;
    float zfac;
    int lay_used;
    short persp;
    short view;
    Object *camera;
    Object *ob_centre;
    BGpic *bgpic;
    View3D *localvd;
    void *ri;
    void *retopo_view_data;
    void *depths;
    char ob_centre_bone[32];
    short drawtype;
    short localview;
    int lay;
    int layact;
    short scenelock;
    short around;
    short camzoom;
    char pivot_last;
    char pad1;
    float lens;
    float grid;
    float gridview;
    float pixsize;
    float near;
    float far;
    float camdx;
    float camdy;
    float ofs[3];
    float cursor[3];
    short gridlines;
    short viewbut;
    short gridflag;
    short modeselect;
    short menunr;
    short texnr;
    short twtype;
    short twmode;
    short twflag;
    short twdrawflag;
    float twmat[4][4];
    float clip[4][4];
    BoundBox *clipbb;
    ListBase afterdraw;
    short zbuf;
    short transp;
    short xray;
    short flag;
    short flag2;
    short gridsubdiv;
    short keyflags;
    char ndofmode;
    char ndoffilter;
    void *properties_storage;
    bGPdata *gpd;
    float lviewquat[4];
    short lpersp;
    short lview;
    short pad2;
    short pad3;
};

class View2D
{
public:
    rctf tot;
    rctf cur;
    rcti vert;
    rcti hor;
    rcti mask;
    float min[2];
    float max[2];
    float minzoom;
    float maxzoom;
    short scroll;
    short keeptot;
    short keepaspect;
    short keepzoom;
    short oldwinx;
    short oldwiny;
    int flag;
    float cursor[2];
    short around;
    char pad1;
};

class SpaceLink
{
public:
    SpaceLink *next;
    SpaceLink *prev;
    int spacetype;
    float blockscale;
    ScrArea *area;
    short blockhandler[8];
};

class SpaceInfo
{
public:
    SpaceLink *next;
    SpaceLink *prev;
    int spacetype;
    float blockscale;
    ScrArea *area;
    short blockhandler[8];
};

class SpaceIpo
{
public:
    SpaceLink *next;
    SpaceLink *prev;
    int spacetype;
    float blockscale;
    ScrArea *area;
    short blockhandler[8];
    int rowbut;
    int pad1;
    View2D v2d;
    void *editipo;
    ListBase ipokey;
    Ipo *ipo;
    ID *from;
    char actname[32];
    char constname[32];
    char bonename[32];
    short totipo;
    short pin;
    short butofs;
    short channel;
    short showkey;
    short blocktype;
    short menunr;
    short lock;
    int flag;
    float median[3];
    rctf tot;
};

class SpaceButs
{
public:
    SpaceLink *next;
    SpaceLink *prev;
    int spacetype;
    float blockscale;
    ScrArea *area;
    void *ri;
    short blockhandler[8];
    short cursens;
    short curact;
    short align;
    short tabo;
    View2D v2d;
    short mainb;
    short menunr;
    short pin;
    short mainbo;
    void *lockpoin;
    short texnr;
    char texfrom;
    char showgroup;
    short modeltype;
    short scriptblock;
    short scaflag;
    short re_align;
    short oldkeypress;
    char pad1;
    char flag;
    char texact;
    char tab[7];
};

class SpaceSeq
{
public:
    SpaceLink *next;
    SpaceLink *prev;
    int spacetype;
    float blockscale;
    ScrArea *area;
    short blockhandler[8];
    View2D v2d;
    float xof;
    float yof;
    short mainb;
    short render_size;
    short chanshown;
    short zebra;
    int flag;
    float zoom;
    bGPdata *gpd;
};

class SpaceFile
{
public:
    SpaceLink *next;
    SpaceLink *prev;
    int spacetype;
    float blockscale;
    ScrArea *area;
    short blockhandler[8];
    void *filelist;
    int totfile;
    char title[24];
    char dir[240];
    char file[80];
    short type;
    short ofs;
    short flag;
    short sort;
    short maxnamelen;
    short collums;
    short f_fp;
    short pad1;
    int pad2;
    char fp_str[8];
    void *libfiledata;
    short retval;
    short menu;
    short act;
    short ipotype;
    void (*returnfunc)();
    void (*returnfunc_event)();
    void (*returnfunc_args)();
    void *arg1;
    void *arg2;
    short *menup;
    char *pupmenu;
};

class SpaceOops
{
public:
    SpaceLink *next;
    SpaceLink *prev;
    int spacetype;
    float blockscale;
    ScrArea *area;
    short blockhandler[8];
    View2D v2d;
    ListBase oops;
    short pin;
    short visiflag;
    short flag;
    short rt;
    void *lockpoin;
    ListBase tree;
    TreeStore *treestore;
    char search_string[32];
    TreeStoreElem search_tse;
    int search_flags;
    int do_;
    short type;
    short outlinevis;
    short storeflag;
    short deps_flags;
};

class SpaceImage
{
public:
    SpaceLink *next;
    SpaceLink *prev;
    int spacetype;
    float blockscale;
    ScrArea *area;
    short blockhandler[8];
    View2D v2d;
    Image *image;
    ImageUser iuser;
    CurveMapping *cumap;
    short mode;
    short menunr;
    short imanr;
    short curtile;
    int flag;
    short selectmode;
    short imtypenr;
    short lock;
    short pin;
    float zoom;
    char dt_uv;
    char sticky;
    char dt_uvstretch;
    char pad1;
    float xof;
    float yof;
    float centx;
    float centy;
    bGPdata *gpd;
};

class SpaceNla
{
public:
    SpaceLink *next;
    SpaceLink *prev;
    int spacetype;
    float blockscale;
    ScrArea *area;
    short blockhandler[8];
    short menunr;
    short lock;
    short autosnap;
    short flag;
    View2D v2d;
};

class SpaceText
{
public:
    SpaceLink *next;
    SpaceLink *prev;
    int spacetype;
    float blockscale;
    ScrArea *area;
    short blockhandler[8];
    Text *text;
    int top;
    int viewlines;
    short flags;
    short menunr;
    int font_id;
    int lheight;
    int left;
    int showlinenrs;
    int tabnumber;
    int currtab_set;
    int showsyntax;
    int overwrite;
    float pix_per_line;
    rcti txtscroll;
    rcti txtbar;
    int wordwrap;
    int doplugins;
};

class Script
{
public:
    ID id;
    void *py_draw;
    void *py_event;
    void *py_button;
    void *py_browsercallback;
    void *py_globaldict;
    int flags;
    int lastspace;
    char scriptname[256];
    char scriptarg[256];
};

class SpaceScript
{
public:
    SpaceLink *next;
    SpaceLink *prev;
    int spacetype;
    float blockscale;
    ScrArea *area;
    Script *script;
    short flags;
    short menunr;
    int pad1;
    void *but_refs;
};

class SpaceTime
{
public:
    SpaceLink *next;
    SpaceLink *prev;
    int spacetype;
    float blockscale;
    ScrArea *area;
    View2D v2d;
    int flag;
    int redraws;
};

class SpaceNode
{
public:
    SpaceLink *next;
    SpaceLink *prev;
    int spacetype;
    float blockscale;
    ScrArea *area;
    short blockhandler[8];
    View2D v2d;
    ID *id;
    ID *from;
    short flag;
    short menunr;
    float aspect;
    void *curfont;
    float xof;
    float yof;
    bNodeTree *nodetree;
    bNodeTree *edittree;
    int treetype;
    short texfrom;
    short pad1;
    bGPdata *gpd;
};

class SpaceImaSel
{
public:
    SpaceLink *next;
    SpaceLink *prev;
    int spacetype;
    float blockscale;
    ScrArea *area;
    short blockhandler[8];
    View2D v2d;
    void *files;
    char title[24];
    char dir[240];
    char file[80];
    short type;
    short menu;
    short flag;
    short sort;
    void *curfont;
    int active_file;
    int numtilesx;
    int numtilesy;
    int selstate;
    rcti viewrect;
    rcti bookmarkrect;
    float scrollpos;
    float scrollheight;
    float scrollarea;
    float aspect;
    short retval;
    short ipotype;
    short filter;
    short active_bookmark;
    short pad1;
    short pad2;
    short prv_w;
    short prv_h;
    void (*returnfunc)();
    void (*returnfunc_event)();
    void (*returnfunc_args)();
    void *arg1;
    void *arg2;
    short *menup;
    char *pupmenu;
    void *img;
};

class ThemeUI
{
public:
    char outline[4];
    char neutral[4];
    char action[4];
    char setting[4];
    char setting1[4];
    char setting2[4];
    char num[4];
    char textfield[4];
    char textfield_hi[4];
    char popup[4];
    char text[4];
    char text_hi[4];
    char menu_back[4];
    char menu_item[4];
    char menu_hilite[4];
    char menu_text[4];
    char menu_text_hi[4];
    char but_drawtype;
    char pad1;
    char iconfile[80];
};

class ThemeSpace
{
public:
    char back[4];
    char text[4];
    char text_hi[4];
    char header[4];
    char panel[4];
    char shade1[4];
    char shade2[4];
    char hilite[4];
    char grid[4];
    char wire[4];
    char select[4];
    char lamp[4];
    char active[4];
    char group[4];
    char group_active[4];
    char transform[4];
    char vertex[4];
    char vertex_select[4];
    char edge[4];
    char edge_select[4];
    char edge_seam[4];
    char edge_sharp[4];
    char edge_facesel[4];
    char face[4];
    char face_select[4];
    char face_dot[4];
    char normal[4];
    char bone_solid[4];
    char bone_pose[4];
    char strip[4];
    char strip_select[4];
    char cframe[4];
    char pad1;
    char vertex_size;
    char facedot_size;
    char bpad[2];
    char syntaxl[4];
    char syntaxn[4];
    char syntaxb[4];
    char syntaxv[4];
    char syntaxc[4];
    char movie[4];
    char image[4];
    char scene[4];
    char audio[4];
    char effect[4];
    char plugin[4];
    char transition[4];
    char meta[4];
    char editmesh_active[4];
    char handle_vertex[4];
    char handle_vertex_select[4];
    char handle_vertex_size;
    char hpad[7];
};

class ThemeWireColor
{
public:
    char solid[4];
    char select[4];
    char active[4];
    short flag;
    short pad1;
};

class bTheme
{
public:
    bTheme *next;
    bTheme *prev;
    char name[32];
    ThemeUI tui;
    ThemeSpace tbuts;
    ThemeSpace tv3d;
    ThemeSpace tfile;
    ThemeSpace tipo;
    ThemeSpace tinfo;
    ThemeSpace tsnd;
    ThemeSpace tact;
    ThemeSpace tnla;
    ThemeSpace tseq;
    ThemeSpace tima;
    ThemeSpace timasel;
    ThemeSpace text;
    ThemeSpace toops;
    ThemeSpace ttime;
    ThemeSpace tnode;
    ThemeWireColor tarm[20];
    char bpad[4];
    char bpad1[4];
};

class SolidLight
{
public:
    int flag;
    int pad1;
    float col[4];
    float spec[4];
    float vec[4];
};

class UserDef
{
public:
    int flag;
    int dupflag;
    int savetime;
    char tempdir[160];
    char fontdir[160];
    char renderdir[160];
    char textudir[160];
    char plugtexdir[160];
    char plugseqdir[160];
    char pythondir[160];
    char sounddir[160];
    char yfexportdir[160];
    short versions;
    short vrmlflag;
    int gameflags;
    int wheellinescroll;
    int uiflag;
    int language;
    short userpref;
    short viewzoom;
    short console_buffer;
    short console_out;
    int mixbufsize;
    int fontsize;
    short encoding;
    short transopts;
    short menuthreshold1;
    short menuthreshold2;
    char fontname[256];
    ListBase themes;
    short undosteps;
    short undomemory;
    short gp_manhattendist;
    short gp_euclideandist;
    short gp_eraser;
    short gp_settings;
    short tb_leftmouse;
    short tb_rightmouse;
    SolidLight light[3];
    short tw_hotspot;
    short tw_flag;
    short tw_handlesize;
    short tw_size;
    int textimeout;
    int texcollectrate;
    int memcachelimit;
    int prefetchframes;
    short frameserverport;
    short pad1;
    short obcenter_dia;
    short rvisize;
    short rvibright;
    short recent_files;
    short smooth_viewtx;
    short glreslimit;
    short ndof_pan;
    short ndof_rotate;
    short curssize;
    short pad2;
    char pad3;
    char versemaster[160];
    char verseuser[160];
    float glalphaclip;
    short autokey_mode;
    short autokey_flag;
    ColorBand coba_weight;
};

class bScreen
{
public:
    ID id;
    ListBase vertbase;
    ListBase edgebase;
    ListBase areabase;
    Scene *scene;
    short startx;
    short endx;
    short starty;
    short endy;
    short sizex;
    short sizey;
    short scenenr;
    short screennr;
    short full;
    short pad1;
    short mainwin;
    short winakt;
    short handler[8];
};

class ScrVert
{
public:
    ScrVert *next;
    ScrVert *prev;
    ScrVert *newv;
    vec2s vec;
    int flag;
};

class ScrEdge
{
public:
    ScrEdge *next;
    ScrEdge *prev;
    ScrVert *v1;
    ScrVert *v2;
    short border;
    short flag;
    int pad1;
};

class Panel
{
public:
    Panel *next;
    Panel *prev;
    char panelname[64];
    char tabname[64];
    char drawname[64];
    short ofsx;
    short ofsy;
    short sizex;
    short sizey;
    short flag;
    short active;
    short control;
    short snap;
    short old_ofsx;
    short old_ofsy;
    int sortcounter;
    Panel *paneltab;
};

class ScrArea
{
public:
    ScrArea *next;
    ScrArea *prev;
    ScrVert *v1;
    ScrVert *v2;
    ScrVert *v3;
    ScrVert *v4;
    bScreen *full;
    float winmat[4][4];
    rcti totrct;
    rcti headrct;
    rcti winrct;
    short headwin;
    short win;
    short headertype;
    char spacetype;
    char butspacetype;
    short winx;
    short winy;
    char head_swap;
    char head_equal;
    char win_swap;
    char win_equal;
    short headbutlen;
    short headbutofs;
    short cursor;
    short flag;
    ScriptLink scriptlink;
    ListBase spacedata;
    ListBase uiblocks;
    ListBase panels;
};

class FileGlobal
{
public:
    char subvstr[4];
    short subversion;
    short pad1;
    short minversion;
    short minsubversion;
    short displaymode;
    short winpos;
    void *curscreen;
    void *curscene;
    int fileflags;
    int globalf;
};

class StripElem
{
public:
    char name[80];
};

class TStripElem
{
public:
    void *ibuf;
    void *ibuf_comp;
    TStripElem *se1;
    TStripElem *se2;
    TStripElem *se3;
    short ok;
    short flag;
    int nr;
};

class StripCrop
{
public:
    int top;
    int bottom;
    int left;
    int right;
};

class StripTransform
{
public:
    int xofs;
    int yofs;
};

class StripColorBalance
{
public:
    float lift[3];
    float gamma[3];
    float gain[3];
    int flag;
    int mode;
    float exposure;
    float saturation;
    int pad1;
    void *gui;
};

class StripProxy
{
public:
    char dir[160];
    char file[80];
    void *anim;
    short size;
    short quality;
    int pad1;
};

class Strip
{
public:
    Strip *next;
    Strip *prev;
    int rt;
    int len;
    int us;
    int done;
    int startstill;
    int endstill;
    StripElem *stripdata;
    char dir[160];
    int orx;
    int ory;
    StripProxy *proxy;
    StripCrop *crop;
    StripTransform *transform;
    StripColorBalance *color_balance;
    TStripElem *tstripdata;
    TStripElem *tstripdata_startstill;
    TStripElem *tstripdata_endstill;
    void *ibuf_startstill;
    void *ibuf_endstill;
};

class PluginSeq
{
public:
    char name[256];
    void *handle;
    char *pname;
    int vars;
    int version;
    void *varstr;
    float *cfra;
    float data[32];
    void *instance_private_data;
    void **current_private_data;
    void (*doit)();
    void (*callback)();
};

class Sequence
{
public:
    Sequence *next;
    Sequence *prev;
    void *tmp;
    void *lib;
    char name[24];
    int flag;
    int type;
    int len;
    int start;
    int startofs;
    int endofs;
    int startstill;
    int endstill;
    int machine;
    int depth;
    int startdisp;
    int enddisp;
    float mul;
    float handsize;
    int sfra;
    int anim_preseek;
    Strip *strip;
    Ipo *ipo;
    Scene *scene;
    void *anim;
    float facf0;
    float facf1;
    PluginSeq *plugin;
    Sequence *seq1;
    Sequence *seq2;
    Sequence *seq3;
    ListBase seqbase;
    bSound *sound;
    void *hdaudio;
    float level;
    float pan;
    int scenenr;
    float strobe;
    void *effectdata;
    int anim_startofs;
    int anim_endofs;
    int blend_mode;
    float blend_opacity;
};

class MetaStack
{
public:
    MetaStack *next;
    MetaStack *prev;
    ListBase *oldbasep;
    Sequence *parseq;
};

class Editing
{
public:
    ListBase *seqbasep;
    ListBase seqbase;
    ListBase metastack;
    short flag;
    short pad1;
    int rt;
};

class WipeVars
{
public:
    float edgeWidth;
    float angle;
    short forward;
    short wipetype;
};

class GlowVars
{
public:
    float fMini;
    float fClamp;
    float fBoost;
    float dDist;
    int dQuality;
    int bNoComp;
};

class TransformVars
{
public:
    float ScalexIni;
    float ScaleyIni;
    float ScalexFin;
    float ScaleyFin;
    float xIni;
    float xFin;
    float yIni;
    float yFin;
    float rotIni;
    float rotFin;
    int percent;
    int interpolation;
};

class SolidColorVars
{
public:
    float col[3];
    float pad1;
};

class SpeedControlVars
{
public:
    float *frameMap;
    float globalSpeed;
    int flags;
    int length;
    int lastValidFrame;
    int blendFrames;
    int pad1;
};

class Effect
{
public:
    Effect *next;
    Effect *prev;
    short type;
    short flag;
    short buttype;
    short rt;
};

class BuildEff
{
public:
    BuildEff *next;
    BuildEff *prev;
    short type;
    short flag;
    short buttype;
    short rt;
    float len;
    float sfra;
};

class PartEff
{
public:
    PartEff *next;
    PartEff *prev;
    short type;
    short flag;
    short buttype;
    short stype;
    short vertgroup;
    short userjit;
    float sta;
    float end;
    float lifetime;
    int totpart;
    int totkey;
    int seed;
    float normfac;
    float obfac;
    float randfac;
    float texfac;
    float randlife;
    float force[3];
    float damp;
    float nabla;
    float vectsize;
    float maxlen;
    float pad1;
    float defvec[3];
    float mult[4];
    float life[4];
    short child[4];
    short mat[4];
    short texmap;
    short curmult;
    short staticstep;
    short omat;
    short timetex;
    short speedtex;
    short flag2;
    short flag2neg;
    short disp;
    short vertgroup_v;
    char vgroupname[32];
    char vgroupname_v[32];
    float imat[4][4];
    void *keys;
    Group *group;
};

class WaveEff
{
public:
    WaveEff *next;
    WaveEff *prev;
    short type;
    short flag;
    short buttype;
    short stype;
    float startx;
    float starty;
    float height;
    float width;
    float narrow;
    float speed;
    float minfac;
    float damp;
    float timeoffs;
    float lifetime;
};

class TreeStore
{
public:
    int totelem;
    int usedelem;
    TreeStoreElem *data;
};

class Oops
{
public:
    Oops *next;
    Oops *prev;
    short type;
    short flag;
    short dt;
    short hide;
    float x;
    float y;
    float dx;
    float dy;
    ID *id;
    ListBase link;
};

class bProperty
{
public:
    bProperty *next;
    bProperty *prev;
    char name[32];
    short type;
    short otype;
    int data;
    int old;
    short flag;
    short pad1;
    void *poin;
    void *oldpoin;
};

class bNearSensor
{
public:
    char name[32];
    float dist;
    float resetdist;
    int lastval;
    int pad1;
};

class bMouseSensor
{
public:
    short type;
    short flag;
    short pad1;
    short pad2;
};

class bTouchSensor
{
public:
    char name[32];
    Material *ma;
    float dist;
    float pad1;
};

class bKeyboardSensor
{
public:
    short key;
    short qual;
    short type;
    short qual2;
    char targetName[32];
    char toggleName[32];
};

class bPropertySensor
{
public:
    int type;
    int pad1;
    char name[32];
    char value[32];
    char maxvalue[32];
};

class bActuatorSensor
{
public:
    int type;
    int pad1;
    char name[32];
};

class bDelaySensor
{
public:
    short delay;
    short duration;
    short flag;
    short pad1;
};

class bCollisionSensor
{
public:
    char name[32];
    char materialName[32];
    short damptimer;
    short damp;
    short mode;
    short pad1;
};

class bRadarSensor
{
public:
    char name[32];
    float angle;
    float range;
    short flag;
    short axis;
};

class bRandomSensor
{
public:
    char name[32];
    int seed;
    int delay;
};

class bRaySensor
{
public:
    char name[32];
    float range;
    char propname[32];
    char matname[32];
    short mode;
    short pad1;
    int axisflag;
};

class bMessageSensor
{
public:
    Object *fromObject;
    char subject[32];
    char body[32];
};

class bSensor
{
public:
    bSensor *next;
    bSensor *prev;
    short type;
    short otype;
    short flag;
    short pulse;
    short freq;
    short totlinks;
    short pad1;
    short pad2;
    char name[32];
    void *data;
    bController **links;
    Object *ob;
    short invert;
    short level;
    short tap;
    short pad3;
};

class bJoystickSensor
{
public:
    char name[32];
    char type;
    char joyindex;
    short flag;
    short axis;
    short axis_single;
    int axisf;
    int button;
    int hat;
    int hatf;
    int precision;
};

class bExpressionCont
{
public:
    char str[128];
};

class bPythonCont
{
public:
    Text *text;
    char module[64];
    int mode;
    int flag;
};

class bController
{
public:
    bController *next;
    bController *prev;
    bController *mynew;
    short type;
    short flag;
    short inputs;
    short totlinks;
    short otype;
    short totslinks;
    short pad1;
    short pad2;
    char name[32];
    void *data;
    bActuator **links;
    bSensor **slinks;
    short val;
    short valo;
    int state_mask;
};

class bAddObjectActuator
{
public:
    int time;
    int pad1;
    Object *ob;
};

class bActionActuator
{
public:
    bAction *act;
    short type;
    short flag;
    int sta;
    int end;
    char name[32];
    char frameProp[32];
    short blendin;
    short priority;
    short end_reset;
    short strideaxis;
    float stridelength;
};

class bSoundActuator
{
public:
    short flag;
    short sndnr;
    int sta;
    int end;
    short pad1;
    bSound *sound;
    short type;
    short makecopy;
    short copymade;
    short pad2;
};

class bCDActuator
{
public:
    short flag;
    short sndnr;
    int sta;
    int end;
    short type;
    short track;
    float volume;
};

class bEditObjectActuator
{
public:
    int time;
    short type;
    short flag;
    Object *ob;
    Mesh *me;
    char name[32];
    float linVelocity[3];
    float angVelocity[3];
    float mass;
    short localflag;
    short dyn_operation;
};

class bSceneActuator
{
public:
    short type;
    short flag;
    int pad1;
    Scene *scene;
    Object *camera;
};

class bPropertyActuator
{
public:
    int flag;
    int type;
    char name[32];
    char value[32];
    Object *ob;
};

class bObjectActuator
{
public:
    short flag;
    short type;
    short otype;
    short damping;
    float forceloc[3];
    float forcerot[3];
    float loc[3];
    float rot[3];
    float dloc[3];
    float drot[3];
    float linearvelocity[3];
    float angularvelocity[3];
    Object *reference;
};

class bIpoActuator
{
public:
    short flag;
    short type;
    int sta;
    int end;
    char name[32];
    char frameProp[32];
    short pad1;
    short cur;
    short butsta;
    short butend;
};

class bCameraActuator
{
public:
    Object *ob;
    float height;
    float min;
    float max;
    float fac;
    short flag;
    short axis;
    float visifac;
};

class bConstraintActuator
{
public:
    short type;
    short mode;
    short flag;
    short damp;
    short time;
    short rotdamp;
    int pad1;
    float minloc[3];
    float maxloc[3];
    float minrot[3];
    float maxrot[3];
    char matprop[32];
};

class bGroupActuator
{
public:
    short flag;
    short type;
    int sta;
    int end;
    char name[32];
    short pad1;
    short cur;
    short butsta;
    short butend;
    Group *group;
};

class bRandomActuator
{
public:
    int seed;
    int distribution;
    int int_arg_1;
    int int_arg_2;
    float float_arg_1;
    float float_arg_2;
    char propname[32];
};

class bMessageActuator
{
public:
    char toPropName[32];
    Object *toObject;
    char subject[32];
    short bodyType;
    short pad1;
    int pad2;
    char body[32];
};

class bGameActuator
{
public:
    short flag;
    short type;
    int sta;
    int end;
    char filename[64];
    char loadaniname[64];
};

class bVisibilityActuator
{
public:
    int flag;
};

class bTwoDFilterActuator
{
public:
    char pad1;
    short type;
    short flag;
    int int_arg;
    float float_arg;
    Text *text;
};

class bParentActuator
{
public:
    char pad1;
    short flag;
    int type;
    Object *ob;
};

class bStateActuator
{
public:
    int type;
    int mask;
};

class bActuator
{
public:
    bActuator *next;
    bActuator *prev;
    bActuator *mynew;
    short type;
    short flag;
    short otype;
    short go;
    char name[32];
    void *data;
    Object *ob;
};

class FreeCamera
{
public:
    float mass;
    float accelleration;
    float maxspeed;
    float maxrotspeed;
    float maxtiltspeed;
    int flag;
    float rotdamp;
    float tiltdamp;
    float speeddamp;
    float pad1;
};

class bSound
{
public:
    ID id;
    char name[160];
    void *sample;
    void *stream;
    PackedFile *packedfile;
    PackedFile *newpackedfile;
    void *snd_sound;
    Ipo *ipo;
    float volume;
    float panning;
    float attenuation;
    float pitch;
    float min_gain;
    float max_gain;
    float distance;
    int flags;
    int streamlen;
    char channels;
    char highprio;
    char pad1;
};

class bSoundListener
{
public:
    ID id;
    float gain;
    float dopplerfactor;
    float dopplervelocity;
    short numsoundsblender;
    short numsoundsgameengine;
};

class SpaceSound
{
public:
    SpaceLink *next;
    SpaceLink *prev;
    int spacetype;
    float blockscale;
    ScrArea *area;
    View2D v2d;
    bSound *sound;
    short mode;
    short sndnr;
    short xof;
    short yof;
    short flag;
    short lock;
    int pad1;
};

class GroupObject
{
public:
    GroupObject *next;
    GroupObject *prev;
    Object *ob;
    void *lampren;
    int recalc;
    int pad1;
};

class Group
{
public:
    ID id;
    ListBase gobject;
    int layer;
    float dupli_ofs[3];
};

class Bone
{
public:
    Bone *next;
    Bone *prev;
    Bone *parent;
    ListBase childbase;
    char name[32];
    float roll;
    float head[3];
    float tail[3];
    float bone_mat[3][3];
    int flag;
    float arm_head[3];
    float arm_tail[3];
    float arm_mat[4][4];
    float dist;
    float weight;
    float xwidth;
    float length;
    float zwidth;
    float ease1;
    float ease2;
    float rad_head;
    float rad_tail;
    float size[3];
    short layer;
    short segments;
};

class bArmature
{
public:
    ID id;
    ListBase bonebase;
    ListBase chainbase;
    int flag;
    int drawtype;
    short deformflag;
    short pathflag;
    short layer;
    short layer_protected;
    short ghostep;
    short ghostsize;
    short ghosttype;
    short pathsize;
    int ghostsf;
    int ghostef;
    int pathsf;
    int pathef;
    int pathbc;
    int pathac;
};

class bPoseChannel
{
public:
    bPoseChannel *next;
    bPoseChannel *prev;
    ListBase constraints;
    char name[32];
    short flag;
    short constflag;
    short ikflag;
    short selectflag;
    short protectflag;
    short agrp_index;
    int pathlen;
    int pathsf;
    int pathef;
    Bone *bone;
    bPoseChannel *parent;
    bPoseChannel *child;
    ListBase iktree;
    void *b_bone_mats;
    void *dual_quat;
    void *b_bone_dual_quats;
    float loc[3];
    float size[3];
    float quat[4];
    float chan_mat[4][4];
    float pose_mat[4][4];
    float constinv[4][4];
    float pose_head[3];
    float pose_tail[3];
    float limitmin[3];
    float limitmax[3];
    float stiffness[3];
    float ikstretch;
    float *path;
    Object *custom;
};

class bPose
{
public:
    ListBase chanbase;
    short flag;
    short proxy_layer;
    float ctime;
    float stride_offset[3];
    float cyclic_offset[3];
    ListBase agroups;
    int active_group;
    int pad1;
};

class bActionGroup
{
public:
    bActionGroup *next;
    bActionGroup *prev;
    ListBase channels;
    int flag;
    int customCol;
    char name[32];
    ThemeWireColor cs;
};

class bActionChannel
{
public:
    bActionChannel *next;
    bActionChannel *prev;
    bActionGroup *grp;
    Ipo *ipo;
    ListBase constraintChannels;
    int flag;
    char name[32];
    int reserved1;
};

class bAction
{
public:
    ID id;
    ListBase chanbase;
    ListBase groups;
    ListBase markers;
    int active_marker;
    int pad1;
};

class SpaceAction
{
public:
    SpaceLink *next;
    SpaceLink *prev;
    int spacetype;
    float blockscale;
    ScrArea *area;
    short blockhandler[8];
    View2D v2d;
    bAction *action;
    char mode;
    char autosnap;
    short flag;
    short actnr;
    short pin;
    short lock;
    short actwidth;
    float timeslide;
};

class bConstraintChannel
{
public:
    bConstraintChannel *next;
    bConstraintChannel *prev;
    Ipo *ipo;
    short flag;
    char name[30];
};

class bConstraint
{
public:
    bConstraint *next;
    bConstraint *prev;
    void *data;
    short type;
    short flag;
    char ownspace;
    char tarspace;
    char name[30];
    float enforce;
    float headtail;
    int pad1;
    Ipo *ipo;
};

class bConstraintTarget
{
public:
    bConstraintTarget *next;
    bConstraintTarget *prev;
    Object *tar;
    char subtarget[32];
    float matrix[4][4];
    short space;
    short flag;
    short type;
    short pad1;
};

class bPythonConstraint
{
public:
    Text *text;
    IDProperty *prop;
    int flag;
    int tarnum;
    ListBase targets;
    Object *tar;
    char subtarget[32];
};

class bKinematicConstraint
{
public:
    Object *tar;
    short iterations;
    short flag;
    short rootbone;
    short max_rootbone;
    char subtarget[32];
    Object *poletar;
    char polesubtarget[32];
    float poleangle;
    float weight;
    float orientweight;
    float grabtarget[3];
};

class bTrackToConstraint
{
public:
    Object *tar;
    int reserved1;
    int reserved2;
    int flags;
    int pad1;
    char subtarget[32];
};

class bRotateLikeConstraint
{
public:
    Object *tar;
    int flag;
    int reserved1;
    char subtarget[32];
};

class bLocateLikeConstraint
{
public:
    Object *tar;
    int flag;
    int reserved1;
    char subtarget[32];
};

class bMinMaxConstraint
{
public:
    Object *tar;
    int minmaxflag;
    float offset;
    int flag;
    short sticky;
    short stuck;
    short pad1;
    short pad2;
    float cache[3];
    char subtarget[32];
};

class bSizeLikeConstraint
{
public:
    Object *tar;
    int flag;
    int reserved1;
    char subtarget[32];
};

class bActionConstraint
{
public:
    Object *tar;
    short type;
    short local;
    int start;
    int end;
    float min;
    float max;
    int pad1;
    bAction *act;
    char subtarget[32];
};

class bLockTrackConstraint
{
public:
    Object *tar;
    int trackflag;
    int lockflag;
    char subtarget[32];
};

class bFollowPathConstraint
{
public:
    Object *tar;
    float offset;
    int followflag;
    int trackflag;
    int upflag;
};

class bStretchToConstraint
{
public:
    Object *tar;
    int volmode;
    int plane;
    float orglength;
    float bulge;
    char subtarget[32];
};

class bRigidBodyJointConstraint
{
public:
    Object *tar;
    Object *child;
    int type;
    float pivX;
    float pivY;
    float pivZ;
    float axX;
    float axY;
    float axZ;
    float minLimit[6];
    float maxLimit[6];
    float extraFz;
    short flag;
    short pad1;
    short pad2;
    short pad3;
};

class bClampToConstraint
{
public:
    Object *tar;
    int flag;
    int flag2;
};

class bChildOfConstraint
{
public:
    Object *tar;
    int flag;
    int pad1;
    float invmat[4][4];
    char subtarget[32];
};

class bTransformConstraint
{
public:
    Object *tar;
    char subtarget[32];
    short from;
    short to;
    char map[3];
    char expo;
    float from_min[3];
    float from_max[3];
    float to_min[3];
    float to_max[3];
};

class bLocLimitConstraint
{
public:
    float xmin;
    float xmax;
    float ymin;
    float ymax;
    float zmin;
    float zmax;
    short flag;
    short flag2;
};

class bRotLimitConstraint
{
public:
    float xmin;
    float xmax;
    float ymin;
    float ymax;
    float zmin;
    float zmax;
    short flag;
    short flag2;
};

class bSizeLimitConstraint
{
public:
    float xmin;
    float xmax;
    float ymin;
    float ymax;
    float zmin;
    float zmax;
    short flag;
    short flag2;
};

class bDistLimitConstraint
{
public:
    Object *tar;
    char subtarget[32];
    float dist;
    float soft;
    short flag;
    short mode;
    int pad1;
};

class bShrinkwrapConstraint
{
public:
    Object *target;
    float dist;
    short shrinkType;
    char projAxis;
    char pad1;
};

class bActionModifier
{
public:
    bActionModifier *next;
    bActionModifier *prev;
    short type;
    short flag;
    char channel[32];
    float noisesize;
    float turbul;
    short channels;
    short no_rot_axis;
    Object *ob;
};

class bActionStrip
{
public:
    bActionStrip *next;
    bActionStrip *prev;
    short flag;
    short mode;
    short stride_axis;
    short curmod;
    Ipo *ipo;
    bAction *act;
    Object *object;
    float start;
    float end;
    float actstart;
    float actend;
    float actoffs;
    float stridelen;
    float repeat;
    float scale;
    float blendin;
    float blendout;
    char stridechannel[32];
    char offs_bone[32];
    ListBase modifiers;
};

class bNodeStack
{
public:
    float vec[4];
    float min;
    float max;
    void *data;
    short hasinput;
    short hasoutput;
    short datatype;
    short sockettype;
};

class bNodeSocket
{
public:
    bNodeSocket *next;
    bNodeSocket *prev;
    bNodeSocket *new_sock;
    char name[32];
    bNodeStack ns;
    short type;
    short flag;
    short limit;
    short stack_index;
    short intern;
    short stack_index_ext;
    int pad1;
    float locx;
    float locy;
    int own_index;
    int to_index;
    bNodeSocket *tosock;
    bNodeLink *link;
};

class bNode
{
public:
    bNode *next;
    bNode *prev;
    bNode *new_node;
    char name[32];
    char username[32];
    short type;
    short flag;
    short done;
    short level;
    short lasty;
    short menunr;
    short stack_index;
    short nr;
    ListBase inputs;
    ListBase outputs;
    ID *id;
    void *storage;
    float locx;
    float locy;
    float width;
    float miniwidth;
    short custom1;
    short custom2;
    float custom3;
    float custom4;
    short need_exec;
    short exec;
    rctf totr;
    rctf butr;
    rctf prvr;
    void *preview;
    void *typeinfo;
};

class bNodeLink
{
public:
    bNodeLink *next;
    bNodeLink *prev;
    bNode *fromnode;
    bNode *tonode;
    bNodeSocket *fromsock;
    bNodeSocket *tosock;
};

class bNodeTree
{
public:
    ID id;
    ListBase nodes;
    ListBase links;
    bNodeStack *stack;
    ListBase *threadstack;
    int type;
    int init;
    int stacksize;
    int cur_index;
    ListBase alltypes;
    void *owntype;
    bNodeSocket *selin;
    bNodeSocket *selout;
    void (*timecursor)();
    void (*stats_draw)();
    int (*test_break)();
};

class NodeImageAnim
{
public:
    int frames;
    int sfra;
    int nr;
    char cyclic;
    char movie;
    short pad1;
};

class NodeBlurData
{
public:
    short sizex;
    short sizey;
    short samples;
    short maxspeed;
    short minspeed;
    short relative;
    float fac;
    float percentx;
    float percenty;
    short filtertype;
    char bokeh;
    char gamma;
    short curved;
    short pad1;
    int image_in_width;
    int image_in_height;
};

class NodeDBlurData
{
public:
    float center_x;
    float center_y;
    float distance;
    float angle;
    float spin;
    float zoom;
    short iter;
    char wrap;
    char pad1;
};

class NodeBilateralBlurData
{
public:
    float sigma_color;
    float sigma_space;
    short iter;
    short pad1;
};

class NodeHueSat
{
public:
    float hue;
    float sat;
    float val;
};

class NodeImageFile
{
public:
    char name[256];
    short imtype;
    short subimtype;
    short quality;
    short codec;
    int sfra;
    int efra;
};

class NodeChroma
{
public:
    float t1;
    float t2;
    float t3;
    float fsize;
    float fstrength;
    float falpha;
    float key[4];
};

class NodeTwoXYs
{
public:
    short x1;
    short x2;
    short y1;
    short y2;
};

class NodeTwoFloats
{
public:
    float x;
    float y;
};

class NodeGeometry
{
public:
    char uvname[32];
    char colname[32];
};

class NodeVertexCol
{
public:
    char name[32];
};

class NodeDefocus
{
public:
    char bktype;
    char rotation;
    char preview;
    char gamco;
    short samples;
    short no_zbuf;
    float fstop;
    float maxblur;
    float bthresh;
    float scale;
};

class NodeScriptDict
{
public:
    void *dict;
    void *node;
};

class NodeGlare
{
public:
    char quality;
    char type;
    char iter;
    char angle;
    char angle_ofs;
    char size;
    char pad1;
    float colmod;
    float mix;
    float threshold;
    float fade;
};

class NodeTonemap
{
public:
    float key;
    float offset;
    float gamma;
    float f;
    float m;
    float a;
    float c;
    int type;
};

class NodeLensDist
{
public:
    short jit;
    short proj;
    short fit;
    short pad1;
};

class TexNodeOutput
{
public:
    char name[32];
};

class CurveMapPoint
{
public:
    float x;
    float y;
    short flag;
    short shorty;
};

class CurveMap
{
public:
    short totpoint;
    short flag;
    float range;
    float mintable;
    float maxtable;
    float ext_in[2];
    float ext_out[2];
    CurveMapPoint *curve;
    CurveMapPoint *table;
    CurveMapPoint *premultable;
};

class CurveMapping
{
public:
    int flag;
    int cur;
    rctf curr;
    rctf clipr;
    CurveMap cm[4];
    float black[3];
    float white[3];
    float bwmul[3];
    float sample[3];
};

class BrushClone
{
public:
    Image *image;
    float offset[2];
    float alpha;
    float pad1;
};

class Brush
{
public:
    ID id;
    short flag;
    short blend;
    int size;
    float innerradius;
    float spacing;
    float rate;
    float rgb[3];
    float alpha;
    short texact;
    short pad1;
    MTex *mtex[18];
    BrushClone clone;
};

class CustomDataLayer
{
public:
    int type;
    int offset;
    int flag;
    int active;
    int active_rnd;
    int active_clone;
    int active_mask;
    char pad1;
    char name[32];
    void *data;
};

class HairKey
{
public:
    float co[3];
    float time;
    float weight;
    short editflag;
    short pad1;
};

class ParticleKey
{
public:
    float co[3];
    float vel[3];
    float rot[4];
    float ave[3];
    float time;
};

class ChildParticle
{
public:
    int num;
    int parent;
    int pa[4];
    float w[4];
    float fuv[4];
    float foffset;
    float rand[3];
};

class ParticleData
{
public:
    Object *stick_ob;
    ParticleKey state;
    ParticleKey prev_state;
    HairKey *hair;
    ParticleKey *keys;
    float i_rot[4];
    float r_rot[4];
    float r_ave[3];
    float r_ve[3];
    float fuv[4];
    float foffset;
    float time;
    float lifetime;
    float dietime;
    float bank;
    float size;
    float sizemul;
    int num;
    int num_dmcache;
    int pad1;
    int totkey;
    int bpi;
    short flag;
    short alive;
    short loop;
    short rt2;
};

class ParticleSettings
{
public:
    ID id;
    int flag;
    short type;
    short from;
    short distr;
    short phystype;
    short rotmode;
    short avemode;
    short reactevent;
    short draw;
    short draw_as;
    short draw_size;
    short childtype;
    short draw_step;
    short ren_step;
    short hair_step;
    short keys_step;
    short adapt_angle;
    short adapt_pix;
    short disp;
    short omat;
    short interpolation;
    short rotfrom;
    short integrator;
    short kink;
    short kink_axis;
    short nbetween;
    short boidneighbours;
    short bb_align;
    short bb_uv_split;
    short bb_anim;
    short bb_split_offset;
    float bb_tilt;
    float bb_rand_tilt;
    float bb_offset[2];
    short simplify_flag;
    short simplify_refsize;
    float simplify_rate;
    float simplify_transition;
    float simplify_viewport;
    float sta;
    float end;
    float lifetime;
    float randlife;
    float timetweak;
    float jitfac;
    float keyed_time;
    float eff_hair;
    float rt;
    int totpart;
    int userjit;
    int grid_res;
    float normfac;
    float obfac;
    float randfac;
    float partfac;
    float tanfac;
    float tanphase;
    float reactfac;
    float avefac;
    float phasefac;
    float randrotfac;
    float randphasefac;
    float mass;
    float size;
    float randsize;
    float reactshape;
    float acc[3];
    float dragfac;
    float brownfac;
    float dampfac;
    float length;
    float abslength;
    float randlength;
    int child_nbr;
    int ren_child_nbr;
    float parents;
    float childsize;
    float childrandsize;
    float childrad;
    float childflat;
    float childspread;
    float clumpfac;
    float clumppow;
    float kink_amp;
    float kink_freq;
    float kink_shape;
    float rough1;
    float rough1_size;
    float rough2;
    float rough2_size;
    float rough2_thres;
    float rough_end;
    float rough_end_shape;
    float branch_thres;
    float draw_line[2];
    float max_vel;
    float max_lat_acc;
    float max_tan_acc;
    float average_vel;
    float banking;
    float max_bank;
    float groundz;
    float boidfac[8];
    char boidrule[8];
    Group *dup_group;
    Group *eff_group;
    Object *dup_ob;
    Object *bb_ob;
    Ipo *ipo;
    PartDeflect *pd;
    PartDeflect *pd2;
};

class ParticleSystem
{
public:
    ParticleSystem *next;
    ParticleSystem *prev;
    ParticleSettings *part;
    ParticleData *particles;
    ChildParticle *child;
    void *edit;
    void **pathcache;
    void **childcache;
    ListBase pathcachebufs;
    ListBase childcachebufs;
    SoftBody *soft;
    Object *target_ob;
    Object *keyed_ob;
    Object *lattice;
    ListBase effectors;
    ListBase reactevents;
    float imat[4][4];
    float cfra;
    int seed;
    int flag;
    int totpart;
    int totchild;
    int totcached;
    int totchildcache;
    int rt;
    short recalc;
    short target_psys;
    short keyed_psys;
    short totkeyed;
    short softflag;
    short bakespace;
    char bb_uvname[3][32];
    short vgroup[12];
    short vg_neg;
    short rt3;
    void *renderdata;
    PointCache *pointcache;
};

class ClothSimSettings
{
public:
    void *cache;
    float mingoal;
    float Cdis;
    float Cvi;
    float gravity;
    float *pad1;
    float dt;
    float mass;
    float structural;
    float shear;
    float bending;
    float max_bend;
    float max_struct;
    float max_shear;
    float avg_spring_len;
    float timescale;
    float maxgoal;
    float eff_force_scale;
    float eff_wind_scale;
    float sim_time_old;
    float defgoal;
    float goalspring;
    float goalfrict;
    int stepsPerFrame;
    int flags;
    int preroll;
    int maxspringlen;
    short solver_type;
    short vgroup_bend;
    short vgroup_mass;
    short vgroup_struct;
    short presets;
    short pad2;
    int pad3;
};

class ClothCollSettings
{
public:
    void *collision_list;
    float epsilon;
    float self_friction;
    float friction;
    float selfepsilon;
    int flags;
    short self_loop_count;
    short loop_count;
};

class bGPDspoint
{
public:
    float x;
    float y;
    float z;
    float pressure;
};

class bGPDstroke
{
public:
    bGPDstroke *next;
    bGPDstroke *prev;
    bGPDspoint *points;
    int totpoints;
    short thickness;
    short flag;
};

class bGPDframe
{
public:
    bGPDframe *next;
    bGPDframe *prev;
    ListBase strokes;
    int framenum;
    int flag;
};

class bGPDlayer
{
public:
    bGPDlayer *next;
    bGPDlayer *prev;
    ListBase frames;
    bGPDframe *actframe;
    int flag;
    short thickness;
    short gstep;
    float color[4];
    char info[128];
};

class bGPdata
{
public:
    ListBase layers;
    int flag;
    short sbuffer_size;
    short sbuffer_sflag;
    void *sbuffer;
};

}
#endif//_Blender_h_
