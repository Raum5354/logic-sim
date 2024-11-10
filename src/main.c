#include <stdio.h>
#include "ui.h"

int main(int argc, char *argv[]) {
	printf("Starting Logic Sim: Digital Circuit Simulator\n");

    // Initialise the GUI
    if (initialize_ui() != 0) {
        printf("Failed to initialize UI.\n");
        return 1;
    }

    return 0;
}
