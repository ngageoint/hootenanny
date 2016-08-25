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
 * @copyright Copyright (C) 2012, 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef TESTUTILS_H
#define TESTUTILS_H

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// hoot
#include <hoot/core/OsmMap.h>

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

#define HOOT_STR_EQUALS(v1, v2) \
{ \
  stringstream ss1, ss2; \
  ss1 << v1; \
  ss2 << v2; \
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
using namespace std;

class TestUtils
{
public:
  class RegisteredReset
  {
  public:
    virtual void reset() = 0;
  };

  TestUtils();

  static void dumpString(const string& str);

  static std::string readFile(QString f1);

  static bool compareMaps(shared_ptr<OsmMap> map1, shared_ptr<OsmMap> map2);

  static bool compareMaps(const QString& map1, const QString map2);

  static NodePtr createNode(OsmMapPtr map, Status status, double x, double y,
    double circularError = 15.0, Tags tags = Tags());

  static WayPtr createWay(OsmMapPtr map, Status s, Coordinate c[], Meters ce = 15,
                          const QString& note = "");

  static WayPtr createWay(OsmMapPtr map, const QList<NodePtr>& nodes, Status status = Status::Unknown1,
    Meters circularError = 15, Tags tags = Tags());

  static RelationPtr createRelation(
    OsmMapPtr map, const QList<ElementPtr>& elements, Status status = Status::Unknown1,
    Meters circularError = 15);

  static ElementPtr getElementWithNote(OsmMapPtr map, QString note);

  /**
   * Gets a single element by tag
   *
   * @param map map containing the element
   * @param tagKey tag key to search for
   * @param tagValue tag value to search for
   * @return Returns a single element with the intput tag key/value pair; fails if more than one element
   */
  static ElementPtr getElementWithTag(OsmMapPtr map, const QString tagKey, const QString tagValue);

  /**
   * Return the singleton instance.
   */
  static shared_ptr<TestUtils> getInstance();

  /**
   * Register a way to reset the environment. This is most useful in plugins to avoid circular
   * references.
   */
  void registerReset(RegisteredReset* r) { _resets.append(r); }

  /**
   * Resets the test environment to a known state.
   */
  static void resetEnvironment();

  /**
   * Converts a string into a format that can be cut/paste into c++ code.
   */
  static QString toQuotedString(QString str);

private:
  QList<RegisteredReset*> _resets;

  static shared_ptr<TestUtils> _theInstance;
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
    TestUtils::getInstance()->registerReset(this);
  }

  virtual void reset()
  {
    T::reset();
  }
};

#define TEST_UTILS_REGISTER_RESET(ClassName)      \
  static hoot::AutoRegisterResetInstance<ClassName> ClassName##AutoRegisterReset;


}

#endif // TESTUTILS_H
