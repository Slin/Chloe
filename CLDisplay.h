//
// Created by Nils Daumann on 06.01.16.
//

#ifndef ___CLDISPLAY_H___
#define ___CLDISPLAY_H___

#include "vendor/oled/Edison_OLED.h"

namespace CL
{
	class Display
	{
		public:
			enum Button
			{
				A,
				B,
				Up,
				Down,
				Left,
				Right,
				Select
			};

			Display();

			void ShowText(const char *text);
			void Clear();
			bool IsButtonPressed(Button button);

		private:
			edOLED _oled;
	};
}
#endif //___CLDISPLAY_H___
