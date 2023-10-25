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
 * @copyright Copyright (C) 2023 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/ApiElementRetrievalInterface.h>

namespace hoot
{

class ApiElementRetrievalInterfaceTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ApiElementRetrievalInterfaceTest);
  CPPUNIT_TEST(checkOsmQueryTest);
  CPPUNIT_TEST(CheckOverpassQueryTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ApiElementRetrievalInterfaceTest() = default;

  const QString overpass_url = "https://overpass-api.de/api/interpreter";
  const QString osm_api_url = "https://api.openstreetmap.org/api/0.6/map";

  void checkOsmQueryTest()
  {
    ApiElementRetrievalInterface uut;
    std::set<ElementId> missing;
    missing.insert(ElementId(ElementType::Way, 12345));
    missing.insert(ElementId(ElementType::Relation, 54321));
    uut.setMissingElements(missing);

    std::vector<QString> osm_queries(
      {
        "https://api.openstreetmap.org/api/0.6/way/12345/full",
        "https://api.openstreetmap.org/api/0.6/relation/54321/full"
      });

    int i = 0;
    while (uut.hasMoreQueries())
    {
      QString query = uut.getNextQuery(osm_api_url);
      HOOT_STR_EQUALS(osm_queries[i], query);
      ++i;
    }
  }

  void CheckOverpassQueryTest()
  {
    ApiElementRetrievalInterface uut;
    std::set<ElementId> missing;
    missing.insert(ElementId(ElementType::Way, 12345));
    missing.insert(ElementId(ElementType::Relation, 54321));
    uut.setMissingElements(missing);

    std::vector<QString> osm_queries(
      {
        "https://overpass-api.de/api/interpreter?data=[out:xml];(way(id:12345);(._;>;););out body;",
        "https://overpass-api.de/api/interpreter?data=[out:xml];(relation(id:54321);(._;>;););out body;"
      });

    int i = 0;
    while (uut.hasMoreQueries())
    {
      QString query = uut.getNextQuery(overpass_url);
      HOOT_STR_EQUALS(osm_queries[i], query);
      ++i;
    }
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ApiElementRetrievalInterfaceTest, "quick");

}
