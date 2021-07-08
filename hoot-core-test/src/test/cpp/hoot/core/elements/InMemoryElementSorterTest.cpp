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
 * @copyright Copyright (C) 2012, 2013, 2014, 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/InMemoryElementSorter.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

class InMemoryElementSorterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(InMemoryElementSorterTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  InMemoryElementSorterTest() : HootTestFixture("test-files/elements/", UNUSED_PATH)
  {
  }

  void runTest()
  {
    //Since we're processing an unsorted map data file, we'll get some missing ref warnings
    //from the xml reader before its sorted that we don't care to see.
    DisableLog dl;

    OsmMapPtr inputMap = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(
      inputMap,
      _inputPath + "ExternalMergeElementSorterTest.osm",
      true);

    InMemoryElementSorter elementSorter(inputMap);

    int index = 0;
    while (elementSorter.hasMoreElements())
    {
      ElementPtr element = elementSorter.readNextElement();
      LOG_TRACE(element->toString());
      LOG_VART(index);
      //elements should be returned in the order nodes, ways, then relations
      if (index >= 0 && index <=15)
      {
        CPPUNIT_ASSERT(element->getElementType() == ElementType::Node);
      }
      else if (index >= 16 && index <= 19)
      {
        CPPUNIT_ASSERT(element->getElementType() == ElementType::Way);
      }
      if (index >= 20 && index <= 28)
      {
        CPPUNIT_ASSERT(element->getElementType() == ElementType::Relation);
      }
      index++;
    }
    CPPUNIT_ASSERT_EQUAL(29, index);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(InMemoryElementSorterTest, "quick");

}



