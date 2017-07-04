

extern  char APL_memcmp_Bychar(unsigned char * pbDestBuf, unsigned char * pbSrcBuf,unsigned short ucLen);
extern void APL_memcpy(unsigned char * ucDestination, unsigned char * ucSource, unsigned short ucLen);
extern void APL_memset(unsigned char * ucDestination,unsigned char ucVal,unsigned short ucLen);

#define  memcmp APL_memcmp_Bychar
#define  memcpy APL_memcpy
#define  memset APL_memset
