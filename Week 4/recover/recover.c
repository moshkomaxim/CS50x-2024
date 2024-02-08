#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int num_sectors = 0;
    typedef uint8_t BYTE;
    short SECTOR_SIZE = 512;
    BYTE buffer[SECTOR_SIZE];
    bool is_jpeg = 0;
    short num_files = 0;
    char file_name[8];
    FILE *read_file;
    FILE *write_file;

    if (argc != 2)
    {
        printf("Usage: ./recover filename\n");
        return 1;
    }

    read_file = fopen(argv[1], "r");

    if (read_file == NULL)
    {
        printf("Can't open file %s", argv[1]);
    }

    while (fread(&buffer, sizeof(BYTE), SECTOR_SIZE, read_file) == SECTOR_SIZE)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] == 0xe0))
        {
            if (is_jpeg == true)
            {
                fclose(write_file);
                num_files = num_files + 1;
                sprintf(file_name, "%03d.jpg", num_files);
                write_file = fopen(file_name, "w");
                fwrite(buffer, 1, 512, write_file);
                continue;
            }
            else
            {
                is_jpeg = true;
                sprintf(file_name, "%03d.jpg", num_files);
                write_file = fopen(file_name, "w");
                fwrite(buffer, 1, 512, write_file);
                continue;
            }
        }
        else if (is_jpeg == true)
        {
            fwrite(buffer, 1, 512, write_file);
        }
        num_sectors++;
    }
    fclose(read_file);
    fclose(write_file);
    return 0;
}
