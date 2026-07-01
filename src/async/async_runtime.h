#ifndef ASYNC_RUNTIME_H
#define ASYNC_RUNTIME_H

/**
 * @file async_runtime.h
 * @brief Runtime вывода готовых блоков async.
 */

#include <iosfwd>

#include "command_block.h"

/**
 * @brief Выполняет вывод готовых блоков команд.
 *
 * Текущая реализация синхронная. Позже этот класс станет владельцем очередей и
 * worker-потоков для консольного и файлового вывода.
 */
class AsyncRuntime {
public:
    /**
     * @brief Возвращает общий runtime async-библиотеки.
     */
    static AsyncRuntime& instance();

    /**
     * @brief Передаёт готовый блок в подсистему вывода.
     *
     * @param block Готовый блок команд.
     */
    void publish(const CommandBlock& block);

    /**
     * @brief Подменяет поток консольного вывода.
     *
     * Используется временно для тестируемости приложения.
     */
    void set_output(std::ostream& output);

    /**
     * @brief Возвращает текущий поток консольного вывода.
     */
    std::ostream& output() const;

private:
    AsyncRuntime();

    /// Поток для консольного вывода.
    std::ostream* m_output;
};

#endif  // ASYNC_RUNTIME_H
