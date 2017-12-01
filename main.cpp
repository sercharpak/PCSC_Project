#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sndfile.h>

#include <iostream>

#define BLOCK_SIZE 512
#define ARRAY_LEN(x)    ((int) (sizeof (x) / sizeof (x [0])))
#define MAX(x,y)        ((x) > (y) ? (x) : (y))
#define MIN(x,y)        ((x) < (y) ? (x) : (y))

static void print_usage(char *progname) {
    printf("\nUsage : %s <input file> <output file>\n", progname);
    puts("\n"
                 "    Where the output file will contain a line for each frame\n"
                 "    and a column for each channel.\n");

} /* print_usage */

static void convert_to_text(SNDFILE *infile, FILE *outfile, int channels) {
    double buf[channels * BLOCK_SIZE];
    int k, m, readcount;

    while ((readcount = sf_readf_double(infile, buf, BLOCK_SIZE)) > 0) {
        std::cout<<readcount<<std::endl;
        for (k = 0; k < readcount; k++) {
            for (m = 0; m < channels; m++){
                fprintf(outfile, " % 12.10f", buf[k * channels + m]);
            }

            fprintf(outfile, "\n");
        };
    };

    return;
} /* convert_to_text */


int main(int argc, char *argv[]) {
    char *progname, *infilename, *outfilename;
    SNDFILE *infile = NULL;
    SNDFILE *outfile = NULL;
    FILE *outfile2 = NULL;
    SF_INFO sfinfo;

    double buffer [1024] ;
    sf_count_t count ;

    progname = strrchr(argv[0], '/');
    progname = progname ? progname + 1 : argv[0];

    if (argc != 3) {
        print_usage(progname);
        return 1;
    };

    infilename = argv[1];
    outfilename = argv[2];

    if (strcmp(infilename, outfilename) == 0) {
        printf("Error : Input and output filenames are the same.\n\n");
        print_usage(progname);
        return 1;
    };

    if (infilename[0] == '-') {
        printf("Error : Input filename (%s) looks like an option.\n\n", infilename);
        print_usage(progname);
        return 1;
    };

    if (outfilename[0] == '-') {
        printf("Error : Output filename (%s) looks like an option.\n\n",
               outfilename);
        print_usage(progname);
        return 1;
    };

    memset(&sfinfo, 0, sizeof(sfinfo));

    if ((infile = sf_open(infilename, SFM_READ, &sfinfo)) == NULL) {
        printf("Not able to open input file %s.\n", infilename);
        puts(sf_strerror(NULL));
        return 1;
    };

    /* Open the output file. */
    if ((outfile2 = fopen("outfile2.txt", "w")) == NULL) {
        printf("Not able to open output file %s : %s\n", "outfile2.txt",
               sf_strerror(NULL));
        return 1;
    };
    if ((outfile = sf_open (argv [2], SFM_WRITE, &sfinfo)) == NULL) {
        printf ("Error : Not able to open output file '%s'\n", argv [argc - 1]);
        sf_close (infile);
        exit (1);
    }
    //std::cout<<sfinfo.frames<<std::endl;
    while ((count = sf_read_double (infile, buffer, ARRAY_LEN (buffer))) > 0) {
        //std::cout<<count<<std::endl;
        for (int i = 0; i < 1024; i++){
            for (int m = 0; m < 2; m++){
                std::cout<< buffer[i * 2 + m] <<" ";
            }
            std::cout<<std::endl;
            buffer[i] *= 0.5;
            //std::cout<<buffer[i]<<std::endl;
        }
        sf_write_double (outfile, buffer, count);
    }



    fprintf(outfile2, "# Converted from file %s.\n", infilename);
    fprintf(outfile2, "# Channels %d, Sample rate %d\n", sfinfo.channels,
            sfinfo.samplerate);

    convert_to_text(infile, outfile2, sfinfo.channels);

    sf_close(infile);
    sf_close(outfile);
    fclose(outfile2);

    return 0;
} /* main */