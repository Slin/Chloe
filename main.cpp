#include <iostream>
#include <csignal>

#include "CLSensors.h"
#include "CLDisplay.h"
#include "CLMotors.h"
#include "CLNetwork.h"
#include "CLController.h"

//#define CHLOE_TEST
//#define CHLOE_ESC_CALIBRATION

CL::Display *_display = nullptr;
CL::Motors *_motors = nullptr;

void displayText(const std::string &text, CL::Display &display)
{
	std::string output("Chloe\n\n");
	output += text;
	display.ShowText(output.c_str());
}

void sigkill(int signum)
{
	if(_motors)
	{
		_motors->SetSpeed(CL::Motors::Motor::FrontLeft, 0.0f);
		_motors->SetSpeed(CL::Motors::Motor::FrontRight, 0.0f);
		_motors->SetSpeed(CL::Motors::Motor::BackLeft, 0.0f);
		_motors->SetSpeed(CL::Motors::Motor::BackRight, 0.0f);
	}

	if(_display)
	{
		_display->Clear();
	}

	mraa_deinit();

	exit(0);
}

#if !defined(CHLOE_TEST) && !defined(CHLOE_ESC_CALIBRATION)
int main()
{
	signal(SIGINT, sigkill);
	signal(SIGQUIT, sigkill);
	signal(SIGTERM, sigkill);

	CL::Display display;
	_display = &display;

	displayText("Calibra...\nDON'T MOVE", display);
	CL::Sensors sensors;
//	sensors.Calibrate(20.0f);

	CL::Network network;

	displayText("Waiting", display);
	while(!network.GetIsStarted())
	{
		network.Update();
		usleep(1000);
	}

	CL::Motors motors;
	CL::Controller controller;

	_motors = &motors;

	displayText("Active", display);
	while(network.GetIsStarted() && network.GetIsConnected())
	{
		network.Update();
		sensors.Update();
		controller.Update(sensors.orientation, CL::Quaternion(network.GetTargetOrientation()), network.GetUpSpeed());
		network.SendOrientation(sensors.orientation.GetEulerAngle());

		motors.SetSpeed(CL::Motors::Motor::FrontLeft, controller.GetSpeed(CL::Motors::Motor::FrontLeft));
		motors.SetSpeed(CL::Motors::Motor::FrontRight, controller.GetSpeed(CL::Motors::Motor::FrontRight));
		motors.SetSpeed(CL::Motors::Motor::BackLeft, controller.GetSpeed(CL::Motors::Motor::BackLeft));
		motors.SetSpeed(CL::Motors::Motor::BackRight, controller.GetSpeed(CL::Motors::Motor::BackRight));
	}

	motors.SetSpeed(CL::Motors::Motor::FrontLeft, 0.0f);
	motors.SetSpeed(CL::Motors::Motor::FrontRight, 0.0f);
	motors.SetSpeed(CL::Motors::Motor::BackLeft, 0.0f);
	motors.SetSpeed(CL::Motors::Motor::BackRight, 0.0f);

	display.Clear();
	mraa_deinit();

	return MRAA_SUCCESS;
}
#elif defined(CHLOE_TEST)
int main()
{
	CL::Motors motors;

	CL::Sensors sensors;
	sensors.Calibrate(3.0f);
	CL::Controller controller;

	while(1)
	{
		sensors.Update();
		controller.Update(sensors.orientation, 0.3f);

		motors.SetSpeed(CL::Motors::Motor::FrontLeft, controller.GetSpeed(CL::Motors::Motor::FrontLeft));
		motors.SetSpeed(CL::Motors::Motor::FrontRight, controller.GetSpeed(CL::Motors::Motor::FrontRight));
		motors.SetSpeed(CL::Motors::Motor::BackLeft, controller.GetSpeed(CL::Motors::Motor::BackLeft));
		motors.SetSpeed(CL::Motors::Motor::BackRight, controller.GetSpeed(CL::Motors::Motor::BackRight));
	}

	motors.SetSpeed(CL::Motors::Motor::FrontLeft, 0.0f);
	motors.SetSpeed(CL::Motors::Motor::FrontRight, 0.0f);
	motors.SetSpeed(CL::Motors::Motor::BackLeft, 0.0f);
	motors.SetSpeed(CL::Motors::Motor::BackRight, 0.0f);

	mraa_deinit();

	return MRAA_SUCCESS;
}
#elif defined(CHLOE_ESC_CALIBRATION)

int main()
{
	CL::Motors motors;
	CL::Display display;

	displayText("Connect the\nBatterie", display);

	motors.SetSpeed(CL::Motors::Motor::FrontLeft, 1.0f);
	motors.SetSpeed(CL::Motors::Motor::FrontRight, 1.0f);
	motors.SetSpeed(CL::Motors::Motor::BackLeft, 1.0f);
	motors.SetSpeed(CL::Motors::Motor::BackRight, 1.0f);

	usleep(10000000);

	motors.SetSpeed(CL::Motors::Motor::FrontLeft, 0.0f);
	motors.SetSpeed(CL::Motors::Motor::FrontRight, 0.0f);
	motors.SetSpeed(CL::Motors::Motor::BackLeft, 0.0f);
	motors.SetSpeed(CL::Motors::Motor::BackRight, 0.0f);

	usleep(5000000);

	display.Clear();

	mraa_deinit();

	return MRAA_SUCCESS;
}

#endif
