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
//�����߳�

static void NotifyCB(evutil_socket_t fd, short which,void *arg)//�����̵߳Ļص�����
{
	XThread* t =(XThread*) arg;
	t->Notify(fd, which);

}
void XThread::Notify(evutil_socket_t fd, short which)
{
	//ˮƽ������ֻҪû�н�����ɣ��ͻ��ٴν���
	char buf[2] = { 0 };
#ifdef _WIN32
	int re = recv(fd, buf, 1, 0);
#else
	//linux ��ʹ�ùܵ���������recv
	int re = recv(fd, buf, 1, 0);
#endif
	if (re <= 0)
	{
		cout << "rev empty" << endl;
		return;
	}
	cout << id << "Thread" << buf<< endl;
	XTask* task = NULL;
	//��ȡ���񣬳�ʼ������
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

//�̼߳���
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
	//����socket
	Setup();
	//�����߳�
	thread th(&XThread::Main, this);
	//�Ͽ������߳�����
	th.detach();
}


//�߳���ں���
void XThread::Main()
{
	cout << id << "begin()" << endl;
	event_base_dispatch(base);
	event_base_free(base);
	cout << id << "XThread::Main() end" << endl;

}
bool XThread::Setup() {
	//windows��pair socket linux�ùܵ�
#ifdef _WIN32
		//����һ��socket pair ���Ի���ͨ�ţ�fds[0]�Ƕ�,fds[1]д
	evutil_socket_t fds[2];
	if (evutil_socketpair(AF_INET, SOCK_STREAM, 0, fds) < 0) {
		cout << "evutil_socketpair failed" << endl;
		return false;
	}
	//���÷�����
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
	//��ȡ�󶨵�event�¼��У�д��Ҫ����
	notify_send_fd = fds[1];
	//����libevent������(����)
	event_config* ev_conf=event_config_new();
	event_config_set_flag(ev_conf, EVENT_BASE_FLAG_NOLOCK);
	this->base = event_base_new_with_config(ev_conf);
	event_config_free(ev_conf);
	if (!base)
	{
		cerr << "event base failed" << endl;
		return false;
	}
	//�����ܵ������¼��������߳�ִ������
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


