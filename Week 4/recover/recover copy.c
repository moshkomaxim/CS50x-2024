#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    int num_sectors = 0;
    typedef uint8_t BYTE;
    BYTE HEADER_SIZE = 64;
    short SECTOR_SIZE = 512;
    BYTE buffer[SECTOR_SIZE];
    bool is_jpeg = 0;
    int num_files = 0;
    char file_name[10];
    FILE * write_file;

    if (argc != 2)
    {
        printf("Usage: ./recover filename\n");
        return 1;
    }

    FILE * read_file = fopen(argv[1], "r");

    if (read_file == NULL)
    {
        printf("Can't open file %s", argv[1]);
    }

    while(fread(&buffer, sizeof(BYTE), SECTOR_SIZE, read_file) == SECTOR_SIZE)
    {
        for(int byte = 0; byte < SECTOR_SIZE; byte++)
        {
            if (buffer[byte] == 0xff &&
                buffer[byte+1] == 0xd8 &&
                buffer[byte+2] == 0xff &&
                (buffer[byte+3] == 0xe0))
            {
                if (is_jpeg == true)
                {
                    fclose(write_file);
                    num_files = num_files + 1;
                    sprintf(file_name, "%03d.jpg", num_files);
                    write_file = fopen(file_name, "w");
                    fwrite(&buffer[byte], 1, 1, write_file);
                    continue;
                }
                else
                {
                    is_jpeg = true;
                    num_files = num_files;
                    sprintf(file_name, "%03d.jpg", num_files);
                    write_file = fopen(file_name, "w");
                    fwrite(&buffer[byte], 1, 1, write_file);
                    continue;
                }
            }
            else if (is_jpeg == true)
            {
                fwrite(&buffer[byte], 1, 1, write_file);
            }
        }
        num_sectors++;
    }
        fclose(read_file);
        fclose(write_file);
        return 0;
}
