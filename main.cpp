#include <iostream>
#include <unistd.h>
#include <string>
#include "threadpool.h"
using namespace std;

class myTask:public cTask{
    void run(){
        printf("%s\n",(char*)m_data);
        int x=rand()%4 +1;
        sleep(x);
    }
};

int main(){
    myTask taskobj;
    char stmp[]="hello";
    taskobj.setData((void*)stmp);
    cThreadpool m_threadpool(5);

    for(int i=0;i<10;i++)
        m_threadpool.AddTask(&taskobj);
    
    for(;;){
        printf("threr are still %d task need to handle \n",m_threadpool.GetTaskSize());
        if(m_threadpool.GetTaskSize()==0){
            printf("stop all\n");
            m_threadpool.stopAll();
            exit(0);
        }
        sleep(2);
        printf("2 sec later..\n");
    }
    return 0;
}