#ifndef AUDIO_PROOF_MIX_H
#define AUDIO_PROOF_MIX_H
#include <stdint.h>
#define MIX_BYTES 112 /* 10.17 ms at Paula period 322 */
#define FX_COUNT 16 /* 15 source files plus the louder health-pickup variant */
typedef struct { const int8_t *data; uint32_t length; uint8_t priority,cooldown; } Effect;
typedef struct { const int8_t *data; uint32_t remaining; uint8_t priority; } Voice;
typedef struct {
    Voice voice[2]; uint8_t cooldown[FX_COUNT];
    uint32_t requests[FX_COUNT],starts[FX_COUNT],rejected[FX_COUNT];
    uint32_t rendered,completed[2],overlaps;
} Mixer;
int mixRequest(Mixer *m,const Effect *fx,unsigned id);
unsigned mixSchedule(Mixer *m,const Effect *fx,uint32_t field);
void mixField(Mixer *m);
void mixRender(Mixer *m,int8_t *output,unsigned count);
#endif
