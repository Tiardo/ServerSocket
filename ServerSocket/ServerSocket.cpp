
#define	WIN32_LEAN_AND_MEAN

#include <iostream>
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;


int main()
{
	WSADATA wsaData;
	ADDRINFO hints;
	ADDRINFO* addrResult = NULL;
	SOCKET ClientSocket = INVALID_SOCKET;
	SOCKET LesonSocket = INVALID_SOCKET;


	const char* sendBuffer = "Server +";
	char recvBuffer[512];


	int result;
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (result != 0)
	{
		cout << "Warning Nax , result = " << result << endl;
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;



	result = getaddrinfo(NULL, "8803", &hints, &addrResult);
	if (result != 0) {

		cout << "Problemka s soketom ili locallhostom" << result << endl;
		WSACleanup();
		return 1;
	}


	LesonSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
	if (LesonSocket == NULL) {

		cout << "ConnectSocket NICHT ichWIll" << endl;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}


	result = bind(LesonSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
	if (result == SOCKET_ERROR)
	{
		cout << "Beda s bindingom" << endl;
		closesocket(LesonSocket);
		LesonSocket = INVALID_SOCKET;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}


	result = listen(LesonSocket, SOMAXCONN);
	if (result == SOCKET_ERROR) {

		cout << "listen faulledd" << endl;
		closesocket(LesonSocket);
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}


	ClientSocket = accept(LesonSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {

		cout << "accept faulledd" << endl;
		closesocket(LesonSocket);
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;

	}

	closesocket(LesonSocket);


	do {

		ZeroMemory(recvBuffer, 512);
		result = recv(ClientSocket, recvBuffer, 512, 0);
		if (result > 0)
		{






			cout << "Soobwenie1 " << result << endl;
			cout << "Soobwenie2 " << recvBuffer << endl;
			result = send(ClientSocket, sendBuffer, (int)strlen(sendBuffer), 0);

			if (result == SOCKET_ERROR) {

				cout << "send error " << endl;
				closesocket(ClientSocket);
				freeaddrinfo(addrResult);
				WSACleanup();
				return 1;
			}

		}
		else if (result == 0)
			cout << "close connect blea " << endl;
		else
		{
			cout << "connekt ok" << endl;
			closesocket(ClientSocket);
			freeaddrinfo(addrResult);
			WSACleanup();
			return 1;
		}
			
	} while (result > 0);

	result = shutdown(ClientSocket, SD_SEND);

	if (result == SOCKET_ERROR) {
		cout << "sutdovn blea" << endl;
		closesocket(ClientSocket);
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;

	}

	closesocket(ClientSocket);
	freeaddrinfo(addrResult);
	WSACleanup();
	return 0;

}
