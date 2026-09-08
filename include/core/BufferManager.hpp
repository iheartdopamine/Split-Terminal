#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <cstddef>

namespace split {

struct MemoryChunk {
    size_t address;
    uint8_t value;
    bool isCorrupted;
};

class BufferManager {
public:
    BufferManager(size_t bufferSize = 64);

    void generateRandomStream();
    void corruptRandomSector();
    bool splitAndPurge();
    
    [[nodiscard]] const std::vector<uint8_t>& getRawData() const { return m_buffer; }
    [[nodiscard]] size_t getSize() const { return m_buffer.size(); }
    [[nodiscard]] size_t getCorruptedCount() const;

private:
    std::vector<uint8_t> m_buffer;
};

} // namespace split