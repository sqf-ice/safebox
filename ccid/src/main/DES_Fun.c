

void DES_En(unsigned char *m , unsigned char * k, unsigned char * e);
void DES_De(unsigned char *m , unsigned char * k, unsigned char * e);


void byte2bit(unsigned char * , unsigned char *) ;
void bit2byte(unsigned char * , unsigned char *) ;
//void keychange(unsigned char * , unsigned char **) ;
void keychange(unsigned char oldkey[8] , unsigned char newkey[16][8]);
void s_replace(unsigned char *) ;



void  DES_Encrypt(unsigned char *m , unsigned char * k, unsigned char * e,int n,int * rn) //按ECB方式进行加密
{
	int i,j,p,q;
	unsigned char dat0[8];

	if (n==0) return;    

    for(i=0;i<n;i++)  e[i]=m[i];
	p= n % 8; 
	q= (n-1) >> 3; //即 (n-1) 整除 8；
    *rn=8 * (q+1);

   if (p>0)
	 {
		 e[n]=0x80;
		 if (p<7) 	 for(i=1;i<= 7-p;i++) e[n+i]=0x00;
	 }     
   
   for(i=0;i<=q;i++)
   {
	   for(j=0;j<8;j++) dat0[j]=e[i*8+j];
	   DES_En(dat0,k,dat0); 
	   for(j=0;j<8;j++) e[i*8+j]=dat0[j];
   }

}

void  DES_Decrypt(unsigned char *m , unsigned char * k, unsigned char * e,int n,int * rn)
{
	int i,j,p,q;
	unsigned char dat0[8];

	if (n==0) return;    

    for(i=0;i<n;i++)  e[i]=m[i];
	p= n % 8; 
	q= (n-1) >> 3; //即 (n-1) 整除 8；
    *rn=8 * (q+1);

   if (p>0)
	 {
		 e[n]=0x80;
		 if (p<7) 	 for(i=1;i<= 7-p;i++) e[n+i]=0x00;
	 }     
   
   for(i=0;i<=q;i++)
   {
	   for(j=0;j<8;j++) dat0[j]=e[i*8+j];
	   DES_De(dat0,k,dat0); 	   
	   for(j=0;j<8;j++) e[i*8+j]=dat0[j];
   }

}


void  DES_Triple_Encrypt(unsigned char *m ,unsigned char * k, unsigned char * e,int  n,int * rn)
{
	int i,j,p,q;
	unsigned char dat0[8],kr[8];

	if (n==0) return;

    for(i=0;i<8;i++)  kr[i]=k[i+8];

    for(i=0;i<n;i++)  e[i]=m[i];
	p= n % 8; 
	q= (n-1) >> 3; //即 (n-1) 整除 8；
    *rn=8 * (q+1);

   if (p>0)
	 {
		 e[n]=0x80;
		 if (p<7) 	 for(i=1;i<= 7-p;i++) e[n+i]=0x00;
	 }     
   
   for(i=0;i<=q;i++)
   {
	   for(j=0;j<8;j++) dat0[j]=e[i*8+j];
	   DES_En(dat0,k,dat0); //左加
	   DES_De(dat0,kr,dat0);//右解
	   DES_En(dat0,k,dat0); //左加
	   for(j=0;j<8;j++) e[i*8+j]=dat0[j];
   }



}

void  DES_Triple_Decrypt(unsigned char *m ,unsigned char * k, unsigned char * e,int  n,int * rn)
{
	int i,j,p,q;
	unsigned char dat0[8],kr[8];

	if (n==0) return;

    for(i=0;i<8;i++)  kr[i]=k[i+8];

    for(i=0;i<n;i++)  e[i]=m[i];
	p= n % 8; 
	q= (n-1) >> 3; //即 (n-1) 整除 8；
    *rn=8 * (q+1);

   if (p>0)
	 {
		 e[n]=0x80;
		 if (p<7) 	 for(i=1;i<= 7-p;i++) e[n+i]=0x00;
	 }     
   
   for(i=0;i<=q;i++)
   {
	   for(j=0;j<8;j++) dat0[j]=e[i*8+j];
	   DES_De(dat0,k,dat0); //左解
	   DES_En(dat0,kr,dat0);//右加
	   DES_De(dat0,k,dat0); //左解
	   for(j=0;j<8;j++) e[i*8+j]=dat0[j];
   }

}



void DES_En(unsigned char m_bit[8] , unsigned char k_bit[8] , unsigned char e_bit[8])
{
int ip[64] = {  58,50,42,34,26,18,10,2,
		60,52,44,36,28,20,12,4,
		62,54,46,38,30,22,14,6,
		64,56,48,40,32,24,16,8,
		57,49,41,33,25,17,9,1,
		59,51,43,35,27,19,11,3,
		61,53,45,37,29,21,13,5,
		63,55,47,39,31,23,15,7
		} ;
int ip_1[64] = {40,8,48,16,56,24,64,32,
		39,7,47,15,55,23,63,31,
		38,6,46,14,54,22,62,30,
		37,5,45,13,53,21,61,29,
		36,4,44,12,52,20,60,28,
		35,3,43,11,51,19,59,27,
		34,2,42,10,50,18,58,26,
		33,1,41,9,49,17,57,25
		} ;
int e[48] = {   32,1, 2, 3, 4, 5,
		4, 5, 6, 7, 8, 9,
		8, 9, 10,11,12,13,
		12,13,14,15,16,17,
		16,17,18,19,20,21,
		20,21,22,23,24,25,
		24,25,26,27,28,29,
		28,29,30,31,32,1
		} ;
unsigned char m_bit1[8] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} ;
unsigned char m_byte[64] ;
unsigned char m_byte1[64] ;
unsigned char key_n[16][8] ;
unsigned char l_bit[17][8] ;
unsigned char r_bit[17][8] ;
unsigned char e_byte[64] ;
unsigned char e_byte1[64] ;
unsigned char r_byte[64] ;
unsigned char r_byte1[64] ;
int i = 0 , j = 0;

/*initialize*/
for(i=0;i<=15;i++)
	{
	for(j=0;j<=7;j++)
		{
		l_bit[i][j] = 0x0 ;
		r_bit[i][j] = 0x0 ;
		key_n[i][j] = 0x0 ;
		}
	}
for(i=0;i<=63;i++)
	{
	m_byte[i] = 0x0 ;
	m_byte1[i] = 0x0 ;
	r_byte[i] = 0x0 ;
	r_byte1[i] = 0x0 ;
	e_byte[i] = 0x0 ;
	e_byte1[i] = 0x0 ;
	}

keychange(k_bit,key_n) ;/*get the 48bit key x 16 (16rows x 6byte in key_n)*/
bit2byte(m_bit,m_byte) ;/*change to byte*/
for(i=0;i<=63;i++) m_byte1[i] = m_byte[ip[i] - 1] ;/*goto IP swap bit*/
byte2bit(m_byte1,m_bit1) ;/*re-change to bit*/
for(i=0;i<=3;i++) l_bit[0][i] = m_bit1[i] ;/*move left 32bit -> l0*/
for(i=4;i<=7;i++) r_bit[0][i - 4] = m_bit1[i] ;/*move right 32bit -> r0*/

for(i=1;i<=16;i++) /*16 layer*/
	{
	for(j=0;j<=3;j++) l_bit[i][j] = r_bit[i-1][j] ;/*L(n) = R(n-1)*/

	/*comput f(R(n-1),k)*/
	bit2byte(r_bit[i-1],r_byte) ;
	for(j=0;j<=47;j++) r_byte1[j] = r_byte[e[j] - 1] ;/*goto E swap bit*/
	byte2bit(r_byte1,r_bit[i-1]) ;/*now r_bit is 48bit*/

	/*xor 48bit key*/
	for(j=0;j<=5;j++) r_bit[i-1][j] = r_bit[i-1][j] ^ key_n[i-1][j] ;

	/*goto <S replace> and <P swap bit>*/
	s_replace(r_bit[i-1]) ;/*change 48bit r_bit[i-1]->32bit r_bit[i-1]*/
	for(j=0;j<=3;j++)/*get next r_bit*/
		{
		 r_bit[i][j] = l_bit[i-1][j] ^ r_bit[i-1][j] ;/*f(R(n-1),k)*/
		}
	}/*end of endes*/
for(i=0;i<=3;i++) e_bit[i] = r_bit[16][i] ;
for(i=4;i<=7;i++) e_bit[i] = l_bit[16][i - 4] ;
/*r_bit + l_bit -> e_bit(64bit)*/

bit2byte(e_bit,e_byte) ;/*change to byte for swap bit IP-1*/
for(i=0;i<=63;i++) e_byte1[i] = e_byte[ip_1[i] - 1] ;/*goto IP-1 swap bit*/
byte2bit(e_byte1,e_bit) ;/*got e_bit*/

}/*end of data encryption*/

/*--------------------------------------------------*/
/*                  data uncryption                 */
/*--------------------------------------------------*/
void DES_De(unsigned char m_bit[8] , unsigned char k_bit[8] , unsigned char e_bit[8])
/*NOTE: in fact , m_bit is encryption data , e_bit is uncryption*/
{
int ip[64] = {  58,50,42,34,26,18,10,2,
		60,52,44,36,28,20,12,4,
		62,54,46,38,30,22,14,6,
		64,56,48,40,32,24,16,8,
		57,49,41,33,25,17,9,1,
		59,51,43,35,27,19,11,3,
		61,53,45,37,29,21,13,5,
		63,55,47,39,31,23,15,7
		} ;
int ip_1[64] = {40,8,48,16,56,24,64,32,
		39,7,47,15,55,23,63,31,
		38,6,46,14,54,22,62,30,
		37,5,45,13,53,21,61,29,
		36,4,44,12,52,20,60,28,
		35,3,43,11,51,19,59,27,
		34,2,42,10,50,18,58,26,
		33,1,41,9,49,17,57,25
		} ;
int e[48] = {   32,1, 2, 3, 4, 5,
		4, 5, 6, 7, 8, 9,
		8, 9, 10,11,12,13,
		12,13,14,15,16,17,
		16,17,18,19,20,21,
		20,21,22,23,24,25,
		24,25,26,27,28,29,
		28,29,30,31,32,1
		} ;
unsigned char m_bit1[8] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} ;
unsigned char m_byte[64] ;
unsigned char m_byte1[64] ;
unsigned char key_n[16][8] ;
unsigned char l_bit[17][8] ;
unsigned char r_bit[17][8] ;
unsigned char e_byte[64] ;
unsigned char e_byte1[64] ;
unsigned char l_byte[64] ;
unsigned char l_byte1[64] ;
int i = 0 , j = 0 ;

/*initialize*/
for(i=0;i<=15;i++)
	{
	for(j=0;j<=7;j++)
		{
		l_bit[i][j] = 0x0 ;
		r_bit[i][j] = 0x0 ;
		key_n[i][j] = 0x0 ;
		}
	}
for(i=0;i<=63;i++)
	{
	m_byte[i] = 0x0 ;
	m_byte1[i] = 0x0 ;
	l_byte[i] = 0x0 ;
	l_byte1[i] = 0x0 ;
	e_byte[i] = 0x0 ;
	e_byte1[i] = 0x0 ;
	}

keychange(k_bit,key_n) ;/*get the 48bit key x 16 (16rows x 6byte in key_n)*/

bit2byte(m_bit,m_byte) ;/*change to byte*/
for(i=0;i<=63;i++) m_byte1[i] = m_byte[ip[i] - 1] ;/*goto IP swap bit*/
byte2bit(m_byte1,m_bit1) ;/*re-change to bit*/
for(i=0;i<=3;i++) r_bit[16][i] = m_bit1[i] ;/*move left 32bit -> r16*/
for(i=4;i<=7;i++) l_bit[16][i - 4] = m_bit1[i] ;/*move right 32bit -> l16*/

for(i=16;i>=1;i--) /*->(16) layer from 16 -> 1*/
	{
	for(j=0;j<=3;j++) r_bit[i-1][j] = l_bit[i][j] ;/*R(n-1) = L(n)*/

	/*comput f(L(n),k)*/
	bit2byte(l_bit[i],l_byte) ;
	for(j=0;j<=47;j++) l_byte1[j] = l_byte[e[j] - 1] ;/*goto E swap bit*/
	byte2bit(l_byte1,l_bit[i]) ;/*now r_bit is 48bit*/
	/*xor 48bit key*/
	for(j=0;j<=5;j++) l_bit[i][j] = l_bit[i][j] ^ key_n[i-1][j] ;
	/*goto <S replace> and <P swap bit>*/
	s_replace(l_bit[i]) ;/*change 48bit l_bit[i]->32bit l_bit[i]*/
	for(j=0;j<=3;j++)/*get PREV l_bit*/
		{
		 l_bit[i-1][j] = r_bit[i][j] ^ l_bit[i][j] ;/*f(L(n),k)*/
		}
	}/*end of undes*/
for(i=0;i<=3;i++) e_bit[i] = l_bit[0][i] ;
for(i=4;i<=7;i++) e_bit[i] = r_bit[0][i - 4] ;
/*r_bit + l_bit -> e_bit(64bit)*/

bit2byte(e_bit,e_byte) ;/*change to byte for swap bit IP-1*/
for(i=0;i<=63;i++) e_byte1[i] = e_byte[ip_1[i] - 1] ;/*goto IP-1 swap bit*/
byte2bit(e_byte1,e_bit) ;/*got e_bit*/
/*now ! in fact , the e_bit is the uncryption data.*/

}/*end of uncryption data*/


void  byte2bit(unsigned char tbyte[64] , unsigned char tbit[8])
/*change 64byte to 64bit(8byte)*/
{
    int i,j;
    unsigned char tMask;
	
    for(i=0;i<8;i++)
		tbit[i]=0x00;

    for(i = 0;i < 8;i++)  
    {
		tMask=0x80;
		for(j = 0;j < 8;j++)
		{
			if (tbyte[i*8+j]==0x01) tbit[i]=tbit[i] | tMask;
			tMask>>=1;
		}

	}


}/*end of byte2bit*/

void bit2byte(unsigned char tbit[8] , unsigned char tbyte[64])
/*change 64bit(8byte) to 64byte*/
{
    int i,j;
    unsigned char tMask;
    
    
	for(i=0 ; i<64 ; i++)
	{
		tbyte[i] = 0x00 ;
	}

	for(i=0 ; i<8 ; i++)
	{
		tMask=0x80;
		for(j=0;j<8;j++)
		{
			if (tbit[i] & tMask) tbyte[i*8+j]=0x01;
			tMask>>=1;
		}
	}

}/* end of bit2byte */ 

/*--------------------------------------------------*/
/*                  change the key                  */
/*--------------------------------------------------*/
void keychange(unsigned char oldkey[8] , unsigned char newkey[16][8])
{
 int i=0,j=0,k=0 ;
 int pc_1[56] ={57,49,41,33,25,17,9,
   		1,58,50,42,34,26,18,
		10,2,59,51,43,35,27,
		19,11,3,60,52,44,36,
		63,55,47,39,31,23,15,
		7,62,54,46,38,30,22,
		14,6,61,53,45,37,29,
		21,13,5,28,20,12,4} ;
int pc_2[48] = {14,17,11,24,1,5,
		3,28,15,6,21,10,
		23,19,12,4,26,8,
		16,7,27,20,13,2,
		41,52,31,37,47,55,
		30,40,51,45,33,48,
		44,49,39,56,34,53,
		46,42,50,36,29,32} ;
int ccmovebit[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1} ;
unsigned char oldkey_byte[64] ;
unsigned char oldkey_byte1[64] ;
unsigned char oldkey_byte2[64] ;
unsigned char oldkey_c[28] ;
unsigned char oldkey_d[28] ;
unsigned char cc_temp ;
unsigned char newkey_byte[16][64] ;

bit2byte(oldkey,oldkey_byte) ;/*change to byte*/
for(i=0;i<=55;i++) oldkey_byte1[i] = oldkey_byte[pc_1[i] - 1] ;/*goto PC-1*/
for(i=0;i<=27;i++) oldkey_c[i] = oldkey_byte1[i] ;/*move 28bit -> c0*/
for(i=28;i<=55;i++) oldkey_d[i -28] = oldkey_byte1[i] ;/*move other 28bit -> d0*/

/*cc_movebit : get c1-16,d1-16*/
for(i=0;i<=15;i++)
{
	for(j=1;j<=ccmovebit[i];j++)
	{
		cc_temp = oldkey_c[0] ; /*move out the first bit*/
		for(k=0;k<=26;k++)
		{
			oldkey_c[k] = oldkey_c[k+1] ;
		}
		oldkey_c[27] = cc_temp ; /*move the first bit to the last bit*/

		cc_temp = oldkey_d[0] ; /*move out the first bit*/
                for(k=0;k<=26;k++)
		{
			oldkey_d[k] = oldkey_d[k+1] ;
		}
		oldkey_d[27] = cc_temp ; /*move the first bit to the last bit*/
	}  /*cc_movebit*/
	/*goto pc-2 change bit*/
	for(k=0;k<=27;k++) oldkey_byte2[k] = oldkey_c[k] ;
	for(k=28;k<=55;k++) oldkey_byte2[k] = oldkey_d[k-28] ;
	/*add c(i)+d(i) -> for pc-2 change 56bit to 48bit k(i)*/
	for(k=0;k<=47;k++) newkey_byte[i][k] = oldkey_byte2[pc_2[k] - 1] ;

	/*ready to next k(i)*/
}/*end of one of change the 48bit key*/
/*byte to bit for 48bit newkey*/
for(i=0;i<=15;i++) byte2bit(newkey_byte[i],newkey[i]) ;
}/*end of keychange*/


void s_replace(unsigned char s_bit[8])
{
int p[32] = {   16,7,20,21,
		29,12,28,17,
		1,15,23,26,
		5,18,31,10,
		2,8,24,14,
		32,27,3,9,
		19,13,30,6,
		22,11,4,25
		} ;
unsigned char s[8][4][16] = 
				
	{           14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
				0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
				4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
				15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13,
				
				15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
				3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
				0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
				13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9,
				
				10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
				13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
				13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
				1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12,
				
				7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
				13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
				10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
				3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14,
				
				2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
				14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
				4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
				11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3,
				
				12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
				10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
				9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
				4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13,
				
				4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
				13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
				1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
				6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12,
				
				13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
				1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
				7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
				2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
	};

int i=0 , j=0 ;
unsigned char s_byte[64] ;
unsigned char s_byte1[64] ;
unsigned char s_bit_temp[8] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} ;
unsigned char row = 0 , col = 0 ;
unsigned char s_out_bit[8] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} ;
unsigned char s_out_bit1[8] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} ;

/*initialize*/
for(i=0;i<=63;i++) s_byte[i] = 0x0 ;
for(i=0;i<=63;i++) s_byte1[i] = 0x0 ;

/*change 48bit(8bit x 6byte) to 48bit(6bit x 8byte)*/
bit2byte(s_bit,s_byte) ;
for(i=0;i<=7;i++)
	{
        for(j=0;j<=63;j++) s_byte1[j] = 0x0 ;/*clear temp*/

	s_byte1[6] = s_byte[i*6] ;  /*get bit 0 in 6 bit*/
	s_byte1[7] = s_byte[i*6+5] ;/*get bit 5 in 6 bit*/
	byte2bit(s_byte1,s_bit_temp) ;/* 0000 00?? */
	row = s_bit_temp[0] ;/*get row[i]*/

	for(j=0;j<=63;j++) s_byte1[j] = 0x0 ;/*clear temp*/

	s_byte1[4] = s_byte[i*6+1] ;/*0000 ????*/
	s_byte1[5] = s_byte[i*6+2] ;
	s_byte1[6] = s_byte[i*6+3] ;
	s_byte1[7] = s_byte[i*6+4] ;
	byte2bit(s_byte1,s_bit_temp) ;
	col = s_bit_temp[0] ;/*get column in S table*/

	/*get number from S table with row and col*/
	s_out_bit[i] = s[i][row][col] ;


	} /*s_out_bit[0-7]:0000???? 0000???? 0000???? 0000????...0000???? */

	/*change 64bit to 32bit : clean 0000(high 4bit)*/
	s_out_bit1[0] = (s_out_bit[0]<<4) + s_out_bit[1] ;
	s_out_bit1[1] = (s_out_bit[2]<<4) + s_out_bit[3] ;
	s_out_bit1[2] = (s_out_bit[4]<<4) + s_out_bit[5] ;
	s_out_bit1[3] = (s_out_bit[6]<<4) + s_out_bit[7] ;
	/*now s_out_bit1[0-7] = ???????? ???????? ???????? ???????? 0000..*/
        for(i=0;i<=63;i++) s_byte[i] = 0x0 ;
	for(i=0;i<=63;i++) s_byte1[i] = 0x0 ;
	bit2byte(s_out_bit1 , s_byte) ;/*change byte for P swap bit*/
	for(i=0;i<=31;i++) s_byte1[i] = s_byte[p[i] - 1] ;/*goto P swap bit*/
	for(i=0;i<=7;i++) s_bit[i] = 0x0 ;
	byte2bit(s_byte1 , s_bit) ;
	/*now ! we got 32bit f(R,K)*/
}

/*end of S_replace*/

