#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

#include "CLSensors.h"
#include "CLDisplay.h"
#include "CLMotors.h"
#include "CLNetwork.h"

void displayText(const std::string &text, CL::Display &display)
{
	std::string output("Chloe\n\n");
	output += text;
	display.ShowText(output.c_str());
}

int main()
{
	CL::Display display;

	displayText("Calibra...\nDON'T MOVE", display);
	CL::Sensors sensors;
	sensors.Calibrate(1.0f);

	CL::Network network;

	displayText("Waiting", display);
	while(!network.GetIsStarted())
	{
		network.Update();
		usleep(1000);
	}

	CL::Motors motors;

	displayText("Active", display);
	while(network.GetIsStarted() && network.GetIsConnected())
	{
		network.Update();
		sensors.Update();

		motors.SetSpeed(CL::Motors::Motor::FrontLeft, network.GetUpSpeed());
		motors.SetSpeed(CL::Motors::Motor::FrontRight, network.GetUpSpeed());
		motors.SetSpeed(CL::Motors::Motor::BackLeft, network.GetUpSpeed());
		motors.SetSpeed(CL::Motors::Motor::BackRight, network.GetUpSpeed());
	}

	motors.SetSpeed(CL::Motors::Motor::FrontLeft, 0.0f);
	motors.SetSpeed(CL::Motors::Motor::FrontRight, 0.0f);
	motors.SetSpeed(CL::Motors::Motor::BackLeft, 0.0f);
	motors.SetSpeed(CL::Motors::Motor::BackRight, 0.0f);

	display.Clear();
	mraa_deinit();

	std::cout << "Closed" << std::endl;

	return MRAA_SUCCESS;
}
