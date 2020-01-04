#include "parker.h"
#include <iostream>

using namespace std;

Parker::Parker() {
	pthread_mutex_init(&_mutex, NULL);
	pthread_cond_init(&_cond, NULL);
}

void Parker::park(long millis) {
	struct timespec ts;
	struct timeval now;
	int status = pthread_mutex_lock(&_mutex);
	gettimeofday(&now, NULL);
	ts.tv_sec = time(NULL) + millis / 1000;
	ts.tv_nsec = now.tv_usec * 1000 + 1000 * 1000 * (millis % 1000);
	ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
	ts.tv_nsec %= (1000 * 1000 * 1000);
	status = pthread_cond_timedwait(&_cond, &_mutex, &ts);
	if (status == 0) {
		// TODO: singaled
	}
	else if (status == ETIMEDOUT) {
		// TODO: Time out.
	}
	status = pthread_mutex_unlock(&_mutex);
}

void Parker::unpark() {
	int status = pthread_mutex_lock(&_mutex);
	status = pthread_cond_signal(&_cond);
	status = pthread_mutex_unlock(&_mutex);
}