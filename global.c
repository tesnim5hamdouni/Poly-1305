#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <gmp.h>
#include "poly1305.h"


#define BLOCK_SIZE 16
#define DEBUG 0
#define PROD 1


/*
extract from the first 32 bytes, the 16-byte r and s values
*/
void extract_rs(char * key, mpz_t r, mpz_t s){
    char * r_str = malloc(33);
    char * s_str = malloc(33);
    memcpy(r_str, key, 32);
    memcpy(s_str, key+32, 32);
    r_str[32] = '\0';
    s_str[32] = '\0';

    //reverse endianness naively
    char tmp, tmp2;
    for (int i=0; i<16; i+=2){
        tmp = r_str[i];
        tmp2 = r_str[i+1];
        r_str[i] = r_str[31-i-1];
        r_str[i+1] = r_str[31-i];
        r_str[31-i-1] = tmp;
        r_str[31-i] = tmp2;

        tmp = s_str[i];
        tmp2 = s_str[i+1];
        s_str[i] = s_str[31-i-1];
        s_str[i+1] = s_str[31-i];
        s_str[31-i-1] = tmp;
        s_str[31-i] = tmp2;
    }

    mpz_set_str(r, r_str, 16);
    mpz_set_str(s, s_str, 16);

#if DEBUG
    gmp_printf("inside extract_rs r: %Zx\n", r);
    gmp_printf("inside extract_rs s: %Zx\n", s);
#endif
 
    free(r_str);
    free(s_str);
}

/*
clamp r
*/
void clamp(mpz_t r){
    mpz_t mask;
    mpz_init_set_str(mask, "0ffffffc0ffffffc0ffffffc0fffffff", 16);
    mpz_and(r, r, mask);
#if DEBUG
    gmp_printf("inside clamp r: %Zx\n", r);
#endif
    mpz_clear(mask);
}

/*
generating MAC
*/
void poly1305_mpz(char * key, FILE * file, char * mac, mpz_t a){
    mpz_t r, s;
    mpz_inits(r,s,NULL);
    extract_rs(key, r, s);
    clamp(r);

    mpz_t p, mpz_block ,tmp; 
    mpz_inits(p,tmp,mpz_block,NULL);
    mpz_set_str(p, "3fffffffffffffffffffffffffffffffb", 16); //p is 2^130-5
    mpz_set_ui(a,0); //a is set to 0

    char * block = malloc(BLOCK_SIZE+1); //block is 16 bytes + 1 bit
    int read = 0;
    while ((read = fread(block, 1, BLOCK_SIZE, file)) > 0){
        //notice in RFC that the message is treated as ascii string in little endian, hence mpz_import with order -1
        mpz_import(mpz_block, read, -1, 1, 0, 0, block); 
        mpz_set_ui(tmp, 1);
        mpz_mul_2exp(tmp, tmp, 8*read); //for the 1 bit at the end
        mpz_add(mpz_block, mpz_block, tmp); 
#if DEBUG
        gmp_printf("mpz block padded: %Zx\n", mpz_block);
#endif
        mpz_add(a, a, mpz_block);
#if DEBUG
        gmp_printf("acc+bloc: %Zx\n", a);
#endif
        mpz_mul(a, a, r);
#if DEBUG
        gmp_printf("acc*r: %Zx\n", a);
#endif
        mpz_mod(a, a, p);
#if DEBUG
        gmp_printf("acc*r mod p: %Zx\n--------------\n", a);
#endif
    }

    mpz_add(a, a, s);
    mpz_mod(a, a, p);

    free(block);
    mpz_clears(r,s,p,tmp,mpz_block,NULL);
       
}



void poly1305_gen(char * key, FILE * file, char * mac){
    mpz_t a;
    mpz_init(a);
    poly1305_mpz(key, file, mac, a);
    //convert to string
    char * a_str = mpz_get_str(NULL, 16, a);
    //reverse endianness
    for (int i=0; i<32; i+=2){
        mac[i] = a_str[32-i-1];
        mac[i+1] = a_str[32-i];
    }
    mac[33] = '\0';

    free(a_str);
    mpz_clear(a);
}

void poly1305_check(char * key, FILE * file, char * mac){
    char * mac_gen = malloc(33);
    poly1305_gen(key, file, mac_gen);
    if (strcmp(mac, mac_gen) == 0){
        printf("ACCEPT\n");
    } else {
        printf("REJECT\n");
    }
    free(mac_gen);
}




/*
print result
*/
void print_mac(char * mac){
    printf("\n");
    for (int i=0; i<30; i+=2){
        printf("%c%c:", mac[i], mac[i+1]);
    }
    printf("%c%c\n", mac[30], mac[31]);
}

