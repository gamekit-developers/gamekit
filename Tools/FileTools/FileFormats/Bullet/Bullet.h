#ifndef _Bullet_h_
#define _Bullet_h_
// Generated from a Bullet(277) file.

/** \addtogroup Bullet
*  @{
*/

struct btRigidBodyData;
struct PointerArray;
struct btPhysicsSystem;
struct ListBase;
struct btVector3FloatData;
struct btVector3DoubleData;
struct btMatrix3x3FloatData;
struct btMatrix3x3DoubleData;
struct btTransformFloatData;
struct btTransformDoubleData;
struct btBvhSubtreeInfoData;
struct btOptimizedBvhNodeFloatData;
struct btOptimizedBvhNodeDoubleData;
struct btQuantizedBvhNodeData;
struct btQuantizedBvhFloatData;
struct btQuantizedBvhDoubleData;
struct btCollisionShapeData;
struct btStaticPlaneShapeData;
struct btConvexInternalShapeData;
struct btPositionAndRadius;
struct btMultiSphereShapeData;
struct btIntIndexData;
struct btShortIntIndexData;
struct btShortIntIndexTripletData;
struct btMeshPartData;
struct btStridingMeshInterfaceData;
struct btTriangleMeshShapeData;
struct btCompoundShapeChildData;
struct btCompoundShapeData;
struct btCylinderShapeData;
struct btCapsuleShapeData;
struct btTriangleInfoData;
struct btTriangleInfoMapData;
struct btGImpactMeshShapeData;
struct btConvexHullShapeData;
struct btCollisionObjectDoubleData;
struct btCollisionObjectFloatData;
struct btRigidBodyFloatData;
struct btRigidBodyDoubleData;
struct btConstraintInfo1;
struct btTypedConstraintData;
struct btPoint2PointConstraintFloatData;
struct btPoint2PointConstraintDoubleData;
struct btHingeConstraintDoubleData;
struct btHingeConstraintFloatData;
struct btConeTwistConstraintData;
struct btGeneric6DofConstraintData;
struct btSliderConstraintData;



typedef struct PointerArray
{
	int            m_size;
	int            m_capacity;
	void*           m_data;
} PointerArray;

typedef struct ListBase
{
	void*           first;
	void*           last;
} ListBase;

typedef struct btVector3FloatData
{
	float           m_floats[4];
} btVector3FloatData;

#pragma pack(push, 4)
typedef struct btVector3DoubleData
{
	double           m_floats[4];
} btVector3DoubleData;
#pragma pack(pop)

typedef struct btBvhSubtreeInfoData
{
	int             m_rootNodeIndex;
	int             m_subtreeSize;
	short           m_quantizedAabbMin[3];
	short           m_quantizedAabbMax[3];
} btBvhSubtreeInfoData;

typedef struct btQuantizedBvhNodeData
{
	short           m_quantizedAabbMin[3];
	short           m_quantizedAabbMax[3];
	int             m_escapeIndexOrTriangleIndex;
} btQuantizedBvhNodeData;

typedef struct btCollisionShapeData
{
	char*           m_name;
	int            m_shapeType;
	char           m_padding[4];
} btCollisionShapeData;

typedef struct btIntIndexData
{
	int           m_value;
} btIntIndexData;

typedef struct btShortIntIndexData
{
	short           m_value;
	char            m_pad[2];
} btShortIntIndexData;

typedef struct btShortIntIndexTripletData
{
	short           m_values[3];
	char            m_pad[2];
} btShortIntIndexTripletData;

typedef struct btMeshPartData
{
	btVector3FloatData*                   m_vertices3f;
	btVector3DoubleData*                  m_vertices3d;
	btIntIndexData*                       m_indices32;
	btShortIntIndexTripletData*           m_3indices16;
	btShortIntIndexData*                  m_indices16;
	int                                  m_numTriangles;
	int                                  m_numVertices;
} btMeshPartData;

typedef struct btTriangleInfoData
{
	int             m_flags;
	float           m_edgeV0V1Angle;
	float           m_edgeV1V2Angle;
	float           m_edgeV2V0Angle;
} btTriangleInfoData;

typedef struct btTriangleInfoMapData
{
	int*                          m_hashTablePtr;
	int*                          m_nextPtr;
	btTriangleInfoData*           m_valueArrayPtr;
	int*                          m_keyArrayPtr;
	float                        m_convexEpsilon;
	float                        m_planarEpsilon;
	float                        m_equalVertexThreshold;
	float                        m_edgeDistanceThreshold;
	float                        m_zeroAreaThreshold;
	int                          m_nextSize;
	int                          m_hashTableSize;
	int                          m_numValues;
	int                          m_numKeys;
	char                         m_padding[4];
} btTriangleInfoMapData;

typedef struct btConstraintInfo1
{
	int           m_numConstraintRows;
	int           nub;
} btConstraintInfo1;

typedef struct btTypedConstraintData
{
	struct btRigidBodyData*    m_rbA;
	struct btRigidBodyData*    m_rbB;
	char*                      m_name;
	int                       m_objectType;
	int                       m_userConstraintType;
	int                       m_userConstraintId;
	int                       m_needsFeedback;
	float                     m_appliedImpulse;
	float                     m_dbgDrawSize;
	int                       m_disableCollisionsBetweenLinkedBodies;
	char                      m_pad4[4];
} btTypedConstraintData;

typedef struct btPhysicsSystem
{
	PointerArray           m_collisionShapes;
	PointerArray           m_collisionObjects;
	PointerArray           m_constraints;
} btPhysicsSystem;

typedef struct btMatrix3x3FloatData
{
	btVector3FloatData           m_el[3];
} btMatrix3x3FloatData;

typedef struct btMatrix3x3DoubleData
{
	btVector3DoubleData           m_el[3];
} btMatrix3x3DoubleData;

typedef struct btTransformFloatData
{
	btMatrix3x3FloatData           m_basis;
	btVector3FloatData             m_origin;
} btTransformFloatData;

typedef struct btTransformDoubleData
{
	btMatrix3x3DoubleData           m_basis;
	btVector3DoubleData             m_origin;
} btTransformDoubleData;

typedef struct btOptimizedBvhNodeFloatData
{
	btVector3FloatData           m_aabbMinOrg;
	btVector3FloatData           m_aabbMaxOrg;
	int                          m_escapeIndex;
	int                          m_subPart;
	int                          m_triangleIndex;
	char                         m_pad[4];
} btOptimizedBvhNodeFloatData;

typedef struct btOptimizedBvhNodeDoubleData
{
	btVector3DoubleData           m_aabbMinOrg;
	btVector3DoubleData           m_aabbMaxOrg;
	int                           m_escapeIndex;
	int                           m_subPart;
	int                           m_triangleIndex;
	char                          m_pad[4];
} btOptimizedBvhNodeDoubleData;

typedef struct btQuantizedBvhFloatData
{
	btVector3FloatData                    m_bvhAabbMin;
	btVector3FloatData                    m_bvhAabbMax;
	btVector3FloatData                    m_bvhQuantization;
	int                                   m_curNodeIndex;
	int                                   m_useQuantization;
	int                                   m_numContiguousLeafNodes;
	int                                   m_numQuantizedContiguousNodes;
	btOptimizedBvhNodeFloatData*           m_contiguousNodesPtr;
	btQuantizedBvhNodeData*                m_quantizedContiguousNodesPtr;
	btBvhSubtreeInfoData*                  m_subTreeInfoPtr;
	int                                   m_traversalMode;
	int                                   m_numSubtreeHeaders;
} btQuantizedBvhFloatData;

typedef struct btQuantizedBvhDoubleData
{
	btVector3DoubleData                    m_bvhAabbMin;
	btVector3DoubleData                    m_bvhAabbMax;
	btVector3DoubleData                    m_bvhQuantization;
	int                                    m_curNodeIndex;
	int                                    m_useQuantization;
	int                                    m_numContiguousLeafNodes;
	int                                    m_numQuantizedContiguousNodes;
	btOptimizedBvhNodeDoubleData*           m_contiguousNodesPtr;
	btQuantizedBvhNodeData*                 m_quantizedContiguousNodesPtr;
	int                                    m_traversalMode;
	int                                    m_numSubtreeHeaders;
	btBvhSubtreeInfoData*                   m_subTreeInfoPtr;
} btQuantizedBvhDoubleData;

typedef struct btStaticPlaneShapeData
{
	btCollisionShapeData           m_collisionShapeData;
	btVector3FloatData             m_localScaling;
	btVector3FloatData             m_planeNormal;
	float                          m_planeConstant;
	char                           m_pad[4];
} btStaticPlaneShapeData;

typedef struct btConvexInternalShapeData
{
	btCollisionShapeData           m_collisionShapeData;
	btVector3FloatData             m_localScaling;
	btVector3FloatData             m_implicitShapeDimensions;
	float                          m_collisionMargin;
	int                            m_padding;
} btConvexInternalShapeData;

typedef struct btPositionAndRadius
{
	btVector3FloatData           m_pos;
	float                        m_radius;
} btPositionAndRadius;

typedef struct btMultiSphereShapeData
{
	btConvexInternalShapeData           m_convexInternalShapeData;
	btPositionAndRadius*                 m_localPositionArrayPtr;
	int                                 m_localPositionArraySize;
	char                                m_padding[4];
} btMultiSphereShapeData;

typedef struct btStridingMeshInterfaceData
{
	btMeshPartData*               m_meshPartsPtr;
	btVector3FloatData           m_scaling;
	int                          m_numMeshParts;
	char                         m_padding[4];
} btStridingMeshInterfaceData;

typedef struct btTriangleMeshShapeData
{
	btCollisionShapeData                  m_collisionShapeData;
	btStridingMeshInterfaceData           m_meshInterface;
	btQuantizedBvhFloatData*               m_quantizedFloatBvh;
	btQuantizedBvhDoubleData*              m_quantizedDoubleBvh;
	btTriangleInfoMapData*                 m_triangleInfoMap;
	float                                 m_collisionMargin;
	char                                  m_pad3[4];
} btTriangleMeshShapeData;

typedef struct btCompoundShapeChildData
{
	btTransformFloatData           m_transform;
	btCollisionShapeData*           m_childShape;
	int                            m_childShapeType;
	float                          m_childMargin;
} btCompoundShapeChildData;

typedef struct btCompoundShapeData
{
	btCollisionShapeData               m_collisionShapeData;
	btCompoundShapeChildData*           m_childShapePtr;
	int                                m_numChildShapes;
	float                              m_collisionMargin;
} btCompoundShapeData;

typedef struct btCylinderShapeData
{
	btConvexInternalShapeData           m_convexInternalShapeData;
	int                                 m_upAxis;
	char                                m_padding[4];
} btCylinderShapeData;

typedef struct btCapsuleShapeData
{
	btConvexInternalShapeData           m_convexInternalShapeData;
	int                                 m_upAxis;
	char                                m_padding[4];
} btCapsuleShapeData;

typedef struct btGImpactMeshShapeData
{
	btCollisionShapeData                  m_collisionShapeData;
	btStridingMeshInterfaceData           m_meshInterface;
	btVector3FloatData                    m_localScaling;
	float                                 m_collisionMargin;
	int                                   m_gimpactSubType;
} btGImpactMeshShapeData;

typedef struct btConvexHullShapeData
{
	btConvexInternalShapeData           m_convexInternalShapeData;
	btVector3FloatData*                  m_unscaledPointsFloatPtr;
	btVector3DoubleData*                 m_unscaledPointsDoublePtr;
	int                                 m_numUnscaledPoints;
	char                                m_padding3[4];
} btConvexHullShapeData;

typedef struct btCollisionObjectDoubleData
{
	void*                            m_broadphaseHandle;
	void*                            m_collisionShape;
	btCollisionShapeData*            m_rootCollisionShape;
	char*                            m_name;
	btTransformDoubleData           m_worldTransform;
	btTransformDoubleData           m_interpolationWorldTransform;
	btVector3DoubleData             m_interpolationLinearVelocity;
	btVector3DoubleData             m_interpolationAngularVelocity;
	btVector3DoubleData             m_anisotropicFriction;
	double                          m_contactProcessingThreshold;
	double                          m_deactivationTime;
	double                          m_friction;
	double                          m_restitution;
	double                          m_hitFraction;
	double                          m_ccdSweptSphereRadius;
	double                          m_ccdMotionThreshold;
	int                             m_hasAnisotropicFriction;
	int                             m_collisionFlags;
	int                             m_islandTag1;
	int                             m_companionId;
	int                             m_activationState1;
	int                             m_internalType;
	int                             m_checkCollideWith;
	char                            m_padding[4];
} btCollisionObjectDoubleData;

typedef struct btCollisionObjectFloatData
{
	void*                           m_broadphaseHandle;
	void*                           m_collisionShape;
	btCollisionShapeData*           m_rootCollisionShape;
	char*                           m_name;
	btTransformFloatData           m_worldTransform;
	btTransformFloatData           m_interpolationWorldTransform;
	btVector3FloatData             m_interpolationLinearVelocity;
	btVector3FloatData             m_interpolationAngularVelocity;
	btVector3FloatData             m_anisotropicFriction;
	float                          m_contactProcessingThreshold;
	float                          m_deactivationTime;
	float                          m_friction;
	float                          m_restitution;
	float                          m_hitFraction;
	float                          m_ccdSweptSphereRadius;
	float                          m_ccdMotionThreshold;
	int                            m_hasAnisotropicFriction;
	int                            m_collisionFlags;
	int                            m_islandTag1;
	int                            m_companionId;
	int                            m_activationState1;
	int                            m_internalType;
	int                            m_checkCollideWith;
} btCollisionObjectFloatData;

typedef struct btRigidBodyFloatData
{
	btCollisionObjectFloatData           m_collisionObjectData;
	btMatrix3x3FloatData                 m_invInertiaTensorWorld;
	btVector3FloatData                   m_linearVelocity;
	btVector3FloatData                   m_angularVelocity;
	btVector3FloatData                   m_angularFactor;
	btVector3FloatData                   m_linearFactor;
	btVector3FloatData                   m_gravity;
	btVector3FloatData                   m_gravity_acceleration;
	btVector3FloatData                   m_invInertiaLocal;
	btVector3FloatData                   m_totalForce;
	btVector3FloatData                   m_totalTorque;
	float                                m_inverseMass;
	float                                m_linearDamping;
	float                                m_angularDamping;
	float                                m_additionalDampingFactor;
	float                                m_additionalLinearDampingThresholdSqr;
	float                                m_additionalAngularDampingThresholdSqr;
	float                                m_additionalAngularDampingFactor;
	float                                m_linearSleepingThreshold;
	float                                m_angularSleepingThreshold;
	int                                  m_additionalDamping;
} btRigidBodyFloatData;

typedef struct btRigidBodyDoubleData
{
	btCollisionObjectDoubleData           m_collisionObjectData;
	btMatrix3x3DoubleData                 m_invInertiaTensorWorld;
	btVector3DoubleData                   m_linearVelocity;
	btVector3DoubleData                   m_angularVelocity;
	btVector3DoubleData                   m_angularFactor;
	btVector3DoubleData                   m_linearFactor;
	btVector3DoubleData                   m_gravity;
	btVector3DoubleData                   m_gravity_acceleration;
	btVector3DoubleData                   m_invInertiaLocal;
	btVector3DoubleData                   m_totalForce;
	btVector3DoubleData                   m_totalTorque;
	double                                m_inverseMass;
	double                                m_linearDamping;
	double                                m_angularDamping;
	double                                m_additionalDampingFactor;
	double                                m_additionalLinearDampingThresholdSqr;
	double                                m_additionalAngularDampingThresholdSqr;
	double                                m_additionalAngularDampingFactor;
	double                                m_linearSleepingThreshold;
	double                                m_angularSleepingThreshold;
	int                                   m_additionalDamping;
	char                                  m_padding[4];
} btRigidBodyDoubleData;

typedef struct btPoint2PointConstraintFloatData
{
	btTypedConstraintData           m_typeConstraintData;
	btVector3FloatData              m_pivotInA;
	btVector3FloatData              m_pivotInB;
} btPoint2PointConstraintFloatData;

typedef struct btPoint2PointConstraintDoubleData
{
	btTypedConstraintData           m_typeConstraintData;
	btVector3DoubleData             m_pivotInA;
	btVector3DoubleData             m_pivotInB;
} btPoint2PointConstraintDoubleData;

typedef struct btHingeConstraintDoubleData
{
	btTypedConstraintData           m_typeConstraintData;
	btTransformDoubleData           m_rbAFrame;
	btTransformDoubleData           m_rbBFrame;
	int                             m_useReferenceFrameA;
	int                             m_angularOnly;
	int                             m_enableAngularMotor;
	float                           m_motorTargetVelocity;
	float                           m_maxMotorImpulse;
	float                           m_lowerLimit;
	float                           m_upperLimit;
	float                           m_limitSoftness;
	float                           m_biasFactor;
	float                           m_relaxationFactor;
} btHingeConstraintDoubleData;

typedef struct btHingeConstraintFloatData
{
	btTypedConstraintData           m_typeConstraintData;
	btTransformFloatData            m_rbAFrame;
	btTransformFloatData            m_rbBFrame;
	int                             m_useReferenceFrameA;
	int                             m_angularOnly;
	int                             m_enableAngularMotor;
	float                           m_motorTargetVelocity;
	float                           m_maxMotorImpulse;
	float                           m_lowerLimit;
	float                           m_upperLimit;
	float                           m_limitSoftness;
	float                           m_biasFactor;
	float                           m_relaxationFactor;
} btHingeConstraintFloatData;

typedef struct btConeTwistConstraintData
{
	btTypedConstraintData           m_typeConstraintData;
	btTransformFloatData            m_rbAFrame;
	btTransformFloatData            m_rbBFrame;
	float                           m_swingSpan1;
	float                           m_swingSpan2;
	float                           m_twistSpan;
	float                           m_limitSoftness;
	float                           m_biasFactor;
	float                           m_relaxationFactor;
	float                           m_damping;
	char                            m_pad[4];
} btConeTwistConstraintData;

typedef struct btGeneric6DofConstraintData
{
	btTypedConstraintData           m_typeConstraintData;
	btTransformFloatData            m_rbAFrame;
	btTransformFloatData            m_rbBFrame;
	btVector3FloatData              m_linearUpperLimit;
	btVector3FloatData              m_linearLowerLimit;
	btVector3FloatData              m_angularUpperLimit;
	btVector3FloatData              m_angularLowerLimit;
	int                             m_useLinearReferenceFrameA;
	int                             m_useOffsetForConstraintFrame;
} btGeneric6DofConstraintData;

typedef struct btSliderConstraintData
{
	btTypedConstraintData           m_typeConstraintData;
	btTransformFloatData            m_rbAFrame;
	btTransformFloatData            m_rbBFrame;
	float                           m_linearUpperLimit;
	float                           m_linearLowerLimit;
	float                           m_angularUpperLimit;
	float                           m_angularLowerLimit;
	int                             m_useLinearReferenceFrameA;
	int                             m_useOffsetForConstraintFrame;
} btSliderConstraintData;

/** @}*/

#endif//_Bullet_h_
