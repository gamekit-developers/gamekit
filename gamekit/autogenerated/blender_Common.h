/* Copyright (C) 2006 Charlie C
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/
// Auto generated from makesdna dna.c
#ifndef __BLENDERCOMMON_H__
#define __BLENDERCOMMON_H__

// put an empty struct in the case
typedef struct bInvalidHandle {
	int unused;
}bInvalidHandle;

namespace Blender {
    class Link;
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
    class Ipo;
    class KeyBlock;
    class Key;
    class ScriptLink;
    class TextLine;
    class Text;
    class PackedFile;
    class Camera;
    class PreviewImage;
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
    class IpoDriver;
    class IpoCurve;
    class Mesh;
    class TFace;
    class MFace;
    class MEdge;
    class MDeformWeight;
    class MDeformVert;
    class MVert;
    class MCol;
    class MSticky;
    class MSelect;
    class MTFace;
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
    class ArrayModifierData;
    class MirrorModifierData;
    class EdgeSplitModifierData;
    class DisplaceModifierData;
    class UVProjectModifierData;
    class DecimateModifierData;
    class WaveModifierData;
    class ArmatureModifierData;
    class HookModifierData;
    class SoftbodyModifierData;
    class BooleanModifierData;
    class Lattice;
    class bDeformGroup;
    class BoundBox;
    class OcInfo;
    class LBuf;
    class Object;
    class ObHook;
    class PartDeflect;
    class SBVertex;
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
    class GameFraming;
    class TimeMarker;
    class ImagePaintSettings;
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
    class SpaceScript;
    class SpaceTime;
    class SpaceNode;
    class SpaceImaSel;
    class ThemeUI;
    class ThemeSpace;
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
    class bActionChannel;
    class bAction;
    class SpaceAction;
    class bConstraintChannel;
    class bConstraint;
    class bKinematicConstraint;
    class bTrackToConstraint;
    class bRotateLikeConstraint;
    class bLocateLikeConstraint;
    class bMinMaxConstraint;
    class bSizeLikeConstraint;
    class bActionConstraint;
    class bLockTrackConstraint;
    class bFollowPathConstraint;
    class bDistanceLimitConstraint;
    class bRotationConstraint;
    class bStretchToConstraint;
    class bLocLimitConstraint;
    class bRotLimitConstraint;
    class bSizeLimitConstraint;
    class bRigidBodyJointConstraint;
    class bActionModifier;
    class bActionStrip;
    class bNodeStack;
    class bNodeSocket;
    class bNode;
    class bNodeLink;
    class bNodeTree;
    class NodeImageAnim;
    class NodeBlurData;
    class NodeHueSat;
    class NodeImageFile;
    class NodeChroma;
    class NodeGeometry;
    class NodeVertexCol;
    class NodeDefocus;
    class CurveMapPoint;
    class CurveMap;
    class CurveMapping;
    class BrushClone;
    class Brush;
    class CustomDataLayer;
    class CustomData;
}
#endif//__BLENDERCOMMON_H__
