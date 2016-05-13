#include "CLSensors.h"
#include "vendor/imu/SFE_LSM9DS0.h"
#include <iostream>

namespace CL
{
	Sensors::Sensors()// : _overflow(false)
	{
		_imu = new LSM9DS0(0x6B, 0x1D);
		_imu->begin(LSM9DS0::gyro_scale::G_SCALE_245DPS, LSM9DS0::accel_scale::A_SCALE_4G, LSM9DS0::mag_scale::M_SCALE_4GS, LSM9DS0::gyro_odr::G_ODR_190_BW_50, LSM9DS0::accel_odr::A_ODR_200, LSM9DS0::mag_odr::M_ODR_125);

		_gyroscopeTime = std::chrono::high_resolution_clock::now();

		_accelerometerCorrection.x = 0.0622763;
		_accelerometerCorrection.y = -0.000116977;
		_accelerometerCorrection.z = 0.00188158;
		_accelerometerCorrection.w = 0.998055;

		_gyroscopeCorrection.x = -2.05624;
		_gyroscopeCorrection.y = 2.10551;
		_gyroscopeCorrection.z = 0.0f;
	}

	Sensors::~Sensors()
	{
		delete _imu;
	}

	void Sensors::Update()
	{
		if(_imu->newXData())
		{
			_imu->readAccel();

			Vector3 accelerometer;
			accelerometer.x = _imu->calcAccel(_imu->ax);
			accelerometer.y = _imu->calcAccel(_imu->ay);
			accelerometer.z = _imu->calcAccel(_imu->az);
			accelerometer.Normalize();

			float pitch = atan2(-accelerometer.x, sqrt(accelerometer.y*accelerometer.y + accelerometer.z*accelerometer.z))*180.0/M_PI;
			float roll = atan2(accelerometer.y,( accelerometer.z>0.0f?1.0f:-1.0f)*sqrt(accelerometer.z*accelerometer.z + 0.001f*accelerometer.x*accelerometer.x))*180.0/M_PI;

			CL::Quaternion _currentAccelerometer = Quaternion::WithEulerAngle(Vector3(0.0f, pitch, roll));
			_currentAccelerometer /= _accelerometerCorrection;

			_accelerometer = _accelerometer.GetLerpSpherical(_currentAccelerometer, 0.1f);
		}
		if(_imu->newGData())
		{
			auto newTime = std::chrono::high_resolution_clock::now();
			float timeDiff = std::chrono::duration_cast<std::chrono::nanoseconds>(newTime - _gyroscopeTime).count()/1000000000.0f;
			_gyroscopeTime = newTime;
			_imu->readGyro();

			Vector3 gyroscope;
			gyroscope.x = _imu->calcGyro(_imu->gx);
			gyroscope.y = _imu->calcGyro(_imu->gy);
//			gyroscope.z = _imu->calcGyro(_imu->gz);
			gyroscope -= _gyroscopeCorrection;

			_gyroscope = orientation*Quaternion::WithEulerAngle(Vector3(0.0f, gyroscope.y*timeDiff, gyroscope.x*timeDiff));
		}
/*		if(_imu->newMData())
		{
			_imu->readMag();
			_imu->readTemp();

			_magnetometer.x = _imu->calcMag(_imu->mx);
			_magnetometer.y = _imu->calcMag(_imu->my);
			_magnetometer.z = _imu->calcMag(_imu->mz);
		}*/


		// Of course, we may care if an overflow occurred; we can check that
		//  easily enough from an internal register on the part. There are functions
		//  to check for overflow per device.
/*		_overflow = _imu->xDataOverflow() |
					_imu->gDataOverflow() |
					_imu->mDataOverflow();

		if(_overflow)
		{
			std::cout << "WARNING: DATA OVERFLOW!!!" << std::endl;
		}*/

//		_madgwick.updateIMU(_gyroscope.x, _gyroscope.y, _gyroscope.z, _accelerometer.x, _accelerometer.y, _accelerometer.z);
//		orientation = Quaternion(_madgwick.q1, _madgwick.q2, _madgwick.q3, _madgwick.q0);


		orientation = Quaternion::WithLerpSpherical(_accelerometer, _gyroscope, 0.99f);

		Vector3 euler = _accelerometer.GetEulerAngle();
//		std::cout << "acc: (" << _accelerometer.x << ", " << _accelerometer.y << ", " << _accelerometer.z << ")" << std::endl;
//		std::cout << "rot: (" << euler.y << ", " << euler.z << ")" << std::endl;
	}

	void Sensors::Calibrate(float time)
	{
		_accelerometerCorrection = Quaternion();
		_gyroscopeCorrection = Vector3();
		auto calibrationStart = std::chrono::high_resolution_clock::now();
		_gyroscopeTime = std::chrono::high_resolution_clock::now();
		while(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - calibrationStart).count() < time*1000000000)
		{
			if(_imu->newXData())
			{
				_imu->readAccel();

				Vector3 accelerometer;
				accelerometer.x = _imu->calcAccel(_imu->ax);
				accelerometer.y = _imu->calcAccel(_imu->ay);
				accelerometer.z = _imu->calcAccel(_imu->az);
				accelerometer.Normalize();

				float pitch = atan2(-accelerometer.x, sqrt(accelerometer.y*accelerometer.y + accelerometer.z*accelerometer.z))*180.0/M_PI;
				float roll = atan2(accelerometer.y,( accelerometer.z>0.0f?1.0f:-1.0f)*sqrt(accelerometer.z*accelerometer.z + 0.001f*accelerometer.x*accelerometer.x))*180.0/M_PI;

				_accelerometerCorrection = _accelerometerCorrection.GetLerpSpherical(Quaternion::WithEulerAngle(Vector3(0.0f, pitch, roll)), 0.1f);
			}

			if(_imu->newGData())
			{
				auto newTime = std::chrono::high_resolution_clock::now();
				float timeDiff = std::chrono::duration_cast<std::chrono::nanoseconds>(newTime - _gyroscopeTime).count()/1000000000.0f;
				_gyroscopeTime = newTime;
				_imu->readGyro();

				Vector3 gyroscope;
				gyroscope.x = _imu->calcGyro(_imu->gx);
				gyroscope.y = _imu->calcGyro(_imu->gy);
//				gyroscope.z = _imu->calcGyro(_imu->gz);
				_gyroscopeCorrection += gyroscope*timeDiff;
			}
		}
		_gyroscopeCorrection /= time;
		orientation = Quaternion::WithIdentity();

		std::cout << "Accelerometer Correction: (" << _accelerometerCorrection.x << ", " << _accelerometerCorrection.y << ", " << _accelerometerCorrection.z << ", " << _accelerometerCorrection.w << ")" << std::endl;
		std::cout << "Gyroscope Correction: (" << _gyroscopeCorrection.x << ", " << _gyroscopeCorrection.y << ", " << _gyroscopeCorrection.z << ")" << std::endl;
	}

/*	Vector3 Sensors::CalculateSimpleAngles(Vector3 mag, Vector3 acc, float declination)
	{
		Vector3 angles;
		float zz = acc.z * acc.z;
*/
		/* See eq. 37 and 38 in
		 * http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf */
/*		angles.x = -atan2(acc.y, sqrt(acc.x * acc.x + zz)) * (180.0 / M_PI);
		angles.y = atan2(acc.x, sqrt(acc.y * acc.y + zz)) * (180.0 / M_PI);

		angles.z = atan2 (mag.x, mag.y) * (180.0 / M_PI) - declination;
		if (angles.z > 180)
			angles.z -= 360;
		else if (angles.z < -180)
			angles.z += 360;
	}*/
}
