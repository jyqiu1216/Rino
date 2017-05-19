var wsUrl = "ws://localhost:12341";
var ws = null;

function sendMessageWS(message) {
	
    if (ws != null) {
        ws.send(message);
	}
	else{
		
	}
}



function initWebSocket(ip, port) {
    try {
        if (typeof MozWebSocket == 'function')
            WebSocket = MozWebSocket;

        if (ws && ws.readyState == 1)
            ws.close();

        // ≥ı ºªØ WebSocket  "ws://localhost:12341"
		var url = "ws://" + ip + ":" + port;
        ws = new WebSocket(url);

        ws.onopen = function (event) {
            debug("CONNECTED");
        };
        ws.onclose = function (event) {
            debug("DISCONNECTED");
        };
        ws.onmessage = function (event) {
            handleWebSocketMsg(event.data);
        };
        ws.onerror = function (event) {
            debug('ERROR: ' + event.data);
        };
    } catch (exception) {
        debug('ERROR: ' + exception);
    }
}

function stopWebSocket() {
    if (ws)
        ws.close();
}

function checkSocket() {
    if (ws != null) {
        var stateStr;
        switch (ws.readyState) {
            case 0: {
                stateStr = "CONNECTING";
                break;
            }
            case 1: {
                stateStr = "OPEN";
                break;
            }
            case 2: {
                stateStr = "CLOSING";
                break;
            }
            case 3: {
                stateStr = "CLOSED";
                break;
            }
            default: {
                stateStr = "UNKNOW";
                break;
            }
        }
        debug("WebSocket state = " + ws.readyState + " ( " + stateStr + " )");
    } else {
        debug("WebSocket is null");
    }
}

function handleWebSocketMsg(message) {
    
}


initWebSocket("localhost", "12341")


