# libevent_server_
基于libevent的C++多线程服务器
</p>基本函数作用：</p>
XThreadPool
</p>bool Init(int threadCount = 10)</p>
</p>  初始化线程</p>
 </p> 创建好线程XThread对象并安装和启动</p>
</p>bool Dispatch(XTask *task)</p>
</p>  分发线程，使用轮询方式</p>
</p>  每个线程处理多个任务</p>
</p>  添加任务</p>
</p>XTask</p>
 </p> 激活线程</p>
 </p> 发送管道消息</p>
</p>XThread</p>
</p>bool Setup();</p>
</p>  安装线程，初始化后libevent的事件</p>
</p>  创建用于线程激活的管道（windows用互相通信的socket socketpair）</p>
</p>  每个线程一个event_base</p>
</p>void Start();</p>
</p>  开始线程的运行</p>
</p>  启动线程，使用c++11的thread</p>
</p>void Main();</p>
</p>  线程函数，libevent的事件循环</p>
</p>void AddTask(XTask *t);</p>
</p>  添加处理任务，一个线程可以同时处理多个任务，他们共用一个event_base</p>
</p> 添加到任务队列</p>
</p>  用了锁，线程安全</p>
</p>void Activate();</p>
</p>  向此线程发出激活的管道消息</p>
</p>  发送一个c</p>
</p>void Notify(evutilsockett fd);</p>
</p>  收到线程池发出的激活消息</p>
</p>  获取待处理任务 并处理</p>
</p>XTask</p>
</p>virtual bool Init()=0;</p>
</p>  任务初始化接口</p>
</p>struct event_base *base;</p>
</p>int sock;</p>
</p>  接口类</p>
