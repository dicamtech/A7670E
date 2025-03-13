#include <unity.h>
#include "../brooderApp/D2BrooderAlarm/src/common.h"
#include "../brooderApp/D2BrooderAlarm/src/paramsmgr.h"
#include "../brooderApp/D2BrooderAlarm/src/paramsmgr.cpp"
#include "../brooderApp/D2BrooderAlarm/src/appmapmgr.cpp"
#include "../brooderApp/D2ParamsGenerator/api/paramdescr.cpp"

// Test that a unique alias is correctly assigned and fields are set.
void test_ParamsMgr_UniqueAlias(void) {
  AppMapMgr.Reset();
  ParamsMgr.Reset();
  TParam<int> owner;
  ParamsMgr.PushParam(owner,  owner, PAC_None, EPStore::NVS, "");

  TParam<int> param;
  ParamsMgr.PushParam(param, owner, PAC_System, EPStore::RAM, "system");

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
  TEST_ASSERT_EQUAL_INT(2, ParamsMgr.GetParamCount());
}

// Test that a duplicate alias is not assigned to the second parameter.
void test_ParamsMgr_SameAlias(void) {
  AppMapMgr.Reset();
  ParamsMgr.Reset();
  TParam<int> owner;
  ParamsMgr.PushParam(owner,  owner, PAC_System, EPStore::NVS, "");

  // First parameter: add with alias "site".
  TParam<int> param1;
  ParamsMgr.PushParam(param1, owner, PAC_Site, EPStore::NVS, "site");
  TEST_ASSERT_EQUAL_STRING("site", param1.GetAlias().c_str());
  TEST_ASSERT_EQUAL_INT(1, param1.GetPID());

  // Second parameter: try to add with the same alias.
  TParam<int> param2;
  ParamsMgr.PushParam(param2, owner, PAC_Zone, EPStore::NVS, "site");
  // Since the alias "site" already exists, the second parameter should not receive it.
  TEST_ASSERT_EQUAL_STRING("", param2.GetAlias().c_str());
  TEST_ASSERT_EQUAL_INT(2, param2.GetPID());

  // Verify that the manager now holds two parameters.
  TEST_ASSERT_EQUAL_INT(3, ParamsMgr.GetParamCount());
}

// Test that a duplicate alias is not assigned to the second parameter.
void test_ParamsMgr_AutoIndexing(void) {
  AppMapMgr.Reset();
  ParamsMgr.Reset();
  TParam<int> owner;

  TParam<int> param1;
  TParam<int> param2;
  TParam<int> param3;

  ParamsMgr.PushParam(owner,  owner, PAC_System, EPStore::NVS, "");
  ParamsMgr.PushParam(param1, owner, PAC_Site, EPStore::NVS, "");
  ParamsMgr.PushParam(param2, owner, PAC_Site, EPStore::NVS, "");
  ParamsMgr.PushParam(param3, owner, PAC_Site, EPStore::NVS, "");

  TEST_ASSERT_EQUAL_INT(0, owner.GetIdx());
  TEST_ASSERT_EQUAL_INT(0, param1.GetIdx());
  TEST_ASSERT_EQUAL_INT(1, param2.GetIdx());
  TEST_ASSERT_EQUAL_INT(2, param3.GetIdx());

  TEST_ASSERT_EQUAL_INT(0, owner.GetPID());
  TEST_ASSERT_EQUAL_INT(1, param1.GetPID());
  TEST_ASSERT_EQUAL_INT(2, param2.GetPID());
  TEST_ASSERT_EQUAL_INT(3, param3.GetPID());

  // Verify that the manager now holds two parameters.
  TEST_ASSERT_EQUAL_INT(4, ParamsMgr.GetParamCount());
}

void test_ParamsMgr_AutoAliasing(void){
  AppMapMgr.Reset();
  ParamsMgr.Reset();
  TParam<int> owner;

  TParam<int> param1;
  TParam<int> param2;
  TParam<int> param3;

  ParamsMgr.PushParam(owner,  owner, PAC_System, EPStore::NVS, "*");
  ParamsMgr.PushParam(param1, owner, PAC_Site, EPStore::NVS, "*");
  ParamsMgr.PushParam(param2, owner, PAC_Site, EPStore::NVS, "*");
  ParamsMgr.PushParam(param3, owner, PAC_Site, EPStore::NVS, "*");

  TEST_ASSERT_EQUAL_STRING(owner.GetDescr()->Classname, owner.GetAlias().c_str());
  TEST_ASSERT_EQUAL_STRING(param1.GetDescr()->Classname, param1.GetAlias().c_str());
  TEST_ASSERT_EQUAL_STRING("", param2.GetAlias().c_str());
  TEST_ASSERT_EQUAL_STRING("", param3.GetAlias().c_str());

}

void test_ParamsMgr_InitParamPID(void){
  AppMapMgr.Reset();
  ParamsMgr.Reset();
  TParam<int> owner;

  ParamsMgr.PushParam(owner,  owner, PAC_System, EPStore::NVS, "*");

  // The owner pid should be auto assigned to 0 
  TEST_ASSERT_EQUAL_INT(0, owner.GetPID());
  TEST_ASSERT_EQUAL_INT(0, owner.GetOwner());
  TEST_ASSERT_EQUAL_INT(0, owner.GetIdx());
}

void test_ParamsMgr_NonExistent(void) {
  AppMapMgr.Reset();
  ParamsMgr.Reset();
  TParam<int> none, system;
  ParamsMgr.PushParam(none, none, PAC_None, EPStore::NVS, "None");
  ParamsMgr.PushParam(system, none, PAC_System, EPStore::NVS, "System");
  
  int pid = ParamsMgr.FindPidFromAlias("NonExistent");
  TEST_ASSERT_EQUAL(UNSET_PID, pid);
}

void test_ParamsMgr_InitDefault(void) {
  AppMapMgr.Reset();
  ParamsMgr.Reset();
  TParam<int> none, system, siteID, outputMax, softwareVersion;
  ParamsMgr.PushParam(none, none, PAC_None, EPStore::NVS, "None");
  ParamsMgr.PushParam(system, none, PAC_System, EPStore::NVS, "System");
  ParamsMgr.PushParam(siteID, system, PAC_SiteID, EPStore::NVS, "*");
  ParamsMgr.PushParam(outputMax, system, PAC_OutputMax, EPStore::NVS, "*");
  ParamsMgr.PushParam(softwareVersion, system, PAC_SoftwareVersion, EPStore::NVS, "*");
  
  TEST_ASSERT_EQUAL(0, none.GetValue());
  TEST_ASSERT_EQUAL(0, system.GetValue());
  TEST_ASSERT_EQUAL(-1, siteID.GetValue());
  TEST_ASSERT_EQUAL(100, outputMax.GetValue());
  TEST_ASSERT_EQUAL(-1, softwareVersion.GetValue());
}

void test_ParamsMgr_GetChanges(void) {
  AppMapMgr.Reset();
  ParamsMgr.Reset();
  TParam<int> none, system, siteID, outputMax, softwareVersion;
  ParamsMgr.PushParam(none, none, PAC_None, EPStore::NVS, "None");
  ParamsMgr.PushParam(system, none, PAC_System, EPStore::NVS, "System");
  ParamsMgr.PushParam(siteID, system, PAC_SiteID, EPStore::NVS, "*");
  ParamsMgr.PushParam(outputMax, system, PAC_OutputMax, EPStore::NVS, "*");
  ParamsMgr.PushParam(softwareVersion, system, PAC_SoftwareVersion, EPStore::NVS, "*");

  // we set to true to farce the value to be changed because some parameters are not settable!! 
  none.SetValue(1, true);
  system.SetValue(1, true);
  siteID.SetValue(1, true);
  outputMax.SetValue(1, true);
  softwareVersion.SetValue(1, true);

  std::bitset<MAX_NUM_OF_PARAMS> bits;
  std::string changes;
  ParamsMgr.GetChanges(changes, bits);

  TEST_ASSERT_EQUAL_STRING("0,1\n1,1\n2,1\n3,1\n4,1\n", changes.c_str());
  TEST_ASSERT_EQUAL(true, bits.test(0));
  TEST_ASSERT_EQUAL(true, bits.test(1));
  TEST_ASSERT_EQUAL(true, bits.test(2));
  TEST_ASSERT_EQUAL(true, bits.test(3));
  TEST_ASSERT_EQUAL(true, bits.test(4));

  ParamsMgr.ClearChanges(bits);
  TEST_ASSERT_EQUAL(false, bits.test(0));
  TEST_ASSERT_EQUAL(false, bits.test(1));
  TEST_ASSERT_EQUAL(false, bits.test(2));
  TEST_ASSERT_EQUAL(false, bits.test(3));
  TEST_ASSERT_EQUAL(false, bits.test(4));

  AppMapMgr.Save("brooderApp/D2BrooderAlarm/appmapTest/testParamsMgr.csv");
}


//---------------------------------------------------------------------
// main() for native tests
//---------------------------------------------------------------------
void test_TParamsMgr(void) {
  RUN_TEST(test_ParamsMgr_UniqueAlias);
  RUN_TEST(test_ParamsMgr_SameAlias);
  RUN_TEST(test_ParamsMgr_AutoIndexing);
  RUN_TEST(test_ParamsMgr_AutoAliasing);
  RUN_TEST(test_ParamsMgr_InitParamPID);
  RUN_TEST(test_ParamsMgr_NonExistent);
  RUN_TEST(test_ParamsMgr_InitDefault);
  RUN_TEST(test_ParamsMgr_GetChanges);
}
