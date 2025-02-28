#include <unity.h>
#include "../brooderApp/D2BrooderAlarm/src/param.h"
#include "../brooderApp/D2BrooderAlarm/src/param.cpp"

void test_Param_int() {
  TParam<int32_t> paramInt(0);

  // Valid positive conversion
  TEST_ASSERT_EQUAL_INT(
    static_cast<int>(EPError::None),
    static_cast<int>(paramInt.SetValueFromStr("123"))
  );
  TEST_ASSERT_EQUAL(123, paramInt.GetValue());

  // Valid negative conversion
  TEST_ASSERT_EQUAL_INT(
    static_cast<int>(EPError::None),
    static_cast<int>(paramInt.SetValueFromStr("-123"))
  );
  TEST_ASSERT_EQUAL(-123, paramInt.GetValue());

  // Boundary values
  TEST_ASSERT_EQUAL_INT(
    static_cast<int>(EPError::None),
    static_cast<int>(paramInt.SetValueFromStr(std::to_string(INT32_MAX).c_str()))
  );
  TEST_ASSERT_EQUAL(INT32_MAX, paramInt.GetValue());

  TEST_ASSERT_EQUAL_INT(
    static_cast<int>(EPError::None),
    static_cast<int>(paramInt.SetValueFromStr(std::to_string(INT32_MIN).c_str()))
  );
  TEST_ASSERT_EQUAL(INT32_MIN, paramInt.GetValue());

  // Invalid conversion (non-numeric)
  TEST_ASSERT_EQUAL_INT(
    static_cast<int>(EPError::TypeMismatch),
    static_cast<int>(paramInt.SetValueFromStr("123abc"))
  );

  // Invalid conversion (empty string)
  TEST_ASSERT_EQUAL_INT(
    static_cast<int>(EPError::TypeMismatch),
    static_cast<int>(paramInt.SetValueFromStr(""))
  );
}

void test_Param_string() {
  TParam<std::string> paramStr("default");

  // Valid string conversion
  TEST_ASSERT_EQUAL_INT(
    static_cast<int>(EPError::None),
    static_cast<int>(paramStr.SetValueFromStr("Hello, World!"))
  );
  TEST_ASSERT_EQUAL_STRING("Hello, World!", paramStr.GetValue().c_str());

  // Empty string
  TEST_ASSERT_EQUAL_INT(
    static_cast<int>(EPError::None),
    static_cast<int>(paramStr.SetValueFromStr(""))
  );
  TEST_ASSERT_EQUAL_STRING("", paramStr.GetValue().c_str());
}

void test_Param_uint16() {
  TParam<uint16_t> paramUint(0);

  // Valid positive conversion
  TEST_ASSERT_EQUAL_INT(
    static_cast<int>(EPError::None),
    static_cast<int>(paramUint.SetValueFromStr("65535"))
  );
  TEST_ASSERT_EQUAL(65535, paramUint.GetValue());

  // Zero value
  TEST_ASSERT_EQUAL_INT(
    static_cast<int>(EPError::None),
    static_cast<int>(paramUint.SetValueFromStr("0"))
  );
  TEST_ASSERT_EQUAL(0, paramUint.GetValue());

  // Invalid conversion (non-numeric)
  TEST_ASSERT_EQUAL_INT(
    static_cast<int>(EPError::TypeMismatch),
    static_cast<int>(paramUint.SetValueFromStr("abc"))
  );
}

void test_Param_to_string(){
  TParam<uint16_t> paramUint(777);
  TEST_ASSERT_EQUAL_STRING("777", paramUint.ToString().c_str());

  TParam<std::string> paramStr("Dicam ltd");
  TEST_ASSERT_EQUAL_STRING("Dicam ltd", paramStr.ToString().c_str());

  TParam<int16_t> paramInt(-777);
  TEST_ASSERT_EQUAL_STRING("-777", paramInt.ToString().c_str());
}

//---------------------------------------------------------------------
// main() for native tests
//---------------------------------------------------------------------
void test_TParam(void) {
  RUN_TEST(test_Param_int);
  RUN_TEST(test_Param_string);
  RUN_TEST(test_Param_uint16);
  RUN_TEST(test_Param_to_string);
}
