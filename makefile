

all:	hello

hello:	gcc freqlist.c -c -g -Wall \
    	gcc huffman.c -c -g -Wall  \
     	gcc arraylist.c -c -g -Wall\
     	gcc WAVFile.c -c -g -Wall  \
     	gcc CWVFile.c -c -g -Wall  \
     	gcc main.c -o cwv.exe -g -Wall *.o

clean: 	rm *.o cwv.exe

