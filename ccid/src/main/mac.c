
#define BYTE unsigned char
#define WORD unsigned short
#define ADWORD unsigned int
#define DWORD unsigned long

#define code
#define data
#define idata
#define xdata
#define large

#define MYREG16(addr)  (((WORD) (*((unsigned char volatile *)(addr))) <<8) | (*((unsigned char volatile *)(addr+1))))
#define MYREG32(addr)  (((unsigned long)(MYREG16(addr)) << 16) | (MYREG16(addr + 2)))

#define A_MASK 0x70

#define A_DES       0x00
#define A_3DES2     0x10
#define A_SSF33     0x20
#define A_SCB2      0x30
#define A_3DES      0x10
#define A_SM4       0x50


extern void  SMS4_Decrypt(unsigned char *m ,unsigned char * k, unsigned char * e,int  n,int * rn);
extern void  SMS4_Encrypt(unsigned char *m ,unsigned char * k, unsigned char * e,int  n,int * rn);




 void API_MAC( unsigned char xdata *msg, unsigned char xdata *key, unsigned char xdata *mac)
{
    unsigned char xdata tmp[16], last_block[16];
    unsigned char  data i, j, k_len;
    unsigned char  data num_blk, tmp_byte;
    WORD len_msg;
	int rn;

    k_len = key[0];
    key[0] = 0x08;

    //len_msg = *msg; 
    len_msg = MYREG16(msg);
    
    num_blk = (len_msg >> 4);
    num_blk++;

    for (i = 0; i < 16; i++)
    {
        tmp[i] = 0;
        last_block[i] = 0;
    }

    tmp_byte = (len_msg & 0x0f);
    //msg++;
    msg += 2;
    
    if (tmp_byte > 0)
    {
        APL_memcpy(last_block, msg + (len_msg & 0xFFF0), tmp_byte);
    }
    last_block[tmp_byte] = 0x80;

    if (num_blk > 1 )
    { 
        for (i = 0; i < num_blk - 1; i++)
        {
            for (j = 0; j < 16; j++)
            { 
                tmp[j] = tmp[j] ^ msg[j];
            }    
           // APL_ALG_Symm(0x30, tmp, 1, &key[1], tmp);  //SM1
		   SMS4_Encrypt(tmp,&key[1],tmp,16,&rn);
           msg += 16;
        }
    }

    for (j = 0; j < 16; j++)  //the last block
    { 
        tmp[j] = tmp[j] ^ last_block[j];
    }    

    //APL_ALG_Symm(0x30, tmp, 1, &key[1], last_block);
	SMS4_Encrypt(tmp,&key[1],last_block,16,&rn);

    APL_memcpy(mac + 1, last_block, 4);
}




