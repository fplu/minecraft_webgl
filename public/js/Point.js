function Point(x, y, z) {
	this.x = x;
	this.y = y;
	this.z = z;
}

function distance(p1, p2) {
	return (p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y)+(p1.z-p2.z)*(p1.z-p2.z);
}

function norme(p) {
	return p.x*p.x+p.y*p.y+p.z*p.z;
}


function newNorme(p, newnorme) {
	var oldNorme = norme(p);
	if(oldNorme == 0) {
		return p;
	}

	p.x = p.x*newnorme/oldNorme;
	p.y = p.y*newnorme/oldNorme;
	p.z = p.z*newnorme/oldNorme;

	return p;
}

function addPoint(p1, p2) {
	p1.x+=p2.x;
	p1.y+=p2.y;
	p1.z+=p2.z;

	return p1;
}