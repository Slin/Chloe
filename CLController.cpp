//
// Created by Nils Daumann on 25.01.16.
//

#include "CLController.h"

#include <iostream>

namespace CL
{
	Controller::Controller()
	{
		for(int i = 0; i < 4; i++)
		{
			_pids[i] = new PID(0.0016f, 0.00001f, 0.1f);
		}
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

		if(speed > 0.2f)
		{
			//Z zeigt nach hinten, X nach rechts und Y nach oben
			_speedFrontLeft = std::max(std::min(speed + GetControlSpeed(0, Vector3(-20.50f, 0.0f, -20.50f)), 1.0f), 0.0f);
			_speedFrontRight = std::max(std::min(speed + GetControlSpeed(1, Vector3(20.50f, 0.0f, -20.50f)), 1.0f), 0.0f);
			_speedBackLeft = std::max(std::min(speed + GetControlSpeed(2, Vector3(-20.50f, 0.0f, 20.50f)), 1.0f), 0.0f);
			_speedBackRight = std::max(std::min(speed + GetControlSpeed(3, Vector3(20.50f, 0.0f, 20.50f)), 1.0f), 0.0f);
		}
		else
		{
			//Z zeigt nach hinten, X nach rechts und Y nach oben
			_speedFrontLeft = 0.0f;
			_speedFrontRight = 0.0f;
			_speedBackLeft = 0.0f;
			_speedBackRight = 0.0f;
		}
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

		float distance = motorOffset.GetDistance(position);

		if(motorOffset.y > position.y)
			return distance;
		else
			return -distance;
	}

	float Controller::GetControlSpeed(int motor, Vector3 motorOffset)
	{
		Vector3 position = _currentRotationWithoutYaw.GetRotatedVector(motorOffset);
		float distance = GetControlDistance(motorOffset);
		float controlSpeed = _pids[motor]->Update(distance, position.y);
//		printf("controlSpeed: (%i, %f)\n", motor, controlSpeed);
		return controlSpeed;
	}
}
