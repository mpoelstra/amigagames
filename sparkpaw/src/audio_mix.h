#ifndef SPARKPAW_AUDIO_MIX_H
#define SPARKPAW_AUDIO_MIX_H
#include <stdint.h>
#define MIX_BYTES 112 /* 10.17 ms at Paula period 322 */
#ifdef SPARKPAW_DROWNED_JOINED
#define FX_COUNT 18 /* Append Harrier defeat after the Drowned checkpoint. */
#else
#define FX_COUNT 17 /* Append Harrier defeat after existing effects. */
#endif
typedef struct { const int8_t *data; uint32_t length; uint8_t priority,cooldown; } Effect;
typedef struct { const int8_t *data; uint32_t remaining; uint8_t priority; } Voice;
typedef struct {
    Voice voice[2]; uint8_t cooldown[FX_COUNT];
} Mixer;
int mixRequest(Mixer *m,const Effect *fx,unsigned id);
void mixField(Mixer *m);
void mixRender(Mixer *m,int8_t *output,unsigned count);
#endif
