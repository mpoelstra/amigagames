#include "disk_media.h"
#ifdef SPARKPAW_MULTI_ADF
#include <dos/dos.h>
#include <dos/dosextens.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/graphics.h>
#include <string.h>
#include "title.h"
#include "platform_amiga.h"

static UBYTE activeDrive;
/* Scan both drives without DOS requesters. Markers are bounded, versioned and
   read-only. Never retain a file or directory lock across a disk change. */
static BOOL findDisk(UBYTE disk)
{
    struct Process *process=(struct Process *)FindTask(NULL);
    APTR window=process->pr_WindowPtr;
    UBYTE drive; BOOL found=FALSE;
    char path[]="DF0:Sparkpaw.disk",marker[8];
    const char *wanted=disk==1?"SP07D1\n":"SP07D2\n";
    process->pr_WindowPtr=(APTR)-1;
    for(drive=0;drive<2;drive++) {
        BPTR file; LONG count;
        path[2]=(char)('0'+drive);
        file=Open(path,MODE_OLDFILE);
        if(!file) continue;
        count=Read(file,marker,sizeof(marker)); Close(file);
        if(count==7&&!memcmp(marker,wanted,7)) {
            activeDrive=drive; found=TRUE; break;
        }
    }
    process->pr_WindowPtr=window;
    return found;
}

BOOL diskMediaRequire(UBYTE disk)
{
    UBYTE frame;
    if(disk<1||disk>2) return FALSE;
    if(findDisk(disk)) return TRUE;
    if(!titleShowInsertDisk(disk)) return FALSE;
    for(;;) {
        if(findDisk(disk)) return titleShowReplayLoading();
        for(frame=0;frame<25;frame++) WaitTOF();
    }
}

BPTR diskMediaOpen(const char *name,LONG mode)
{
    char path[96]; const char *leaf;
    struct Process *process=(struct Process *)FindTask(NULL);
    APTR window=process->pr_WindowPtr; BPTR file;
    if(strncmp(name,"PROGDIR:assets/runtime/",23)) return Open((STRPTR)name,mode);
    leaf=name+23;
    if(strlen(leaf)>60) return 0;
    strcpy(path,"DF0:assets/runtime/"); path[2]=(char)('0'+activeDrive);
    strcat(path,leaf);
    if(strlen(path)>5&&!strcmp(path+strlen(path)-5,".spbm"))
        strcpy(path+strlen(path)-5,".spr1");
    process->pr_WindowPtr=(APTR)-1;
    file=Open(path,mode);
    process->pr_WindowPtr=window;
    return file;
}
#endif
