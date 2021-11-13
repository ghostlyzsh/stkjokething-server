// delete file later

const socket = new WebSocket("ws://127.0.0.1:8443");

function chatMsg(value) {
    socket.send(value);
}

socket.onopen = (e) => {
    console.log("Connected successfully");
}

socket.onmessage = (e) => {
    console.log("Message Recieved: " + e.data);
    var chat = document.getElementById('chat');
    chat.innerText += '> ' + e.data + '\n';
}
