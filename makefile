all: poly1305-gen poly1305-check

poly1305-gen: poly1305-gen.c global.c
	gcc -o poly1305-gen poly1305-gen.c global.c -lgmp

poly1305-check: poly1305-check.c global.c
	gcc -o poly1305-check poly1305-check.c global.c -lgmp
clean:
	rm -f poly1305-gen poly1305-check 