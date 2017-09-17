all: y86emul y86dis

clean:
	rm y86emul
	rm y86dis


y86emul: y86emul.h y86emul.c
	gcc -o y86emul -Wall -lm y86emul.c

y86dis: y86emul.h y86dis.c
	gcc -o y86dis -Wall -lm y86dis.c

