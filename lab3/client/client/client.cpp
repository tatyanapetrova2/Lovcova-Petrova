#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <sstream>
#include <string>
#include <document.h>
#include <prettywriter.h>

#define _WIN32_WINNT 0x501

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>

#pragma comment(lib, "Ws2_32.lib")


#define SERVERADDR "127.0.0.1"
#define PORT 228
#define BUF_SIZE 1024

using namespace std;
using namespace rapidjson;

struct state {
	int count;
	state() { count = 100; }
	state(int val) { count = val; }
	string toJSON() {
		Value json_val;
		Document doc;
		auto& allocator = doc.GetAllocator();
		doc.SetObject();
		json_val.SetInt(count);
		doc.AddMember("count", json_val, allocator);

		StringBuffer buffer;
		PrettyWriter<StringBuffer> writer(buffer);
		doc.Accept(writer);
		return buffer.GetString();
	}

	static state fromJSON(const char* json) {
		Document doc;
		doc.Parse(json);
		state result(doc["count"].GetInt());
		return result;
	}
};

void main() {
	setlocale(LC_ALL, "rus");
	
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		cout << "Error WSAStartup" << endl;
		system("pause");
		return;
	}
	SOCKET so = socket(AF_INET, SOCK_STREAM, 0);
	if (so < 0) {
		cout << "Error socket" << endl;
		WSACleanup();
		system("pause");
		return;
	}
	sockaddr_in dest_addr;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(PORT);
	HOSTENT* hst;
	if (inet_addr(SERVERADDR) != INADDR_NONE)
		dest_addr.sin_addr.s_addr = inet_addr(SERVERADDR);
	else if (hst = gethostbyname(SERVERADDR))
			// hst->h_addr_list содержит не массив адресов,
			// а массив указателей на адреса
			((unsigned long*)&dest_addr.sin_addr)[0] =
			((unsigned long**)hst->h_addr_list)[0][0];
		else
		{
			printf("Invalid address %s\n", SERVERADDR);
			closesocket(so);
			WSACleanup();
			return;
		}
	if (connect(so, (sockaddr*)&dest_addr, sizeof(dest_addr))) {
		cout << "Connect error" << endl;
		WSACleanup();
		system("pause");
		return;
	}
	
	cout << "Игра со спичками" << endl;
	string str;
	char buf[BUF_SIZE];
	state st;
	int result;
	while (true) {
		result = recv(so, buf, BUF_SIZE, 0);
		buf[result] = '\0';
		st = state::fromJSON(buf);
		cout << "Спичек " << st.count << endl;
		if (st.count < 10) {
			cout << "Вы проиграли" << endl;
			system("pause");
			closesocket(so);
			WSACleanup();
			return;
		}
		cout << "Уберите спички [10, 20]: ";

		while (cin >> str) {
			result = stoi(str);
			if (result < 10 || result>20 || result>st.count)cout << "Вы ничего не поняли, уберите от 10 до 20 спичек включительно, ну и меньше или равно чем есть: ";
			else break;

		}
		st.count -= result;
		str = st.toJSON();
		cout << "Спичек стало " << st.count << endl;
		send(so, str.c_str(), str.size(), 0);
		if (st.count < 10) {
			cout << "Вы выиграли" << endl;
			system("pause");
			closesocket(so);
			WSACleanup();
			return;
		}

		

		
	}
	WSACleanup();
}