#include <iostream>
//#include <csignal>

#include "CLSensors.h"
#include "CLDisplay.h"
#include "CLMotors.h"
#include "CLNetwork.h"
#include "CLController.h"

//#define CHLOE_TEST

/*void termination_handler(int signum)
{

}*/

void displayText(const std::string &text, CL::Display &display)
{
	std::string output("Chloe\n\n");
	output += text;
	display.ShowText(output.c_str());
}

#ifndef CHLOE_TEST
int main()
{
/*	struct sigaction new_action, old_action;

	//Set up the structure to specify the new action.
	new_action.sa_handler = termination_handler;
	sigemptyset(&new_action.sa_mask);
	new_action.sa_flags = 0;

	sigaction(SIGINT, NULL, &old_action);
	if(old_action.sa_handler != SIG_IGN)
		sigaction(SIGINT, &new_action, NULL);
	sigaction(SIGHUP, NULL, &old_action);
	if(old_action.sa_handler != SIG_IGN)
		sigaction(SIGHUP, &new_action, NULL);
	sigaction(SIGTERM, NULL, &old_action);
	if(old_action.sa_handler != SIG_IGN)
		sigaction(SIGTERM, &new_action, NULL);*/


	CL::Display display;

	displayText("Calibra...\nDON'T MOVE", display);
	CL::Sensors sensors;
	sensors.Calibrate(3.0f);

	CL::Network network;

	displayText("Waiting", display);
	while(!network.GetIsStarted())
	{
		network.Update();
		usleep(1000);
	}

	CL::Motors motors;
	CL::Controller controller;

	displayText("Active", display);
	while(network.GetIsStarted() && network.GetIsConnected())
	{
		network.Update();
		sensors.Update();
		controller.Update(sensors.orientation, network.GetUpSpeed());

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
#else
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
#endif
