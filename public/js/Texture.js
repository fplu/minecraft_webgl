var texturePack = Array();
var skyTexture;
var nombreTexture;
//var textureLight = Array();
//var nombreLight = 9;



function handleLoadedTexture(texture) {
	gl.pixelStorei(gl.UNPACK_PREMULTIPLY_ALPHA_WEBGL, true);
	gl.bindTexture(gl.TEXTURE_2D, texture);
	gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, texture.image);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
	gl.bindTexture(gl.TEXTURE_2D, null);
}



function initTexture() {
	var i;
	nombreTexture = _nombreTextureTransfer();

	for(i=0; i<nombreTexture; i++) {
		var texture = gl.createTexture(); 
		texture.image = new Image; 
		texturePack.push(texture);
	}

	texturePack[0].image.onload = function () {
		handleLoadedTexture(texturePack[0])
	}
	texturePack[0].image.src = "assets/images/stone.png";


	texturePack[1].image.onload = function () {
		handleLoadedTexture(texturePack[1])
	}
	texturePack[1].image.src = "assets/images/dirt.png";


	texturePack[2].image.onload = function () {
		handleLoadedTexture(texturePack[2])
	}
	texturePack[2].image.src = "assets/images/grass.png";

	texturePack[3].image.onload = function () {
		handleLoadedTexture(texturePack[3])
	}
	texturePack[3].image.src = "assets/images/grass2.png";

	texturePack[4].image.onload = function () {
		handleLoadedTexture(texturePack[4])
	}
	texturePack[4].image.src = "assets/images/sand.png";

	texturePack[5].image.onload = function () {
		handleLoadedTexture(texturePack[5])
	}
	texturePack[5].image.src = "assets/images/water.png";


	texturePack[6].image.onload = function () {
		handleLoadedTexture(texturePack[6])
	}
	texturePack[6].image.src = "assets/images/glowstone.png";

	texturePack[7].image.onload = function () {
		handleLoadedTexture(texturePack[7])
	}
	texturePack[7].image.src = "assets/images/trunk.png";


	texturePack[8].image.onload = function () {
		handleLoadedTexture(texturePack[8])
	}
	texturePack[8].image.src = "assets/images/trunk2.png";

	texturePack[9].image.onload = function () {
		handleLoadedTexture(texturePack[9])
	}
	texturePack[9].image.src = "assets/images/foliage.png";

	skyTexture = gl.createTexture(); 
	skyTexture.image = new Image;
	skyTexture.image.onload = function () {
		handleLoadedTexture(skyTexture)
	}
	skyTexture.image.src = "assets/images/skyBoxDay.png";

/*	for(i=0; i<nombreLight; i++) {
		var texture = gl.createTexture(); 
		texture.image = new Image; 
		textureLight.push(texture);
	}
	textureLight[0].image.onload = function () {
		handleLoadedTexture(textureLight[0])
	}
	textureLight[0].image.src = "assets/images/test26.png";

	textureLight[1].image.onload = function () {
		handleLoadedTexture(textureLight[1])
	}
	textureLight[1].image.src = "assets/images/test51.png";

	textureLight[2].image.onload = function () {
		handleLoadedTexture(textureLight[2])
	}
	textureLight[2].image.src = "assets/images/test77.png";

	textureLight[3].image.onload = function () {
		handleLoadedTexture(textureLight[3])
	}
	textureLight[3].image.src = "assets/images/test102.png";

	textureLight[4].image.onload = function () {
		handleLoadedTexture(textureLight[4])
	}
	textureLight[4].image.src = "assets/images/test128.png";

	textureLight[5].image.onload = function () {
		handleLoadedTexture(textureLight[5])
	}
	textureLight[5].image.src = "assets/images/test154.png";

	textureLight[6].image.onload = function () {
		handleLoadedTexture(textureLight[6])
	}
	textureLight[6].image.src = "assets/images/test179.png";

	textureLight[7].image.onload = function () {
		handleLoadedTexture(textureLight[7])
	}
	textureLight[7].image.src = "assets/images/test205.png";

	textureLight[8].image.onload = function () {
		handleLoadedTexture(textureLight[8])
	}
	textureLight[8].image.src = "assets/images/test230.png";*/


}
