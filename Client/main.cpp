#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <iostream> 
#include <string> 
#include <winsock2.h>

using namespace std;

void ShowErrorMessage(string message) {
	cout << "[오류발생]:" << message << "\n";
	system("pause");
	exit(1);
}

int main() {
	WSADATA wsaData;
	SOCKET clientSocket;
	SOCKADDR_IN serverAddress;

	int serverPort = 9876;
	char received[256];
	string sent;

	//Winsock 초기화. 
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) ShowErrorMessage("WSAStartup()");

	clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (clientSocket == INVALID_SOCKET) ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddress.sin_port = htons(serverPort);

	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) ShowErrorMessage("connect()\n");
	cout << "[현재상태] connect \n";

	while (1) {//클라이언트의 메세지를 받아서 그대로 다시 전달합니다.
		
		cout << "[메세지 전송]: " ;
		getline(cin, sent);
		if (sent == "") continue;
		send(clientSocket, sent.c_str(), sent.length(), 0);
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = '\0';
		if (strcmp(received, "[exit]") == 0) {
			cout << "[서버 종료]:\n";
			break;
		}
		cout << "[서버 메세지]: " << received << '\n';
	}
	closesocket(clientSocket);
	WSACleanup();
	system("pause");
	return 0;
}