#include <Siv3D.hpp>
#include <emscripten/websocket.h>

class WebSocket
{
    static EM_BOOL WebSocketOpen(int eventType, const EmscriptenWebSocketOpenEvent *e, void *userData)
    {
        Console << U"WebSocket Open";
        return 0;
    }

    static EM_BOOL WebSocketClose(int eventType, const EmscriptenWebSocketCloseEvent *e, void *userData)
    {
        Console << U"WebSocket Close";
        return 0;
    }

    static EM_BOOL WebSocketError(int eventType, const EmscriptenWebSocketErrorEvent *e, void *userData)
    {
        Console << U"WebSocket Error eventType=" << eventType;
        return 0;
    }

    static EM_BOOL WebSocketMessage(int eventType, const EmscriptenWebSocketMessageEvent *e, void *userData)
    {
        Console << U"WebSocket Message numBytes=" << e->numBytes;
        Console << U"Text:" << Unicode::FromUTF8(std::string((char *)e->data));

        /*if (e->isText)
        {
            Console << U"Text:" << Unicode::FromUTF8(std::string((char *)e->data));
        }
        else
        {
            const char numberToChar[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

            std::string text;
            for (int i = 0; i < e->numBytes; ++i)
            {
                text.push_back(numberToChar[*(e->data + i) >> 4]);
                text.push_back(numberToChar[*(e->data + i) & 0x0F]);
                text.push_back(' ');
            }

            Console << U"Binary:" << Unicode::FromUTF8(text);
        }*/

        return 0;
    }

    EMSCRIPTEN_WEBSOCKET_T socket;

public:
    WebSocket(const char *url)
    {
        EmscriptenWebSocketCreateAttributes attr;
        emscripten_websocket_init_create_attributes(&attr);
        attr.url = url;

        socket = emscripten_websocket_new(&attr);

        emscripten_websocket_set_onopen_callback(socket, this, WebSocketOpen);
        emscripten_websocket_set_onclose_callback(socket, this, WebSocketClose);
        emscripten_websocket_set_onerror_callback(socket, this, WebSocketError);
        emscripten_websocket_set_onmessage_callback(socket, this, WebSocketMessage);
    }

    void SendBinary(uint8_t *binary, size_t size)
    {
        emscripten_websocket_send_binary(socket, binary, size);
    }

    void SendText(const std::string &text)
    {
        emscripten_websocket_send_utf8_text(socket, text.c_str());
    }
};
