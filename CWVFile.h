
/**
 * Comprime WAV em CWV.
 * @param cwv ponteiro de escrita para arquivo CWV.
 * @param wav ponteiro de leitura para arquivo WAV.
 */
void CWVCompress(FILE * cwv, FILE * wav);

/**
 * Descmprime CWV em WAV.
 * @param wav ponteiro de escrita para arquivo WAV.
 * @param cwv ponteiro de leitura para arquivo CWV.
 */
void CWVDecompress(FILE * wav, FILE * cwv);