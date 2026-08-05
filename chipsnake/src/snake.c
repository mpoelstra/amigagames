/*
 * Amiga Snake
 *
 * A small, system-friendly AmigaOS game using intuition.library and
 * graphics.library.  It intentionally uses only classic Amiga APIs and
 * C89-era language features.
 */

#include <exec/types.h>
#include <exec/memory.h>
#include <exec/execbase.h>
#include <devices/inputevent.h>
#include <dos/dos.h>
#include <intuition/intuition.h>
#include <intuition/screens.h>
#include <graphics/gfxbase.h>
#include <graphics/displayinfo.h>
#include <graphics/rastport.h>
#include <utility/tagitem.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>

#include <stdio.h>
#include <string.h>

#include "modplayer.h"

#define SCREEN_W       320
#define SCREEN_H       256
#define CELL_SIZE        8
#define BOARD_W         40
#define BOARD_H         27
#define BOARD_Y         40
#define BACKGROUND_H   (BOARD_H * CELL_SIZE)
#define MAX_SNAKE      (BOARD_W * BOARD_H)
#define TICKS_PER_MOVE   5
#define ARENA_COUNT       3
#define GAME_MUSIC_COUNT  3
#define FOOD_TIMEOUT_SECONDS 7
#define STAR_COUNT        48
#define HIGH_SCORE_COUNT  10
#define HIGH_NAME_LENGTH  15
#define HIGH_SCORE_FILE   "PROGDIR:chipsnake.highscores"
#define HIGH_SCORE_TEMP   "PROGDIR:chipsnake.highscores.tmp"
#define HIGH_SCORE_BACKUP "PROGDIR:chipsnake.highscores.bak"

#define MODE_TITLE         0
#define MODE_GAME          1
#define MODE_NAME_ENTRY    2
#define MODE_HIGH_SCORES   3

#define DIR_UP           0
#define DIR_RIGHT        1
#define DIR_DOWN         2
#define DIR_LEFT         3

#define PEN_SNAKE       12
#define PEN_FOOD        13
#define PEN_HEAD        14
#define PEN_TEXT        15
#define LAST_SAFE_PEN    4

#define RAWKEY_SPACE  0x40
#define RAWKEY_ESC    0x45
#define RAWKEY_W      0x11
#define RAWKEY_A      0x20
#define RAWKEY_S      0x21
#define RAWKEY_D      0x22
#define RAWKEY_H      0x25
#define RAWKEY_N      0x36
#define RAWKEY_BACKSPACE 0x41
#define RAWKEY_RETURN 0x44
#define RAWKEY_UP     0x4c
#define RAWKEY_DOWN   0x4d
#define RAWKEY_RIGHT  0x4e
#define RAWKEY_LEFT   0x4f

struct Point {
    WORD x;
    WORD y;
};

struct Star {
    WORD x;
    WORD y;
    WORD speed;
};

struct HighScore {
    char name[HIGH_NAME_LENGTH + 1];
    ULONG score;
};

struct IntuitionBase *IntuitionBase = NULL;
struct GfxBase *GfxBase = NULL;

static struct Screen *gameScreen = NULL;
static struct Window *gameWindow = NULL;
static struct RastPort *rp = NULL;
static UWORD *hiddenPointer = NULL;
static struct RastPort introStripRp;
static struct BitMap backgroundBitmap;
static struct BitMap introBitmap;
static BOOL backgroundBitmapReady;
static BOOL introBitmapReady;
static BOOL introImageLoaded;
static BOOL backgroundLoaded;

static struct Point snake[MAX_SNAKE];
static WORD snakeLength;
static WORD direction;
static WORD wantedDirection;
static struct Point food;
static struct Star stars[STAR_COUNT];
static UBYTE obstacles[BOARD_H][BOARD_W];
static WORD arenaNumber;
static WORD lastMusicNumber = -1;
static WORD screenMode;
static BOOL gameOver;
static BOOL paused;
static BOOL quitGame;
static ULONG score;
static ULONG foodAgeTicks;
static ULONG titleFrame;
static ULONG randomState = 0x13579bdfUL;
static struct HighScore highScores[HIGH_SCORE_COUNT];
static char lastPlayerName[HIGH_NAME_LENGTH + 1];
static char enteredName[HIGH_NAME_LENGTH + 1];
static WORD enteredNameLength;
static WORD pendingHighScoreRank;
static WORD newestHighScoreRank = -1;
static ULONG highScoreFrame;
static BOOL highScoreImageLoaded;
static BOOL highScoreFromGame;
static BOOL joystickFire;

static void setTitlePalette(void);

static const char *arenaNames[] = {
    "CIRCUIT",
    "BOING LAB",
    "WORKBENCH"
};

static const char *arenaFiles[] = {
    "backgrounds/circuit.iff",
    "backgrounds/boing.iff",
    "backgrounds/workbench.iff"
};

static const char *gameMusicFiles[] = {
    "music/chipsnake-circuit.mod",
    "music/chipsnake-turbo.mod",
    "music/chipsnake-neon.mod"
};

static const char marqueeText[] =
    "*** SPACE TO START *** H FOR HIGH SCORES *** ESC TO QUIT *** "
    "MRDIG PRODUCTIONS *** "
    "COPYRIGHT 2026 *** THIS GAME IS MADE BY 100% AI ***";

static BOOL samePoint(struct Point a, struct Point b)
{
    return a.x == b.x && a.y == b.y;
}

static ULONG nextRandom(void)
{
    randomState = randomState * 1103515245UL + 12345UL;
    return randomState;
}

static void putUlong(UBYTE *destination, ULONG value)
{
    destination[0] = (UBYTE)(value >> 24);
    destination[1] = (UBYTE)(value >> 16);
    destination[2] = (UBYTE)(value >> 8);
    destination[3] = (UBYTE)value;
}

static ULONG getUlong(const UBYTE *source)
{
    return ((ULONG)source[0] << 24) |
        ((ULONG)source[1] << 16) |
        ((ULONG)source[2] << 8) |
        (ULONG)source[3];
}

static ULONG highScoreChecksum(const UBYTE *data, ULONG length)
{
    ULONG checksum;
    ULONG i;

    checksum = 0x43485348UL;
    for (i = 0; i < length; ++i) {
        checksum = (checksum << 5) | (checksum >> 27);
        checksum ^= data[i];
    }
    return checksum;
}

static void initializeDefaultHighScores(void)
{
    static const char *names[HIGH_SCORE_COUNT] = {
        "MrDig", "Paula", "Copper", "Blitter", "Denise",
        "Agnus", "BoingBall", "Guru", "Workbench", "ChipSnake"
    };
    static const ULONG scores[HIGH_SCORE_COUNT] = {
        5000, 4000, 3000, 2500, 2000,
        1500, 1200, 1000, 750, 500
    };
    WORD i;

    for (i = 0; i < HIGH_SCORE_COUNT; ++i) {
        strcpy(highScores[i].name, names[i]);
        highScores[i].score = scores[i];
    }
    strcpy(lastPlayerName, "PLAYER");
}

static void loadHighScores(void)
{
    UBYTE data[226];
    BPTR file;
    LONG length;
    ULONG storedChecksum;
    WORD i;
    ULONG offset;

    initializeDefaultHighScores();
    file = Open(HIGH_SCORE_FILE, MODE_OLDFILE);
    if (file == 0) {
        return;
    }
    length = Read(file, data, sizeof(data));
    Close(file);
    if (length != sizeof(data) || memcmp(data, "CSHS", 4) != 0 ||
        data[4] != 2 || data[5] != HIGH_SCORE_COUNT) {
        return;
    }
    storedChecksum = getUlong(data + sizeof(data) - 4);
    if (storedChecksum != highScoreChecksum(data, sizeof(data) - 4)) {
        return;
    }
    memcpy(lastPlayerName, data + 6, HIGH_NAME_LENGTH + 1);
    lastPlayerName[HIGH_NAME_LENGTH] = '\0';
    offset = 22;
    for (i = 0; i < HIGH_SCORE_COUNT; ++i) {
        memcpy(highScores[i].name, data + offset,
            HIGH_NAME_LENGTH + 1);
        highScores[i].name[HIGH_NAME_LENGTH] = '\0';
        highScores[i].score =
            getUlong(data + offset + HIGH_NAME_LENGTH + 1);
        offset += HIGH_NAME_LENGTH + 5;
    }
}

static BOOL saveHighScores(void)
{
    UBYTE data[226];
    BPTR file;
    ULONG offset;
    WORD i;
    BOOL saved;

    memset(data, 0, sizeof(data));
    memcpy(data, "CSHS", 4);
    data[4] = 2;
    data[5] = HIGH_SCORE_COUNT;
    strncpy((char *)data + 6, lastPlayerName, HIGH_NAME_LENGTH);
    offset = 22;
    for (i = 0; i < HIGH_SCORE_COUNT; ++i) {
        strncpy((char *)data + offset, highScores[i].name,
            HIGH_NAME_LENGTH);
        putUlong(data + offset + HIGH_NAME_LENGTH + 1,
            highScores[i].score);
        offset += HIGH_NAME_LENGTH + 5;
    }
    putUlong(data + sizeof(data) - 4,
        highScoreChecksum(data, sizeof(data) - 4));

    DeleteFile(HIGH_SCORE_TEMP);
    file = Open(HIGH_SCORE_TEMP, MODE_NEWFILE);
    if (file == 0) {
        return FALSE;
    }
    saved = Write(file, data, sizeof(data)) == sizeof(data);
    Close(file);
    if (!saved) {
        DeleteFile(HIGH_SCORE_TEMP);
        return FALSE;
    }

    DeleteFile(HIGH_SCORE_BACKUP);
    Rename(HIGH_SCORE_FILE, HIGH_SCORE_BACKUP);
    if (!Rename(HIGH_SCORE_TEMP, HIGH_SCORE_FILE)) {
        Rename(HIGH_SCORE_BACKUP, HIGH_SCORE_FILE);
        DeleteFile(HIGH_SCORE_TEMP);
        return FALSE;
    }
    DeleteFile(HIGH_SCORE_BACKUP);
    return TRUE;
}

static WORD findHighScoreRank(ULONG value)
{
    WORD i;

    for (i = 0; i < HIGH_SCORE_COUNT; ++i) {
        if (value > highScores[i].score) {
            return i;
        }
    }
    return -1;
}

static void insertHighScore(WORD rank, const char *name, ULONG value)
{
    WORD i;

    for (i = HIGH_SCORE_COUNT - 1; i > rank; --i) {
        highScores[i] = highScores[i - 1];
    }
    strncpy(highScores[rank].name, name, HIGH_NAME_LENGTH);
    highScores[rank].name[HIGH_NAME_LENGTH] = '\0';
    highScores[rank].score = value;
}

static BOOL allocateBackgroundBitmap(void)
{
    WORD plane;

    InitBitMap(&backgroundBitmap, 4, SCREEN_W, BACKGROUND_H);
    for (plane = 0; plane < 4; ++plane) {
        backgroundBitmap.Planes[plane] =
            AllocRaster(SCREEN_W, BACKGROUND_H);
        if (backgroundBitmap.Planes[plane] == NULL) {
            while (--plane >= 0) {
                FreeRaster(backgroundBitmap.Planes[plane],
                    SCREEN_W, BACKGROUND_H);
                backgroundBitmap.Planes[plane] = NULL;
            }
            return FALSE;
        }
    }
    backgroundBitmapReady = TRUE;
    return TRUE;
}

static void freeBackgroundBitmap(void)
{
    WORD plane;

    if (!backgroundBitmapReady) {
        return;
    }
    for (plane = 0; plane < 4; ++plane) {
        if (backgroundBitmap.Planes[plane] != NULL) {
            FreeRaster(backgroundBitmap.Planes[plane],
                SCREEN_W, BACKGROUND_H);
            backgroundBitmap.Planes[plane] = NULL;
        }
    }
    backgroundBitmapReady = FALSE;
}

static BOOL allocateIntroBitmap(void)
{
    WORD plane;

    InitBitMap(&introBitmap, 4, SCREEN_W, SCREEN_H);
    for (plane = 0; plane < 4; ++plane) {
        introBitmap.Planes[plane] = AllocRaster(SCREEN_W, SCREEN_H);
        if (introBitmap.Planes[plane] == NULL) {
            while (--plane >= 0) {
                FreeRaster(introBitmap.Planes[plane], SCREEN_W, SCREEN_H);
                introBitmap.Planes[plane] = NULL;
            }
            return FALSE;
        }
    }
    introBitmapReady = TRUE;
    return TRUE;
}

static void freeIntroBitmap(void)
{
    WORD plane;

    if (!introBitmapReady) {
        return;
    }
    for (plane = 0; plane < 4; ++plane) {
        if (introBitmap.Planes[plane] != NULL) {
            FreeRaster(introBitmap.Planes[plane], SCREEN_W, SCREEN_H);
            introBitmap.Planes[plane] = NULL;
        }
    }
    introBitmapReady = FALSE;
}

static BOOL readExact(BPTR file, APTR data, LONG length)
{
    return Read(file, data, length) == length;
}

static BOOL loadIntroImage(const char *filename)
{
    BPTR file;
    UBYTE id[4];
    UBYTE header[20];
    UBYTE colors[48];
    ULONG formSize;
    ULONG chunkSize;
    ULONG bytesRead;
    LONG skip;
    WORD row;
    WORD plane;
    WORD pen;
    BOOL haveHeader;
    BOOL haveBody;

    if (!introBitmapReady) {
        return FALSE;
    }
    file = Open((STRPTR)filename, MODE_OLDFILE);
    if (file == 0) {
        return FALSE;
    }
    haveHeader = FALSE;
    haveBody = FALSE;
    if (!readExact(file, id, 4) || memcmp(id, "FORM", 4) != 0 ||
        !readExact(file, &formSize, 4) ||
        !readExact(file, id, 4) || memcmp(id, "ILBM", 4) != 0) {
        Close(file);
        return FALSE;
    }

    while (readExact(file, id, 4) && readExact(file, &chunkSize, 4)) {
        bytesRead = 0;
        if (memcmp(id, "BMHD", 4) == 0 && chunkSize >= 20) {
            if (!readExact(file, header, 20)) {
                break;
            }
            bytesRead = 20;
            if ((((UWORD)header[0] << 8) | header[1]) == SCREEN_W &&
                (((UWORD)header[2] << 8) | header[3]) == SCREEN_H &&
                header[8] == 4 && header[10] == 0) {
                haveHeader = TRUE;
            }
        } else if (memcmp(id, "CMAP", 4) == 0 && chunkSize >= 48) {
            if (!readExact(file, colors, 48)) {
                break;
            }
            bytesRead = 48;
            for (pen = 0; pen < 16; ++pen) {
                SetRGB4(&gameScreen->ViewPort, pen,
                    colors[pen * 3] >> 4,
                    colors[pen * 3 + 1] >> 4,
                    colors[pen * 3 + 2] >> 4);
            }
        } else if (memcmp(id, "BODY", 4) == 0 && haveHeader &&
                   chunkSize >= (ULONG)(SCREEN_W / 8 * SCREEN_H * 4)) {
            for (row = 0; row < SCREEN_H; ++row) {
                for (plane = 0; plane < 4; ++plane) {
                    UBYTE *destination;

                    destination = (UBYTE *)introBitmap.Planes[plane] +
                        row * introBitmap.BytesPerRow;
                    if (!readExact(file, destination, SCREEN_W / 8)) {
                        Close(file);
                        return FALSE;
                    }
                    bytesRead += SCREEN_W / 8;
                }
            }
            haveBody = TRUE;
        }
        skip = (LONG)(chunkSize - bytesRead);
        if (chunkSize & 1) {
            ++skip;
        }
        if (skip > 0 && Seek(file, skip, OFFSET_CURRENT) == -1) {
            break;
        }
    }
    Close(file);
    return haveHeader && haveBody;
}

static UBYTE getBackgroundPen(WORD x, WORD y)
{
    UBYTE pen;
    UBYTE bit;
    UBYTE *source;
    WORD plane;

    pen = 0;
    bit = (UBYTE)(1 << (7 - (x & 7)));
    for (plane = 0; plane < 4; ++plane) {
        source = (UBYTE *)backgroundBitmap.Planes[plane] +
            y * backgroundBitmap.BytesPerRow + (x >> 3);
        if ((*source & bit) != 0) {
            pen |= (UBYTE)(1 << plane);
        }
    }
    return pen;
}

static void deriveCollisionFromBackground(void)
{
    WORD gridX;
    WORD gridY;
    WORD x;
    WORD y;
    WORD solidPixels;
    UBYTE pen;

    memset(obstacles, 0, sizeof(obstacles));
    for (gridY = 0; gridY < BOARD_H; ++gridY) {
        for (gridX = 0; gridX < BOARD_W; ++gridX) {
            solidPixels = 0;
            for (y = gridY * CELL_SIZE + 2;
                 y < gridY * CELL_SIZE + 6; ++y) {
                for (x = gridX * CELL_SIZE + 2;
                     x < gridX * CELL_SIZE + 6; ++x) {
                    pen = getBackgroundPen(x, y);
                    if (pen > LAST_SAFE_PEN && pen < PEN_SNAKE) {
                        ++solidPixels;
                    }
                }
            }
            if (solidPixels >= 4) {
                obstacles[gridY][gridX] = 1;
            }
        }
    }

    /* Always preserve the initial horizontal spawn corridor. */
    for (x = 15; x < 22; ++x) {
        obstacles[13][x] = 0;
    }
}

static void restoreGamePens(void)
{
    SetRGB4(&gameScreen->ViewPort, PEN_SNAKE, 0, 12, 0);
    SetRGB4(&gameScreen->ViewPort, PEN_FOOD, 15, 2, 2);
    SetRGB4(&gameScreen->ViewPort, PEN_HEAD, 8, 15, 8);
    SetRGB4(&gameScreen->ViewPort, PEN_TEXT, 15, 15, 15);
}

static BOOL loadBackground(const char *filename)
{
    BPTR file;
    UBYTE id[4];
    UBYTE header[20];
    UBYTE colors[48];
    ULONG formSize;
    ULONG chunkSize;
    ULONG bytesRead;
    LONG skip;
    WORD width;
    WORD height;
    WORD depth;
    WORD row;
    WORD plane;
    WORD pen;
    BOOL haveHeader;
    BOOL haveBody;

    if (!backgroundBitmapReady) {
        return FALSE;
    }

    file = Open((STRPTR)filename, MODE_OLDFILE);
    if (file == 0) {
        return FALSE;
    }

    haveHeader = FALSE;
    haveBody = FALSE;
    memset(obstacles, 0, sizeof(obstacles));

    if (!readExact(file, id, 4) || memcmp(id, "FORM", 4) != 0 ||
        !readExact(file, &formSize, 4) ||
        !readExact(file, id, 4) || memcmp(id, "ILBM", 4) != 0) {
        Close(file);
        return FALSE;
    }

    while (readExact(file, id, 4) &&
           readExact(file, &chunkSize, 4)) {
        bytesRead = 0;

        if (memcmp(id, "BMHD", 4) == 0 && chunkSize >= 20) {
            if (!readExact(file, header, 20)) {
                break;
            }
            bytesRead = 20;
            width = (WORD)(((UWORD)header[0] << 8) | header[1]);
            height = (WORD)(((UWORD)header[2] << 8) | header[3]);
            depth = header[8];
            if (width == SCREEN_W && height == BACKGROUND_H &&
                depth == 4 && header[10] == 0) {
                haveHeader = TRUE;
            }
        } else if (memcmp(id, "CMAP", 4) == 0 && chunkSize >= 48) {
            if (!readExact(file, colors, 48)) {
                break;
            }
            bytesRead = 48;
            for (pen = 0; pen <= 11; ++pen) {
                SetRGB4(&gameScreen->ViewPort, pen,
                    colors[pen * 3] >> 4,
                    colors[pen * 3 + 1] >> 4,
                    colors[pen * 3 + 2] >> 4);
            }
            restoreGamePens();
        } else if (memcmp(id, "BODY", 4) == 0 && haveHeader &&
                   chunkSize >= (ULONG)(SCREEN_W / 8 *
                                        BACKGROUND_H * 4)) {
            for (row = 0; row < BACKGROUND_H; ++row) {
                for (plane = 0; plane < 4; ++plane) {
                    UBYTE *destination;

                    destination =
                        (UBYTE *)backgroundBitmap.Planes[plane] +
                        row * backgroundBitmap.BytesPerRow;
                    if (!readExact(file, destination, SCREEN_W / 8)) {
                        Close(file);
                        return FALSE;
                    }
                    bytesRead += SCREEN_W / 8;
                }
            }
            haveBody = TRUE;
        } else if (memcmp(id, "SNKM", 4) == 0 &&
                   chunkSize >= sizeof(obstacles)) {
            if (!readExact(file, obstacles, sizeof(obstacles))) {
                break;
            }
            bytesRead = sizeof(obstacles);
        }

        skip = (LONG)(chunkSize - bytesRead);
        if (chunkSize & 1) {
            ++skip;
        }
        if (skip > 0 && Seek(file, skip, OFFSET_CURRENT) == -1) {
            break;
        }
    }

    Close(file);
    if (!haveHeader || !haveBody) {
        memset(obstacles, 0, sizeof(obstacles));
        restoreGamePens();
        return FALSE;
    }
    deriveCollisionFromBackground();
    return TRUE;
}

static BOOL snakeOccupies(WORD x, WORD y, WORD count)
{
    WORD i;

    for (i = 0; i < count; ++i) {
        if (snake[i].x == x && snake[i].y == y) {
            return TRUE;
        }
    }
    return FALSE;
}

static void drawCell(WORD x, WORD y, ULONG pen)
{
    WORD px;
    WORD py;

    px = x * CELL_SIZE;
    py = BOARD_Y + y * CELL_SIZE;
    SetAPen(rp, pen);
    RectFill(rp, px + 1, py + 1, px + CELL_SIZE - 2, py + CELL_SIZE - 2);
}

static void drawBackgroundCell(WORD x, WORD y)
{
    WORD px;
    WORD py;
    ULONG pen;

    px = x * CELL_SIZE;
    py = BOARD_Y + y * CELL_SIZE;
    pen = ((x + y) & 1) ? 4 : 5;
    SetAPen(rp, pen);
    RectFill(rp, px, py, px + CELL_SIZE - 1, py + CELL_SIZE - 1);

    if (arenaNumber == 0 && ((x % 10) == 0 || (y % 9) == 0)) {
        SetAPen(rp, 9);
        if ((x % 10) == 0) {
            RectFill(rp, px + 3, py, px + 3, py + CELL_SIZE - 1);
        }
        if ((y % 9) == 0) {
            RectFill(rp, px, py + 3, px + CELL_SIZE - 1, py + 3);
        }
    } else if (arenaNumber == 1 && ((x + y) % 13) == 0) {
        SetAPen(rp, 10);
        RectFill(rp, px + 2, py + 2, px + 5, py + 5);
        SetAPen(rp, 11);
        RectFill(rp, px + 4, py + 2, px + 5, py + 3);
    } else if (arenaNumber == 2 && (y == 2 || y == BOARD_H - 3)) {
        SetAPen(rp, 12);
        RectFill(rp, px, py + 5, px + CELL_SIZE - 1, py + 5);
    }
}

static void clearCell(WORD x, WORD y)
{
    if (backgroundLoaded) {
        BltBitMap(&backgroundBitmap,
            x * CELL_SIZE, y * CELL_SIZE,
            rp->BitMap,
            x * CELL_SIZE, BOARD_Y + y * CELL_SIZE,
            CELL_SIZE, CELL_SIZE, 0xc0, 0xff, NULL);
    } else {
        drawBackgroundCell(x, y);
    }
}

static void setObstacleRect(WORD x, WORD y, WORD w, WORD h, UBYTE type)
{
    WORD xx;
    WORD yy;

    for (yy = y; yy < y + h; ++yy) {
        for (xx = x; xx < x + w; ++xx) {
            if (xx >= 0 && xx < BOARD_W && yy >= 0 && yy < BOARD_H) {
                obstacles[yy][xx] = type;
            }
        }
    }
}

static void buildArena(void)
{
    memset(obstacles, 0, sizeof(obstacles));

    if (arenaNumber == 0) {
        setObstacleRect(4, 4, 8, 2, 1);
        setObstacleRect(28, 4, 8, 2, 1);
        setObstacleRect(4, 21, 8, 2, 1);
        setObstacleRect(28, 21, 8, 2, 1);
        setObstacleRect(8, 10, 2, 5, 2);
        setObstacleRect(30, 12, 2, 5, 2);
    } else if (arenaNumber == 1) {
        setObstacleRect(5, 5, 3, 3, 3);
        setObstacleRect(32, 5, 3, 3, 3);
        setObstacleRect(5, 19, 3, 3, 3);
        setObstacleRect(32, 19, 3, 3, 3);
        setObstacleRect(13, 8, 2, 5, 2);
        setObstacleRect(25, 14, 2, 5, 2);
    } else {
        setObstacleRect(3, 4, 10, 2, 3);
        setObstacleRect(27, 4, 10, 2, 3);
        setObstacleRect(3, 21, 10, 2, 3);
        setObstacleRect(27, 21, 10, 2, 3);
        setObstacleRect(11, 11, 4, 2, 1);
        setObstacleRect(25, 14, 4, 2, 1);
    }
}

static void drawObstacle(WORD x, WORD y, UBYTE type)
{
    WORD px;
    WORD py;
    ULONG bodyPen;

    px = x * CELL_SIZE;
    py = BOARD_Y + y * CELL_SIZE;
    bodyPen = type == 1 ? 6 : (type == 2 ? 7 : 8);

    SetAPen(rp, 0);
    RectFill(rp, px, py, px + CELL_SIZE - 1, py + CELL_SIZE - 1);
    SetAPen(rp, bodyPen);
    RectFill(rp, px + 1, py + 1, px + CELL_SIZE - 2, py + CELL_SIZE - 2);
    SetAPen(rp, type == 2 ? 13 : 14);
    RectFill(rp, px + 2, py + 2, px + CELL_SIZE - 3, py + 2);
}

static void drawArena(void)
{
    WORD x;
    WORD y;

    if (backgroundLoaded) {
        BltBitMap(&backgroundBitmap, 0, 0, rp->BitMap,
            0, BOARD_Y, SCREEN_W, BACKGROUND_H,
            0xc0, 0xff, NULL);
        WaitBlit();
        return;
    }

    for (y = 0; y < BOARD_H; ++y) {
        for (x = 0; x < BOARD_W; ++x) {
            if (obstacles[y][x] != 0) {
                drawObstacle(x, y, obstacles[y][x]);
            } else {
                drawBackgroundCell(x, y);
            }
        }
    }
}

static void drawStatus(void)
{
    char text[80];

    SetAPen(rp, 0);
    RectFill(rp, 0, 0, SCREEN_W - 1, BOARD_Y - 2);
    SetAPen(rp, PEN_TEXT);
    sprintf(text, "SNAKE %lu   %s", score, arenaNames[arenaNumber]);
    Move(rp, 8, 12);
    Text(rp, text, (ULONG)strlen(text));

    Move(rp, 8, 28);
    if (gameOver) {
        Text(rp, "GAME OVER - SPACE TO RESTART", 28);
    } else if (paused) {
        Text(rp, "PAUSED - SPACE TO CONTINUE", 26);
    } else {
        Text(rp, "ARROWS/WASD MOVE  SPACE PAUSE  ESC QUIT", 39);
    }
}

static void drawBoard(void)
{
    WORD i;

    drawArena();
    drawCell(food.x, food.y, PEN_FOOD);
    for (i = snakeLength - 1; i >= 0; --i) {
        drawCell(snake[i].x, snake[i].y,
            i == 0 ? PEN_HEAD : PEN_SNAKE);
    }
    drawStatus();
}

static void placeFood(void)
{
    WORD x;
    WORD y;

    if (snakeLength >= MAX_SNAKE) {
        gameOver = TRUE;
        return;
    }

    do {
        x = (WORD)(nextRandom() % BOARD_W);
        y = (WORD)(nextRandom() % BOARD_H);
    } while (snakeOccupies(x, y, snakeLength) ||
             obstacles[y][x] != 0);

    food.x = x;
    food.y = y;
    foodAgeTicks = 0;
}

static void relocateFood(void)
{
    struct Point oldFood;

    oldFood = food;
    placeFood();
    clearCell(oldFood.x, oldFood.y);
    drawCell(food.x, food.y, PEN_FOOD);
}

static void startGame(void)
{
    WORD i;
    WORD newArena;
    WORD musicNumber;

    snakeLength = 5;
    for (i = 0; i < snakeLength; ++i) {
        snake[i].x = 20 - i;
        snake[i].y = 13;
    }
    direction = DIR_RIGHT;
    wantedDirection = DIR_RIGHT;
    score = 0;
    gameOver = FALSE;
    paused = FALSE;
    randomState ^= (ULONG)gameScreen->MouseX;
    randomState ^= ((ULONG)gameScreen->MouseY << 16);
    newArena = (WORD)(nextRandom() % ARENA_COUNT);
    if (arenaNumber >= 0 && newArena == arenaNumber) {
        newArena = (WORD)((newArena + 1) % ARENA_COUNT);
    }
    arenaNumber = newArena;
    musicNumber = (WORD)(nextRandom() % GAME_MUSIC_COUNT);
    if (musicNumber == lastMusicNumber) {
        musicNumber = (WORD)((musicNumber + 1 +
            nextRandom() % (GAME_MUSIC_COUNT - 1)) % GAME_MUSIC_COUNT);
    }
    lastMusicNumber = musicNumber;
    musicPlay(gameMusicFiles[musicNumber]);
    backgroundLoaded = loadBackground(arenaFiles[arenaNumber]);
    if (!backgroundLoaded) {
        buildArena();
    }
    placeFood();
    drawBoard();
}

static BOOL isOpposite(WORD a, WORD b)
{
    return ((a == DIR_UP && b == DIR_DOWN) ||
            (a == DIR_DOWN && b == DIR_UP) ||
            (a == DIR_LEFT && b == DIR_RIGHT) ||
            (a == DIR_RIGHT && b == DIR_LEFT));
}

static UBYTE titleGlyphRow(char character, WORD row)
{
    static const UBYTE glyphA[7] = {14, 17, 17, 31, 17, 17, 17};
    static const UBYTE glyphC[7] = {14, 17, 16, 16, 16, 17, 14};
    static const UBYTE glyphD[7] = {30, 17, 17, 17, 17, 17, 30};
    static const UBYTE glyphE[7] = {31, 16, 16, 30, 16, 16, 31};
    static const UBYTE glyphG[7] = {14, 17, 16, 23, 17, 17, 14};
    static const UBYTE glyphH[7] = {17, 17, 17, 31, 17, 17, 17};
    static const UBYTE glyphI[7] = {31, 4, 4, 4, 4, 4, 31};
    static const UBYTE glyphK[7] = {17, 18, 20, 24, 20, 18, 17};
    static const UBYTE glyphM[7] = {17, 27, 21, 21, 17, 17, 17};
    static const UBYTE glyphN[7] = {17, 25, 21, 19, 17, 17, 17};
    static const UBYTE glyphP[7] = {30, 17, 17, 30, 16, 16, 16};
    static const UBYTE glyphR[7] = {30, 17, 17, 30, 20, 18, 17};
    static const UBYTE glyphS[7] = {15, 16, 16, 14, 1, 1, 30};
    static const UBYTE glyphQuote[7] = {4, 4, 8, 0, 0, 0, 0};
    const UBYTE *glyph;

    glyph = NULL;
    switch (character) {
        case 'A': glyph = glyphA; break;
        case 'C': glyph = glyphC; break;
        case 'D': glyph = glyphD; break;
        case 'E': glyph = glyphE; break;
        case 'G': glyph = glyphG; break;
        case 'H': glyph = glyphH; break;
        case 'I': glyph = glyphI; break;
        case 'K': glyph = glyphK; break;
        case 'M': glyph = glyphM; break;
        case 'N': glyph = glyphN; break;
        case 'P': glyph = glyphP; break;
        case 'R': glyph = glyphR; break;
        case 'S': glyph = glyphS; break;
        case '\'': glyph = glyphQuote; break;
    }
    return glyph == NULL ? 0 : glyph[row];
}

static void drawTitleText(const char *text, WORD y, WORD scale,
                          ULONG topPen, ULONG bottomPen)
{
    WORD length;
    WORD width;
    WORD startX;
    WORD character;
    WORD row;
    WORD column;
    UBYTE bits;
    WORD x;
    ULONG pen;

    length = (WORD)strlen(text);
    width = length * 6 * scale - scale;
    startX = (SCREEN_W - width) / 2;

    for (row = 0; row < 7; ++row) {
        pen = row < 3 ? topPen : bottomPen;
        for (character = 0; character < length; ++character) {
            bits = titleGlyphRow(text[character], row);
            for (column = 0; column < 5; ++column) {
                if ((bits & (1 << (4 - column))) != 0) {
                    x = startX + character * 6 * scale +
                        column * scale;
                    SetAPen(rp, 5);
                    RectFill(rp, x + 2, y + row * scale + 2,
                        x + scale + 1, y + (row + 1) * scale + 1);
                    SetAPen(rp, pen);
                    RectFill(rp, x, y + row * scale,
                        x + scale - 1, y + (row + 1) * scale - 1);
                }
            }
        }
    }
}

static void drawCenteredText(const char *text, WORD y, ULONG pen)
{
    WORD width;

    width = (WORD)TextLength(rp, (STRPTR)text, (ULONG)strlen(text));
    SetAPen(rp, pen);
    Move(rp, (SCREEN_W - width) / 2, y);
    Text(rp, (STRPTR)text, (ULONG)strlen(text));
}

static UBYTE smallGlyphRow(char character, WORD row)
{
    static const char charset[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!-_";
    static const UBYTE glyphs[][7] = {
        {0,0,0,0,0,0,0},
        {14,17,17,31,17,17,17}, {30,17,17,30,17,17,30},
        {14,17,16,16,16,17,14}, {30,17,17,17,17,17,30},
        {31,16,16,30,16,16,31}, {31,16,16,30,16,16,16},
        {14,17,16,23,17,17,14}, {17,17,17,31,17,17,17},
        {31,4,4,4,4,4,31}, {7,2,2,2,2,18,12},
        {17,18,20,24,20,18,17}, {16,16,16,16,16,16,31},
        {17,27,21,21,17,17,17}, {17,25,21,19,17,17,17},
        {14,17,17,17,17,17,14}, {30,17,17,30,16,16,16},
        {14,17,17,17,21,18,13}, {30,17,17,30,20,18,17},
        {15,16,16,14,1,1,30}, {31,4,4,4,4,4,4},
        {17,17,17,17,17,17,14}, {17,17,17,17,17,10,4},
        {17,17,17,21,21,21,10}, {17,17,10,4,10,17,17},
        {17,17,10,4,4,4,4}, {31,1,2,4,8,16,31},
        {14,17,19,21,25,17,14}, {4,12,4,4,4,4,14},
        {14,17,1,2,4,8,31}, {30,1,1,14,1,1,30},
        {2,6,10,18,31,2,2}, {31,16,16,30,1,1,30},
        {14,16,16,30,17,17,14}, {31,1,2,4,8,8,8},
        {14,17,17,14,17,17,14}, {14,17,17,15,1,1,14},
        {4,4,4,4,4,0,4}, {0,0,0,31,0,0,0},
        {0,0,0,0,0,0,31}
    };
    const char *found;

    if (character >= 'a' && character <= 'z') {
        character = (char)(character - 'a' + 'A');
    }
    found = strchr(charset, character);
    if (found == NULL || row < 0 || row >= 7) {
        return 0;
    }
    return glyphs[found - charset][row];
}

static void drawSmallText(const char *text, WORD x, WORD y, ULONG pen)
{
    WORD character;
    WORD row;
    WORD column;
    UBYTE bits;

    SetAPen(rp, pen);
    for (character = 0; text[character] != '\0'; ++character) {
        for (row = 0; row < 7; ++row) {
            bits = smallGlyphRow(text[character], row);
            for (column = 0; column < 5; ++column) {
                if (bits & (1 << (4 - column))) {
                    WritePixel(rp, x + character * 6 + column, y + row);
                }
            }
        }
    }
}

static void drawHighScoreTable(void)
{
    char text[24];
    WORD i;
    WORD y;
    WORD scoreX;
    ULONG pen;

    for (i = 0; i < HIGH_SCORE_COUNT; ++i) {
        y = (WORD)(82 + i * 12);
        pen = i == 0 ? 14 : (i & 1 ? 6 : 7);
        if (i == newestHighScoreRank &&
            ((highScoreFrame / 18) & 1) == 0) {
            pen = 10;
        }
        sprintf(text, "%d", i + 1);
        drawSmallText(text, i == 9 ? 91 : 97, y, pen);
        drawSmallText(highScores[i].name, 104, y, pen);
        sprintf(text, "%lu", highScores[i].score);
        scoreX = (WORD)(231 - strlen(text) * 6);
        drawSmallText(text, scoreX, y, pen);
    }
}

static void drawHighScoreScreen(void)
{
    if (!highScoreImageLoaded) {
        SetAPen(rp, 0);
        RectFill(rp, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
        drawCenteredText("CHIPSNAKE HALL OF FAME", 30, 15);
    }
    drawHighScoreTable();
}

static void drawNameCursor(void)
{
    WORD x;

    x = (WORD)((SCREEN_W - enteredNameLength * 6) / 2 +
        enteredNameLength * 6);
    if (highScoreImageLoaded) {
        BltBitMap(&introBitmap, x, 157, rp->BitMap, x, 157,
            6, 7, 0xc0, 0xff, NULL);
        WaitBlit();
    } else {
        SetAPen(rp, 1);
        RectFill(rp, x, 157, x + 5, 163);
    }
    if (((highScoreFrame / 15) & 1) == 0) {
        drawSmallText("_", x, 157, 10);
    }
}

static void drawNameEntryScreen(void)
{
    char text[32];
    WORD x;

    if (highScoreImageLoaded) {
        BltBitMap(&introBitmap, 88, 78, rp->BitMap, 88, 78,
            145, 129, 0xc0, 0xff, NULL);
        WaitBlit();
    } else {
        SetAPen(rp, 1);
        RectFill(rp, 70, 65, 249, 211);
    }
    drawSmallText("NEW HIGH SCORE!", 115, 91, 14);
    sprintf(text, "SCORE %lu", score);
    x = (WORD)((SCREEN_W - strlen(text) * 6) / 2);
    drawSmallText(text, x, 116, 7);
    drawSmallText("ENTER YOUR NAME", 115, 139, 6);
    x = (WORD)((SCREEN_W - enteredNameLength * 6) / 2);
    drawSmallText(enteredName, x, 157, 15);
    drawNameCursor();
    drawSmallText("RETURN TO SAVE", 118, 190, 9);
}

static void prepareHighScoreArtwork(void)
{
    highScoreImageLoaded =
        loadIntroImage("assets/highscore/chipsnake-highscore.iff");
    if (highScoreImageLoaded) {
        BltBitMap(&introBitmap, 0, 0, rp->BitMap, 0, 0,
            SCREEN_W, SCREEN_H, 0xc0, 0xff, NULL);
        WaitBlit();
    } else {
        setTitlePalette();
        SetAPen(rp, 0);
        RectFill(rp, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
    }
    highScoreFrame = 0;
}

static void enterHighScoreScreen(BOOL startMusic)
{
    screenMode = MODE_HIGH_SCORES;
    prepareHighScoreArtwork();
    if (startMusic) {
        musicPlay("music/chipsnake-hall.mod");
    }
    drawHighScoreScreen();
}

static void enterNameEntry(WORD rank)
{
    pendingHighScoreRank = rank;
    strncpy(enteredName, lastPlayerName, HIGH_NAME_LENGTH);
    enteredName[HIGH_NAME_LENGTH] = '\0';
    enteredNameLength = (WORD)strlen(enteredName);
    newestHighScoreRank = -1;
    screenMode = MODE_NAME_ENTRY;
    prepareHighScoreArtwork();
    musicPlay("music/chipsnake-hall.mod");
    drawNameEntryScreen();
}

static void finishNameEntry(void)
{
    if (enteredNameLength == 0) {
        strcpy(enteredName, "PLAYER");
        enteredNameLength = 6;
    }
    strcpy(lastPlayerName, enteredName);
    insertHighScore(pendingHighScoreRank, enteredName, score);
    newestHighScoreRank = pendingHighScoreRank;
    saveHighScores();
    enterHighScoreScreen(FALSE);
}

static void beginGameOverFlow(void)
{
    WORD rank;

    highScoreFromGame = TRUE;
    rank = findHighScoreRank(score);
    if (rank >= 0) {
        enterNameEntry(rank);
    } else {
        newestHighScoreRank = -1;
        enterHighScoreScreen(TRUE);
    }
}

static void setTitlePalette(void)
{
    SetRGB4(&gameScreen->ViewPort, 0, 0, 0, 1);
    SetRGB4(&gameScreen->ViewPort, 1, 0, 1, 4);
    SetRGB4(&gameScreen->ViewPort, 2, 1, 3, 8);
    SetRGB4(&gameScreen->ViewPort, 3, 2, 6, 13);
    SetRGB4(&gameScreen->ViewPort, 4, 0, 8, 4);
    SetRGB4(&gameScreen->ViewPort, 5, 2, 3, 5);
    SetRGB4(&gameScreen->ViewPort, 6, 13, 3, 1);
    SetRGB4(&gameScreen->ViewPort, 7, 15, 8, 1);
    SetRGB4(&gameScreen->ViewPort, 8, 15, 13, 2);
    SetRGB4(&gameScreen->ViewPort, 9, 3, 12, 3);
    SetRGB4(&gameScreen->ViewPort, 10, 2, 7, 15);
    SetRGB4(&gameScreen->ViewPort, 11, 8, 4, 13);
    SetRGB4(&gameScreen->ViewPort, 12, 0, 12, 0);
    SetRGB4(&gameScreen->ViewPort, 13, 15, 2, 2);
    SetRGB4(&gameScreen->ViewPort, 14, 8, 15, 8);
    SetRGB4(&gameScreen->ViewPort, 15, 15, 15, 15);
}

static void initializeTitleScreen(void)
{
    WORD i;

    titleFrame = 0;
    for (i = 0; i < STAR_COUNT; ++i) {
        stars[i].x = (WORD)(nextRandom() % SCREEN_W);
        stars[i].y = (WORD)(187 + nextRandom() % 29);
        stars[i].speed = (WORD)(1 + nextRandom() % 3);
    }
    introImageLoaded = loadIntroImage("assets/intro/chipsnake-intro.iff");
    musicPlay("music/chipsnake-neon.mod");
    if (introImageLoaded) {
        BltBitMap(&introBitmap, 0, 0, rp->BitMap, 0, 0,
            SCREEN_W, SCREEN_H, 0xc0, 0xff, NULL);
        WaitBlit();
    } else {
        setTitlePalette();
    }
}

static void drawTitleScreen(void)
{
    WORD i;
    WORD bob;
    WORD barOffset;
    WORD textWidth;
    WORD characterWidth;
    LONG marqueeX;
    LONG characterX;
    ULONG starPen;

    if (introImageLoaded) {
        /*
         * Compose the complete animated strip off-screen.  The display sees
         * one finished blit instead of the clear, shadow and white text being
         * drawn as separate visible operations.
         */
        SetAPen(&introStripRp, 0);
        RectFill(&introStripRp, 0, 0, SCREEN_W - 1, 71);

        /* Stationary stars blink at different rates in the empty dark area. */
        for (i = 0; i < 18; ++i) {
            if (((titleFrame / (ULONG)(5 + stars[i].speed * 3) +
                  (ULONG)i) & 3) != 0) {
                starPen = ((titleFrame / 7 + i) & 1) ? 3 : 15;
                SetAPen(&introStripRp, starPen);
                WritePixel(&introStripRp, stars[i].x, stars[i].y - 184);
                if (stars[i].speed == 3 &&
                    ((titleFrame / 9 + i) & 3) == 0) {
                    WritePixel(&introStripRp,
                        stars[i].x - 1, stars[i].y - 184);
                    WritePixel(&introStripRp,
                        stars[i].x + 1, stars[i].y - 184);
                    WritePixel(&introStripRp,
                        stars[i].x, stars[i].y - 185);
                    WritePixel(&introStripRp,
                        stars[i].x, stars[i].y - 183);
                }
            }
        }

        textWidth = (WORD)TextLength(&introStripRp, (STRPTR)marqueeText,
            (ULONG)strlen(marqueeText));
        marqueeX = SCREEN_W -
            (LONG)(titleFrame % (ULONG)(textWidth + SCREEN_W));
        characterWidth = (WORD)TextLength(&introStripRp,
            (STRPTR)marqueeText, 1);

        /*
         * This RastPort has no Layer to clip negative coordinates.  Draw
         * only complete characters inside the bitmap so the scrolling text
         * can never wrap into or corrupt another scanline.
         */
        for (i = 0; marqueeText[i] != '\0'; ++i) {
            characterX = marqueeX + (LONG)i * characterWidth;
            if (characterX >= 0 &&
                characterX + characterWidth <= SCREEN_W) {
                if (characterX + characterWidth + 2 <= SCREEN_W) {
                    SetAPen(&introStripRp, 4);
                    Move(&introStripRp, characterX + 2, 60);
                    Text(&introStripRp, (STRPTR)&marqueeText[i], 1);
                }
        SetAPen(&introStripRp, 15);
                Move(&introStripRp, characterX, 58);
                Text(&introStripRp, (STRPTR)&marqueeText[i], 1);
            }
        }
        if (!musicIsReady()) {
            SetAPen(&introStripRp, 13);
            Move(&introStripRp, 4, 14);
            Text(&introStripRp, "AUDIO DEVICE UNAVAILABLE", 24);
        }

        BltBitMap(&backgroundBitmap, 0, 0, rp->BitMap, 0, 184,
            SCREEN_W, 72, 0xc0, 0xff, NULL);
        WaitBlit();

        /* Retain the copper-like colour pulse in the title artwork. */
        if (((titleFrame / 4) & 1) == 0) {
            SetRGB4(&gameScreen->ViewPort, 7, 15, 8, 1);
            SetRGB4(&gameScreen->ViewPort, 8, 15, 13, 3);
        } else {
            SetRGB4(&gameScreen->ViewPort, 7, 15, 12, 2);
            SetRGB4(&gameScreen->ViewPort, 8, 15, 7, 1);
        }

        ++titleFrame;
        return;
    }

    SetAPen(rp, 0);
    RectFill(rp, 0, 0, SCREEN_W - 1, SCREEN_H - 1);

    for (i = 0; i < STAR_COUNT; ++i) {
        stars[i].x -= stars[i].speed;
        if (stars[i].x < 0) {
            stars[i].x = SCREEN_W - 1;
            stars[i].y = (WORD)(nextRandom() % SCREEN_H);
        }
        starPen = stars[i].speed == 1 ? 2 :
            (stars[i].speed == 2 ? 10 : 15);
        SetAPen(rp, starPen);
        RectFill(rp, stars[i].x, stars[i].y,
            stars[i].x + stars[i].speed - 1, stars[i].y);
    }

    barOffset = (WORD)((titleFrame / 2) % 16);
    SetAPen(rp, 6);
    RectFill(rp, 0, 25 + barOffset / 8, SCREEN_W - 1,
        26 + barOffset / 8);
    SetAPen(rp, 7);
    RectFill(rp, 0, 28 + barOffset / 8, SCREEN_W - 1,
        29 + barOffset / 8);
    SetAPen(rp, 8);
    RectFill(rp, 0, 31 + barOffset / 8, SCREEN_W - 1,
        32 + barOffset / 8);

    bob = (WORD)((titleFrame / 4) % 12);
    if (bob > 6) {
        bob = 12 - bob;
    }
    drawTitleText("MRDIG'S", 47 + bob, 3, 7, 6);
    drawTitleText("CHIPSNAKE", 76 + bob, 4, 15, 10);

    SetAPen(rp, 6);
    RectFill(rp, 38, 119, 281, 120);
    SetAPen(rp, 8);
    RectFill(rp, 64, 123, 255, 124);
    SetAPen(rp, 9);
    RectFill(rp, 96, 127, 223, 128);

    drawCenteredText("MrDig Productions", 157, 14);
    drawCenteredText("Copyright 2026", 176, 15);
    drawCenteredText("This game is made by 100% AI", 195, 3);

    if (((titleFrame / 25) & 1) == 0) {
        drawCenteredText("SPACE TO START", 226, 8);
    }
    drawCenteredText("ESC TO QUIT", 244, 2);
    ++titleFrame;
}

static char rawKeyCharacter(UWORD code)
{
    static const char topRow[] = "QWERTYUIOP";
    static const char homeRow[] = "ASDFGHJKL";
    static const char bottomRow[] = "ZXCVBNM";

    if (code >= 0x10 && code <= 0x19) {
        return topRow[code - 0x10];
    }
    if (code >= 0x20 && code <= 0x28) {
        return homeRow[code - 0x20];
    }
    if (code >= 0x31 && code <= 0x37) {
        return bottomRow[code - 0x31];
    }
    if (code >= 0x01 && code <= 0x09) {
        return (char)('1' + code - 0x01);
    }
    if (code == 0x0a) {
        return '0';
    }
    if (code == RAWKEY_SPACE) {
        return ' ';
    }
    return '\0';
}

static void returnToTitle(void)
{
    screenMode = MODE_TITLE;
    newestHighScoreRank = -1;
    initializeTitleScreen();
    drawTitleScreen();
}

static void handleKey(UWORD code)
{
    WORD newDirection;
    char character;

    if (code & IECODE_UP_PREFIX) {
        return;
    }

    if (screenMode == MODE_TITLE) {
        if (code == RAWKEY_ESC) {
            quitGame = TRUE;
        } else if (code == RAWKEY_SPACE) {
            screenMode = MODE_GAME;
            startGame();
        } else if (code == RAWKEY_H) {
            newestHighScoreRank = -1;
            highScoreFromGame = FALSE;
            enterHighScoreScreen(TRUE);
        }
        return;
    }

    if (screenMode == MODE_NAME_ENTRY) {
        if (code == RAWKEY_ESC) {
            returnToTitle();
        } else if (code == RAWKEY_RETURN) {
            finishNameEntry();
        } else if (code == RAWKEY_BACKSPACE) {
            if (enteredNameLength > 0) {
                --enteredNameLength;
                enteredName[enteredNameLength] = '\0';
                drawNameEntryScreen();
            }
        } else {
            character = rawKeyCharacter(code);
            if (character != '\0' &&
                enteredNameLength < HIGH_NAME_LENGTH) {
                enteredName[enteredNameLength++] = character;
                enteredName[enteredNameLength] = '\0';
                drawNameEntryScreen();
            }
        }
        return;
    }

    if (screenMode == MODE_HIGH_SCORES) {
        if (code == RAWKEY_ESC) {
            returnToTitle();
        } else if (code == RAWKEY_RETURN || code == RAWKEY_SPACE) {
            if (highScoreFromGame) {
                screenMode = MODE_GAME;
                startGame();
            } else {
                returnToTitle();
            }
        }
        return;
    }

    if (code == RAWKEY_ESC) {
        returnToTitle();
        return;
    }

    if (code == RAWKEY_SPACE) {
        if (gameOver) {
            startGame();
        } else {
            paused = !paused;
            drawStatus();
        }
        return;
    }

    if (code == RAWKEY_N) {
        startGame();
        return;
    }

    newDirection = -1;
    if (code == RAWKEY_UP || code == RAWKEY_W) {
        newDirection = DIR_UP;
    } else if (code == RAWKEY_RIGHT || code == RAWKEY_D) {
        newDirection = DIR_RIGHT;
    } else if (code == RAWKEY_DOWN || code == RAWKEY_S) {
        newDirection = DIR_DOWN;
    } else if (code == RAWKEY_LEFT || code == RAWKEY_A) {
        newDirection = DIR_LEFT;
    }

    if (newDirection >= 0 && !isOpposite(newDirection, direction)) {
        wantedDirection = newDirection;
    }
}

static void pollJoystick(void)
{
    volatile UWORD *joy1dat;
    volatile UBYTE *ciaaPra;
    UWORD value;
    BOOL left;
    BOOL right;
    BOOL up;
    BOOL down;
    BOOL fire;
    WORD newDirection;

    /* Port 2: the standard Amiga game joystick port. */
    joy1dat = (volatile UWORD *)0xdff00c;
    ciaaPra = (volatile UBYTE *)0xbfe001;
    value = *joy1dat;
    left = (value & 0x0200) != 0;
    right = (value & 0x0002) != 0;

    /*
     * JOY1DAT contains quadrature counter bits. Vertical directions are
     * obtained by XORing each vertical bit with its horizontal partner.
     */
    up = ((value & 0x0100) != 0) ^ left;
    down = ((value & 0x0001) != 0) ^ right;
    fire = ((*ciaaPra & 0x80) == 0);

    if (screenMode == MODE_GAME && !paused && !gameOver) {
        newDirection = -1;
        if (up) {
            newDirection = DIR_UP;
        } else if (right) {
            newDirection = DIR_RIGHT;
        } else if (down) {
            newDirection = DIR_DOWN;
        } else if (left) {
            newDirection = DIR_LEFT;
        }
        if (newDirection >= 0 &&
            !isOpposite(newDirection, direction)) {
            wantedDirection = newDirection;
        }
    }

    /* One action per press; holding fire must not repeatedly pause/start. */
    if (fire && !joystickFire && screenMode != MODE_NAME_ENTRY) {
        handleKey(RAWKEY_SPACE);
    }
    joystickFire = fire;
}

static void readInput(void)
{
    struct IntuiMessage *message;
    ULONG messageClass;
    UWORD code;

    while ((message = (struct IntuiMessage *)GetMsg(gameWindow->UserPort)) != NULL) {
        messageClass = message->Class;
        code = message->Code;
        ReplyMsg((struct Message *)message);

        if (messageClass == IDCMP_RAWKEY) {
            handleKey(code);
        }
    }
    pollJoystick();
}

static void moveSnake(void)
{
    struct Point newHead;
    struct Point oldTail;
    BOOL growing;
    WORD collisionCount;
    WORD i;

    direction = wantedDirection;
    newHead = snake[0];

    if (direction == DIR_UP) {
        --newHead.y;
    } else if (direction == DIR_RIGHT) {
        ++newHead.x;
    } else if (direction == DIR_DOWN) {
        ++newHead.y;
    } else {
        --newHead.x;
    }

    if (newHead.x < 0) {
        newHead.x = BOARD_W - 1;
    } else if (newHead.x >= BOARD_W) {
        newHead.x = 0;
    }

    if (newHead.y < 0) {
        newHead.y = BOARD_H - 1;
    } else if (newHead.y >= BOARD_H) {
        newHead.y = 0;
    }

    growing = samePoint(newHead, food);
    collisionCount = growing ? snakeLength : snakeLength - 1;

    if (obstacles[newHead.y][newHead.x] != 0 ||
        snakeOccupies(newHead.x, newHead.y, collisionCount)) {
        gameOver = TRUE;
        drawStatus();
        return;
    }

    oldTail = snake[snakeLength - 1];
    if (growing && snakeLength < MAX_SNAKE) {
        ++snakeLength;
    }

    for (i = snakeLength - 1; i > 0; --i) {
        snake[i] = snake[i - 1];
    }
    snake[0] = newHead;

    if (!growing) {
        clearCell(oldTail.x, oldTail.y);
    } else {
        score += 100;
        placeFood();
        drawCell(food.x, food.y, PEN_FOOD);
        drawStatus();
    }

    if (snakeLength > 1) {
        drawCell(snake[1].x, snake[1].y, PEN_SNAKE);
    }
    drawCell(snake[0].x, snake[0].y, PEN_HEAD);
}

static BOOL openGame(void)
{
    IntuitionBase = (struct IntuitionBase *)
        OpenLibrary("intuition.library", 37);
    GfxBase = (struct GfxBase *)OpenLibrary("graphics.library", 37);
    if (IntuitionBase == NULL || GfxBase == NULL) {
        return FALSE;
    }

    gameScreen = OpenScreenTags(NULL,
        SA_Width, SCREEN_W,
        SA_Height, SCREEN_H,
        SA_Depth, 4,
        SA_DisplayID, LORES_KEY,
        SA_Title, (ULONG)"MrDig's ChipSnake",
        SA_Quiet, TRUE,
        SA_ShowTitle, FALSE,
        TAG_DONE);
    if (gameScreen == NULL) {
        return FALSE;
    }

    gameWindow = OpenWindowTags(NULL,
        WA_CustomScreen, (ULONG)gameScreen,
        WA_Left, 0,
        WA_Top, 0,
        WA_Width, SCREEN_W,
        WA_Height, SCREEN_H,
        WA_Borderless, TRUE,
        WA_Backdrop, TRUE,
        WA_Activate, TRUE,
        WA_RMBTrap, TRUE,
        WA_IDCMP, IDCMP_RAWKEY,
        TAG_DONE);
    if (gameWindow == NULL) {
        return FALSE;
    }

    hiddenPointer = (UWORD *)AllocMem(2 * sizeof(UWORD),
        MEMF_CHIP | MEMF_CLEAR);
    if (hiddenPointer != NULL) {
        SetPointer(gameWindow, hiddenPointer, 1, 16, 0, 0);
    }

    rp = gameWindow->RPort;
    allocateBackgroundBitmap();
    allocateIntroBitmap();
    InitRastPort(&introStripRp);
    introStripRp.BitMap = &backgroundBitmap;
    SetFont(&introStripRp, rp->Font);
    SetRGB4(&gameScreen->ViewPort, 0, 0, 0, 2);
    SetRGB4(&gameScreen->ViewPort, 1, 0, 2, 4);
    SetRGB4(&gameScreen->ViewPort, 2, 0, 3, 6);
    SetRGB4(&gameScreen->ViewPort, 3, 1, 4, 8);
    SetRGB4(&gameScreen->ViewPort, 4, 1, 6, 8);
    SetRGB4(&gameScreen->ViewPort, 5, 3, 4, 6);
    SetRGB4(&gameScreen->ViewPort, 6, 11, 5, 1);
    SetRGB4(&gameScreen->ViewPort, 7, 12, 11, 9);
    SetRGB4(&gameScreen->ViewPort, 8, 11, 2, 2);
    SetRGB4(&gameScreen->ViewPort, 9, 15, 13, 3);
    SetRGB4(&gameScreen->ViewPort, 10, 3, 8, 12);
    SetRGB4(&gameScreen->ViewPort, 11, 15, 7, 2);
    restoreGamePens();
    return TRUE;
}

static void closeGame(void)
{
    musicShutdown();
    freeIntroBitmap();
    freeBackgroundBitmap();
    if (gameWindow != NULL) {
        if (hiddenPointer != NULL) {
            ClearPointer(gameWindow);
            FreeMem(hiddenPointer, 2 * sizeof(UWORD));
            hiddenPointer = NULL;
        }
        CloseWindow(gameWindow);
        gameWindow = NULL;
    }
    if (gameScreen != NULL) {
        CloseScreen(gameScreen);
        gameScreen = NULL;
    }
    if (GfxBase != NULL) {
        CloseLibrary((struct Library *)GfxBase);
        GfxBase = NULL;
    }
    if (IntuitionBase != NULL) {
        CloseLibrary((struct Library *)IntuitionBase);
        IntuitionBase = NULL;
    }
}

int main(void)
{
    WORD tick;

    if (!openGame()) {
        closeGame();
        printf("Amiga Snake: could not open the game screen.\n");
        return 20;
    }

    quitGame = FALSE;
    joystickFire = FALSE;
    arenaNumber = -1;
    loadHighScores();
    musicInitialize(50);
    screenMode = MODE_TITLE;
    initializeTitleScreen();
    drawTitleScreen();
    tick = 0;

    while (!quitGame) {
        WaitTOF();
        musicFrame();
        readInput();
        if (screenMode == MODE_TITLE) {
            drawTitleScreen();
            tick = 0;
        } else if (screenMode == MODE_GAME) {
            if (!paused && !gameOver) {
                ++foodAgeTicks;
                if (foodAgeTicks >=
                    (ULONG)SysBase->VBlankFrequency *
                    FOOD_TIMEOUT_SECONDS) {
                    relocateFood();
                }

                ++tick;
                if (tick >= TICKS_PER_MOVE) {
                    tick = 0;
                    moveSnake();
                    if (gameOver) {
                        beginGameOverFlow();
                    }
                }
            } else {
                tick = 0;
            }
        } else if (screenMode == MODE_NAME_ENTRY) {
            ++highScoreFrame;
            if ((highScoreFrame % 15) == 0) {
                drawNameCursor();
            }
            tick = 0;
        } else if (screenMode == MODE_HIGH_SCORES) {
            ++highScoreFrame;
            if (newestHighScoreRank >= 0 &&
                (highScoreFrame % 18) == 0) {
                drawHighScoreTable();
            }
            tick = 0;
        }
    }

    closeGame();
    return 0;
}
