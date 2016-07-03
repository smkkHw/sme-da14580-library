#ifndef _SME_DA14580_MODEL_H
#define _SME_DA14580_MODEL_H

// UART download code
#define max_code_length          0xA800  // MAX 42K bytes
#define SYSRAM_COPY_BASE_ADDRESS 2000
#define SYSRAM_BASE_ADDRESS      0
#define SOH   0x01
#define STX   0x02
#define ACK   0x06
#define NAK   0x15



#define ACK_TIMEOUT_20SEC 20000
#define DWNLD_TIMEOUT_5MIN 300000
#define DATA_TIMEOUT_2SEC 2000

#endif //_SME_DA14580_MODEL_H
