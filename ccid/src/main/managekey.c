
 #include "mem.h"

#define  ROOTKEY_ADDR    0x1000
#define  USEDKEY_ADDR    0x1040
#define  USEDKEY1_ADDR    0x1080
#define  USEDKEY2_ADDR    0x10C0

extern  unsigned char API_WriteData(DWORD adwAddr, BYTE * pucBuf, UINT16 ucLen);
extern void API_ReadData(DWORD adwAddr,BYTE *pucBuf,WORD ucLen);


unsigned char SaveKey(unsigned char * key,unsigned char index,unsigned char flag)
{

    UINT16 klen; 
	unsigned char index1;

	index1 =  0;

    if(flag==0)
	{
	
			if(key[0]==0x39)
			{
		
				     klen = key[7];

					 
                    if(klen>16)
			        {
			 
			            klen=16;
			 
			        }

				     API_WriteData(ROOTKEY_ADDR+index1,key,klen+8);
			
			}
			else if(key[0]==0x36)
			{
			
				     klen = key[7];
					 
                    if(klen>16)
			        {
			 
			            klen=16;
			 
			        }
				     API_WriteData(USEDKEY_ADDR+index1,key,klen+8);	
		
			}
			else if(key[0]==0x3A)
			{
			
				     klen = key[7];
					 
                    if(klen>16)
			        {
			 
			            klen=16;
			 
			        }
				     API_WriteData(USEDKEY1_ADDR+index1,key,klen+8);	
		
			}
			else if(key[0]==0x30)
			{
			
				     klen = key[7];
					 
                    if(klen>16)
			        {
			 
			            klen=16;
			 
			        }
				     API_WriteData(USEDKEY2_ADDR+index1,key,klen+8);	
		
			}
	}
	else if(flag==1)
	{
		  index1 = 8;
		  			klen = 16;
					 
                    if(klen>16)
			        {
			 
			            klen=16;
			 
			        }
		  API_WriteData(ROOTKEY_ADDR+index1,key,klen);
	
	}
	else if(flag==2)
	{
		   index1 = 8;
		   		  			klen = 16;
					 
                    if(klen>16)
			        {
			 
			            klen=16;
			 
			        }
		  API_WriteData(USEDKEY_ADDR+index1,key,klen);
	
	}
 	else if(flag==3)
	{
		   index1 = 8;
		   		  			klen = 16;
					 
                    if(klen>16)
			        {
			 
			            klen=16;
			 
			        }
		  API_WriteData(USEDKEY1_ADDR+index1,key,klen);
	
	}
    else if(flag==4)
	{
		   index1 = 8;
		   		  			klen = 16;
					 
                    if(klen>16)
			        {
			 
			            klen=16;
			 
			        }
		  API_WriteData(USEDKEY2_ADDR+index1,key,klen);
	
	}
	 return 0;

}


extern unsigned char  API_Key_mode;

unsigned char GetKey(unsigned char * key,unsigned char flag,unsigned char index)
{

    UINT16 klen; 
	unsigned char index1;

	index1=0;

	if(flag==0)
	{


		  	 API_ReadData(ROOTKEY_ADDR+index1,key,8);
			 klen = key[7];

			 API_Key_mode = key[5];

             if(klen>16)
			 {
			 
			   klen=16;
			 
			 }

			 API_ReadData(ROOTKEY_ADDR+index1+8,key+8,klen);

	
	}
	else if(flag==1)
	{

		  	 API_ReadData(USEDKEY_ADDR+index1,key,8);
			 klen = key[7];

			 API_Key_mode = key[5];

			 if(klen>16)
			 {
			 
			   klen=16;
			 
			 }
			 API_ReadData(USEDKEY_ADDR+index1+8,key+8,klen);

		 
	}
	else if(flag==2)
	{

		  	 API_ReadData(USEDKEY1_ADDR+index1,key,8);
			 klen = key[7];

			 API_Key_mode = key[5];

			 if(klen>16)
			 {
			 
			   klen=16;
			 
			 }
			 API_ReadData(USEDKEY1_ADDR+index1+8,key+8,klen);

		 
	}
	else if(flag==3)
	{

		  	 API_ReadData(USEDKEY2_ADDR+index1,key,8);
			 klen = key[7];

			 API_Key_mode = key[5];

			 if(klen>16)
			 {
			 
			   klen=16;
			 
			 }
			 API_ReadData(USEDKEY2_ADDR+index1+8,key+8,klen);

		 
	}

	 return 0;

}

unsigned char IsRootKeyHas(unsigned char ctl)
{
	unsigned char key[8];
	unsigned char klen;
	klen = 8;

	if(ctl&0x01)
	{
   
	   API_ReadData(ROOTKEY_ADDR,key,klen);
	   if((key[0]==0x39)&&(key[1]==0x00))
	   {
	   	  //return 1; 
	   }
	   else
	   {
	      return 0;
	   }
	}


	if(ctl&0x02)
	{
   	   API_ReadData(USEDKEY_ADDR,key,klen);
	   if((key[0]==0x36)&&(key[1]==0x00))
	   {
	   	  //return 1; 
	   }
	   else
	   {
	      return 0;
	   }
	 }

  	if(ctl&0x04)
	{
	   API_ReadData(USEDKEY1_ADDR,key,klen);
	   if((key[0]==0x30)&&(key[1]==0x00))
	   {
	   	  //return 1; 
	   }
	   else
	   {
	      return 0;
	   }
	}

	if(ctl&0x08)
	{
	   API_ReadData(USEDKEY2_ADDR,key,klen);
	   if((key[0]==0x3A)&&(key[1]==0x00))
	   {
	   	  //return 1; 
	   }
	   else
	   {
	      return 0;
	   }
	}


	   return 1;


}

void InitRootKey(void)
{
	unsigned char key[8];
	unsigned char klen;

	key[0]=0xFF;
	key[1]=0xFF;
	key[2]=0xFF;
	key[3]=0xFF;
	key[4]=0xFF;
	key[5]=0xFF;
	key[6]=0xFF;
	key[7]=0xFF;


    API_WriteData(ROOTKEY_ADDR,key,8);
	API_WriteData(USEDKEY_ADDR,key,8);
}


unsigned char CheckKey(unsigned char * key1, unsigned char * key2, unsigned char keylen)
{
  unsigned char i;

  for(i=0;i<keylen;i++)
  {
	 if(key1[i]!=key2[i])
	 {
	    return 0;
	 }
  }

	return 1;


}


unsigned char API_Key_mode;

void  API_Encrypt(unsigned char *m ,unsigned char * k, unsigned char * e,int  n,int * rn)
{

   if(API_Key_mode == 0)
   {
   
   	   DES_Triple_Encrypt(m ,k, e, n,rn);
   
   }
   else if(API_Key_mode == 1)
   {
   
   		SMS4_Encrypt(m ,k, e, n,rn);
   
   }


}

void  API_Decrypt(unsigned char *m ,unsigned char * k, unsigned char * e,int  n,int * rn)
{

   if(API_Key_mode == 0)
   {
   
   		 DES_Triple_Decrypt(m , k,e,n,rn);
   
   }
   else if(API_Key_mode == 1)
   {
   
   		 SMS4_Decrypt(m , k,  e, n, rn);
   
   }



}







