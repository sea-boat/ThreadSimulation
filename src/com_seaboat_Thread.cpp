#include <iostream>
#include <pthread.h>
#include "com_seaboat_Thread.h"
#include <sched.h>
#include <sys/time.h>

using namespace std;

pthread_mutex_t _mutex;
pthread_cond_t _cond;

class JavaThread
{
private:
    JavaVM *jvm;
    jobject jThreadObjectRef;
public:
    ~JavaThread();
    JavaThread(JNIEnv *env, jobject jThreadObject);
    void execRunMethod();
};

JavaThread::JavaThread(JNIEnv *env, jobject jThreadObject) {
    env->GetJavaVM(&(this->jvm));
    this->jThreadObjectRef = env->NewGlobalRef(jThreadObject);
}

JavaThread::~JavaThread() {
    jvm->DetachCurrentThread(); 
}

void JavaThread::execRunMethod() {
    JNIEnv *env;
    if (jvm->AttachCurrentThread((void **)&env, NULL) != 0)
    {
        std::cout << "Failed to attach" << std::endl;
    }
    jclass cls = env->GetObjectClass(jThreadObjectRef);
    jmethodID runId = env->GetMethodID(cls, "run", "()V");
    if (runId != nullptr) {
        env->CallVoidMethod(jThreadObjectRef, runId);
    } else {         
        cout << "No run method found in the Thread object!!" << endl;
    }
    env->DeleteGlobalRef(jThreadObjectRef); 
}

class OSThread
{
private:
    JavaThread* javaThread;
public:
    OSThread(JavaThread* javaThread);
    void call_os_thread();
    static void* thread_entry_function(void *args);
};

OSThread::OSThread(JavaThread* javaThread){
    this->javaThread = javaThread;
}

void OSThread::call_os_thread(){
    pthread_t tid;
    pthread_attr_t Attr;
    pthread_attr_init(&Attr);
    pthread_attr_setdetachstate(&Attr, PTHREAD_CREATE_DETACHED);
    std::cout << "creating linux thread!" << endl;
    if(pthread_create(&tid, &Attr, &OSThread::thread_entry_function, this->javaThread) != 0)
    {
        std::cout << "Error creating thread\n" <<endl;
        return;
    }
    std::cout << "Started a linux thread!" << endl;
    pthread_attr_destroy(&Attr);
}

void* OSThread::thread_entry_function(void *args)
{
    JavaThread *javaThread = (JavaThread*)args;
    javaThread->execRunMethod();
    delete javaThread;
}


JNIEXPORT void JNICALL Java_com_seaboat_Thread_start0(JNIEnv *env, jobject jThreadObject)
{
    std::cout << "creating a JavaThread object!" << endl;
    JavaThread* javaThread = new JavaThread(env, jThreadObject);
    std::cout << "creating a OSThread object!" << endl;
    OSThread osThread(javaThread);
    osThread.call_os_thread();
    return;
}

JNIEXPORT void JNICALL Java_com_seaboat_Thread_yield(JNIEnv *env, jobject jThreadObject)
{
    std::cout << "calling yield operation!" << endl;
    sched_yield();
    return;
}

JNIEXPORT void JNICALL Java_com_seaboat_Thread_sleep(JNIEnv *env, jclass jc, jlong millis)
{
    std::cout << "calling sleep operation!" << endl;
    if(millis ==0){
      sched_yield();
    }else{
      struct timespec outtime;
      struct timeval now;
      gettimeofday(&now, NULL);
      outtime.tv_sec = now.tv_sec + 5;
      outtime.tv_nsec = (now.tv_usec+millis) * 1000;
      int status = pthread_cond_timedwait(&_cond, &_mutex, &outtime);
    }
    return;
}