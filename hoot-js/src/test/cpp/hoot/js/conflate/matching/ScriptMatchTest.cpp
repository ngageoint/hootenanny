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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/elements/MapProjector.h>

#include <hoot/js/HootJsStable.h>
#include <hoot/js/conflate/matching/ScriptMatch.h>
#include <hoot/js/conflate/matching/ScriptMatchCreator.h>

// Qt
#include <QVariant>

using namespace std;
using namespace v8;

namespace hoot
{

class ScriptMatchTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ScriptMatchTest);
  CPPUNIT_TEST(conflictTest);
  CPPUNIT_TEST(cacheTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ScriptMatchTest() :
  HootTestFixture("test-files/js/conflate/matching/", UNUSED_PATH)
  {
  }

  void conflictTest()
  {
    ConfigOptions co;
    conf().set(co.getUuidHelperRepeatableKey(), true);
    conf().set(co.getReaderUseFileStatusKey(), true);
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, _inputPath + "ScriptMatchTest.osm", true);
    MapProjector::projectToPlanar(map);

    std::shared_ptr<const MatchThreshold> mt = std::make_shared<MatchThreshold>(0.6, 0.6, 0.6);

    ScriptMatchCreator smc;
    smc.setArguments(QStringList() << "Line.js");
    vector<ConstMatchPtr> matches;
    smc.createMatches(map, matches, mt);
    HOOT_STR_EQUALS(2, matches.size());
    HOOT_STR_EQUALS(true, matches[0]->isConflicting(matches[1], map));
  }

  void cacheTest()
  {
    ScriptMatch::ConflictKey key1(ElementId::way(-2), ElementId::way(-7));
    QHash<ScriptMatch::ConflictKey, bool> conflicts;

    ScriptMatch::ConflictKey key2(ElementId::way(-2), ElementId::way(-7));
    conflicts[key1] = true;

    HOOT_STR_EQUALS(false, (bool)(conflicts.find(key2) == conflicts.end()));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ScriptMatchTest, "quick");

}
