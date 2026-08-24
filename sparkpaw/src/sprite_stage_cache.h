#ifndef SPARKPAW_SPRITE_STAGE_CACHE_H
#define SPARKPAW_SPRITE_STAGE_CACHE_H

struct SpriteStageCacheState {
    unsigned char valid;
    unsigned char facing;
    unsigned char frame;
};

#define SPRITE_STAGE_CACHE_NEEDS_COPY(state,facingValue,frameValue) \
    (!(state)->valid||(state)->facing!=(facingValue)|| \
     (state)->frame!=(frameValue))

#define SPRITE_STAGE_CACHE_COMMIT(state,facingValue,frameValue) do { \
    (state)->valid=1; \
    (state)->facing=(facingValue); \
    (state)->frame=(frameValue); \
} while(0)

#endif
