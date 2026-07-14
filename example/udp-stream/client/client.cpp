#include <array>
#include <atomic>
#include <boost/asio.hpp>
#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <raylib.h>
#include <thread>

using boost::asio::ip::udp;

std::atomic<bool> isRunning;
std::array<uint16_t, 160*120> rx_buff = {{0}};

class CameraClient {
    public:
        CameraClient(boost::asio::io_context& ctx)
            : sckt_(ctx, udp::endpoint(udp::v4(), 4444))
        {
            start_rx();
        }
    private:
        void start_rx() {
            sckt_.async_receive_from(
                boost::asio::buffer(rx_buff),
                server_ep_,
                std::bind(&CameraClient::handle_rx, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred
                )
            );
        }
        void handle_rx(const boost::system::error_code& err, std::size_t rx_ed) {
            if (err) {
                std::cerr << err.message() << std::endl;
            }

            if (isRunning) {
                start_rx();
            }
        }

        udp::socket sckt_;
        udp::endpoint server_ep_;
};

void udpClientTask() {
    try {
        boost::asio::io_context io_ctx;
        
        CameraClient client(io_ctx);

        io_ctx.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void windowTask() {
    const int screenWidth = 640;
    const int screenHeight = 480;

    InitWindow(screenWidth, screenHeight, "UDP CAMERA CLIENT");
    SetTargetFPS(30);

    struct Image img = {
        (void*)(rx_buff.data()),
        160,
        120,
        1,
        PIXELFORMAT_UNCOMPRESSED_R5G6B5
    };

    Texture2D texture = LoadTextureFromImage(img);

    bool shouldClose = false;

    while (!shouldClose) {
        if (IsKeyReleased(KEY_Q)) {
            shouldClose = true;
        }
        BeginDrawing();
            UpdateTexture(texture, rx_buff.data());
            DrawTextureEx(texture, (Vector2){0.0,0.0}, 0.0, 4.0, WHITE);
        EndDrawing();
    }

    UnloadTexture(texture);

    CloseWindow();
    isRunning = false;
}

int main(int argc, char** argv) {
    isRunning = true;
    std::thread client(udpClientTask);
    std::thread win(windowTask);
    win.join();
    client.join();

    return 0;
}
