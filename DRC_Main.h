#ifndef _DRC_Main_p
#define _DRC_Main_p

void PrtVersion(FILE* fh);
void ProcessDRCCmd(t_DRCCmd* Cmd);
void Load_Database(void);
int __attribute__((format (printf, 2, 3))) pfprintf(FILE* fh,const char *fmt, ...);
void pfputc(char chr,FILE *fh);

#endif // _DRC_Main_p
