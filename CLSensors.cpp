#include "CLSensors.h"
#include "vendor/imu/SFE_LSM9DS0.h"

namespace CL
{
	Sensors::Sensors()// : _overflow(false)
	{
		_imu = new LSM9DS0(0x6B, 0x1D);
		_imu->begin();
//		_imu->setGyroODR(LSM9DS0::G_ODR_380_BW_50);

/*		_accelerometerTime = std::chrono::high_resolution_clock::now();
		_gyroscopeTime = _accelerometerTime;
		_magnetometerTime = _accelerometerTime;*/

		// The begin() function sets up some basic parameters and turns the device
		//  on; you may not need to do more than call it. It also returns the "whoami"
		//  registers from the chip. If all is good, the return value here should be
		//  0x49d4. Here are the initial settings from this function:
		//  Gyro scale:        245 deg/sec max
		//  Xl scale:          4g max
		//  Mag scale:         2 Gauss max
		//  Gyro sample rate:  95Hz
		//  Xl sample rate:    100Hz
		//  Mag sample rate:   100Hz
		// These can be changed either by calling appropriate functions or by
		//  pasing parameters to the begin() function. There are named constants in
		//  the .h file for all scales and data rates; I won't reproduce them here.
		//  Here's the list of fuctions to set the rates/scale:
		//  setMagScale(mag_scale mScl)      setMagODR(mag_odr mRate)
		//  setGyroScale(gyro_scale gScl)    setGyroODR(gyro_odr gRate)
		//  setAccelScale(accel_scale aScl)  setGyroODR(accel_odr aRate)
		// If you want to make these changes at the point of calling begin, here's
		//  the prototype for that function showing the order to pass things:
		//  begin(gyro_scale gScl, accel_scale aScl, mag_scale mScl,
		//				gyro_odr gODR, accel_odr aODR, mag_odr mODR)
		//uint16_t imuResult = _imu->begin();
		//std::cout << std::hex << "Chip ID: 0x" << imuResult << std::dec << " (should be 0x49d4)" << std::endl;
	}

	Sensors::~Sensors()
	{
		delete _imu;
	}

	void Sensors::Update()
	{
		if(_imu->newXData())
		{
//			_accelerometerTime = std::chrono::high_resolution_clock::now();
			_imu->readAccel();

			_accelerometer.x = _imu->calcAccel(_imu->ax);
			_accelerometer.y = _imu->calcAccel(_imu->ay);
			_accelerometer.z = _imu->calcAccel(_imu->az);
		}
		if(_imu->newGData())
		{
/*			auto newTime = std::chrono::high_resolution_clock::now();
			float timeDiff = std::chrono::duration_cast<std::chrono::nanoseconds>(newTime - _gyroscopeTime).count()/1000000000.0f;
			_gyroscopeTime = newTime;*/
			_imu->readGyro();

			_gyroscope.x = _imu->calcGyro(_imu->gx);
			_gyroscope.y = _imu->calcGyro(_imu->gy);
			_gyroscope.z = _imu->calcGyro(_imu->gz);

			_gyroscope -= _gyroscopeCorrection;
//			direction += _gyroscope*timeDiff;
		}
		if(_imu->newMData())
		{
//			_magnetometerTime = std::chrono::high_resolution_clock::now();
			_imu->readMag();
			_imu->readTemp();

			_magnetometer.x = _imu->calcMag(_imu->mx);
			_magnetometer.y = _imu->calcMag(_imu->my);
			_magnetometer.z = _imu->calcMag(_imu->mz);
		}


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

		_madgwick.update(_gyroscope.x, _gyroscope.y, _gyroscope.z, _accelerometer.x, _accelerometer.y, _accelerometer.z, _magnetometer.x, _magnetometer.y, _magnetometer.z);
		orientation = Quaternion(_madgwick.q1, _madgwick.q2, _madgwick.q3, _madgwick.q0);
	}

	void Sensors::Calibrate(float time)
	{
/*		direction = Vector3();
		_gyroscopeCorrection = Vector3();
		auto calibrationStart = std::chrono::high_resolution_clock::now();
		while(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - calibrationStart).count() < time*1000000000)
		{
			Update();
		}
		_gyroscopeCorrection = direction/time;
		direction = Vector3();*/
	}
}
