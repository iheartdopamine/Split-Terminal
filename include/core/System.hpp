#pragma once

#include <string>
#include <vector>
#include <memory>
#include "core/BufferManager.hpp"

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
    
    int m_selectedOption{0};
    int m_processedPackets{0};
    std::vector<std::string> m_logs;

    std::unique_ptr<BufferManager> m_bufferManager;

    void addLog(const std::string& message);
    std::string formatHexView();
};

} // namespace split