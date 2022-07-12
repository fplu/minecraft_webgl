
var skyBoxPositionBuffer = [];
var skyBoxTextureBuffer = [];
var skyBoxIndiceBuffer = [];

var atlasPositionBuffer = [];
var atlasNormalBuffer = [];
var atlasTextureBuffer;
var atlasIndiceBuffer;
var atlasShadowBuffer = [];

var nombreFace = [];
var biggestChunk = [0,0,0];

var faceBufferColor;
var faceBufferIndex;
var faceBufferNormalPosition = [];


function initALLBuffers() {
	initOtherBuffer();
	initSkyBox();
	allocAtlasBuffer();
	initAtlasBuffer();
}

function swapBufferChunkEndX() {
	var tmpA, tmpB, tmpC, tmpD;
	var i, j;

	tmpA = atlasPositionBuffer[0];
	tmpB = atlasNormalBuffer[0];
	tmpC = atlasShadowBuffer[0];
	tmpD = nombreFace[0];

	for(i=0; i<nombreChunk-1; i++) {
			atlasPositionBuffer[i] = atlasPositionBuffer[i+1];
			atlasNormalBuffer[i] = atlasNormalBuffer[i+1];
			atlasShadowBuffer[i] = atlasShadowBuffer[i+1];
			nombreFace[i] = nombreFace[i+1];
	}
	atlasPositionBuffer[nombreChunk-1] = tmpA;
	atlasNormalBuffer[nombreChunk-1] = tmpB;
	atlasShadowBuffer[nombreChunk-1] = tmpC;
	nombreFace[nombreChunk-1] = tmpD;

}

function initAtlasBufferChunk(i, j) {
	var k,l, tabSize;

	for(k=0; k<nombreTexture; k++) {


		_resetSousIndex();
		

		tabSize =  _getAtlasTabSize();
	//	console.log("a : "+tabSize+" i="+i+" j="+j);
		nombreFace[i][j][k] =
		 tabSize;
		atlasPositionTab = new Float32Array(tabSize*12);	
		atlasNormalTab = new Float32Array(tabSize*12);	
		atlasShadowTab = new Float32Array(tabSize*4);	


		for(l=0; l<tabSize*12; l++) {
			atlasPositionTab[l] = _getAtlasPositionTabIndex();
			atlasNormalTab[l] = _getAtlasNormalTabIndex();
		}


		for(l=0; l<tabSize*4; l++) {
			atlasShadowTab[l] = _getAtlasShadowTabIndex();
		}

		gl.bindBuffer(gl.ARRAY_BUFFER, atlasPositionBuffer[i][j][k]);
		gl.bufferData(gl.ARRAY_BUFFER, atlasPositionTab, gl.STATIC_DRAW);
		atlasPositionBuffer[i][j][k].itemSize = 3;
		atlasPositionBuffer[i][j][k].numItems = 4*tabSize;
		

		gl.bindBuffer(gl.ARRAY_BUFFER, atlasNormalBuffer[i][j][k]);
		gl.bufferData(gl.ARRAY_BUFFER, atlasNormalTab, gl.STATIC_DRAW);
		atlasNormalBuffer[i][j][k].itemSize = 3;
		atlasNormalBuffer[i][j][k].numItems = 4*tabSize;


		gl.bindBuffer(gl.ARRAY_BUFFER, atlasShadowBuffer[i][j][k]);
		gl.bufferData(gl.ARRAY_BUFFER, atlasShadowTab, gl.STATIC_DRAW);
		atlasShadowBuffer[i][j][k].itemSize = 1;
		atlasShadowBuffer[i][j][k].numItems = 4*tabSize;

		_growIndex();

	}
}

function initOtherBuffer() {
	initNormalVecteur();
	initTextureCoord();
	initVertexIndice();
}


function initSkyBox() {
	var a = 1;
	var b = -a;
	var i;
	var positionTab = [
		[b, b, b,
		 b, a, b,
		 b, a, a,
		 b, b, a],

		[a, b, b,
		 a, a, b,
		 a, a, a,
		 a, b, a],

		[b, b, b,
		 b, b, a,
		 a, b, a,
		 a, b, b],

		[b, a, b,
		 b, a, a,
		 a, a, a,
		 a, a, b],

		[b, b, b,
		 b, a, b,
		 a, a, b,
		 a, b, b],

		[b, b, a,
		 b, a, a,
		 a, a, a,
		 a, b, a]];

	for(i=0; i<6; i++) {
		skyBoxPositionBuffer.push(gl.createBuffer());
		gl.bindBuffer(gl.ARRAY_BUFFER, skyBoxPositionBuffer[i]);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(positionTab[i]), gl.STATIC_DRAW);
		skyBoxPositionBuffer[i].itemSize = 3;
		skyBoxPositionBuffer[i].numItems = 4;
	}
}


function allocAtlasBuffer() {
	var i, j, k;
	
	atlasTextureBuffer	= gl.createBuffer();
	atlasIndiceBuffer = gl.createBuffer();

	for(i=0; i<nombreChunk; i++) {
		atlasPositionBuffer.push([]);
		atlasNormalBuffer.push([]);
		atlasShadowBuffer.push([]);
		nombreFace.push([]);

		for(j=0; j<nombreChunk; j++) {
			atlasPositionBuffer[i].push([]);
			atlasNormalBuffer[i].push([]);
			atlasShadowBuffer[i].push([]);
			nombreFace[i].push([]);

			for(k=0; k<nombreTexture; k++) {
				atlasPositionBuffer[i][j].push(gl.createBuffer());
				atlasNormalBuffer[i][j].push(gl.createBuffer());
				atlasShadowBuffer[i][j].push(gl.createBuffer());
				nombreFace[i][j].push(0);

			}	
		}
	}
}

function initAtlasStableBuffer() {
		var i,j,k,l,m,n,o, tmp;
	var position;
	var	atlasPositionTab,
		atlasNormalTab,
		atlasTextureTab,
		atlasShadowTab,
		atlasIndiceTab;

	var textureCoords = [
	0.0, 1.0,
	0.0, 0.0,
	1.0, 0.0,
	1.0, 1.0];


	var cubeVertexIndices = [0, 1, 2,  0, 2, 3];




	var tabSize = _tabSizeMax();


	atlasIndiceTab = new Uint16Array(tabSize*6);	
	for(i=0; i<tabSize+1; i++) {
		for(j=0; j<6; j++) {
			atlasIndiceTab[i*6+j] = cubeVertexIndices[j] + 4 * i;
		}
	}
	gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, atlasIndiceBuffer);
	gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, atlasIndiceTab, gl.STATIC_DRAW);
	atlasIndiceBuffer.itemSize = 1;
	atlasIndiceBuffer.numItems = tabSize*6;



	atlasTextureTab = new Float32Array(tabSize*8);	
	for(i=0; i<tabSize+1; i++) {
		for(j=0; j<8; j++) {
			atlasTextureTab[i*8+j] = textureCoords[j];
		}
	}
	gl.bindBuffer(gl.ARRAY_BUFFER, atlasTextureBuffer);
	gl.bufferData(gl.ARRAY_BUFFER, atlasTextureTab, gl.STATIC_DRAW);
	atlasTextureBuffer.itemSize = 2;
	atlasTextureBuffer.numItems = 4*tabSize;

}

function initAtlasBuffer() {
	var i,j,k,l,m,n,o, tmp;
	var position;
	var	atlasPositionTab,
		atlasNormalTab,
		atlasTextureTab,
		atlasShadowTab,
		atlasIndiceTab;

	var textureCoords = [
	0.0, 1.0,
	0.0, 0.0,
	1.0, 0.0,
	1.0, 1.0];


	var cubeVertexIndices = [0, 1, 2,  0, 2, 3];




	var tabSize = _tabSizeMax();


	atlasIndiceTab = new Uint16Array(tabSize*6);	
	for(i=0; i<tabSize+1; i++) {
		for(j=0; j<6; j++) {
			atlasIndiceTab[i*6+j] = cubeVertexIndices[j] + 4 * i;
		}
	}
	gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, atlasIndiceBuffer);
	gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, atlasIndiceTab, gl.STATIC_DRAW);
	atlasIndiceBuffer.itemSize = 1;
	atlasIndiceBuffer.numItems = tabSize*6;



	atlasTextureTab = new Float32Array(tabSize*8);	
	for(i=0; i<tabSize+1; i++) {
		for(j=0; j<8; j++) {
			atlasTextureTab[i*8+j] = textureCoords[j];
		}
	}
	gl.bindBuffer(gl.ARRAY_BUFFER, atlasTextureBuffer);
	gl.bufferData(gl.ARRAY_BUFFER, atlasTextureTab, gl.STATIC_DRAW);
	atlasTextureBuffer.itemSize = 2;
	atlasTextureBuffer.numItems = 4*tabSize;


	_resetIndex();


	for(i=0; i<nombreChunk; i++) {
		for(j=0; j<nombreChunk; j++) {
			for(k=0; k<nombreTexture; k++) {


				_resetSousIndex();
				

				tabSize =  _getAtlasTabSize();
				nombreFace[i][j][k] = tabSize;
				atlasPositionTab = new Float32Array(tabSize*12);	
				atlasNormalTab = new Float32Array(tabSize*12);	
				atlasShadowTab = new Float32Array(tabSize*4);	


				for(l=0; l<tabSize*12; l++) {
					atlasPositionTab[l] = _getAtlasPositionTabIndex();
					atlasNormalTab[l] = _getAtlasNormalTabIndex();
				}


				for(l=0; l<tabSize*4; l++) {
					atlasShadowTab[l] = _getAtlasShadowTabIndex();
				}


				gl.bindBuffer(gl.ARRAY_BUFFER, atlasPositionBuffer[i][j][k]);
				gl.bufferData(gl.ARRAY_BUFFER, atlasPositionTab, gl.STATIC_DRAW);
				atlasPositionBuffer[i][j][k].itemSize = 3;
				atlasPositionBuffer[i][j][k].numItems = 4*tabSize;
				


				gl.bindBuffer(gl.ARRAY_BUFFER, atlasNormalBuffer[i][j][k]);
				gl.bufferData(gl.ARRAY_BUFFER, atlasNormalTab, gl.STATIC_DRAW);
				atlasNormalBuffer[i][j][k].itemSize = 3;
				atlasNormalBuffer[i][j][k].numItems = 4*tabSize;



				gl.bindBuffer(gl.ARRAY_BUFFER, atlasShadowBuffer[i][j][k]);
				gl.bufferData(gl.ARRAY_BUFFER, atlasShadowTab, gl.STATIC_DRAW);
				atlasShadowBuffer[i][j][k].itemSize = 1;
				atlasShadowBuffer[i][j][k].numItems = 4*tabSize;

				_growIndex();
			}
		}
	}
}



function initTextureCoord() {
	var i, j;

	var textureCoords = [
	1.0, 0.0,
	1.0, 1.0,
	0.0, 1.0,
	0.0, 0.0];

	faceBufferColor = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, faceBufferColor);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(textureCoords), gl.STATIC_DRAW);
	faceBufferColor.itemSize = 2;
	faceBufferColor.numItems = 4;
}


function initNormalVecteur() {
	var i;
	var vertexNormals = [
	[-1.0,  0.0,  0.0,
	-1.0,  0.0,  0.0,
	-1.0,  0.0,  0.0,
	-1.0,  0.0,  0.0],

	[1.0,  0.0,  0.0,
	1.0,  0.0,  0.0,
	1.0,  0.0,  0.0,
	1.0,  0.0,  0.0],

	[0.0, -1.0,  0.0,
	0.0,  -1.0,  0.0,
	0.0,  -1.0,  0.0,
	0.0,  -1.0,  0.0],

	[0.0, 1.0,  0.0,
	0.0, 1.0,  0.0,
	0.0, 1.0,  0.0,
	0.0, 1.0,  0.0],

	[0.0,  0.0,  -1.0,
	0.0,  0.0,  -1.0,
	0.0,  0.0,  -1.0,
	0.0,  0.0,  -1.0],

	[0.0,  0.0, 1.0,
	0.0,  0.0, 1.0,
	0.0,  0.0, 1.0,
	0.0,  0.0, 1.0]
	];
	for(i=0; i<6; i++) {
		faceBufferNormalPosition.push(gl.createBuffer());
		gl.bindBuffer(gl.ARRAY_BUFFER, faceBufferNormalPosition[i]);

		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertexNormals[i]), gl.STATIC_DRAW);
		faceBufferNormalPosition[i].itemSize = 3;
		faceBufferNormalPosition[i].numItems = 24;
	}
}

function initVertexIndice() {
	var cubeVertexIndices = [0, 1, 2,      0, 2, 3];

	faceBufferIndex = gl.createBuffer();
	gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, faceBufferIndex);

	gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(cubeVertexIndices), gl.STATIC_DRAW);
	faceBufferIndex.itemSize = 1;
	faceBufferIndex.numItems = 6;
}