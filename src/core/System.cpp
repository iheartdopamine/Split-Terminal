#include "core/System.hpp"
#include <iostream>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

namespace split {

using namespace ftxui;

System::System() = default;

void System::initialize() {
    m_isRunning = true;
    std::iostream::sync_with_stdio(false);
    addLog("SPLIT Protocol initialized.");
    addLog("Waiting for operator command...");
}

void System::addLog(const std::string& message) {
    if (m_logs.size() >= 6) {
        m_logs.erase(m_logs.begin());
    }
    m_logs.push_back("> " + message);
}

void System::run() {
    if (!m_isRunning) return;

    auto screen = ScreenInteractive::TerminalOutput();

    // Opciones del menú interactivo
    std::vector<std::string> entries = {
        "[1] INSPECT BINARY BUFFER",
        "[2] SPLIT DATA STREAM",
        "[3] PURGE CORRUPTED SECTORS",
        "[4] ABORT & EXIT PROTOCOL"
    };

    // Componente Menú
    MenuOption menu_options;
    menu_options.on_enter = [&]() {
        switch (m_selectedOption) {
            case 0:
                addLog("Inspecting sector 0x" + std::to_string(rand() % 9000 + 1000) + "... Buffer clear.");
                break;
            case 1:
                m_processedPackets += 5;
                addLog("Splitting stream... 5 packets processed.");
                break;
            case 2:
                if (m_bufferHealth < 100) {
                    m_bufferHealth = std::min(100, m_bufferHealth + 15);
                    addLog("Buffer purged. Stability restored.");
                } else {
                    addLog("Buffer already at maximum capacity.");
                }
                break;
            case 3:
                addLog("Terminating session...");
                screen.ExitLoopClosure()();
                break;
        }
    };

    auto menu = Menu(&entries, &m_selectedOption, menu_options);

    // Renderizador principal del Layout
    auto renderer = Renderer(menu, [&]() {
        // Renderizar logs de sistema
        Elements log_elements;
        for (const auto& log : m_logs) {
            log_elements.push_back(text(log) | color(Color::GrayLight));
        }

        return vbox({
            text("==================================================") | bold | color(Color::Green),
            text("    S.P.L.I.T. // BINARY DATA INTEGRITY SYSTEM    ") | bold | color(Color::Green),
            text("==================================================") | bold | color(Color::Green),
            separator(),
            hbox({
                text(" OPERATOR STATUS: ") | bold,
                text("ONLINE") | color(Color::Green) | bold,
                text("  |  PACKETS PROCESSED: ") | bold,
                text(std::to_string(m_processedPackets)) | color(Color::Yellow) | bold,
                text("  |  BUFFER: ") | bold,
                text("[" + std::to_string(m_bufferHealth) + "%]") | color(m_bufferHealth > 30 ? Color::Cyan : Color::Red) | bold,
            }),
            separator(),
            hbox({
                vbox({
                    text("COMMAND MENU") | bold | color(Color::Cyan),
                    separator(),
                    menu->Render(),
                }) | border | flex,
                vbox({
                    text("SYSTEM LOGS") | bold | color(Color::Yellow),
                    separator(),
                    vbox(std::move(log_elements)),
                }) | border | flex,
            }),
            separator(),
            text("Use UP/DOWN arrows to navigate | ENTER to execute action") | dim,
        }) | border;
    });

    screen.Loop(renderer);
}

} // namespace split