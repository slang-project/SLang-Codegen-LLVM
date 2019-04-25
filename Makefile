all: link run
	
compile:
	llc -filetype=obj out.ll

link:
	gcc out.o

run:
	./a.out