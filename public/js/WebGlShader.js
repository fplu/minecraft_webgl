

function getShader(gl, id) {
	var shaderScript = document.getElementById(id);
	if (!shaderScript) {
		return null;
	}

	var str = "";
	var k = shaderScript.firstChild;
	while (k) {
		if (k.nodeType == 3) {
			str += k.textContent;
		}
		k = k.nextSibling;
	}

	var shader;
	if (shaderScript.type == "x-shader/x-fragment") {
		shader = gl.createShader(gl.FRAGMENT_SHADER);
	} else if (shaderScript.type == "x-shader/x-vertex") {
		shader = gl.createShader(gl.VERTEX_SHADER);
	} else {
		return null;
	}

	gl.shaderSource(shader, str);
	gl.compileShader(shader);

	if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
		alert(gl.getShaderInfoLog(shader));
		return null;
	}

	return shader;
}

var perFragmentShader;
var perVertexShader;
var skyShader;
var program;


var shadowProgram;
var normalProgram;

function initALLShaders() {
	perFragmentShader = initShaders("per-fragment-lighting-fs", "per-fragment-lighting-vs");
	skyShader = initSkyShader("skyShader-fs", "skyShader-vs");
//	perVertexShader =  initShaders("per-vertex-lighting-fs", "per-vertex-lighting-vs");

//	shadowProgram =  initShaders("SHADOW_FSHADER_SOURCE", "SHADOW_VSHADER_SOURCE");
//	normalProgram =  initShaders("FSHADER_SOURCE", "VSHADER_SOURCE");

	program = perFragmentShader;
	gl.useProgram(program);
}

function initShaders(fragmentShader, vertexShader) {
	var fragmentShader = getShader(gl, fragmentShader);
	var vertexShader = getShader(gl, vertexShader);

	var shaderProgram = gl.createProgram();
	gl.attachShader(shaderProgram, vertexShader);
	gl.attachShader(shaderProgram, fragmentShader);
	gl.linkProgram(shaderProgram);

	if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
		alert("Could not initialise shaders");
	}

//	gl.useProgram(shaderProgram);

	shaderProgram.vertexPositionAttribute = gl.getAttribLocation(shaderProgram, "aVertexPosition");
	gl.enableVertexAttribArray(shaderProgram.vertexPositionAttribute);

	shaderProgram.vertexNormalAttribute = gl.getAttribLocation(shaderProgram, "aVertexNormal");
	gl.enableVertexAttribArray(shaderProgram.vertexNormalAttribute);

	shaderProgram.textureCoordAttribute = gl.getAttribLocation(shaderProgram, "aTextureCoord");
	gl.enableVertexAttribArray(shaderProgram.textureCoordAttribute);

	shaderProgram.shadow = gl.getAttribLocation(shaderProgram, "aShadow");
	gl.enableVertexAttribArray(shaderProgram.shadow);


	shaderProgram.pMatrixUniform = gl.getUniformLocation(shaderProgram, "uPMatrix");
	shaderProgram.mvMatrixUniform = gl.getUniformLocation(shaderProgram, "uMVMatrix");
	shaderProgram.nMatrixUniform = gl.getUniformLocation(shaderProgram, "uNMatrix");
	shaderProgram.samplerUniform = gl.getUniformLocation(shaderProgram, "uSampler");
	
	shaderProgram.uUseLighting = gl.getUniformLocation(shaderProgram, "uUseLighting");
	
	shaderProgram.ambientColorUniform = gl.getUniformLocation(shaderProgram, "uAmbientColor");
	shaderProgram.lightingDirectionUniform = gl.getUniformLocation(shaderProgram, "uLightingDirection");
	shaderProgram.directionalColorUniform = gl.getUniformLocation(shaderProgram, "uDirectionalColor");

	shaderProgram.pointLightingColorUniform =  gl.getUniformLocation(shaderProgram, "uPointLightingColor");
	shaderProgram.pointLightingLocationUniform =  gl.getUniformLocation(shaderProgram, "uPointLightingLocation");


//	shaderProgram.mvMatrixUniformFromLight = gl.getUniformLocation(shaderProgram, "uMVMatrixFromLight");
//	shaderProgram.shadowMap = gl.getUniformLocation(shaderProgram, "u_ShadowMap");

	return shaderProgram;
}

function initSkyShader(fragmentShader, vertexShader) {
	var fragmentShader = getShader(gl, fragmentShader);
	var vertexShader = getShader(gl, vertexShader);

	var shaderProgram = gl.createProgram();
	gl.attachShader(shaderProgram, vertexShader);
	gl.attachShader(shaderProgram, fragmentShader);
	gl.linkProgram(shaderProgram);

	if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
		alert("Could not initialise shaders");
	}


	shaderProgram.vertexPositionAttribute = gl.getAttribLocation(shaderProgram, "aVertexPosition");
	gl.enableVertexAttribArray(shaderProgram.vertexPositionAttribute);



	shaderProgram.textureCoordAttribute = gl.getAttribLocation(shaderProgram, "aTextureCoord");
	gl.enableVertexAttribArray(shaderProgram.textureCoordAttribute);





	shaderProgram.pMatrixUniform = gl.getUniformLocation(shaderProgram, "uPMatrix");
	shaderProgram.mvMatrixUniform = gl.getUniformLocation(shaderProgram, "uMVMatrix");
	shaderProgram.nMatrixUniform = gl.getUniformLocation(shaderProgram, "uNMatrix");
	shaderProgram.samplerUniform = gl.getUniformLocation(shaderProgram, "uSampler");
	
	//shaderProgram.uUseLighting = gl.getUniformLocation(shaderProgram, "uUseLighting");
	
	shaderProgram.ambientColorUniform = gl.getUniformLocation(shaderProgram, "uAmbientColor");
	shaderProgram.lightingDirectionUniform = gl.getUniformLocation(shaderProgram, "uLightingDirection");
	shaderProgram.directionalColorUniform = gl.getUniformLocation(shaderProgram, "uDirectionalColor");

	shaderProgram.pointLightingColorUniform =  gl.getUniformLocation(shaderProgram, "uPointLightingColor");
	shaderProgram.pointLightingLocationUniform =  gl.getUniformLocation(shaderProgram, "uPointLightingLocation");	


	return shaderProgram;
}