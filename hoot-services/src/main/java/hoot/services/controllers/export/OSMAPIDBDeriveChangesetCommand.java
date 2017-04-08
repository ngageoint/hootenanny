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

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.models.osm.Map;


class OSMAPIDBDeriveChangesetCommand extends ExportCommand {
    private static final Logger logger = LoggerFactory.getLogger(OSMAPIDBDeriveChangesetCommand.class);

    OSMAPIDBDeriveChangesetCommand(String jobId, ExportParams params, String debugLevel, Class<?> caller) {
        super(jobId, params, debugLevel, caller);

        String mapName = params.getInput();
        Map conflatedMap = getConflatedMap(mapName);

        String aoi = getAOI(params, conflatedMap);

        List<String> options = super.getCommonExportHootOptions();
        options.add("convert.bounding.box=" + aoi);
        options.add("osm.changeset.sql.file.writer.generate.new.ids=false");

        String userId = params.getUserId();
        if (userId != null) {
            options.add("changeset.user.id=" + userId);
        }
        else {
            throw new RuntimeException("changeset.user.id cannot be null.  Please provide a valid user ID!");
        }

        List<String> hootOptions = toHootOptions(options);

        java.util.Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("HOOT_OPTIONS", hootOptions);
        substitutionMap.put("OSMAPI_DB_URL", OSMAPI_DB_URL);
        substitutionMap.put("INPUT", super.getInput());
        substitutionMap.put("CHANGESET_OUTPUT_PATH", super.getSQLChangesetPath());

        String command = "hoot derive-changeset --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${OSMAPI_DB_URL} " +
                "${INPUT} ${CHANGESET_OUTPUT_PATH} ${OSMAPI_DB_URL}";

        super.configureCommand(command, substitutionMap, caller);
    }
}
