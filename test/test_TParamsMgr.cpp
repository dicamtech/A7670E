#include <unity.h>
#include "../brooderApp/D2BrooderAlarm/src/param.cpp"
#include "../brooderApp/D2BrooderAlarm/src/common.h"
#include "../brooderApp/D2BrooderAlarm/src/paramsmgr.h"
#include "../brooderApp/D2BrooderAlarm/src/paramsmgr.cpp"
#include "../brooderApp/D2ParamsGenerator/api/paramdescr.cpp"

// Test that a unique alias is correctly assigned and fields are set.
void test_AddParamUniqueAlias(void) {
  TParamsMgr mgr;
  TParam<int> owner;
  mgr.PushParam(&owner,  &owner, PAC_None, EPStorage::NVS, "");

  TParam<int> param;
  mgr.PushParam(&param, &owner, PAC_System, EPStorage::RAM, "system");

  // Verify that the parameter's pid is set to 0 (first parameter added).
  TEST_ASSERT_EQUAL_INT(1, param.GetPID());
  // Verify that the parameter's owner is set correctly.
  TEST_ASSERT_EQUAL_INT(owner.GetPID(), param.GetOwner());
  // Verify that pac and idx are set as expected.
  TEST_ASSERT_EQUAL_INT(PAC_System, param.GetPac());
  TEST_ASSERT_EQUAL_INT(0, param.GetIdx());
  // Since the alias is unique, it should be assigned.
  TEST_ASSERT_EQUAL_STRING("system", param.GetAlias().c_str());
  // Check that the description pointer is set (using the global paramdescr).
  TEST_ASSERT_NOT_NULL(param.GetDescr());
  TEST_ASSERT_EQUAL_STRING("System", param.GetDescr()->Classname);
  // Verify that the manager reports one parameter.
  TEST_ASSERT_EQUAL_INT(2, mgr.GetParamCount());
}

// Test that a duplicate alias is not assigned to the second parameter.
void test_AddParamDuplicateAlias(void) {
  TParamsMgr mgr;
  TParam<int> owner;
  mgr.PushParam(&owner,  &owner, PAC_System, EPStorage::NVS, "");

  // First parameter: add with alias "site".
  TParam<int> param1;
  mgr.PushParam(&param1, &owner, PAC_Site, EPStorage::NVS, "site");
  TEST_ASSERT_EQUAL_STRING("site", param1.GetAlias().c_str());
  TEST_ASSERT_EQUAL_INT(1, param1.GetPID());

  // Second parameter: try to add with the same alias.
  TParam<int> param2;
  mgr.PushParam(&param2, &owner, PAC_Zone, EPStorage::NVS, "site");
  // Since the alias "site" already exists, the second parameter should not receive it.
  TEST_ASSERT_EQUAL_STRING("", param2.GetAlias().c_str());
  TEST_ASSERT_EQUAL_INT(2, param2.GetPID());

  // Verify that the manager now holds two parameters.
  TEST_ASSERT_EQUAL_INT(3, mgr.GetParamCount());
}

// Test that a duplicate alias is not assigned to the second parameter.
void test_AutoIndexing(void) {
  TParamsMgr mgr;
  TParam<int> owner;

  TParam<int> param1;
  TParam<int> param2;
  TParam<int> param3;

  mgr.PushParam(&owner,  &owner, PAC_System, EPStorage::NVS, "");
  mgr.PushParam(&param1, &owner, PAC_Site, EPStorage::NVS, "");
  mgr.PushParam(&param2, &owner, PAC_Site, EPStorage::NVS, "");
  mgr.PushParam(&param3, &owner, PAC_Site, EPStorage::NVS, "");

  TEST_ASSERT_EQUAL_INT(0, owner.GetIdx());
  TEST_ASSERT_EQUAL_INT(0, param1.GetIdx());
  TEST_ASSERT_EQUAL_INT(1, param2.GetIdx());
  TEST_ASSERT_EQUAL_INT(2, param3.GetIdx());

  TEST_ASSERT_EQUAL_INT(0, owner.GetPID());
  TEST_ASSERT_EQUAL_INT(1, param1.GetPID());
  TEST_ASSERT_EQUAL_INT(2, param2.GetPID());
  TEST_ASSERT_EQUAL_INT(3, param3.GetPID());

  // Verify that the manager now holds two parameters.
  TEST_ASSERT_EQUAL_INT(4, mgr.GetParamCount());
}

void test_AutoAliasing(void){
  TParamsMgr mgr;
  TParam<int> owner;

  TParam<int> param1;
  TParam<int> param2;
  TParam<int> param3;

  mgr.PushParam(&owner,  &owner, PAC_System, EPStorage::NVS, "*");
  mgr.PushParam(&param1, &owner, PAC_Site, EPStorage::NVS, "*");
  mgr.PushParam(&param2, &owner, PAC_Site, EPStorage::NVS, "*");
  mgr.PushParam(&param3, &owner, PAC_Site, EPStorage::NVS, "*");

  TEST_ASSERT_EQUAL_STRING(owner.GetDescr()->Classname, owner.GetAlias().c_str());
  TEST_ASSERT_EQUAL_STRING(param1.GetDescr()->Classname, param1.GetAlias().c_str());
  TEST_ASSERT_EQUAL_STRING("", param2.GetAlias().c_str());
  TEST_ASSERT_EQUAL_STRING("", param3.GetAlias().c_str());

}

void test_Param0(void){
  TParamsMgr mgr;
  TParam<int> owner;

  mgr.PushParam(&owner,  &owner, PAC_System, EPStorage::NVS, "*");

  // The owner pid should be auto assigned to 0 
  TEST_ASSERT_EQUAL_INT(0, owner.GetPID());
  TEST_ASSERT_EQUAL_INT(0, owner.GetOwner());
  TEST_ASSERT_EQUAL_INT(0, owner.GetIdx());
}

void test_TParamsMgr_Find_NonExistent(void) {
  TParamsMgr mgr;
  TParam<int> none, system;
  mgr.PushParam(&none, &none, PAC_None, EPStorage::NVS, "None");
  mgr.PushParam(&system, &none, PAC_System, EPStorage::NVS, "System");
  
  int pid = mgr.Find("NonExistent");
  TEST_ASSERT_EQUAL(UNSET_PID, pid);
}

//---------------------------------------------------------------------
// main() for native tests
//---------------------------------------------------------------------
void test_TParamsMgr(void) {
  RUN_TEST(test_AddParamUniqueAlias);
  RUN_TEST(test_AddParamDuplicateAlias);
  RUN_TEST(test_AutoIndexing);
  RUN_TEST(test_AutoAliasing);
  RUN_TEST(test_Param0);
  RUN_TEST(test_TParamsMgr_Find_NonExistent);
}
