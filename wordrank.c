#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include <sys/timeb.h>

/*	WordRank - by Greg Scaffidi, March, 2014  

	This program finds the "rank" of a given word compared to other words
	that can be made by re-arranging the letters of the given word.  Any unique
	combination of letters will be considered a different word, even if those 
	words do not appear in a dictionary (ie "BAT" and "TAB" are words, but so
	are "ATB" and "BTA").  If all possible words that can be created from a set
	of letters are listed alphabetically, the first word in the list will have
	rank #1, the next word will have rank #2, and the last word will have the
	highest rank.  For example, consider the following words with their ranks:
	
	Word	Rank
	-----	----
	ABC		1
	ACB		2
	BAC		3
	BCA		4
	CAB		5
	CBA		6

	It is helpful to first understand how to determine the total number of unique
	words that can be made out of a	set of letters.
	
	This total can be expressed in terms of n, where n is the number of 
	letters in the set.  If no letters are repeated, the solution is found by
	taking n! (n factorial), where n is the total number of letters in the word.
	In the example above, n=3.  So, the	rank of the last word in the list is:
	3! = 3 x 2 x 1 = 6.
	
	The calculation to find the rank of the last word in the list when letters
	are repeated is slightly more complex.  To do it, we take n! and divide it 
	by x, where x expresses	the number of times a letter is repeated.  Consider 
	the following list:

	Word	Rank
	-----	----	
	AAAB	1
	AABA	2
	ABAA	3
	BAAA	4
	
	To find the highest rank, we take "(the total number of letters)!" and divide
	by "(the total number of times each letter is repeated)!".  In this case we have
	4 total letters, and 3 repeats. So,

	4!/3! = 24/6 = 4

	This solution appears simple because only 1 letter is repeated, but it is
	easily extended to the case where more than 1 letter repeats.  In this case, we 
	take the product of "(the total number of times each letter is repeated)!" for 
	each letter in the word.  We could have	written the expression above as:
	4!/3! x 1! to get the same answer.  So, consider the following example with
	multiple repeated letters:
	
	Word	Rank
	-----	----	
	AABB	1
	ABAB	2
	ABBA	3
	BAAB	4
	BABA	5
	BBAA	6

	4!/2! * 2! = 24/(2x2) = 24/4 = 6

	To find the rank of a particular word in the list, say 'BCA', we first start
	by alphabetizing the letters and comparing each letter in the alphabetized
	word to the first letter in the word we are trying to rank.  Each time a letter
	doesn't match, we penalize the word by adding (n-i-1)! to it's rank, where 'i' is 
	the	number of the letters in the alphabetized word that we have matched so far. 
	In this case we just subtract 1 from n, because we haven't matched any letters yet 
	(i=0).	B!=A, so the total penalty for 'BCA' increases by:
	(n-i-1)! = (3-0-1)! = 2! = 2x1 = 2.

	Next, we look at the second letter of the alphabetized word and again compare it 
	to the first letter in 'BCA'. It matches (B=B).  We do not penalize the word
	in this case.  Instead, we remove the letter that we matched from the alphabetized
	word.  The alphabetized word becomes 'AC' and 'i' becomes 1.  Then, we start again 
	by comparing the first letter in the alphabetized word, 'A' to the second letter in
	'BCA', 'C'.  It doesn't match so the penalty increases by,
	(n-i-1)! = (3-1-1)! = 1! = 1x1 = 1.

	The total penalty is now 2+1 = 3.
	
	Now, we compare the second letter in the alphabetized word, 'C' to the second letter
	of 'BCA' and they match.  We don't assign a penalty.  The alphabetized word becomes
	just 'A'.  We compare this to the next letter in 'BCA' and they also match.  We don't
	assign a penalty and we are done.  When we started out, the total penalty (rank) was 0.
	Since we decided that the best (lowest) rank in our scheme is #1, we have to add 1 to
	the total penalty to get a rank of #4.

	The same process is used to find the rank of a word when letters are repeated, except
	that the number of repeated letters has to be tracked in addition to the number of total
	letters, and the total penalty has to be divided by:
	"(the total number of times each letter is repeated)! for each letter in the word"
	
	If we are looking for the rank of the word, BOOKKEEPER:
	
	Let R = "(the total number of times each letter is repeated)!"
	R = rB x rO x rK x rE x rP x rR
	R = 1! x 2! x 2! x 3! x 1! x 1!
	R =  1 x  2 x  2 x  6 x  1 x  1
	R = 24
	
	BOOKKEEPER has 10 letters. So, 
	n = 10
	BEEEKKOOPR is the alphabetized version of the word.
	i = 0, since we are just starting and haven't found any matches yet.
	
	Starting with the first letter of the word, we examine each letter in the word and add 
	a penalty according to the formula, ((n-i-1)!/R) when the letters do not match. Once 
	we match the letter, we update i and R, then start over by comparing the next letter 
	against the alphabetized word. But, whenever we find a match, we remove that letter from 
	the alphabetized word and don't consider it again.  When we find a letter that matches in 
	the same position in both words, we don't assign a penalty at all:
	
	B = 0, i = 1, rB = 0! = 1, R = 24
	 E = (n-i-1)!/R = (10 -1 -1)!/24 = 8!/24 = 40320/24 = 1680
	 E = (n-i-1)!/R = (10 -1 -1)!/24 = 8!/24 = 40320/24 = 1680
	 E = (n-i-1)!/R = (10 -1 -1)!/24 = 8!/24 = 40320/24 = 1680
	 K = (n-i-1)!/R = (10 -1 -1)!/24 = 8!/24 = 40320/24 = 1680
	 K = (n-i-1)!/R = (10 -1 -1)!/24 = 8!/24 = 40320/24 = 1680
	O = 1680 x 5 = 8400, i = 2, rO = 1! = 1, R = 12
	 E = (n-i-1)!/R = (10 -2 -1)!/12 = 7!/12 = 5040/12 = 420
	 E = (n-i-1)!/R = (10 -2 -1)!/12 = 7!/12 = 5040/12 = 420
	 E = (n-i-1)!/R = (10 -2 -1)!/12 = 7!/12 = 5040/12 = 420
	 K = (n-i-1)!/R = (10 -2 -1)!/12 = 7!/12 = 5040/12 = 420
	 K = (n-i-1)!/R = (10 -2 -1)!/12 = 7!/12 = 5040/12 = 420
	O = 420 x 5 = 2100, i = 3, rO = 0! = 1, R = 12
	 E = (n-i-1)!/R = (10 -3 -1)!/12 = 6!/12 = 720/12 = 60
	 E = (n-i-1)!/R = (10 -3 -1)!/12 = 6!/12 = 720/12 = 60
	 E = (n-i-1)!/R = (10 -3 -1)!/12 = 6!/12 = 720/12 = 60
	K = 60 x 3 = 180, i = 4, rK = 1! = 1, R = 6
	 E = (n-i-1)!/R = (10 -4 -1)!/6 = 5!/6 = 120/6 = 20
	 E = (n-i-1)!/R = (10 -4 -1)!/6 = 5!/6 = 120/6 = 20
	 E = (n-i-1)!/R = (10 -4 -1)!/6 = 5!/6 = 120/6 = 20
	K = 20 x 3 = 60, i = 5, rK = 0! = 1, R = 6
	E = 0, i = 6, rE = 2! = 2, R = 2
	E = 0, i = 7, rE = 1! = 1, R = 1
	 E = (n-i-1)!/R = (10 -7 -1)!/1 = 2!/1 = 2/1 = 2
	P = 2, i = 8, rP = 0! = 1, R = 1
	E = 0, i = 9, rE = 0! = 1, R = 1
	R = 0, i = 10, rR = 0! = 1, R = 1
	
	After totalling-up the penalties found for each letter, we get 10742.  Because 
	our ranking scheme starts with 1 (not 0) we have to add	1 to find the rank of 
	the word, BOOKKEEPER: 10742 + 1 = 10743.  
	
	The program below accepts a given word as as an argument.  If the argument is found to be
	valid input (a word consisting of 1-25 capital letters), the total number of times each 
	letter appears in the word is stored in an array.  Another array stores a copy of the word 
	that has had it's letters re-arranged in alphabetical order.  These two arrays are used
	along with the algorithm outlined above to find the given word's rank.

*/

// error conditions
#define ERR_TOO_MANY_WORDS		0x01
#define ERR_TOO_MANY_LETTERS	0x02
#define ERR_TOO_FEW_LETTERS		0x04
#define ERR_TOO_FEW_WORDS		0x08
#define ERR_NOT_ALL_CAPS		0x10

unsigned char err;
unsigned int wordLength;

// array to store the number of times
// each letter is repeated in the word
char counts[26] = {0}; 
// used to easily asses whether repeated
// letters still need to be accounted for
unsigned char repeatsTrue = 0;

// the alphabetized version of the word
// ie - the word with rank #1
char alphaWord[26] = {0}; 

// used to build the penalty as letters in
// the word are compared to letters in the
// alphabetized word, as described above
unsigned long long penalty = 0; 

// for keeping a sum total of the word's rank
// set to 1, b/c rank starts at #1
unsigned long long totalPenalty = 1;

// capitalized letters in alphabetical order 
// are used for comparison
const char upper[26] = {
	'A','B','C','D','E',
	'F','G','H','I','J',
	'K','L','M','N','O',
	'P','Q','R','S','T',
	'U','V','W','X','Y',
	'Z'
}; 
// variables for storing what's leftover
// from integer division
unsigned long long remainder = 0;
double extraSum = 0.0;

// for reference, the first 27 factorials
//    0! 	 1  
//    1! 	 1
//    2! 	 2
//    3! 	 6
//    4! 	 24
//    5! 	 120
//    6! 	 720
//    7! 	 5040
//    8! 	 40320
//    9! 	 362880
//   10! 	 3628800
//   11! 	 39916800
//   12! 	 479001600
//   13! 	 6227020800
//   14! 	 87178291200
//   15! 	 1307674368000
//   16! 	 20922789888000
//   17! 	 355687428096000
//   18! 	 6402373705728000
//   19! 	 121645100408832000
//   20! 	 2432902008176640000
//   21! 	 51090942171709440000
//   22! 	 1124000727777607680000
//   23! 	 25852016738884976640000
//   24! 	 620448401733239439360000
//   25! 	 15511210043330985984000000
//   26! 	 403291461126605635584000000

// use these pre-calculated factorials instead of
// computing them at run-time, where possible
unsigned long long factorials [26] = {
	1, 
	1,
	2,
	6,
	24,
	120,
	720,
	5040,
	40320,
	362880,
	3628800,
	39916800,
	479001600,
	6227020800,
	87178291200,
	1307674368000,
	20922789888000,
	355687428096000,
	6402373705728000,
	121645100408832000,
	2432902008176640000,
	1,	// since input should not generate
	1,	// a rank greater than what can be
	1,	// stored in a 64bit integer, these
	1,	// factorials should not be needed.
	1	// set to 1, to provide predictable
};		// behavior if the program attempts
		// to access them.

// Checks that a word is composed of valid characters.
// Alphabetizes the word and provides information
// about the number of characters and repeated characters
//
int checkWord(char word[]) {
	unsigned char i = 0;
	unsigned char j = 0;
	unsigned char letterCount = 0;
	
	printf("\nChecking word: '%s'",word);

	// for each letter in the word
	for(i=0;i<wordLength;++i) {
		if(word[i] >= 'A' && word[i] <= 'Z') {
			++counts[word[i] - 'A'];
		}
		else {
			printf("\n'%c' is not a valid character.",word[i]);
			return ERR_NOT_ALL_CAPS;
		}
	}
	// build the alphabetized word
	for(j=0;j<26;++j) {
		for(i=0;i<counts[j];++i) {
			alphaWord[letterCount] = upper[j];
			++letterCount;
		}
	}
	return 0;
}

// Removes a letter from a string.
// --------------------------------------------------------
// arguments:
// 0 : the word to shrink
// 1 : the character to remove
// 2 : a buffer that will contain the new shrunken word
//
char * shrinkWord (char word[], char letter, char newWord[]) {
	int position = 0;

	// get a pointer to the character in the string
	char *letterPos = strchr(word, letter);
	if(letterPos == NULL)
	{
	  printf("\nshrinkWord() ERROR!"); // report error
	  return newWord;
	}
	position = letterPos - word;

	// copy word up until the character
	memcpy(newWord, word, position);

	// append the rest of the word after the character
	strcat(newWord + position, word + position + 1);
	return newWord;
}

unsigned char checkCounts() {
	unsigned char k = 0;
	for(k=0;k<26;++k) {
		if(counts[k] > 1) {
			return 1;
		}
	}
	return 0;
}

// This function attempts to work around the limitation of the maximum size of
// a 64bit integer. Though the program isn't expected to output an answer with
// a value greater than the capacity of a 64bit integer, the calculations 
// involved in arriving at that answer could involve values that are too large.
// This function attempts to cancel-out values in the numerator and denominator
// prior to performing the division.
// --------------------------------------------------------
// arguments:
// 0 : the numerator factorial
// 1 : an array containing the denominator factorials
//
unsigned long long divFactorials (unsigned char numerator, char denominators[]) {

	// arrays to store the "exploded" factorials
	unsigned char numerPieces[26] = {0};
	unsigned char denomPieces[26] = {0};
	// used for iterating through the arrays
	unsigned char i = 0;
	unsigned char j = 0;
	unsigned char numTemp = 0;
	unsigned char denomTemp = 0;
	// the "whole" part leftover from the integer division of the 
	// numerator and denominator
	unsigned long long result = 1;
	// sum values used to perform the division
	unsigned long long numSum = 1;
	unsigned long long demSum = 1;
	// the remainder from the integer division
	unsigned long long remain = 0;
	// decimal value of the remainder found by
	// dividing 'remain' by 'demSum'
	double exBit = 0.0;

	// explode the factorial into an array of the
	// integers that are multiplied together to form it.
	numTemp = numerator;
	for(i=0;i<26;++i) {
		if(denominators[i]>1) {
			denomPieces[denomTemp] = denominators[i];
			for(j=1;j<denominators[i];++j) {
				denomPieces[denomTemp+j] = denominators[i]-j;
			}
			denomTemp+=(denominators[i]-1);
		}
		if(numTemp) {
			numerPieces[i] = numTemp;
			--numTemp;
		}
	}


	// go through the exploded numerator factorial components
	for (i=0; i < numerator; ++i) {
		// go through the exploded denominator factorial components
		for(j=0;j<denomTemp;++j) {
			// if there is no remainder when the numerator component 
			// is divided by the denominator component it is easy to
			// cancel something out.
			if (!(numerPieces[i] % denomPieces[j])) {
				// they cancel out perfectly!
				if((numerPieces[i] / denomPieces[j]) == 1) {
					numerPieces[i] = denomPieces[j] = 1;
					++i;
					break;
				}
				// the denominator component cancels out completely
				// but the numerator component is just reduced
				else if((numerPieces[i]  / denomPieces[j]) < numerPieces[i]) {
					numerPieces[i] = (numerPieces[i]  / denomPieces[j]);
					denomPieces[j] = 1;
					++i;
					break;
				}
			}
		}
	}

	// total up the numerator
	for(i=0;i<numerator;++i) {
		if(numerPieces[i]>1) {

			numSum *=numerPieces[i];
		}
	}
	// ... and the denominator
	for(i=0;i<denomTemp;++i) {
		if(denomPieces[i]>1) {

			demSum *=denomPieces[i];
		}
	}

	// perform the integer division
	result = numSum / demSum;
	// get the remainder
	remain = numSum % demSum;
	// ... if there is one, convert it to a decimal value
	if(remain) {
		exBit = (double) remain / (double) demSum;
	}
	// keep a running total of the remainder values
	extraSum += exBit;

	return result;
}


// Converts a word to its number. Called recursively.
// --------------------------------------------------------
// arguments:
// 0 : the word to find the number of
// 1 : the alphabetized version of the word
// 2 : length of alphabetized word
// 3 : a value reflecting the number of times the function has been called
//
int wordToNumber(char word[],char newAlpha[], int alphaLength, unsigned char reit ){
	// chars should be safe to use, given the restraints on the problem
	// that words are <= 25 letters, and rank will fit into 64bit integer
	unsigned char i = 0;
	unsigned char j = 0;
	unsigned char k = 0;

	// some buffers for storing the word after it has
	// been manipulated by different functions
	char newWord[26] ={0};
	char newWord1[26] ={0};

	// for each letter in the word
	for(i=reit;i<wordLength;++i) {
		// for each letter in the alphabetized word
		for(j=0;j<alphaLength;++j) {
			// if the letter we are looking at in the word to be ranked
			// does not match the letter at the corresponding position
			// in the alphabetized word, we need to assign a penalty
			if(newAlpha[j]!=word[i]) 
			{
				// here we need to be concerned with using values too large
				// for a 64bit integer.
				// ie, finding the rank of a word like BBBBBBBBBBBBBBBBBBBAAAAAA
				// involves dividing 25! by 19! x 5!
				if(repeatsTrue) {
					penalty += divFactorials(strlen(word) - reit - 1,counts);
				}
				// if we don't need to do a division, we can just add the
				// pre-calculated factorial value to the rank
				else {
					penalty += (unsigned long long) (factorials[strlen(word) - reit - 1]);
				}
			} 
			else {
				for(k=0;k<26;++k) {
					// if the letter is repeated, make sure we reduce the count
					if((upper[k] == newAlpha[j]) && counts[k] > 1) {
						--counts[k];
						// update the repeatsTrue flag
						repeatsTrue = checkCounts();
						break;
					}
				}
				// ... remove that letter from the word 
				strcpy(newWord1, shrinkWord(newAlpha,newAlpha[j],newWord));
				// ... track the total penalty
				totalPenalty += (penalty);
				// ... before resetting penalty
				penalty = 0;
				// ... and call this function to check again
				// using the remaining letters
				wordToNumber(word,newWord1,strlen(newWord1),i+1);
				return 0;
				
			}			 
		}
	}
	// need to cast to float first.......
	printf("\n\nRank  # %llu ",(unsigned long long)totalPenalty + (unsigned long long)(float)extraSum);
	return 0;

}

int printErr(unsigned char num) {
	if(!num) {
		printf("\nNo errors detected in input.");
		return 0;
	}
	printf("\n");
	if(num & ERR_TOO_MANY_WORDS)
		printf("\nYou entered too many words.");
	if(num & ERR_TOO_MANY_LETTERS)
		printf("\nYou entered too many letters.");
	if(num & ERR_TOO_FEW_LETTERS)
		printf("\nYou entered too few letters.");
	if(num & ERR_TOO_FEW_WORDS)
		printf("\nYou entered too few words.");
	if(num & ERR_NOT_ALL_CAPS)
		printf("\nYou entered lower case letters.");
	printf("\nPlease enter just one word consisting of 1-25 capital letters.\n");
	return -1;
}

int main(int argc, char **argv)
{
	struct timeb tstart, tstop;
 
	ftime(&tstart);
	err = 0;
	wordLength = 0;

	// check for valid input
	if(argc < 2) {
		err |= ERR_TOO_FEW_WORDS;
	}
	else if(argc > 2) {
		err |= ERR_TOO_MANY_WORDS;
	}
	else {
		wordLength = strlen(argv[1]);
		if (wordLength < 1) {
			err |= ERR_TOO_FEW_LETTERS;
		}
		else if (wordLength > 25) {
			err |= ERR_TOO_MANY_LETTERS;
		}
		// finish checking for letters.
		// total up the counts for each letter.
		// build an alphabetized word to compare against.
		err |= checkWord(argv[1]);
	}
	
	if(!printErr(err)) {
		// remember if there are repeated letters.
		repeatsTrue = checkCounts();
		wordToNumber(argv[1], alphaWord, wordLength,0);
		printf("found for word: %s\n", argv[1]);
		ftime(&tstop);
		printf("\nelapsed time: %d ms\n", 
			(int) (1000.0 * (tstop.time - tstart.time) + 
			(tstop.millitm - tstart.millitm)));
		
		return 0;
	}
	else
		return err;
}
