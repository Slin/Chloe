//
// Created by Nils Daumann on 24.01.16.
//

#ifndef CHLOE_CLNETWORK_H
#define CHLOE_CLNETWORK_H

#include "enet/enet.h"
#include "math/CLVector.h"

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
		float GetUpSpeed() const {return _upSpeed;}
		Vector3 GetTargetOrientation() const {return _targetOrientation;}
		void SendOrientation(CL::Vector3 orientation);

	private:
		ENetHost *_server;
		ENetPeer *_peer;
		ENetEvent _event;
		bool _isConnected;
		bool _isStarted;
		float _upSpeed;
		Vector3 _targetOrientation;
	};
}


#endif //CHLOE_CLNETWORK_H
