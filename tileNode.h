#pragma once

#define TILESIZE 32

#define TILEX 200	//가로 타일 갯수
#define TILEY 100	//세로 타일 갯수

#define TILESIZEX TILESIZE * TILEX
#define TILESIZEY TILESIZE * TILEY

//타일셋 정의
#define SAMPLETILEX 16
#define SAMPLETILEY 32

#define ATTR_UNMOVE 0x00000001
#define ATTR_REFLECT 0x00000002
#define ATTR_SLOW 0x00000004

enum TERRAIN
{
	TR_GROUND, TR_WALL, TR_NOT_WALL, TR_GROUND2, TR_WALL2 = 5, TR_END =4
};

enum OBJECT
{
	OBJ, OBJ2, OBJ_STRUCT, OBJ_STRUCT2, OBJ_WEB, OBJ3, OBJ_STRUCT3,
	OBJ_NONE
};

enum POS
{
	POS_FLAG1, POS_FLAG2, POS_PACMAN1, POS_PACMAN2
};

enum CTRL
{
	CTRL_SAVE,
	CTRL_LOAD,
	CTRL_TERRAINDRAW,
	CTRL_OBJDRAW,
	CTRL_ERASER,
	CTRL_END
};

struct tagTOOL
{
	D2D_RECT_F rc;
	image* img;
	bool open;
	bool end;
};

struct tagTile
{
	int x, y;
	int sizeX, sizeY;
	int size;
	image* img;
	TERRAIN terrain;
	OBJECT obj;
	D2D_RECT_F rc;
	int terrainFrameX;
	int terrainFrameY;
	int objFrameX;
	int objFrameY;
};


struct tagSampleTile
{
	D2D_RECT_F rcTile;
	int terrainFrameX;
	int terrainFrameY;
	int objFrameX;
	int objFrameY;
	int sizeX, sizeY;
};

struct tagCurrentTile
{
	int x;
	int y;
};

struct saveButton
{
	image* img;
	D2D_RECT_F rc;
	int x, y;
};

struct loadButton
{
	image* img;
	D2D_RECT_F rc;
	int x, y;
};

struct eraserButton
{
	image* img;
	D2D_RECT_F rc;
	int x, y;
};

struct menuButton
{
	image* img;
	D2D_RECT_F rc;
	int x, y;
};

struct CTBOX
{
	image* img;
	D2D_RECT_F rc;
	int x, y;
};