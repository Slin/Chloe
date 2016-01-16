//
// Created by Nils Daumann on 06.01.16.
//

#include "CLDisplay.h"

#include "vendor/gpio/gpio.h"

namespace CL
{
	gpio BUTTON_A(49, INPUT);
	gpio BUTTON_B(46, INPUT);
	gpio BUTTON_UP(44, INPUT);
	gpio BUTTON_DOWN(47, INPUT);
	gpio BUTTON_LEFT(165, INPUT);
	gpio BUTTON_RIGHT(45, INPUT);
	gpio BUTTON_SELECT(48, INPUT);

	Display::Display()
	{
		_oled.begin();
		_oled.clear(PAGE);
		_oled.display();
		_oled.setFontType(0);
	}

	void Display::ShowText(const char *text)
	{
		_oled.clear(PAGE);
		_oled.setCursor(0, 0);
		_oled.print(text);
		_oled.display();
	}

	void Display::Clear()
	{
		_oled.clear(PAGE);
		_oled.display();
	}

	bool Display::IsButtonPressed(Button button)
	{
		switch(button)
		{
			case A:
				return (BUTTON_A.pinRead() == HIGH);
			case B:
				return (BUTTON_B.pinRead() == HIGH);
			case Up:
				return (BUTTON_UP.pinRead() == HIGH);
			case Down:
				return (BUTTON_DOWN.pinRead() == HIGH);
			case Left:
				return (BUTTON_LEFT.pinRead() == HIGH);
			case Right:
				return (BUTTON_RIGHT.pinRead() == HIGH);
			case Select:
				return (BUTTON_SELECT.pinRead() == HIGH);
		}
	}
}
