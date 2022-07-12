// Page enti�rement charg�, on lance le jeu
document.addEventListener("DOMContentLoaded", function () {
    new Game('renderCanvas');
}, false);

Game = function(canvasId) {
    // Canvas et engine d�fini ici
    var canvas = document.getElementById(canvasId);
    var engine = new BABYLON.Engine(canvas, false, null, false);
    var _this = this;
    
    // On initie la sc�ne avec une fonction associ� � l'objet Game
    this.scene = this._initScene(engine);
    
    var _player = new Player(_this, canvas);
    var _arena = new Arena(_this);
    // Permet au jeu de tourner
    engine.runRenderLoop(function () {
        _this.scene.render();
		_this.scene.getAnimationRatio();
	});

    // Ajuste la vue 3D si la fenetre est agrandi ou diminu�
    window.addEventListener("resize", function () {
        if (engine) {
            engine.resize();
        }
    },false);

};


Game.prototype = {
    // Prototype d'initialisation de la sc�ne
    _initScene : function(engine) {
        var scene = new BABYLON.Scene(engine);
        scene.clearColor=new BABYLON.Color3(0.9,0.9,0.9);
        return scene;
    }
};