#include "threadpool.h"

#include <cstdio>

//init static members
vector<cTask*> cThreadpool::m_TaskList;
pthread_mutex_t cThreadpool::m_threadMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cThreadpool::m_threadCond = PTHREAD_COND_INITIALIZER;
bool cThreadpool::shutdown=false;


void* cTask::setData(void *data){
    m_data=data;
}

//constructor
cThreadpool::cThreadpool(int threadNum){
    this->m_ThreadNum=threadNum;
    printf("I will creat %d threads.\n",threadNum);
    Creat();
}

int cThreadpool::Creat(){
    pthread_id= new pthread_t[m_ThreadNum];
    for(int i=0;i<m_ThreadNum;i++){
        pthread_create(&pthread_id[i],NULL,TheadFunc,NULL);
    }

    return 0;
}

void* cThreadpool::TheadFunc(void *threadData){
    pthread_t tid=pthread_self();
    for(;;){
        pthread_mutex_lock(&m_threadMutex);
        if(m_TaskList.empty()&&!shutdown)
            pthread_cond_wait(&m_threadCond,&m_threadMutex);

        if(shutdown){
            pthread_mutex_unlock(&m_threadMutex);
            printf("[tid:%lu]   exit\n",tid);
            pthread_exit(NULL);
        }

        printf("[tid:%lu]  run:\n",tid);
        auto iter=m_TaskList.begin();
        cTask* task=*iter;
        m_TaskList.erase(iter);
        pthread_mutex_unlock(&m_threadMutex);

        task->run();
        printf("[tid:%lu]   idle\n",tid);
    }

    return (void*)0;
}

int cThreadpool::AddTask(cTask *task){
    pthread_mutex_lock(&m_threadMutex);
    m_TaskList.push_back(task);
    pthread_mutex_unlock(&m_threadMutex);
    pthread_cond_signal(&m_threadCond);

    return 0;
}

int cThreadpool::stopAll(){
    if(shutdown)
        return -1;
    printf("all pthreads will stop\n");

    shutdown=true;
    pthread_cond_broadcast(&m_threadCond);

    for(int i=0;i<m_ThreadNum;i++)
        pthread_join(pthread_id[i],NULL);
    delete[] pthread_id;
    pthread_id=NULL;

    pthread_mutex_destroy(&m_threadMutex);
    pthread_cond_destroy(&m_threadCond);

    return 0;
}

int cThreadpool::GetTaskSize(){
    return m_TaskList.size();
}