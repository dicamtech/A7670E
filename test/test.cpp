#include "unity.h"

// Declare external test functions from your test files
extern void test_TParamsMgr();
extern void test_TParam();

#ifdef UNIT_TEST
int main(void) {
  UNITY_BEGIN();
  test_TParamsMgr();  // Call tests defined in test_TParamsMgr.cpp
  test_TParam();      // Call tests defined in test_TParam.cpp
  return UNITY_END();
}
#endif
