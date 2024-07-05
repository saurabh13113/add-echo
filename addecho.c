#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
 
# define HEADER_SIZE 22

int test_int(char *str){
    //makes a temp pointer variable
    char *ptr;
 
    //makes an output variable and does the conversions
    long out = strtol(str, &ptr, 10);
 
    //only accepts values great than 0
    if (out > 0){
        return out;
    }
    else {
        return 0;
    }
}
 
int main(int argc, char **argv){
 
    short header[HEADER_SIZE];
    FILE *source, *dest;
    short samp;
    long size;
    //long sample_count;
    int error;

 
    if (argc < 3 || argc > 7){
        // fprintf("Number of arguments passed is not sufficient\n Usage: %s sourcewav destwav\n", argv[0]);
        // exit(EXIT_FAILURE);
        fprintf(stderr, "Usage: %s [-d delay] [-v volume_scale] inputfile outputfile\n", argv[0]);
        return 1;
    }
 
    int curr;
    long delay = 8000;
    long volume_scale = 4;
 
    while ((curr = getopt (argc, argv, "d:v:")) != -1){
        switch (curr){
            case 'd':
                long temp1 = test_int(optarg);
                if (temp1){
                    delay = temp1;
                }
                break;
            case 'v':
                long temp2 = test_int(optarg);
                if (temp2){
                    volume_scale = temp2;
                }
                break;
            default:
                fprintf(stderr, "Usage: %s [-d delay] [-v volume_scale] inputfile outputfile\n", argv[0]);
                break;
        }
    }
    
    char* input = argv[argc-2]; //The name of the source file
    char* output = argv[argc-1]; //The name of the destination file
 
    // remVocals(source, dest); // Most of the work (removing vocals) will be done in this function. 
    source = fopen(input, "rb"); //Reading the file in binary mode.
    if (source == NULL) { // File not found.
        // perror("File Not Found");
        // fprintf(stderr, "Error: could not open input file\n");
        perror("Error: could not open input file\n");
        return 1;
    }
 
    fseek(source, 0, SEEK_END); // Seek to the end of the input file, to obtain size.
    size = ftell(source)/sizeof(short);       // Sotre size in variable "size"
    // sample_count = size / sizeof(short);    // Divide it by sizeof(short) to find the number of shorts.
    fseek(source, 0, SEEK_SET);     // Reset the file pointer to the beginning of the file
 
    dest = fopen(output, "wb"); //Writing to the file in binary mode.
    // If file does not exist, the code will by default create one.
    if (dest == NULL) {
        // fprintf(stderr, "Error: could not open output file\n");
        perror("Error: could not open output file\n");
        return 1;
    } 
 

    fread(&header, sizeof(short), HEADER_SIZE, source); // Read header integer
    unsigned int * sizeptr;
    unsigned int * nsizeptr;
    sizeptr = (unsigned int *)(header + 2);
    nsizeptr = (unsigned int *)(header + 20);
    *sizeptr += (delay*2);
    *nsizeptr += (delay*2);
    fwrite(&header, sizeof(short), HEADER_SIZE, dest); // Write the header info into the output file.


    short* delay_buffer = (short *)malloc(sizeof(short) * delay); // Create an array in the heap to store the delay samples.

    if (delay_buffer == NULL){
        // printf(stderr, "Memory not able to be allocated on heap");
        perror("Memory not able to be allocated on heap");
        return 1;
    }
 
    for (int i = 0; i < delay; i++) { //Setting the deekay values to 0
        delay_buffer[i] = 0;            // So that theres no empty space on the array when
        }                                // combining writing elements into the output file.
 
    int count1 = 0;
    int count2 = 0;
    int count3 = 0;
    bool flag = false;
    while(fread(&samp, sizeof(short), 1, source) == 1){
            if (count1 < delay){ // IF MALLOC BUFFER FULL, The first delay number samples have been stored for echo.
                short damp = samp/volume_scale; //damp is the modified version of the sample. 
                delay_buffer[count1] = damp; // Adding the current sample (modified) into the delay buffer.
                error = fwrite(&samp, sizeof(short), 1, dest); // Since delay count has not been met, we add the sample unchanged into the final file.
                if (error != 1) {
                    // fprintf(stderr, "Error: could not write a sample\n");
                    perror("Error: could not write a sample\n");
                    return 1;
                    }
                count1++; // Counter increments. 
            }
            else {
		flag = true;
                short final_samp = delay_buffer[count2] + samp; // Create a modified short to add to the output file.
                error = fwrite(&final_samp, sizeof(short), 1, dest); // 
                if (error != 1) {
                    // fprintf(stderr, "Error: could not write a sample\n");
                    perror("Error: could not write a sample\n");
                    return 1;
                    }
                delay_buffer[count2] = samp/volume_scale;
                if(count2 == delay-1){
                    count2 = 0;
                }
                else{
                    count2++;
                }
                count3++;
            }
    }
    printf("%d %d %d \n",count1,count2,count3);
	
   long cond;
   if (size >= delay){cond = size;}
   else{cond = delay;}


    while (count3<cond){ // Loop to add remaining delay samples into the final files. 
        if (!flag) {
            count2 = size;
            flag = true;
        }
	    short final_samp2 = delay_buffer[count2];
        error = fwrite(&final_samp2, sizeof(short), 1, dest); // 
                if (error != 1) {
                    // fprintf(stderr, "Error: could not write a sample\n");
                    perror("Error: could not write a sample\n");
                    return 1;
                    }
        if (count2 == delay-1) {
            count2 = 0;
        }
        else {
            count2++;
        }
        count3++;
    } 
    fclose(source);
    fclose(dest);
    free(delay_buffer);
 
    return 0;
}
