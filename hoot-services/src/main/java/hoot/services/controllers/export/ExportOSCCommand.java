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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.export;

import static hoot.services.HootProperties.HOOTAPI_DB_URL;
import static hoot.services.HootProperties.OSMAPI_DB_URL;

import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import hoot.services.geo.BoundingBox;

/*
== derive-changeset

=== Description

The +derive-changeset+ command creates an OSM changeset file that represents the difference
between two input OSM files.  The changeset can later be applied to an OSM API database.

* +input1+ - OSM data input 1 (e.g. .osm file).
* +input2+ - OSM data input 2 (e.g. .osm file).
* +output+ - Output location (e.g. .osc or .osc.sql file).
* +osmApiDatabaseUrl+ - Target OSM API database the changeset is to be applied to.  Used to maintain
                        element ID continuity with the target database when generating SQL changesets only.
                        Required only if the changeset output format is .osc.sql.

=== Usage

--------------------------------------
derive-changeset (input1) (input2) (output.osc) [jobId] [hootApiDatabaseUrl]
--------------------------------------

==== Examples

--------------------------------------
# xml changeset output
hoot derive-changeset inputData1.osm inputData2.osm outputChangeset.osc

# sql changeset output
hoot derive-changeset inputData1.osm inputData2.osm outputChangeset.osc.sql osmapidb://username:password@localhost:5432/osmApiDatabaseName
--------------------------------------
*/

class ExportOSCCommand extends ExportCommand {

    ExportOSCCommand(String jobId, Map<String, String> paramMap, String debugLevel, Class<?> caller) {
        super(jobId, paramMap, debugLevel, caller);

        String aoi = getBoundingBox(paramMap);
        String outputPath = super.getOutputPath();

        List<String> options = super.getCommonExportHootOptions();
        options.add("-D convert.bounding.box=" + aoi);
        options.add("-D osm.changeset.sql.file.writer.generate.new.ids=false");
        String hootOptions = options.stream().collect(Collectors.joining(" "));

        String input1 = "\"" + OSMAPI_DB_URL + "\"";
        String input2 = "\"" + HOOTAPI_DB_URL + "/" + paramMap.get("input") + "\"";

        //hoot derive-changeset $(HOOT_OPTS) -D convert.bounding.box=$(aoi) -D osm.changeset.sql.file.writer.generate.new.ids=false $(input1) $(input2) "$(OP_OUTPUT)"
        String command = "hoot derive-changeset --" + debugLevel + " " + hootOptions + " " + input1 + " " + input2 + " " + outputPath;

        super.configureCommand(command, caller);
    }

    private static String getBoundingBox(Map<String, String> paramMap) {
        if (!paramMap.containsKey("TASK_BBOX")) {
            throw new IllegalArgumentException("When exporting to a changeset, TASK_BBOX must be specified.");
        }

        BoundingBox bounds = new BoundingBox(paramMap.get("TASK_BBOX"));
        return bounds.getMinLon() + "," + bounds.getMinLat() + "," + bounds.getMaxLon() + "," + bounds.getMaxLat();
    }
}
