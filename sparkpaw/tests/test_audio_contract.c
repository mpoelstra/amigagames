#include <assert.h>
#include <stdio.h>

#include "../src/audio_contract.h"

static void checkOneShot(unsigned long bytes,unsigned short expectedFields)
{
    unsigned long audibleNumerator=bytes*AUDIO_EFFECT_PERIOD*
                                   AUDIO_PAL_FIELDS_PER_SECOND;
    unsigned short fields=audioSampleFields(bytes);
    assert(fields==expectedFields);
    assert((unsigned long)(fields-AUDIO_UPDATE_GUARD_FIELDS)*
           AUDIO_PAL_CLOCK_HZ>=audibleNumerator);
    assert((unsigned long)(fields-AUDIO_UPDATE_GUARD_FIELDS-1UL)*
           AUDIO_PAL_CLOCK_HZ<audibleNumerator);
}

int main(void)
{
    assert(audioSampleFields(0)==0);
    checkOneShot(496,4);    /* tally tick: 45 ms audible, then silence */
    checkOneShot(1874,10);  /* player plasma */
    checkOneShot(1544,9);   /* player hurt */
    checkOneShot(12678,59); /* Storm Triumph: no one-second truncation */
    puts("audio one-shot duration contract passed");
    return 0;
}
