
typedef unsigned short    uint16;
typedef unsigned long    uint32;
typedef unsigned char    uint8;

#define RKNAND_DIASBLE_SECURE_BOOT _IOW('d', 127, unsigned int)
#define RKNAND_ENASBLE_SECURE_BOOT _IOW('d', 126, unsigned int)
#define RKNAND_GET_SN_SECTOR       _IOW('d', 3, unsigned int)

#define RKNAND_GET_VENDOR_SECTOR0       _IOW('v', 16, unsigned int)
#define RKNAND_STORE_VENDOR_SECTOR0     _IOW('v', 17, unsigned int)

#define RKNAND_GET_VENDOR_SECTOR1       _IOW('v', 18, unsigned int)
#define RKNAND_STORE_VENDOR_SECTOR1     _IOW('v', 19, unsigned int)

#define DRM_KEY_OP_TAG              0x4B4D5244 // "DRMK"
#define SN_SECTOR_OP_TAG            0x41444E53 // "SNDA"
#define DIASBLE_SECURE_BOOT_OP_TAG  0x42534444 // "DDSB"
#define ENASBLE_SECURE_BOOT_OP_TAG  0x42534E45 // "ENSB"
#define VENDOR_SECTOR_OP_TAG        0x444E4556 // "VEND"

#define RKNAND_SYS_STORGAE_DATA_LEN 512

typedef struct tagRKNAND_SYS_STORGAE
{
  uint32  tag;
  uint32  len;
  uint8   data[RKNAND_SYS_STORGAE_DATA_LEN];
}RKNAND_SYS_STORGAE;


typedef struct tagSN_SECTOR_INFO
{
  uint32 snSectag;           // "SNDA" 0x41444E53
  uint32 snSecLen;           // 512
  uint16 snLen;              // 0:no sn , 0~30,sn len
  uint8 snData[30];          // sn data
  uint32 reserved2[(0x200-0x20)/4];
}SN_SECTOR_INFO,*pSN_SECTOR_INFO;

