#ifndef READY_PATCH_H
#define READY_PATCH_H
void readyPatchPrepare(unsigned char **source);
void readyPatchApply(unsigned char **target,unsigned char **source,
                     unsigned char oldState,unsigned char newState);
#endif
