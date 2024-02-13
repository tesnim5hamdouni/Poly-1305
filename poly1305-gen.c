#include "poly1305.h"

/*
generate a MAC given a key and a message
*/

int main(int argc, char * argv[]){
#if PROD
    if (argc !=3){
        printf("Usage: ./poly1305-gen <key> <file_name>\n");
        return 1;
    }

    char * input = argv[1];
    char * col_free = malloc(65);
    int j = 0;
    for (int i=0; i<strlen(input); i++){
        if (input[i] != ':'){
            col_free[j] = input[i];
            j++;
        }
    }
    if (strlen(col_free) != 64){
        printf("Key must be 32 long octet string! \n");
        return 1;
    }

    FILE * file = fopen(argv[2], "r");
    if (file == NULL){
        printf("File not found\n");
        return 1;
    }

    char * mac = malloc(33);
    poly1305_gen(col_free, file, mac);
    print_mac(mac);

    free(col_free);
    free(mac);
    fclose(file);   

#endif

#if DEBUG
    char * input = "85:d6:be:78:57:55:6d:33:7f:44:52:fe:42:d5:06:a8:01:03:80:8a:fb:0d:b2:fd:4a:bf:f6:af:41:49:f5:1b";
    char * col_free = malloc(65);
    char * mac = malloc(33);
    int j = 0;
    for (int i=0; i<strlen(input); i++){
        if (input[i] != ':'){
            col_free[j] = input[i];
            j++;
        }
    }

    poly1305_gen(col_free, file, mac);
    printf("MAC: ");
    printf("%s\n", mac);

    free(col_free);


#endif

    return 0;
}