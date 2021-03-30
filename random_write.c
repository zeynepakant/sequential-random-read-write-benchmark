#include <stdio.h>
#include <stdlib.h>
#include <err.h>
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
    int numFiles = atoi(argv[2]);
    size_t bytes_expected = atol(argv[3]);
    double time_spent = 0.0;
    int lower = 0, upper = 10;

    for (int i = 0; i < numFiles; i++)
    {        
        int num = (rand() % (upper - lower + 1)) + lower;
        FILE *fp;
        char *fileSpec = (malloc)(sizeof(char)*5);
        snprintf(fileSpec, 5, "%d", num);
        //printf("%s\n", fileSpec);

        fp = fopen(fileSpec, "w+b");
        if (fp == NULL)
        {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
        
        size_t bytes_wrote;
        char* data = malloc((sizeof(char) * bytes_expected));

        if (data == NULL)
        {
            perror("malloc");
            fclose(fp);
            free(data);
            exit(EXIT_FAILURE);
        }

        srand(time(0));
        clock_t begin = clock();

        bytes_wrote = fwrite(data, sizeof(char), bytes_expected, fp);

        clock_t end = clock();
        time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

        if (bytes_wrote != bytes_expected)
        {
            perror("fwrite");
            fclose(fp);
            free(data);
            exit(EXIT_FAILURE);
        }

        fclose(fp);
        free(data);
        printf("Wrote %ld bytes of the file %s\n", bytes_wrote, fileSpec);
   
    }

    printf("\nBENCHMARK IS SUCCESSFULL \nTime elapsed is %f seconds.\n", time_spent);
    exit(EX_OK);
}