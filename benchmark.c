#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sysexits.h>
#include <unistd.h>
#include <time.h>

#ifdef __linux__
#include <stdint.h>
#include <string.h>
#include <sys/resource.h>
#include <linux/limits.h>
#include <dirent.h>
#endif

int main(int argc, char* argv[])
{
    //Change the current directory
    chdir(argv[1]);

    int n = 0, i = 0;
    DIR* d;
    struct dirent* dir;
    d = opendir(argv[1]);
    int count = atoi(argv[2]);

    if (d == NULL)
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    //Determine the number of files
    while ((dir = readdir(d)) != NULL)
    {
        if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, ".."))
        {

        }
        else {
            n++;
        }
    }

    if (count > n)
    {
        fprintf(stderr, "ERROR: There are not enough files in the directory, write max %d\n", n);
        return EXIT_FAILURE;
    }

    rewinddir(d);

    char* filesList[n];

    //Put file names into the array
    while ((dir = readdir(d)) != NULL)
    {
        if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, ".."))
        {
        }
        else {
            filesList[i] = (char*)malloc(strlen(dir->d_name) + 1);
            strncpy(filesList[i], dir->d_name, strlen(dir->d_name));
            i++;
        }
    }

    rewinddir(d);

    /*for (i = 0; i < n; i++)
        printf("%s\n", filesList[i]);*/

    int lower = 0, upper = n - 1;
    double time_spent = 0.0;

    printf("The read files are:\n");

    for (int i = 0; i < count; i++)
    {
        srand(time(0));
        int num = (rand() % (upper - lower + 1)) + lower;
        //printf("%s %d\n", filesList[num],num);
        size_t bytes_read;
        char* infile = filesList[num];
        FILE* fp = fopen(infile, "r");
        if (fp == NULL)
        {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
        fseek(fp, 0L, SEEK_END);
        size_t bytes_expected = ftell(fp);

        char* data = malloc((sizeof(char) * bytes_expected));

        if (data == NULL)
        {
            perror("malloc");
            fclose(fp);
            free(data);
            exit(EXIT_FAILURE);
        }

        fseek(fp, 0L, SEEK_SET);

        srand(time(0));
        clock_t begin = clock();

        bytes_read = fread(data, sizeof(char), bytes_expected, fp);

        clock_t end = clock();
        time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

        if (bytes_read != bytes_expected)
        {
            perror("fread");
            fclose(fp);
            free(data);
            exit(EXIT_FAILURE);
        }

        free(data);
        fclose(fp);
        printf("Read %ld bytes of the file %s\n", bytes_read, filesList[num]);

    }

    printf("Time elapsed is %f seconds.\n", time_spent);
    exit(EX_OK);
}