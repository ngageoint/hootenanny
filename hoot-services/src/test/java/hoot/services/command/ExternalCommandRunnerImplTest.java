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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
package hoot.services.command;

import static hoot.services.HootProperties.HOME_FOLDER;
import static hoot.services.HootProperties.HOOTAPI_DB_URL;
import static hoot.services.HootProperties.OSMAPI_DB_URL;
import static org.junit.Assert.assertEquals;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.UUID;

import org.apache.commons.io.FileUtils;
import org.junit.AfterClass;
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
        hootOptions.add("\"convert.ops=DecomposeBuildingRelationsVisitor\"");
        hootOptions.add("-D");
        hootOptions.add("\"conflate.add.score.tags=yes\"");
        hootOptions.add("-D");
        hootOptions.add("hootapi.db.writer.overwrite.map=true");
        hootOptions.add("-D");
        hootOptions.add("hootapi.db.writer.create.user=true");
        hootOptions.add("-D");
        hootOptions.add("api.db.email=test@test.com");
        hootOptions.add("-D");
        hootOptions.add("\"map.cleaner.transforms=ReprojectToPlanarOp;" +
                          "DuplicateWayRemover;SuperfluousWayRemover;" +
                          "IntersectionSplitter;UnlikelyIntersectionRemover;" +
                          "DualHighwaySplitter;HighwayImpliedDividedMarker;" +
                          "DuplicateNameRemover;SmallHighwayMerger;" +
                          "RemoveEmptyAreasVisitor;RemoveDuplicateAreasVisitor;" +
                          "NoInformationElementRemover\"");

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

        String command = "/tmp/hoot.sh conflate --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${INPUT1} " +
                "${INPUT2} ${OUTPUT} ${TIMESTAMP}";

        CommandResult result = runner.exec(command, substitutionMap, jobId,
                this.getClass().getName(), FileUtils.getTempDirectory(), false);
    }

    @Test
    public void obfuscate() {

        ExternalCommandRunnerImpl runner = new ExternalCommandRunnerImpl();

        String test1 = "15:21:06.248 INFO ...hoot/core/io/DataConverter.cpp( 184) Converting hootapidb://hoot:hoottest@localhost:5432/hoot/Merged_ToyTest_4a3300b3 to /home/vagrant/hoot/userfiles/tmp/ex_ee03a439891c4b6193a17599247f6a91/Merged_ToyTest_4a3300b3.osm...\n";
        String expected1 = "Converting <hootapidb>/Merged_ToyTest_4a3300b3 to <path>/userfiles/tmp/ex_ee03a439891c4b6193a17599247f6a91/Merged_ToyTest_4a3300b3.osm...\n";
        assertEquals(expected1, runner.obfuscateConsoleLog(test1));


        String test2 = "20:14:57.252 INFO ...hoot/core/io/DataConverter.cpp( 184) Converting /home/vagrant/hoot/userfiles/tmp/upload/78a9478d-0ee0-4517-abdf-756de217ad82/map.geojson to hootapidb://hoot:hoottest@localhost:5432/hoot/mapgeojson...\n";
        String expected2 = "Converting <path>/userfiles/tmp/upload/78a9478d-0ee0-4517-abdf-756de217ad82/map.geojson to <hootapidb>/mapgeojson...\n";
        assertEquals(expected2, runner.obfuscateConsoleLog(test2));


        String test3 = "20:14:57.318 INFO ...ot/core/io/ElementStreamer.cpp( 80) Unable to stream I/O due to input: -756de217ad82/map.geojson and/or output: host:5432/hoot/mapgeojson";
        String expected3 = "Unable to stream I/O due to input: -756de217ad82/map.geojson and/or output: host:5432/hoot/mapgeojson";
        assertEquals(expected3, runner.obfuscateConsoleLog(test3));

        String test4 = "14:13:54.011 INFO  ...conflate/UnifyingConflator.cpp( 248) Converting match set 3 / 17 to a merger...        14:13:54.012 INFO  ...conflate/UnifyingConflator.cpp( 248) Converting match set 4 / 17 to a merger...        14:13:54.012 INFO  ...conflate/UnifyingConflator.cpp( 248) Converting match set 5 / 17 to a merger...\n";
        String expected4 = "Converting match set 3 / 17 to a merger...\nConverting match set 4 / 17 to a merger...\nConverting match set 5 / 17 to a merger...\n";
        assertEquals(expected4, runner.obfuscateConsoleLog(test4));

        String test5 = "17:48:57.817 INFO  .../cpp/hoot/core/ops/NamedOp.cpp(  83)  Applying operation: BuildingOutlineRemoveOp...\n";
        String expected5 = " Applying operation: BuildingOutlineRemoveOp...\n";
        assertEquals(expected5, runner.obfuscateConsoleLog(test5));

    }


    @Test
    public void obfuscate2() {
        ExternalCommandRunnerImpl runner = new ExternalCommandRunnerImpl();
        String test = "hoot.bin changeset-apply --info -D changeset.description=test cut and replace for node snapping at edges -D changeset.hashtags=#hootenanny -D changeset.source=OSM_077953 -D hoot.osm.auth.access.token=SlN0M6VfCBsU044ug44r2nf8dW2o4pFvyTd89JeD -D hoot.osm.auth.access.token.secret=9Hz2B8PZki8UsVPLpI3NUWrAbsIkvo3qQ1i6aHKZ -D hoot.osm.auth.consumer.key=yyawGlA9IeCxK8eDgkInh3lB9EgGDyXECmlv6es6 -D hoot.osm.auth.consumer.secret=1YT1f1aLhIDCloSYkOnwZRfTVH5tl9BIKwQ300eR /home/vagrant/hoot/userfiles/changesets/grail_cdf5650285354566ac4ff7d19d54d360/diff.osc";
        String expected = "hoot.bin changeset-apply --info -D changeset.description=test cut and replace for node snapping at edges -D changeset.hashtags=#hootenanny -D changeset.source=OSM_077953 -D hoot.osm.auth.access.token=<redacted> -D hoot.osm.auth.access.token.secret=<redacted> -D hoot.osm.auth.consumer.key=<redacted> -D hoot.osm.auth.consumer.secret=<redacted> <path>/userfiles/changesets/grail_cdf5650285354566ac4ff7d19d54d360/diff.osc";
        assertEquals(expected, runner.obfuscateConsoleLog(test));

        test = "hoot.bin changeset-apply --info -D changeset.description=test cut and replace for node snapping at edges -D changeset.hashtags=#hootenanny -D changeset.source=OSM_077953 -D hoot.services.oauth2.access.token=SlN0M6VfCBsU044ug44r2nf8dW2o4pFvyTd89JeD-x_ /home/vagrant/hoot/userfiles/changesets/grail_cdf5650285354566ac4ff7d19d54d360/diff.osc";
        expected = "hoot.bin changeset-apply --info -D changeset.description=test cut and replace for node snapping at edges -D changeset.hashtags=#hootenanny -D changeset.source=OSM_077953 -D hoot.services.oauth2.access.token=<redacted> <path>/userfiles/changesets/grail_cdf5650285354566ac4ff7d19d54d360/diff.osc";
        assertEquals(expected, runner.obfuscateConsoleLog(test));

        test = "STATUS Error uploading changeset: 49514460 - Error transferring http://internal-me-test-995734160.us-east-1.elb.amazonaws.com/api/0.6/changeset/49514460/upload - server replied: Conflict (409)    ";
        expected = "STATUS Error uploading changeset: 49514460 - Error transferring <osmapi>/api/0.6/changeset/49514460/upload - server replied: Conflict (409)    ";
        assertEquals(expected, runner.obfuscateConsoleLog(test));

    }

}
