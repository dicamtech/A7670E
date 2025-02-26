#include <unity.h>
#include "../brooderApp/D2BrooderAlarm/src/paramsmgr.h"
#include "../brooderApp/D2BrooderAlarm/src/paramsmgr.cpp"
#include "../brooderApp/D2ParamsGenerator/api/paramdescr.cpp"

// Test that a unique alias is correctly assigned and fields are set.
void test_AddParamUniqueAlias(void) {
  TParamsMgr mgr;
  TParam<int> owner;
  owner.pid = 0;  // Simulate an owner with pid = 0.

  TParam<int> param;
  mgr.AddParam(&param, &owner, PAC_System, 0, EPStorage::RAM, "system");

  // Verify that the parameter's pid is set to 0 (first parameter added).
  TEST_ASSERT_EQUAL_INT(0, param.pid);
  // Verify that the parameter's owner is set correctly.
  TEST_ASSERT_EQUAL_INT(owner.pid, param.ref.owner);
  // Verify that pac and idx are set as expected.
  TEST_ASSERT_EQUAL_INT(PAC_System, param.ref.pac);
  TEST_ASSERT_EQUAL_INT(0, param.ref.idx);
  // Since the alias is unique, it should be assigned.
  TEST_ASSERT_EQUAL_STRING("system", param.ref.strVal.c_str());
  // Check that the description pointer is set (using the global paramdescr).
  TEST_ASSERT_NOT_NULL(param.descr);
  TEST_ASSERT_EQUAL_STRING("System", param.descr->Classname);
  // Verify that the manager reports one parameter.
  TEST_ASSERT_EQUAL_INT(1, mgr.GetParamCount());
}

// Test that a duplicate alias is not assigned to the second parameter.
void test_AddParamDuplicateAlias(void) {
  TParamsMgr mgr;
  TParam<int> owner;
  owner.pid = 0;  // Simulate an owner with pid = 0.

  // First parameter: add with alias "site".
  TParam<int> param1;
  mgr.AddParam(&param1, &owner, PAC_Site, 0, EPStorage::NVS, "site");
  TEST_ASSERT_EQUAL_STRING("site", param1.ref.strVal.c_str());
  TEST_ASSERT_EQUAL_INT(0, param1.pid);

  // Second parameter: try to add with the same alias.
  TParam<int> param2;
  mgr.AddParam(&param2, &owner, PAC_Zone, 8, EPStorage::NVS, "site");
  // Since the alias "site" already exists, the second parameter should not receive it.
  TEST_ASSERT_EQUAL_STRING("", param2.ref.strVal.c_str());
  TEST_ASSERT_EQUAL_INT(1, param2.pid);

  // Verify that the manager now holds two parameters.
  TEST_ASSERT_EQUAL_INT(2, mgr.GetParamCount());
}

//---------------------------------------------------------------------
// main() for native tests
//---------------------------------------------------------------------
void test_TParamsMgr(void) {
  RUN_TEST(test_AddParamUniqueAlias);
  RUN_TEST(test_AddParamDuplicateAlias);
}
