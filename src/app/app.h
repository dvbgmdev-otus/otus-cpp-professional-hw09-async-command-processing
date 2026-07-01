#ifndef APP_H
#define APP_H

/**
 * @file app.h
 * @brief Оркестрация запуска приложения bulk.
 */

#include <iosfwd>

/**
 * @brief Набор потоков ввода-вывода приложения.
 * @ingroup app_group
 *
 * Используется для отделения логики приложения от глобальных std::cin,
 * std::cout и std::cerr, что упрощает тестирование.
 */
struct AppStreams {
    /// Входной поток с командами.
    std::istream& input;

    /// Выходной поток для готовых блоков команд.
    std::ostream& output;

    /// Поток для сообщений об ошибках запуска.
    std::ostream& error;
};

/**
 * @brief Запускает приложение bulk.
 * @ingroup app_group
 *
 * Разбирает аргументы командной строки, создаёт async-контекст и передаёт в
 * него данные из входного потока до EOF.
 *
 * @param argc Количество аргументов командной строки.
 * @param argv Массив аргументов командной строки.
 * @param streams Потоки ввода-вывода приложения.
 * @return Код завершения приложения: 0 при успехе, ненулевое значение при
 * ошибке запуска.
 */
int run_app(int argc, const char* const argv[], AppStreams streams);

#endif  // APP_H
