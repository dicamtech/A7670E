#include "unity.h"

// Declare external test functions from your test files
extern void test_TParamsMgr();
extern void test_SiteSpec();
extern void test_TParam();
extern void test_TSystem();

#ifdef UNIT_TEST
int main(void) {
  UNITY_BEGIN();
  test_TParam();      // Call tests defined in test_TParam.cpp
  test_TParamsMgr();  // Call tests defined in test_TParamsMgr.cpp
  test_SiteSpec();
  test_TSystem();
  return UNITY_END();
}
#endif
