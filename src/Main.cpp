#include <Siv3D.hpp>
#include "WebSocket.hpp"

EM_JS(int, GetCanvasWidth, (), { return canvas.width; });
EM_JS(int, GetCanvasHeight, (), { return canvas.height; });

void Main()
{
    Console << U"Hello World!";

    WebSocket webSocket("wss://snjp.me/server");

    Scene::SetBackground(ColorF{0.8, 0.9, 1.0});

    const Font font{60};

    while (System::Update())
    {
        Window::Resize(GetCanvasWidth(), GetCanvasHeight());

        font(U"Hello, Siv3D!").drawAt(Scene::Center(), Palette::Black);

        Circle{Cursor::Pos(), 40}.draw(ColorF{1, 0, 0, 0.5});

        if (KeyA.down())
        {
            webSocket.SendText("Hello");
        }
    }
}
