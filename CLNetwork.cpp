//
// Created by Nils Daumann on 24.01.16.
//

#include "CLNetwork.h"
#include <iostream>
#include <cmath>

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

	Network::~Network()
	{
		enet_host_destroy(_server);
		enet_deinitialize();
	}

	void Network::Update()
	{
		while(enet_host_service(_server, &_event, 0))
		{
			switch(_event.type)
			{
				case ENET_EVENT_TYPE_CONNECT:
					std::cout << "A new client connected." << std::endl;
					/* Store any relevant client information here. */
					_event.peer->data = (void*)"Controller";
					_isConnected = true;
					_isStarted = false;
					break;

				case ENET_EVENT_TYPE_DISCONNECT:
					std::cout << _event.peer->data << " disconnected." << std::endl;
					/* Reset the peer's client information. */
					_event.peer->data = NULL;
					_isConnected = false;
					break;

				case ENET_EVENT_TYPE_RECEIVE:
					std::string packageData(reinterpret_cast<char*>(_event.packet->data));
					enet_packet_destroy(_event.packet);
					//std::cout << "Received package: (" << packageData << ")" << std::endl;

					if(packageData.compare("start") == 0)
					{
						_isStarted = true;
					}

					if(packageData.compare("stop") == 0)
					{
						_isStarted = false;
						_upSpeed = 0.0f;
					}

					if(!_isStarted)
						break;

					if(packageData.find("up:") == 0)
					{
						float value = std::stof(packageData.substr(3));
						if(std::isfinite(value))
						{
							_upSpeed = value;
						}
						else
						{
							_upSpeed = 0.0f;
						}
					}

					break;
			}
		}
	}
}
