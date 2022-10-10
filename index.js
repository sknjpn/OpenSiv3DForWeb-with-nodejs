// Echo Server

var shortid = require('shortid');

var ws = require('ws');
var server = new ws.Server({ port: 3000 });

server.on('connection', socket => {
    console.log('connection start')

    socket.uid = shortid.generate();
    socket.send(JSON.stringify({ uid: socket.uid, type: 'whoami' }), { binary: false });

    socket.on('message', message => {
        console.log('Receive:');
        console.log(message);

        try {
            const obj = JSON.parse(message);
            obj.uid = socket.uid;
            obj.type = 'message';
            server.clients.forEach(client => {
                client.send(JSON.stringify(obj), { binary: false });
            });
        }
        catch (e) {
            console.log(e);
        }
    });

    socket.on('close', () => {
        console.log('connection close');

        server.clients.forEach(client => client.send(JSON.stringify({ uid: socket.uid, type: 'close' }), { binary: false }));
    });
});
