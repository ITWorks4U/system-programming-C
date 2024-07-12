/**	system programming in C
 *
 *	Scanning the hardware components of your system.
 *	Every second your screen is going to update.
 *
 *	since C is hardware based, you can't access directly on your CPU, RAM module, etc. ...in the usual way
 *	your OS (operating system) gives you the information about your system
 *
 *	by reading the CPU take a look to: "/proc/cpuinfo" or "/proc/loadavg"
 *	by reading RAM use sysinfo (it's a library, function and also a type to use) you can receive your RAM informations and much more
 *
 *	by the way, in "/proc" you're also welcome to read much more, which is not going to show here
 *
 *	ITWorks4U
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <signal.h>

#define	BUFF			200
#define	CLEAR_SCREEN	"\033[2J\033[1;1H"
#define	DESCRIPTION		"Press \"CTRL + C\" to exit.\n\n"

/*	required for signal handler	*/
static bool on_continue = true;

void convert_to_timestamp(unsigned long full_seconds) {
	int hours = full_seconds / 3600;
	int minutes = (full_seconds - (3600 * hours)) / 60;
	int seconds = full_seconds % 60;

	printf(" uptime since: %d hours, %d minutes, %d seconds\n", hours, minutes, seconds);
}

void print_cpu_informations(void) {
	bool on_success = true;
	FILE *cpuinfo = NULL;
	
	if((cpuinfo = fopen("/proc/cpuinfo", "r")) == NULL) {
		perror("fopen()");
		on_success = false;
	}

	if (on_success) {
		char *arg = NULL;
		size_t size = 0;

		bool freq_found = false, nbr_cores_found = false, model_name_found = false,
		cache_size_found = false, vendor_id_found = false;

		while(getdelim(&arg, &size, 0, cpuinfo) != -1) {
			if (strstr(arg, "vendor_id") != NULL) {
				printf("%s\n", arg);
			}


			if (freq_found && nbr_cores_found && model_name_found && cache_size_found && vendor_id_found) {
				break;
			}
		}

		free(arg);
		arg = NULL;

		fclose(cpuinfo);
		cpuinfo = NULL;
	}
}

void print_cpu_load(void) {
	char FileBuffer[BUFF];
	float load;
	int cpu_avg;

	if((cpu_avg = open("/proc/loadavg", O_RDONLY)) < 0) {
		perror("open()");
		exit(EXIT_FAILURE);
	}

	read(cpu_avg, FileBuffer, sizeof(FileBuffer) - 1);
	sscanf(FileBuffer, "%f", &load);
	close(cpu_avg);

	printf(" CPU load: %d %%\n", (int)(load * 100));
}

static double convert_to_gigabytes(unsigned long data) {
	return ((double)data / (1024*1024*1024));
}

void signal_handler(int signum) {
	// printf("received signal: %d <=> %s\n", signum, strsignal(signum));
	// puts("terminating application...");

	printf(CLEAR_SCREEN);
	on_continue = false;

	/*	in case of your application has been compiled with optimized flag and on_continue is never going to be toggle	*/
	// exit(EXIT_SUCCESS);
}

int main(void) {
	/*	in sysinfo almost all properties receives data in bytes only	*/
	struct sysinfo sys;
	signal(SIGINT, signal_handler);

	/*	detailed informations about your CPU(s) core(s)	*/
	// print_cpu_informations();

	/*	updating your output every 1 second	*/
	while(on_continue) {
		printf(CLEAR_SCREEN);
		printf(DESCRIPTION);

		print_cpu_load();

		if (sysinfo(&sys) < 0) {
			perror("sysinfo()");
			return EXIT_FAILURE;
		}

		convert_to_timestamp(sys.uptime);

		printf(" 1, 5, 15 minute RAM load averages:  %lu, %lu, %lu\n", sys.loads[0], sys.loads[1], sys.loads[2]);
		printf(" total amount of RAM: %.3lf GB\n free RAM left: %.3lf GB\n amount of shared RAM: %.3lf GB\n", convert_to_gigabytes(sys.totalram), convert_to_gigabytes(sys.freeram), convert_to_gigabytes(sys.sharedram));
		printf(" memory used by buffers: %.3lf GB\n total swap space available: %.3lf\n", convert_to_gigabytes(sys.bufferram), convert_to_gigabytes(sys.totalswap));
		printf(" swap space left: %.3lf\n #of current processes: %u\n", convert_to_gigabytes(sys.freeswap), sys.procs);

		sleep(1);
	}

	return EXIT_SUCCESS;
}