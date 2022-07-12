var gl;



function initGL(canvas) {
	try {
		gl = canvas.getContext("experimental-webgl", {
			premultipliedAlpha: true
		});
		gl.viewportWidth = canvas.width;
		gl.viewportHeight = canvas.height;
	} catch (e) {
	}
	if (!gl) {
		alert("Could not initialise WebGL, sorry :-(");
	}

	depth_texture_extension = gl.getExtension('WEBGL_depth_texture');
	if (!depth_texture_extension) {
	  console.log('This WebGL program requires the use of the ' +
	    'WEBGL_depth_texture extension. This extension is not supported ' +
	    'by your browser, so this WEBGL program is terminating.');
	  return;
	}
}

function drawALL() {
	gl.viewport(0, 0, gl.viewportWidth, gl.viewportHeight);
	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
	mat4.perspective(45, gl.viewportWidth/gl.viewportHeight, 0.1, 320.0, pMatrix);



	gl.blendFunc(gl.ONE, gl.ONE_MINUS_SRC_ALPHA);
	gl.enable(gl.DEPTH_TEST);
	gl.depthFunc(gl.LEQUAL);

	drawSky();
	gl.clear(gl.DEPTH_BUFFER_BIT);
	drawAtlas();
}


function drawSky() {
	var i;

	mat4.identity(mvMatrix);
	mat4.rotate(mvMatrix, degToRad(player.rot.x), [1, 0, 0]);
	mat4.rotate(mvMatrix, degToRad(player.rot.y), [0, 1, 0]);
	//mat4.translate(mvMatrix, [-player.pos.x, -player.pos.y, -player.pos.z]);
	setMatrixUniforms();


	gl.uniform1i(program.uUseLighting, false);



	//parce qu'il en fallait
	gl.bindBuffer(gl.ARRAY_BUFFER, atlasNormalBuffer[0][0][0]);
	gl.vertexAttribPointer(program.vertexNormalAttribute, atlasNormalBuffer[0][0][0].itemSize, gl.FLOAT, false, 0, 0);

	gl.bindBuffer(gl.ARRAY_BUFFER, atlasShadowBuffer[0][0][0]);
	gl.vertexAttribPointer(program.shadow, atlasShadowBuffer[0][0][0].itemSize, gl.FLOAT, false, 0, 0);
	//...

			

	for(i=0; i<6; i++) {
		gl.bindBuffer(gl.ARRAY_BUFFER, skyBoxPositionBuffer[i]);
		gl.vertexAttribPointer(program.vertexPositionAttribute, skyBoxPositionBuffer[i].itemSize, gl.FLOAT, false, 0, 0);


		gl.bindBuffer(gl.ARRAY_BUFFER, faceBufferColor);
		gl.vertexAttribPointer(program.textureCoordAttribute, faceBufferColor.itemSize, gl.FLOAT, false, 0, 0);


		gl.activeTexture(gl.TEXTURE0);
		gl.bindTexture(gl.TEXTURE_2D, skyTexture);
		gl.uniform1i(program.sampleUniform, 0);


		gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, faceBufferIndex);
		gl.drawElements(gl.TRIANGLES, faceBufferIndex.numItems, gl.UNSIGNED_SHORT, 0);


	}
}


function drawAtlas() {
	var i,j,k,l,m,n;


//	program = perFragmentShader;

	mat4.identity(mvMatrix);
	mat4.rotate(mvMatrix, degToRad(player.rot.x), [1, 0, 0]);
	mat4.rotate(mvMatrix, degToRad(player.rot.y), [0, 1, 0]);
	mat4.translate(mvMatrix, [-player.pos.x, -player.pos.y, -player.pos.z]);
	mat4.translate(mvMatrix, [-player.posAnchorX, 0, -player.posAnchorZ]);

	setMatrixUniforms();


	gl.uniform1i(program.uUseLighting, true);


	gl.uniform3f(
	program.ambientColorUniform,
		0.3,
		0.3,
		0.3
	);


	var lightingDirection = [
		-1.0,
		-1.0,
		-1.0
	];
	var adjustedLD = vec3.create();
	vec3.normalize(lightingDirection, adjustedLD);
	vec3.scale(adjustedLD, -1);
	gl.uniform3fv(program.lightingDirectionUniform, adjustedLD);
	gl.uniform3f(
	program.directionalColorUniform,
		0.006,
		0.006,
		0.006
	);




	gl.uniform3f( 
	program.pointLightingLocationUniform, 
	1000.5,
	1000.5,
	1000.5

	);
	gl.uniform3f( 
	program.pointLightingColorUniform, 
		0.0,
		0.0,
		0.0
	);

	gl.bindBuffer(gl.ARRAY_BUFFER, atlasTextureBuffer);
	gl.vertexAttribPointer(program.textureCoordAttribute, 2, gl.FLOAT, false, 0, 0);

	for(k=0; k<nombreTexture; k++) {
		for(i=0; i<nombreChunk; i++) {
			for(j=0; j<nombreChunk; j++) {
				gl.bindBuffer(gl.ARRAY_BUFFER, atlasPositionBuffer[i][j][k]);
				gl.vertexAttribPointer(program.vertexPositionAttribute, atlasPositionBuffer[i][j][k].itemSize, gl.FLOAT, false, 0, 0);

				gl.bindBuffer(gl.ARRAY_BUFFER, atlasNormalBuffer[i][j][k]);
				gl.vertexAttribPointer(program.vertexNormalAttribute, atlasNormalBuffer[i][j][k].itemSize, gl.FLOAT, false, 0, 0);

				gl.bindBuffer(gl.ARRAY_BUFFER, atlasShadowBuffer[i][j][k]);
				gl.vertexAttribPointer(program.shadow, atlasShadowBuffer[i][j][k].itemSize, gl.FLOAT, false, 0, 0);





				gl.activeTexture(gl.TEXTURE0);
				gl.bindTexture(gl.TEXTURE_2D, texturePack[k]);
				gl.uniform1i(program.sampleUniform, 0);
				


				gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, atlasIndiceBuffer);
				gl.drawElements(gl.TRIANGLES, nombreFace[i][j][k]*6, gl.UNSIGNED_SHORT, 0);
			}
		}
	}
}





function tick() {
	requestAnimFrame(tick);

	updatePosition();

	drawALL();
}

LIGHT = [8.5,
	65.5,
	8.5]
lightMatrix = mat4.create;
function webGLStart() {
	var canvas = document.getElementById("canvas");

	initGL(canvas);
	initALLShaders();

	console.log("wut");
//chunk = em_module.ChunkTransfer();

player.pos.x = _getPlayerPosX();
player.pos.y = _getPlayerPosY();
player.pos.z = _getPlayerPosZ();

player.posAnchorX = _getPlayerPosAnchorX();
player.posAnchorZ = _getPlayerPosAnchorZ();

nombreTexture = _nombreTextureTransfer();
nombreChunk = _nombreChunkTransfer();
/*/	initWorld();
	WorldGeneration();
	visibleFace();
	initLightAndShadow();*/
	
	initALLBuffers();/**/
		//initALLBuffers();

	initTexture();

	gl.clearColor(0, 0.58, 1.0, 1.0);
	gl.enable(gl.DEPTH_TEST);
	gl.enable(gl.BLEND);

	resizeCanvas();

//	tick();
}