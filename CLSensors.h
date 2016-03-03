#ifndef CLSENSORS_H
#define CLSENSORS_H

#include <chrono>
#include "mraa.hpp"
#include "math/CLMatrix.h"
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
//		Vector3 CalculateSimpleAngles(Vector3 mag, Vector3 acc, float declination);

		LSM9DS0 *_imu;

		std::chrono::high_resolution_clock::time_point _gyroscopeTime;

//		bool _overflow;

		Quaternion _accelerometer;
		Quaternion _gyroscope;
//		Vector3 _magnetometer;

		Vector3 _gyroscopeCorrection;

//		AHRS::Madgwick _madgwick;
	};
}

#endif //CLSENSORS_H