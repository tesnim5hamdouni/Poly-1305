#include "poly1305.h"

/*
verify the MAC provided given the key and the message
*/

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: ./poly1305-check <key> <message> <MAC>\n");
        return 1;
    }

    char *input = argv[1];
    char *col_free = malloc(65);
    int j = 0;
    for (int i = 0; i < strlen(input); i++)
    {
        if (input[i] != ':')
        {
            col_free[j] = input[i];
            j++;
        }
    }
    if (strlen(col_free) != 64)
    {
        printf("Key must be 32-byte long octet string! \n");
        return 1;
    }

    FILE *file = fopen(argv[2], "r");
    if (file == NULL)
    {
        printf("File not found\n");
        return 1;
    }

    char *mac = argv[3];
    char *mac_col_free = malloc(33);
    j = 0;
    for (int i = 0; i < strlen(mac); i++)
    {
        if (mac[i] != ':')
        {
            mac_col_free[j] = mac[i];
            j++;
        }
    }
    if (strlen(mac_col_free) != 32)
    {
        printf("MAC must be 16-byte long! \n");
        return 1;
    }

    poly1305_check(col_free, file, mac_col_free);

    return 0;
}