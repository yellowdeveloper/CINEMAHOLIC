#ifndef QPC_MEMBUF_HPP
#define QPC_MEMBUF_HPP

#include <windows.h>

#include <algorithm>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstdio>

enum QpcEvent : uint32_t {
    QPC_RENDER_LOCK_REQUEST = 0,
    QPC_RENDER_LOCK_ACQUIRED,
    QPC_RENDER_START,
    QPC_RENDER_END,
    QPC_RENDER_LOCK_RELEASED,
    QPC_UPDATE_LOCK_REQUEST,
    QPC_UPDATE_LOCK_ACQUIRED,
    QPC_UPDATE_FUNC_START,
    QPC_UPDATE_FUNC_END,
    QPC_UPDATE_LOCK_RELEASED,
    QPC_SLEEP_START,
    QPC_SLEEP_END
};

namespace qpc_mem_buf {
struct LogEntry {
    int64_t qpcTimestamp;
    uint32_t eventId;
    DWORD threadId;
    double value;
};

struct LoggerState {
    static constexpr size_t kMaxLogs = 1000000;
    LogEntry entries[kMaxLogs]{};
    std::atomic<size_t> writeIndex{0};
    size_t capacity = 0;
    int64_t frequency = 0;
    int64_t originTimestamp = 0;
};

inline LoggerState& State() {
    static LoggerState state;
    return state;
}

inline const char* EventName(uint32_t eventId) {
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

inline void QpcMemoryLogger(size_t maxLogs) {
    qpc_mem_buf::LoggerState& state = qpc_mem_buf::State();
    LARGE_INTEGER frequency;
    LARGE_INTEGER now;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&now);
    state.frequency = frequency.QuadPart;
    state.originTimestamp = now.QuadPart;
    state.capacity = maxLogs < qpc_mem_buf::LoggerState::kMaxLogs
        ? maxLogs
        : qpc_mem_buf::LoggerState::kMaxLogs;
    state.writeIndex.store(0, std::memory_order_relaxed);
}

inline void Record(uint32_t eventId, double value = 0.0) {
    qpc_mem_buf::LoggerState& state = qpc_mem_buf::State();
    const size_t index = state.writeIndex.fetch_add(1, std::memory_order_relaxed);
    if (index >= state.capacity) return;

    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    state.entries[index] = { now.QuadPart, eventId, GetCurrentThreadId(), value };
}

inline void DumpQpcMemoryLogger() {
    qpc_mem_buf::LoggerState& state = qpc_mem_buf::State();
    const size_t count = (std::min)(state.writeIndex.load(std::memory_order_relaxed), state.capacity);
    std::sort(state.entries, state.entries + count, [](const qpc_mem_buf::LogEntry& a, const qpc_mem_buf::LogEntry& b) {
        return a.qpcTimestamp < b.qpcTimestamp;
    });

    FILE* file = nullptr;
    if (fopen_s(&file, "qpc_trace.csv", "w") != 0 || file == nullptr) {
        return;
    }

    fprintf(file, "time_ms,event,thread_id,value\n");
    for (size_t i = 0; i < count; ++i) {
        const double timeMs = (double)(state.entries[i].qpcTimestamp - state.originTimestamp) * 1000.0 / (double)state.frequency;
        fprintf(file, "%.6f,%s,%lu,%.6f\n", timeMs, qpc_mem_buf::EventName(state.entries[i].eventId), (unsigned long)state.entries[i].threadId, state.entries[i].value);
    }
    fclose(file);
}

#endif
