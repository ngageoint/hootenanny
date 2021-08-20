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
 * @copyright Copyright (C) 2014, 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/visitors/TagKeyCountVisitor.h>
#include <hoot/core/visitors/KeepTagsVisitor.h>

namespace hoot
{

class KeepTagsVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(KeepTagsVisitorTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(runWildcardTest);
  CPPUNIT_TEST_SUITE_END();

public:

  KeepTagsVisitorTest() : HootTestFixture("test-files/visitors/", UNUSED_PATH)
  {
    setResetType(ResetBasic);
  }

  void runTest()
  {
    OsmMapPtr map = _loadMap();

    QStringList keysToKeep;
    keysToKeep.append("highway");
    keysToKeep.append("blah");
    KeepTagsVisitor visitor(keysToKeep);
    map->visitRw(visitor);

    std::shared_ptr<TagKeyCountVisitor> keyCountVisitor =
      std::make_shared<TagKeyCountVisitor>("source");
    map->visitRo(*keyCountVisitor);
    HOOT_STR_EQUALS("0", keyCountVisitor->getStat());

    keyCountVisitor = std::make_shared<TagKeyCountVisitor>("highway");
    map->visitRo(*keyCountVisitor);
    HOOT_STR_EQUALS("6", keyCountVisitor->getStat());

    keyCountVisitor = std::make_shared<TagKeyCountVisitor>("blah");
    map->visitRo(*keyCountVisitor);
    HOOT_STR_EQUALS("0", keyCountVisitor->getStat());
  }

  void runWildcardTest()
  {
    OsmMapPtr map = _loadMap();

    QStringList keysToKeep;
    keysToKeep.append("high*");
    keysToKeep.append("blah");
    KeepTagsVisitor visitor(keysToKeep);
    map->visitRw(visitor);

    std::shared_ptr<TagKeyCountVisitor> keyCountVisitor =
      std::make_shared<TagKeyCountVisitor>("source");
    map->visitRo(*keyCountVisitor);
    HOOT_STR_EQUALS("0", keyCountVisitor->getStat());

    keyCountVisitor = std::make_shared<TagKeyCountVisitor>("highway");
    map->visitRo(*keyCountVisitor);
    HOOT_STR_EQUALS("6", keyCountVisitor->getStat());

    keyCountVisitor = std::make_shared<TagKeyCountVisitor>("blah");
    map->visitRo(*keyCountVisitor);
    HOOT_STR_EQUALS("0", keyCountVisitor->getStat());
  }

private:

  OsmMapPtr _loadMap()
  {
    OsmXmlReader reader;
    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "TagRenameKeyVisitorTest.osm", map);
    return map;
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(KeepTagsVisitorTest, "quick");

}


