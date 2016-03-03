//
// Created by Nils Daumann on 25.01.16.
//

#ifndef CHLOE_CLCONTROLLER_H
#define CHLOE_CLCONTROLLER_H

#include "math/CLVector.h"
#include "math/CLMatrix.h"
#include "math/CLQuaternion.h"
#include "CLMotors.h"

namespace CL
{
	class Controller
	{
	public:
		Controller();
		~Controller();

		void Update(const Quaternion &orientation, float speed);

		float GetSpeed(Motors::Motor motor);

	private:
		float GetControlDistance(Vector3 motorOffset);

		CL::Quaternion _currentOrientation;
		CL::Quaternion _currentRotationWithoutYaw;

		float _speedFrontLeft;
		float _speedFrontRight;
		float _speedBackLeft;
		float _speedBackRight;
	};
}

#endif //CHLOE_CLCONTROLLER_H
