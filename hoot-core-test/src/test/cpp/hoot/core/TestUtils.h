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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef TESTUTILS_H
#define TESTUTILS_H

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// hoot
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>

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
   * @brief resetConfigs Reset the configuration from the default, Testing.conf, and input files
   * @param confs custom config files to load during reset; if left blank the default config
   * in ConfigOptions will be loaded
   */
  static void resetConfigs(const QStringList confs = QStringList());
  /**
   * @brief resetEnvironment Resets the test environment to a known state.
   * @param confs custom confs to load during reset; if left blank the default config in
   * ConfigOptions will be loaded
   */
  static void resetEnvironment(const QStringList confs = QStringList());

  static bool compareMaps(OsmMapPtr map1, OsmMapPtr map2);
  static bool compareMaps(const QString& map1, const QString& map2);

  static void dumpString(const std::string& str);
  static std::string readFile(QString f1);
  /**
   * Converts a string into a format that can be cut/paste into c++ code.
   */
  static QString toQuotedString(QString str);
  static void verifyStdMatchesOutputIgnoreDate(
    const QString& stdFilePath, const QString& outFilePath);

  static NodePtr createNode(
    const OsmMapPtr& map, const QString& note = "", const Status& status = Status::Unknown1,
    const double x = 0.0, const double y = 0.0,
    const Meters circularError = ConfigOptions().getCircularErrorDefaultValue(),
    const Tags& tags = Tags());

  static WayPtr createWay(
    const OsmMapPtr& map, const geos::geom::Coordinate c[] = nullptr,
    const QString& note = "", const Status& s = Status::Unknown1,
    const Meters circularError = ConfigOptions().getCircularErrorDefaultValue(),
    const Tags& tags = Tags());
  static WayPtr createWay(
    const OsmMapPtr& map, const QList<NodePtr>& nodes = QList<NodePtr>(),
    const QString& note = "", const Status& status = Status::Unknown1,
    const Meters circularError = ConfigOptions().getCircularErrorDefaultValue(),
    const Tags& tags = Tags());
  static WayPtr createWay(
    const OsmMapPtr& map, const QList<ElementId>& nodeIds = QList<ElementId>(),
    const QString& note = "", const Status& status = Status::Unknown1,
    const Meters circularError = ConfigOptions().getCircularErrorDefaultValue(),
    const Tags& tags = Tags());

  static RelationPtr createRelation(
    const OsmMapPtr& map, const QList<ElementPtr>& elements = QList<ElementPtr>(),
    const QString& note = "", const Status& status = Status::Unknown1,
    const Meters circularError = ConfigOptions().getCircularErrorDefaultValue(),
    const Tags& tags = Tags());
  static RelationPtr createRelation(
    const OsmMapPtr& map, const QList<ElementId>& elementIds = QList<ElementId>(),
    const QString& note = "", const Status& status = Status::Unknown1,
    const Meters circularError = ConfigOptions().getCircularErrorDefaultValue(),
    const Tags& tags = Tags());

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
   * @brief runConflateOpReductionTest Runs a conflate op reduction test which tests for which superfluous conflate pre/post/cleaning
   * ops are removed by SuperfluousConflateOpRemover. This is in TestUtils b/c it is shared by
   * SuperfluousConflateOpRemoveTest in hoot-core and SuperfluousConflateOpRemoveJsTest in hoot-js.
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

  TestUtils();

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

class HootTestFixture : public CppUnit::TestFixture
{
protected:

  /** Each Reset* builds on the prior, Configs also resets Basic, Environment rests Basic and Configs, etc. */
  enum HootTestReset
  {
    ResetBasic,         // resets counters
    ResetConfigs,       // resets configurations
    ResetEnvironment,   // resets entire environment except for MatchFactory (see additional
                        // explanation in resetEnvironment)
    ResetAll            // resets entire environment (config, etc.); This can be fairly resource
                        // expensive due to the call to ScriptMatchCreator::setArguments but
                        // only if you have script matchers configured to run.
  };

  /**
   *  @brief Constructor to set the paths to begin with $HOOT_HOME if used, default reset to none,
   *  and create the output path if needed
   */
  HootTestFixture(const QString& inputPath = UNUSED_PATH, const QString& outputPath = UNUSED_PATH);
  virtual ~HootTestFixture() = default;

  /**
   * @brief setResetType Set the reset type to do basic, all, or none
   * @param reset Enum type to resent
   */
  void setResetType(HootTestReset reset) { _reset = reset; }

  /**
   * @brief getEnvString Record the current state of the environment for comparison
   * @return the environment as a string
   */
  QString getEnvString();

  /** Path relative from $HOOT_HOME to the input folder of the test */
  const QString _inputPath;
  /** Path relative from $HOOT_HOME to the output folder of the test */
  const QString _outputPath;

public:

  void setUp() override;

  void tearDown() override;

  static void setCompareEnv(bool compare) { _compareEnv = compare; }

  static const QString UNUSED_PATH;

private:

  /** Reset flag on setup to reset nothing, basic IDs, or everything */
  HootTestReset _reset;
  /** Environment string for environment comparison for this test */
  QString _defaultEnv;
  /** Compare default the environment to the environment after the test, useful in debugging tests */
  static bool _compareEnv;
};

#define TEST_UTILS_REGISTER_RESET(ClassName)      \
  static hoot::AutoRegisterResetInstance<ClassName> ClassName##AutoRegisterReset;

}

#endif // TESTUTILS_H
