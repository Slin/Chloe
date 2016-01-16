//
// Created by Nils Daumann on 06.01.16.
//

#include "vendor/oled/Edison_OLED.h"

#ifndef ___CLDISPLAY_H___
#define ___CLDISPLAY_H___

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
			edOled _oled;
	};
}
#endif //___CLDISPLAY_H___
