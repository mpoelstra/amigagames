#ifndef DROWNED_COLLECTIBLE_RESTORE_H
#define DROWNED_COLLECTIBLE_RESTORE_H
/* Masked Bob starts at any pixel; restore starts on a 16px word boundary. */
static unsigned short drownedCollectibleRestoreWidth(short x)
{
    return (unsigned short)(((x&15)+16+15)&~15);
}
#endif
