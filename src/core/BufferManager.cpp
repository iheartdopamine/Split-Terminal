#include "core/BufferManager.hpp"
#include <random>
#include <algorithm>

namespace split {

BufferManager::BufferManager(size_t bufferSize) : m_buffer(bufferSize, 0x00) {
    generateRandomStream();
}

void BufferManager::generateRandomStream() {
    std::random_device rd;
    std::mt19937 gen(rd());
    // Usamos 'int' en lugar de 'uint8_t' para ser 100% compatible con MSVC
    std::uniform_int_distribution<int> dist(0x20, 0x7E);

    for (auto& byte : m_buffer) {
        byte = static_cast<uint8_t>(dist(gen));
    }
}

void BufferManager::corruptRandomSector() {
    if (m_buffer.empty()) return;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, m_buffer.size() - 1);

    m_buffer[dist(gen)] = 0xFF;
}

size_t BufferManager::getCorruptedCount() const {
    return static_cast<size_t>(std::count(m_buffer.begin(), m_buffer.end(), 0xFF));
}

bool BufferManager::splitAndPurge() {
    size_t corruptedBefore = getCorruptedCount();
    
    m_buffer.erase(std::remove(m_buffer.begin(), m_buffer.end(), 0xFF), m_buffer.end());

    std::random_device rd;
    std::mt19937 gen(rd());
    // Usamos 'int' en lugar de 'uint8_t'
    std::uniform_int_distribution<int> dist(0x30, 0x39);
    
    while (m_buffer.size() < 64) {
        m_buffer.push_back(static_cast<uint8_t>(dist(gen)));
    }

    return corruptedBefore > 0;
}

} // namespace split