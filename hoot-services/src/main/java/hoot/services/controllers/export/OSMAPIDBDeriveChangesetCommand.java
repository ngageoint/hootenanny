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

import static hoot.services.HootProperties.OSMAPI_DB_URL;

import java.util.HashMap;
import java.util.List;
import java.util.stream.Collectors;

import hoot.services.models.osm.Map;


class OSMAPIDBDeriveChangesetCommand extends ExportCommand {

    OSMAPIDBDeriveChangesetCommand(String jobId, ExportParams params, String debugLevel, Class<?> caller) {
        super(jobId, params, debugLevel, caller);

        String mapName = params.getInput();
        Map conflatedMap = getConflatedMap(mapName);

        String aoi = getAOI(params, conflatedMap);
        String userId = params.getUserId();

        List<String> options = super.getCommonExportHootOptions();
        options.add("-D convert.bounding.box=" + aoi);
        options.add("-D osm.changeset.sql.file.writer.generate.new.ids=false");

        if (userId != null) {
            options.add("-D changeset.user.id=" + userId);
        }
        else {
            throw new RuntimeException("changeset.user.id cannot be null.  Please provide a valid user ID!");
        }

        String hootOptions = options.stream().collect(Collectors.joining(" "));

        java.util.Map<String, String> substitutionMap = new HashMap<>();
        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("HOOT_OPTIONS", hootOptions);
        substitutionMap.put("OSMAPI_DB_URL", OSMAPI_DB_URL);
        substitutionMap.put("INPUT", super.getInput());
        substitutionMap.put("CHANGESET_OUTPUT_PATH", super.getSQLChangesetPath());

        // '' around ${} signifies that quoting is needed
        String command = "hoot derive-changeset --${DEBUG_LEVEL} ${HOOT_OPTIONS} " +
                "'${OSMAPI_DB_URL}' '${INPUT}' '${CHANGESET_OUTPUT_PATH}' '${OSMAPI_DB_URL}'";

        super.configureCommand(command, substitutionMap, caller);
    }
}
