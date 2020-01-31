#include "auto_mode.h"
#include "interactive.h"

// #define INTERACTIVE

int main(int argc, char *argv[]) {
	#ifdef INTERACTIVE
	#warning INTERACTIVE
	interactiveMain();
	#else
	#warning AUTOMODE
	handle_automode_args(argc,argv);
	#endif
	return 0;
}

