#include "stm32f10x_gpio.h"
#include "stm32f10x.h"
#include "stm32f10x_spi.h"


#define Select_IS8()     GPIO_ResetBits(GPIOC, GPIO_Pin_7)

#define NotSelect_IS8()    GPIO_SetBits(GPIOC, GPIO_Pin_7)


void IS8_Delay_emu(unsigned int var)
{
  unsigned int cnt,i;


 for(i=0;i<var;i++)
 {	
 
    cnt= 200;
    while(cnt--);
  }


}


void IS8_Rst(void)
{

	GPIO_SetBits(GPIOE, GPIO_Pin_8);

	IS8_Delay_emu(1000);

	GPIO_ResetBits(GPIOE, GPIO_Pin_8);

    IS8_Delay_emu(1000);
	IS8_Delay_emu(1000);

    GPIO_SetBits(GPIOE, GPIO_Pin_8);
	IS8_Delay_emu(1000);

}

 unsigned char IS8_TransferData(unsigned char * sbuffer, unsigned int slen, unsigned char * rbuffer, unsigned int  rlen,unsigned int timeout)
 {
 
 	 //	Select_IS8(); 
 		unsigned  int  i,j;

	//	i =  timeout;

 		ISU_SPI_sendNByte(sbuffer,slen);
 
		for(i=0;i<timeout;i++)
		{
		   j=20;
		  while(j--);
		}
 			  
 		ISU_SPI_recieveNByte(rbuffer,rlen);
 
	//	NotSelect_IS8(); 

 		return 0;
 
 
 }


 
  extern unsigned char SPI_recieveOneByte(void);
 unsigned char IS8_TransferData_RecievNBytes(unsigned char * sbuffer, unsigned int slen, unsigned char * rbuffer, unsigned int *rlen)
 {
 	   
	   unsigned int temp;

 	   ISU_SPI_sendNByte(sbuffer,slen);

	   temp = ISU_SPI_recieveOneByte();

	   ISU_SPI_recieveNByte(rbuffer+1,temp);
	
	   rbuffer[0]=temp;
       
	   *rlen =  temp+1;
	        
 	   return 0;
 
 }

  unsigned char IS8_TransferData_Recieving(unsigned char * sbuffer, unsigned int slen, unsigned char * rbuffer, unsigned int rlen,unsigned char flag,unsigned int num)
 {
 	   
	   unsigned int temp,i;

 	   ISU_SPI_sendNByte(sbuffer,slen);


	    for(i=0;i<num;i++)
		{

		   temp = 0;
	       temp = ISU_SPI_recieveOneByte();

			 if(flag==0xFF)
			 {
			 	if(temp !=0)
			  {
			  	  ISU_SPI_recieveNByte(rbuffer+1,rlen-1);
			  	  rbuffer[0]=temp;
				   return 0;
			  }
			 
			 }
			 else
			 {
			  if(temp ==flag)
			  {
			  	  ISU_SPI_recieveNByte(rbuffer+1,rlen-1);
			  	  rbuffer[0]=temp;
				   return 0;
			  }
			 }
 		}
	   
	
	   
       
	        
 	   return 1;
 
 }

  /*
 unsigned char  IS8_CheckStatus(unsigned char * buf)
 {
 	unsigned char rev[8],rev2[8],temp,i;
	


	 GPIO_SetBits(GPIOE, GPIO_Pin_8);
	for(i=0;i<8;i++)
	{
	   rev2[i]=0;
	
	}
	SPI_Delay_emu();
		for(i=0;i<8;i++)
	{
	   rev[i]=0;
	
	}
	 GPIO_ResetBits(GPIOE, GPIO_Pin_8);
	   SPI_Delay_emu();
	 	SPI_Delay_emu();

		 GPIO_SetBits(GPIOE, GPIO_Pin_8);

   	rev[0]= 0xFB;
	rev[1]= 0x33;
	rev[2]= 0x44;
	rev[3]= 0x55;
	rev[4]= 0x66;
	rev[5]= 0xBF;
		 SPI_Delay_emu();
	   SPI_Delay_emu();
	   	   SPI_Delay_emu();
		   	   SPI_Delay_emu();
			   	   SPI_Delay_emu();
				   	   SPI_Delay_emu();	   SPI_Delay_emu();
					   	   SPI_Delay_emu();
	Select_IS8();  
	//SPI_Delay_emu();
	temp = 0xFF;
 	SPI_sendNByte(rev,6);
   // NotSelect_IS8();
	SPI_Delay_emu();
    SPI_Delay_emu();  	   
	SPI_Delay_emu();
	SPI_Delay_emu();
		SPI_Delay_emu();
			SPI_Delay_emu();
				SPI_Delay_emu();	SPI_Delay_emu();
					SPI_Delay_emu();
						SPI_Delay_emu();
 
	temp = SPI_recieveNByte(buf,5);
	//SPI_Delay_emu();
	NotSelect_IS8(); 

	return rev2[0];
	  
	if(temp==1)
	{
	   return rev2;
	}

	if(temp==0)
	{
	
	  return 0;
	} 

   

	return 2;  
 
 }


  unsigned char  IS8_CheckStatus2(unsigned char * buf)
 {
 	unsigned char rev[8],rev2[8],temp,i;
	


	// GPIO_SetBits(GPIOE, GPIO_Pin_8);
	for(i=0;i<8;i++)
	{
	   rev2[i]=0;
	
	}
//	SPI_Delay_emu();
		for(i=0;i<8;i++)
	{
	   rev[i]=0;
	
	}
//	 GPIO_ResetBits(GPIOE, GPIO_Pin_8);
	//   SPI_Delay_emu();
	 //	SPI_Delay_emu();

//		 GPIO_SetBits(GPIOE, GPIO_Pin_8);

   	rev[0]= 0xFB;
	rev[1]= 0x00;
	rev[2]= 0x44;
	rev[3]= 0x00;
	rev[4]= 0x66;
	rev[5]= 0xD9;
		 SPI_Delay_emu();
	   SPI_Delay_emu();
	   	   SPI_Delay_emu();
		   	   SPI_Delay_emu();
			   	   SPI_Delay_emu();
				   	   SPI_Delay_emu();	   SPI_Delay_emu();
					   	   SPI_Delay_emu();
	Select_IS8();  
	//SPI_Delay_emu();
	temp = 0xFF;
 	SPI_sendNByte(rev,6);
   // NotSelect_IS8();
	SPI_Delay_emu();
    SPI_Delay_emu();  	   
	SPI_Delay_emu();
	SPI_Delay_emu();
		SPI_Delay_emu();
    SPI_Delay_emu();  	   
	SPI_Delay_emu();
	SPI_Delay_emu();
		SPI_Delay_emu();
    SPI_Delay_emu();  	   
	SPI_Delay_emu();
	SPI_Delay_emu();
		SPI_Delay_emu();
			SPI_Delay_emu();
				SPI_Delay_emu();	SPI_Delay_emu();
					SPI_Delay_emu();
						SPI_Delay_emu();
		 //  SPI_Delay_emu();
		 //  	   SPI_Delay_emu();
	//Select_IS8(); 
	temp = SPI_recieveNByte(buf,4);
	//SPI_Delay_emu();
	NotSelect_IS8(); 

	return rev2[0];
	  
	if(temp==1)
	{
	   return rev2;
	}

	if(temp==0)
	{
	
	  return 0;
	} 

   

	return 2;   
 }
*/



unsigned char IsIs8uStatuesModeReady(void)
{
   unsigned char temp_sbuf[3],temp_rbuf[3],num;

   temp_sbuf[0]=0xE5;
   num==0;
  do
  {
	IS8_Delay_emu(1);
    num++;
	if(num>100) return 1;
    IS8_TransferData(temp_sbuf,1,temp_rbuf,2,0);

  }while(temp_rbuf[0]!=0x01);

  return 0;
}

unsigned char Is8uXor(unsigned char * buf, unsigned int len)
{

	unsigned int  i;
	unsigned char  sum;

	sum = 0;

	for(i=0;i<len;i++)
	{
	   sum = sum ^ buf[i];
	
	}

	return sum;
}




