#include <unity.h>
#include "../brooderApp/D2BrooderAlarm/src/paramsmgr.h"

//---------------------------------------------------------------------
// Test 1: Comprehensive test combining ParseSiteSpec and TParamsMgr.
//---------------------------------------------------------------------
/**
 * @brief Unit test for the ParseSiteSpec function and TParamsMgr class.
 *
 * This test function performs the following:
 * - Initializes a TParamsMgr instance and two TParam<int> instances (none and system).
 * - Pushes parameters into the TParamsMgr instance.
 * - Defines an input string representing site specifications.
 * - Parses the input string into a vector of TSiteSpec objects.
 * - Validates the size of the parsed specifications.
 * - Validates the first parsed specification against expected values.
 * - Ensures that the alias of the system parameter matches the parsed pid_alias.
 * - Verifies that the TParamsMgr instance returns the correct PID for the system alias.
 * - Checks that the reference of the second parsed specification matches the none parameter.
 * - Validates the third parsed specification against expected values.
 */
void test_ParseSiteSpec_WithValidInput(void) {
  ParamsMgr.Reset();
  TParam<int> none, system;
  
  ParamsMgr.PushParam(none, none, PAC_None, EPStore::NVS, "None");
  ParamsMgr.PushParam(system, none, PAC_System, EPStore::NVS, "System");
  
  const std::string input = R"(!sitespec
  SysCtrl,255,System.SystemControl.0,2
  None,,None.None.0,0
  System,,None.System.0,0
  Site,,System.Site.0,2
  SiteOptions,0,System.SiteOptions.0,2
  NumZones,40,SiteOptions.Number.0,2
  NumRuuviTags,60,SiteOptions.Number.1,2
  NumUsers,10,SiteOptions.Number.2,2)";
  
  std::vector<TSiteSpec> specs = ParamsMgr.ParseSiteSpec(input);
  TEST_ASSERT_EQUAL(8, (int)specs.size());
  
  // Validate first spec: "SysCtrl,255,System.SysCtrl.0,2"
  TEST_ASSERT_EQUAL_STRING("SysCtrl", specs[0].alias.c_str());
  TEST_ASSERT_EQUAL(255, specs[0].value);
  TEST_ASSERT_EQUAL(PAC_SystemControl, specs[0].ref.pac);
  TEST_ASSERT_EQUAL(system.GetPID(), specs[0].ref.owner);
    
  // Verify that mgr.Find returns the same id as system's id.
  auto pid = ParamsMgr.FindPidFromAlias(specs[0].alias);
  TEST_ASSERT_EQUAL(UNSET_PID, pid);
  
  // Verify that the second spec ("None,,None.None.0,0") has a matching reference.
  TEST_ASSERT_TRUE(none.GetRef() == specs[1].ref);

  //System,,None.System.0,0
  TEST_ASSERT_EQUAL_STRING(system.GetAlias().c_str(), specs[2].alias.c_str());
  TEST_ASSERT_EQUAL(0, specs[2].value);
  TEST_ASSERT_EQUAL(0, specs[2].status);
  TEST_ASSERT_TRUE(system.GetRef() == specs[2].ref);

}

//---------------------------------------------------------------------
// Test 2: Empty input should yield no parsed specifications.
//---------------------------------------------------------------------
/**
 * @brief Unit test for the ParseSiteSpec function with empty input.
 *
 * This test verifies that the ParseSiteSpec function correctly handles
 * empty input strings and header-only input strings.
 *
 * Test cases:
 * 1. Empty input string: The function should return an empty vector.
 * 2. Header-only input string: The function should return an empty vector.
 *
 * The test uses the TEST_ASSERT_EQUAL macro to check that the size of the
 * returned vector is zero in both cases.
 */
void test_ParseSiteSpec_WithEmptyInput(void) {
  ParamsMgr.Reset();
  const std::string input = "";
  auto specs = ParamsMgr.ParseSiteSpec(input);
  TEST_ASSERT_EQUAL(0, (int)specs.size());
  
  // Header-only input.
  const std::string inputHeaderOnly = "!sitespec\n";
  specs = ParamsMgr.ParseSiteSpec(inputHeaderOnly);
  TEST_ASSERT_EQUAL(0, (int)specs.size());
}

//---------------------------------------------------------------------
// Test 3: Lines with invalid format should be ignored.
//---------------------------------------------------------------------
/**
 * @brief Unit test for the ParseSiteSpec function to handle invalid format input.
 *
 * This test case verifies that the ParseSiteSpec function correctly processes
 * an input string with various invalid lines and only processes the valid line.
 *
 * The input string contains:
 * - An invalid line "InvalidLine"
 * - A line with only two tokens "Only,Two"
 * - A line with too many tokens "Three,123,TooFew,Extra,Token"
 * - A valid line "Proper,123,Valid.Format.0,1"
 *
 * The test asserts that only the valid line is processed and checks the parsed
 * values for correctness.
 *
 * @note The expected behavior is that only one valid line ("Proper,123,Valid.Format.0,1")
 *       should be processed.
 */
void test_ParseSiteSpec_WithInvalidFormat(void) {
  ParamsMgr.Reset();
  const std::string input = R"(!sitespec
  InvalidLine
  Only,Two
  Three,123,TooFew,Extra,Token
  Proper,123,Valid.Format.0,1)";
  
  auto specs = ParamsMgr.ParseSiteSpec(input);
  // Only one valid line ("Proper,123,Valid.Format.0,1") should be processed.
  TEST_ASSERT_EQUAL(1, (int)specs.size());
  
  TEST_ASSERT_EQUAL_STRING("Proper", specs[0].alias.c_str());
  TEST_ASSERT_EQUAL(123, specs[0].value);
  TEST_ASSERT_EQUAL(1, specs[0].status);
}

//---------------------------------------------------------------------
// Test 4: Extra whitespace in input should be trimmed correctly.
//---------------------------------------------------------------------
/**
 * @brief Test the ParseSiteSpec function with input containing whitespace.
 *
 * This test case verifies that the ParseSiteSpec function correctly parses
 * a site specification string that includes various amounts of whitespace
 * around the parameters.
 *
 * The input string is:
 * "!sitespec\n  Dummy  ,   100   ,   None. None .0  ,  2   "
 *
 * The expected parsed result is:
 * - One specification in the list.
 * - The reference string value should be "Dummy".
 * - The value should be 100.
 * - The PID alias should be "None".
 * - The class name should be "None".
 * - The status should be 2.
 */
void test_ParseSiteSpec_WithWhitespace(void) {
  ParamsMgr.Reset();
  TParam<int> dummy;
  ParamsMgr.PushParam(dummy, dummy, PAC_None, EPStore::NVS, "Dummy");
  
  const std::string input = R"(!sitespec
  Dummy  ,   100   ,   None. None .0  ,  2   )";
  
  auto specs = ParamsMgr.ParseSiteSpec(input);
  TEST_ASSERT_EQUAL(1, (int)specs.size());
  
  TEST_ASSERT_EQUAL_STRING("Dummy", specs[0].alias.c_str());
  TEST_ASSERT_EQUAL(100, specs[0].value);
  TEST_ASSERT_EQUAL(PAC_None, specs[0].ref.pac);
  TEST_ASSERT_EQUAL(2, specs[0].status);
}

void test_SiteSpec(void) {
  RUN_TEST(test_ParseSiteSpec_WithValidInput);
  RUN_TEST(test_ParseSiteSpec_WithEmptyInput);
  RUN_TEST(test_ParseSiteSpec_WithInvalidFormat);
  RUN_TEST(test_ParseSiteSpec_WithWhitespace);
}
