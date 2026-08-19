#include <assert.h>

#include "../src/packed_crc32.h"

int main(void)
{
    static const unsigned char text[]="123456789";
    unsigned int crc=0xffffffffU;
    unsigned int index;
    for(index=0;index<sizeof(text)-1;index++)
        crc=packedCRC32Byte(crc,text[index]);
    assert((crc^0xffffffffU)==0xcbf43926U);
    return 0;
}
