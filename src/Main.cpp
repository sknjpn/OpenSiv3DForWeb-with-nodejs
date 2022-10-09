#include <Siv3D.hpp>
#include <emscripten/websocket.h>

EM_JS(int, GetCanvasWidth, (), { return canvas.width; });
EM_JS(int, GetCanvasHeight, (), { return canvas.height; });

EM_BOOL WebSocketOpen(int eventType, const EmscriptenWebSocketOpenEvent *e, void *userData)
{
    Console << U"WebSocket Open";
    return 0;
}

EM_BOOL WebSocketClose(int eventType, const EmscriptenWebSocketCloseEvent *e, void *userData)
{
    Console << U"WebSocket Close";
    return 0;
}

EM_BOOL WebSocketError(int eventType, const EmscriptenWebSocketErrorEvent *e, void *userData)
{
    Console << U"WebSocket Error eventType=" << eventType;
    return 0;
}

EM_BOOL WebSocketMessage(int eventType, const EmscriptenWebSocketMessageEvent *e, void *userData)
{
    Console << U"WebSocket Message numBytes=" << e->numBytes;

    if (e->isText)
    {
        Console << U"Text:" << Unicode::FromUTF8(std::string((char *)e->data));
    }
    else
    {
        const char numberToChar[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

        std::string text;
        for (int i = 0; i < e->numBytes; ++i)
        {
            text.push_back(numberToChar[*(e->data + i) >> 8]);
            text.push_back(numberToChar[*(e->data + i) & 0x0F]);
            text.push_back(' ');
        }

        Console << U"Binary:" << Unicode::FromUTF8(text);
    }

    return 0;
}

void Main()
{
    Console << U"Hello World!";

    EmscriptenWebSocketCreateAttributes attr;
    emscripten_websocket_init_create_attributes(&attr);
    attr.url = "wss://snjp.me/server";
    EMSCRIPTEN_WEBSOCKET_T socket = emscripten_websocket_new(&attr);

    emscripten_websocket_set_onopen_callback(socket, nullptr, WebSocketOpen);
    emscripten_websocket_set_onclose_callback(socket, nullptr, WebSocketClose);
    emscripten_websocket_set_onerror_callback(socket, nullptr, WebSocketError);
    emscripten_websocket_set_onmessage_callback(socket, nullptr, WebSocketMessage);

    // emscripten_websocket_send_utf8_text(socket, "hello world!");
    // char data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    // emscripten_websocket_send_binary(socket, data, sizeof(data));

    Scene::SetBackground(ColorF{0.8, 0.9, 1.0});

    const Font font{60};

    while (System::Update())
    {
        Window::Resize(GetCanvasWidth(), GetCanvasHeight());

        font(U"Hello, Siv3D!").drawAt(Scene::Center(), Palette::Black);

        Circle{Cursor::Pos(), 40}.draw(ColorF{1, 0, 0, 0.5});

        if (KeyA.down())
        {
            emscripten_websocket_send_utf8_text(socket, "こんにちは!");
            emscripten_websocket_send_utf8_text(socket, "Hi!");
        }

        if (KeyC.down())
        {
            char data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            emscripten_websocket_send_binary(socket, data, sizeof(data));
        }
    }
}
