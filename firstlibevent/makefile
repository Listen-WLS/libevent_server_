test_thread_pool:test_thread_pool.cpp XThreadPool.cpp XThread.cpp XFtpserverCMD.cpp
	g++ $^ -o $@ -levent -lpthread
	./$@
clean:
	rm -rf test_thread_pool
	rm -rf *.o
