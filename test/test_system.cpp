#include <unity.h>
#include "../brooderApp/D2BrooderAlarm/src/common.h"
#include "../brooderApp/D2BrooderAlarm/src/paramsmgr.h"
#include "../brooderApp/D2BrooderAlarm/src/system.h"
#include "../brooderApp/D2BrooderAlarm/src/system.cpp"
#include "../brooderApp/D2BrooderAlarm/src/zone.h"
#include "../brooderApp/D2BrooderAlarm/src/zone.cpp"
#include "../brooderApp/D2BrooderAlarm/src/alarm.h"
#include "../brooderApp/D2BrooderAlarm/src/alarm.cpp"

static const int numZones = 50;
static const int numRuuviTags = 10;
static std::string swVerRef = "";
static prid_t swVerPid = UNSET_PID;

const std::string sitespecdata = "!sitespec\n"
    "SysCtrl,255,System.SysCtrl.0,2\n"
    "None,,None.None.0,0\n"
    "System,,None.System.0,0\n"
    "Site,,System.Site.0,2\n"
    "SiteOptions1,0,System.SiteOptions.0,2\n"
    "NumZones," + std::to_string(numZones) + ",System.SiteOptions.1,2\n"
    "NumRuuviTags," + std::to_string(numRuuviTags) + ",System.SiteOptions.2,2\n"
    "NumUsers,10,System.SiteOptions.3,2";


#define TEST_APPMAP_CSV_FILE "brooderApp/D2BrooderAlarm/appmapTest/testSystem.csv"
#define TEST_APPMAP_SAVE_LOAD "brooderApp/D2BrooderAlarm/appmapTest/testAppMapSaveLoad.csv"
#define TEST_PARAMSMGR_SAVE_LOAD "brooderApp/D2BrooderAlarm/appmapTest/testParamsMgrSaveLoad.csv"

// Test that a unique alias is correctly assigned and fields are set.
void test_System_Build(void) {
  AppMapMgr.Reset();
  ParamsMgr.Reset();
  TSystem system;

  AppMapMgr.Load(TEST_APPMAP_CSV_FILE); // First we load appmap
  system.BuildSystemParams(); // Build core system parameters
  system.BuildSiteParams(); // Build site parameters
  system.BuildOtapParams(); // Build otap parameters

  swVerRef = FwMgr.swVer.GetRef().ToString();
  swVerPid = FwMgr.swVer.GetPID();
  TEST_ASSERT_EQUAL_STRING(swVerRef.c_str(), FwMgr.swVer.GetRef().ToString().c_str());

  auto sitespecs = ParamsMgr.ParseSiteSpec(sitespecdata);
  ParamsMgr.ApplySiteSpec(sitespecs);

  system.BuildZoneParams(); // Build the zone params
  system.BuildRuuviTagParams(); // Build the ruuvi tag params
  system.BuildUserParams(); // Build user parameters

  AppMapMgr.Save(TEST_APPMAP_CSV_FILE, true);

  auto zones = system.GetZones();
  TEST_ASSERT_EQUAL(numZones, zones->size());
  auto ruuvitags = system.GetRuuviTags();
  TEST_ASSERT_EQUAL(numRuuviTags, ruuvitags->size());

}

// Test system behavior when OTAP parameters are not built.
void test_System_No_OTAP(void) {
  AppMapMgr.Reset();
  ParamsMgr.Reset();
  TSystem system;

  AppMapMgr.Load(TEST_APPMAP_CSV_FILE); // First we load appmap
  system.BuildSystemParams(); // Build core system parameters
  system.BuildSiteParams(); // Build site parameters

  auto sitespecs = ParamsMgr.ParseSiteSpec(sitespecdata);
  ParamsMgr.ApplySiteSpec(sitespecs);

  system.BuildZoneParams(); // Build the zone params
  system.BuildRuuviTagParams(); // Build the ruuvi tag params

  // AppMapMgr.Save(TEST_APPMAP_CSV_FILE, true);

  auto appmapswverPID = AppMapMgr.Find(swVerRef);
  TEST_ASSERT_EQUAL(swVerPid, appmapswverPID); // Check that the PID is correctly assigned even if the OTAP parameters are not built.

  auto appmapswverData = AppMapMgr.GetAppMapData(appmapswverPID); // Check that the data is correctly assigned even if the OTAP parameters are not built.
  TEST_ASSERT_NOT_NULL(appmapswverData); 
  TEST_ASSERT_EQUAL_STRING(swVerRef.c_str(), appmapswverData->ref.c_str()); 
  TEST_ASSERT_EQUAL(swVerPid, std::stoi(appmapswverData->pid));
  TEST_ASSERT_EQUAL_STRING(FwMgr.swVer.GetAlias().c_str(), appmapswverData->alias.c_str());
  TEST_ASSERT_EQUAL_STRING("1", appmapswverData->status.c_str()); // should be inactive 

  system.BuildOtapParams(); // Build otap parameters
  TEST_ASSERT_EQUAL_STRING("0", appmapswverData->status.c_str()); // should be active again

}

// Test that a newly added parameter has the correct status.
void test_System_newparam_status(void) {
  AppMapMgr.Reset();
  ParamsMgr.Reset();
  TSystem system;

  AppMapMgr.Load(TEST_APPMAP_CSV_FILE); // First we load appmap
  system.BuildSystemParams(); // Build core system parameters

  TParam<uint32_t> newparam(0);
  ParamsMgr.PushParam(newparam, sp.none, PAC_Number, EPStore::RAM, "NewParam");

  auto newparamPID = AppMapMgr.Find(newparam.GetRef().ToString());
  TEST_ASSERT_NOT_EQUAL(UNSET_PID, newparamPID); // Check that the PID is correctly assigned 

  auto newparamData = AppMapMgr.GetAppMapData(newparamPID); // Check that the data is correctly assigned
  TEST_ASSERT_NOT_NULL(newparamData);
  TEST_ASSERT_EQUAL_STRING("4", newparamData->status.c_str()); // should be new

}

// Test saving and loading parameter values.
void test_System_SaveLoad(void){
  AppMapMgr.Reset();
  ParamsMgr.Reset();
  TParam<uint32_t> none(0);
  TParam<uint32_t> sys(0);
  TParam<uint32_t> site(0);
  TParam<int> testInt(0);
  TParam<std::string> testStr("Init Value");

  AppMapMgr.Load(TEST_APPMAP_SAVE_LOAD); // First we load appmap
  
  auto buildParams = [&]() {
    none = 0;
    sys = 0;
    site = 0;
    testInt = 0;
    testStr = "Init Value";
    _PP(none, none, PAC_None, EPStore::RAM, "None");
    _PP(sys, none, PAC_System, EPStore::RAM, "Sys");
    _PP(site, sys, PAC_Site, EPStore::RAM, "Site");
    _PP(testInt, sys, PAC_BitMap, EPStore::NVS, "TestInt");
    _PP(testStr, sys, PAC_String, EPStore::NVS, "TestStr");
  };

  buildParams();

  // generate random values
  int randInt = random();
  std::string randStr = "Random Value " + std::to_string(randInt);

  testInt = randInt;
  testStr = randStr;
  none = randInt;
  site = randInt;

  AppMapMgr.Save(TEST_APPMAP_SAVE_LOAD, true);
  ParamsMgr.SaveStorage(TEST_PARAMSMGR_SAVE_LOAD);

  TEST_ASSERT_NOT_EQUAL(randInt, none.GetValue());  // none is read only, thus it cannot be signed a new value
  TEST_ASSERT_EQUAL(0, sys.GetValue());
  TEST_ASSERT_EQUAL(randInt, site.GetValue()); // site is RW thus it can be signed a new value
  TEST_ASSERT_EQUAL(randInt, testInt.GetValue());
  TEST_ASSERT_EQUAL_STRING(randStr.c_str(), testStr.GetValue().c_str());
  TEST_ASSERT_EQUAL_STRING(std::to_string(randInt).c_str(), testInt.GetValAsStr().c_str());
  TEST_ASSERT_EQUAL_STRING(randStr.c_str(), testStr.GetValAsStr().c_str());

  // Reset and load the saved values
  AppMapMgr.Reset();
  ParamsMgr.Reset();
  AppMapMgr.Load(TEST_APPMAP_SAVE_LOAD); // First we load appmap
  buildParams();
  ParamsMgr.LoadStorage(TEST_PARAMSMGR_SAVE_LOAD);

  TEST_ASSERT_EQUAL(0, sys.GetValue());
  TEST_ASSERT_NOT_EQUAL(randInt, site.GetValue()); 
  TEST_ASSERT_EQUAL(0, site.GetValue()); // site is not saved, this should be 0
  TEST_ASSERT_EQUAL(randInt, testInt.GetValue());
  TEST_ASSERT_EQUAL_STRING(randStr.c_str(), testStr.GetValue().c_str());
}

//---------------------------------------------------------------------
// main() for native tests
//---------------------------------------------------------------------
void test_TSystem(void) {
  RUN_TEST(test_System_Build);
  RUN_TEST(test_System_No_OTAP);
  RUN_TEST(test_System_newparam_status);
  RUN_TEST(test_System_SaveLoad);
}
