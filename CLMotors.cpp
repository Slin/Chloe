//
// Created by Nils Daumann on 13.01.16.
//

#include "CLMotors.h"

#include <iostream>

#define MOTOR_MIN_VALUE 1960
#define MOTOR_MAX_VALUE 3500
#define MOTOR_ARM_VALUE 1730

namespace CL
{
	Motors::Motors()
	{
		// We need to create an I2c object that we can pass to
		//  the pca9685 constructor. If you have more than one
		//  PCA9685 device on your bus (either by stacking more
		//  than one PWM block or by adding external boards via
		//  the I2C Expansion Block), you'll need to create a
		//  different I2c object for each one!
		_pwmI2C = new mraa::I2c(1); // Tell the I2c object which bus it's on.

		_pwm = new pca9685(_pwmI2C, 0x40); // 0x40 is the default address for the PCA9685.

		_pwm->enableServoMode();
		_pwm->setPrescaler(static_cast<uint8_t>(14)); //~400Hz

		_pwm->setChlTime(static_cast<uint8_t>(Motor::FrontLeft), 0, MOTOR_ARM_VALUE);
		_pwm->setChlTime(static_cast<uint8_t>(Motor::FrontRight), 0, MOTOR_ARM_VALUE);
		_pwm->setChlTime(static_cast<uint8_t>(Motor::BackLeft), 0, MOTOR_ARM_VALUE);
		_pwm->setChlTime(static_cast<uint8_t>(Motor::BackRight), 0, MOTOR_ARM_VALUE);

		usleep(100000);
	}

	Motors::~Motors()
	{
		_pwm->setChlTime(static_cast<uint8_t>(Motor::FrontLeft), 0, 0);
		_pwm->setChlTime(static_cast<uint8_t>(Motor::FrontRight), 0, 0);
		_pwm->setChlTime(static_cast<uint8_t>(Motor::BackLeft), 0, 0);
		_pwm->setChlTime(static_cast<uint8_t>(Motor::BackRight), 0, 0);

		delete _pwm;
		delete _pwmI2C;
	}

	void Motors::SetSpeed(Motor motor, float speed)
	{
		speed = std::min(std::max(speed, 0.0f), 1.0f);
		_pwm->setChlTime(static_cast<uint8_t>(motor), 0, MOTOR_MIN_VALUE+speed*(MOTOR_MAX_VALUE-MOTOR_MIN_VALUE));
	}
}
