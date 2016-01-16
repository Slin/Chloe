#ifndef CLSENSORS_H
#define CLSENSORS_H

#include <chrono>
#include "mraa.hpp"
#include "math/CLVector.h"
#include "math/CLQuaternion.h"
#include "vendor/ahrs/MadgwickAHRS.h"

class LSM9DS0;
namespace CL
{
    class Sensors
	{
	public:
		Sensors();
		~Sensors();

		void Update();
		void Calibrate(float time);

		Quaternion orientation;

	private:
		LSM9DS0 *_imu;

/*		std::chrono::high_resolution_clock::time_point _accelerometerTime;
		std::chrono::high_resolution_clock::time_point _gyroscopeTime;
		std::chrono::high_resolution_clock::time_point _magnetometerTime;*/

//		bool _overflow;

		Vector3 _accelerometer;
		Vector3 _gyroscope;
		Vector3 _magnetometer;

		Vector3 _gyroscopeCorrection;

		AHRS::Madgwick _madgwick;
	};
}

#endif //CLSENSORS_H