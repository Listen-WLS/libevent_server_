#include "XThread.h"
#include<thread>
#include<iostream>
#include<event2/event.h>
#include "XTask.h"
#ifdef _WIN32
#else
#include<unistd.h>
#endif
using namespace std;
//启动线程

static void NotifyCB(evutil_socket_t fd, short which,void *arg)//激活线程的回调函数
{
	XThread* t =(XThread*) arg;
	t->Notify(fd, which);

}
void XThread::Notify(evutil_socket_t fd, short which)
{
	//水平触发，只要没有接受完成，就会再次进入
	char buf[2] = { 0 };
#ifdef _WIN32
	int re = recv(fd, buf, 1, 0);
#else
	//linux 中使用管道，不能用recv
	int re = recv(fd, buf, 1, 0);
#endif
	if (re <= 0)
	{
		cout << "rev empty" << endl;
		return;
	}
	cout << id << "Thread" << buf<< endl;
	XTask* task = NULL;
	//获取任务，初始化任务
	tasks_mutex.lock();
	if (tasks.empty()) {
		tasks_mutex.unlock();
		return;
	}
	task = tasks.front();
	tasks.pop_front();
	tasks_mutex.unlock();
	task->Init();
}
void XThread::AddTask(XTask* t)
{
	if (!t)return;
	t->base = this->base;
	tasks_mutex.lock();
	tasks.push_back(t);
	tasks_mutex.unlock();
}

//线程激活
void XThread::Activate()
{
#ifdef _WIN32
	int re = send(this->notify_send_fd, "c",1, 0);
#else
	int re = write(this->notify_send_fd, "c", 1);
#endif
	if (re <= 0)
	{
		cerr << "XThread::Activate() failed" << endl;
	}
}
void XThread::Start()
{
	//创建socket
	Setup();
	//启动线程
	thread th(&XThread::Main, this);
	//断开与主线程链接
	th.detach();
}


//线程入口函数
void XThread::Main()
{
	cout << id << "begin()" << endl;
	event_base_dispatch(base);
	event_base_free(base);
	cout << id << "XThread::Main() end" << endl;

}
bool XThread::Setup() {
	//windows用pair socket linux用管道
#ifdef _WIN32
		//创建一个socket pair 可以互相通信，fds[0]是读,fds[1]写
	evutil_socket_t fds[2];
	if (evutil_socketpair(AF_INET, SOCK_STREAM, 0, fds) < 0) {
		cout << "evutil_socketpair failed" << endl;
		return false;
	}
	//设置非阻塞
	evutil_make_socket_nonblocking(fds[0]);
	evutil_make_socket_nonblocking(fds[1]);

#else
	int fds[2];
	if (pipe(fds))
	{
		cerr << "pip failed!" << endl;
		return false;
	}
#endif
	//读取绑定到event事件中，写入要保存
	notify_send_fd = fds[1];
	//创建libevent上下文(无锁)
	event_config* ev_conf=event_config_new();
	event_config_set_flag(ev_conf, EVENT_BASE_FLAG_NOLOCK);
	this->base = event_base_new_with_config(ev_conf);
	event_config_free(ev_conf);
	if (!base)
	{
		cerr << "event base failed" << endl;
		return false;
	}
	//创建管道监听事件，激活线程执行任务
	event* ev = event_new(base, fds[0], EV_READ | EV_PERSIST, NotifyCB, this);
	event_add(ev, 0);
	return true;
}
XThread::XThread()
{

}
XThread::~XThread()
{

}


