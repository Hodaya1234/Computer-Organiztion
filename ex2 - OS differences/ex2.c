//
// Hodaya Koslowsky
//

#include <string.h>
#include <stdio.h>
#define KEEP 1
#define SWAP 0

/**
 * match an operating system flag with a string of it's new line character
 * @param c the string that represents a OS flag.
 * @return a string of the new line char.
 */
char* checkString(char * c);

/**
 * Copy the source file to a new file, without changes.
 * @param source the source file
 * @param destFile destination file name
 */
void createCopy(FILE * source, char * destFile);

/**
 * copy the source file with new line character of a source operating system
 * to a new file with new line of a different operating system.
 * if keep is equal KEEP do not change endiannes.
 * if it is equal SWAP, switch the two chars of buffer before writing
 * @param source file name
 * @param destName destination file name
 * @param sourceOs source operating system for the first new line character
 * @param destOs destination operating system for the new - new line character
 * @param keep whether or not the function should change endianness
 */
void changeEnter(FILE * source, char * destName, char* sourceOs, char* destOs, int keep);

/**
 * switches the two bytes of a char buffer.
 * @param chars the char buffer to switch the bytes of.
 */
void changeTwoChars(char * chars);

static const int NUM_CHARS = 2;
static const int SIZE_CHAR = 1;

int main(int argc, char* argv[]) {

    if(argc < 3) {
        return 0;
    }

    char * sourceName = argv[1];
    char * destName = argv[2];

    FILE *source = fopen(sourceName, "rb");

    if(source == NULL) {
        return 0;
    }

    if(argc == 3) {
        //no additional flag
        createCopy(source, destName);
    }

    if(argc == 5) {
        //change the new line char

        char* sourceOs = checkString(argv[3]);
        char* destOs = checkString(argv[4]);
        if((sourceOs==NULL) || (destOs==NULL)) {
            return 0;
        }
        changeEnter(source, destName, sourceOs, destOs, KEEP);
    }

    if(argc == 6) {
        char* sourceOs = checkString(argv[3]);
        char* destOs = checkString(argv[4]);
        if((sourceOs==NULL) || (destOs==NULL)) {
            return 0;
        }
        //if the last flag is keep, make the changeEnter function.
        if (strcmp(argv[5], "-keep") == 0){
            changeEnter(source, destName, sourceOs, destOs, KEEP);
        } else if (strcmp(argv[5], "-swap") == 0){
            changeEnter(source, destName, sourceOs, destOs, SWAP);
        }
    }
    fclose(source);
    return 0;
}

char* checkString(char * c){
    if(strcmp(c, "-unix") == 0) {
        return "\n";
    }
    if(strcmp(c, "-win") == 0) {
        return "\r\n";
    }
    if(strcmp(c, "-mac") == 0) {
        return "\r";
    } else {
        return NULL;
    }
}

void createCopy(FILE * source, char * destFile) {
    FILE *dest = fopen(destFile, "wb");
    if(dest == NULL) {
        return;
    }
    char buffer[NUM_CHARS];
    size_t s = fread(buffer, SIZE_CHAR, NUM_CHARS, source);
    while(s == NUM_CHARS) {
        fwrite(buffer, SIZE_CHAR, s, dest);
        s = fread(buffer, SIZE_CHAR, NUM_CHARS, source);
    }
    fclose(dest);
}

void changeEnter(FILE * source, char * destName, char* sourceOs, char* destOs, int keep){

    FILE *dest = fopen(destName, "wb");
    char buffer[NUM_CHARS];

    size_t s = fread(buffer, SIZE_CHAR, NUM_CHARS, source);

    //once updated, buffer[endiannes] returns the byte which is not 0
    int endianness = -1;

    while (s == NUM_CHARS) {
        if((((buffer[0] == '\n') || (buffer[0] == '\r')) && (buffer[1] == 0))
           || (((buffer[1] == '\n') || (buffer[1] == '\r')) && (buffer[0] == 0))) {
            if((strcmp(sourceOs, "\r\n") == 0) || (strcmp(sourceOs, "\r") == 0)) {
                // if the source is windows or mac, the new line has '\r'
                //check the place of new line and determine endianness
                if (buffer[0] == '\r') {
                    endianness = 0;
                } else if (buffer[1] == '\r') {
                    endianness = 1;
                } else {
                    continue;
                }
                if (strcmp(sourceOs, "\r\n") == 0) {
                    s = fread(buffer, SIZE_CHAR, NUM_CHARS, source);
                }
                if (strcmp(destOs, "\r") == 0) {
                    buffer[endianness] = '\r';
                } else if (strcmp(destOs, "\n") == 0) {
                    buffer[endianness] = '\n';
                } else if (strcmp(destOs, "\r\n") == 0) {
                    buffer[endianness] = '\r';
                    if(keep == SWAP) {
                        changeTwoChars(buffer);
                    }
                    fwrite(buffer, SIZE_CHAR, s, dest);
                    if(keep == SWAP) {
                        changeTwoChars(buffer);
                    }
                    buffer[endianness] = '\n';
                }
            } else if (strcmp(sourceOs, "\n") == 0) {
                // if source is unix, and the new line is '\n'
                if(buffer[0] == '\n') {
                    endianness = 0;
                } else if(buffer[1] == '\n') {
                    endianness = 1;
                } else {
                    continue;
                }
                if(strcmp(destOs, "\r\n") == 0) {
                    buffer[endianness] = '\r';
                    if(keep == SWAP) {
                        changeTwoChars(buffer);
                    }
                    fwrite(buffer, SIZE_CHAR, s, dest);
                    if(keep == SWAP) {
                        changeTwoChars(buffer);
                    }
                    buffer[endianness] = '\n';
                } else if(strcmp(destOs, "\r") == 0) {
                    buffer[endianness] = '\r';
                }
            }
        }
        if(keep == SWAP) {
            changeTwoChars(buffer);
        }
        fwrite(buffer, SIZE_CHAR, s, dest);
        s = fread(buffer, SIZE_CHAR, NUM_CHARS, source);
    }
    fclose(dest);
}

void changeTwoChars(char * chars){
    char temp = chars[0];
    chars[0] = chars[1];
    chars[1] = temp;
}
