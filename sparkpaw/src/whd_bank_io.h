/* Include after DOS prototypes; no fallback disk access for missing assets. */
#ifdef SPARKPAW_WHD_BANKS
#include "whd_banks.h"
#undef Open
#undef Read
#undef Seek
#undef Close
#define Open whdBankOpen
#define Read whdBankRead
#define Seek whdBankSeek
#define Close whdBankClose
#endif
