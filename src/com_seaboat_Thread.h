/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_seaboat_Thread */

#ifndef _Included_com_seaboat_Thread
#define _Included_com_seaboat_Thread
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_seaboat_Thread
 * Method:    yield
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_seaboat_Thread_yield(JNIEnv*, jobject);

/*
 * Class:     com_seaboat_Thread
 * Method:    start0
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_seaboat_Thread_start0(JNIEnv*, jobject);

/*
 * Class:     com_seaboat_Thread
 * Method:    sleep
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_seaboat_Thread_sleep(JNIEnv*, jclass, jlong,
		jint);

/*
 * Class:     com_seaboat_Thread
 * Method:    monitorEnter
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_seaboat_Thread_monitorEnter(JNIEnv*, jclass);

/*
 * Class:     com_seaboat_Thread
 * Method:    monitorExit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_seaboat_Thread_monitorExit(JNIEnv*, jclass);

/*
 * Class:     com_seaboat_Thread
 * Method:    isInterrupted
 * Signature: 
 */
JNIEXPORT bool JNICALL Java_com_seaboat_Thread_isInterrupted(JNIEnv*, jobject);

/*
 * Class:     com_seaboat_Thread
 * Method:    interrupt
 * Signature: 
 */
JNIEXPORT void JNICALL Java_com_seaboat_Thread_interrupt(JNIEnv*, jobject);

#ifdef __cplusplus
}
#endif
#endif
