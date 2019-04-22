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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.command;

import static hoot.services.HootProperties.OSMAPI_DB_URL;
import static hoot.services.HootProperties.UPLOAD_FOLDER;
import static org.junit.Assert.assertEquals;
import static hoot.services.HootProperties.HOME_FOLDER;
import static hoot.services.HootProperties.HOOTAPI_DB_URL;

import java.io.File;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.UUID;

import org.apache.commons.io.FileUtils;
import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import hoot.services.utils.HootCustomPropertiesSetter;


public class ExternalCommandRunnerImplTest {
    private static String original_HOME_FOLDER;

    @BeforeClass
    public static void oneTimeSetup() throws Exception {
        original_HOME_FOLDER = HOME_FOLDER;
        HootCustomPropertiesSetter.setProperty("HOME_FOLDER", "/home/vagrant/hoot");
    }

    @AfterClass
    public static void afterClass() throws Exception {
        HootCustomPropertiesSetter.setProperty("HOME_FOLDER", original_HOME_FOLDER);
    }

    @Test
    public void exec() throws Exception {
        ExternalCommandRunner runner = new ExternalCommandRunnerImpl();

        List<String> hootOptions = new LinkedList<>();
        hootOptions.add("-D");
        hootOptions.add("\"convert.ops=hoot::DecomposeBuildingRelationsVisitor\"");
        hootOptions.add("-D");
        hootOptions.add("\"conflate.add.score.tags=yes\"");
        hootOptions.add("-D");
        hootOptions.add("hootapi.db.writer.overwrite.map=true");
        hootOptions.add("-D");
        hootOptions.add("hootapi.db.writer.create.user=true");
        hootOptions.add("-D");
        hootOptions.add("api.db.email=test@test.com");
        hootOptions.add("-D");
        hootOptions.add("\"map.cleaner.transforms=hoot::ReprojectToPlanarOp;" +
                          "hoot::DuplicateWayRemover;hoot::SuperfluousWayRemover;" +
                          "hoot::IntersectionSplitter;hoot::UnlikelyIntersectionRemover;" +
                          "hoot::DualWaySplitter;hoot::ImpliedDividedMarker;" +
                          "hoot::DuplicateNameRemover;hoot::SmallWayMerger;" +
                          "hoot::RemoveEmptyAreasVisitor;hoot::RemoveDuplicateAreaVisitor;" +
                          "hoot::NoInformationElementRemover\"");

        // String input1 = "osmapidb://hoot:hoottest@localhost:5432/osmapi test";
        String input1 = OSMAPI_DB_URL + " test";
        // String input2 = "hootapidb://hoot:hoottest@localhost:5432/hoot/394";
        String input2 = HOOTAPI_DB_URL + "/394";
        // String output = "hootapidb://hoot:hoottest@localhost:5432/hoot/osmapi_test_AllDataTypesACucumber";
        String output = HOOTAPI_DB_URL + "/osmapi_test_AllDataTypesACucumber";

        Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("DEBUG_LEVEL", "info");
        substitutionMap.put("HOOT_OPTIONS", hootOptions);
        substitutionMap.put("INPUT1", input1);
        substitutionMap.put("INPUT2", input2);
        substitutionMap.put("OUTPUT", output);
        substitutionMap.put("TIMESTAMP", "2017-04-06 01:07:26.980");

        String jobId = UUID.randomUUID().toString();

        String command = "/tmp/hoot.sh conflate --${DEBUG_LEVEL} -C RemoveReview2Pre.conf ${HOOT_OPTIONS} ${INPUT1} " +
                "${INPUT2} ${OUTPUT} ${TIMESTAMP}";

        CommandResult result = runner.exec(command, substitutionMap, jobId,
                this.getClass().getName(), FileUtils.getTempDirectory(), false);
    }

    @Test
    public void obfuscate() {

        ExternalCommandRunnerImpl runner = new ExternalCommandRunnerImpl();

        String test1 = "15:21:06.248 INFO ...hoot/core/io/DataConverter.cpp( 184) Converting hootapidb://hoot:hoottest@localhost:5432/hoot/Merged_ToyTest_4a3300b3 to /home/vagrant/hoot/userfiles/tmp/ex_ee03a439891c4b6193a17599247f6a91/Merged_ToyTest_4a3300b3.osm...";
        String expected1 = "Converting <hootapidb>/Merged_ToyTest_4a3300b3 to <path>/userfiles/tmp/ex_ee03a439891c4b6193a17599247f6a91/Merged_ToyTest_4a3300b3.osm...";
        assertEquals(expected1, runner.obfuscateConsoleLog(test1));


        String test2 = "20:14:57.252 INFO ...hoot/core/io/DataConverter.cpp( 184) Converting /home/vagrant/hoot/userfiles/tmp/upload/78a9478d-0ee0-4517-abdf-756de217ad82/map.geojson to hootapidb://hoot:hoottest@localhost:5432/hoot/mapgeojson...";
        String expected2 = "Converting <path>/userfiles/tmp/upload/78a9478d-0ee0-4517-abdf-756de217ad82/map.geojson to <hootapidb>/mapgeojson...";
        assertEquals(expected2, runner.obfuscateConsoleLog(test2));


        String test3 = "20:14:57.318 INFO ...ot/core/io/ElementStreamer.cpp( 80) Unable to stream I/O due to input: -756de217ad82/map.geojson and/or output: host:5432/hoot/mapgeojson";
        String expected3 = "Unable to stream I/O due to input: -756de217ad82/map.geojson and/or output: host:5432/hoot/mapgeojson";
        assertEquals(expected3, runner.obfuscateConsoleLog(test3));

        String test4 = "14:13:54.011 INFO  ...conflate/UnifyingConflator.cpp( 248) Converting match set 3 / 17 to a merger...        14:13:54.012 INFO  ...conflate/UnifyingConflator.cpp( 248) Converting match set 4 / 17 to a merger...        14:13:54.012 INFO  ...conflate/UnifyingConflator.cpp( 248) Converting match set 5 / 17 to a merger...";
        String expected4 = "Converting match set 3 / 17 to a merger...\nConverting match set 4 / 17 to a merger...\nConverting match set 5 / 17 to a merger...";
        assertEquals(expected4, runner.obfuscateConsoleLog(test4));


    }

}
