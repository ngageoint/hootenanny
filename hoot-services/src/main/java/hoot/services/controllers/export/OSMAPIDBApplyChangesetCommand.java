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

import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;


class OSMAPIDBApplyChangesetCommand extends ExportCommand {

    OSMAPIDBApplyChangesetCommand(String jobId, Map<String, String> paramMap, String debugLevel, Class<?> caller) {
        super(jobId, paramMap, debugLevel, caller);

        List<String> options = super.getCommonExportHootOptions();
        String hootOptions = options.stream().collect(Collectors.joining(" "));

        String mapName = paramMap.get("input");
        hoot.services.models.osm.Map conflatedMap = getConflatedMap(mapName);

        // AOI = Area of Interest
        String conflictAOI = getAOI(paramMap, conflatedMap);

        //timestamp of the form: "yyyy-MM-dd hh:mm:ss.zzz" used to prevent writing conflicted dat
        String conflictTimestamp = getExportTimeTagFrom(conflatedMap);

        // Services currently always write changeset with sql
        String sqlChangeset = super.getSQLChangesetPath();

        String targetDatabaseUrl = OSMAPI_DB_URL;

        //hoot apply-changeset $(HOOT_OPTS) $(changesetoutput) "$(OSM_API_DB_URL)" "$(aoi)" "$(changesetsourcedatatimestamp)"
        String command = "hoot apply-changeset --" + debugLevel + " " + hootOptions + " " +
                            quote(sqlChangeset) + " " + quote(targetDatabaseUrl) + " " + quote(conflictAOI) + " " + quote(conflictTimestamp);

        super.configureCommand(command, caller);
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

        return tags.get(exportTimeTag);
    }
}
