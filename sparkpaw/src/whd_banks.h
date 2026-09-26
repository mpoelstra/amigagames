#ifndef SPARKPAW_WHD_BANKS_H
#define SPARKPAW_WHD_BANKS_H
#include <exec/types.h>
#include <dos/dos.h>
/* One shared backend, including calls from the namespaced Drowned module.
   Bank changes are explicit and only legal at released, black transitions. */
BOOL whdBanksBoot(void);
BOOL whdBanksSelect(UWORD section);
BOOL whdBanksFinishIntro(void);
void whdBanksClose(void);
BPTR whdBankOpen(const char *name,LONG mode);
LONG whdBankRead(BPTR file,void *target,LONG size);
LONG whdBankSeek(BPTR file,LONG offset,LONG mode);
LONG whdBankClose(BPTR file);
#endif
