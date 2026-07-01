#ifndef ASYNC_INTERNAL_H
#define ASYNC_INTERNAL_H

/**
 * @file async_internal.h
 * @brief Внутренние настройки async.
 */

#include "command_reader.h"

namespace async {
namespace detail {

/// Источник времени для обработки входных команд.
using Clock = CommandReader::Clock;

/**
 * @brief Временно подменяет clock для async.
 *
 * Используется приложением для сохранения тестируемости run_app(). Публичный
 * интерфейс библиотеки остаётся в async.h.
 */
class ScopedSettings {
public:
    explicit ScopedSettings(Clock clock);
    ~ScopedSettings();

    ScopedSettings(const ScopedSettings&) = delete;
    ScopedSettings& operator=(const ScopedSettings&) = delete;

private:
    Clock m_previous_clock;
};

}  // namespace detail
}  // namespace async

#endif  // ASYNC_INTERNAL_H
