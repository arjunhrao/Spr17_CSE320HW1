#include "hw1.h"

// For your helper functions (you may add additional files also)
// DO NOT define a main function here!
int mystrcmp(const char* a, const char* b) {
	//want to basically check the chars at each pointer to make sure they're the same
	//and advance if they are. Stop if one or both are null
	const unsigned char *stra = (const unsigned char *)a;
    const unsigned char *strb = (const unsigned char *)b;

    while (*strb != '\0') {
        if (*stra == '\0') return  -1; //then strb is longer
        if (*stra > *strb)   return  1;//then stra has a char that differs and is greater
        if (*stra < *strb)   return -1;//strb has a char that differs and is greater
        stra++;strb++;//increment pointers' addresses to next char
    }
    if (*stra != '\0') return 1; //then stra is longer
    return 0;
}

int mystrln(const char *aptr) {
	int i = 0;
	for (; *aptr != '\0'; aptr++) {
				i++;
	}
	return i;
}

char validargs(int argc, char** argv, FILE** in, FILE** out) {
	char ret = 0;

	/* code here */
	if (argc < 2) //mode is 0
		return ret;

	char** ptr = argv;	//ptr points to argv, ptr = address of argv

	int counter = 0;

	ptr++;
	counter++;
	if (mystrcmp(*ptr, "-h") == 0) {
		//The most significant bit (msb) of the mode byte is set to 1,
		//if the -h flag has been passed. No other bits should be set.
		ret = ret | 128;//or with 10000000 to set msb to 1, rest to what they were
		return ret;//Remember, we don't care what else exists in this case.
	}

	//if we haven't returned by now, we must have 5 or 6 arguments for validity.
	if (argc > 6 || argc < 5)
		return ret;	//return 0


	if (mystrcmp(*ptr, "-s") == 0){//sub cypher
		ret = ret | 64;
		if (argc == 5) {//in this case n was not provided. Default to 320.
			char* alpha_ptr2 = Alphabet;
			int alphabetLen2 = mystrln(alpha_ptr2);
			long res2 = 320;
			int temp2 = res2%alphabetLen2;
			ret = ret & 224;//and with 11100000
			ret = ret | temp2;//set last 5 bits
		}
	}
	else if (mystrcmp(*ptr, "-t") == 0)	{//it can't be 6 arguments if it's t though.
		if (argc == 6)
			return 0;
		ret = ret & 191;//and with 10111111
		ret = ret | 20;//or with 20 so non-zero. Could or with any non-zero thing
	}
	else return 0; // if it's not h or s or t, return 0

	ptr++;
	if (mystrcmp(*ptr, "-d") == 0)
		ret = ret | 32; //or with 00100000
	else if (mystrcmp(*ptr, "-e") == 0)
		ret = ret & 223; //and with 11011111
	else return 0; //if it's not d or e, return 0 (invalid)


	if (argc == 6) {//then we have -s and and 6th arg n
		//int j = 0;//char temp = 1;
		ptr += 3;//sets ptr to the n value
		char *endptr;
		long result = strtol(*ptr, &endptr, 10); //in standard c library
		printf("%s", endptr);
		if (*endptr != '\0')//endptr points to first nonnumeric character
			return 0;//if not null terminator, nonnumeric chars --> invalid n
		//else it is a valid 'n' and bits must be set
		else {
			char* alpha_ptr = Alphabet;
			int alphabetLen = mystrln(alpha_ptr);
			//alphabetLen is 0 here for some reason... so is result above
			int temp = result%alphabetLen;//note that result == n now if n is an integer
			//temp should be btwn 0 and 31, so can just set lsb's to it
			ret = ret & 224;//and with 11100000
			ret = ret | temp;//set last 5 bits
		}
		ptr = ptr - 3; //put ptr back where it was
	}
	ptr++; //now ptr is at infile
	//if we reach here, need to do file stuff.



		/*
		while (temp != '\0') {
			temp = *(*(ptr+3)+j); //this will give the first character of "n"
			if temp
		}*/




	/*
	for (; counter < argc; counter++) {
		printf("%s", *ptr); //this needs an argument of char *
		//printf("\n");
		//printf("%d", sizeof(*ptr));
		if (counter == 1) {//let's examine the argument afte program_name.
			if (*ptr == "-h")
				return ret;	//return 0. Remember, we don't care what else exists in this case.
			if (*ptr)

			printf("%s", *ptr);

		}
			//if (*ptr == smth)
		ptr++;
	}
	*/

	//if there are at least 2 arguments and the second one is -h, then do the help stuff

	//first argument must be bin/hw1

	//char** ptr2 = argv;

	//if (*(ptr2 + 0) != "bin/hw1")
		//return 0;

	//if (*(ptr + 1) == "-h")
	//	return 0;


	//If no -h, then 5 or 6 arguments is valid



	return ret;
}


