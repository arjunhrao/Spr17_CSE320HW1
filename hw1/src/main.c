#include "hw1.h"

int main(int argc, char **argv) {

    FILE* in;
    FILE* out;

    /* Note: create a variable to assign the result of validargs */
    int mode = validargs(argc, argv, &in, &out); //int or char?
    if (mode == 0) {
    	//USAGE(mode);
    	return EXIT_FAILURE;
    }
    else
    	return EXIT_SUCCESS;
}
