/* Frozen pre-block renderer: host parity oracle, never linked on Amiga. */
static void mixRenderReference(Mixer *m,int8_t *out,unsigned count)
{
    unsigned i,j;int value;
    if(m->voice[0].remaining&&m->voice[1].remaining) m->overlaps++;
    for(i=0;i<count;i++) {
        value=0;
        for(j=0;j<2;j++) if(m->voice[j].remaining) {
            value+=*m->voice[j].data++;
            if(!--m->voice[j].remaining) {m->voice[j].priority=0;m->completed[j]++;}
        }
        out[i]=(int8_t)value; /* inputs scaled to [-64,63]: sum cannot clip */
    }
    m->rendered+=count;
}
