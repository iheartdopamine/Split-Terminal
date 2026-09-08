#include "core/System.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

namespace split {

using namespace ftxui;

System::System() : m_bufferManager(std::make_unique<BufferManager>(64)) {}

void System::initialize() {
    m_isRunning = true;
    std::iostream::sync_with_stdio(false);
    addLog("SPLIT Core Protocol initialized.");
    addLog("Binary Buffer Inspector Ready [64 Bytes].");
}

void System::addLog(const std::string& message) {
    if (m_logs.size() >= 5) {
        m_logs.erase(m_logs.begin());
    }
    m_logs.push_back("> " + message);
}

std::string System::formatHexView() {
    std::ostringstream ss;
    const auto& raw = m_bufferManager->getRawData();
    for (size_t i = 0; i < raw.size(); ++i) {
        if (i > 0 && i % 16 == 0) ss << "\n";
        ss << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(raw[i]) << " ";
    }
    return ss.str();
}

void System::run() {
    if (!m_isRunning) return;

    auto screen = ScreenInteractive::TerminalOutput();

    std::vector<std::string> entries = {
        "[1] INSPECT BINARY STREAM",
        "[2] INJECT SECTOR CORRUPTION",
        "[3] SPLIT & PURGE STREAM (CLEAN)",
        "[4] ABORT & EXIT PROTOCOL"
    };

    MenuOption menu_options;
    menu_options.on_enter = [&]() {
        switch (m_selectedOption) {
            case 0:
                addLog("Stream inspect OK. Corrupted bytes: " + std::to_string(m_bufferManager->getCorruptedCount()));
                break;
            case 1:
                m_bufferManager->corruptRandomSector();
                addLog("WARNING: Synthetic corruption injected into stream.");
                break;
            case 2:
                if (m_bufferManager->splitAndPurge()) {
                    m_processedPackets += 1;
                    addLog("SUCCESS: Stream split and corrupted bytes purged.");
                } else {
                    addLog("Stream nominal. No corrupt sectors detected.");
                }
                break;
            case 3:
                addLog("Terminating protocol...");
                screen.ExitLoopClosure()();
                break;
        }
    };

    auto menu = Menu(&entries, &m_selectedOption, menu_options);

    auto renderer = Renderer(menu, [&]() {
        Elements log_elements;
        for (const auto& log : m_logs) {
            log_elements.push_back(color(Color::GrayLight, text(log)));
        }

        size_t corrupted = m_bufferManager->getCorruptedCount();

        Element corruptedElement;
        if (corrupted > 0) {
            corruptedElement = color(Color::Red, text(std::to_string(corrupted)));
        } else {
            corruptedElement = color(Color::Cyan, text(std::to_string(corrupted)));
        }
        corruptedElement = bold(corruptedElement);

        return border(vbox({
            bold(color(Color::Green, text("=================================================="))),
            bold(color(Color::Green, text("    S.P.L.I.T. // BINARY DATA INTEGRITY SYSTEM    "))),
            bold(color(Color::Green, text("=================================================="))),
            separator(),
            hbox({
                bold(text(" OPERATOR STATUS: ")),
                bold(color(Color::Green, text("ONLINE"))),
                bold(text("  |  STREAMS SPLIT: ")),
                bold(color(Color::Yellow, text(std::to_string(m_processedPackets)))),
                bold(text("  |  CORRUPTED SECTORS: ")),
                corruptedElement,
            }),
            separator(),
            hbox({
                // Columna del Menú (Ancho fijo o proporcional)
                border(vbox({
                    bold(color(Color::Cyan, text("COMMAND MENU"))),
                    separator(),
                    menu->Render(),
                })) | size(WIDTH, GREATER_THAN, 38),
                
                // Columna del Inspector Hexadecimal
                border(vbox({
                    bold(color(Color::Yellow, text("HEXADECIMAL MEMORY INSPECTOR"))),
                    separator(),
                    color(Color::GreenLight, paragraph(formatHexView())),
                })) | flex
            }),
            separator(),
            border(vbox({
                bold(color(Color::GrayDark, text("SYSTEM LOGS"))),
                vbox(std::move(log_elements)),
            })),
            separator(),
            dim(text("Use UP/DOWN arrows to navigate | ENTER to execute action")),
        }));
    });

    screen.Loop(renderer);
}

} // namespace split