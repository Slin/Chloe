//
// Created by Nils Daumann on 10.05.16.
//

#ifndef CHLOE_CLPID_H
#define CHLOE_CLPID_H

namespace CL
{
	class PID
	{
	public:
		PID(float p = 0.1f, float i = 0.0001f, float d = 10.0f) : _p(p), _i(i), _d(d){}

		float Update(float error, float position)
		{
			double pTerm = error*_p;

			_integralState += error;
			if(_integralState > 30.0f) _integralState = 30.0f;
			else if(_integralState < -30.0f) _integralState = -30.0f;
			double iTerm = _integralState*_i;

			double dTerm = (position - _positionState)*_d;
			_positionState = position;

			return pTerm + iTerm - dTerm;
		}

	private:
		float _integralState;
		float _positionState;

		float _p;
		float _i;
		float _d;
	};
}


#endif //CHLOE_CLPID_H
