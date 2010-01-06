#include <AwPoint.h>
#include <AwVector.h>
#include <AwMatrix.h>
#include <AwQuaternion.h>

void solveIK(const AwPoint &startJointPos,
			 const AwPoint &midJointPos,
			 const AwPoint &effectorPos,
			 const AwPoint &handlePos,
			 const AwVector &poleVector,
			 double twistValue,
			 AwQuaternion &qStart,
			 AwQuaternion &qMid);
