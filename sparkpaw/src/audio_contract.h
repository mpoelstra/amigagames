#ifndef SPARKPAW_AUDIO_CONTRACT_H
#define SPARKPAW_AUDIO_CONTRACT_H

/* PAL A1200 colour clock and the period used by every current Sparkpaw
   effect. One byte is one signed 8-bit Paula sample. */
#define AUDIO_PAL_CLOCK_HZ 3546895UL
#define AUDIO_EFFECT_PERIOD 322UL
#define AUDIO_PAL_FIELDS_PER_SECOND 50UL

/* Some call paths update the audio counters again in the field that starts an
   effect. Keep one guard field so the final sample cannot be cut short. The
   Paula reload target is silence, so this guard can never repeat the sample. */
#define AUDIO_UPDATE_GUARD_FIELDS 1UL

static unsigned short audioSampleFields(unsigned long sampleBytes)
{
    unsigned long numerator;
    unsigned long fields;
    if(!sampleBytes) return 0;
    numerator=sampleBytes*AUDIO_EFFECT_PERIOD*AUDIO_PAL_FIELDS_PER_SECOND;
    fields=(numerator+AUDIO_PAL_CLOCK_HZ-1UL)/AUDIO_PAL_CLOCK_HZ;
    fields+=AUDIO_UPDATE_GUARD_FIELDS;
    return (unsigned short)fields;
}

#endif
