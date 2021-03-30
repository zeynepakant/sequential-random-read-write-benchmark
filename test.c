#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <fcntl.h>
#include <sysexits.h>
#include <unistd.h>
#include <time.h>

int main()
{
    int fd;
    size_t bytes_read;
    double* data;
    char* infile = "file.dat";
    FILE *fp = fopen(infile, "r");
    fseek(fp, 0L, SEEK_END);
    double time_spent = 0.0;

    // calculating the size of the file 
    long int bytes_expected = ftell(fp);

    if ((fd = open(infile, O_RDWR)) < 0)
        err(EX_NOINPUT, "%s", infile);

    if ((data = malloc(bytes_expected)) == NULL)
        err(EX_OSERR, "data malloc");

    clock_t begin = clock();

    bytes_read = read(fd, data, bytes_expected);

    clock_t end = clock();

    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

    if (bytes_read != bytes_expected)
        err(EX_DATAERR, "Read only %ld of %ld bytes",
            bytes_read, bytes_expected);

    printf("Read %ld bytes of the file\n", bytes_read);
    printf("\nBENCHMARK IS SUCCESSFULL \nTime elapsed is %f seconds.\n", time_spent);
    /* ... operate on data ... */

    free(data);

    exit(EX_OK);
}