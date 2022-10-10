#include <Siv3D.hpp>
#include "WebSocket.hpp"

EM_JS(int, GetCanvasWidth, (), { return canvas.width; });
EM_JS(int, GetCanvasHeight, (), { return canvas.height; });

void Main()
{
    Console << U"Hello World!";

    WebSocket webSocket("wss://snjp.me/server");

    {
        JSON json;
        json[U"msg"] = U"Hello World!";
        webSocket.SendText(json.formatUTF8Minimum());
    }

    Scene::SetBackground(ColorF{0.8, 0.9, 1.0});
    const Font font{60};

    std::map<String, s3d::Point> m_users;
    String uid;

    while (System::Update())
    {
        Window::Resize(GetCanvasWidth(), GetCanvasHeight());

        while (webSocket.hasReceivedText())
        {
            JSON json = JSON::Parse(Unicode::FromUTF8(webSocket.getReceivedTextAndPopFromBuffer()));
            if (json[U"type"].getString() == U"whoami")
            {
                uid = json[U"uid"].getString();
            }
            else if (json[U"uid"].getString() != uid)
            {
                auto uid = json[U"uid"].getString();
                auto cursor = json[U"cursor"].getOpt<s3d::Point>();
                if (cursor)
                {
                    m_users[uid] = cursor.value();
                }

                if (json[U"type"].getString() == U"close")
                {
                    m_users[uid].clear();
                }
            }
        }

        Circle{Cursor::Pos(), 40}.draw(ColorF{1, 0, 0, 0.5});

        for (const auto &user : m_users)
            Circle{user.second, 40}.draw(ColorF{1, 0, 0, 0.25});

        //if (KeyC.down())
        {
            JSON json;
            json[U"cursor"] = Cursor::Pos();
            webSocket.SendText(json.formatUTF8Minimum());
        }

        if (KeyA.down())
        {
            JSON json;
            json[U"msg"] = U"Key A!";
            webSocket.SendText(json.formatUTF8Minimum());
        }
    }
}
