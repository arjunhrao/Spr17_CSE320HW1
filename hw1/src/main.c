#include "hw1.h"
#include "debug.h"
#include "info.h"

int main(int argc, char **argv) {

    FILE* in;
    FILE* out;

    //debug("Program has %d args\n", argc);

    /* Note: create a variable to assign the result of validargs */
    char mode = validargs(argc, argv, &in, &out); //int or char?
    int succ = 0;
    if (mode == 0) {
    	USAGE(mode);
    	return EXIT_FAILURE;
    }
    else if ( (mode & 128) == 128) { //help
        USAGE(mode);
        return EXIT_SUCCESS;
    }
    else if ( (mode & 64) == 64) {//if and with 01000000 = 32, then -s --> sub cipher
        //get 5 lsb's as value to pass in for n
        char n = mode & 31;
        //int tempc = 0;
        //char* aptr = Alphabet;
        //int infoaLen = mystrln(aptr);

        if ( (mode & 32) == 32) {//then encrypt
            succ = decr_sc(in, out, n);
            //print out shifted alphabet to stderr using info call
            //info("\nshifted alphabet: %s%s", infoaLen - n;
            //succ = decr_sc(aptr, stderr, )
        }
        else { //decrypt
            succ = encr_sc(in, out, n);
        }

        /*
        for (; tempc < n; tempc++) {
            if (*alpha_ptr==c)
                break;
            alpha_ptr++;
        }
        */
        //need to do print to stderr AFTER encrypt/decrypt. I don't believe argc and argv were changed
        //info(argc, argv);

        //info("\nshifted alphabet: %s", );
        info("\nshift amount: %d", n);
        //info("Program has %d args\n", argc);


    }
    else { //-t --> tutneseUSAGE(mode);
        if ( (mode & 32) == 32) {//then encrypt
            succ = decr_t(in, out);
        }
        else { //decrypt
            succ = encr_t(in, out);

        }
    }





    if (succ)
        return EXIT_SUCCESS;
    return EXIT_SUCCESS;

}
