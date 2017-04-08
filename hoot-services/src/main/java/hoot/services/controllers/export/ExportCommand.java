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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.export;

import static hoot.services.HootProperties.*;

import java.io.File;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;

import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.command.ExternalCommand;
import hoot.services.geo.BoundingBox;
import hoot.services.models.osm.Map;
import hoot.services.utils.DbUtils;


class ExportCommand extends ExternalCommand {
    private static final Logger logger = LoggerFactory.getLogger(ExportCommand.class);

    private final String jobId;
    private final ExportParams params;

    ExportCommand(String jobId, ExportParams params, String debugLevel, Class<?> caller) {
        this.jobId = jobId;
        this.params = params;

        List<String> hootOptions = toHootOptions(this.getCommonExportHootOptions());

        java.util.Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("HOOT_OPTIONS", hootOptions);
        substitutionMap.put("TRANSLATION_PATH", new File(HOME_FOLDER, params.getTranslation()).getAbsolutePath());
        substitutionMap.put("INPUT_PATH", this.getInput());
        substitutionMap.put("OUTPUT_PATH", this.getOutputPath());

        String command = "hoot osm2ogr --${DEBUG_LEVEL} -C RemoveReview2Pre.conf ${HOOT_OPTIONS} ${TRANSLATION_PATH} ${INPUT_PATH} ${OUTPUT_PATH}";

        super.configureCommand(command, substitutionMap, caller);
    }

    List<String> getCommonExportHootOptions() {
        List<String> options = new LinkedList<>();
        options.add("osm2ogr.ops=hoot::DecomposeBuildingRelationsVisitor");
        options.add("hootapi.db.writer.overwrite.map=true");
        options.add("hootapi.db.writer.create.user=true");
        options.add("api.db.email=test@test.com");

        //# Add the option to have status tags as text with "Input1" instead of "1" or "Unknown1"
        //ifeq "$(textstatus)" "true"
        //    HOOT_OPTS+= -D writer.text.status=true
        //endif
        if (params.getTextStatus()) {
            options.add("writer.text.status=true");
        }

        //# Add the option to append
        //ifeq "$(append)" "true"
        //    HOOT_OPTS+= -D ogr.append.data=true
        //endif
        if (params.getAppend()) {
            options.add("ogr.append.data=true");
        }

        return options;
    }

    String getOutputPath() {
        File outputFolder = this.getWorkFolder();
        File outputFile;

        if (!StringUtils.isBlank(params.getOutputName())) {
            outputFile = new File(outputFolder, params.getOutputName() + "." + params.getOutputType());
        }
        else {
            outputFile = new File(outputFolder, jobId + "." + params.getOutputType());
        }

        return outputFile.getAbsolutePath();
    }

    String getInput() {
        //ifeq "$(inputtype)" "file"
        //    INPUT_PATH=$(input)
        //endif
        if (! params.getInputType().equalsIgnoreCase("file")) {
            return HOOTAPI_DB_URL + "/" + params.getInput();
        }

        return params.getInput();
    }

    static Map getConflatedMap(String mapName) {
        Long mapId = DbUtils.getMapIdByName(mapName);

        if (mapId == null) {
            throw new IllegalStateException("Error exporting data.  No map exists with name: " + mapName);
        }

        Map conflatedMap = new Map(mapId);
        conflatedMap.setDisplayName(mapName);
        conflatedMap.setTags(DbUtils.getMapsTableTags(mapId));

        return conflatedMap;
    }

    static String getAOI(ExportParams params, Map conflatedMap) {
        // if sent a bbox in the url (reflecting task grid bounds)
        // use that, otherwise use the bounds of the conflated output
        BoundingBox boundingBox;
        if (params.getBounds() != null) {
            boundingBox = new BoundingBox(params.getBounds());
        }
        else {
            boundingBox = conflatedMap.getBounds();
        }

        return boundingBox.getMinLon() + "," + boundingBox.getMinLat() + "," + boundingBox.getMaxLon() + "," + boundingBox.getMaxLat();
    }

    String getSQLChangesetPath() {
        // Services currently always write changeset with sql
        return new File(this.getWorkFolder(), "changeset-" + jobId + ".osc.sql").getAbsolutePath();
    }

    private File getWorkFolder() {
        return new File(TEMP_OUTPUT_PATH, jobId);
    }
}
