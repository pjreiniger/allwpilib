/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2017. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/#ifndef UNIT_JSON_H_
#define UNIT_JSON_H_

#include <ostream>

#include "wpi/json.h"
#include <gmock/gmock.h>

#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(__clang__)
#pragma GCC diagnostic ignored "-Wunused-comparison"
#endif

// Alias CHECK test macros with googletest macros
#define REQUIRE(...) ASSERT_TRUE((__VA_ARGS__))
#define CHECK(...) EXPECT_TRUE((__VA_ARGS__))
#define CHECK_FALSE(...) EXPECT_FALSE((__VA_ARGS__))

#define CAPTURE(...) SCOPED_TRACE(__VA_ARGS__);
#define INFO(...) ;
#define INFO_WITH_TEMP(...) ;

// Alias exception catchers
#define CHECK_THROWS_WITH_AS(statement, expected_message, expected_exception) \
  EXPECT_THAT([&]() { statement; }, ::testing::ThrowsMessage<typename std::remove_const<                                  \
                    typename std::remove_reference<expected_exception>::type>::type>(expected_message));

#define CHECK_THROWS_AS(statement, expected_exception) \
  EXPECT_THAT([&]() { statement; }, ::testing::Throws<typename std::remove_const<                                  \
                    typename std::remove_reference<expected_exception>::type>::type>());

#define CHECK_THROWS_WITH(statement, expected_message) \
  CHECK_THROWS_WITH_AS(statement, expected_message, std::exception)

#define CHECK_THROWS_WITH_STD_STR(statement, expected_message) CHECK_THROWS_WITH(statement, expected_message)
#define CHECK_NOTHROW(...) __VA_ARGS__

#endif
