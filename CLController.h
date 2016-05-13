//
// Created by Nils Daumann on 25.01.16.
//

#ifndef CHLOE_CLCONTROLLER_H
#define CHLOE_CLCONTROLLER_H

#include "math/CLVector.h"
#include "math/CLMatrix.h"
#include "math/CLQuaternion.h"
#include "CLMotors.h"
#include "CLPID.h"

namespace CL
{
	class Controller
	{
	public:
		Controller();
		~Controller();

		void Update(const Quaternion &orientation, const Quaternion &targetOrientation, float speed);

		float GetSpeed(Motors::Motor motor);

	private:
		float GetControlDistance(Vector3 motorOffset);
		float GetControlSpeed(int motor, Vector3 motorOffset);

		CL::Quaternion _currentOrientation;
		CL::Quaternion _currentRotationWithoutYaw;
		CL::Quaternion _targetOrientation;

		float _speedFrontLeft;
		float _speedFrontRight;
		float _speedBackLeft;
		float _speedBackRight;

		PID *_pids[4];
	};
}

#endif //CHLOE_CLCONTROLLER_H
