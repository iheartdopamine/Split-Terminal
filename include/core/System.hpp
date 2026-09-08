#pragma once

#include <string>
#include <vector>

namespace split {

class System {
public:
    System();
    ~System() = default;

    void initialize();
    void run();

private:
    bool m_isRunning{false};
    std::string m_version{"0.1.0"};
    
    // Estado del sistema interactivo
    int m_selectedOption{0};
    int m_bufferHealth{100};
    int m_processedPackets{0};
    std::vector<std::string> m_logs;

    void addLog(const std::string& message);
};

} // namespace split