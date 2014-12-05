SCC0661_Projeto2
================

Compressão e descompressão de arquivos .wav

Para compilar:

    $ gcc arraylist.c -c
    $ gcc freqlist.c -c
    $ gcc huffman.c -c
    $ gcc WAVFile.c -c
    $ gcc CWVFile.c -c
    $ gcc main.c -o cwv[.exe] *.o

Para executar:
    ./cwv [-c|-d] [arquivo de entrada] [arquivo de saida]
        -c para compressao (entrada WAV e saida CWV).
        -d para descompressao (entrada CWV e saida WAV).

Davi Di�rio Mendes - 7546989

