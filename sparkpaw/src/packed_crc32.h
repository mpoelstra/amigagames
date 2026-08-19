#ifndef SPARKPAW_PACKED_CRC32_H
#define SPARKPAW_PACKED_CRC32_H

static unsigned int packedCRC32Byte(unsigned int crc,unsigned char value)
{
    static const unsigned int nibble[16]={
        0x00000000U,0x1db71064U,0x3b6e20c8U,0x26d930acU,
        0x76dc4190U,0x6b6b51f4U,0x4db26158U,0x5005713cU,
        0xedb88320U,0xf00f9344U,0xd6d6a3e8U,0xcb61b38cU,
        0x9b64c2b0U,0x86d3d2d4U,0xa00ae278U,0xbdbdf21cU
    };
    crc^=value;
    crc=(crc>>4)^nibble[crc&15];
    return (crc>>4)^nibble[crc&15];
}

#endif
