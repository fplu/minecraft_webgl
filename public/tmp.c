
EMSCRIPTEN_KEEPALIVE actuChunkXEnd() {
	int i, j, n;


	for (j = 1; j < nombreChunk - 1; j++) {//libère
		freeBlock(globalChunk[1][j].block);
	}

	for (i = 1; i < nombreChunk - 2; i++) {//décale, ATTENTION -2
		for (j = 1; j < nombreChunk - 1; j++) {
			globalChunk[i][j] = globalChunk[i + 1][j];
		}
	}

	for (j = 1; j < nombreChunk - 1; j++) {//realloc
		globalChunk[i][j].initBlock();
		determineReferencePChunk(globalChunk, i, j);
	}

	for (j = 1; j < nombreChunk - 1; j++) {//terra forme
		createChunkRelief(globalChunk, i, j);
	}

	for (j = 0; j < nombreChunk; j++) {//put grass
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

	tabSizeWorld(globalChunk);
	for (i = 1; i < nombreChunk - 1; i++) {//free buffer
		for (j = 1; j < nombreChunk - 1; j++) {
			for (n = 0; j < NOMBRETEXTURE; j++) {
				free(atlasPositionTab[i][j][n]);
				free(atlasPositionTab[i][j][n]);
				free(atlasPositionTab[i][j][n]);
			}
		}
	}

	
	tabSizeWorld(chunk);
	allocAtlasBufferWorldStep2();
	resetTabSizeWorld();
	calculBufferWorld(chunk);
	biggestChunkInWorld();
	/*for (i = nombreChunk - 3; i < nombreChunk - 1; i++) {//alloc new buffer
		for (j = 1; j < nombreChunk - 1; j++) {
			allocAtlasBufferChunkStep2(i, j);
		}
	}

	for (i = nombreChunk - 3; i < nombreChunk - 1; i++) {//reset tabBuffer
		for (j = 1; j < nombreChunk - 1; j++) {
			resetTabSizeChunk(i, j);
		}
	}


	for (i = nombreChunk - 3; i < nombreChunk - 1; i++) {
		for (j = 1; j < nombreChunk - 1; j++) {
			calculBufferChunk(chunk, i, j);
		}
	}*/



}