#include "XFtpserverCMD.h"
#include <event2/event.h>
#include<event2/bufferevent.h>
#include<iostream>
#include<cstring>
#include<string>
using namespace std;

void EventCB(struct bufferevent *bev,short what,void *arg)
{
	XFtpserverCMD* cmd = (XFtpserverCMD*)arg;
	//如果对方网络断掉，或者机器死机可能收不到数据
	if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR| BEV_EVENT_TIMEOUT))
	{
		cout << "BEV_EVENT_EOF | BEV_EVENT_ERROR|BEV_EVENT_TIMEOUT" << endl;
		bufferevent_free(bev);
		delete cmd;
	}
}

//子线程XThread
static void readcb(bufferevent* bev, void* arg)
{
	XFtpserverCMD* cmd = (XFtpserverCMD*)arg;
	char data[1024] = { 0 };
	for (;;)
	{
		int len = bufferevent_read(bev, data, sizeof(data) - 1);
		if (len <= 0)break;
		data[len] = '\0';
		cout << data << flush;
		if (strstr(data, "quit"))
		{
			bufferevent_free(bev);
			delete cmd;
			break;

		}
	}
}

//初始化任务,运行在子线程
bool XFtpserverCMD::Init()
{
	//监听socket bufferevent
	//base socket
	cout << "XFtpserverCMD::Init()" << endl;
	// base socket
	bufferevent*bev= bufferevent_socket_new(base, sock, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(bev,readcb, 0, EventCB, this);
	bufferevent_enable(bev, EV_READ | EV_WRITE);
	//添加超时
	timeval rt = { 10,0 };
	bufferevent_set_timeouts(bev, &rt, 0);
	return true;
}

XFtpserverCMD::XFtpserverCMD()
{

}
XFtpserverCMD::~XFtpserverCMD()
{

}
