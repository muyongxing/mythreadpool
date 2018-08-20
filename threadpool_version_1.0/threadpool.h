#ifndef __THREAD_POOL_H
#define __THREAD_POOL_H

#include <vector>
#include <string>
#include <pthread.h>

using namespace std;

class cTask
{
  protected:
    string m_TaskName;
    void *m_data;
  public:
    cTask() = default;
    cTask(string taskName) : m_TaskName(taskName),m_data(NULL){};
    virtual void run() = 0;
    void *setData(void *data);

    virtual ~cTask() {}
};

class cThreadpool
{
  private:
    static vector<cTask *> m_TaskList;
    int m_ThreadNum;
    pthread_t *pthread_id;
    static bool shutdown;

    static pthread_mutex_t m_threadMutex;
    static pthread_cond_t m_threadCond;

  protected:
    static void* TheadFunc(void *threadData);
    static int MoveToIdle(pthread_t tid);
    static int MoveToBusy(pthread_t tid);
    int Creat();

    public:
    cThreadpool(int threadNum);//f
    int AddTask(cTask *task);
    int stopAll();
    int GetTaskSize();
};

#endif
