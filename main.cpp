#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <gmp.h>
#include <sysexits.h>
#include <string.h>
#include "wesolowski.h"

using namespace std;
// argv arguments :
//    t : log2 of difficulty (must be an integer)
//    lambda : length of modulus
//    k : bit-level security of hashing function
//    Size of Lenstra window w
static void usage(void)
{
        puts("Encrypts base 10 numerical data with time-lock algorithm.\n");
        puts("Usage: ./vdf [-h] [-v] [-t test_time] [-i input_data_file] [-d difficulty(30)] [-l lambda(128)] [-k bit-level secutity(128)]");
        exit(EX_USAGE);
}
int main(int argc, char *argv[])
{

        // Argument parsing
        unsigned long int test_time = 0, time_enc = 0,x=0;
        unsigned long lambda=512,k=512;
        int w=0;
        mpz_t message;
        mpz_init(message);
        FILE *input_file = NULL,*output_file=NULL;
        for (int i = 1; i < argc; i++)
        {
                if (!strcmp(argv[i], "-h"))
                {
                        usage();
                }
                else if (!strcmp(argv[i], "-v"))
                {
                        puts("VDF v 1.0");
                        exit(EX_OK);
                }
                else if (!strcmp(argv[i], "-t"))
                {
                        time_enc = strtoul(argv[++i], NULL, BASE10);
                }
                else if (!strcmp(argv[i], "-x"))
                {
                        x = strtoul(argv[++i], NULL, BASE10);
                }
                else if (!strcmp(argv[i], "-i"))
                {
                        input_file = fopen(argv[++i], "r");
                        if (!input_file)
                        {
                                fprintf(stderr, "Error opening %s\n", argv[i]);
                                exit(EX_NOINPUT);
                        }
                        if (input_file)
                        {
                                gmp_fscanf(input_file, "%Zd", message);
                        }
                        else
                        {
                                puts("Enter data to encrypt (should be base 10 number):");
                                gmp_scanf("%Zd", message);
                        }
                }
                else if (!strcmp(argv[i], "-l"))
                {
                        lambda = strtoul(argv[++i], NULL, BASE10);
                }
                else if (!strcmp(argv[i], "-k"))
                {
                        k = strtoul(argv[++i], NULL, BASE10);
                }
        }


        srand(time(NULL));

        Wesolowski vdf = Wesolowski();

        // Running the Setup phase of algorithm
        vdf.setup(lambda, k);
        // Drawing a random input from the RSA group
        
        // mpz_t x;
        // mpz_init(x);
        vdf.generate();
        vdf.test_perf();
        vdf.time_enc = time_enc;
        vdf.encrypt(message);

        // Here is the evaluation part. We combine the evaluation and proof however there are two chrono for the separate phases.
        mpz_t l, pi;
        mpz_init(l);
        mpz_init(pi);
        // mpz_set_ui(vdf.t,x);
        cout<<"T: "<<vdf.t<<endl;
        vdf.evaluate(l, pi, vdf.t, vdf.a, pow(2, lambda));

        // Here we run the naive and optimized verifications
        bool result_verif;
        if (w == 0)
        {
                result_verif = vdf.naive_verify(vdf.a, vdf.lvalue, vdf.apowerq);
                // result_verif = vdf.naive_verify(vdf.a, vdf.lvalue, vdf.a);
        }
        // std::ofstream file;
        // file.open("result/" + std::to_string(t) + "_" +std::to_string(lambda) + "_" + std::to_string(k) + "_" + std::to_string(w) + ".csv",std::ofstream::out | std::ofstream::app);

        // file << vdf.setup_time.count() << ";" << vdf.eval_time.count() << ";"
        //      << vdf.proof_time.count() << ";" << vdf.verif_time.count() << "\n";
        // std::cout << vdf.verif_time.count() << std::endl;

        // file.close();
        vdf.unsetup();
        if(input_file) fclose(input_file);
        return 0;
}
