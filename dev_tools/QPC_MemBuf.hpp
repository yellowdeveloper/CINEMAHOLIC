#ifndef QPC_MEMBUF_HPP
#define QPC_MEMBUF_HPP

#include <cstddef>
#include <cstdint>

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

void QpcMemoryLogger(size_t maxLogs);
void Record(uint32_t eventId, double value = 0.0);
void DumpQpcMemoryLogger();

#endif
