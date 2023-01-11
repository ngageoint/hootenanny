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
 * @copyright Copyright (C) 2022, 2023 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/ParallelBoundedApiReader.h>

namespace hoot
{

class ParallelBoundedApiReaderTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ParallelBoundedApiReaderTest);
  CPPUNIT_TEST(parseErrorTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ParallelBoundedApiReaderTest() = default;

  void parseErrorTest()
  {
    QString expected_result = "runtime error: Query ran out of memory in \"query\" at line 1. It would need at least 95 MB of RAM to continue.";
    ParallelBoundedApiReader uut;
    QString json_result =
        "{"
        "\"version\": 0.6,"
        "\"generator\": \"NOME Overpass API 0.7.57 c954ae26\","
        "\"osm3s\": {"
        "  \"timestamp_osm_base\": \"2021-09-09T17:21:17Z\","
        "  \"copyright\": \"The data included in this document is from NOME and OpenStreetMap. The data is made available under ODbL. data included in this document falls under NOME Terms of Use https://nome.vgihub.geointservices.io/disclaimer/NOME_Enclave_Disclaimer_V4.pdf\""
        "},"
        "\"elements\": ["
        "],"
        "\"remark\": \"runtime error: Query ran out of memory in \"query\" at line 1. It would need at least 95 MB of RAM to continue.\""
        "}";
    QString error;
    CPPUNIT_ASSERT(uut._isQueryError(json_result, error));
    HOOT_STR_EQUALS(expected_result, error);

    QString xml_result =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        "<osm version=\"0.6\" generator=\"NOME Overpass API 0.7.57 c954ae26\">"
        "<note>The data included in this document is from NOME and OpenStreetMap. The data is made available under ODbL. data included in this document falls under NOME Terms of Use https://nome.vgihub.geointservices.io/disclaimer/NOME_Enclave_Disclaimer_V4.pdf</note>"
        "<meta osm_base=\"2021-09-09T17:21:17Z\"/>"
        "<remark> runtime error: Query ran out of memory in \"query\" at line 1. It would need at least 95 MB of RAM to continue. </remark>"
        "</osm>";
    error = "";
    CPPUNIT_ASSERT(uut._isQueryError(xml_result, error));
    HOOT_STR_EQUALS(expected_result, error);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ParallelBoundedApiReaderTest, "quick");

}

