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
import static hoot.services.HootProperties.HOOTAPI_DB_URL;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.UUID;

import org.apache.commons.io.FileUtils;
import org.junit.Test;


public class ExternalCommandRunnerImplTest {

    @Test
    public void exec() throws Exception {
        ExternalCommandRunner runner = new ExternalCommandRunnerImpl();

        List<String> hootOptions = new LinkedList<>();
        hootOptions.add("-D");
        hootOptions.add("\"osm2ogr.ops=hoot::DecomposeBuildingRelationsVisitor\"");
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
}