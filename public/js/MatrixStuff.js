var mvMatrix = mat4.create();
var mvMatrixStack = [];
var pMatrix = mat4.create();

function mvPushMatrix() {
	var copy = mat4.create();
	mat4.set(mvMatrix, copy);
	mvMatrixStack.push(copy);
}

function mvPopMatrix() {
	if (mvMatrixStack.length == 0) {
		throw "Invalid popMatrix!";
	}
	mvMatrix = mvMatrixStack.pop();
}


function setMatrixUniforms() {
	gl.uniformMatrix4fv(program.pMatrixUniform, false, pMatrix);
	gl.uniformMatrix4fv(program.mvMatrixUniform, false, mvMatrix);


	

	var normalMatrix = mat3.create(); 

	mat4.toMat3(pMatrix, normalMatrix);



	//mat3.transpose(normalMatrix); 
	gl.uniformMatrix3fv(program.nMatrixUniform, false, normalMatrix);
}
