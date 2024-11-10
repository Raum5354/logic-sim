#include <stdio.h>

int main(int argc, char *argv[]) {
	printf("Starting EduLogic: Digital Circuit Simulator\n");

    // Initialize the GUI
    if (initialize_ui() != 0) {
        printf("Failed to initialize UI.\n");
        return 1;
    }

    return 0;
}
