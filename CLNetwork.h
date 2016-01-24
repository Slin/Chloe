//
// Created by Nils Daumann on 24.01.16.
//

#ifndef CHLOE_CLNETWORK_H
#define CHLOE_CLNETWORK_H

#include "vendor/enet/include/enet/enet.h"

namespace CL
{
	class Network
	{
	public:
		Network();
		~Network();

		void Update();

	private:
		ENetHost *_server;
		ENetEvent *_event;
	};
}


#endif //CHLOE_CLNETWORK_H
