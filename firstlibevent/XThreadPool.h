#pragma once
#include<vector>
#include"XTask.h"
class XThread;
//class XTask;
class XThreadPool
{
public:
	//单件模式
	static XThreadPool* Get()
	{
		static XThreadPool p;
		return &p;
	}
	//初始化所有线程
	void Init(int threadCount);
	//分发线程
	void Dispatch(XTask* task);
private:
	//nums of thread
	int threadCount = 0;
	int lastThread = -1;
	//线程池线程
	std::vector<XThread*>threads;
	XThreadPool() {};
};

