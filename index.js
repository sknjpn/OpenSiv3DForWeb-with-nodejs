console.log('Hello World!');

var ws = require('ws');
var server = new ws.Server({ port: 3000, path: 'server' });

server.on('connection', ws => {

    ws.on('message', message => {
        console.log(message);

        server.clients.forEach(client => {
            client.send(message);
        });
    });

    ws.on('close', () => {
        console.log('close');
    });
});
