#include <iostream>
#include <pthread.h>
#include "com_seaboat_Thread.h"
#include <sched.h>

using namespace std;

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

JNIEXPORT void JNICALL Java_com_seaboat_Thread_yield0(JNIEnv *env, jobject jThreadObject)
{
    sched_yield();
}