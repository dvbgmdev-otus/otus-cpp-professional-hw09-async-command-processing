/**
 * @file cli_args_test.cpp
 * @brief Unit-тесты разбора аргументов командной строки.
 */

#include "cli_args.h"

#include <gtest/gtest.h>

#include <string>

#if (1)  // 1. Успешный разбор аргументов

// 1.1 Положительное целое число задает размер статического блока.
TEST(CliArgsTest, Parse_WhenPositiveIntegerPassed_ReturnsBlockSize) {
    const char* argv[] = { "bulk", "3" };

    const CliArgsParseResult result = parse_cli_args(2, argv);

    EXPECT_TRUE(result.success);
    EXPECT_EQ(3U, result.block_size);
    EXPECT_TRUE(result.error.empty());
}

#endif  // Успешный разбор аргументов

#if (1)  // 2. Ошибки количества аргументов

// 2.1 Отсутствие размера блока возвращает ошибку.
TEST(CliArgsTest, Parse_WhenBlockSizeMissing_ReturnsError) {
    const char* argv[] = { "bulk" };

    const CliArgsParseResult result = parse_cli_args(1, argv);

    EXPECT_FALSE(result.success);
    EXPECT_EQ(0U, result.block_size);
    EXPECT_FALSE(result.error.empty());
}

// 2.2 Лишний аргумент возвращает ошибку.
TEST(CliArgsTest, Parse_WhenExtraArgumentPassed_ReturnsError) {
    const char* argv[] = { "bulk", "3", "extra" };

    const CliArgsParseResult result = parse_cli_args(3, argv);

    EXPECT_FALSE(result.success);
    EXPECT_EQ(0U, result.block_size);
    EXPECT_FALSE(result.error.empty());
}

#endif  // Ошибки количества аргументов

#if (1)  // 3. Ошибки значения размера блока

// 3.1 Нулевой размер блока возвращает ошибку.
TEST(CliArgsTest, Parse_WhenZeroPassed_ReturnsError) {
    const char* argv[] = { "bulk", "0" };

    const CliArgsParseResult result = parse_cli_args(2, argv);

    EXPECT_FALSE(result.success);
    EXPECT_EQ(0U, result.block_size);
    EXPECT_FALSE(result.error.empty());
}

// 3.2 Отрицательное значение возвращает ошибку.
TEST(CliArgsTest, Parse_WhenNegativeIntegerPassed_ReturnsError) {
    const char* argv[] = { "bulk", "-3" };

    const CliArgsParseResult result = parse_cli_args(2, argv);

    EXPECT_FALSE(result.success);
    EXPECT_EQ(0U, result.block_size);
    EXPECT_FALSE(result.error.empty());
}

// 3.3 Нечисловое значение возвращает ошибку.
TEST(CliArgsTest, Parse_WhenTextPassed_ReturnsError) {
    const char* argv[] = { "bulk", "abc" };

    const CliArgsParseResult result = parse_cli_args(2, argv);

    EXPECT_FALSE(result.success);
    EXPECT_EQ(0U, result.block_size);
    EXPECT_FALSE(result.error.empty());
}

// 3.4 Значение с лишними символами возвращает ошибку.
TEST(CliArgsTest, Parse_WhenTrailingCharactersPassed_ReturnsError) {
    const char* argv[] = { "bulk", "3abc" };

    const CliArgsParseResult result = parse_cli_args(2, argv);

    EXPECT_FALSE(result.success);
    EXPECT_EQ(0U, result.block_size);
    EXPECT_FALSE(result.error.empty());
}

// 3.5 Значение со знаком плюс возвращает ошибку.
TEST(CliArgsTest, Parse_WhenPlusSignPassed_ReturnsError) {
    const char* argv[] = { "bulk", "+3" };

    const CliArgsParseResult result = parse_cli_args(2, argv);

    EXPECT_FALSE(result.success);
    EXPECT_EQ(0U, result.block_size);
    EXPECT_FALSE(result.error.empty());
}

// 3.6 Значение из дефисов возвращает ошибку.
TEST(CliArgsTest, Parse_WhenDashesPassed_ReturnsError) {
    const char* argv[] = { "bulk", "--" };

    const CliArgsParseResult result = parse_cli_args(2, argv);

    EXPECT_FALSE(result.success);
    EXPECT_EQ(0U, result.block_size);
    EXPECT_FALSE(result.error.empty());
}

#endif  // Ошибки значения размера блока
