#include <unity.h>
#include "../brooderApp/D2BrooderAlarm/src/common.h"
#include "../brooderApp/D2BrooderAlarm/src/paramsmgr.h"
#include "../brooderApp/D2BrooderAlarm/src/system.h"
#include "../brooderApp/D2BrooderAlarm/src/system.cpp"

const std::string sitespecdata = R"(!sitespec
SysCtrl,255,System.SysCtrl.0,2
None,,None.None.0,0
System,,None.System.0,0
Site,,System.Site.0,2
SiteOptions1,0,System.SiteOptions.0,2
NumZones,40,System.SiteOptions.1,2
NumRuuviTags,60,System.SiteOptions.2,2
NumUsers,10,System.SiteOptions.3,2)";

#define TEST_APPMAP_CSV_FILE "brooderApp/D2BrooderAlarm/appmapTest/testSystem.csv"

// Test that a unique alias is correctly assigned and fields are set.
void test_System_Build(void) {
  AppMapMgr.Reset();
  ParamsMgr.Reset();
  TSystem system;

  AppMapMgr.Load(TEST_APPMAP_CSV_FILE); // First we load appmap
  system.BuildSystemParams(); // Build core system parameters

  auto sitespecs = ParamsMgr.ParseSiteSpec(sitespecdata);
  ParamsMgr.ApplySiteSpec(sitespecs);

  system.BuildZoneParams(); // Build the zone params
  system.BuildRuuviTagParams(); // Build the ruuvi tag params

  AppMapMgr.Save(TEST_APPMAP_CSV_FILE, true);

  auto zones = system.GetZones();
  TEST_ASSERT_EQUAL(40, zones->size());
  auto ruuvitags = system.GetRuuviTags();
  TEST_ASSERT_EQUAL(60, ruuvitags->size());


}


//---------------------------------------------------------------------
// main() for native tests
//---------------------------------------------------------------------
void test_TSystem(void) {
  RUN_TEST(test_System_Build);
}
