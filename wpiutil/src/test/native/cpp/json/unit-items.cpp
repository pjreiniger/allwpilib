//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++ (supporting code)
// |  |  |__   |  |  | | | |  version 3.11.2
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// SPDX-FileCopyrightText: 2013-2022 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT

#include "gtest/gtest.h"

#include "unit-json.h"
using wpi::json;




#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wrange-loop-construct"
#endif




TEST(IteratorWrapperObjectTest, Value)
{
    json j = { {"A", 1}, {"B", 2} };
    int counter = 1;

    for (auto i : json::iterator_wrapper(j)) // NOLINT(performance-for-range-copy)
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "A");
                CHECK(i.value() == json(1));
                break;
            }

            case 2:
            {
                CHECK(i.key() == "B");
                CHECK(i.value() == json(2));
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}

TEST(IteratorWrapperObjectTest, Reference)
{
    json j = { {"A", 1}, {"B", 2} };
    int counter = 1;

    for (auto& i : json::iterator_wrapper(j)) // NOLINT(readability-qualified-auto)
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "A");
                CHECK(i.value() == json(1));

                // change the value
                i.value() = json(11);
                CHECK(i.value() == json(11));
                break;
            }

            case 2:
            {
                CHECK(i.key() == "B");
                CHECK(i.value() == json(2));

                // change the value
                i.value() = json(22);
                CHECK(i.value() == json(22));
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);

    // check if values where changed
    CHECK(j == json({ {"A", 11}, {"B", 22} }));
}

TEST(IteratorWrapperObjectTest, ConstValue)
{
    json j = { {"A", 1}, {"B", 2} };
    int counter = 1;

    for (const auto i : json::iterator_wrapper(j)) // NOLINT(performance-for-range-copy)
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "A");
                CHECK(i.value() == json(1));
                break;
            }

            case 2:
            {
                CHECK(i.key() == "B");
                CHECK(i.value() == json(2));
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}

TEST(IteratorWrapperObjectTest, ConstReference)
{
    json j = { {"A", 1}, {"B", 2} };
    int counter = 1;

    for (const auto& i : json::iterator_wrapper(j))
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "A");
                CHECK(i.value() == json(1));
                break;
            }

            case 2:
            {
                CHECK(i.key() == "B");
                CHECK(i.value() == json(2));
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}




TEST(IteratorWrapperConstObjectTest, Value)
{
    const json j = { {"A", 1}, {"B", 2} };
    int counter = 1;

    for (auto i : json::iterator_wrapper(j)) // NOLINT(performance-for-range-copy)
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "A");
                CHECK(i.value() == json(1));
                break;
            }

            case 2:
            {
                CHECK(i.key() == "B");
                CHECK(i.value() == json(2));
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}

TEST(IteratorWrapperConstObjectTest, Reference)
{
    const json j = { {"A", 1}, {"B", 2} };
    int counter = 1;

    for (auto& i : json::iterator_wrapper(j)) // NOLINT(readability-qualified-auto)
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "A");
                CHECK(i.value() == json(1));
                break;
            }

            case 2:
            {
                CHECK(i.key() == "B");
                CHECK(i.value() == json(2));
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}

TEST(IteratorWrapperConstObjectTest, ConstValue)
{
    const json j = { {"A", 1}, {"B", 2} };
    int counter = 1;

    for (const auto i : json::iterator_wrapper(j)) // NOLINT(performance-for-range-copy)
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "A");
                CHECK(i.value() == json(1));
                break;
            }

            case 2:
            {
                CHECK(i.key() == "B");
                CHECK(i.value() == json(2));
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}

TEST(IteratorWrapperConstObjectTest, ConstReference)
{
    const json j = { {"A", 1}, {"B", 2} };
    int counter = 1;

    for (const auto& i : json::iterator_wrapper(j))
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "A");
                CHECK(i.value() == json(1));
                break;
            }

            case 2:
            {
                CHECK(i.key() == "B");
                CHECK(i.value() == json(2));
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}




TEST(IteratorWrapperArrayTest, Value)
{
    json j = { "A", "B" };
    int counter = 1;

    for (auto i : json::iterator_wrapper(j)) // NOLINT(performance-for-range-copy)
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "0");
                CHECK(i.value() == "A");
                break;
            }

            case 2:
            {
                CHECK(i.key() == "1");
                CHECK(i.value() == "B");
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}

TEST(IteratorWrapperArrayTest, Reference)
{
    json j = { "A", "B" };
    int counter = 1;

    for (auto& i : json::iterator_wrapper(j)) // NOLINT(readability-qualified-auto)
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "0");
                CHECK(i.value() == "A");

                // change the value
                i.value() = "AA";
                CHECK(i.value() == "AA");
                break;
            }

            case 2:
            {
                CHECK(i.key() == "1");
                CHECK(i.value() == "B");

                // change the value
                i.value() = "BB";
                CHECK(i.value() == "BB");
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);

    // check if values where changed
    CHECK(j == json({ "AA", "BB" }));
}

TEST(IteratorWrapperArrayTest, ConstValue)
{
    json j = { "A", "B" };
    int counter = 1;

    for (const auto i : json::iterator_wrapper(j)) // NOLINT(performance-for-range-copy)
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "0");
                CHECK(i.value() == "A");
                break;
            }

            case 2:
            {
                CHECK(i.key() == "1");
                CHECK(i.value() == "B");
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}

TEST(IteratorWrapperArrayTest, ConstReference)
{
    json j = { "A", "B" };
    int counter = 1;

    for (const auto& i : json::iterator_wrapper(j))
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "0");
                CHECK(i.value() == "A");
                break;
            }

            case 2:
            {
                CHECK(i.key() == "1");
                CHECK(i.value() == "B");
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}




TEST(IteratorWrapperConstArrayTest, Value)
{
    const json j = { "A", "B" };
    int counter = 1;

    for (auto i : json::iterator_wrapper(j)) // NOLINT(performance-for-range-copy)
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "0");
                CHECK(i.value() == "A");
                break;
            }

            case 2:
            {
                CHECK(i.key() == "1");
                CHECK(i.value() == "B");
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}

TEST(IteratorWrapperConstArrayTest, Reference)
{
    const json j = { "A", "B" };
    int counter = 1;

    for (auto& i : json::iterator_wrapper(j)) // NOLINT(readability-qualified-auto)
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "0");
                CHECK(i.value() == "A");
                break;
            }

            case 2:
            {
                CHECK(i.key() == "1");
                CHECK(i.value() == "B");
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}

TEST(IteratorWrapperConstArrayTest, ConstValue)
{
    const json j = { "A", "B" };
    int counter = 1;

    for (const auto i : json::iterator_wrapper(j)) // NOLINT(performance-for-range-copy)
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "0");
                CHECK(i.value() == "A");
                break;
            }

            case 2:
            {
                CHECK(i.key() == "1");
                CHECK(i.value() == "B");
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}

TEST(IteratorWrapperConstArrayTest, ConstReference)
{
    const json j = { "A", "B" };
    int counter = 1;

    for (const auto& i : json::iterator_wrapper(j))
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "0");
                CHECK(i.value() == "A");
                break;
            }

            case 2:
            {
                CHECK(i.key() == "1");
                CHECK(i.value() == "B");
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}




TEST(IteratorWrapperPrimitiveTest, Value)
{
    json j = 1;
    int counter = 1;

    for (auto i : json::iterator_wrapper(j)) // NOLINT(performance-for-range-copy)
    {
        ++counter;
        CHECK(i.key() == "");
        CHECK(i.value() == json(1));
    }

    CHECK(counter == 2);
}

TEST(IteratorWrapperPrimitiveTest, Reference)
{
    json j = 1;
    int counter = 1;

    for (auto& i : json::iterator_wrapper(j)) // NOLINT(readability-qualified-auto)
    {
        ++counter;
        CHECK(i.key() == "");
        CHECK(i.value() == json(1));

        // change value
        i.value() = json(2);
    }

    CHECK(counter == 2);

    // check if value has changed
    CHECK(j == json(2));
}

TEST(IteratorWrapperPrimitiveTest, ConstValue)
{
    json j = 1;
    int counter = 1;

    for (const auto i : json::iterator_wrapper(j)) // NOLINT(performance-for-range-copy)
    {
        ++counter;
        CHECK(i.key() == "");
        CHECK(i.value() == json(1));
    }

    CHECK(counter == 2);
}

TEST(IteratorWrapperPrimitiveTest, ConstReference)
{
    json j = 1;
    int counter = 1;

    for (const auto& i : json::iterator_wrapper(j))
    {
        ++counter;
        CHECK(i.key() == "");
        CHECK(i.value() == json(1));
    }

    CHECK(counter == 2);
}




TEST(IteratorWrapperConstPrimitiveTest, Value)
{
    const json j = 1;
    int counter = 1;

    for (auto i : json::iterator_wrapper(j)) // NOLINT(performance-for-range-copy)
    {
        ++counter;
        CHECK(i.key() == "");
        CHECK(i.value() == json(1));
    }

    CHECK(counter == 2);
}

TEST(IteratorWrapperConstPrimitiveTest, Reference)
{
    const json j = 1;
    int counter = 1;

    for (auto& i : json::iterator_wrapper(j)) // NOLINT(readability-qualified-auto)
    {
        ++counter;
        CHECK(i.key() == "");
        CHECK(i.value() == json(1));
    }

    CHECK(counter == 2);
}

TEST(IteratorWrapperConstPrimitiveTest, ConstValue)
{
    const json j = 1;
    int counter = 1;

    for (const auto i : json::iterator_wrapper(j)) // NOLINT(performance-for-range-copy)
    {
        ++counter;
        CHECK(i.key() == "");
        CHECK(i.value() == json(1));
    }

    CHECK(counter == 2);
}

TEST(IteratorWrapperConstPrimitiveTest, ConstReference)
{
    const json j = 1;
    int counter = 1;

    for (const auto& i : json::iterator_wrapper(j))
    {
        ++counter;
        CHECK(i.key() == "");
        CHECK(i.value() == json(1));
    }

    CHECK(counter == 2);
}







TEST(ItemsObjectTest, Value)
{
    json j = { {"A", 1}, {"B", 2} };
    int counter = 1;

    for (auto i : j.items()) // NOLINT(performance-for-range-copy)
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "A");
                CHECK(i.value() == json(1));
                break;
            }

            case 2:
            {
                CHECK(i.key() == "B");
                CHECK(i.value() == json(2));
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}

TEST(ItemsObjectTest, Reference)
{
    json j = { {"A", 1}, {"B", 2} };
    int counter = 1;

    for (auto& i : j.items()) // NOLINT(readability-qualified-auto)
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "A");
                CHECK(i.value() == json(1));

                // change the value
                i.value() = json(11);
                CHECK(i.value() == json(11));
                break;
            }

            case 2:
            {
                CHECK(i.key() == "B");
                CHECK(i.value() == json(2));

                // change the value
                i.value() = json(22);
                CHECK(i.value() == json(22));
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);

    // check if values where changed
    CHECK(j == json({ {"A", 11}, {"B", 22} }));
}

TEST(ItemsObjectTest, ConstValue)
{
    json j = { {"A", 1}, {"B", 2} };
    int counter = 1;

    for (const auto i : j.items()) // NOLINT(performance-for-range-copy)
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "A");
                CHECK(i.value() == json(1));
                break;
            }

            case 2:
            {
                CHECK(i.key() == "B");
                CHECK(i.value() == json(2));
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}

TEST(ItemsObjectTest, ConstReference)
{
    json j = { {"A", 1}, {"B", 2} };
    int counter = 1;

    for (const auto& i : j.items())
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "A");
                CHECK(i.value() == json(1));
                break;
            }

            case 2:
            {
                CHECK(i.key() == "B");
                CHECK(i.value() == json(2));
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}

#ifdef JSON_HAS_CPP_17

TEST(ItemsObjectTest, StructuredBindings)
{
    json j = { {"A", 1}, {"B", 2} };

    std::map<std::string, int> m;

    for (auto const&[key, value] : j.items())
    {
        m.emplace(key, value);
    }

    CHECK(j.get<decltype(m)>() == m);
}

#endif




TEST(ItemsConstObjectTest, Value)
{
    const json j = { {"A", 1}, {"B", 2} };
    int counter = 1;

    for (auto i : j.items()) // NOLINT(performance-for-range-copy)
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "A");
                CHECK(i.value() == json(1));
                break;
            }

            case 2:
            {
                CHECK(i.key() == "B");
                CHECK(i.value() == json(2));
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}

TEST(ItemsConstObjectTest, Reference)
{
    const json j = { {"A", 1}, {"B", 2} };
    int counter = 1;

    for (auto& i : j.items()) // NOLINT(readability-qualified-auto)
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "A");
                CHECK(i.value() == json(1));
                break;
            }

            case 2:
            {
                CHECK(i.key() == "B");
                CHECK(i.value() == json(2));
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}

TEST(ItemsConstObjectTest, ConstValue)
{
    const json j = { {"A", 1}, {"B", 2} };
    int counter = 1;

    for (const auto i : j.items()) // NOLINT(performance-for-range-copy)
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "A");
                CHECK(i.value() == json(1));
                break;
            }

            case 2:
            {
                CHECK(i.key() == "B");
                CHECK(i.value() == json(2));
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}

TEST(ItemsConstObjectTest, ConstReference)
{
    const json j = { {"A", 1}, {"B", 2} };
    int counter = 1;

    for (const auto& i : j.items())
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "A");
                CHECK(i.value() == json(1));
                break;
            }

            case 2:
            {
                CHECK(i.key() == "B");
                CHECK(i.value() == json(2));
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}




TEST(ItemsArrayTest, Value)
{
    json j = { "A", "B" };
    int counter = 1;

    for (auto i : j.items()) // NOLINT(performance-for-range-copy)
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "0");
                CHECK(i.value() == "A");
                break;
            }

            case 2:
            {
                CHECK(i.key() == "1");
                CHECK(i.value() == "B");
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}

TEST(ItemsArrayTest, Reference)
{
    json j = { "A", "B" };
    int counter = 1;

    for (auto& i : j.items()) // NOLINT(readability-qualified-auto)
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "0");
                CHECK(i.value() == "A");

                // change the value
                i.value() = "AA";
                CHECK(i.value() == "AA");
                break;
            }

            case 2:
            {
                CHECK(i.key() == "1");
                CHECK(i.value() == "B");

                // change the value
                i.value() = "BB";
                CHECK(i.value() == "BB");
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);

    // check if values where changed
    CHECK(j == json({ "AA", "BB" }));
}

TEST(ItemsArrayTest, ConstValue)
{
    json j = { "A", "B" };
    int counter = 1;

    for (const auto i : j.items()) // NOLINT(performance-for-range-copy)
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "0");
                CHECK(i.value() == "A");
                break;
            }

            case 2:
            {
                CHECK(i.key() == "1");
                CHECK(i.value() == "B");
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}

TEST(ItemsArrayTest, ConstReference)
{
    json j = { "A", "B" };
    int counter = 1;

    for (const auto& i : j.items())
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "0");
                CHECK(i.value() == "A");
                break;
            }

            case 2:
            {
                CHECK(i.key() == "1");
                CHECK(i.value() == "B");
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}




TEST(ItemsConstArrayTest, Value)
{
    const json j = { "A", "B" };
    int counter = 1;

    for (auto i : j.items()) // NOLINT(performance-for-range-copy)
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "0");
                CHECK(i.value() == "A");
                break;
            }

            case 2:
            {
                CHECK(i.key() == "1");
                CHECK(i.value() == "B");
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}

TEST(ItemsConstArrayTest, Reference)
{
    const json j = { "A", "B" };
    int counter = 1;

    for (auto& i : j.items()) // NOLINT(readability-qualified-auto)
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "0");
                CHECK(i.value() == "A");
                break;
            }

            case 2:
            {
                CHECK(i.key() == "1");
                CHECK(i.value() == "B");
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}

TEST(ItemsConstArrayTest, ConstValue)
{
    const json j = { "A", "B" };
    int counter = 1;

    for (const auto i : j.items()) // NOLINT(performance-for-range-copy)
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "0");
                CHECK(i.value() == "A");
                break;
            }

            case 2:
            {
                CHECK(i.key() == "1");
                CHECK(i.value() == "B");
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}

TEST(ItemsConstArrayTest, ConstReference)
{
    const json j = { "A", "B" };
    int counter = 1;

    for (const auto& i : j.items())
    {
        switch (counter++)
        {
            case 1:
            {
                CHECK(i.key() == "0");
                CHECK(i.value() == "A");
                break;
            }

            case 2:
            {
                CHECK(i.key() == "1");
                CHECK(i.value() == "B");
                break;
            }

            default:
            {
                break;
            }
        }
    }

    CHECK(counter == 3);
}




TEST(ItemsPrimitiveTest, Value)
{
    json j = 1;
    int counter = 1;

    for (auto i : j.items()) // NOLINT(performance-for-range-copy)
    {
        ++counter;
        CHECK(i.key() == "");
        CHECK(i.value() == json(1));
    }

    CHECK(counter == 2);
}

TEST(ItemsPrimitiveTest, Reference)
{
    json j = 1;
    int counter = 1;

    for (auto& i : j.items()) // NOLINT(readability-qualified-auto)
    {
        ++counter;
        CHECK(i.key() == "");
        CHECK(i.value() == json(1));

        // change value
        i.value() = json(2);
    }

    CHECK(counter == 2);

    // check if value has changed
    CHECK(j == json(2));
}

TEST(ItemsPrimitiveTest, ConstValue)
{
    json j = 1;
    int counter = 1;

    for (const auto i : j.items()) // NOLINT(performance-for-range-copy)
    {
        ++counter;
        CHECK(i.key() == "");
        CHECK(i.value() == json(1));
    }

    CHECK(counter == 2);
}

TEST(ItemsPrimitiveTest, ConstReference)
{
    json j = 1;
    int counter = 1;

    for (const auto& i : j.items())
    {
        ++counter;
        CHECK(i.key() == "");
        CHECK(i.value() == json(1));
    }

    CHECK(counter == 2);
}




TEST(ItemsConstPrimitiveTest, Value)
{
    const json j = 1;
    int counter = 1;

    for (auto i : j.items()) // NOLINT(performance-for-range-copy)
    {
        ++counter;
        CHECK(i.key() == "");
        CHECK(i.value() == json(1));
    }

    CHECK(counter == 2);
}

TEST(ItemsConstPrimitiveTest, Reference)
{
    const json j = 1;
    int counter = 1;

    for (auto& i : j.items()) // NOLINT(readability-qualified-auto)
    {
        ++counter;
        CHECK(i.key() == "");
        CHECK(i.value() == json(1));
    }

    CHECK(counter == 2);
}

TEST(ItemsConstPrimitiveTest, ConstValue)
{
    const json j = 1;
    int counter = 1;

    for (const auto i : j.items()) // NOLINT(performance-for-range-copy)
    {
        ++counter;
        CHECK(i.key() == "");
        CHECK(i.value() == json(1));
    }

    CHECK(counter == 2);
}

TEST(ItemsConstPrimitiveTest, ConstReference)
{
    const json j = 1;
    int counter = 1;

    for (const auto& i : j.items())
    {
        ++counter;
        CHECK(i.key() == "");
        CHECK(i.value() == json(1));
    }

    CHECK(counter == 2);
}


