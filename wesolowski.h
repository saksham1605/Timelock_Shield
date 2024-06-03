#ifndef WESOLOWSKI_H
#define WESOLOWSKI_H

#include <gmp.h>
#include <vector>
#include <chrono>

#include "proof.h"
//ADDED CODE HERE
#define DEF_TESTIME 1 /* seconds */
#define SQUARES_PER_CICLE 1000UL /* squares to do each cicle */
#define PRIME_LEN 512 /* prime bit length */
#define BASE10 10
#define BASE16 16


class Wesolowski {
public:
Wesolowski();

void setup(unsigned long lambda, unsigned long k);
void generate(void);
void test_perf(void);
void encrypt(mpz_t message);
void unsetup(void);
void generate_prime(mpz_t& rop, gmp_randstate_t& rstate, const mp_bitcnt_t& n);
Proof evaluate(mpz_t l, mpz_t pi, mpz_t t,mpz_t a, const long challenge);
bool naive_verify(mpz_t a, mpz_t l, mpz_t pi);
std::chrono::duration<double> setup_time;
std::chrono::duration<double> eval_time;
std::chrono::duration<double> proof_time;
std::chrono::duration<double> verif_time;
std::chrono::duration<double> verif_time_opti;
unsigned long int time_enc, test_time;
// static mpz_t n;
mpz_t Ck, b, a, e, t;
mpz_t apowerq;
mpz_t lvalue;
mpz_t N;
mpz_t p;
mpz_t q;
mpz_t fi_n;
private:
mpz_t y_saved;


int k;
int lambda;
mpz_t challenge;


unsigned long int S;

gmp_randstate_t rstate;

void HashG(mpz_t& dest, mpz_t hashed);
void hash_prime(mpz_t l);

void unit_test();

int bit_length;
};

#endif
