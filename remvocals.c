#include <stdio.h>
#include <stdlib.h>

#define HEADER_SIZE 44

int main(int argc, char **argv){

    short header[HEADER_SIZE];
    FILE *source, *dest;

    if (argc != 3){
        fprintf(stderr, "Usage: %s inputfile outputfile\n", argv[0]);
        return 1;
    }

    char* input = argv[1]; //The name of the source file
    char* output = argv[2]; //The name of the destination file

    // remVocals(source, dest); // Most of the work (removing vocals) will be done in this function. 
    source = fopen(input, "rb"); //Reading the file in binary mode.
    if (source == NULL) { // File not found or does not exist.
        perror("Error: could not open input file\n");
        return 1;
    }

    dest = fopen(output, "wb"); //Writing to the file in binary mode.
    // If file does not exist, the code will by default create one.
    if (dest == NULL) { 
        perror("Error: could not open output file\n"); //Error incase there are problems in accessing or creating the out file.
        fclose(source);
        return 1;
    } 
    
    fread(&header, sizeof(short), HEADER_SIZE, source); // Read header info from orig file.
    fwrite(&header, sizeof(short), HEADER_SIZE, dest); // Write the header info into the output file.
    
    short left;  // Variable to hold left byte
    short right; // Variable to hold right byte

    // The following While loop was adapted from a ChatGPT response. The response was not copied nor entirely used, only for inspiration to solve an obstacle.   
    // "C code to read left and right byte from a WAV file" prompt. ChatGPT, GPT-3.5, Open-AI, 11 Feb. 2024, https://chat.openai.com/share/1f62f0c8-c12e-4058-80ad-5f47a215c7c1
    while (fread(&left, sizeof(short), 1, source) == 1 && fread(&right, sizeof(short), 1, source) == 1) { //Loops until no more bytes left in the orig file.
        short product = (left - right)/2; // Variable to hold modified byte --> written into out file. 
        fwrite(&product, sizeof(short), 1, dest); // Adding modified byte into the left channel of this pair of bytes. 
        fwrite(&product, sizeof(short), 1, dest); // Adding modified byte into the right channel of this pair of bytes.
    }


    fclose(dest);
    fclose(source);


    return 0;
}