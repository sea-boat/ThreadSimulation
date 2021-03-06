#include <iostream>
#include <pthread.h>
#include "com_seaboat_Thread.h"
#include <sched.h>
#include <sys/time.h>
#include "parker.h"
#include <map>

using namespace std;

class Monitor {
public:
	pthread_mutex_t _mutex;
	Monitor();
};

Monitor::Monitor() {
	pthread_mutex_init(&_mutex, NULL);
}

class JavaThread {
public:
	JavaVM *jvm;
	jobject jThreadObjectRef;
	Parker parker;
	bool interruptState = false;
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
	if (jvm->AttachCurrentThread((void**) &env, NULL) != 0) {
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

class OSThread {
private:
	JavaThread *javaThread;
public:
	OSThread(JavaThread *javaThread);
	void call_os_thread();
	static void* thread_entry_function(void *args);
};

OSThread::OSThread(JavaThread *javaThread) {
	this->javaThread = javaThread;
}

void OSThread::call_os_thread() {
	pthread_t tid;
	pthread_attr_t Attr;
	pthread_attr_init(&Attr);
	pthread_attr_setdetachstate(&Attr, PTHREAD_CREATE_DETACHED);
	std::cout << "creating linux thread!" << endl;
	if (pthread_create(&tid, &Attr, &OSThread::thread_entry_function,
			this->javaThread) != 0) {
		std::cout << "Error creating thread\n" << endl;
		return;
	}
	std::cout << "Started a linux thread! tid=" << tid << endl;
	pthread_attr_destroy(&Attr);
}

void* OSThread::thread_entry_function(void *args) {
	JavaThread *javaThread = (JavaThread*) args;
	javaThread->execRunMethod();
	delete javaThread;
	return NULL;
}

map<jint, JavaThread*> threads;

JNIEXPORT void JNICALL Java_com_seaboat_Thread_start0(JNIEnv *env,
		jobject jThreadObject) {
	std::cout << "creating a JavaThread object!" << endl;
	JavaThread *javaThread = new JavaThread(env, jThreadObject);

	//将新线程保存到map中，方便后面根据threadId来获取javaThread
	jclass cls = env->GetObjectClass(javaThread->jThreadObjectRef);
	jfieldID fID = env->GetFieldID(cls, "threadId", "I");
	jint threadId = env->GetIntField(javaThread->jThreadObjectRef, fID);
	threads.insert(map<jint, JavaThread*>::value_type(threadId, javaThread));

	std::cout << "threadId = " << threadId << endl;
	std::cout << "creating a OSThread object!" << endl;
	OSThread osThread(javaThread);
	osThread.call_os_thread();
	return;
}

JNIEXPORT void JNICALL Java_com_seaboat_Thread_yield(JNIEnv *env,
		jobject jThreadObject) {
	std::cout << "calling yield operation!" << endl;
	sched_yield();
	return;
}

JNIEXPORT void JNICALL Java_com_seaboat_Thread_sleep(JNIEnv *env, jclass jc,
		jlong millis, jint threadId) {
	std::cout << "calling sleep operation!" << endl;
	if (millis == 0) {
		sched_yield();
	} else {
		//主线程
		if (threadId == 0) {
			Parker parker;
			parker.park(millis);
			return;
		}
		JavaThread *javaThread = threads.find(threadId)->second;
		javaThread->parker.park(millis);
	}
	return;
}

//为方便起见，这里假设所有线程都使用同一个monitor，实际JDK中由synchronized指定。
Monitor monitor;

JNIEXPORT void JNICALL Java_com_seaboat_Thread_monitorEnter(JNIEnv *env,
		jclass jc) {
	pthread_mutex_lock(&(monitor._mutex));
	std::cout << "monitorEnter tid=" << pthread_self() << endl;
	return;
}

JNIEXPORT void JNICALL Java_com_seaboat_Thread_monitorExit(JNIEnv *env,
		jclass jc) {
	pthread_mutex_unlock(&(monitor._mutex));
	std::cout << "monitorExit tid=" << pthread_self() << endl;
	return;
}

JNIEXPORT bool JNICALL Java_com_seaboat_Thread_isInterrupted(JNIEnv *env,
		jobject jThreadObject) {
	std::cout << "getting interrupt flag..." << endl;
	jclass cls = env->GetObjectClass(jThreadObject);
	jfieldID fID = env->GetFieldID(cls, "threadId", "I");
	jint threadId = env->GetIntField(jThreadObject, fID);
	JavaThread *javaThread = threads.find(threadId)->second;
	return javaThread->interruptState;
}

JNIEXPORT void JNICALL Java_com_seaboat_Thread_interrupt(JNIEnv *env,
		jobject jThreadObject) {
	std::cout << "interrupt..." << endl;
	jclass cls = env->GetObjectClass(jThreadObject);
	jfieldID fID = env->GetFieldID(cls, "threadId", "I");
	jint threadId = env->GetIntField(jThreadObject, fID);
	JavaThread *javaThread = threads.find(threadId)->second;
	javaThread->interruptState = true;
	javaThread->parker.unpark();
	return;
}

