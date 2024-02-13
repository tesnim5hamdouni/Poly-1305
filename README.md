# Poly-1305
An implementation of poly-1305 MAC following RFC 7539

## usage:
poly1305-gen should compute an authenticator on a file.  It will take two arguments:

    A 32-byte key, given as a 64-character hexadecimal string (which will be split into r and s)
    The name of the file to authenticate.

It should write a 16-byte authenticator to standard output, as a 32-character hexadecimal string.

poly1305-check should verify an authenticator on a file.  It should take three arguments:

    A 32-byte key, given as a 64-character hexadecimal string (which will be split into r and s) - i.e., the same key as in poly1305-gen
    The name of the file to authenticate
    A 16-byte authenticator tag, given as a 32-character hexadecimal string.

It should print out either ACCEPT or REJECT, according to whether the authenticator is valid or not.
