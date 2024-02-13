#ifndef POLY1305_H
#define POLY1305_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <gmp.h>

#define BLOCK_SIZE 16
#define DEBUG 0
#define PROD 1

void extract_rs(char * key, mpz_t r, mpz_t s);
void clamp(mpz_t r);
void poly1305_mpz(char * key, FILE * file, char * mac, mpz_t a);
void poly1305_gen(char * key, FILE * file, char * mac);
void print_mac(char * mac);
void poly1305_check(char * key, FILE * file, char * mac);


#endif  // POLY1305_H
