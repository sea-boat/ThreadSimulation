#include <iostream>
#include <pthread.h>
#include "com_seaboat_Thread.h"

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
public:
    void call_os_thread(JavaThread* javaThread);
    static void* thread_entry_function(void *args);
};

 void OSThread::call_os_thread(JavaThread *javaThread){
    pthread_t tid;
    std::cout << "creating linux thread!" << endl;
    if(pthread_create(&tid, NULL, &OSThread::thread_entry_function, (void *)javaThread) != 0)
    {
        std::cout << "Error creating thread\n" <<endl;
        return;
    }
    pthread_join(tid,NULL);
}

void* OSThread::thread_entry_function(void *args)
{
    JavaThread *javaThread = (JavaThread*)args;
    javaThread->execRunMethod();
    delete javaThread;
    return NULL;
}

JNIEXPORT void JNICALL Java_com_seaboat_Thread_start0(JNIEnv *env, jobject jThreadObject)
{
    std::cout << "creating a JavaThread object!" << endl;
    JavaThread* javaThread = new JavaThread(env, jThreadObject);
    std::cout << "creating a OSThread object!" << endl;
    OSThread osThread;
    osThread.call_os_thread(javaThread);
    std::cout << "Started a linux thread!" << endl;
    return;
}