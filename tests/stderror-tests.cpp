#include <gtest/gtest.h>
#include <stderror.h>

namespace {

void test_std_error_log_formatter(const char **messages, size_t messages_count) {
    for (size_t idx = 0; idx < messages_count; idx++)
        fprintf(stdout, "%s" "\n", messages[idx]);
}

char *test_std_error_dump_formatter(const char **messages, size_t messages_count) {
    std::string result;

    for (size_t idx = 0; idx < messages_count; idx++)
        result += std::string(messages[idx]) + "\n";

    return strdup(result.c_str());
}

} /* namespace */

TEST(std_error, new) {
    std_error_t *error = std_error_new("hello");

    EXPECT_NE(error, nullptr);
    EXPECT_EQ(std_error_count(error), 1);
    EXPECT_STREQ(std_error_last(error), "hello");

    std_error_free_safe(&error);
}

TEST(std_error, append) {
    std_error_t *error = std_error_new("hello");
    std_error_t *updated = std_error_append(error, "world");

    EXPECT_EQ(error, updated);
    EXPECT_EQ(std_error_count(error), 2);
    EXPECT_STREQ(std_error_first(error), "hello");
    EXPECT_STREQ(std_error_last(error), "world");

    EXPECT_DEBUG_DEATH(std_error_append(nullptr, ""), ".*");

    std_error_free_safe(&error);
}

TEST(std_error, get) {
    std_error_t *error = std_error_new("hello");
    std_error_append(error, "world");
    std_error_append(error, "!!!");

    EXPECT_STREQ(std_error_get(error, 0), "hello");
    EXPECT_STREQ(std_error_get(error, 1), "world");
    EXPECT_STREQ(std_error_get(error, 2), "!!!");

    EXPECT_DEBUG_DEATH(std_error_get(error, 3), ".*");
    EXPECT_DEBUG_DEATH(std_error_get(nullptr, 0), ".*");

    std_error_free_safe(&error);
}

TEST(std_error, first) {
    std_error_t *error = std_error_new("hello");
    std_error_append(error, "world");
    std_error_append(error, "!!!");

    EXPECT_STREQ(std_error_first(error), "hello");
    EXPECT_DEBUG_DEATH(std_error_first(nullptr), ".*");

    std_error_free_safe(&error);
}

TEST(std_error, last) {
    std_error_t *error = std_error_new("hello");
    std_error_append(error, "world");
    std_error_append(error, "!!!");

    EXPECT_STREQ(std_error_last(error), "!!!");
    EXPECT_DEBUG_DEATH(std_error_last(nullptr), ".*");

    std_error_free_safe(&error);
}

TEST(std_error, count) {
    std_error_t *error = std_error_new("hello");
    EXPECT_EQ(std_error_count(error), 1);

    std_error_append(error, "world");
    EXPECT_EQ(std_error_count(error), 2);

    std_error_append(error, "!!!");
    EXPECT_EQ(std_error_count(error), 3);

    EXPECT_DEBUG_DEATH(std_error_count(nullptr), ".*");

    std_error_free_safe(&error);
}

TEST(std_error, free) {
    EXPECT_DEBUG_DEATH(std_error_free_safe(nullptr), ".*");
}

TEST(std_error, free_safe) {
    std_error_t *error = std_error_new("hello");

    std_error_free_safe(&error);
    EXPECT_EQ(error, nullptr);

    std_error_t *nullable = nullptr;
    EXPECT_DEBUG_DEATH(std_error_free_safe(&nullable), ".*");

    EXPECT_DEBUG_DEATH(std_error_free_safe(nullptr), ".*");
}

TEST(std_error, log) {
    std_error_t *error = std_error_new("hello");
    std_error_append(error, "world");
    std_error_append(error, "!!!");

    testing::internal::CaptureStderr();
    std_error_log(error);

    std::string log = testing::internal::GetCapturedStderr();
    EXPECT_EQ(log, "[error]"    "\n"
                   "   - hello" "\n"
                   "   - world" "\n"
                   "   - !!!"   "\n");

    std_error_free_safe(&error);
}

TEST(std_error, log_to) {
    std_error_t *error = std_error_new("hello");
    std_error_append(error, "world");
    std_error_append(error, "!!!");

    testing::internal::CaptureStdout();
    std_error_log_to(error, stdout);

    std::string log = testing::internal::GetCapturedStdout();
    EXPECT_EQ(log, "[error]"    "\n"
                   "   - hello" "\n"
                   "   - world" "\n"
                   "   - !!!"   "\n");

    std_error_free_safe(&error);
}

TEST(std_error, log_formatted) {
    std_error_t *error = std_error_new("hello");
    std_error_append(error, "world");
    std_error_append(error, "!!!");

    testing::internal::CaptureStdout();
    std_error_log_formatted(error, test_std_error_log_formatter);

    std::string log = testing::internal::GetCapturedStdout();
    EXPECT_EQ(log, "hello" "\n"
                   "world" "\n"
                   "!!!"   "\n");

    std_error_free_safe(&error);
}

TEST(std_error, dump) {
    std_error_t *error = std_error_new("hello");
    std_error_append(error, "world");
    std_error_append(error, "!!!");

    char *dump = std_error_dump(error);

    EXPECT_STREQ(dump, "[error]" "\n"
                       "   - hello" "\n"
                       "   - world" "\n"
                       "   - !!!"   "\n");

    free(dump);
    std_error_free_safe(&error);
}

TEST(std_error, dump_formatted) {
    std_error_t *error = std_error_new("hello");
    std_error_append(error, "world");
    std_error_append(error, "!!!");

    char *dump = std_error_dump_formatted(error, test_std_error_dump_formatter);

    EXPECT_STREQ(dump, "hello" "\n"
                       "world" "\n"
                       "!!!"   "\n");

    free(dump);
    std_error_free_safe(&error);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
