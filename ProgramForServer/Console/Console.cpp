// Console.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <io.h>
#include <string.h>
#include <iostream>

#include<event2/event.h>
#include<event2/bufferevent.h>
#include<event2/buffer.h>
#include<event2/util.h>

#include <windows.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"libevent_core.lib")

int tcp_connect_server(const char* server_ip, int port);
void cmd_msg_cb(int fd, short events, void* arg);
void server_msg_cb(struct bufferevent* bev, void* arg);
void error_cb(struct bufferevent *bev, short event, void *arg);

struct bufferevent* g_bev = nullptr;

int main(int argc, char* argv[])
{
	for (int i = 0; i < argc; ++i)
	{
		std::string s = argv[i];
	}
	if (argc < 3)
	{
		printf("please input 2 parameter\n");
		system("pause");
		return -1;
	}


	//两个参数依次是服务器端的IP地址、端口号
	std::string s = argv[1];
	int iport = atoi(argv[2]);
	int sockfd = tcp_connect_server(argv[1], atoi(argv[2]));
	if (sockfd == -1)
	{
		perror("tcp_connect error ");
		system("pause");
		return -1;
	}

	printf("connect to server successful\n");

	struct event_base* base = event_base_new();

	g_bev = bufferevent_socket_new(base, sockfd, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(g_bev, server_msg_cb, NULL, error_cb, NULL);
	bufferevent_enable(g_bev, EV_READ | EV_PERSIST);

	event_base_dispatch(base);

	printf("finished \n");
	system("pause");
	return 0;
}

void cmd_msg_cb()
{
	char msg[1024];

	sprintf_s(msg, "hello , I'm Console!");

	if (g_bev)
	{
		//把终端的消息发送给服务器端
		bufferevent_write(g_bev, msg, sizeof(msg));
	}
}


void server_msg_cb(struct bufferevent* bev, void* arg)
{
	char msg[1024];

	size_t len = bufferevent_read(bev, msg, sizeof(msg));
	msg[len] = '\0';

	printf("recv %s from server\n", msg);
}


void error_cb(struct bufferevent *bev, short event, void *arg)
{

	if (event & BEV_EVENT_EOF)
		printf("connection closed\n");
	else if (event & BEV_EVENT_ERROR)
		printf("some other error\n");

	//这将自动close套接字和free读写缓冲区
	bufferevent_free(bev);
}

typedef struct sockaddr SA;
int tcp_connect_server(const char* server_ip, int port)
{
	int sockfd, status, save_errno;
	struct sockaddr_in server_addr;

	memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.S_un.S_addr = inet_addr(server_ip);
	if (status == 0) //the server_ip is not valid value
	{
		errno = EINVAL;
		return -1;
	}

	sockfd = ::socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		return sockfd;


	status = ::connect(sockfd, (SA*)&server_addr, sizeof(server_addr));

	if (status == -1)
	{
		save_errno = errno;
		::close(sockfd);
		errno = save_errno; //the close may be error
		return -1;
	}

	evutil_make_socket_nonblocking(sockfd);

	return sockfd;
}