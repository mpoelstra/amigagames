#include "audio_mix.h"
#include <string.h>
int mixRequest(Mixer *m,const Effect *fx,unsigned id)
{
    Voice *v;unsigned cooldownId;
    if(id>=FX_COUNT) return 0;
    cooldownId=id==15?6:id; /* health shares the production collect cooldown */
    v=&m->voice[id==0?0:1];
    if(m->cooldown[cooldownId]||(v->remaining&&v->priority>fx[id].priority)) {
        return 0;
    }
    v->data=fx[id].data;v->remaining=fx[id].length;
    v->priority=fx[id].priority;m->cooldown[cooldownId]=fx[id].cooldown;
    return 1;
}
void mixField(Mixer *m)
{
    unsigned i;for(i=0;i<FX_COUNT;i++) if(m->cooldown[i]) m->cooldown[i]--;
}
/* Divide at voice tails: at most two active spans and one silence span.
   The IRQ owns voice state while rendering; requests publish with AUD3 masked. */
void mixRender(Mixer *m,int8_t *out,unsigned count)
{
    Voice *a=&m->voice[0],*b=&m->voice[1];
    unsigned left=count,n;
    while(left) {
        n=left;
        if(a->remaining&&b->remaining) {
            if(a->remaining<n)n=(unsigned)a->remaining;
            if(b->remaining<n)n=(unsigned)b->remaining;
            {
                const int8_t *pa=a->data,*pb=b->data;
                int8_t *dest=out;unsigned span=n;
                do {*dest++=(int8_t)(*pa+++*pb++);}while(--span);
            }
        } else if(a->remaining||b->remaining) {
            Voice *v=a->remaining?a:b;
            if(v->remaining<n)n=(unsigned)v->remaining;
            memcpy(out,v->data,n);
        } else {
            memset(out,0,n);
        }
        if(a->remaining) {
            a->data+=n;a->remaining-=n;
            if(!a->remaining){a->priority=0;}
        }
        if(b->remaining) {
            b->data+=n;b->remaining-=n;
            if(!b->remaining){b->priority=0;}
        }
        out+=n;left-=n;
    }
}
