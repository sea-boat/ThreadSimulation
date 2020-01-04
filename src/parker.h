#include <sys/time.h>
#include <pthread.h>
#include <errno.h>

class Parker {
private:
	pthread_mutex_t _mutex;
	pthread_cond_t _cond;
public:
	Parker();
	void park(long millis);
	void unpark();
};