#define _WIN32_WINNT 0x501

#include <iostream>
#include <sstream>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <document.h>
#include <prettywriter.h>

#pragma comment(lib, "Ws2_32.lib")

#define SERVERADDR "127.0.0.1"
#define PORT "228"
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


void main()
{
    setlocale(LC_ALL, "rus");
    cout << "Cервер запущен" << endl;
    WSADATA wsaData; 
    if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
        cout << "Error WSAStartup" << endl;
        system("pause");
        return;
    }
    struct addrinfo* addr = NULL; 
    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET; 
    hints.ai_socktype = SOCK_STREAM; 
    hints.ai_protocol = IPPROTO_TCP; 
    hints.ai_flags = AI_PASSIVE; 
    getaddrinfo(SERVERADDR, PORT, &hints, &addr);

    int listen_socket = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (listen_socket < 0) {
        cout << "Error socket" << endl;
        WSACleanup();
        system("pause");
        return;
    }
    if (bind(listen_socket, addr->ai_addr, (int)addr->ai_addrlen)) {
        cout<< "Error bind" <<endl;
        closesocket(listen_socket); 
        WSACleanup();
        return;
    }

    if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR) {
        cout << "Error listen" << endl;
        closesocket(listen_socket);
        WSACleanup();
        return;
    }
    char buf[BUF_SIZE];
    cout << "Ожидание первого клиента" << endl;
    int first = accept(listen_socket, NULL, NULL);
    cout << "Ожидание второго клиента" << endl;
    int second = accept(listen_socket, NULL, NULL);
    cout << "Клиенты подключились" << endl;
    int result;

    state st;
    string str;
    while (true) {
        str = st.toJSON();
        send(first, str.c_str(), str.size(), 0);
        if (st.count < 10) {
            cout << "Победил второй" << endl;
            break;
        }
        result = recv(first, buf, BUF_SIZE, 0);
        buf[result] = '\0';
        st = state::fromJSON(buf);
        str = st.toJSON();
        send(second, str.c_str(), str.size(), 0);
        if (st.count < 10) {
            cout << "Победил первый" << endl;
            break;
        }
        result = recv(second, buf, BUF_SIZE, 0);
        buf[result] = '\0';
        st = state::fromJSON(buf);
        

    }
   
    WSACleanup();
    system("pause");
    return;
}
