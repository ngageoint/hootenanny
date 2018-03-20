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
package hoot.services.controllers.export;

import static hoot.services.HootProperties.OSMAPI_DB_URL;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


class ApplyChangesetCommand extends ExportCommand {
    private static final Logger logger = LoggerFactory.getLogger(ApplyChangesetCommand.class);

    ApplyChangesetCommand(String jobId, ExportParams params, String debugLevel, Class<?> caller) {
        super(jobId, params);

        List<String> hootOptions = toHootOptions(super.getCommonExportHootOptions());

        Long mapId = Long.parseLong(params.getInput());
        hoot.services.models.osm.Map conflatedMap = getConflatedMap(mapId);

        // AOI = Area of Interest
        String conflictAOI = getAOI(params, conflatedMap);

        //timestamp of the form: "yyyy-MM-dd hh:mm:ss.zzz" used to prevent writing conflicted data
        String conflictTimestamp = getExportTimeTagFrom(conflatedMap);

        // Services currently always write changeset with sql
        String sqlChangesetPath = super.getSQLChangesetPath();

        Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("HOOT_OPTIONS", hootOptions);
        substitutionMap.put("SQL_CHANGESET_PATH", sqlChangesetPath);
        substitutionMap.put("TARGET_DATABASE_URL", OSMAPI_DB_URL);
        substitutionMap.put("CONFLICT_AOI", conflictAOI);
        substitutionMap.put("CONFLICT_TIMESTAMP", conflictTimestamp);

        String command = "hoot changeset-apply --${DEBUG_LEVEL} ${HOOT_OPTIONS} " +
                "${SQL_CHANGESET_PATH} ${TARGET_DATABASE_URL} ${CONFLICT_AOI} ${CONFLICT_TIMESTAMP}";

        super.configureCommand(command, substitutionMap, caller);
    }

    private static String getExportTimeTagFrom(hoot.services.models.osm.Map conflatedMap) {
        Map<String, String> tags = (Map<String, String>) conflatedMap.getTags();
        String exportTimeTag = "osm_api_db_export_time";

        //+osm_api_db_export_time+ is a timestamp that's written at the time the data in the OSM API database is first exported.
        // It's checked against when writing the resulting changeset after the conflation job to see if any other changesets
        // were added to the OSM API db between the export time and the time the changeset is written.
        if (! tags.containsKey(exportTimeTag)) {
            throw new IllegalStateException("Error exporting data.  Map with ID: " + conflatedMap.getId() +
                    " is missing " + exportTimeTag + ".");
        }

        return tags.get(exportTimeTag).trim();
    }
}
