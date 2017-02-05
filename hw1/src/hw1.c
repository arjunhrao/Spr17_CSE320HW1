#include "hw1.h"

// For your helper functions (you may add additional files also)
// DO NOT define a main function here!
int alphaLen = 0;
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
			int alphabetLen2 = mystrln(alpha_ptr2); alphaLen = alphabetLen2;
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
	if (mystrcmp(*ptr, "-e") == 0)
		ret = ret | 32; //or with 00100000
	else if (mystrcmp(*ptr, "-d") == 0)
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
			int alphabetLen = mystrln(alpha_ptr); alphaLen = alphabetLen;
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
	if (mystrcmp(*ptr, "-") == 0)
		*in = stdin;
	else {
		*in = fopen(*ptr, "r");
		if (*in == NULL)
			return 0;
	}
	ptr++;

	if (mystrcmp(*ptr, "-") == 0)
		*out = stdout;
	else
		*out = fopen(*ptr, "w");


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

	return ret;
}

int encr_sc(FILE* in, FILE* out, char n) {//n is a char from 0-31 that is our offset, or key
	//we still have alphaLen, a global variable, from passing into validargs.
	//but i'm not entirely sure if it will stay, and recalculating it is just a line so..
	char* alpha_ptr = Alphabet;
	char* alpha_ptr2 = Alphabet;
	char* temp_ptr;
	int aLen = mystrln(alpha_ptr);
	//note also that if we get here, our input file was valid.
	int c;
	//int resu;
	int counter = 0;
	while ((c = fgetc(in)) != EOF) {
		alpha_ptr = Alphabet;//reset ptr
		alpha_ptr2 = Alphabet;

		if ((c > 96) && (c < 123))
			c = c - 32;//make uppercase
		//for each char: encrypt, and then write to out file
		//find c's place in the alphabet
		for (; counter < aLen; counter++) {
			if (*alpha_ptr==c)
				break;
			alpha_ptr++;
		}
		counter = 0;//reset counter
		//when we reach here, alpha_ptr should be on the right letter, unless
		//it's not in the alphabet in which case it will be on '\0'
		if (*alpha_ptr == '\0') {
			//then we just encrypt the same letter
			fprintf(out, "%c", c);
		}
		else { //alpha_ptr is on the right letter.
			temp_ptr = alpha_ptr + n; //this is where it should be encrypted to unless it needs to carry around
			//printf("%d", aLen);//printf("%c", *temp_ptr);//printf("%s", temp_ptr);
			if ( (temp_ptr - alpha_ptr2) >= (aLen)) {//if the address of the resu char is past the last char
				//so the problem i had to debug here was that i used alpha_ptr as the ptr
				//to the beginning of the alphabet, but it's been adjusted. so I made an alpha_ptr2
				temp_ptr = temp_ptr - aLen;//then carry around (subtract len of alphabet for desired address)
			}
			fprintf(out, "%c", *temp_ptr);//now write the result
		}

	}
	fclose(in);
	fclose(out);
	printf("%c", 'Q');
	return 1;

}

int decr_sc(FILE* in, FILE* out, char n) {
	return 1;
}
