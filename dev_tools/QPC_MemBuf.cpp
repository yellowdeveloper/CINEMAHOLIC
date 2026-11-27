#include "QPC_MemBuf.hpp"

#include <windows.h>

#include <algorithm>
#include <atomic>
#include <cstdio>
#include <vector>

namespace {
struct LogEntry {
    int64_t qpcTimestamp;
    uint32_t eventId;
    DWORD threadId;
    double value;
};

struct LoggerState {
    std::vector<LogEntry> entries;
    std::atomic<size_t> writeIndex{0};
    size_t capacity = 0;
    int64_t frequency = 0;
    int64_t originTimestamp = 0;
};

LoggerState logger;

const char* EventName(uint32_t eventId) {
    switch (eventId) {
    case QPC_RENDER_LOCK_REQUEST: return "render_lock_request";
    case QPC_RENDER_LOCK_ACQUIRED: return "render_lock_acquired";
    case QPC_RENDER_START: return "render_start";
    case QPC_RENDER_END: return "render_end";
    case QPC_RENDER_LOCK_RELEASED: return "render_lock_released";
    case QPC_UPDATE_LOCK_REQUEST: return "update_lock_request";
    case QPC_UPDATE_LOCK_ACQUIRED: return "update_lock_acquired";
    case QPC_UPDATE_FUNC_START: return "update_func_start";
    case QPC_UPDATE_FUNC_END: return "update_func_end";
    case QPC_UPDATE_LOCK_RELEASED: return "update_lock_released";
    case QPC_SLEEP_START: return "sleep_start";
    case QPC_SLEEP_END: return "sleep_end";
    default: return "unknown";
    }
}
}

void QpcMemoryLogger(size_t maxLogs) {
    LARGE_INTEGER frequency;
    LARGE_INTEGER now;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&now);

    logger.entries.clear();
    logger.entries.resize(maxLogs);
    logger.capacity = logger.entries.size();
    logger.writeIndex.store(0, std::memory_order_relaxed);
    logger.frequency = frequency.QuadPart;
    logger.originTimestamp = now.QuadPart;
}

void Record(uint32_t eventId, double value) {
    const size_t index = logger.writeIndex.fetch_add(1, std::memory_order_relaxed);
    if (index >= logger.capacity) return;

    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    logger.entries[index] = { now.QuadPart, eventId, GetCurrentThreadId(), value };
}

void DumpQpcMemoryLogger() {
    const size_t count = (std::min)(logger.writeIndex.load(std::memory_order_relaxed), logger.capacity);
    std::sort(logger.entries.begin(), logger.entries.begin() + count, [](const LogEntry& a, const LogEntry& b) {
        return a.qpcTimestamp < b.qpcTimestamp;
    });

    FILE* file = nullptr;
    if (fopen_s(&file, "qpc_trace.csv", "w") != 0 || file == nullptr) {
        return;
    }

    fprintf(file, "time_ms,event,thread_id,value\n");
    for (size_t i = 0; i < count; ++i) {
        const double timeMs = (double)(logger.entries[i].qpcTimestamp - logger.originTimestamp) * 1000.0 / (double)logger.frequency;
        fprintf(file, "%.6f,%s,%lu,%.6f\n", timeMs, EventName(logger.entries[i].eventId), (unsigned long)logger.entries[i].threadId, logger.entries[i].value);
    }
    fclose(file);
}
