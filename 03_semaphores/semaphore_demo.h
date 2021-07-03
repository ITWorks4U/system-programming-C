/**	system programming in C
 *
 *	How to use semaphores in C.
 *
 *	ITWorks4U
*/

#ifndef SEMAPHORE_DEMO_H
#define SEMAPHORE_DEMO_H

#include <semaphore.h>
sem_t mutex_to_use;							/*	global mutex		*/

void *thread_function(void *arg);			/*	function prototype	*/

#endif