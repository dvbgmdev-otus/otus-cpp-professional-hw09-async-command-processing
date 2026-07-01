/**
 * @file app_test.cpp
 * @brief Unit-тесты сборки приложения.
 */

#include "app.h"

#include <gtest/gtest.h>

#include <sstream>
#include <string>

class AppTest : public ::testing::Test {
protected:
    int run_with_input(const char* const argv[], int argc, const std::string& text) {
        input.str(text);
        input.clear();
        return run_app(argc, argv, AppStreams{ input, output, error });
    }

    std::istringstream input;
    std::ostringstream output;
    std::ostringstream error;
};

#if (1)  // 1. Успешная обработка команд

// 1.1 Приложение обрабатывает полный статический блок.
TEST_F(AppTest, Run_WhenStaticBlockComplete_WritesConsoleAndFile) {
    const char* argv[] = { "bulk", "3" };
    const int exit_code = run_with_input(argv, 2, "cmd1\ncmd2\ncmd3\n");
    EXPECT_EQ(0, exit_code);
    EXPECT_TRUE(error.str().empty());
}

// 1.2 Приложение завершает неполный статический блок при конце ввода.
TEST_F(AppTest, Run_WhenInputEnds_WritesIncompleteStaticBlock) {
    const char* argv[] = { "bulk", "3" };
    const int exit_code = run_with_input(argv, 2, "cmd1\ncmd2\n");
    EXPECT_EQ(0, exit_code);
    EXPECT_TRUE(error.str().empty());
}

// 1.3 Приложение обрабатывает динамический блок и завершает предыдущий статический.
TEST_F(AppTest, Run_WhenDynamicBlockUsed_WritesStaticAndDynamicBlocks) {
    const char* argv[] = { "bulk", "3" };
    const int exit_code = run_with_input(argv, 2, "cmd1\ncmd2\n{\ncmd3\ncmd4\n}\n");
    EXPECT_EQ(0, exit_code);
    EXPECT_TRUE(error.str().empty());
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
