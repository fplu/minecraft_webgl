#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <emscripten/emscripten.h>
#include "index.h"

#define visible0setTrue(a) a|=0b00000001
#define visible1setTrue(a) a|=0b00000010
#define visible2setTrue(a) a|=0b00000100
#define visible3setTrue(a) a|=0b00001000
#define visible4setTrue(a) a|=0b00010000
#define visible5setTrue(a) a|=0b00100000
#define visible6setTrue(a) a|=0b01000000
#define visible7setTrue(a) a|=0b10000000

#define visible0setFalse(a) a&=0b11111110
#define visible1setFalse(a) a&=0b11111101
#define visible2setFalse(a) a&=0b11111011
#define visible3setFalse(a) a&=0b11110111
#define visible4setFalse(a) a&=0b11101111
#define visible5setFalse(a) a&=0b11011111
#define visible6setFalse(a) a&=0b10111111
#define visible7setFalse(a) a&=0b01111111

#define visible0get(a) (a&0b00000001) ==  0b00000001 ? 1 : 0
#define visible1get(a) (a&0b00000010) ==  0b00000010 ? 1 : 0
#define visible2get(a) (a&0b00000100) ==  0b00000100 ? 1 : 0
#define visible3get(a) (a&0b00001000) ==  0b00001000 ? 1 : 0
#define visible4get(a) (a&0b00010000) ==  0b00010000 ? 1 : 0
#define visible5get(a) (a&0b00100000) ==  0b00100000 ? 1 : 0
#define visible6get(a) (a&0b01000000) ==  0b01000000 ? 1 : 0
#define visible7get(a) (a&0b10000000) ==  0b10000000 ? 1 : 0


#define set4FirstBit(a, b) a= (a & 0b11110000) + b
#define set4LastBit(a, b) a= (a & 0b00001111) + (b<<4)

#define get4FirstBit(a) a & 0b00001111
#define get4LastBit(a)  (a>>4) & 0b00001111

#define POW2(a) a*a
#define POW4(a) a*a*a*a
#define MAX(a, b) a<b ? b : a 
#define MIN(a, b) a<b ? a : b 
#define ABS(a) a<0 ? -a : a
#define NOTATEXTURE 0xFF

EMSCRIPTEN_KEEPALIVE Chunk** globalChunk;
EMSCRIPTEN_KEEPALIVE int nombreChunk = 20;
EMSCRIPTEN_KEEPALIVE float ****atlasPositionTab = NULL;
EMSCRIPTEN_KEEPALIVE float ****atlasNormalTab = NULL;
//EMSCRIPTEN_KEEPALIVE float ****atlasTextureTab = NULL;
EMSCRIPTEN_KEEPALIVE float ****atlasShadowTab = NULL;
//EMSCRIPTEN_KEEPALIVE short ****atlasIndiceTab = NULL;
EMSCRIPTEN_KEEPALIVE int biggestChunk[3] = { 0, 0, 0 };
EMSCRIPTEN_KEEPALIVE int ***tabSize = NULL;
EMSCRIPTEN_KEEPALIVE int indexTab[3] = { 0, 0, 0 };
EMSCRIPTEN_KEEPALIVE int sousIndexTab[3] = { 0, 0, 0 };

EMSCRIPTEN_KEEPALIVE float playerSize[3] = { 1.f, 2.f, 1.f };
EMSCRIPTEN_KEEPALIVE float playerPos[3] = { 0, 0, 0};
EMSCRIPTEN_KEEPALIVE float playerSpeed[3] = { 0, 0, 0 };
EMSCRIPTEN_KEEPALIVE int playerPosAnchor[2] = { 0, 0 };

EMSCRIPTEN_KEEPALIVE int playerPosAnchor0 = 0;


int main(int argc, char ** argv) {
	int i, j, k;
	printf("WebAssembly module loaded\n");

	srand(time(NULL));

	playerPosAnchor0 = BLOCKPERCHUNK*nombreChunk / 2;

	globalChunk = initChunk();
	if (globalChunk == NULL) {
		return -1;
	}
	printf("initChunk correctly\n");

	WorldGeneration(globalChunk);
	printf("init Word\n");

	visibleFaceWorld(globalChunk);
	initLightAndShadowWorld(globalChunk);

	printf("init ligth and shadow\n");

	initAtlasBufferWorld(globalChunk);
	printf("init Buffer\n");

	setPlayerPos();
	printf("setPlayerPos");

	EM_ASM(webGLStart());
	
	//EM_ASM(initOtherBuffer());
	//EM_ASM(initSkyBox());
	//EM_ASM(initAtlasBuffer());

	//EM_ASM(initAtlasBufferStep1());
	
	//printf("init 2 : %d\n", tabSize[biggestChunk[0]][biggestChunk[1]][biggestChunk[2]]);
	//int crotte = tabSize[biggestChunk[0]][biggestChunk[1]][biggestChunk[2]];
	//EM_ASM(initAtlasBufferStepTest(), crotte);
	//printf("init 2Bis : %d\n", tabSize[biggestChunk[0]][biggestChunk[1]][biggestChunk[2]]);

	//EM_ASM(initAtlasBufferStep2());// , *atlasIndiceTab[biggestChunk[0]][biggestChunk[1]][biggestChunk[2]]);

/*	for (i = 0; i < nombreChunk; i++) {
		for (j = 0; j < nombreChunk; j++) {
			for (k = 0; k < NOMBRETEXTURE; k++) {
				EM_ASM(initAtlasBufferStep3(), i, j, k, tabSize[i][j][k], atlasPositionTab[i][j][k], atlasNormalTab[i][j][k], atlasTextureTab[i][j][k], atlasShadowTab[i][j][k]);
			}
		}
	}*/
	
	//EM_ASM(initAtlasBuffersStep3(), biggestChunk, tabSize, atlasPositionTab, atlasNormalTab, atlasTextureTab, atlasShadowTab, atlasIndiceTab);

	printf("init 3\n");
	
	EM_ASM(tick());
	

	return 0;
}

Chunk** initChunk() {
	Chunk** res = NULL;
	int i, j;

	res = (Chunk**)calloc(sizeof(Chunk*),nombreChunk);
	if (res == NULL) {
		return NULL;
	}

	for (i = 0; i < nombreChunk; i++) {
		res[i] = (Chunk*)calloc(sizeof(Chunk),nombreChunk);
		if (res[i] == NULL) {
			freeChunk(res);
			return NULL;
		}
	}


	for (i = 1; i < nombreChunk - 1; i++) {
		for (j = 1; j < nombreChunk - 1; j++) {
			res[i][j].visible = 1;
			res[i][j].block = initBlock();
			if (res[i][j].block == NULL) {
				freeChunk(res);
				return NULL;
			}
		}
	}

	return res;
}

EMSCRIPTEN_KEEPALIVE Block* initBlock() {
	Block* res = NULL;
	int index;

	res = (Block*)malloc(sizeof(Block)*BLOCKPERCHUNK*CHUNKHEIGHT*BLOCKPERCHUNK);
	if (res == NULL) {
		return NULL;
	}

	for (index = 0; index < BLOCKPERCHUNK*CHUNKHEIGHT*BLOCKPERCHUNK; index++) {
		res[index].light = NOMBRELIGHT;
		res[index].texture = 0xFF;
		res[index].face0And1 = 0xFF;
		res[index].face2And3 = 0xFF;
		res[index].face4And5 = 0xFF;
		res[index].visible = 0;
	}

	return res;
}


EMSCRIPTEN_KEEPALIVE void freeChunk(Chunk** a) {
	int i, j;

	if (a == NULL) {
		return;
	}

	for (i = 0; i < nombreChunk; i++) {
		if (a[i] != NULL) {
			for (j = 0; j < nombreChunk; j++) {
				freeBlock(a[i][j].block);
			}
			free(a[i]);
		}
	}

	free(a);
}


EMSCRIPTEN_KEEPALIVE void freeBlock(Block* a) {
	int x, y, z;

	if (a == NULL) {
		return;
	}

	free(a);
}


EMSCRIPTEN_KEEPALIVE void WorldGeneration(Chunk** chunk) {
	int i, j, k;
	int tmp;

	createReliefWorld(chunk);

	putGrassWorld(chunk);

	putTreeWorld(chunk);



}


EMSCRIPTEN_KEEPALIVE void createReliefWorld(Chunk** chunk) {
	int i, j;
	for (i = 0; i < nombreChunk; i++) {
		for (j = 0; j < nombreChunk; j++) {
			determineReferencePChunk(chunk, i, j);
		}
	}

	for (i = 0; i < nombreChunk; i++) {
		for (j = 0; j < nombreChunk; j++) {
			createChunkRelief(chunk, i, j);
		}
	}
}

EMSCRIPTEN_KEEPALIVE void determineReferencePChunk(Chunk** chunk, int i, int j) {
	chunk[i][j].referencePx = ((unsigned int)rand()) % BLOCKPERCHUNK;
	chunk[i][j].referencePy = ((unsigned int)rand()) % 100 + 64;
	chunk[i][j].referencePz = ((unsigned int)rand()) % BLOCKPERCHUNK;
}

EMSCRIPTEN_KEEPALIVE void createChunkRelief(Chunk** chunk, int i, int j) {
	int x, y, z, a, b;
	float h, coeff, tmp;
	if (i + 1 >= nombreChunk || i - 1 < 0 || j + 1 >= nombreChunk || j - 1 < 0) {
		return;
	}

	coeff = 0;

	for (x = 0; x < BLOCKPERCHUNK; x++) {
		for (z = 0; z < BLOCKPERCHUNK; z++) {
			if (x == chunk[i][j].referencePx && z == chunk[i][j].referencePz) {
				h = chunk[i][j].referencePy;
			}
			else {
				coeff = 0;
				h = 0;
				for (a = -1; a <= 1; a++) {
					for (b = -1; b <= 1; b++) {
						tmp = POW4(distance2D(chunk[i + a][j + b].referencePx + a * 16, chunk[i + a][j + b].referencePz + b * 16, x, z));
						coeff += 1.f / tmp;
						h += chunk[i + a][j + b].referencePy / tmp;
					}
				}
				h /= coeff;
			}
			for (y = 0; y <= h; y++) {
				chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].visible = 0xFF;
				chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].texture = STONE;
				chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].light = NOMBRELIGHT;
			}
			for (; y < CHUNKHEIGHT; y++) {
				chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].visible = 0xFF;
				chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].texture = NOTATEXTURE;
				chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].light = NOMBRELIGHT;
			}
		}
	}
}

EMSCRIPTEN_KEEPALIVE void putGrassWorld(Chunk** chunk) {
	int i, j;

	for (i = 0; i < nombreChunk; i++) {
		for (j = 0; j < nombreChunk; j++) {
			putGrassChunk(chunk, i, j);
		}
	}
}

EMSCRIPTEN_KEEPALIVE void putGrassChunk(Chunk** chunk, int i, int j) {
	int x, y, z;
	int b;

	for (x = 0; x < BLOCKPERCHUNK; x++) {
		for (z = 0; z < BLOCKPERCHUNK; z++) {
			for (y = 0; y < CHUNKHEIGHT && chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].texture != NOTATEXTURE; y++);
			if (y > CHUNKHEIGHT - 3) {
				y = CHUNKHEIGHT - 3;
			}
			for (b = 0; b < 2; b++) {
				chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + (y + b)*BLOCKPERCHUNK + z].visible = 0xFF;
				chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + (y + b)*BLOCKPERCHUNK + z].texture = DIRT;
				chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + (y + b)*BLOCKPERCHUNK + z].light = NOMBRELIGHT;
			}
			chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + (y + b)*BLOCKPERCHUNK + z].visible = 0xFF;
			chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + (y + b)*BLOCKPERCHUNK + z].texture = GRASS;
			chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + (y + b)*BLOCKPERCHUNK + z].light = NOMBRELIGHT;

		}
	}
}

EMSCRIPTEN_KEEPALIVE void putTreeWorld(Chunk** chunk) {
	int i, j;
	for (i = 1; i < nombreChunk - 1; i++) {
		for (j = 1; j < nombreChunk - 1; j++) {
			putTreeChunk(chunk, i, j);
		}
	}
}

EMSCRIPTEN_KEEPALIVE void putTreeChunk(Chunk** chunk, int i, int j) {
	int x, y, z, x2, y2, z2;
	int a, b, c;
	int moy;
	int random;

	for (x = 0; x < BLOCKPERCHUNK; x++) {
		for (z = 0; z < BLOCKPERCHUNK; z++) {
			for (y = 0; y < CHUNKHEIGHT && chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].texture != NOTATEXTURE; y++);

			random = ((unsigned int)rand()) % (10 + 2 * y);
			if (random == 0) {
				random = (((unsigned int)rand()) % (3)) + 3;
				for (b = 0; b < random && y + b < CHUNKHEIGHT; b++) {
					chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + (y + b)*BLOCKPERCHUNK + z].visible = 0xFF;
					chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + (y + b)*BLOCKPERCHUNK + z].texture = TRUNK;
					chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + (y + b)*BLOCKPERCHUNK + z].light = NOMBRELIGHT;
				}

				for (x2 = -random; x2 <= random; x2++) {
					a = (i*BLOCKPERCHUNK + x + x2) / 16;
					if (a >= 1 && x2 + x + i * BLOCKPERCHUNK < BLOCKPERCHUNK*(nombreChunk - 1)) {
						for (y2 = random / 2; y2 <= random*1.5f; y2++) {
							if (y2 + y < CHUNKHEIGHT) {
								for (z2 = -random; z2 <= random; z2++) {
									b = (j*BLOCKPERCHUNK + z + z2) / 16;
									c = ((i*BLOCKPERCHUNK + x + x2) % 16)*BLOCKPERCHUNK*CHUNKHEIGHT + (y2 + y)*BLOCKPERCHUNK + (j*BLOCKPERCHUNK + z + z2) % 16;
									if (b >= 1 && z2 + z + j * BLOCKPERCHUNK < BLOCKPERCHUNK*(nombreChunk - 1) &&
										chunk[a][b].block[c].texture == NOTATEXTURE &&
										distance(x2, y2 - (random / 2), z2, 0.f, 0.f, 0.f) < random*random) {

										chunk[a][b].block[c].visible = 0xFF;
										chunk[a][b].block[c].texture = FOLIAGE;
										chunk[a][b].block[c].light = NOMBRELIGHT;
									}
								}
							}

						}
					}
				}

			}
		}
	}
}




EMSCRIPTEN_KEEPALIVE void visibleFaceWorld(Chunk** chunk) {
	int i, j;

	for (i = 1; i<nombreChunk - 1; i++) {
		for (j = 1; j<nombreChunk - 1; j++) {
			visibleFaceChunk(chunk, i, j);
		}
	}
}

EMSCRIPTEN_KEEPALIVE void visibleFaceChunk(Chunk** chunk, int i, int j) {
	int x, y, z;
	Block* tmp;

	for (x = 0; x<BLOCKPERCHUNK; x++) {
		for (y = 0; y<CHUNKHEIGHT; y++) {
			for (z = 0; z<BLOCKPERCHUNK; z++) {
				tmp = &chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z];
				if (x>0 && (getTransparentTexture(chunk[i][j].block[(x - 1)*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].texture) == 0
					/*|| chunk[i][j].block[(x - 1)*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].texture == tmp->texture*/)) {
					visible0setFalse(tmp->visible);
				}
				else if (i <= 1 && x == 0) {
					visible0setFalse(tmp->visible);
				}
				else if (x == 0 && (getTransparentTexture(chunk[i - 1][j].block[(BLOCKPERCHUNK - 1)* BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].texture) == 0
					/*|| chunk[i - 1][j].block[(BLOCKPERCHUNK - 1)* BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].texture == tmp->texture*/)) {
					visible0setFalse(tmp->visible);
				}
				else {
					visible0setTrue(tmp->visible);
				}

				if (x<BLOCKPERCHUNK - 1 && (getTransparentTexture(chunk[i][j].block[(x + 1)*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].texture) == 0
					|| chunk[i][j].block[(x + 1)*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].texture == tmp->texture)) {
					visible1setFalse(tmp->visible);
				}
				else if (i >= nombreChunk - 2 && x == BLOCKPERCHUNK - 1) {
					visible1setFalse(tmp->visible);
				}
				else if (x == BLOCKPERCHUNK - 1 && (getTransparentTexture(chunk[i + 1][j].block[y*BLOCKPERCHUNK + z].texture) == 0
					|| chunk[i + 1][j].block[y*BLOCKPERCHUNK + z].texture == tmp->texture)) {
					visible1setFalse(tmp->visible);
				}
				else {
					visible1setTrue(tmp->visible);
				}


				if (y > 0 && (getTransparentTexture(chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + (y - 1)*BLOCKPERCHUNK + z].texture) == 0
					/*|| chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + (y - 1)*BLOCKPERCHUNK + z].texture == tmp->texture*/)) {
					visible2setFalse(tmp->visible);
				}
				else if (y == 0) {
					visible2setFalse(tmp->visible);
				}
				else {
					visible2setTrue(tmp->visible);
				}

				if (y < CHUNKHEIGHT - 1 && (getTransparentTexture(chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + (y + 1)*BLOCKPERCHUNK + z].texture) == 0
					|| chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + (y + 1)*BLOCKPERCHUNK + z].texture == tmp->texture)) {
					visible3setFalse(tmp->visible);
				}
				/*else if (y == CHUNKHEIGHT - 1) {
				visible3setFalse(tmp->visible);
				}*/
				else {
					visible3setTrue(tmp->visible);
				}


				if (z>0 && (getTransparentTexture(chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + (z - 1)].texture) == 0
					/*|| chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + (z - 1)].texture == tmp->texture*/)) {
					visible4setFalse(tmp->visible);
				}
				else if (j <= 1 && z == 0) {
					visible4setFalse(tmp->visible);
				}
				else if (z == 0 && (getTransparentTexture(chunk[i][j - 1].block[x * BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + BLOCKPERCHUNK - 1].texture) == 0
					|| chunk[i][j - 1].block[x * BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + BLOCKPERCHUNK - 1].texture == tmp->texture)) {
					visible4setFalse(tmp->visible);
				}
				else {
					visible4setTrue(tmp->visible);
				}

				if (z<BLOCKPERCHUNK - 1 && (getTransparentTexture(chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + (z + 1)].texture) == 0
					|| chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + (z + 1)].texture == tmp->texture)) {
					visible5setFalse(tmp->visible);
				}
				else if (j >= nombreChunk - 2 && z == BLOCKPERCHUNK - 1) {
					visible5setFalse(tmp->visible);
				}
				else if (z == BLOCKPERCHUNK - 1 && (getTransparentTexture(chunk[i][j + 1].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK].texture) == 0
					|| chunk[i][j + 1].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK].texture == tmp->texture)) {
					visible5setFalse(tmp->visible);
				}
				else {
					visible5setTrue(tmp->visible);
				}
			}
		}
	}
}

EMSCRIPTEN_KEEPALIVE void initLightAndShadowWorld(Chunk** chunk) {
	initSunLightWorld(chunk);
	initLightAndShadowWorldStep1(chunk);
	initLightAndShadowWorldStep2(chunk);
}



EMSCRIPTEN_KEEPALIVE void initSunLightWorld(Chunk** chunk) {
	int i, j;
	for (i = 1; i < nombreChunk - 1; i++) {
		for (j = 1; j < nombreChunk - 1; j++) {
			initSunLightChunk(chunk, i, j);
		}
	}
}

EMSCRIPTEN_KEEPALIVE void initSunLightChunk(Chunk** chunk, int i, int j) {
	int x, y, z;
	for (x = 0; x < BLOCKPERCHUNK; x++) {
		for (z = 0; z < BLOCKPERCHUNK; z++) {
			for (y = CHUNKHEIGHT - 1; y >= 0; y--) {
				if (chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].texture == NOTATEXTURE) {
					chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].light = 0;
				}
				else {
					break;
				}
			}
			for (; y >= 0; y--) {
				if (chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].texture == NOTATEXTURE) {
					chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].light = NOMBRELIGHT;
				}
			}
		}
	}
}

EMSCRIPTEN_KEEPALIVE void initLightAndShadowWorldStep1(Chunk** chunk) { //cube light
	int i, j, n;
	
	for (n = 0; n <= NOMBRELIGHT; n++) {
		for (i = 1; i < nombreChunk - 1; i++) {
			for (j = 1; j < nombreChunk - 1; j++) {
				initLightAndShadowChunkStep1(chunk, i, j, n);
			}
		}
	}

}

EMSCRIPTEN_KEEPALIVE void initLightAndShadowChunkStep1(Chunk** chunk, int i, int j, int n) {
	int x, y, z, light;
	for (x = 0; x<BLOCKPERCHUNK; x++) {
		for (y = 0; y<CHUNKHEIGHT; y++) {
			for (z = 0; z<BLOCKPERCHUNK; z++) {
				if (chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].light == n) {

					light = chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].light;

					if (x>0 && light<chunk[i][j].block[(x - 1)*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].light
						&& chunk[i][j].block[(x - 1)*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].texture == NOTATEXTURE) {
						chunk[i][j].block[(x - 1)*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].light = light + 1;
					}
					else if (i>0 && x == 0 && light<chunk[i - 1][j].block[(BLOCKPERCHUNK - 1)*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].light
						&& chunk[i - 1][j].block[(BLOCKPERCHUNK - 1)*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].texture == NOTATEXTURE) {
						chunk[i - 1][j].block[(BLOCKPERCHUNK - 1)*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].light = light + 1;
					}

					if (x<BLOCKPERCHUNK - 1 && light<chunk[i][j].block[(x + 1)*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].light
						&& chunk[i][j].block[(x + 1)*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].texture == NOTATEXTURE) {
						chunk[i][j].block[(x + 1)*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].light = light + 1;
					}
					else if (i<nombreChunk - 1 && x == BLOCKPERCHUNK - 1 && light<chunk[i + 1][j].block[y*BLOCKPERCHUNK + z].light
						&& chunk[i + 1][j].block[y*BLOCKPERCHUNK + z].texture == NOTATEXTURE) {
						chunk[i + 1][j].block[y*BLOCKPERCHUNK + z].light = light + 1;
					}


					if (y>0 && light<chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + (y - 1)*BLOCKPERCHUNK + z].light
						&& chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + (y - 1)*BLOCKPERCHUNK + z].texture == NOTATEXTURE) {
						chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + (y - 1)*BLOCKPERCHUNK + z].light = light + 1;
					}

					if (y<CHUNKHEIGHT - 1 && light<chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + (y + 1)*BLOCKPERCHUNK + z].light
						&& chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + (y + 1)*BLOCKPERCHUNK + z].texture == NOTATEXTURE) {
						chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + (y + 1)*BLOCKPERCHUNK + z].light = light + 1;
					}


					if (z>0 && light<chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z - 1].light
						&& chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z - 1].texture == NOTATEXTURE) {
						chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z - 1].light = light + 1;
					}
					else if (j>0 && z == 0 && light<chunk[i][j - 1].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + BLOCKPERCHUNK - 1].light
						&& chunk[i][j - 1].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + BLOCKPERCHUNK - 1].texture == NOTATEXTURE) {
						chunk[i][j - 1].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + BLOCKPERCHUNK - 1].light = light + 1;
					}

					if (z<BLOCKPERCHUNK - 1 && light<chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z + 1].light
						&& chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z + 1].texture == NOTATEXTURE) {
						chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z + 1].light = light + 1;
					}
					else if (j<nombreChunk - 1 && z == BLOCKPERCHUNK - 1 && light<chunk[i][j + 1].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK].light
						&& chunk[i][j + 1].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK].texture == NOTATEXTURE) {
						chunk[i][j + 1].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK].light = light + 1;
					}
				}
			}
		}
	}
}


EMSCRIPTEN_KEEPALIVE void initLightAndShadowWorldStep2(Chunk** chunk) { //face light
	int i, j;

	for (i = 1; i < nombreChunk - 1; i++) {
		for (j = 1; j < nombreChunk - 1; j++) {
			initLightAndShadowChunkStep2(chunk, i, j);
		}
	}
}

EMSCRIPTEN_KEEPALIVE void initLightAndShadowChunkStep2(Chunk** chunk, int i, int j) {
	int x, y, z;

	for (x = 0; x<BLOCKPERCHUNK; x++) {
		for (y = 0; y<CHUNKHEIGHT; y++) {
			for (z = 0; z<BLOCKPERCHUNK; z++) {
				if (visible6get(chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].visible)) {
					if (visible0get(chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].visible)) {
						if (x>0) {
							set4FirstBit(chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].face0And1, chunk[i][j].block[(x - 1)*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].light);
						}
						else if (i>0) {
							set4FirstBit(chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].face0And1, chunk[i - 1][j].block[(BLOCKPERCHUNK - 1)*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].light);
						}
					}

					if (visible1get(chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].visible)) {
						if (x<BLOCKPERCHUNK - 1) {
							set4LastBit(chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].face0And1, chunk[i][j].block[(x + 1)*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].light);
						}
						else if (i<nombreChunk - 1) {
							set4LastBit(chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].face0And1, chunk[i + 1][j].block[y*BLOCKPERCHUNK + z].light);
						}
					}


					if (visible2get(chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].visible)) {
						if (y>0) {
							set4FirstBit(chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].face2And3, chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + (y - 1)*BLOCKPERCHUNK + z].light);
						}
					}

					if (visible3get(chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].visible)) {
						if (y<CHUNKHEIGHT - 1) {
							set4LastBit(chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].face2And3, chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + (y + 1)*BLOCKPERCHUNK + z].light);
						}
						else if (y == CHUNKHEIGHT - 1) {
							set4LastBit(chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].face2And3, 0);
						}
					}


					if (visible4get(chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].visible)) {
						if (z>0) {
							set4FirstBit(chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].face4And5, chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z - 1].light);
						}
						else if (j>0) {
							set4FirstBit(chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].face4And5, chunk[i][j - 1].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + BLOCKPERCHUNK - 1].light);
						}
					}

					if (visible5get(chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].visible)) {
						if (z<BLOCKPERCHUNK - 1) {
							set4LastBit(chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].face4And5, chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z + 1].light);
						}
						else if (j<nombreChunk - 1) {
							set4LastBit(chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].face4And5, chunk[i][j + 1].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK].light);
						}
					}
				}
			}
		}
	}
}




EMSCRIPTEN_KEEPALIVE void initAtlasBufferWorld(Chunk** chunk) {
	allocAtlasBufferWorldStep1();
	tabSizeWorld(chunk);
	biggestChunkInWorld();
	//EM_ASM(repareBuffer());
	allocAtlasBufferWorldStep2();
	resetTabSizeWorld();
	calculBufferWorld(chunk);
}

EMSCRIPTEN_KEEPALIVE void allocAtlasBufferWorldStep1() {
	int i, j;

	atlasPositionTab = (float****)malloc(sizeof(float***)*(nombreChunk - 2));
	atlasNormalTab = (float****)malloc(sizeof(float***)*(nombreChunk - 2));
	atlasShadowTab = (float****)malloc(sizeof(float***)*(nombreChunk - 2));
	tabSize = (int***)malloc(sizeof(int**)*(nombreChunk - 2));

	for (i = 0; i<(nombreChunk - 2); i++) {

		atlasPositionTab[i] = (float***)malloc(sizeof(float**)*(nombreChunk - 2));
		atlasNormalTab[i] = (float***)malloc(sizeof(float**)*(nombreChunk - 2));
		atlasShadowTab[i] = (float***)malloc(sizeof(float**)*(nombreChunk - 2));
		tabSize[i] = (int**)malloc(sizeof(int*)*(nombreChunk - 2));

		for (j = 0; j<(nombreChunk - 2); j++) {
			atlasPositionTab[i][j] = (float**)malloc(sizeof(float*)*NOMBRETEXTURE);
			atlasNormalTab[i][j] = (float**)malloc(sizeof(float*)*NOMBRETEXTURE);
			atlasShadowTab[i][j] = (float**)malloc(sizeof(float*)*NOMBRETEXTURE);
			tabSize[i][j] = (int*)calloc(sizeof(int), NOMBRETEXTURE);

		}
	}
}

EMSCRIPTEN_KEEPALIVE void tabSizeWorld(Chunk** chunk) {
	int i, j;

	for (i = 1; i<nombreChunk - 1; i++) {
		for (j = 1; j<nombreChunk - 1; j++) {
			tabSizeChunk(chunk, i, j);
		}
	}
}

EMSCRIPTEN_KEEPALIVE void tabSizeChunk(Chunk** chunk, int i, int j) {
	int x, y, z, n;
	char tmp;
	for (x = 0; x<BLOCKPERCHUNK; x++) {
		for (y = 0; y<CHUNKHEIGHT; y++) {
			for (z = 0; z<BLOCKPERCHUNK; z++) {
				if (visible6get(chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].visible)) {
					for (n = 0; n<6; n++) {
						if (faceVisible(chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].visible, n) && chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].texture != NOTATEXTURE) {
							tmp = chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].texture;
							tmp = specialTexture(tmp, n);

							tabSize[i - 1][j - 1][tmp]++;
						}
					}
				}
			}
		}
	}
}

EMSCRIPTEN_KEEPALIVE void allocAtlasBufferWorldStep2() {
	int i, j;
	for (i = 1; i < nombreChunk - 1; i++) {
		for (j = 1; j < nombreChunk - 1; j++) {
			allocAtlasBufferChunkStep2(i, j);
		}
	}
}

EMSCRIPTEN_KEEPALIVE void allocAtlasBufferChunkStep2(int i, int j) {
	int k;
	for (k = 0; k < NOMBRETEXTURE; k++) {
		atlasPositionTab[i - 1][j - 1][k] = (float*)malloc(sizeof(float)*tabSize[i - 1][j - 1][k] * 12);
		atlasNormalTab[i - 1][j - 1][k] = (float*)malloc(sizeof(float)*tabSize[i - 1][j - 1][k] * 12);
		atlasShadowTab[i - 1][j - 1][k] = (float*)malloc(sizeof(float)*tabSize[i - 1][j - 1][k] * 4);
	}
}

EMSCRIPTEN_KEEPALIVE void resetTabSizeWorld() {
	int i, j;
	for (i = 1; i < nombreChunk - 1; i++) {
		for (j = 1; j < nombreChunk - 1; j++) {
			resetTabSizeChunk(i, j);
		}
	}
}

EMSCRIPTEN_KEEPALIVE void resetTabSizeChunk(int i, int j) {
	int k;
	for (k = 0; k < NOMBRETEXTURE; k++) {
		tabSize[i - 1][j - 1][k] = 0;
	}
}

EMSCRIPTEN_KEEPALIVE void calculBufferWorld(Chunk** chunk) {
	int i, j;
	for (i = 1; i < nombreChunk - 1; i++) {
		for (j = 1; j < nombreChunk - 1; j++) {
			calculBufferChunk(chunk, i, j, 0, 0);
		}
	}
}

EMSCRIPTEN_KEEPALIVE void calculBufferChunk(Chunk** chunk, int i, int j, int ajustX, int ajustZ) {
	int x, y, z, n, o, tmp;
	int position[6][12];


	float vertexNormals[6][12] = {
		{ -1.0, 0.0, 0.0,
		-1.0, 0.0, 0.0,
		-1.0, 0.0, 0.0,
		-1.0, 0.0, 0.0 },

		{ 1.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		1.0, 0.0, 0.0 },

		{ 0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0 },

		{ 0.0, 1.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0 },

		{ 0.0, 0.0, -1.0,
		0.0, 0.0, -1.0,
		0.0, 0.0, -1.0,
		0.0, 0.0, -1.0 },

		{ 0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0 }
	};


	int textureCoords[8] = {
		0.0, 1.0,
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0 };




	for (x = 0; x<BLOCKPERCHUNK; x++) {
		for (y = 0; y<CHUNKHEIGHT; y++) {
			for (z = 0; z<BLOCKPERCHUNK; z++) {
				if (visible6get(chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].visible)) {
					position[0][0] = i * 16 + x + ajustX;
					position[0][1] = y;
					position[0][2] = j * 16 + z + ajustZ;

					position[0][3] = i * 16 + x + ajustX;
					position[0][4] = 1 + y;
					position[0][5] = j * 16 + z + ajustZ;

					position[0][6] = i * 16 + x + ajustX;
					position[0][7] = 1 + y;
					position[0][8] = 1 + j * 16 + z + ajustZ;

					position[0][9] = i * 16 + x + ajustX;
					position[0][10] = y;
					position[0][11] = 1 + j * 16 + z + ajustZ;


					position[1][0] = 1 + position[0][0];
					position[1][1] = position[0][1];
					position[1][2] = position[0][2];

					position[1][3] = 1 + position[0][3];
					position[1][4] = position[0][4];
					position[1][5] = position[0][5];

					position[1][6] = 1 + position[0][6];
					position[1][7] = position[0][7];
					position[1][8] = position[0][8];

					position[1][9] = 1 + position[0][9];
					position[1][10] = position[0][10];
					position[1][11] = position[0][11];


					position[2][0] = i * 16 + x + ajustX;
					position[2][1] = y;
					position[2][2] = j * 16 + z + ajustZ;

					position[2][3] = i * 16 + x + ajustX;
					position[2][4] = y;
					position[2][5] = 1 + j * 16 + z + ajustZ;

					position[2][6] = 1 + i * 16 + x + ajustX;
					position[2][7] = y;
					position[2][8] = 1 + j * 16 + z + ajustZ;

					position[2][9] = 1 + i * 16 + x + ajustX;
					position[2][10] = y;
					position[2][11] = j * 16 + z + ajustZ;


					position[3][0] = position[2][0];
					position[3][1] = 1 + position[2][1];
					position[3][2] = position[2][2];

					position[3][3] = position[2][3];
					position[3][4] = 1 + position[2][4];
					position[3][5] = position[2][5];

					position[3][6] = position[2][6];
					position[3][7] = 1 + position[2][7];
					position[3][8] = position[2][8];

					position[3][9] = position[2][9];
					position[3][10] = 1 + position[2][10];
					position[3][11] = position[2][11];


					position[4][0] = i * 16 + x + ajustX;
					position[4][1] = y;
					position[4][2] = j * 16 + z + ajustZ;

					position[4][3] = i * 16 + x + ajustX;
					position[4][4] = 1 + y;
					position[4][5] = j * 16 + z + ajustZ;

					position[4][6] = 1 + i * 16 + x + ajustX;
					position[4][7] = 1 + y;
					position[4][8] = j * 16 + z + ajustZ;

					position[4][9] = 1 + i * 16 + x + ajustX;
					position[4][10] = y;
					position[4][11] = j * 16 + z + ajustZ;


					position[5][0] = position[4][0];
					position[5][1] = position[4][1];
					position[5][2] = 1 + position[4][2];

					position[5][3] = position[4][3];
					position[5][4] = position[4][4];
					position[5][5] = 1 + position[4][5];

					position[5][6] = position[4][6];
					position[5][7] = position[4][7];
					position[5][8] = 1 + position[4][8];

					position[5][9] = position[4][9];
					position[5][10] = position[4][10];
					position[5][11] = 1 + position[4][11];

					for (n = 0; n<6; n++) {
						if (faceVisible(chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].visible, n) && chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].texture != NOTATEXTURE) {
							tmp = chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z].texture;
							tmp = specialTexture(tmp, n);

							for (o = 0; o<12; o++) {
								atlasPositionTab[i - 1][j - 1][tmp][tabSize[i - 1][j - 1][tmp] * 12 + o] = position[n][o];
										atlasNormalTab[i - 1][j - 1][tmp][tabSize[i - 1][j - 1][tmp] * 12 + o] = vertexNormals[n][o];

							}


							for (o = 0; o<4; o++) {
								atlasShadowTab[i - 1][j - 1][tmp][tabSize[i - 1][j - 1][tmp] * 4 + o] = 1.f - POW2(faceLight(chunk[i][j].block[x*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + z], n)) / (float)(POW2(NOMBRELIGHT + 2));
							}


							tabSize[i - 1][j - 1][tmp]++;
						}
					}
				}
			}
		}
	}
}


EMSCRIPTEN_KEEPALIVE int biggestChunkInWorld() {
	int i, j, k;
	int res = 0;
	for (i = 1; i<nombreChunk - 1; i++) {
		for (j = 1; j<nombreChunk - 1; j++) {
			for (k = 0; k<NOMBRETEXTURE; k++) {
				if (tabSize[biggestChunk[0]][biggestChunk[1]][biggestChunk[2]]<tabSize[i - 1][j - 1][k - 1]) {
					biggestChunk[0] = i - 1;
					biggestChunk[1] = j - 1;
					biggestChunk[2] = k - 1;
					res = 1;
				}
			}
		}
	}
	return res;
}





EMSCRIPTEN_KEEPALIVE void freeAtlasBuffer() {
	int i, j, k;

	for (i = 0; i<nombreChunk; i++) {
		for (j = 0; j<nombreChunk; j++) {
			for (k = 0; k < nombreChunk; k++) {
				free(atlasPositionTab[i][j][k]);
				free(atlasNormalTab[i][j][k]);
				//free(atlasTextureTab[i][j][k]);
				free(atlasShadowTab[i][j][k]);
				//free(atlasIndiceTab[i][j][k]);
			}
			free(atlasPositionTab[i][j]);
			free(atlasNormalTab[i][j]);
			//free(atlasTextureTab[i][j]);
			free(atlasShadowTab[i][j]);
			//free(atlasIndiceTab[i][j]);
			free(tabSize[i][j]);
		}
		free(atlasPositionTab[i]);
		free(atlasNormalTab[i]);
		//free(atlasTextureTab[i]);
		free(atlasShadowTab[i]);
		//free(atlasIndiceTab[i]);
		free(tabSize[i]);
	}
	free(atlasPositionTab);
	free(atlasNormalTab);
	//free(atlasTextureTab);
	free(atlasShadowTab);
	//free(atlasIndiceTab);
	free(tabSize);
}





EMSCRIPTEN_KEEPALIVE void setPlayerPos() {
	int y, a, b, c, d;
	playerPos[0] = playerPosAnchor0;
	playerPos[2] = playerPosAnchor0;
	
	a = ((int)playerPos[0]) / BLOCKPERCHUNK;
	b = ((int)playerPos[2]) / BLOCKPERCHUNK;
	c = ((int)playerPos[0]) % BLOCKPERCHUNK;
	d = ((int)playerPos[2]) % BLOCKPERCHUNK;

	for (y = 0; y < CHUNKHEIGHT && globalChunk[a][b].block[c*BLOCKPERCHUNK*CHUNKHEIGHT + y*BLOCKPERCHUNK + d].texture != NOTATEXTURE; y++);
	playerPos[1] = y + 1;

	playerPosAnchor[0] = playerPosAnchor0;
	playerPosAnchor[1] = playerPosAnchor0;

	playerPos[0] = 0;
	playerPos[2] = 0;
}


EMSCRIPTEN_KEEPALIVE char faceLight(Block a, char n) {
	switch (n) {
	case 0:
		return get4FirstBit(a.face0And1);
	case 1:
		return get4LastBit(a.face0And1);
	case 2:
		return get4FirstBit(a.face2And3);
	case 3:
		return get4LastBit(a.face2And3);
	case 4:
		return get4FirstBit(a.face4And5);
	case 5:
		return get4LastBit(a.face4And5);
	}
	return 0;
}

EMSCRIPTEN_KEEPALIVE char faceVisible(char a, char n) {
	switch (n) {
	case 0:
		return visible0get(a);
	case 1:
		return visible1get(a);
	case 2:
		return visible2get(a);
	case 3:
		return visible3get(a);
	case 4:
		return visible4get(a);
	case 5:
		return visible5get(a);
	}
	return 0;
}


EMSCRIPTEN_KEEPALIVE char getTransparentTexture(unsigned char texture) {
	if (texture == NOTATEXTURE || texture == WATER || texture == FOLIAGE) {
		return 1;
	}
	return 0;
}


EMSCRIPTEN_KEEPALIVE int specialTexture(unsigned char texture, int n) {
	if (texture == GRASS) {
		if (n == 2) {//face dessous
			texture = DIRT;
		}
		else if (n != 3) {//face latérale
			texture = GRASS2;
		}
	}
	if (texture == TRUNK) {
		if (n == 2 || n == 3) {//face dessous
			texture = TRUNK2;
		}
	}
	return texture;
}
EMSCRIPTEN_KEEPALIVE float distance2D(float ax, float ay, float bx, float by) {
	return (ax - bx)*(ax - bx) + (ay - by)*(ay - by);
}
EMSCRIPTEN_KEEPALIVE float distance(float ax, float ay, float az, float bx, float by, float bz) {
	return (ax - bx)*(ax - bx) + (ay - by)*(ay - by) + (az - bz)*(az - bz);
}

EMSCRIPTEN_KEEPALIVE int nEquf(float a, float b) {
	if (a<b + LIM0 && a>b - LIM0) {
		return 0;
	}
	return 1;
}

EMSCRIPTEN_KEEPALIVE Chunk** ChunkTransfer() {

	return globalChunk;
}

EMSCRIPTEN_KEEPALIVE int nombreChunkTransfer() {
	return nombreChunk - 2;
}

EMSCRIPTEN_KEEPALIVE int nombreTextureTransfer() {
	return NOMBRETEXTURE;
}


EMSCRIPTEN_KEEPALIVE int tabSizeMax() {
	//printf("big2 : %d, %d, %d\n", biggestChunk[0], biggestChunk[1], biggestChunk[2]);
	//printf("bizerzerg2 : %d\n", tabSize[2][3][0]);
	//printf("bizeretrzerg2 : %d\n", tabSize[1][0][0]);

	return tabSize[biggestChunk[0]][biggestChunk[1]][biggestChunk[2]];
}

/*EMSCRIPTEN_KEEPALIVE short* atlasIndiceTabMax() {
	printf("p : %p\n", atlasIndiceTab[biggestChunk[0]][biggestChunk[1]][biggestChunk[2]]);
	return atlasIndiceTab[biggestChunk[0]][biggestChunk[1]][biggestChunk[2]];
	}*/

EMSCRIPTEN_KEEPALIVE void resetIndex() {
	int i;
	for (i = 0; i < 3; i++) {
		indexTab[i] = 0;
	}
}

EMSCRIPTEN_KEEPALIVE void resetSousIndex() {
	int i;
	for (i = 0; i < 3; i++) {
		sousIndexTab[i] = 0;
	}
}



EMSCRIPTEN_KEEPALIVE int getAtlasTabSize() {
	return tabSize[indexTab[0]][indexTab[1]][indexTab[2]];
}

EMSCRIPTEN_KEEPALIVE void growIndex() {
	indexTab[2]++;
	if (indexTab[2] >= NOMBRETEXTURE) {
		indexTab[2] = 0;
		indexTab[1]++;
		if (indexTab[1] >= nombreChunk - 2) {
			indexTab[1] = 0;
			indexTab[0]++;
			if (indexTab[0] >= nombreChunk - 2) {
				indexTab[0] = 0;
			}
		}
	}
}

EMSCRIPTEN_KEEPALIVE float getAtlasPositionTabIndex() {
	sousIndexTab[0]++;
	return atlasPositionTab[indexTab[0]][indexTab[1]][indexTab[2]][sousIndexTab[0] - 1];
}

EMSCRIPTEN_KEEPALIVE float getAtlasNormalTabIndex() {
	sousIndexTab[1]++;
	return atlasNormalTab[indexTab[0]][indexTab[1]][indexTab[2]][sousIndexTab[1] - 1];
}


EMSCRIPTEN_KEEPALIVE float getAtlasShadowTabIndex() {
	sousIndexTab[2]++;
	return atlasShadowTab[indexTab[0]][indexTab[1]][indexTab[2]][sousIndexTab[2] - 1];
}



EMSCRIPTEN_KEEPALIVE void hitBoxWorld(float px, float  py, float pz, float vx, float  vy, float vz) {
	int i, j, k;
	int a, b, c;
	/*if (playerPosAnchor[0]+BLOCKPERCHUNK <= (int)playerPos[0]) {
		createChunk(0);
		}*/

	/*if (playerSpeed[1] != 0) {
		playerSpeed[1] -= 0.01f;
		playerSpeed[1] = playerSpeed[1] < -1.4f ? -1.4f : playerSpeed[1];
		vy = playerSpeed[1];
	}*/


	playerPos[0] = px;
	playerPos[1] = py;
	playerPos[2] = pz;
	px += playerPosAnchor0;
	pz += playerPosAnchor0;
	//; py -= playerSize[1] / 4;


	/*a = vx > 0 ? (px) : (px);
	b = vy > 0 ? (py) : (py);
	c = vz > 0 ? (pz) : (pz);

	if (getTransparentTexture(globalChunk[a / 16][c / 16].block[(a % BLOCKPERCHUNK)*BLOCKPERCHUNK*CHUNKHEIGHT + b*BLOCKPERCHUNK + (c%BLOCKPERCHUNK)].texture) == 1) {
		px += vx;
		py += vy;
		pz += vz;
		goto end;
	}*/


	if (vx != 0 && vy != 0 && vz != 0) {
		a = vx > 0 ? (px + vx) : (px + vx);
		b = vy > 0 ? (py + vy) : (py + vy);
		c = vz > 0 ? (pz + vz) : (pz + vz);
		if (getTransparentTexture(globalChunk[a / 16][c / 16].block[(a % BLOCKPERCHUNK)*BLOCKPERCHUNK*CHUNKHEIGHT + b*BLOCKPERCHUNK + (c%BLOCKPERCHUNK)].texture) == 1) {
			px += vx;
			py += vy;
			pz += vz;
			goto end;
		}
	}

	if (vx != 0 && vz != 0) {
		a = vx > 0 ? (px + vx) : (px + vx);
		b = py;
		c = vz > 0 ? (pz + vz) : (pz + vz);
		if (getTransparentTexture(globalChunk[a / 16][c / 16].block[(a % BLOCKPERCHUNK)*BLOCKPERCHUNK*CHUNKHEIGHT + b*BLOCKPERCHUNK + (c%BLOCKPERCHUNK)].texture) == 1) {
			px += vx;
			pz += vz;
			goto end;
		}
	}

	if (vx != 0 && vy != 0) {
		a = vx > 0 ? (px + vx) : (px + vx);
		b = vy > 0 ? (py + vy) : (py + vy);
		c = pz;
		if (getTransparentTexture(globalChunk[a / 16][c / 16].block[(a % BLOCKPERCHUNK)*BLOCKPERCHUNK*CHUNKHEIGHT + b*BLOCKPERCHUNK + (c%BLOCKPERCHUNK)].texture) == 1) {
			px += vx;
			py += vy;
			goto end;
		}
	}

	if (vy != 0 && vz != 0) {
		a = px;
		b = vy > 0 ? (py + vy) : (py + vy);
		c = vz > 0 ? (pz + vz) : (pz + vz);
		if (getTransparentTexture(globalChunk[a / 16][c / 16].block[(a % BLOCKPERCHUNK)*BLOCKPERCHUNK*CHUNKHEIGHT + b*BLOCKPERCHUNK + (c%BLOCKPERCHUNK)].texture) == 1) {
			py += vy;
			pz += vz;
			goto end;
		}
	}


	if (vx != 0) {
		a = vx > 0 ? (px + vx) : (px + vx);
		b = py;
		c = pz;
		if (getTransparentTexture(globalChunk[a / 16][c / 16].block[(a % BLOCKPERCHUNK)*BLOCKPERCHUNK*CHUNKHEIGHT + b*BLOCKPERCHUNK + (c%BLOCKPERCHUNK)].texture) == 1) {
			px += vx;
			goto end;
		}
	}

	if (vz != 0) {
		a = px;
		b = py;
		c = vz > 0 ? (pz + vz) : (pz + vz);
		if (getTransparentTexture(globalChunk[a / 16][c / 16].block[(a % BLOCKPERCHUNK)*BLOCKPERCHUNK*CHUNKHEIGHT + b*BLOCKPERCHUNK + (c%BLOCKPERCHUNK)].texture) == 1) {
			pz += vz;
			goto end;
		}
	}


	if (vy != 0) {
		a = px;
		b = vy > 0 ? (py + vy) : (py + vy);
		c = pz;
		if (getTransparentTexture(globalChunk[a / 16][c / 16].block[(a % BLOCKPERCHUNK)*BLOCKPERCHUNK*CHUNKHEIGHT + b*BLOCKPERCHUNK + (c%BLOCKPERCHUNK)].texture) == 1) {
			py += vy;
			goto end;
		}
	}


end:

	//py += playerSize[1] / 4;
	px -= playerPosAnchor0;
	pz -= playerPosAnchor0;

	playerSpeed[0] = px - playerPos[0];
	playerSpeed[1] = py - playerPos[1];
	playerSpeed[2] = pz - playerPos[2];


	if (nEquf(playerSpeed[1], vy)) {
		py = vy < 0 ? floorf(py) + LIM0 : ceilf(py) - LIM0;
	}

	if (nEquf(playerSpeed[0], vx)) {
		px = vx < 0 ? floorf(px) + LIM0 : ceilf(px) - LIM0;
	}

	if (nEquf(playerSpeed[2], vz)) {
		pz = vz < 0 ? floorf(pz) + LIM0 : ceilf(pz) - LIM0;
	}
	playerPos[0] = px;
	playerPos[1] = py;
	playerPos[2] = pz;

	if (BLOCKPERCHUNK < playerPos[0]) {
		actuChunkXEnd();
	}

}

EMSCRIPTEN_KEEPALIVE void actuChunkXEnd() {
	int i, j, n;
	float ***a, ***b, ***c;
	int **d;
	playerPosAnchor[0] += BLOCKPERCHUNK;
	playerPos[0] -= BLOCKPERCHUNK;

	for (j = 1; j < nombreChunk - 1; j++) {//libère
		globalChunk[nombreChunk - 1][j].block = globalChunk[1][j].block;
	}

	for (i = 0; i < nombreChunk - 1; i++) {//décale
		for (j = 0; j < nombreChunk; j++) {
			globalChunk[i][j] = globalChunk[i + 1][j];
		}
	}

	i = nombreChunk - 2;
	/*for (j = 1; j < nombreChunk - 1; j++) {//realloc
		globalChunk[i][j].block = initBlock();
	}*/

	for (j = 0; j < nombreChunk; j++) {//new invisible Line
		determineReferencePChunk(globalChunk, nombreChunk - 1, j);
	}

	for (j = 1; j < nombreChunk - 1; j++) {//terra forme
		createChunkRelief(globalChunk, i, j);
	}

	for (j = 1; j < nombreChunk - 1; j++) {//put grass
		putGrassChunk(globalChunk, i, j);
	}

	for (j = 1; j < nombreChunk - 1; j++) {//put Tree
		putTreeChunk(globalChunk, i, j);
	}



	for (i = nombreChunk - 3; i < nombreChunk - 1; i++) {//face visible
		for (j = 1; j < nombreChunk - 1; j++) {
			visibleFaceChunk(globalChunk, i, j);
		}
	}

	for (i = nombreChunk - 3; i < nombreChunk - 1; i++) {//sun
		for (j = 1; j < nombreChunk - 1; j++) {
			initSunLightChunk(globalChunk, i, j);
		}
	}

	for (n = 0; n <= NOMBRELIGHT; n++) {//cube light
		for (i = nombreChunk - 3; i < nombreChunk - 1; i++) {
			for (j = 1; j < nombreChunk - 1; j++) {
				initLightAndShadowChunkStep1(globalChunk, i, j, n);
			}
		}
	}

	for (i = nombreChunk - 3; i < nombreChunk - 1; i++) {//face light
		for (j = 1; j < nombreChunk - 1; j++) {
			initLightAndShadowChunkStep2(globalChunk, i, j);
		}
	}

	
	a = atlasPositionTab[0];
	b = atlasNormalTab[0];
	c = atlasShadowTab[0];
	d = tabSize[0];
	for (i = 0; i < nombreChunk - 3; i++) {
		atlasPositionTab[i] = atlasPositionTab[i+1];
		atlasNormalTab[i] = atlasNormalTab[i + 1];
		atlasShadowTab[i] = atlasShadowTab[i + 1];
		tabSize[i] = tabSize[i + 1];

	}
	atlasPositionTab[i] = a;
	atlasNormalTab[i] = b;
	atlasShadowTab[i] = c;
	tabSize[i] = d;

	

	for (i = nombreChunk -4; i < nombreChunk - 2; i++) {//free buffer
		for (j = 0; j < nombreChunk - 2; j++) {
			for (n = 0; j < NOMBRETEXTURE; j++) {
				free(atlasPositionTab[i][j][n]);
				free(atlasNormalTab[i][j][n]);
				free(atlasShadowTab[i][j][n]);
			}
		}
	}



	for (i = nombreChunk - 3; i<nombreChunk - 1; i++) {
		for (j = 1; j<nombreChunk - 1; j++) {
			tabSizeChunk(globalChunk, i, j);
		}
	}
	if (biggestChunkInWorld() == 1) {
		EM_ASM(initAtlasStableBuffer());
	}
	for (i = nombreChunk - 3; i < nombreChunk - 1; i++) {
		for (j = 1; j < nombreChunk - 1; j++) {
			allocAtlasBufferChunkStep2(i, j);
		}
	}

	for (i = nombreChunk - 3; i < nombreChunk - 1; i++) {
		for (j = 1; j < nombreChunk - 1; j++) {
			resetTabSizeChunk(i, j);
		}
	}


	for (i = nombreChunk - 3; i < nombreChunk - 1; i++) {
		for (j = 1; j < nombreChunk - 1; j++) {
			calculBufferChunk(globalChunk, i, j, playerPosAnchor[0] - playerPosAnchor0, playerPosAnchor[1] - playerPosAnchor0);
		}
	}


	EM_ASM(swapBufferChunkEndX());
	for (i = nombreChunk - 4; i < nombreChunk - 2; i++) {
		for (j = 0; j < nombreChunk - 2; j++) {
			indexTab[0] = i;
			indexTab[1] = j;
			indexTab[2] = 0;
			EM_ASM({ initAtlasBufferChunk($0, $1) }, i, j);
		}
	}


}

EMSCRIPTEN_KEEPALIVE Plan createPlan(int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3, int x4, int y4, int z4) {
	Plan res;

	res.a = (y1 - y2)*(z1 - z3) - (z1 - z2)*(y1 - y3);
	res.b = (z1 - z2)*(x1 - x3) - (x1 - x2)*(z1 - z3);
	res.c = (x1 - x2)*(y1 - y3) - (y1 - y2)*(x1 - x3);
	res.d = -(res.a*x1 + res.b*y1 + res.c*z1);
	if (res.d + res.a*x4 + res.b*y4 + res.c*z4 < 0) {
		res.a *= -1;
		res.b *= -1;
		res.c *= -1;
		res.d *= -1;

	}

	return res;
}

EMSCRIPTEN_KEEPALIVE float getPlayerPosX() {
	return playerPos[0];
}

EMSCRIPTEN_KEEPALIVE float getPlayerPosY() {
	return playerPos[1];
}

EMSCRIPTEN_KEEPALIVE float getPlayerPosZ() {
	return playerPos[2];
}

EMSCRIPTEN_KEEPALIVE float getPlayerPosAnchorX() {
	return playerPosAnchor[0];
}



EMSCRIPTEN_KEEPALIVE float getPlayerPosAnchorZ() {
	return playerPosAnchor[1];
}
