#include <unity.h>
#include "../brooderApp/D2BrooderAlarm/src/ruuvitag.h"
#include "../brooderApp/D2BrooderAlarm/src/paramsmgr.h"

// #include "../brooderApp/D2BrooderAlarm/src/ruuvitag.cpp"

#define TEST_APPMAP_CSV_FILE "brooderApp/D2BrooderAlarm/appmapTest/testSystem.csv"

void test_ruuvidata_int() {
  AppMapMgr.Reset();
  ParamsMgr.Reset();

  TRuuviTagData tag;
  TParam<int> owner;
  ParamsMgr.PushParam(owner,  owner, PAC_None, EPStore::NVS, "");

  tag.Build(owner);
  TEST_ASSERT_EQUAL(ID_READERR_I16, tag.temp.GetValue());
  TEST_ASSERT_EQUAL(ID_READERR_I16, tag.rh.GetValue());
  TEST_ASSERT_EQUAL(D2_READERR_U16, tag.batt.GetValue());
  TEST_ASSERT_EQUAL(D2_READERR_U16, tag.battLevel.GetValue());
  TEST_ASSERT_EQUAL(D2_READERR_U16, tag.m_seq.GetValue());
  TEST_ASSERT_EQUAL(ID_READERR_I16, tag.rssi.GetValue());
  TEST_ASSERT_EQUAL(0, tag.alarmFlags.GetValue());
  TEST_ASSERT_EQUAL(0, tag.alarmMask.GetValue());
  TEST_ASSERT_EQUAL_STRING("", tag.name.GetValue().c_str());
  TEST_ASSERT_EQUAL(RUUVI_BV_LOW_MV, tag.GetmvLow());
  TEST_ASSERT_EQUAL(RUUVI_BV_HIGH_MV, tag.GetmvHigh());
  TEST_ASSERT_NULL(tag.siteAlarmMask);
  TEST_ASSERT_NULL(tag.siteAlarmFlags);
  TEST_ASSERT_NULL(tag.battTH);
}

//---------------------------------------------------------------------
// main() for native tests
//---------------------------------------------------------------------
void test_TRuuviTagData(void) {
  RUN_TEST(test_ruuvidata_int);
}
