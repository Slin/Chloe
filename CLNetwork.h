//
// Created by Nils Daumann on 24.01.16.
//

#ifndef CHLOE_CLNETWORK_H
#define CHLOE_CLNETWORK_H

#include "enet/enet.h"

namespace CL
{
	class Network
	{
	public:
		Network();
		~Network();

		void Update();

		bool GetIsConnected() const {return _isConnected;}
		bool GetIsStarted() const {return _isStarted;}
		float GetUpSpeed()const {return _upSpeed;}

	private:
		ENetHost *_server;
		ENetEvent _event;
		bool _isConnected;
		bool _isStarted;
		float _upSpeed;
	};
}


#endif //CHLOE_CLNETWORK_H
