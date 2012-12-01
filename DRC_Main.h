#ifndef _DRC_Main_p
#define _DRC_Main_p

char* Terminate(char* start,char* fence);
void PrtVersion(FILE* fh);
bool MatchStr(char *str1,char *str2);
bool atob(char* str);
void ProcessDRCCmd(t_DRCCmd* Cmd);
void Load_Database(void);
void Init(void);
int pfprintf(FILE* fh,const char *fmt, ...);
void pfputc(char chr,FILE *fh);

#endif // _DRC_Main_p
