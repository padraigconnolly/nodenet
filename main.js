const express = require('express')
const app = express();
var http = require('http').createServer(app);
var io = require('socket.io')(http);

// Mongodb Init
// TODO: Add Yaml config to provide server details (Not PW)
/*var MongoClient = require('mongodb').MongoClient
f = require('util').format;
var database;
const username = "test123"
export MYPASS=tester
const password = process.env.MYPASS;
var url = f('mongodb://%s:%s@<mongodb-host>:27017',
    username, password);
*/
// Static Web Init
app.use(express.static(__dirname + '/public'));
app.use('/cytoscape', express.static(__dirname + '/node_modules/cytoscape/dist'));
app.use('/cytoscape-edgehandles', express.static(__dirname + '/node_modules/cytoscape-edgehandles'));
app.use('/popperjs', express.static(__dirname + '/node_modules/@popperjs'));
app.use('/cytoscape-popper', express.static(__dirname + '/node_modules/cytoscape-popper'));
app.use('/lodash', express.static(__dirname + '/node_modules/lodash'));
app.get('/', (req, res) => {
    res.sendFile(__dirname + '/home.html')
});

// Connect To DB
/*MongoClient.connect(url, { useUnifiedTopology: true }, function(err, db) {
    if (err) {
        console.log('Unable to connect to database, bad address or login details?');
        throw err;
    };
    console.log('Connected to Mongodb Database!');
    database = db.db("mydb");
*/
http.listen(8000, () => {
  console.log('Node Net app can be logged into at localhost:8000')
});

//});

io.on('connection', (socket) => {
    console.log('a user connected');

    socket.on('event', (msg) => {
        console.log(msg);
    });

    socket.on('disconnect', () => {
        console.log('user disconnected');
    });


});
