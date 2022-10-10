// Echo Server

var shortid = require('shortid');

var ws = require('ws');
var server = new ws.Server({ port: 3000 });

server.on('connection', socket => {
    console.log('connection start')

    socket.send(shortid.generate(), { binary: false });

    socket.on('message', message => {
        console.log('Receive:');
        console.log(message);

        server.clients.forEach(client => {
            client.send(message, { binary: false });
        });
    });

    socket.on('close', () => {
        console.log('connection close');
    });
});
