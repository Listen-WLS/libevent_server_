# libevent_server_
基于libevent的C++多线程服务器
XThreadPool
bool Init(int threadCount = 10)
  初始化线程
  创建好线程XThread对象并安装和启动
bool Dispatch(XTask *task)
  分发线程，使用轮询方式
  每个线程处理多个任务
  添加任务
XTask
  激活线程
  发送管道消息
XThread
bool Setup();
  安装线程，初始化后libevent的事件
  创建用于线程激活的管道（windows用互相通信的socket socketpair）
  每个线程一个event_base
void Start();
  开始线程的运行
  启动线程，使用c++11的thread
void Main();
  线程函数，libevent的事件循环
void AddTask(XTask *t);
  添加处理任务，一个线程可以同时处理多个任务，他们共用一个event_base
  添加到任务队列
  用了锁，线程安全
void Activate();
  向此线程发出激活的管道消息
  发送一个c
void Notify(evutilsockett fd);
  收到线程池发出的激活消息
  获取待处理任务 并处理
XTask
virtual bool Init()=0;
  任务初始化接口
struct event_base *base;
int sock;
  接口类
