//
// Created by Nils Daumann on 24.01.16.
//

#include "CLNetwork.h"
#include <iostream>

namespace CL
{
	Network::Network()
	{
		if (enet_initialize () != 0)
		{
			std::cout << "An error occurred while initializing ENet" << std::endl;
			return;
		}

		ENetAddress address;
		address.host = ENET_HOST_ANY;
		address.port = 1234;
		_server = enet_host_create(&address /* the address to bind the server host to */,
								   1      /* allow up to 1 clients and/or outgoing connections */,
								   2      /* allow up to 2 channels to be used, 0 and 1 */,
								   0      /* assume any amount of incoming bandwidth */,
								   0      /* assume any amount of outgoing bandwidth */);
		if(!_server)
		{
			std::cout << "An error occurred while trying to create an ENet server host." << std::endl;
			return;
		}
	}

	void Network::Update()
	{
		enet_host_service(_server, _event, 0);
	}

	Network::~Network()
	{
		enet_host_destroy(_server);
		enet_deinitialize();
	}
}
