#include <assert.h>
#include <stdio.h>

#define TILE_SIZE 16
#define LEVEL_FLOOR_Y 200

static int solidAt(int x,int y)
{
    int tileX,tileY;
    if(x<0||x>=3072||y<0) return 1;
    if(y>=LEVEL_FLOOR_Y) {
        int hazard=(x>=1584&&x<1664)||(x>=2432&&x<2512)||
                   (x>=2112&&x<2176)||(x>=2784&&x<2864);
        if(!hazard) return 1;
        if(y>=224) return 0;
    }
    tileX=x/TILE_SIZE; tileY=y/TILE_SIZE;
    return ((tileX*13+tileY*7+(tileX>>2))&15)==0;
}

static int horizontalReference(int left,int right,int y)
{
    int x;
    for(x=left;x<=right;x++) if(solidAt(x,y)) return 1;
    return 0;
}

static int horizontalTile(int left,int right,int y)
{
    int x;
    for(x=left;x<=right;x=(x&~(TILE_SIZE-1))+TILE_SIZE)
        if(solidAt(x,y)) return 1;
    return 0;
}

static int verticalReference(int x,int top,int bottom)
{
    int y;
    for(y=top;y<=bottom;y++) if(solidAt(x,y)) return 1;
    return 0;
}

static int verticalTile(int x,int top,int bottom)
{
    int y;
    for(y=top;y<=bottom;) {
        int next;
        if(solidAt(x,y)) return 1;
        next=(y&~(TILE_SIZE-1))+TILE_SIZE;
        if(y<LEVEL_FLOOR_Y&&next>LEVEL_FLOOR_Y) next=LEVEL_FLOOR_Y;
        y=next;
    }
    return 0;
}

int main(void)
{
    int x,y,length;
    for(y=-1;y<=225;y++)
        for(x=-1;x<=3072;x++)
            for(length=0;length<=40;length++)
                assert(horizontalReference(x,x+length,y)==
                       horizontalTile(x,x+length,y));
    for(x=-1;x<=3072;x++)
        for(y=-1;y<=225;y++)
            for(length=0;length<=48;length++)
                assert(verticalReference(x,y,y+length)==
                       verticalTile(x,y,y+length));
    puts("PASS: tile-span collision matches pixel-span reference");
    return 0;
}
