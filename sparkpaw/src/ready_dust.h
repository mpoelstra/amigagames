#ifndef READY_DUST_H
#define READY_DUST_H
#define READY_DUST_DIRTY 48
struct ReadyDustHistory {
    unsigned char count;
    unsigned short offset[READY_DUST_DIRTY];
    unsigned char original[READY_DUST_DIRTY][6];
};
void readyDustRestore(unsigned char **planes,struct ReadyDustHistory *history);
void readyDustDraw(unsigned char **planes,struct ReadyDustHistory *history,
                   unsigned long frame,const unsigned char *pens,unsigned char state);
#endif
