CC = gcc
CFLAGS = -Wall

all: calc

calc: parser.tab.c lex.yy.c
	$(CC) $(CFLAGS) -o calc parser.tab.c lex.yy.c -lm

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: lexer.l parser.tab.h
	flex lexer.l

clean:
	rm -f calc parser.tab.c parser.tab.h lex.yy.c

.PHONY: all clean
