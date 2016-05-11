//
// Created by Nils Daumann on 13.01.16.
//

#ifndef CHLOE_MOTORS_H
#define CHLOE_MOTORS_H

#include <unistd.h>
#include "mraa.hpp"
#include "vendor/pwm/SFE_PCA9685.h"

namespace CL
{
	class Motors
	{
	public:
		enum Motor
		{
			FrontLeft = 0,
			FrontRight = 1,
			BackRight = 2,
			BackLeft = 3
		};

		Motors();
		~Motors();

		void Initialize();
		void SetSpeed(Motor motor, float speed);

	private:
		uint16_t _startTime;
		uint16_t _stopTime;
		mraa::I2c* _pwmI2C;
		pca9685 *_pwm;
	};
}

#endif //CHLOE_MOTORS_H
