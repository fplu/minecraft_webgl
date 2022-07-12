
var zIsPressed = 0;
var sIsPressed = 0;
var qIsPressed = 0;
var dIsPressed = 0;
var wIsPressed = 0;
var spaceIsPressed = 0;
var keyDownIsPressed = 0;
var keyUpIsPressed = 0;
var keyLeftIsPressed = 0;
var keyRightIsPressed = 0;

window.addEventListener('keydown', function(e) {
	switch (e.keyCode){
	case 90 ://Z
		zIsPressed = sIsPressed + 1;
//		player.vit.z = -player.playerSpeed;
	break;
	case 83 ://S
		sIsPressed = zIsPressed + 1;
		// player.vit.z = player.playerSpeed;
		break;
	case 81 ://Q
		qIsPressed = dIsPressed + 1;
		// player.vit.x = -player.playerSpeed;
	break;
	case 68 ://D
		dIsPressed = qIsPressed + 1;
		// player.vit.x = player.playerSpeed;
	break;
	case 87 ://W
		wIsPressed = spaceIsPressed + 1;
		// player.vit.y = -player.playerSpeed;
	break;
	case 32 ://space
		spaceIsPressed =  wIsPressed + 1;
		// player.vit.y = player.playerSpeed;
	break;
	case 40 ://keydown
		keyDownIsPressed = keyUpIsPressed + 1;
		// player.rot.x+=player.rotationSpeed;
	break;
	case 38 ://keyup
		keyUpIsPressed =  keyDownIsPressed + 1;

		// player.rot.x-=player.rotationSpeed;
	break;
	case 37 ://keyleft
		keyLeftIsPressed = keyRightIsPressed + 1;

		// player.rot.y-=player.rotationSpeed;
	break;
	case 39 ://keyright

		keyRightIsPressed = keyLeftIsPressed + 1;
		// player.rot.y+=player.rotationSpeed;
	break;
	}
}, false);


window.addEventListener('keyup', function(e) {
	switch (e.keyCode){
	case 90 ://Z
		zIsPressed = 0;
//		player.vit.z = -player.playerSpeed;
	break;
	case 83 ://S
		sIsPressed = 0;
		// player.vit.z = player.playerSpeed;
		break;
	case 81 ://Q
		qIsPressed = 0
		// player.vit.x = -player.playerSpeed;
	break;
	case 68 ://D
		dIsPressed = 0;
		// player.vit.x = player.playerSpeed;
	break;
	case 87 ://W
		wIsPressed = 0;
		// player.vit.y = -player.playerSpeed;
	break;
	case 32 ://space
		spaceIsPressed = 0;
		// player.vit.y = player.playerSpeed;
	break;
		case 40 ://keydown
		keyDownIsPressed = 0;
		// player.rot.x+=player.rotationSpeed;
	break;
	case 38 ://keyup
		keyUpIsPressed = false;

		// player.rot.x-=player.rotationSpeed;
	break;
	case 37 ://keyleft
		keyLeftIsPressed = false;

		// player.rot.y-=player.rotationSpeed;
	break;
	case 39 ://keyright

		keyRightIsPressed = false;
		// player.rot.y+=player.rotationSpeed;
	break;
	}
}, false);
window.addEventListener('resize', resizeCanvas, false);
function resizeCanvas() {
	gl.viewportWidth = document.body.offsetWidth;
	gl.viewportHeight = document.body.offsetHeight;

	canvas.width = gl.viewportWidth;
	canvas.height = gl.viewportHeight;
}


window.addEventListener('keyleft', function(e) {       
}, false);
window.addEventListener('keyright', function(e) {

}, false);
window.addEventListener('z', function(e) {

}, false);

function updatePosition() {


	if(zIsPressed > sIsPressed) {
		player.vit.z = -player.playerSpeed;
	}
	else if(zIsPressed < sIsPressed) {
		player.vit.z = +player.playerSpeed;
	}
	else {
		player.vit.z = 0;
	}

	if(qIsPressed > dIsPressed) {
		player.vit.x = -player.playerSpeed;
	}
	else if(qIsPressed < dIsPressed) {
		player.vit.x = +player.playerSpeed;
	}
	else {
		player.vit.x = 0;
	}

	/*if(wIsPressed > spaceIsPressed) {
		player.vit.y = -40*player.playerSpeed;
	}
	else*/ if(wIsPressed < spaceIsPressed) {
		player.vit.y = 40*player.playerSpeed;
	}
	else {
		player.vit.y = 0;
	}

	if(keyDownIsPressed > keyUpIsPressed) {
		player.rot.x+=player.rotationSpeed;
		if(player.rot.x>90) {
			player.rot.x = 90;
		}
	}
	else if(keyDownIsPressed < keyUpIsPressed) {
		player.rot.x-=player.rotationSpeed;
		if(player.rot.x<-90) {
			player.rot.x = -90;
		}
	}

	if(keyLeftIsPressed > keyRightIsPressed) {
		player.rot.y-=player.rotationSpeed;
	}
	else if(keyLeftIsPressed < keyRightIsPressed) {
		player.rot.y+=player.rotationSpeed;
	}

	var tmpY = player.vit.y;
	player.vit.y = 0;
	player.vit = newNorme(player.vit, player.playerSpeed);
	var tmp = vec3.create([player.vit.x, player.vit.y, player.vit.z]);

		mat4.identity(mvMatrix);
	//mat4.rotate(mvMatrix, -degToRad(player.rot.x), [1, 0, 0]);
	mat4.rotate(mvMatrix, -degToRad(player.rot.y), [0, 1, 0]);
	tmp = mat4.multiplyVec3(mvMatrix, tmp, tmp);

		//var zorg = mat4.toMat3(mvMatrix);
	//console.log(mvMatrix[0]+", "+mvMatrix[1]+", "+mvMatrix[2])
/*	player.vit.x = zorg[0]*player.vit.x + zorg[1]*player.vit.y + zorg[2]*player.vit.z;
	player.vit.y = zorg[3]*player.vit.x + zorg[4]*player.vit.y + zorg[5]*player.vit.z;
	player.vit.z = zorg[6]*player.vit.x + zorg[7]*player.vit.y + zorg[9]*player.vit.z;*/
	player.vit.x = tmp[0];
	player.vit.y = tmpY;
	player.vit.z = tmp[2];




	//player.vit = newNorme(player.vit, player.playerSpeed);

Module.ccall('hitBoxWorld', 'number', ['number','number','number','number','number','number'], [player.pos.x,player.pos.y,player.pos.z,player.vit.x,player.vit.y,player.vit.z ])
		//player.pos = addPoint(player.pos, player.vit);
	
player.pos.x = _getPlayerPosX();
player.pos.y = _getPlayerPosY();
player.pos.z = _getPlayerPosZ();


player.posAnchorX = _getPlayerPosAnchorX();
player.posAnchorZ = _getPlayerPosAnchorZ();

//	player.vit.x = 0;
//	player.vit.y = 0;
//	player.vit.z = 0;

}
