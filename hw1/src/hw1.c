#include "hw1.h"
#include "debug.h"


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
	aptr = aptr-i;

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
	return 1;

}



int decr_sc(FILE* in, FILE* out, char n) {
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
			temp_ptr = alpha_ptr - n; //this is where it should be encrypted to unless it needs to carry around
			//printf("%d", aLen);//printf("%c", *temp_ptr);//printf("%s", temp_ptr);
			if ( (alpha_ptr2 - temp_ptr) > (0)) {//if the ptr goes before the beginning
				temp_ptr = temp_ptr + aLen;//then carry around (subtract len of alphabet for desired address)
			}
			fprintf(out, "%c", *temp_ptr);//now write the result
		}

	}
	fclose(in);
	fclose(out);
	return 1;
}





int tut_str(int c, char** tutptr, FILE* in, FILE* out, int doubLet) {

	int caps = 0;
	if ((c >= 65) && (c <= 90)) {
		caps = 1;
		c = c + 32; //make lowercase
	}

	while (*tutptr != NULL) { //while the char* to the string we examine is not null


		if (c == **tutptr) {//check if starting char is the one we want
			//if caps, print out string from 1 past the first char after printing out the caps char
			if (caps == 1) {
				fprintf(out, "%c", (c = c-32));
				fprintf(out, "%s", ( (*tutptr)+1)  );
			}
			else {
				fprintf(out, "%s", *tutptr);//write the corresponding string
			}

			return 1;
		}
		tutptr++; //go to next string / column
		//debug("\ngot past: %s \t", *tutptr);
	}
	//debug("\nnot in tutnese array: %d \t", c);

	//reach here --> char not found. But we must've had an alphabetical char coming in, so it's not in
	//the tutnese array (it's a vowel or unmapped cons). This means we just print the char itself.
	//unless it's a vowel and from a double-letter, in which case we also print a "t" to end squa.

	if (doubLet == 1) {//note: c is the lowercase version here no matter what.
		if (c == 97 || c == 101 || c == 105 || c == 111 || c == 117)
			fprintf(out, "%s", "t");
	}


	if (caps == 1) {		//note: checking caps so that we can print what case c originally was.
		fprintf(out, "%c", (c = c-32));
	}
	else {
		fprintf(out, "%c", (c));
	}

	return 0;
}




int check_pair(int* charptr, char** tutptr, FILE* in, FILE* out) {//preconditions: list them
	//check null terminating chars cases (should be c++ normally, just c for doubles at end if did 2x++)
	int c = *charptr;
	int d = fgetc(in); //next char
	//debug("\nit's c: %d \t", c);
	//debug("\nit's d: %d \t", d);

	if (c == '\0')	//don't think this is necessary bc the above while loop should end it
		return d;			//but oh well, doesn't hurt
	else if (d == '\0') {
		tut_str(c, tutptr, in, out, 0); //print out c and return
	}



	int capsc = 0; int capsd = 0;

	if ( (c >= 65) && (c <= 90) ) {
		capsc = 1;
		//debug("\nC IS CAPS: %d \t", c);
		c = c + 32; //make lowercase
	}
	if ( (d >= 65) && (d <= 90) ) {
		capsd = 1;
		d = d + 32; //make lowercase
	}


	if (c != d)	{//compare c and the character after it
		//if the chars aren't the same, print the first one in tutnese
		if (capsc == 1)
			c = c - 32;//make uppercase again
		if (capsd == 1)
			d = d - 32;//make uppercase again
		tut_str(c, tutptr, in, out, 0); //after this, will automatically go to next while iteration
	}
	else {//they're equivalent. Check for cases.
		if (capsc == 1)
			fprintf(out, "%s", "Squa" );
		else
			fprintf(out, "%s", "squa" );

		if (capsc == 1)
			c = c - 32;
		//now print the tut_str for d.
		if (capsd == 1)
			d = d - 32; //makes uppercase again if it was, so tut_str can figure out if it's caps.
		tut_str(d, tutptr, in, out, 1);

		//if chars are the same, increment charptr? no, read in another character since you looked at
		//and used the immediately next one.
		d = fgetc(in); //moves up one
		return d;
	}
	return d;

}





int encr_t(FILE* in, FILE* out) {


	int c;
	char** tutptr;
	int* charptr; //ik this name sucks bc c is an int but just understand that.
	int isLetter = 0;
	//int isCaps;
	//int prev_double;	//1 if we care about the previous char
	c = fgetc(in);

	while (c != EOF) {
		//debug("\nencr value c: %d \t", c);

		isLetter = 0;
		//isCaps = 0;

		tutptr = Tutnese;
		charptr = &c;
		//if the char is not in the alphabet, we simply print the char and go on to the next one
		if ((c >= 97) && (c <= 122)) {
			isLetter = 1; //isCaps = 0;
		}
		else if ((c >= 65) && (c <= 90)) {
			isLetter = 1; //isCaps = 1;
		}

		if (isLetter == 0) {
			//debug("\nnot letter c: %d \t", c);
			fprintf(out, "%c", c);//if it's not a letter, just print it.
			//break; //break iteration of while loop, it'll go back up and try next char
			c = fgetc(in);
		}
		else {
			//debug("\nyes letter c: %d \t", c);
			//parse input 2 chars at a time. For a pair, if the letters differ, print the first in the pair?
			//also will need to deal with null terminator being the latter in a pair...
			c = check_pair(charptr, tutptr, in, out);//check pair of chars starting at given pointer
		}

	}

	return 1;
}




int check_in_tut(int* charptr, char** tutptr, FILE* in, FILE* out) {
	int c = *charptr;
	//int d = fgetc(in);
	int caps = 0;
	int tem = 0;
	int z = 0;
	int getc = 0;
	int getc1 = 0;
	int getc2 = 0;
	int getc3 = 0;
	int getc4 = 0;
	int whileiter = 0; //num times we go through the while loop
	//int getc5 = 0;
	//int isLetter = 0;

	if ( (c >= 65) && (c <= 90) ) {
		caps = 1;
		//debug("\nC IS CAPS: %d \t", c);
		c = c + 32; //make lowercase
	}

	tutptr = Tutnese;
	while (*tutptr != NULL) { //while the char* to the string we examine is not null
		//tutptr = tutptr - whileiter;
		whileiter++;
		//debug("\nan iter of strs: %d \t", c);
		debug("at string: %c \t", **tutptr);

		if (c == **tutptr) {//check if starting char is the one we want. If it is, we iterate through
			//to see if the input file matches the syllable.
			getc = c;
			tem = 0;
			z = mystrln(*tutptr);
			debug("\nz is: %d \t", z);
			int m = 0;
			for (; tem < z; tem++ ) {
				(*tutptr) = (*tutptr) + m;
				if (**tutptr == getc) {
					(*tutptr) = (*tutptr) - m;
					m++;

					//(*tutptr)++;		m++;
					getc = fgetc(in);
					debug("\none iter of chars: %d \t", c);
				}
				else {//failure unless s
					if (c != 's')	//should still be lowercase
						return 127;
					else {//c is s. Since we found a corresponding syllable for s, this means that
						//getc = fgetc(in);
						(*tutptr)--;
						if (getc != 'q')
							return 127;
						getc = fgetc(in);
						if (getc != 'u')
							return 127;
						getc = fgetc(in);
						if (getc != 'a')
							return 127;
						//if we get here, then we know it's squa or squat.
						getc = fgetc(in);
						if (getc == 't'){ //double vowel
							getc = fgetc(in); //getc is the vowel we want to print

							if (caps == 1) {//the case should be the same as that of the S though
								if  ( (getc >= 65) && (getc <= 90) ) {
									c = getc; //uppercase version of second vowel
								} else { //double is uppercase, lowercase
									c = getc - 32; //get uppercase
								}
							}

							else {//first caps == false --> it's lowercase
								if  ( (getc >= 65) && (getc <= 90) ) {
									c = getc + 32; //lowercase version of second vowel
								} else {
									c = getc; //both lowercase
								}
							}
							fprintf(out, "%c", c);
							return getc;//return getc so the second vowel gets printed after.

						}//remember, all this happened with s being mapped to smth.
						else {//success, but only if squa is followed by a consonant.
							//we already know it is a letter.
							if ( !(getc == 'a' || getc == 'e' || getc == 'i' || getc == 'o' || getc == 'u'
								|| getc == 'A' || getc == 'E' || getc == 'I' || getc == 'O' || getc == 'U') ) {

								if (caps == 1) {
									if  ( (getc >= 65) && (getc <= 90) ) {
										c = getc; //uppercase version of second vowel
									} else { //double is uppercase, lowercase
										c = getc - 32; //get uppercase
									}
								}

								else {//first caps == false --> it's lowercase
									if  ( (getc >= 65) && (getc <= 90) ) {
										c = getc + 32; //lowercase version of second vowel
									} else {
										c = getc; //both lowercase
									}
								}
								fprintf(out, "%c", c);
								return getc;
							}
							else //is a vowel
								return 127;
						}
					}
				}

			}
			//if we get past the for loop, then they were equivalent.
			//(*tutptr) = *tutptr - m;

			if (caps == 1) {
				fprintf(out, "%c", (c = c-32));
			} else {
				fprintf(out, "%c", c);//write the corresponding string
			}
			return getc;//(getc = fgetc(in)); //want to return just getc since it's already past the
			//string syllable.
		}
		tutptr++; //go to next string / column
		//debug("\ngot past: %s \t", *tutptr);
	}
	//tutptr = tutptr - whileiter;
	//if we make it past the while loop, then the char, which is a letter, was unmapped.
	//IT COULD be an s, so check for squa / squat. If it's not squa or squat, just print
	//the char s and go back to one after it in getc's using the buffer and ungetc perhaps.
	getc1 = fgetc(in);
	if (getc1 == 'q') {
		getc2 = fgetc(in);
		if (getc2 == 'u'){
			getc3 = fgetc(in);
			if (getc3 == 'a') {
				getc = fgetc(in);
				if (getc == 't' || getc == 'T'){ //double vowel
					getc4 = fgetc(in); //getc is the vowel we want to print
					if (caps == 1) {//the case should be the same as that of the S though
						if  ( (getc4 >= 65) && (getc4 <= 90) ) {
							c = getc4; //uppercase version of second vowel
						} else { //double is uppercase, lowercase
							c = getc4 - 32; //get uppercase
						}
					}

					else {//first caps == false --> it's lowercase
						if  ( (getc4 >= 65) && (getc4 <= 90) ) {
							c = getc4 + 32; //lowercase version of second vowel
						} else {
							c = getc4; //both lowercase
						}
					}
					fprintf(out, "%c", c);
					return getc4;//return getc so the second vowel gets printed after.
				}
				else {//success, but only if squa is followed by a consonant.
					//we already know it is a letter.		getc is still what it needs to be
					if ( !(getc == 'a' || getc == 'e' || getc == 'i' || getc == 'o' || getc == 'u'
						|| getc == 'A' || getc == 'E' || getc == 'I' || getc == 'O' || getc == 'U') ) {

						if (caps == 1) {
							if  ( (getc >= 65) && (getc <= 90) ) {
								c = getc; //uppercase version of second vowel
							} else { //double is uppercase, lowercase
								c = getc - 32; //get uppercase
							}
						}

						else {//first caps == false --> it's lowercase
							if  ( (getc >= 65) && (getc <= 90) ) {
								c = getc + 32; //lowercase version of second vowel
							} else {
								c = getc; //both lowercase
							}
						}
						fprintf(out, "%c", c);
						return getc;
					}
					else //is a vowel. Put back the vowel. we just print the s or S and move on.
						//remember to restore / unget chars.
						ungetc(getc, in);
				}//if we didn't return, then we had to put it all back.
			}
			else
				ungetc(getc3, in);
		}
		else
			ungetc(getc2, in);

	}
	else	//potentially: the next one wasn't q. put it back and let's just print the s
		ungetc(getc1, in);

	//print the curr char
	if (caps == 1) {
		fprintf(out, "%c", (c = c-32));
	} else {
		fprintf(out, "%c", c);//write the corresponding string
	}
	return ( getc = fgetc(in) ); //return the next char

}




int decr_t(FILE* in, FILE* out) {

	int c;
	char** tutptr;
	int* charptr; //ik this name sucks bc c is an int but just understand that.
	int isLetter = 0;
	//int isCaps;
	//int prev_double;	//1 if we care about the previous char
	c = fgetc(in);
	while (c!= EOF) {
		charptr = &c;
		//if c is a char used in the tutnese array, then we iterate through and compare
		//to make sure they are the same. Let's check if c is in the array.
		tutptr = Tutnese;
		isLetter = 0;

		if ((c >= 97) && (c <= 122)) {
			isLetter = 1; //isCaps = 0;
		}
		else if ((c >= 65) && (c <= 90)) {
			isLetter = 1; //isCaps = 1;
		}

		if (isLetter == 0) {
			//debug("\nnot letter c: %d \t", c);
			fprintf(out, "%c", c);//if it's not a letter, just print it.
			c = fgetc(in);
		}
		else { //if it is a letter, we check if it's in the tutnese array.
			debug("\nis letter: %d \t", c);
			c = check_in_tut(charptr, tutptr, in, out);//check pair of chars starting at given pointer
			debug("\npost check, is letter: %d \t", c);
			if (c == 127) //returned in the case of error decrypting
				return 127;
			//if not, then it was valid and printed and c is where it should be now.
		}
	}

	return 1;
}



