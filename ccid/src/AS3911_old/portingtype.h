

typedef struct EmvPicc_s
{
    unsigned char uid[10]; /*!< UID or PUPI of the PICC. */
    unsigned char  uidLength; /*!< Length of the UID/PUPI in bytes. */
    unsigned char  fwi; /*!< Frame wait integer of the PICC. */
    unsigned char  fsci; /*!< Frame size integer of the PICC. */
    unsigned char  sfgi; /*!< Special frame guard time of the PICC. */
    unsigned char  dPiccPcd; /*!< Datarate bits PICC->PCD. */
    unsigned char  dPcdPicc; /*!< Datarate bits PCD->PICC. */

    signed  short int  (*activate)(struct EmvPicc_s *picc); /*!< Activation function callback. */
    signed  short int (*remove)(struct EmvPicc_s *picc); /*!< Card removal function callback. */
} EmvPicc_t;


#define IPT_LEN_KPINFO 3 //密钥对信息长度
#define IPT_POS_KEYPAIR_EXIST 0x00 //密钥对信息-存在
#define IPT_POS_KEYPAIR_ALG 0x01 //密钥对信息-算法
#define IPT_POS_KEYPAIR_LEN 0x02 //密钥对信息-长度
#define IPT_POS_RSAN 0x03 //RSA-N
#define IPT_POS_RSAP 0x0103 //RSA-p
#define IPT_POS_RSAQ 0x0183 //RSA-q
#define IPT_POS_RSADP 0x0203 //RSA-dp
#define IPT_POS_RSADQ 0x0283 //RSA-dq
#define IPT_POS_RSAQINV 0x0303 //RSA-qinv
#define IPT_POS_KEYPAIRHASH 0x0383 //密钥对HASH值


extern unsigned char  RSAECCBUF1[98];




