#include <chrono>
#include <cmath>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include "wesolowski.h"
#include <thread>
#include <gmpxx.h>
using namespace std;

// ADDED CODE HERE
#define DEF_TESTIME 1            /* seconds */
#define SQUARES_PER_CICLE 1000UL /* squares to do each cicle */
#define PRIME_LEN 512            /* prime bit length */
#define BASE10 10
#define BASE16 16

void Wesolowski::generate_prime(mpz_t &rop, gmp_randstate_t &rstate, const mp_bitcnt_t &n)
{
        mpz_urandomb(rop, rstate, 2 * k);
        mpz_nextprime(rop, rop);
}

void Wesolowski::hash_prime(mpz_t l)
{
        mpz_urandomb(challenge, rstate, 3 * k);
        if (mpz_cmp_si(challenge, 0) == 0)
        {
                mpz_urandomb(challenge, rstate, 3 * k);
        }
        mpz_nextprime(l, challenge);
}

Wesolowski::Wesolowski()
{
}

void Wesolowski::setup(unsigned long _lambda, unsigned long _k)
{

        auto start = std::chrono::high_resolution_clock::now();

        mpz_init(a);
        mpz_init(b);
        mpz_init(e);
        mpz_init(t);
        mpz_init(Ck);
        mpz_init(lvalue);
        mpz_init(y_saved);

        lambda = _lambda;
        k = _k;
        mpz_init(challenge);
        k = _k;

        FILE *fp;
        unsigned long int random_seed;

        fp = fopen("/dev/urandom", "r");
        int temp = fscanf(fp, "%lu", &random_seed);
        fclose(fp);

        gmp_randinit_default(rstate);
        gmp_randseed_ui(rstate, random_seed);

        mpz_init(p);
        generate_prime(p, rstate, lambda / 2);
        mpz_init(q);
        generate_prime(q, rstate, lambda / 2);
        mpz_init(N);
        mpz_mul(N, p, q);

        // ADDING CODE TO MAKE IT SUITABLE FOR ENCRYPTION
        mpz_sub_ui(p, p, 1UL);
        mpz_sub_ui(q, q, 1UL);
        mpz_init(fi_n);
        mpz_mul(fi_n, p, q);

        auto finish = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
        time_enc = 10;
        mpz_clear(p);
        mpz_clear(q);
        cout << "Setup Time: " << duration.count()<<" micro seconds" << endl;
}

// Creates a random input for the VDF
void Wesolowski::generate(void)
{
        // ADDED CODE HER
        mpz_urandomb(a, rstate, lambda);
        mpz_sub_ui(N, N, 2UL);
        mpz_mod(a, a, N);
        mpz_add_ui(N, N, 2UL);
        mpz_add_ui(a, a, 2UL);
}

void Wesolowski::test_perf(void)
{
        unsigned long cicles = 0;
        clock_t t0, ticks, t_test;
        mpz_t x;
        mpz_init(x);
        mpz_set(x, a);
        test_time = 1; // DEFINE THE TEST TIME
        ticks = CLOCKS_PER_SEC * test_time;
        t0 = clock();
        t_test = t0 + ticks;
        do
        {
                mpz_set_ui(t, SQUARES_PER_CICLE);
                while (mpz_cmp_ui(t, 0) > 0)
                {
                        mpz_powm_ui(x, x, 2UL, N);
                        mpz_sub_ui(t, t, 1UL);
                }
                cicles++;
        } while (clock() < t_test);
        S = (unsigned long)((SQUARES_PER_CICLE * cicles) / test_time);
}

void Wesolowski::encrypt(mpz_t message)
{
        cout<<"\n============ENCRYPTION STARTED================"<<endl;
        auto start = std::chrono::high_resolution_clock::now();
        mpz_t T, two;
        mpz_init(T);
        mpz_init(two);
        /* calculate challenge to reach desired time */
        mpz_set_ui(T, time_enc);
        mpz_mul_ui(t, T, S);

        /* calculate b */
        mpz_set_ui(two, 2UL);
        mpz_powm(e, two, t, fi_n);
        mpz_powm(b, a, e, N);

        /* encrypt key with b */
        mpz_add(Ck, b, message);

        cout<<"Encrypted Message : "<<Ck<<endl;
        cout<<"Base : "<<a<<endl;
        // cout<<"T: "<<t<<endl;

        mpz_clear(T);
        mpz_clear(two);
        mpz_clear(message);
        mpz_clear(fi_n);

        auto finish = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
        cout << "Encryption Time: " << duration.count()<<" micro seconds" << endl;
        cout<<"\n============ENCRYPTION ENDED================"<<endl;
        
}

void Wesolowski::unsetup(void)
{
        mpz_clear(N);
        mpz_clear(Ck);
        mpz_clear(t);
        mpz_clear(a);
        mpz_clear(e);
        gmp_randclear(rstate);
        mpz_clear(apowerq);
        mpz_clear(lvalue);
        mpz_clear(y_saved);
        mpz_clear(challenge);
        mpz_clear(b);
}

Proof Wesolowski::evaluate(mpz_t l, mpz_t pi, mpz_t time, mpz_t base, const long challenge)
{

        // HERE WE START THE EVALUATION
        cout<<"\n============DECRYPTION STARTED================"<<endl;
        auto start_eval = std::chrono::high_resolution_clock::now();

        mpz_t decrypted_code;
        mpz_init(decrypted_code);
        mpz_t temp;
        mpz_init(temp);
        mpz_set(temp,time);
        mpz_t x;
        mpz_init(x);
        mpz_set(x,base);
        while (mpz_cmp_ui(time, 0UL))
        {
                mpz_powm_ui(base, base, 2UL, N);
                mpz_sub_ui(time, time, 1UL);
        }
        /* decrypt */
        mpz_sub(decrypted_code, Ck, base);
        mpz_mod(base,base,N);
        mpz_set(y_saved, base);
        mpz_set(base,x);
        auto finish_eval = std::chrono::high_resolution_clock::now();
        cout << "Message is:" << decrypted_code << endl;
        mpz_clear(decrypted_code);
        auto eval_time = std::chrono::duration_cast<std::chrono::seconds>(finish_eval - start_eval);
        // eval_time = finish_eval - start_eval;
        cout << "Puzzle Evaluation Time: " << eval_time.count()<<" seconds" << endl;
        cout<<"\n============DECRYPTION ENDED================"<<endl;

        // WE FINISHED THE EVALUATION

        // HERE WE START THE PROOF COMPUTATION
        hash_prime(l);
        mpz_mod(l, l, N);
        mpz_set(lvalue, l);
        cout<<"L value:"<<lvalue<<endl;
        cout<<"\n============PROOF COMPUTATION STARTED================"<<endl;
        auto start_proof = std::chrono::high_resolution_clock::now();


        mpz_t dividend;
        mpz_init(dividend);
        mpz_set_ui(dividend, 1);
        mpz_mul_2exp(dividend, dividend, mpz_get_ui(temp));
        mpz_t quotient;
        mpz_t remainder;

        mpz_init(quotient);
        mpz_init(remainder);

        mpz_fdiv_qr(quotient, remainder, dividend, lvalue);
        mpz_init(apowerq);
        mpz_powm(apowerq, a, quotient, N);
        mpz_set(pi, apowerq);
        mpz_set(t,temp);

        // cout<<"Pi : "<<apowerq<<endl;

        // this_thread::sleep_for(std::chrono::seconds(5));

        auto finish_proof = std::chrono::high_resolution_clock::now();
        auto proof_time = std::chrono::duration_cast<std::chrono::microseconds>(finish_proof - start_proof);
        cout << "Proof generation Time: " << proof_time.count()<<" micro seconds" << endl;
        cout<<"\n============PROOF COMPUTATION ENDED================"<<endl;
        Proof proof_sent = Proof();
        return proof_sent;
}

bool Wesolowski::naive_verify(mpz_t a, mpz_t l, mpz_t pi)
{
        cout<<"\n============VERIFICATION STARTED================"<<endl;
        auto start_verif = std::chrono::high_resolution_clock::now();

        mpz_t remainder;
        mpz_t two;
        mpz_init(two);
        mpz_set_ui(two, 2UL);
        mpz_init(remainder);
        mpz_powm(remainder, two, t, l);

        std::chrono::duration<double> exp_time;

        auto start_exp = std::chrono::high_resolution_clock::now();

        mpz_t y, y_tmp;
        mpz_init(y);
        mpz_init(y_tmp);
        mpz_powm(y, pi, l, N);
        mpz_powm(y_tmp, a, remainder, N);
        mpz_mul(y, y, y_tmp);
        mpz_mod(y, y, N);
        cout<<"N :" <<N<<endl;
        auto finish_exp = std::chrono::high_resolution_clock::now();
        cout<<"Y_SAVED "<<y_saved<<endl;
        if (mpz_cmp(y, y_saved) == 0)
        {
                auto finish_verif = std::chrono::high_resolution_clock::now();

                verif_time = std::chrono::duration_cast<std::chrono::microseconds>(finish_verif - start_verif);
                cout << "Verified\n";
                cout << "Verification Time: " << verif_time.count()<<" micro seconds" << endl;
                exp_time = std::chrono::duration_cast<std::chrono::microseconds>(finish_exp - start_exp);
                std::cout << "Exponentiation Time : " << exp_time.count()<<" micro seconds" << std::endl;
                cout<<"\n============VERIFICATION ENDED================"<<endl;
                return 1;
        }
        else
        {
                std::cout << "Not Verified" << std::endl;
                cout<<"\n============VERIFICATION ENDED================"<<endl;
                exit(1);
                return 0;
        }
        

}
