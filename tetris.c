/// @file tetris.c
/// 
/// @author Van Yu
/// @version 0.1
/// @date 2014-06-06

#include "nesutil.h"

extern u8 nt_col;
extern u8 sp_col;

#define HSIZE 11
#define VSIZE 22

#define BLOCK_START_X 6
#define BLOCK_START_Y 1

#define FRAME_START_X 5
#define FRAME_START_Y 3

#define BLOCK_TILE_INDEX 0xC0
#define NOTHING_INDEX    0x00
#define FRAME_TILE_INDEX 0xD4                          

#define LOOP_TIMES_FOR_SLEEP 600
#define TIME_DROP_DOWN 10

#define unitindex(x, y) (y * HSIZE + x)

#define write_unit(x, y) do { \
  gotoxy(FRAME_START_X + x + 1, FRAME_START_Y + y); \
  cputc(BLOCK_TILE_INDEX); \
} while (0)

#define clear_unit(x, y) do { \
  putc_xy(VRAM_name_0, FRAME_START_X + x + 1, FRAME_START_Y + y + 1, NOTHING_INDEX); \
} while (0)

#define write_frame(x, y) do { \
  putc_xy(VRAM_name_0, FRAME_START_X + x, FRAME_START_Y + y, FRAME_TILE_INDEX); \
} while (0)


//--------------------------------------------------------------------------------------- 

struct point {
  signed char x;
  signed char y;
};

static struct point pointBuffer;

void getRotatedPoint (struct point * p)
{
  pointBuffer.x = -p->y;
  pointBuffer.y = p->x;
}

struct block {
  u8 id;
  u8 color;
  struct point points[4];
};

static struct block tetrisPool [7];
// cc65 sucks, does not support such kind of initialization
/*
 = {
  {
    0, 0x00, { 
      { -1,0 },
      { 0,0 },
      { 1,0 },
      { 2,0 }
    }
  },

  {
    1, 0x01, { 
      { -1,0 },
      { 0,0 },
      { 0,1 },
      { 1,1 }
    }
  },

  {
    2, 0x02, { 
      { 0,-1 },
      { 0,0 },
      { 0,1 },
      { 1,1 }
    }
  },

  {
    3, 0x03, { 
      { 1,-1 },
      { 1,0 },
      { 1,1 },
      { 0,1 }
    }
  },
  {
    4, 0x00, { 
      { 0,0 },
      { 1,0 },
      { 1,1 },
      { 0,1 }
    }
  }

};
*/

static struct block currBlock;

const u8 numBlockType = 7;

static u8 currId, nextId;
static signed char currX = BLOCK_START_X, currY = BLOCK_START_Y, nextX = FRAME_START_X + HSIZE + 3, nextY = FRAME_START_Y + 4;

static u8 gameIsOver = 0;
static u16 timeDropDown = TIME_DROP_DOWN;
static u16 timeGo = 0;
static u8 timeReduceStep = 27;

static u8  nLevel = 0;
static u16 nLines = 0;
static u16 nScore = 0;

static u8 key, okey; // for compatible
static u8 fixedBlocks[HSIZE * VSIZE];
static signed char baseY = 0, boundY = VSIZE;
static u8 fullFlags[VSIZE];

static SPRITE currSP[4];
static SPRITE nextSP[4];

//--------------------------------------------------------------------------------------- ;

void   assignCurrentBlock(u8 id);
void   drawFrame();
void   getRotatedPoint(struct point * p);
void   init();
void   moveDir();
void   moveDown();
void   moveLineToLine(u8 fromLine, u8 toLine);
void   refreshDropdownBlock();
void   refreshLines(u8 yTo, u8 yFrom);
void   refreshNextBlock();
void   rotateShape();
void   run();
void   updateBlock();

void   gameover();
void   pause();
void   update_info();

u8     isValidPosition(struct point * p);
u8     okToMove(signed char x, signed char y);
u8     tryMoveDir(u8 buttonDir);

void gameover()
{
  gotoxy(FRAME_START_X + 1, FRAME_START_Y + (VSIZE >> 1));
  cprintf("game over");

  pause();
}

void pause()
{
  while (1)
  {
    key = read_joy();
    if (presskey(button_START))
      return;
    okey = key;
  }
}

void update_info()
{
  gotoxy(19, 10);
  cprintf("Lines: %03d", nLines);
  gotoxy(19, 12);
  cprintf("Score: %04d", nScore);
  gotoxy(19, 14);
  cprintf("Level: %02d", nLevel);
}

int main()
{
  run();
  return 0;
}

void init()
{
  u8 i, j;

  // ugly initialization

  // Line
  tetrisPool[0].id = 0;
  tetrisPool[0].color = 1;
  tetrisPool[0].points[0].x = -1;
  tetrisPool[0].points[0].y = 0;
  tetrisPool[0].points[1].x = 0;
  tetrisPool[0].points[1].y = 0;
  tetrisPool[0].points[2].x = 1;
  tetrisPool[0].points[2].y = 0;
  tetrisPool[0].points[3].x = 2;
  tetrisPool[0].points[3].y = 0;
  
  // Z
  tetrisPool[1].id = 1;
  tetrisPool[1].color = 1;
  tetrisPool[1].points[0].x = -1;
  tetrisPool[1].points[0].y = 0;
  tetrisPool[1].points[1].x = 0;
  tetrisPool[1].points[1].y = 0;
  tetrisPool[1].points[2].x = 0;
  tetrisPool[1].points[2].y = 1;
  tetrisPool[1].points[3].x = 1;
  tetrisPool[1].points[3].y = 1;
  
  // L
  tetrisPool[2].id = 2;
  tetrisPool[2].color = 1;
  tetrisPool[2].points[0].x = 0;
  tetrisPool[2].points[0].y = -1;
  tetrisPool[2].points[1].x = 0;
  tetrisPool[2].points[1].y = 0;
  tetrisPool[2].points[2].x = 0;
  tetrisPool[2].points[2].y = 1;
  tetrisPool[2].points[3].x = 1;
  tetrisPool[2].points[3].y = 1;

  // mirrored L
  tetrisPool[3].id = 3;
  tetrisPool[3].color = 1;
  tetrisPool[3].points[0].x = 1;
  tetrisPool[3].points[0].y = -1;
  tetrisPool[3].points[1].x = 1;
  tetrisPool[3].points[1].y = 0;
  tetrisPool[3].points[2].x = 1;
  tetrisPool[3].points[2].y = 1;
  tetrisPool[3].points[3].x = 0;
  tetrisPool[3].points[3].y = 1;
  
  // square
  tetrisPool[4].id = 4;
  tetrisPool[4].color = 1;
  tetrisPool[4].points[0].x = 0;
  tetrisPool[4].points[0].y = 0;
  tetrisPool[4].points[1].x = 1;
  tetrisPool[4].points[1].y = 0;
  tetrisPool[4].points[2].x = 1;
  tetrisPool[4].points[2].y = 1;
  tetrisPool[4].points[3].x = 0;
  tetrisPool[4].points[3].y = 1;
  
  // S
  tetrisPool[5].id = 5;
  tetrisPool[5].color = 1;
  tetrisPool[5].points[0].x = 0;
  tetrisPool[5].points[0].y = 0;
  tetrisPool[5].points[1].x = 1;
  tetrisPool[5].points[1].y = 0;
  tetrisPool[5].points[2].x = 0;
  tetrisPool[5].points[2].y = 1;
  tetrisPool[5].points[3].x = -1;
  tetrisPool[5].points[3].y = 1;

  // triangle
  tetrisPool[6].id = 6;
  tetrisPool[6].color = 1;
  tetrisPool[6].points[0].x = 0;
  tetrisPool[6].points[0].y = 0;
  tetrisPool[6].points[1].x = 1;
  tetrisPool[6].points[1].y = 1;
  tetrisPool[6].points[2].x = 0;
  tetrisPool[6].points[2].y = 1;
  tetrisPool[6].points[3].x = -1;
  tetrisPool[6].points[3].y = 1;

  nLevel = nScore = nLines = 0;

  for ( i = 0; i < HSIZE*VSIZE; i++ )
    fixedBlocks[i] = 0;

  refreshLines(VSIZE - 1, 0);

  i = 64;
	set_VRAM_add(VRAM_name_0 + 960);
	while(i){
		address(PPU_memory_dat)= 0x00;
		--i;
	}

  disable_graphics();
  // load_BG_palette((u8*)(&nt_col));
  load_BG_palette((u8*)(&sp_col));
  load_SP_palette((u8*)(&sp_col));
  drawFrame();
  updateBlock();
  update_info();
  enable_graphics();
}

void drawFrame()
{
  u8 i;

  for (i = 0; i < HSIZE + 2; i++)
  {
    write_frame( i, 0 );
    write_frame( i, VSIZE + 1 );
  }

  for (i = 0; i < VSIZE + 2; i++)
  {
    write_frame( 0, i );
    write_frame( HSIZE + 1, i );
  }
}

void run()
{
  u16 i = 0;
  while (1)
  {
    init();
    while (!gameIsOver)
    {
      if (++timeGo >= timeDropDown)
      {
        moveDown();
        timeGo = 0;
      }

      key = read_joy();

      if (presskey(button_START))
      {
        pause();
      }

      if (downkey(button_A) || downkey(button_UP))
      {
        rotateShape();
      }

      if (isdirkey(key))
      {
        moveDir();
      }

      if (boundY == 0)
        gameIsOver = 1;

      okey = key;

      while (i++ < LOOP_TIMES_FOR_SLEEP) ;
      i = 0;
    }

    gameover();
  }
}

void rotateShape()
{
  u8 i;
  struct point p[4];
  for (i = 0; i < 4; ++i)
  {
    getRotatedPoint(currBlock.points + i);
    p[i].x = currX + pointBuffer.x;
    p[i].y = currY + pointBuffer.y;
  }

  for (i = 0; i < 4; i++)
  {
    if (!isValidPosition(p + i))
    {
      return;
    }
  }

  for (i = 0; i < 4; i++)
  {
    getRotatedPoint(currBlock.points + i);
    currBlock.points[i].x = pointBuffer.x;
    currBlock.points[i].y = pointBuffer.y;
  }
}

void refreshLines(u8 yTo, u8 yFrom)
{
  u8 i, j, mark = 0;

  if (yTo >= VSIZE)
    yTo = VSIZE - 1;

  if (yFrom > yTo)
    return;

  wait_VBlank();
  disable_graphics();
  for (i = yFrom; i <= yTo; i++)
  {
    for (j = 0; j < HSIZE; j++)
    {
      mark = fixedBlocks[unitindex(j, i)];

      if (mark)
        write_unit(j, i);
      else
        clear_unit(j, i);
    }
  }
  enable_graphics();
}


void moveDown()
{
  u8 i, lineBase, j, count = 0;
  signed char y, x;

  baseY = 0;

  if (tryMoveDir(button_DOWN))
  {
    return;
  }
  else
  { // collide
    for (i = 0; i < 4; ++i)
    {
      y = currY + currBlock.points[i].y;
      x = currX + currBlock.points[i].x;
      fixedBlocks[y * HSIZE + x] = 1;
      write_unit(x, y);
      if (y > baseY)
        baseY = y;
      if (y < boundY)
        boundY = y;
    }
    
    // reset fullFlag
    for (i = 0; i < VSIZE; i++)
      fullFlags[i] = 0;

    // judge full lines
    for (i = baseY, j = 0; j < 4; i--, j++)
    {
      y = 1;  // full line flag
      lineBase = i * HSIZE;
      for (x = 0; x < HSIZE; x++)
      {
        if (fixedBlocks[lineBase + x] == 0)
        {
          y = 0;
          break;
        }
      }

      if (y)
      {
        fullFlags[i] = 1;
        count += 1;
      }
    }

    if (count > 0)
    {
      // adjust fixed blocks
      for (i = baseY, j = baseY; j >= boundY; i--, j--)
      {
        while (fullFlags[j])
          j--;

        if (j < i)
          moveLineToLine(j, i);
      }

      refreshLines(baseY, boundY);
    }

    boundY += count;
    nLines += count;
    nScore += (count * count) << 1;
    nLevel  = nLines >> 4;

    timeDropDown = TIME_DROP_DOWN - nLevel;

    update_info();

    if (currY == 1)
      boundY = 0;

    // relocate dropping block
    currX = BLOCK_START_X;
    currY = BLOCK_START_Y;

    updateBlock();
  }
}

void assignCurrentBlock(u8 id)
{
  currBlock.id = id;
  currBlock.color = tetrisPool[id].color;
  currBlock.points[0] = tetrisPool[id].points[0];
  currBlock.points[1] = tetrisPool[id].points[1];
  currBlock.points[2] = tetrisPool[id].points[2];
  currBlock.points[3] = tetrisPool[id].points[3];
}

void updateBlock()
{
  currId = nextId;
  nextId = rnd(numBlockType);

  assignCurrentBlock(currId);

  refreshDropdownBlock();
  refreshNextBlock();
}

void refreshNextBlock()
{
  u8 i;
  for (i = 0; i < 4; i++)
  {
    nextSP[i].x = (nextX + tetrisPool[nextId].points[i].x) << 3;
    nextSP[i].y = (nextY + tetrisPool[nextId].points[i].y) << 3;
    nextSP[i].attr = tetrisPool[nextId].color;
    nextSP[i].tile = BLOCK_TILE_INDEX;
  }

  wait_VBlank();
  load_SP( (u8*)nextSP, 4, 4 );
}

void refreshDropdownBlock()
{
  u8 i;
  for (i = 0; i < 4; i++)
  {
    currSP[i].x = (currX + FRAME_START_X + 1 + currBlock.points[i].x) << 3;
    currSP[i].y = (currY + FRAME_START_Y + 1 + currBlock.points[i].y) << 3;
    currSP[i].attr = currBlock.color;
    currSP[i].tile = BLOCK_TILE_INDEX;
  }

  wait_VBlank();
  load_SP( (u8*)currSP, 0, 4 );
}

void moveLineToLine(u8 fromLine, u8 toLine)
{
  u8 i, fromBase, toBase;
  if (fromLine >= toLine)
  {
    return;
  }

  fromBase = fromLine * HSIZE;
  toBase   =   toLine * HSIZE;

  for (i = 0; i < HSIZE; i++)
  {
    fixedBlocks[toBase + i] = fixedBlocks[fromBase + i];
  }
}

void moveDir()
{
  if (presskey(button_DOWN))
  {
    moveDown();
  }
  else if (presskey(button_LEFT))
  {
    tryMoveDir(button_LEFT);
  }
  else if (presskey(button_RIGHT))
  {
    tryMoveDir(button_RIGHT);
  }
  else
  {
    // ignore up direction
  }
}

u8 tryMoveDir(u8 buttonDir)
{
  signed char x = 0, y = 0;

  switch (buttonDir)
  {
    case button_LEFT:
      x = -1;
      break;
    case button_RIGHT:
      x = 1;
      break;
    case button_DOWN:
      y = 1;
      break;
    default:
      // do nothing
      break;
  }

  if (!okToMove(x, y))
  {
    return 0;
  }

  currX += x;
  currY += y;

  refreshDropdownBlock();
  refreshNextBlock();

  return 1;
}

u8 okToMove(signed char x, signed char y)
{
  struct point pos, posB;

  pos.x = currX + x;
  pos.y = currY + y;

  for (x = 0; x < 4; ++x)
  {
    posB.x = pos.x + currBlock.points[x].x;
    posB.y = pos.y + currBlock.points[x].y;
    if (!isValidPosition(&posB))
    {
      return 0;
    }
  }

  return 1;
}

u8 isValidPosition(struct point * p)
{
  if (p->x < HSIZE && p->x >= 0 && p->y >=0 && p->y < VSIZE)
  {
    if ( fixedBlocks[p->y*HSIZE + p->x] != 0 )
      return 0;
    return 1;
  }
  return 0;
}
