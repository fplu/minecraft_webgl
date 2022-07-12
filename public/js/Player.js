function Player() {
	this.pos = new Point(100,70,100);
	this.posAnchorX = 0;
this.posAnchorZ = 0;

	this.vit = new Point(0,0,0);
	this.rot = new Point(0,0,0);
	this.playerSpeed = 0.005;
	this.rotationSpeed = 2;
}

player = new Player();