// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;
const int HEAD_LEN = sizeof(uint8_t);
const int SAMPLE_LEN = sizeof(int16_t);

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file

    uint8_t header[HEADER_SIZE];
    fread(header, HEAD_LEN, HEADER_SIZE, input);
    if (header == NULL)
    {
        printf("Invalid header.\n");
        return 1;
    }

    fwrite(&header, HEAD_LEN, HEADER_SIZE, output);

    int16_t *sample = (int16_t *) malloc(SAMPLE_LEN);
    while (fread(sample, SAMPLE_LEN, 1, input) == 1)
    {
        *sample = *sample * factor;
        fwrite(sample, SAMPLE_LEN, 1, output);
    }
    free(sample);
    // TODO: Read samples from input file and write updated data to output file

    // Close files
    fclose(input);
    fclose(output);
}
