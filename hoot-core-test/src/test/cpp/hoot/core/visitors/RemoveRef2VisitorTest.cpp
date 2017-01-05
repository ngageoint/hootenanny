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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/filters/PoiCriterion.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/visitors/RemoveRef2Visitor.h>

// Qt
#include <QDebug>
#include <QDir>

// TGS
#include <tgs/Statistics/Random.h>

#include "../TestUtils.h"

namespace hoot
{
using namespace Tgs;

class RemoveRef2VisitorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(RemoveRef2VisitorTest);
  CPPUNIT_TEST(runToyTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runToyTest()
  {
    TestUtils::resetEnvironment();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::getInstance().read(map, "test-files/visitors/RemoveRef2VisitorInput.osm");

    ElementCriterionPtr c(new PoiCriterion);
    RemoveRef2Visitor v;
    v.addCriterion(c);
    map->visitRw(v);

    HOOT_STR_EQUALS("{\"version\": 0.6,\"generator\": \"Hootenanny\",\"elements\": [\n"
                    "{\"type\":\"node\",\"id\":-861027,\"lat\":37.801158,\"lon\":-122.41708},\n"
                    "{\"type\":\"node\",\"id\":-861032,\"lat\":37.80132,\"lon\":-122.41689},\n"
                    "{\"type\":\"node\",\"id\":-861035,\"lat\":37.801179,\"lon\":-122.41703},\n"
                    "{\"type\":\"node\",\"id\":-861038,\"lat\":37.801294,\"lon\":-122.41694},\n"
                    "{\"type\":\"node\",\"id\":-861054,\"lat\":37.801136,\"lon\":-122.41677,\"tags\":{\"" + MetadataTags::Ref1() + "\":\"A\",\"poi\":\"yes\",\"" + MetadataTags::ErrorCircular() + "\":\"15\"}},\n"
                    "{\"type\":\"node\",\"id\":-861055,\"lat\":37.801128,\"lon\":-122.41674,\"tags\":{\"" + MetadataTags::Ref2() + "\":\"none\",\"poi\":\"yes\",\"" + MetadataTags::ErrorCircular() + "\":\"15\"}},\n"
                    "{\"type\":\"node\",\"id\":-861077,\"lat\":37.80112,\"lon\":-122.41676,\"tags\":{\"" + MetadataTags::Ref2() + "\":\"none\",\"poi\":\"yes\",\"" + MetadataTags::ErrorCircular() + "\":\"15\"}},\n"
                    "{\"type\":\"node\",\"id\":-861082,\"lat\":37.801247,\"lon\":-122.41672,\"tags\":{\"" + MetadataTags::Ref2() + "\":\"none\",\"amenity\":\"restaurant\",\"" + MetadataTags::ErrorCircular() + "\":\"15\"}},\n"
                    "{\"type\":\"node\",\"id\":-861083,\"lat\":37.801263,\"lon\":-122.41673,\"tags\":{\"" + MetadataTags::Ref1() + "\":\"C\",\"amenity\":\"restaurant\",\"" + MetadataTags::ErrorCircular() + "\":\"15\"}},\n"
                    "{\"type\":\"way\",\"id\":-861037,\"nodes\":[-861035,-861038],\"tags\":{\"" + MetadataTags::Ref2() + "\":\"B\",\"highway\":\"road\",\"" + MetadataTags::ErrorCircular() + "\":\"15\"},\n"
                    "{\"type\":\"way\",\"id\":-861029,\"nodes\":[-861027,-861032],\"tags\":{\"" + MetadataTags::Ref1() + "\":\"B\",\"highway\":\"road\",\"" + MetadataTags::ErrorCircular() + "\":\"15\"}]\n"
                    "}\n",
                    OsmJsonWriter(8).toString(map));
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RemoveRef2VisitorTest, "quick");

}


