#ifndef INPUTBOX_H
#define INPUTBOX_H

#define BLOCKPERCHUNK 16
#define	CHUNKHEIGHT 256
#define NOMBRELIGHT 7
#define NOMBRETEXTURE 10
#define false 0
#define true 1
#define LIM0 0.00001


#define STONE 0
#define DIRT 1
#define GRASS 2
#define GRASS2 3
#define SAND 4
#define WATER 5
#define GLOWSTONE 6
#define TRUNK 7
#define TRUNK2 8
#define FOLIAGE 9

typedef struct BlockS {
	unsigned char light;
	unsigned char texture;
	unsigned char face0And1; //4 bits light faces 1 puis 4 autre faces 2
	unsigned char face2And3;
	unsigned char face4And5;
	unsigned char visible; //6 bits faces visibles, 1 cube visible, 1 cubeDayLight, le 8 ème bit est inutile
	
	//	Face *face;
}Block;

typedef struct ChunkS {
	unsigned char referencePx;
	unsigned char referencePy;
	unsigned char referencePz;

	char visible;
	Block *block;
}Chunk;


typedef struct PlanS {
	int a;
	int b;
	int c;
	int d;
}Plan;


Chunk** initChunk();
EMSCRIPTEN_KEEPALIVE Block* initBlock();
EMSCRIPTEN_KEEPALIVE void freeChunk(Chunk** a);
EMSCRIPTEN_KEEPALIVE void freeBlock(Block* a);


EMSCRIPTEN_KEEPALIVE void WorldGeneration(Chunk** a);
EMSCRIPTEN_KEEPALIVE void createReliefWorld(Chunk** chunk);
EMSCRIPTEN_KEEPALIVE void determineReferencePChunk(Chunk** chunk, int i, int j);
EMSCRIPTEN_KEEPALIVE void createChunkRelief(Chunk** chunk, int i, int j);
EMSCRIPTEN_KEEPALIVE void putGrassWorld(Chunk** chunk);
EMSCRIPTEN_KEEPALIVE void putGrassChunk(Chunk** chunk, int i, int j);
EMSCRIPTEN_KEEPALIVE void putTreeWorld(Chunk** chunk);
EMSCRIPTEN_KEEPALIVE void putTreeChunk(Chunk** chunk, int i, int j);



EMSCRIPTEN_KEEPALIVE void visibleFaceWorld(Chunk** chunk);
EMSCRIPTEN_KEEPALIVE void visibleFaceChunk(Chunk** chunk, int i, int j);

EMSCRIPTEN_KEEPALIVE void initLightAndShadowWorld(Chunk** chunk);
EMSCRIPTEN_KEEPALIVE void initSunLightWorld(Chunk** chunk);
EMSCRIPTEN_KEEPALIVE void initSunLightChunk(Chunk** chunk, int i, int j);
EMSCRIPTEN_KEEPALIVE void initLightAndShadowWorldStep1(Chunk** chunk);
EMSCRIPTEN_KEEPALIVE void initLightAndShadowChunkStep1(Chunk** chunk, int i, int j, int n);
EMSCRIPTEN_KEEPALIVE void initLightAndShadowWorldStep2(Chunk** chunk);
EMSCRIPTEN_KEEPALIVE void initLightAndShadowChunkStep2(Chunk** chunk, int i, int j);

EMSCRIPTEN_KEEPALIVE void initAtlasBufferWorld(Chunk** chunk);
EMSCRIPTEN_KEEPALIVE void allocAtlasBufferWorldStep1();
EMSCRIPTEN_KEEPALIVE void tabSizeWorld(Chunk** chunk);
EMSCRIPTEN_KEEPALIVE void tabSizeChunk(Chunk** chunk, int i, int j);
EMSCRIPTEN_KEEPALIVE void allocAtlasBufferWorldStep2();
EMSCRIPTEN_KEEPALIVE void allocAtlasBufferChunkStep2(int i, int j);
EMSCRIPTEN_KEEPALIVE void resetTabSizeWorld();
EMSCRIPTEN_KEEPALIVE void resetTabSizeChunk(int i, int j);
EMSCRIPTEN_KEEPALIVE void calculBufferWorld(Chunk** chunk);
EMSCRIPTEN_KEEPALIVE void calculBufferChunk(Chunk** chunk, int i, int j, int ajustX, int ajustZ);
EMSCRIPTEN_KEEPALIVE int biggestChunkInWorld();

EMSCRIPTEN_KEEPALIVE void actuChunkXEnd();

	
EMSCRIPTEN_KEEPALIVE int specialTexture(unsigned char texture, int n);
EMSCRIPTEN_KEEPALIVE float distance(float ax, float ay, float az, float bx, float by, float bz);
EMSCRIPTEN_KEEPALIVE float distance2D(float ax, float ay, float bx, float by);
EMSCRIPTEN_KEEPALIVE char faceLight(Block a, char n);
EMSCRIPTEN_KEEPALIVE char faceVisible(char a, char n);
EMSCRIPTEN_KEEPALIVE char getTransparentTexture(unsigned char texture);
EMSCRIPTEN_KEEPALIVE int nEquf(float a, float b);

EMSCRIPTEN_KEEPALIVE void setPlayerPos();


EMSCRIPTEN_KEEPALIVE void freeAtlasBuffer();
EMSCRIPTEN_KEEPALIVE Chunk** ChunkTransfer();
EMSCRIPTEN_KEEPALIVE int nombreChunkTransfer();
//EMSCRIPTEN_KEEPALIVE short* atlasIndiceTabMax();

EMSCRIPTEN_KEEPALIVE void resetIndex();
//int EMSCRIPTEN_KEEPALIVE hitBoxWorld(float px, float  py, float pz, float vx, float  vy, float vz);


EMSCRIPTEN_KEEPALIVE Plan createPlan(int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3, int x4, int y4, int z4);

#endif



