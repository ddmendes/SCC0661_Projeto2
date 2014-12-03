

:all
    gcc freqlist.c -c -g 
    gcc huffman.c -c -g 
    gcc arraylist.c -c -g 
    gcc test.c -o test.exe *.o

:clean
    rm *.o test.exe

