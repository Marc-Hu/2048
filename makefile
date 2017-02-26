all: run

run: clearMain compile
	./mainFinal

compile:
	gcc mainFinal.c -Wall -o mainFinal

clearMain:
	rm -rf mainFinal
