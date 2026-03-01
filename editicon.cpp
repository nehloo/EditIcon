
/* ICONS EDITOR 3.0 - by Gybbon (c) 1998 */

#include <alloc.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <dir.h>
#include <direct.h>
#include <stdlib.h>
#include <graphics.h>
#include <math.h>
#include <ctype.h>
#include <dos.h>
#include <time.h>
#include <bios.h>
#include <process.h>
#include <mem.h>
#include <errno.h>

void initmouse(void);
void showmouse(void);
void hidemouse(void);
void mousexy(int &,int &,int &);

void windows(int,int,int,int);
void menu(void);
void options(void);

void newfile(void);
void savefile(void);
void openfile(void);
void changebgc(void);
void changeicdim(void);
void help(int);
void about(void);
void opensavewindow(void);
void choosefiles(void);

void getbgordim(int,char*,char*);
void areyousure(void);
void showbigicon(void);
void showsmallicon(void);
void separator(int,int,int,int,int);
void getputicon(char* ,int,int);
void GetFileName(const char*,char*);
int FileExists(const char*);

void save_screen(int,int,int,int);
void restore_screen(int,int,int,int);

FILE *db;
void *image;
unsigned size;

FILE *fp;

int rc;
int ret;
int redimensrc;
int sx,sy,m; // mouse settings.

int MATR[50][50];
char f[300][15];
int i,j,q;
int k=7; // k-surface bars
int col=1; // bars' color
int bgcolor=7; // background color
int tx=100,ty=100; // table's coordinates
int xx,yy; // matrix coordinates
char fname[81];

//int endoffile=0;
int opensave;
char s[_MAX_PATH];
//char drive[_MAX_DRIVE];
//char dir[_MAX_DIR];
//char file[_MAX_FNAME];
//char ext[_MAX_EXT];

int _newfile=0,_savefile=0,_openfile=0,_changebg=0,_redimens=0,_help=0,_about=0;

int preview=1;

int child=0;

void main(int argc,char *argv[])
{
int gd=DETECT,gm;
initgraph(&gd,&gm,"bgi");
menu();

if (argc!=0)
  {
  if (!strcmp(argv[1],"child"))
    child=1;
  hidemouse();
  db=fopen(argv[1],"r+a");
  if (!db)
    {
    initmouse();
    options();
    }
  rewind(db);
  fscanf(db,"%d\n",&rc);
  fread(MATR,sizeof(MATR),1,db);
  fclose(db);
//  restore_screen(180,170);
  setfillstyle(1,8);
  bar(0,90,500,460);
  showbigicon();
  showsmallicon();
  setfillstyle(1,1);
  bar(330,5,600,18);
  settextstyle(11,0,1);
  setcolor(14);
  outtextxy(330,5,argv[1]);
  delay(150);
  }
initmouse();
options();
}

void menu()
{
rc=16;
setfillstyle(1,8);
bar(0,0,640,480);
setfillstyle(1,1);
bar(0,0,640,18);
setfillstyle(1,7);
bar(0,30,640,48);
bar(0,462,640,480);
windows(600,464,635,478);

//separator(0,400,640,0,15);

chdir("\\borlandc\\icons");
getputicon("icon.jco",2,1);
getputicon("i.jco",2,463);
getputicon("newfile.jco",2,32);
getputicon("savefile.jco",22,32);
getputicon("open.jco",42,32);
getputicon("changebg.jco",62,32);
getputicon("redimens.jco",82,32);
getputicon("help.jco",102,32);
getputicon("about.jco",622,32);
getputicon("exit.jco",622,2);

for (i=0;i<rc;i++)
  for (j=0;j<rc;j++)
    {
    MATR[i][j]=bgcolor;
    putpixel(10+i,90+j,bgcolor);
    }
showbigicon();

settextstyle(2,0,5);
setcolor(14);
outtextxy(30,0,"Icons Editor 3.0 - (c) Gybbon, 1998. - ");

for (i=0;i<16;i++)
  {
  setfillstyle(1,i);
  bar(570,60+i*20,635,78+i*20);
  setcolor(15);
  rectangle(570,60+i*20,635,78+i*20);
  }

setfillstyle(1,col);
bar(5,55,70,73);
setcolor(15);
rectangle(5,55,70,73);
windows(75,55,190,75);
setcolor(0);
settextstyle(11,0,1);
outtextxy(82,62,"Current color");

strcpy(fname,"Untitled");
settextstyle(11,0,1);
setcolor(14);
outtextxy(330,5,fname);
}

void options()
{
char ch[2];
int timerh,timerm,th=0,tm=0;

showmouse();
while (1)
  {
  // clock time.
  struct time t;
  int clock;
  gettime(&t);
  if (th==0)
    {
    timerh=t.ti_hour;
    th=1;
    }
   else
    if (timerh!=t.ti_hour)
      {
      windows(600,464,635,478);
      th=0;
      }
  if (tm==0)
    {
    timerm=t.ti_min;
    tm=1;
    }
   else
    if (timerm!=t.ti_min)
      {
      windows(600,464,635,478);
      tm=0;
      }
  if (clock!=t.ti_min)
    {
    hidemouse();
    windows(600,464,635,478);
    setcolor(0);
    settextstyle(2,0,4);
    sprintf(ch,"%d",t.ti_hour);
    if (t.ti_hour<10)
      {
      outtextxy(603,465,"0");
      outtextxy(610,465,ch);
      }
     else
      outtextxy(603,465,ch);
    outtextxy(616,464,":");
    sprintf(ch,"%d",t.ti_min);
    if (t.ti_min<10)
      {
      outtextxy(620,465,"0");
      outtextxy(627,465,ch);
      }
     else
      outtextxy(620,465,ch);
    showmouse();
    }

  clock=t.ti_min;

  mousexy(sx,sy,m);

  // color selection.
  if ((sx>570) && (sx<635) && (sy>60) && (sy<78+15*20) && (m==1) && ((int)((sy-60)/20)!=col))
    {
    hidemouse();
    col=(int)((sy-60)/20);
    setfillstyle(1,col);
    bar(5,55,70,73);
    setcolor(15);
    rectangle(5,55,70,73);
    showmouse();
    }

  // efective drawing.
  if ((sx>tx) && (sx<tx+rc*k) && (sy>ty) && (sy<ty+rc*k) && (m==1) && (getpixel((int)((sx-tx)/k)+10,(int)((sy-ty)/k)+90)!=col))
    {
    hidemouse();
    setfillstyle(1,col);
    xx=(int)((sx-tx)/k);
    yy=(int)((sy-ty)/k);
    bar(tx+xx*k+1,ty+yy*k+1,tx+(xx+1)*k-1,ty+(yy+1)*k-1);
    MATR[yy][xx]=col;
    putpixel(10+xx,90+yy,MATR[yy][xx]);
    showmouse();
    }

  // new file.
  if ((sx>2) && (sx<18) && (sy>32) && (sy<48))
    {
    if (m==1)
      newfile();
    if (_newfile==0)
      {
      settextstyle(11,0,3);
      setcolor(0);
      outtextxy(30,467,"Create a new file.");
      _newfile=1;
      }
    }

  // new file CLOSE.
  if (((sx<2) || (sx>18) || (sy<32) || (sy>48)) && (_newfile==1))
    {
    setfillstyle(1,7);
    bar(30,462,600,480);
    _newfile=0;
    }

  // saving file.
  if ((sx>22) && (sx<38) && (sy>32) && (sy<48))
    {
    if (m==1)
      savefile();
    if (_savefile==0)
      {
      settextstyle(11,0,3);
      setcolor(0);
      outtextxy(30,467,"Save current file.");
      _savefile=1;
      }
    }

  // save file CLOSE.
  if (((sx<22) || (sx>38) || (sy<32) || (sy>48)) && (_savefile==1))
    {
    setfillstyle(1,7);
    bar(30,462,600,480);
    _savefile=0;
    }

  // opening file.
  if ((sx>42) && (sx<58) && (sy>32) && (sy<48))
    {
    if (m==1)
      openfile();
    if (_openfile==0)
      {
      settextstyle(11,0,3);
      setcolor(0);
      outtextxy(30,467,"Open a file.");
      _openfile=1;
      }
    }

  // open file CLOSE.
  if (((sx<42) || (sx>58) || (sy<32) || (sy>48)) && (_openfile==1))
    {
    setfillstyle(1,7);
    bar(30,462,600,480);
    _openfile=0;
    }

  // change background color.
  if ((sx>62) && (sx<78) && (sy>32) && (sy<48))
    {
    if (m==1)
      changebgc();
    if (_changebg==0)
      {
      settextstyle(11,0,3);
      setcolor(0);
      outtextxy(30,467,"Change background's color.");
      _changebg=1;
      }
    }

  // change background's color CLOSE.
  if (((sx<62) || (sx>78) || (sy<32) || (sy>48)) && (_changebg==1))
    {
    setfillstyle(1,7);
    bar(30,462,600,480);
    _changebg=0;
    }

  // change icon's dimensions.
  if ((sx>82) && (sx<98) && (sy>32) && (sy<48))
    {
    if (m==1)
      changeicdim();
    if (_redimens==0)
      {
      settextstyle(11,0,3);
      setcolor(0);
      outtextxy(30,467,"Change icon's dimensions.");
      _redimens=1;
      }
    }

  // change icon's dimensions CLOSE.
  if (((sx<82) || (sx>98) || (sy<32) || (sy>48)) && (_redimens==1))
    {
    setfillstyle(1,7);
    bar(30,462,600,480);
    _redimens=0;
    }

  // about this program.
  if ((sx>622) && (sx<638) && (sy>32) && (sy<48))
    {
    if (m==1)
      about();
    if (_about==0)
      {
      settextstyle(11,0,3);
      setcolor(0);
      outtextxy(30,467,"About this program.");
      _about=1;
      }
    }

  // about CLOSE.
  if (((sx<622) || (sx>638) || (sy<32) || (sy>48)) && (_about==1))
    {
    setfillstyle(1,7);
    bar(30,462,600,480);
    _about=0;
    }

  // help CLOSE.
  if (((sx<102) || (sx>118) || (sy<32) || (sy>48)) && (_help==1))
    {
    setfillstyle(1,7);
    bar(30,462,600,480);
    _help=0;
    }

  // help option.
  if ((sx>102) && (sx<118) && (sy>32) && (sy<48))
    {
    if (m==1)
      {
      hidemouse();
      save_screen(100,150,540,300);
      windows(100,150,540,300);
      windows(103,153,537,176);
      getputicon("\\editicon\\icons\\exit.jco",107,157);
      getputicon("\\editicon\\icons\\uparrow.jco",498,157);
      getputicon("\\editicon\\icons\\dnarrow.jco",518,157);
      settextstyle(2,0,6);
      setcolor(14);
      outtextxy(175,154,"Icons Editor 3.0. Help Option.");
      windows(105,180,535,295);
      showmouse();
      help(1);
      }
    if (_help==0)
      {
      settextstyle(11,0,3);
      setcolor(0);
      outtextxy(30,467,"Help option.");
      _help=1;
      }
    }

  // exit program.
  if ((sx>600) && (sx<640) && (sy>0) && (sy<18) && (m==1))
    {
    if (child==1)
      {
      hidemouse();
      exit(0);
      }
    hidemouse();
    closegraph();
    chdir("\\editicon");
    exit(EXIT_SUCCESS);
    }
  if (bioskey(1)!=0)
    switch (toupper(bioskey(0)))
      {
      case 27: if (child==1)
		 {
		 hidemouse();
		 exit(0);
		 }
	       hidemouse();
	       closegraph();
	       chdir("\\editicon");
	       exit(EXIT_SUCCESS);
      }
  }
}

void changebgc()
{
getbgordim(1,"Choose the background color from right side.","ATTENTION: Current icon will be erased!");
for (i=0;i<rc;i++)
  for (j=0;j<rc;j++)
    MATR[i][j]=bgcolor;
showbigicon();
showsmallicon();
options();
}

void getbgordim(int no,char *text1,char *text2)
{
hidemouse();
save_screen(460-(460-180)/2-((strlen(text1)<strlen(text2)?strlen(text2):strlen(text1))/2)*8-10,190,460-(460-180)/2-((strlen(text1)<strlen(text2)?strlen(text2):strlen(text1))/2)*8+(strlen(text1)<strlen(text2)?strlen(text2):strlen(text1))*8+10,290);
windows(460-(460-180)/2-((strlen(text1)<strlen(text2)?strlen(text2):strlen(text1))/2)*8-10,190,460-(460-180)/2-((strlen(text1)<strlen(text2)?strlen(text2):strlen(text1))/2)*8+(strlen(text1)<strlen(text2)?strlen(text2):strlen(text1))*8+10,290);
getputicon("\\editicon\\icons\\question.jco",310,200);
setcolor(0);
settextstyle(11,0,1);
outtextxy(460-(460-180)/2-(strlen(text1)/2)*8,230,text1);
outtextxy(460-(460-180)/2-(strlen(text2)/2)*8,245,text2);
getputicon("\\editicon\\icons\\cancel.jco",303,255);
showmouse();
if (no==1)
  {
  hidemouse();
  setlinestyle(1,0,3);
  setcolor(15);
  rectangle(566,56,639,78+15*20+4);
  setlinestyle(0,0,1);
  showmouse();
  while (1)
    {
    mousexy(sx,sy,m);
    if ((sx>303) && (sx<303+33) && (sy>260) && (sy<260+23) && (m==1))
      {
      hidemouse();
      restore_screen(460-(460-180)/2-((strlen(text1)<strlen(text2)?strlen(text2):strlen(text1))/2)*8-10,190,460-(460-180)/2-((strlen(text1)<strlen(text2)?strlen(text2):strlen(text1))/2)*8+(strlen(text1)<strlen(text2)?strlen(text2):strlen(text1))*8+10,290);
      setlinestyle(1,0,3);
      setcolor(8);
      rectangle(566,56,639,78+15*20+4);
      setlinestyle(0,0,1);
      options();
      }
    if ((sx>570) && (sx<635) && (sy>60) && (sy<78+15*20) && (m==1))
      {
      hidemouse();
      bgcolor=(int)((sy-60)/20);
      restore_screen(460-(460-180)/2-((strlen(text1)<strlen(text2)?strlen(text2):strlen(text1))/2)*8-10,190,460-(460-180)/2-((strlen(text1)<strlen(text2)?strlen(text2):strlen(text1))/2)*8+(strlen(text1)<strlen(text2)?strlen(text2):strlen(text1))*8+10,290);
      setlinestyle(1,0,3);
      setcolor(8);
      rectangle(566,56,639,78+15*20+4);
      setlinestyle(0,0,1);
      return;
      }
    }
  }
if (no==2)
  {
  hidemouse();
  gotoxy(38,16);
  printf("  %d  ",rc);
  getputicon("\\editicon\\icons\\ok.jco",390,250);
  getputicon("\\editicon\\icons\\uparrow.jco",278,240);
  getputicon("\\editicon\\icons\\dnarrow.jco",346,240);
  showmouse();
  while (1)
    {
    mousexy(sx,sy,m);
    if ((sx>303) && (sx<303+33) && (sy>260) && (sy<260+23) && (m==1))
      {
      hidemouse();
      restore_screen(460-(460-180)/2-((strlen(text1)<strlen(text2)?strlen(text2):strlen(text1))/2)*8-10,190,460-(460-180)/2-((strlen(text1)<strlen(text2)?strlen(text2):strlen(text1))/2)*8+(strlen(text1)<strlen(text2)?strlen(text2):strlen(text1))*8+10,290);
      rc=redimensrc;
      options();
      }
    if ((sx>278) && (sx<278+16) && (sy>240) && (sy<240+16) && (m==1) && (rc<50))
      {
      rc++;
      gotoxy(40,16);
      printf("%d",rc);
      delay(150);
      }
    if ((sx>346) && (sx<346+16) && (sy>240) && (sy<240+16) && (m==1) && (rc>8))
      {
      rc--;
      gotoxy(40,16);
      printf("%d",rc);
      delay(150);
      }
    if ((sx>390) && (sx<390+24) && (sy>250) && (sy<250+24) && (m==1))
      return;
    }
  }
}

void changeicdim()
{
redimensrc=rc;
getbgordim(2,"Choose the new size of icon."," ");
hidemouse();
for (i=0;i<rc;i++)
  for (j=0;j<rc;j++)
    MATR[i][j]=bgcolor;
setfillstyle(1,8);
bar(0,90,500,460);
showbigicon();
showsmallicon();
options();
}

void newfile()
{
if (strcmpi(fname,"untitled")==1)
  {
  hidemouse();
  for (i=0;i<rc;i++)
    for (j=0;j<rc;j++)
      MATR[i][j]=bgcolor;
  showbigicon();
  showsmallicon();
  setfillstyle(1,1);
  bar(330,5,600,18);
  strcpy(fname,"Untitled");
  settextstyle(11,0,1);
  setcolor(14);
  outtextxy(330,5,fname);
  options();
  }
 else
  areyousure();
}

void areyousure()
{
hidemouse();
save_screen(220,190,420,290);
windows(220,190,420,290);
getputicon("\\editicon\\icons\\exclamat.jco",310,200);
setcolor(0);
settextstyle(11,0,1);
outtextxy(230,230,"This icon is not saved.");
outtextxy(225,240,"Do you want to continue?");
getputicon("\\editicon\\icons\\continue.jco",240,255);
getputicon("\\editicon\\icons\\cancel.jco",367,255);
showmouse();
while (1)
  {
  mousexy(sx,sy,m);
  if ((sx>240) && (sx<240+33) && (sy>260) && (sy<260+23) && (m==1))
    {
    hidemouse();
    restore_screen(220,190,420,290);
    rc=16;
    for (i=0;i<rc;i++)
      for (j=0;j<rc;j++)
	MATR[i][j]=bgcolor;
    setfillstyle(1,8);
    bar(0,90,500,460);
    showbigicon();
    showsmallicon();
    setfillstyle(1,1);
    bar(330,5,600,18);
    strcpy(fname,"Untitled");
    settextstyle(11,0,1);
    setcolor(14);
    outtextxy(330,5,fname);
    options();
    }
  if ((sx>367) && (sx<367+33) && (sy>260) && (sy<260+23) && (m==1))
    {
    hidemouse();
    restore_screen(220,190,420,290);
    options();
    }
  }
}

void help(int page)
{
char helppage[2];
settextstyle(11,0,4);
setcolor(0);
if (page==1)
  {
  hidemouse();
  getputicon("\\editicon\\icons\\_abicon.jco",115,190);
  outtextxy(230,200,"programed by Nelu,1998");
  outtextxy(190,220,"==================================");
  outtextxy(190,230,"=     [city removed],ROMANIA,    =");
  outtextxy(190,240,"=       [address removed]        =");
  outtextxy(190,250,"=       cod [code removed]       =");
  outtextxy(190,260,"=     Tel.: [phone removed]      =");
  outtextxy(190,270,"==================================");
  showmouse();
  }
if (page==2)
  {
  hidemouse();
  getputicon("\\editicon\\icons\\open.jco",115,190);
  outtextxy(290,200,"=OPEN=");
  outtextxy(125,230,"      To open a file from the hard disk,press the");
  outtextxy(125,240,"mouse button in OPEN window under menu,than enter");
  outtextxy(125,250,"the name of the file.");
  showmouse();
  }
if (page==3)
  {
  hidemouse();
  getputicon("\\editicon\\icons\\savefile.jco",115,190);
  outtextxy(290,200,"=SAVE=");
  outtextxy(125,230,"      To save the icon on hard disk or on floppy,");
  outtextxy(125,240,"press the mouse button in SAVE window under menu.");
  outtextxy(125,250,"Than enter the name of the file.");
  showmouse();
  }
if (page==4)
  {
  hidemouse();
  getputicon("\\editicon\\icons\\newfile.jco",115,190);
  outtextxy(295,200,"=NEW=");
  outtextxy(125,230,"      To create a new icon,press the mouse button");
  outtextxy(125,240,"in NEW window under menu.");
  showmouse();
  }
if (page==5)
  {
  hidemouse();
  getputicon("\\editicon\\icons\\changebg.jco",115,190);
  outtextxy(230,200,"=CHANGE ICON'S BACKGROUND=");
  outtextxy(125,230,"      To change the background of the icon, press");
  outtextxy(125,240,"the mouse button in CHANGE window under the menu.");
  showmouse();
  }
if (page==6)
  {
  hidemouse();
  getputicon("\\editicon\\icons\\redimens.jco",115,190);
  outtextxy(270,200,"=REDIMENSION=");
  outtextxy(125,230,"      To change icon's dimensions,press the mouse");
  outtextxy(125,240,"button in REDIMENSION window under the menu.");
  showmouse();
  }
if (page==7)
  {
  hidemouse();
  getputicon("\\editicon\\icons\\help.jco",115,190);
  outtextxy(290,200,"=HELP=");
  outtextxy(125,230,"      To get help about using this program, press");
  outtextxy(125,240,"the mouse button on HELP window under the menu.");
  showmouse();
  }
if (page==8)
  {
  hidemouse();
  getputicon("\\editicon\\icons\\about.jco",115,190);
  outtextxy(285,200,"=ABOUT=");
  outtextxy(125,230,"      To get some  informations about the  author");
  outtextxy(125,240,"of this program, press the mouse button on  ABOUT");
  outtextxy(125,250,"window under the menu, right side.");
  showmouse();
  }
if (page==9)
  {
  hidemouse();
  getputicon("\\editicon\\icons\\chcolor.jco",115,190);
  outtextxy(260,200,"=CHANGE COLOR=");
  outtextxy(125,230,"      To change color for drawing inside the icon");
  outtextxy(125,240,"press the mouse button in one of the colored rec-");
  outtextxy(125,250,"tangles from right side of the screen.");
  showmouse();
  }
if (page==10)
  {
  hidemouse();
  getputicon("\\editicon\\icons\\exit.jco",115,190);
  outtextxy(290,200,"=EXIT=");
  outtextxy(125,230,"         To exit this program, press");
  outtextxy(125,240,"the mouse button in EXIT window from up-right.");
  outtextxy(125,255,"       Don't you forget to say 'Good Bye' !");
  showmouse();
  }
sprintf(helppage,"%d",page);
setcolor(8);
outtextxy(115,280,"Page ");
outtextxy(150,280,helppage);
delay(100);
while(1)
  {
  mousexy(sx,sy,m);
  if ((sx>107) && (sx<107+16) && (sy>157) && (sy<157+16))
    if (m==1)
      {
      hidemouse();
      restore_screen(100,150,540,300);
      showmouse();
      delay(200);
      options();
      }
  if ((sx>498) && (sx<498+16) && (sy>157) && (sy<157+16) && (m==1))
    if (page<10)
      {
      windows(105,180,535,295);
      page++;
      help(page);
      }
  if ((sx>518) && (sx<518+16) && (sy>157) && (sy<157+16) && (m==1))
    if (page>1)
      {
      windows(105,180,535,295);
      page--;
      help(page);
      }
  }
}

void opensavewindow()
{
getcwd(s,_MAX_PATH);
hidemouse();
save_screen(180,170,460,350);
windows(180,170,460,350);
windows(183,173,457,196);
getputicon("\\editicon\\icons\\exit.jco",438,177);
setcolor(0);
settextstyle(11,0,1);
if (opensave==1)
  outtextxy(190,182,"Save file.");
if (opensave==2)
  outtextxy(190,182,"Open file.");
windows(190,200,370,290);
windows(344,203,367,287);
getputicon("\\editicon\\icons\\uparrow.jco",348,207);
getputicon("\\editicon\\icons\\dnarrow.jco",348,268);
getputicon("\\editicon\\icons\\backdir.jco",190,297);
getputicon("\\editicon\\icons\\chdrive.jco",430,275);
setcolor(0);
outtextxy(380,280,"Drive:");
if (preview==0)
  {
  getputicon("\\editicon\\icons\\combo1i.jco",375,200);
  setcolor(8);
  outtextxy(395,205,"Preview");
  }
 else
  {
  getputicon("\\editicon\\icons\\combo1a.jco",375,200);
  setcolor(0);
  outtextxy(395,205,"Preview");
  }
if (opensave==1)
  {
  gotoxy(25,21);
  printf("                     ");
  }
setcolor(15);
rectangle(191,319,360,336);
getputicon("\\editicon\\icons\\continue.jco",380,315);
getputicon("\\editicon\\icons\\cancel.jco",420,315);
showmouse();
choosefiles();
}

void choosefiles()
{
//int curwidth;
char temp[2];
struct ffblk ffblk;
int done,fcur=0,curent=1,tot=0;
chdir(s);
hidemouse();
for (i=0;i<10;i++)
  for (j=0;j<15;j++)
    f[i][j]=' ';
setfillstyle(1,7);
bar(195,204,340,285);
bar(210,302,455,312);
if (strlen(s)<=30)
  {
  setcolor(8);
  outtextxy(210,302,s);
  }
 else
  {
  setcolor(4);
  outtextxy(210,302,"WARNING: Press BACKDIR button.");
  }
settextstyle(11,0,1);
setcolor(14);
done = findfirst("*.*",&ffblk,FA_DIREC);
while ((!done) && (tot!=299))
  {
  strcpy(f[tot],ffblk.ff_name);
  done = findnext(&ffblk);
  tot++;
  }
for (i=0;i<tot;i++)
  if ((f[i][strlen(f[i])-4]=='.') || (f[i][strlen(f[i])-3]=='.') || (f[i][strlen(f[i])-2]=='.'))
    for (j=0;j<strlen(f[i]);j++)
      f[i][j]=(char)(tolower((int)(f[i][j])));
//curwidth=(int)((267-224)/tot)+1;
//setfillstyle(1,8);
//bar(347,224,364,224+curwidth-1);
//setfillstyle(1,7);
outtextxy(220,213,f[1]);
setcolor(0);
for (fcur=2;fcur<8;fcur++)
  outtextxy(220,213+(fcur-1)*10,f[fcur]);
showmouse();
fcur=0;
while (1)
  {
  mousexy(sx,sy,m);
//  if ((sx>430) && (sx<430+16) && (sy>275) && (sy<275+16) && (m==1))
//    {
//    hidemouse();
//    windows(320-200,365,320+207,386);
//    gotoxy(17,24);
//    printf(" ");
//    gotoxy(18,24);
//    for (i=1;i<25;i++)
//      {
//      if (_chdrive(i)==0)
//	{
//	printf("%c ",i+'A'-1);
//	}
//       else
//	{
//	printf("%c ",i+'A'-1);
//	}
//      }
//    showmouse();
//    while(1)
//      {
//      mousexy(sx,sy,m);
//      if ((sx>320-200+5) && (sx<320+207-5) && (sy>367) && (sy<384) && (m==1))
//	{
//	gotoxy(1,1);
//	printf("%d",(sx-320+200-5)/20);
//	}
//      if (bioskey(1)!=0)
//	switch (toupper(bioskey(0)))
//	  {
//	  case 27: setfillstyle(1,8);
//		   bar(320-200,365,320+207,386);
//		   choosefiles();
//	  }
//      }
//    delay(150);
//    }
  if ((sx>375) && (sx<450) && (sy>200) && (sy<200+16) && (m==1))
    {
    if (preview==0)
      {
      hidemouse();
      getputicon("\\editicon\\icons\\combo1a.jco",375,200);
      setcolor(0);
      outtextxy(395,205,"Preview");
      preview=1;
      if ((strcmp(f[curent],"..")) && (f[curent][strlen(f[curent])-3]=='j') && (f[curent][strlen(f[curent])-2]=='c') && (f[curent][strlen(f[curent])-1]=='o'))
	{
	bar(390,220,440,270);
	getputicon(f[curent],390,220);
	}
       else
	bar(390,220,440,270);
      showmouse();
      }
     else
      {
      hidemouse();
      getputicon("\\editicon\\icons\\combo1i.jco",375,200);
      setcolor(8);
      outtextxy(395,205,"Preview");
      preview=0;
      bar(390,220,440,270);
      showmouse();
      }
     delay(150);
     }
  if ((sx>190) && (sx<190+16) && (sy>297) && (sy<297+16) && (m==1))
    {
//    bar(347,224,364,267);
    bar(390,220,440,270);
    if (strlen(s)>3)
      for (i=strlen(s);i>0;i--)
	{
	if (s[i]=='\\')
	  {
	  for (j=i;j<strlen(s);j++)
	    s[j]='\0';
	  break;
	  }
	}
    if (strlen(s)==2)
      strcat(s,"\\");
    chdir(s);
    hidemouse();
    setfillstyle(1,7);
    bar(195,205,340,285);
    showmouse();
    delay(150);
    choosefiles();
    }
  if ((sx>191) && (sx<360) && (sy>319) && (sy<336) && (opensave==1) && (m==1))
    {
    GetFileName(".jco",fname);
    hidemouse();
    db=fopen(fname,"w+a");
    if (!db)
      {
      restore_screen(180,170,460,350);
      options();
      }
    rewind(db);
    fprintf(db,"%d\n",rc);
    fwrite(&MATR,sizeof(MATR),1,db);
    fclose(db);
    setfillstyle(1,1);
    bar(330,5,600,18);
    settextstyle(11,0,1);
    setcolor(14);
    outtextxy(330,5,fname);
    options();
    }
  if ((sx>420) && (sx<420+33) && (sy>320) && (sy<315+33-5) && (m==1))
    {
    hidemouse();
    restore_screen(180,170,460,350);
    delay(150);
    options();
    }
  if ((sx>380) && (sx<380+33) && (sy>320) && (sy<315+33-5) && (m==1))
    {
    if ((opensave==1) && (strcmp(f[curent],"..")))
      {
      hidemouse();
      db=fopen(f[curent],"w+a");
      if (!db)
	{
	restore_screen(180,170,460,350);
	options();
	}
      rewind(db);
      fprintf(db,"%d\n",rc);
      fwrite(&MATR,sizeof(MATR),1,db);
      fclose(db);
      restore_screen(180,170,460,350);
      setfillstyle(1,1);
      bar(330,5,600,18);
      settextstyle(11,0,1);
      setcolor(14);
      outtextxy(330,5,f[curent]);
      options();
      }
    if ((opensave==2) && (strcmp(f[curent],"..")))
      {
      hidemouse();
      db=fopen(f[curent],"r+a");
      if (!db)
	{
	restore_screen(180,170,460,350);
	options();
	}
      rewind(db);
      fscanf(db,"%d\n",&rc);
      fread(MATR,sizeof(MATR),1,db);
      fclose(db);
      restore_screen(180,170,460,350);
      setfillstyle(1,8);
      bar(0,90,500,460);
      showbigicon();
      showsmallicon();
      setfillstyle(1,1);
      bar(330,5,600,18);
      settextstyle(11,0,1);
      setcolor(14);
      outtextxy(330,5,f[curent]);
      delay(150);
      options();
      }
    }
// double click.
  if ((sx>200) && (sx<335) && (sy>213) && (sy<283) && (m==1) && ((int)((sy-213)/10)!=fcur))
    {
    if (fcur<(int)((sy-213)/10))
      curent=curent+((int)((sy-213)/10)-fcur);
     else
      curent=curent-(fcur-(int)((sy-213)/10));
     fcur=(int)((sy-213)/10);
    hidemouse();
    setfillstyle(1,7);
    bar(195,205,340,285);
    for (i=0;i<fcur;i++)
      {
      setcolor(0);
      outtextxy(220,213+i*10,f[curent-fcur+i]);
      }
    setcolor(14);
    outtextxy(220,213+(fcur-1)*10+10,f[curent]);
    for (i=fcur+1;i<7;i++)
      {
      setcolor(0);
      outtextxy(220,213+i*10,f[curent+i-fcur]);
      }
    if ((strcmp(f[curent],"..")) && (f[curent][strlen(f[curent])-3]=='j') && (f[curent][strlen(f[curent])-2]=='c') && (f[curent][strlen(f[curent])-1]=='o') && (preview==1))
      {
      bar(390,220,440,270);
      getputicon(f[curent],390,220);
      }
     else
      bar(390,220,440,270);
    showmouse();
    delay(150);
    mousexy(sx,sy,m);
    if ((m==1) && (f[curent][strlen(f[curent])-4]=='.') || (f[curent][strlen(f[curent])-3]=='.') || (f[curent][strlen(f[curent])-2]=='.'))
      {
      if ((opensave==1) && (strcmp(f[curent],"..")))
	{
	hidemouse();
	db=fopen(f[curent],"w+a");
	if (!db)
	  {
	  restore_screen(180,170,460,350);
	  options();
	  }
	rewind(db);
	fprintf(db,"%d\n",rc);
	fwrite(&MATR,sizeof(MATR),1,db);
	fclose(db);
	restore_screen(180,170,460,350);
	setfillstyle(1,1);
	bar(330,5,600,18);
	settextstyle(11,0,1);
	setcolor(14);
	outtextxy(330,5,f[curent]);
	options();
	}
      if ((opensave==2) && (strcmp(f[curent],"..")))
	{
	hidemouse();
	db=fopen(f[curent],"r+a");
	if (!db)
	  {
	  restore_screen(180,170,460,350);
	  options();
	  }
	rewind(db);
	fscanf(db,"%d\n",&rc);
	fread(MATR,sizeof(MATR),1,db);
	fclose(db);
	restore_screen(180,170,460,350);
	setfillstyle(1,8);
	bar(0,90,500,460);
	showbigicon();
	showsmallicon();
	setfillstyle(1,1);
	bar(330,5,600,18);
	settextstyle(11,0,1);
	setcolor(14);
	outtextxy(330,5,f[curent]);
	delay(150);
	options();
	}
      }
    }
//  gotoxy(1,1);
//  printf("%d  ",(int)(f[curent][0]));
  if ((sx>200) && (sx<335) && (sy>213) && (sy<283) && (m==1) && ((int)(f[curent][0])<91) && ((int)(f[curent][0])>47))
    {
//    delay(150);
//    mousexy(sx,sy,m);
//    if (m==1)
//      {
      if (strlen(s)>3)
	strcat(s,"\\");
      strcat(s,f[curent]);
      delay(150);
      choosefiles();
//      }
    }
  if ((sx>200) && (sx<335) && (sy>213) && (sy<283) && (m==1) && ((int)((sy-213)/10)==fcur))
    {
    showmouse();
    delay(95);
    mousexy(sx,sy,m);
    if ((m==1) && (f[curent][strlen(f[curent])-4]=='.') || (f[curent][strlen(f[curent])-3]=='.') || (f[curent][strlen(f[curent])-2]=='.'))
      {
      if ((opensave==1) && (strcmp(f[curent],"..")))
	{
	hidemouse();
	db=fopen(f[curent],"w+a");
	if (!db)
	  {
	  restore_screen(180,170,460,350);
	  options();
	  }
	rewind(db);
	fprintf(db,"%d\n",rc);
	fwrite(&MATR,sizeof(MATR),1,db);
	fclose(db);
	restore_screen(180,170,460,350);
	setfillstyle(1,1);
	bar(330,5,600,18);
	settextstyle(11,0,1);
	setcolor(14);
	outtextxy(330,5,f[curent]);
	options();
	}
      if ((opensave==2) && (strcmp(f[curent],"..")))
	{
	hidemouse();
	db=fopen(f[curent],"r+a");
	if (!db)
	  {
	  restore_screen(180,170,460,350);
	  options();
	  }
	rewind(db);
	fscanf(db,"%d\n",&rc);
	fread(MATR,sizeof(MATR),1,db);
	fclose(db);
	restore_screen(180,170,460,350);
	setfillstyle(1,8);
	bar(0,90,500,460);
	showbigicon();
	showsmallicon();
	setfillstyle(1,1);
	bar(330,5,600,18);
	settextstyle(11,0,1);
	setcolor(14);
	outtextxy(330,5,f[curent]);
	delay(150);
	options();
	}
      }
    }
  if ((sx>348) && (sx<348+16) && (sy>268) && (sy<268+16) && (m==1) && (curent<tot-1))
    {
    curent++;
    if (fcur<6)
      fcur++;
    hidemouse();
//    setfillstyle(1,7);
//    bar(347,224+(curent-2)*curwidth-1,364,224+(curent-2)*curwidth+curwidth-1);
//    setfillstyle(1,8);
//    bar(347,224+(curent-1)*curwidth-1,364,224+(curent-1)*curwidth+curwidth-1);
    setfillstyle(1,7);
    bar(195,205,340,285);
    for (i=0;i<fcur;i++)
      {
      setcolor(0);
      outtextxy(220,213+i*10,f[curent-fcur+i]);
      }
    setcolor(14);
    outtextxy(220,213+(fcur-1)*10+10,f[curent]);
    for (i=fcur+1;i<7;i++)
      {
      setcolor(0);
      outtextxy(220,213+i*10,f[curent+i-fcur]);
      }
    if ((strcmp(f[curent],"..")) && (f[curent][strlen(f[curent])-3]=='j') && (f[curent][strlen(f[curent])-2]=='c') && (f[curent][strlen(f[curent])-1]=='o') && (preview==1))
      {
      bar(390,220,440,270);
      getputicon(f[curent],390,220);
      }
     else
      bar(390,220,440,270);
    delay(150);
    showmouse();
    }
  if ((sx>348) && (sx<348+16) && (sy>207) && (sy<207+16) && (m==1))
    {
    if (curent>1)
      {
      curent--;
      if (fcur!=0)
	fcur--;
      hidemouse();
      setfillstyle(1,7);
      bar(195,205,340,285);
      for (i=-1;i<fcur-1;i++)
	{
	setcolor(0);
	outtextxy(220,213+(i+1)*10,f[curent-fcur+i+1]);
	}
      setcolor(14);
      outtextxy(220,213+(fcur-1)*10+10,f[curent]);
      for (i=fcur;i<6;i++)
	{
	setcolor(0);
	outtextxy(220,213+i*10+10,f[curent+i-fcur+1]);
	}
    if ((strcmp(f[curent],"..")) && (f[curent][strlen(f[curent])-3]=='j') && (f[curent][strlen(f[curent])-2]=='c') && (f[curent][strlen(f[curent])-1]=='o') && (preview==1))
	{
	bar(390,220,440,270);
	getputicon(f[curent],390,220);
	}
       else
	bar(390,220,440,270);
      }
    delay(150);
    showmouse();
    }
  if ((sx>438) && (sx<438+16) && (sy>177) && (sy<177+16) && (m==1))
    {
    hidemouse();
    restore_screen(180,170,460,350);
//    endoffile=0;
    delay(150);
    options();
    }
  }
}

void savefile()
{
opensave=1;
opensavewindow();
}

void openfile()
{
opensave=2;
opensavewindow();
}

void about()
{
hidemouse();
save_screen(220,190,420,290);
windows(220,190,420,290);
getputicon("\\editicon\\icons\\icon1.jco",235,200);
settextstyle(11,0,1);
setcolor(0);
outtextxy(260,205,"ICONS EDITOR 3.0");
settextstyle(2,0,4);
setcolor(8);
outtextxy(230,220,"   This program was made in");
outtextxy(230,230,"        BorlandC++ 3.1");
outtextxy(230,240,"           by Nelu.");
setcolor(15);
outtextxy(229,219,"   This program was made in");
outtextxy(229,229,"        BorlandC++ 3.1");
outtextxy(229,239,"           by Nelu.");
setcolor(1);
outtextxy(230,250," For other informations,call to");
setcolor(14);
outtextxy(230,260,"        [phone removed]");
setcolor(0);
outtextxy(230,270,"         [city removed]");
showmouse();
while (1)
  {
  mousexy(sx,sy,m);
  if (((sx<622) || (sx>638) || (sy<32) || (sy>48)) && (m==1))
    {
    hidemouse();
    restore_screen(220,190,420,290);
    options();
    }
  }
}

void showbigicon()
{
int ii,ij;
setcolor(15);
for (ii=0;ii<=rc;ii++)
  for (ij=0;ij<=rc;ij++)
    {
    line(tx,ty+ij*k,tx+rc*k,ty+ij*k);
    line(tx+ii*k,ty,tx+ii*k,ty+rc*k);
    }
for (ii=0;ii<rc;ii++)
  for (ij=0;ij<rc;ij++)
    {
    setfillstyle(1,MATR[ij][ii]);
    bar(tx+ii*k+1,ty+ij*k+1,tx+(ii+1)*k-1,ty+(ij+1)*k-1);
    }
}

void showsmallicon()
{
int ii,ij;
for (ii=0;ii<rc;ii++)
  for (ij=0;ij<rc;ij++)
    putpixel(10+ii,90+ij,MATR[ij][ii]);
}

void getputicon(char *n,int ix,int iy)
{
int ii,ij;
int getrc;
int matrice[50][50];
db=fopen(n,"r+a");
if (!db)
  return;
rewind(db);
fscanf(db,"%d\n",&getrc);
fread(matrice,sizeof(matrice),1,db);
fclose(db);
for (ii=0;ii<getrc;ii++)
  for (ij=0;ij<getrc;ij++)
    putpixel(ix+ii,iy+ij,matrice[ij][ii]);
}


// USEFUL SUBRUTINES...

void save_screen(int xx1,int yy1,int xx2,int yy2)
{
int ssi,ssj;
fp=fopen("\\editicon\\temp.gph","w+");
if (!fp)
  return;
rewind(fp);
hidemouse();
for (ssi=yy1;ssi<=yy2;ssi++)
  for (ssj=xx1;ssj<=xx2;ssj++)
    fprintf(fp,"%d ",getpixel(ssj,ssi));
fclose(fp);
showmouse();
}

void restore_screen(int xx1,int yy1,int xx2,int yy2)
{
int ssi,ssj,point;
fp=fopen("\\editicon\\temp.gph","r");
if (!fp)
  return;
rewind(fp);
hidemouse();
for (ssi=yy1;ssi<=yy2;ssi++)
  for (ssj=xx1;ssj<=xx2;ssj++)
    {
    fscanf(fp,"%d",&point);
    putpixel(ssj,ssi,point);
    }
fclose(fp);
showmouse();
}

void initmouse()
{
REGS regs;
regs.x.ax =0x0000;
int86(0x33, &regs, &regs);
}

void showmouse()
{
REGS regs;
regs.x.ax =0x0001;
int86(0x33, &regs, &regs);
}

void hidemouse()
{
REGS regs;
regs.x.ax =0x0002;  /* set cursor position */
int86(0x33, &regs, &regs);
}

void mousexy(int &sx,int &sy,int &m)
{
REGS regs;
regs.x.ax =0x0003;  /* set cursor position */
int86(0x33, &regs, &regs);
m=regs.x.bx;
sx=regs.x.cx;
sy=regs.x.dx;
}

void windows(int x1,int y1,int x2,int y2)
{
int weight;
setcolor(7);
setfillstyle(1,7);
bar(x1,y1,x2,y2);
setcolor(8);
rectangle(x1,y1,x2,y2);
weight=2;
moveto(x1+1,y2-1);
lineto(x1+weight,y2-weight);
lineto(x2-weight,y2-weight);
lineto(x2-weight,y1+weight);
lineto(x2-1,y1+1);
setfillstyle(1,8);
floodfill(x2-2,y2-2,8);
setcolor(15);
moveto(x2-1,y1+1);
lineto(x2-weight,y1+weight);
lineto(x1+weight,y1+weight);
lineto(x1+weight,y2-weight);
lineto(x1+1,y2-1);
lineto(x1+1,y1+1);
lineto(x2-1,y1+1);
setfillstyle(1,15);
floodfill(x1+2,y1+2,15);
}

void separator(int sx1,int sy1,int sx2,int col1,int col2)
{
setcolor(col1);
line(sx1,sy1,sx2,sy1);
line(sx1,sy1+1,sx2,sy1+1);
setcolor(col2);
line(sx1,sy1+2,sx2,sy1+2);
//line(sx1,sy1+3,sx2,sy1+3);
}

void GetFileName(const char *ext,char *fs)
{
char *cp;
//save_screen(200,130,440,200);
//windows(200,130,440,200);
//setcolor(8);
//outtextxy(275,140,"File to proceed:");
hidemouse();
gotoxy(26,21);
//printf("                      ");
//gotoxy(30,11);
fgets(fs,81,stdin);
fflush(stdin);
cp=strchr(fs,'\n');
if (cp)
  *cp='\0';
if (strlen(fs)==0)
  {
  restore_screen(180,170,460,350);
  return;
  }
cp=strchr(fs,'.');
if (!cp && strlen(fs)+strlen(ext)<81)
  strcat(fs,ext);
restore_screen(180,170,460,350);
showmouse();
}

int FileExists(const char *path)
{
fp=fopen(path,"rb");
int result=(fp!=NULL);
if (fp)
  fclose(fp);
return result;
}