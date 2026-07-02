#ifndef ASYNC_CONTEXT_H
#define ASYNC_CONTEXT_H

/**
 * @file async_context.h
 * @brief Внутренний контекст одного подключения async.
 */

#include <cstddef>
#include <string>

#include "bulk_processor.h"
#include "command_reader.h"

/**
 * @brief Состояние одного подключения, созданного async::connect().
 *
 * Хранит независимый BulkProcessor и незавершённую строку между вызовами
 * receive(). Внешнему коду этот тип не виден: наружу возвращается только
 * async::handle_t.
 */
class AsyncContext {
public:
    /// Обработчик готового блока команд.
    using CommandBlockHandler = BulkProcessor::CommandBlockHandler;

    /// Источник времени для команд.
    using Clock = CommandReader::Clock;

    /**
     * @brief Создаёт контекст с заданным размером статического блока.
     *
     * @param block_size Размер статического блока команд.
     * @param command_block_handler Обработчик готовых блоков команд.
     * @param clock Источник времени для команд.
     */
    AsyncContext(std::size_t block_size,
                 CommandBlockHandler command_block_handler,
                 Clock clock = Clock{});

    /**
     * @brief Принимает очередной фрагмент входных данных.
     *
     * @param data Указатель на начало входного буфера.
     * @param size Размер входного буфера в байтах.
     */
    void receive(const char* data, std::size_t size);

    /**
     * @brief Завершает обработку текущего входного потока.
     */
    void disconnect();

private:
    /**
     * @brief Передаёт одну готовую строку в processor.
     *
     * @param line Строка команды или служебная строка `{` / `}`.
     */
    void process_line(const std::string& line);

    /// Processor, формирующий блоки команд для этого подключения.
    BulkProcessor m_processor;

    /// Источник времени получения команд.
    Clock m_clock;

    /// Накопленная часть строки, не завершённая символом перевода строки.
    std::string m_pending_line;
};

#endif  // ASYNC_CONTEXT_H
