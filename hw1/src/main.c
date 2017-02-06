#include "hw1.h"
#include "debug.h"
#include "info.h"

int main(int argc, char **argv) {

    FILE* in;
    FILE* out;
    int op = 0;

    //debug("Program has %d args\n", argc);

    /* Note: create a variable to assign the result of validargs */
    char mode = validargs(argc, argv, &in, &out); //int or char?
    int succ = 0;

    //debug("\n mode is: %d \t", mode);
    if (mode == 0) {
    	USAGE(EXIT_FAILURE);
    	return EXIT_FAILURE;
    }
    else if ( (mode & 128) == 128) { //help
        USAGE(EXIT_SUCCESS);
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
            op = 1;
            //print out shifted alphabet to stderr using info call
            //info("\nshifted alphabet: %s%s", infoaLen - n;
            //succ = decr_sc(aptr, stderr, )
        }
        else { //decrypt
            succ = encr_sc(in, out, n);
            op = 2;
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
        //info("\nshift amount: %d", n");
        info("\nshift amount: %d", n);
        info("\ninput file: %s", *(argv+3));
        info("\noutput file: %s", *(argv+4));
        if (op == 1)
            info("\noperation: %s", "decryption");
        else if (op == 2)
            info("\noperation: %s", "encryption");


        //info("Program has %d args\n", argc);


    }
    else { //-t --> tutneseUSAGE(mode);
        if ( (mode & 32) == 32) {//then encrypt
            succ = decr_t(in, out);
            if (succ == 127)
                return EXIT_FAILURE;
        }
        else { //decrypt
            succ = encr_t(in, out);

        }
    }




    debug("\n mode is: %d \t", mode);
    if (succ == 1)
        return EXIT_SUCCESS;
    return EXIT_SUCCESS;

}
