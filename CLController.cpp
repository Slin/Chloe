//
// Created by Nils Daumann on 25.01.16.
//

#include "CLController.h"

#include <iostream>

namespace CL
{
	Controller::Controller()
	{

	}

	Controller::~Controller()
	{

	}

	void Controller::Update(const Quaternion &orientation, float speed)
	{
		_currentOrientation = orientation;
		_currentRotationWithoutYaw = _currentOrientation;
//		_currentRotationWithoutYaw.z = 0.0f;
//		_currentRotationWithoutYaw.Normalize();

		//Z zeigt nach hinten, X nach rechts und Y nach oben
		_speedFrontLeft = std::min(GetControlDistance(Vector3(-20.50f, 0.0f, -20.50f))*0.01f, 0.2f);
		_speedFrontRight = std::min(GetControlDistance(Vector3(20.50f, 0.0f, -20.50f))*0.01f, 0.2f);
		_speedBackLeft = std::min(GetControlDistance(Vector3(-20.50f, 0.0f, 20.50f))*0.01f, 0.2f);
		_speedBackRight = std::min(GetControlDistance(Vector3(20.50f, 0.0f, 20.50f))*0.01f, 0.2f);
	}

	float Controller::GetSpeed(Motors::Motor motor)
	{
		switch(motor)
		{
			case Motors::Motor::FrontLeft:
				return _speedFrontLeft;
			case Motors::Motor::FrontRight:
				return _speedFrontRight;
			case Motors::Motor::BackLeft:
				return _speedBackLeft;
			case Motors::Motor::BackRight:
				return _speedBackRight;
		}
	}

	float Controller::GetControlDistance(Vector3 motorOffset)
	{
		Vector3 position = _currentRotationWithoutYaw.GetRotatedVector(motorOffset);

		if(motorOffset.y > position.y)
			return motorOffset.GetDistance(position);

		return 0.0f;
	}
}
