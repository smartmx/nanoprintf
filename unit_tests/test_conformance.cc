#include "CppUTest/TestHarness.h"
#include "nanoprintf_in_unit_tests.h"

TEST_GROUP(conformance){};

namespace {
char conformance_buf[256];
void CheckConformance(char const *fmt, ...) {
    std::string actual;
    {
        va_list v;
        va_start(v, fmt);
        npf_vsnprintf(conformance_buf, sizeof(conformance_buf), fmt, v);
        va_end(v);
        actual = conformance_buf;
    }
    std::string expected;
    {
        va_list v;
        va_start(v, fmt);
        vsnprintf(conformance_buf, sizeof(conformance_buf), fmt, v);
        va_end(v);
        expected = conformance_buf;
    }
    CHECK_EQUAL(expected, actual);
}
}  // namespace

TEST(conformance, Percent) {
    CheckConformance("%%");
    CheckConformance("%-%");
    CheckConformance("% %");
    CheckConformance("%0%");
    CheckConformance("%+%");
    CheckConformance("%#%");
    CheckConformance("%10%");
    CheckConformance("%.10%");
    CheckConformance("%-10%");
    CheckConformance("%10.10%");
}

TEST(conformance, Char) {
    // every char
    for (int i = CHAR_MIN; i < CHAR_MAX; ++i) {
        CheckConformance("%c", i);
    }

    // right justify field width
    for (int precision = 0; precision < 20; ++precision) {
        char fmt[8];
        sprintf(fmt, "%%%dc", precision);
        CheckConformance(fmt, 'A');
    }

    // left justify field width
    for (int precision = 0; precision < 20; ++precision) {
        char fmt[8];
        sprintf(fmt, "%%-%dc", precision);
        CheckConformance(fmt, 'A');
    }
}

TEST(conformance, UnsignedInt) {
    CheckConformance("%u", 0);
    CheckConformance("%u", UINT_MAX);
}

TEST(conformance, PrependSign) {
    CheckConformance("%+%");
    CheckConformance("%+c", 'A');
    CheckConformance("%+c", 0);
    CheckConformance("%+c", -1);
    CheckConformance("%+i", -1);
    CheckConformance("%+i", 0);
    CheckConformance("%+i", 1);
    CheckConformance("%+u", 0);
    CheckConformance("%+u", 1);
    CheckConformance("%+o", 0);
    CheckConformance("%+o", 1);
}

TEST(conformance, PrependSpace) {
    CheckConformance("% %");
    CheckConformance("% c", 'A');
    CheckConformance("% c", 0);
    CheckConformance("% c", -1);
    CheckConformance("% i", -1);
    CheckConformance("% i", 0);
    CheckConformance("% i", 1);
    CheckConformance("% u", 0);
    CheckConformance("% u", 1);
    CheckConformance("% o", 0);
    CheckConformance("% o", 1);
}

TEST(conformance, FieldWidthStrings) {
    CheckConformance("%10s", "hello");
    CheckConformance("%10s", "hello world!");
}

TEST(conformance, PrecisionStrings) {
    //    CheckConformance("%.10s", "hello world this string is > 10");
}

TEST(conformance, FieldWidthAndSign) {
    CheckConformance("%+4c", 1);
    CheckConformance("%+4c", 0);
    CheckConformance("%+4c", -1);
    CheckConformance("%+4i", 1);
    CheckConformance("%+4i", 0);
    CheckConformance("%+4i", -1);
    CheckConformance("%+4u", 1);
    CheckConformance("%+4u", 0);
}

TEST(conformance, FieldWidthAndZeroPad) {
    // zero-pad is undefined on char
    CheckConformance("%04i", 1);
    CheckConformance("%04i", 0);
    CheckConformance("%04i", -1);
    CheckConformance("%04u", 1);
    CheckConformance("%04u", 0);
}