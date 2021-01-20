/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef TESTUTILS_H
#define TESTUTILS_H

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/FileUtils.h>

// Qt
#include <QString>

// Standard
#include <string>
#include <sstream>

inline QString toQString(const QString& s)
{
  return s;
}

inline QString toQString(const char* s)
{
  return QString::fromUtf8(s);
}

inline QString toQString(const std::string& s)
{
  return QString::fromStdString(s);
}

#define HOOT_STR_EQUALS(expected, actual) \
{ \
  std::stringstream ss1, ss2; \
  ss1 << expected; \
  ss2 << actual; \
  CPPUNIT_ASSERT_EQUAL(ss1.str(), ss2.str()); \
} \

#define HOOT_FILE_EQUALS(expected, actual)              \
{ \
  QString expectedStr = toQString(expected); \
  QString actualStr = toQString(actual); \
  if (expectedStr.endsWith(".osm") && actualStr.endsWith(".osm")) \
  { \
    if (TestUtils::compareMaps(expected, actual) == false) \
    { \
      CPPUNIT_FAIL((QString("Maps do not match: ") + \
        expected + " " + actual).toStdString()); \
    } \
  } \
  else if (TestUtils::readFile(expected) != TestUtils::readFile(actual)) \
  { \
    CPPUNIT_FAIL((QString("Files do not match: ") + \
      expected + " " + actual).toStdString()); \
  } \
} \

namespace hoot
{

class TestUtils
{
public:

  class RegisteredReset
  {
    public:

      virtual void reset() = 0;
  };

  TestUtils();

  static void dumpString(const std::string& str);

  static std::string readFile(QString f1);

  static bool compareMaps(OsmMapPtr map1, OsmMapPtr map2);

  static bool compareMaps(const QString& map1, const QString& map2);

  static NodePtr createNode(OsmMapPtr map, Status status, double x, double y,
    Meters circularError = ConfigOptions().getCircularErrorDefaultValue(), Tags tags = Tags());

  static WayPtr createWay(
    OsmMapPtr map, Status s, geos::geom::Coordinate c[],
    Meters circularError = ConfigOptions().getCircularErrorDefaultValue(),
    const QString& note = "");

  static WayPtr createWay(
    OsmMapPtr map, geos::geom::Coordinate c[], Status status = Status::Unknown1,
    Meters circularError = ConfigOptions().getCircularErrorDefaultValue(), Tags tags = Tags());

  static WayPtr createWay(
    OsmMapPtr map, const QList<NodePtr>& nodes, Status status = Status::Unknown1,
    Meters circularError = ConfigOptions().getCircularErrorDefaultValue(), Tags tags = Tags());

  /*
   * For creating a way where you just need to operate on its tags and
   * don't care about the geometric aspect of it.
   */
  static WayPtr createDummyWay(OsmMapPtr map, Status status = Status::Unknown1);

  static RelationPtr createRelation(
    OsmMapPtr map, const QList<ElementPtr>& elements, Status status = Status::Unknown1,
    Meters circularError = ConfigOptions().getCircularErrorDefaultValue(), Tags tags = Tags());

  static ElementPtr getElementWithNote(OsmMapPtr map, QString note);

  /**
   * Gets a single element by tag
   *
   * @param map map containing the element
   * @param tagKey tag key to search for
   * @param tagValue tag value to search for
   * @return Returns a single element with the intput tag key/value pair; fails if more than one
   * element
   */
  static ElementPtr getElementWithTag(
    OsmMapPtr map, const QString& tagKey, const QString& tagValue);

  /**
   * Return the singleton instance.
   */
  static TestUtils& getInstance();

  /**
   * Register a way to reset the environment. This is most useful in plugins to avoid circular
   * references.
   */
  void registerReset(RegisteredReset* r) { _resets.append(r); }

  /**
   * Resets the test environment including counters, keys, and seeds
   */
  static void resetBasic();

  /**
   * Resets the test environment to a known state.
   *
   * @param confs custom confs to load during reset; if left blank the default config in
   * ConfigOptions will be loaded
   */
  static void resetEnvironment(const QStringList confs = QStringList());

  /**
   * Converts a string into a format that can be cut/paste into c++ code.
   */
  static QString toQuotedString(QString str);

  static void verifyStdMatchesOutputIgnoreDate(
    const QString& stdFilePath, const QString& outFilePath);

  /**
   * This is a snapshot of the option, conflate.pre.ops (circa 2/12/20), for testing purposes.
   *
   * @return a list of operator class names
   */
  static QStringList getConflateCmdSnapshotPreOps();

  /**
   * This is a snapshot of the option, conflate.post.ops (circa 2/12/20), for testing purposes.
   *
   * @return a list of operator class names
   */
  static QStringList getConflateCmdSnapshotPostOps();

  /**
   * This is a snapshot of the option, map.cleaner.transforms (circa 2/12/20), for testing purposes.
   *
   * @return a list of operator class names
   */
  static QStringList getConflateCmdSnapshotCleaningOps();

  /**
   * Runs a conflate op reduction test which tests for which superfluous conflate pre/post/cleaning
   * ops are removed by SuperfluousConflateOpRemover. This is in TestUtils b/c it is shared by
   * SuperfluousConflateOpRemoveTest in hoot-core and SuperfluousConflateOpRemoveJsTest in hoot-js.
   *
   * @param matchCreators the match creator class names involved in the conflation job
   * @param expectedPreOpSize the expected number of conflation pre ops after op reduction
   * @param expectedPostOpsSize the expected number of conflation post ops after op reduction
   * @param expectedCleaningOpsSize the expected number of conflation cleaning ops after op
   * reduction
   */
  static void runConflateOpReductionTest(
    const QStringList& matchCreators, const int expectedPreOpSize, const int expectedPostOpsSize,
    const int expectedCleaningOpsSize);

private:

  QList<RegisteredReset*> _resets;

  static std::shared_ptr<TestUtils> _theInstance;
};

/**
 * Auto-register reset for a class.
 */
template<class T>
class AutoRegisterResetInstance : public TestUtils::RegisteredReset
{
public:

  AutoRegisterResetInstance()
  {
    TestUtils::getInstance().registerReset(this);
  }

  virtual ~AutoRegisterResetInstance() = default;

  virtual void reset()
  {
    T::reset();
  }
};

#define TEST_UTILS_REGISTER_RESET(ClassName)      \
  static hoot::AutoRegisterResetInstance<ClassName> ClassName##AutoRegisterReset;

class HootTestFixture : public CppUnit::TestFixture
{
protected:

  enum HootTestReset
  {
    ResetNone,
    ResetBasic,
    ResetAll
  };

  /** Constructor to set the paths to begin with $HOOT_HOME if used, default reset to none,
   *  and create the output path if needed
   */
  HootTestFixture(const QString& inputPath = UNUSED_PATH, const QString& outputPath = UNUSED_PATH)
    : _inputPath((inputPath != UNUSED_PATH) ? ConfPath::getHootHome() + "/" + inputPath : inputPath),
      _outputPath((outputPath != UNUSED_PATH) ? ConfPath::getHootHome() + "/" + outputPath : outputPath),
      _reset(ResetNone)
  {
    if (outputPath != UNUSED_PATH)
      FileUtils::makeDir(_outputPath);
  }

  virtual ~HootTestFixture() = default;

  /**
   * @brief setResetType Set the reset type to do basic, all, or none
   * @param reset Enum type to resent
   */
  void setResetType(HootTestReset reset) { _reset = reset; }

  /** Path relative from $HOOT_HOME to the input folder of the test */
  const QString _inputPath;
  /** Path relative from $HOOT_HOME to the output folder of the test */
  const QString _outputPath;

public:

  /**
   * @brief setUp Overload of the CppUnit::TextFixture::setUp() to reset Hootenanny environment
   */
  virtual void setUp()
  {
    if (_reset == ResetAll)
    {
      // resetEnvironment reloads Testing.conf, so we don't need to do it here.
      TestUtils::resetEnvironment();
    }
    else
    {
      if (_reset == ResetBasic)
      {
        TestUtils::resetBasic();
      }

      // We require that all tests use Testing.conf as a starting point and any conf values
      // specified by it may be overridden when necessary.
      conf().loadJson(ConfPath::search("Testing.conf"));
    }
  }

  static const QString UNUSED_PATH;

private:

  /** Reset flag on setup to reset nothing, basic IDs, or everything */
  HootTestReset _reset;
};

}

#endif // TESTUTILS_H
