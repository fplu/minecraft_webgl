const cubeSize = 1;
const PI = 3.14;
const test = 10;
var World = [];

Arena = function(game) {
    // Appel des variables nécéssaires
    this.game = game;
    var scene = game.scene;

    // Création de notre lumière principale
    var light = new BABYLON.HemisphericLight("light1", new BABYLON.Vector3(0, 20, 0), scene);    
	
	lireWorld();
	optimizeWorld(scene);

};

function afficheWorld() {
	
}

function lireWorld() {
	for(var i=0; i<test; i++) {
		World.push([]);
		for(var j=0; j<test; j++) {
			World[i].push([]);
			for(var k=0; k<test; k++) {
				if(/*(Math.abs(Math.round(Math.random()*10))%4)*/0==0) {
					World[i][j].push(true);
				}
				else {
					World[i][j].push(false);
				}
			}
		}
	}
}

function optimizeWorld(scene)
{
	var mainMaterialWall = new BABYLON.StandardMaterial("wallTexture", scene);
    mainMaterialWall.diffuseTexture = new BABYLON.Texture("assets/images/wood.jpg", scene);
	mainMaterialWall.diffuseTexture.uScale = 1;
    mainMaterialWall.diffuseTexture.vScale = 1;
	mainMaterialWall.freeze(); //
	var mainFace = 	BABYLON.Mesh.CreateGround("mainFace", cubeSize, cubeSize, 1, scene);

	mainFace.convertToUnIndexedMesh(); //
	mainFace.position = new BABYLON.Vector3(-cubeSize*2,0,-cubeSize*2);
	mainFace.material = mainMaterialWall;
	
	var face = [];
var materialWall = [];
var compteurFace = 0;
	var saut = [-1,-1,-1,-1,-1,-1];
	var tmp = 1;
	
	for(var i=0; i<test; i++) {
		for(var j=0; j<test; j++) {
			for(var k=0; k<test; k++) {
				if(World[i][j][k]==true) {
					if(saut[0]<k && (i+1>=test || World[i+1][j][k]==false)) {//face du haut
						face.push(mainFace.clone("face"+compteurFace));
						materialWall.push(mainMaterialWall.clone("materialWall"+compteurFace));
						face[compteurFace].rotation = new BABYLON.Vector3(0,0,-PI/2);
						face[compteurFace].material = materialWall[compteurFace];
						saut[0] = k;
						tmp = 1;
						while(saut[0]+1<test && World[i][j][saut[0]+1]==true && (i+1>=test || World[i+1][j][saut[0]+1]==false)) {
							saut[0]++;
							tmp++;
						}
						face[compteurFace].scaling = new  BABYLON.Vector3(1,1,tmp);
						face[compteurFace].material.diffuseTexture.uScale = 1;
						face[compteurFace].material.diffuseTexture.vScale = tmp;
						face[compteurFace].position = new BABYLON.Vector3(i*cubeSize+cubeSize/2,j*cubeSize,cubeSize*(k+(tmp-1)/2));
						compteurFace++;
					}
					if(saut[1]<k && (i-1<0 || World[i-1][j][k]==false)) {//face du bas
						face.push(mainFace.clone("face"+compteurFace));
						materialWall.push(mainMaterialWall.clone("materialWall"+compteurFace));
						face[compteurFace].rotation = new BABYLON.Vector3(0,0,PI/2);
						face[compteurFace].material = materialWall[compteurFace];
						saut[1] = k;
						tmp = 1;
						while(saut[1]+1<test && World[i][j][saut[1]+1]==true && (i-1<0 || World[i-1][j][saut[1]+1]==false)) {
							saut[1]++;
							tmp++;
						}
						face[compteurFace].scaling = new  BABYLON.Vector3(1,1,tmp);
						face[compteurFace].material.diffuseTexture.uScale = 1;
						face[compteurFace].material.diffuseTexture.vScale = tmp;
						face[compteurFace].position = new BABYLON.Vector3(i*cubeSize-cubeSize/2,j*cubeSize,cubeSize*(k+(tmp-1)/2));
						compteurFace++;
					}
				}
				if(World[k][i][j]==true) {
					if(saut[2]<k && (i+1>=test || World[k][i+1][j]==false)) {//face du haut
						face.push(mainFace.clone("face"+compteurFace));
						materialWall.push(mainMaterialWall.clone("materialWall"+compteurFace));
						face[compteurFace].material = materialWall[compteurFace];
						saut[2] = k;
						tmp = 1;
						while(saut[2]+1<test && World[saut[2]+1][i][j]==true && (i+1>=test || World[saut[2]+1][i+1][j]==false)) {
							saut[2]++;
							tmp++;
						}
						face[compteurFace].scaling = new  BABYLON.Vector3(tmp,1,1);
						face[compteurFace].material.diffuseTexture.uScale = tmp;
						face[compteurFace].material.diffuseTexture.vScale = 1;
						face[compteurFace].position = new BABYLON.Vector3((k+(tmp-1)/2)*cubeSize,i*cubeSize+cubeSize/2,cubeSize*j);
						compteurFace++;
					}
					if(saut[3]<k && (i-1<0 || World[k][i-1][j]==false)) {//face du bas
						face.push(mainFace.clone("face"+compteurFace));
						materialWall.push(mainMaterialWall.clone("materialWall"+compteurFace));
						face[compteurFace].material = materialWall[compteurFace];
						saut[3] = k;
						tmp = 1;
						while(saut[3]+1<test && World[saut[3]+1][i][j]==true && (i-1<0 || World[saut[3]+1][i-1][j]==false)) {
							saut[3]++;
							tmp++;
						}
						face[compteurFace].scaling = new  BABYLON.Vector3(tmp,1,1);
						face[compteurFace].material.diffuseTexture.uScale = tmp;
						face[compteurFace].material.diffuseTexture.vScale = 1;
						face[compteurFace].rotation = new BABYLON.Vector3(PI,0,0);
						face[compteurFace].position = new BABYLON.Vector3((k+(tmp-1)/2)*cubeSize,i*cubeSize-cubeSize/2,cubeSize*j);
						compteurFace++;
					}
				}
				if(World[j][k][i]==true) {
					if(saut[4]<k && (i+1>=test || World[j][k][i+1]==false)) {//face du haut
						face.push(mainFace.clone("face"+compteurFace));
						materialWall.push(mainMaterialWall.clone("materialWall"+compteurFace));
						face[compteurFace].rotation = new BABYLON.Vector3(PI/2,0,0);
						face[compteurFace].material = materialWall[compteurFace];
						saut[4] = k;
						tmp = 1;
						while(saut[4]+1<test && World[j][saut[4]+1][i]==true && (i+1>=test || World[j][saut[4]+1][i+1]==false)) {
							saut[4]++;
							tmp++;
						}
						face[compteurFace].scaling = new  BABYLON.Vector3(1,1,tmp);
						face[compteurFace].material.diffuseTexture.uScale = 1;
						face[compteurFace].material.diffuseTexture.vScale = tmp;
						face[compteurFace].position = new BABYLON.Vector3(j*cubeSize,(k+(tmp-1)/2)*cubeSize,cubeSize*i+cubeSize/2);
						compteurFace++;
					}
					if(saut[5]<k && (i-1<0 || World[j][k][i-1]==false)) {//face du bas
						face.push(mainFace.clone("face"+compteurFace));
						materialWall.push(mainMaterialWall.clone("materialWall"+compteurFace));
						face[compteurFace].rotation = new BABYLON.Vector3(-PI/2,0,0);
						face[compteurFace].material = materialWall[compteurFace];
						saut[5] = k;
						tmp = 1;
						while(saut[5]+1<test && World[j][saut[5]+1][i]==true && (i-1<0 || World[j][saut[5]+1][i-1]==false)) {
							saut[5]++;
							tmp++;
						}
						face[compteurFace].scaling = new  BABYLON.Vector3(1,1,tmp);
						face[compteurFace].material.diffuseTexture.uScale = 1;
						face[compteurFace].material.diffuseTexture.vScale = tmp;
						face[compteurFace].position = new BABYLON.Vector3(j*cubeSize,(k+(tmp-1)/2)*cubeSize,cubeSize*i-cubeSize/2);
						compteurFace++;
					}
				}
			}
			for(var k=0; k<6; k++) {
				saut[k] = -1;
			}
		}
	}
							
}


