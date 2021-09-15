#pragma once
#include<event2/event.h>
#include<list>
#include<mutex>
#include"XTask.h"
//class XTask;
class XThread
{
public:
	//start thread
	void Start();
	//thread entrance function
	void Main();
	//��װ�̣߳���ʼ��event_base�͹ܵ������¼����ڼ����߳�
	bool Setup();
	//�յ����̷߳��ļ�����Ϣ(�̳߳طַ�)
	void Notify(evutil_socket_t fd, short which);
	//�̼߳���
	void Activate();
	//��Ӵ��������,һ���߳�ͬʱ���Դ��������񣬹���һ��event_base
	void AddTask(XTask* t);
	int id;
	XThread();
	~XThread();
private:
	int notify_send_fd = 0;
	struct event_base* base = 0;
	//�����б�
	std::list<XTask*>tasks;
	//�̰߳�ȫ ����
	std::mutex  tasks_mutex;
	
};

