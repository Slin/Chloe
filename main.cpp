#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

#include "CLSensors.h"
#include "CLDisplay.h"
#include "CLMotors.h"

std::atomic<float> directionX;
std::atomic<float> directionY;
std::atomic<float> directionZ;
std::atomic<int> currentState;


void updateIMU()
{
	CL::Sensors sensors;
//	CL::Motors motors;

	currentState = 1;
	sensors.Calibrate(5.0f);
	currentState = 2;

//	motors.Initialize();

	while(currentState == 2)
	{
		sensors.Update();

		CL::Vector3 direction = sensors.orientation.GetRotatedVector(CL::Vector3(0.0f, 1.0f, 0.0f));

		directionX = direction.x;
		directionY = direction.y;
		directionZ = direction.z;
	}
}

int main()
{
	std::cout << "blubb" << std::endl;

	currentState = 0;
	std::thread(&updateIMU).detach();

	float speed = 0.0f;
	CL::Motors motors;
	CL::Display display;
	while(display.IsButtonPressed(CL::Display::Button::A))
	{
		std::string output("Chloe\n\n");

		if(currentState == 1)
		{
			output += "Calibra...\nDON'T MOVE";
		}
		else
		{
			output += "X: " + std::to_string(directionX).substr(0, 6) + "\n";
			output += "Y: " + std::to_string(directionY).substr(0, 6) + "\n";
			output += "Z: " + std::to_string(directionZ).substr(0, 6) + "\n";
			output += "Spd: " + std::to_string(speed).substr(0, 4);
		}

		display.ShowText(output.c_str());

		speed += 0.02f*(display.IsButtonPressed(CL::Display::Button::Up)-display.IsButtonPressed(CL::Display::Button::Down));
		speed = fminf(fmaxf(speed, 0.0f), 1.0f);
		motors.SetSpeed(CL::Motors::Motor::FrontLeft, speed);
		motors.SetSpeed(CL::Motors::Motor::FrontRight, speed);
		motors.SetSpeed(CL::Motors::Motor::BackLeft, speed);
		motors.SetSpeed(CL::Motors::Motor::BackRight, speed);

//		std::cout << output;
	}

	currentState = 3;
	display.Clear();
	mraa_deinit();
	return MRAA_SUCCESS;
}
