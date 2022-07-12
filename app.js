
var app = require('express')(),
	express = require('express'),
    server = require('http').createServer(app),
    io = require('socket.io').listen(server),
    ent = require('ent'), // pour une meilleur securite
    fs = require('fs');

	app.use(express.static(__dirname + '/public'));
// Chargement de la page index.html
app.get('/', function (req, res) {
    res.sendfile(__dirname + '/public/index.html');
});


server.listen(8080);