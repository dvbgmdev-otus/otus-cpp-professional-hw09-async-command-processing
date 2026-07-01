/**
 * @file app_test.cpp
 * @brief Unit-тесты сборки приложения.
 */

#include "app.h"

#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>

class AppTest : public ::testing::Test {
protected:
    void SetUp() override { remove_log_files(); }

    void TearDown() override { remove_log_files(); }

    std::time_t next_time() { return 100 + m_clock_calls++; }

    int run_with_input(const char* const argv[], int argc, const std::string& text) {
        input.str(text);
        input.clear();
        return run_app(
            argc, argv, AppStreams{ input, output, error }, [this]() { return next_time(); });
    }

    std::string read_async_log_file(std::time_t timestamp) const {
        for (std::size_t index = 1; index <= max_test_log_index; ++index) {
            const std::string file_name =
                "bulk" + std::to_string(timestamp) + "_" + std::to_string(index) + ".log";
            std::ifstream input_file(file_name);
            if (input_file.is_open()) {
                std::ostringstream content;
                content << input_file.rdbuf();
                return content.str();
            }
        }
        return {};
    }

    void remove_log_files() const {
        for (std::time_t timestamp = 100; timestamp < 120; ++timestamp) {
            const std::string file_name = "bulk" + std::to_string(timestamp) + ".log";
            std::remove(file_name.c_str());
            for (std::size_t index = 1; index <= max_test_log_index; ++index) {
                const std::string postfixed_file_name = "bulk" + std::to_string(timestamp) + "_" +
                                                       std::to_string(index) + ".log";
                std::remove(postfixed_file_name.c_str());
            }
        }
    }

    std::istringstream input;
    std::ostringstream output;
    std::ostringstream error;

private:
    static const std::size_t max_test_log_index = 100;

    std::size_t m_clock_calls = 0;
};

#if (1)  // 1. Успешная обработка команд

// 1.1 Приложение выводит полный статический блок в консоль и файл.
TEST_F(AppTest, Run_WhenStaticBlockComplete_WritesConsoleAndFile) {
    const char* argv[] = { "bulk", "3" };
    const int exit_code = run_with_input(argv, 2, "cmd1\ncmd2\ncmd3\n");
    EXPECT_EQ(0, exit_code);
    EXPECT_EQ("bulk: cmd1, cmd2, cmd3\n", output.str());
    EXPECT_TRUE(error.str().empty());
    EXPECT_EQ("bulk: cmd1, cmd2, cmd3\n", read_async_log_file(100));
}

// 1.2 Приложение завершает неполный статический блок при конце ввода.
TEST_F(AppTest, Run_WhenInputEnds_WritesIncompleteStaticBlock) {
    const char* argv[] = { "bulk", "3" };
    const int exit_code = run_with_input(argv, 2, "cmd1\ncmd2\n");
    EXPECT_EQ(0, exit_code);
    EXPECT_EQ("bulk: cmd1, cmd2\n", output.str());
    EXPECT_TRUE(error.str().empty());
    EXPECT_EQ("bulk: cmd1, cmd2\n", read_async_log_file(100));
}

// 1.3 Приложение обрабатывает динамический блок и завершает предыдущий статический.
TEST_F(AppTest, Run_WhenDynamicBlockUsed_WritesStaticAndDynamicBlocks) {
    const char* argv[] = { "bulk", "3" };
    const int exit_code = run_with_input(argv, 2, "cmd1\ncmd2\n{\ncmd3\ncmd4\n}\n");
    EXPECT_EQ(0, exit_code);
    EXPECT_EQ("bulk: cmd1, cmd2\nbulk: cmd3, cmd4\n", output.str());
    EXPECT_TRUE(error.str().empty());
    EXPECT_EQ("bulk: cmd1, cmd2\n", read_async_log_file(100));
    EXPECT_EQ("bulk: cmd3, cmd4\n", read_async_log_file(103));
}

#endif

#if (1)  // 2. Ошибки запуска

// 2.1 Ошибка аргументов возвращает ненулевой код и пишет сообщение в stderr.
TEST_F(AppTest, Run_WhenCliArgsInvalid_ReturnsError) {
    const char* argv[] = { "bulk" };
    const int exit_code = run_with_input(argv, 1, "cmd1\n");
    EXPECT_NE(0, exit_code);
    EXPECT_TRUE(output.str().empty());
    EXPECT_FALSE(error.str().empty());
}

#endif
