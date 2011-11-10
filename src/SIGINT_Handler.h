#ifndef SIGINT_HANDLER_H_
#define SIGINT_HANDLER_H_

#include <signal.h>
#include <assert.h>

#include "EventHandler.h"

class SIGINT_Handler : public EventHandler {

	private:
		sig_atomic_t gracefulQuit;

	public:

		SIGINT_Handler () {
			this->gracefulQuit = 0;
		}

		virtual int handleSignal ( int signum ) {
			assert ( signum == SIGINT );
			this->gracefulQuit = 1;
			return 0;
		}

		sig_atomic_t getGracefulQuit () {
			return this->gracefulQuit;
		}

};

#endif /* SIGINT_HANDLER_H_ */
