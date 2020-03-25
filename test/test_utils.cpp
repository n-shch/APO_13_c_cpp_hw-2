//
// Created by nick on 04.03.2020.
//


#include "gtest/gtest.h"

extern "C" {
    #include "../include/testy/utils.h"
}

char line[] = "commit d5d7ecaf88dc1f492f05cb055350b47db2987e79";
char field[] = "commit";
char not_valid_field[] = "password";
char *result = line + strlen(field);


TEST(search_field_test, valid_data) {
    ASSERT_EQ(result, search_field(line, field));
}

TEST(search_field_test, not_valid_data) {
    ASSERT_EQ(NULL, search_field(line, not_valid_field));
}
