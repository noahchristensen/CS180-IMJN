#include <iostream>
#include <WS2tcpip.h>
#include <vector>
#include <string>
#include<fstream>
#include <sstream>
#include "search.h"

// Include the Winsock library (lib) file
#pragma comment (lib, "ws2_32.lib")

// Saves us from typing std::cout << etc. etc. etc.
using namespace std;


// Main entry point into the server
void main()
{
	//parse the CSV file and put in ParseData
	vector<vector<string>> parseData;
	vector<string> clientDat;
	vector<vector<string>> searchRes;
	searchRes.clear();
	cout << "Parsing Data..." << endl;
	read(parseData);
	cout << "Done Parsing... Server starting..." << endl;
	// INITIALIZE WINSOCK

	// Structure to store the WinSock version. This is filled in
	// on the call to WSAStartup()
	WSADATA data;

	// To start WinSock, the required version must be passed to
	// WSAStartup(). This server is going to use WinSock version
	// 2 so create a word that will store 2 and 2 in hex i.e.
	// 0x0202
	WORD version = MAKEWORD(2, 2);

	// Start WinSock
	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0)
	{
		// Not ok! Get out quickly
		cout << "Can't start Winsock! " << wsOk;
		return;
	}

	// SOCKET CREATION AND BINDING

	// Create a socket, notice that it is a user datagram socket (UDP)
	SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);

	// Create a server hint structure for the server
	sockaddr_in serverHint;
	serverHint.sin_addr.S_un.S_addr = ADDR_ANY; // Us any IP address available on the machine
	serverHint.sin_family = AF_INET; // Address format is IPv4
	serverHint.sin_port = htons(54000); // Convert from little to big endian

	// Try and bind the socket to the IP and port
	if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
	{
		cout << "Can't bind socket! " << WSAGetLastError() << endl;
		return;
	}

	// MAIN LOOP SETUP AND ENTRY

	sockaddr_in client; // Use to hold the client information (port / ip address)
	int clientLength = sizeof(client); // The size of the client information

	char buf[1024]; // data sent by client
	char bufS[2048]; // data to be sent by server

	// Enter a loop
	while (true)
	{
		searchRes.clear();
		ZeroMemory(&client, clientLength); // Clear the client structure
		ZeroMemory(buf, 1024); // Clear the receive buffer
		ZeroMemory(bufS, 2048); // Clear the receive buffer

		// Wait for message
		int bytesIn = recvfrom(in, buf, 1024, 0, (sockaddr*)&client, &clientLength);
		if (bytesIn == SOCKET_ERROR)
		{
			cout << "Error receiving from client " << WSAGetLastError() << endl;
			continue;
		}

		// Display message and client info
		char clientIp[256]; // Create enough space to convert the address byte array
		ZeroMemory(clientIp, 256); // to string of characters

		// Convert from byte array to chars
		inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

		// Display the message / who sent it
		cout << "Message recv from " << clientIp << " : " << buf << endl;
		parseClient(buf, parseData, searchRes); //searched results are in searchRes

		if (!searchRes.empty())
		{
			string dataString = "";
			vector<string> miniVec;
			int max = 0;
			if (50 < searchRes.size())
			{
				max = 50;
			}
			else
			{
				max = searchRes.size();
			}

			for (int j = 0; j < max; j++)
			{
				miniVec = searchRes.at(j);
				if (!miniVec.empty())
				{
					for (int i = 0; i < miniVec.size(); i++)
					{
						dataString.append(miniVec.at(i));
					}
				}

				dataString.append("\n");
			}
			int num = dataString.size();

			strcpy_s(bufS, num + 1, dataString.c_str());

			int sendOk = sendto(in, bufS, 2048, 0, (sockaddr*)&client, sizeof(client));
			cout << "Sending message to " << clientIp << " : " << bufS << endl;
		}
		else
		{
			string noResults = "No Results Found";
			int num = noResults.size();

			strcpy_s(bufS, num + 1, noResults.c_str());
			int sendOk = sendto(in, bufS, 2048, 0, (sockaddr*)&client, sizeof(client));
		}
	}

	// Close socket
	closesocket(in);

	// Shutdown winsock
	WSACleanup();
}
