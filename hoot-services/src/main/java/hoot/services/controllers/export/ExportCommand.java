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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.export;

import static hoot.services.HootProperties.*;

import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;

import org.apache.commons.io.FileUtils;
import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.command.ExternalCommand;
import hoot.services.command.common.UnTARFileCommand;
import hoot.services.geo.BoundingBox;
import hoot.services.models.osm.Map;
import hoot.services.utils.DbUtils;


class ExportCommand extends ExternalCommand {
    private static final Logger logger = LoggerFactory.getLogger(ExportCommand.class);

    private final ExportParams params;

    ExportCommand(String jobId, ExportParams exportParams) {
        super(jobId);
        this.params = exportParams;
    }

    ExportCommand(String jobId, ExportParams params, String debugLevel, Class<?> caller) {
        this(jobId, params);

        if (params.getAppend()) {
            appendToFGDB();
        }

        List<String> hootOptions = toHootOptions(this.getCommonExportHootOptions());

        java.util.Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("HOOT_OPTIONS", hootOptions);
        substitutionMap.put("TRANSLATION_PATH", new File(HOME_FOLDER, params.getTranslation()).getAbsolutePath());
        substitutionMap.put("INPUT_PATH", this.getInput());
        substitutionMap.put("OUTPUT_PATH", this.getOutputPath());

        String command = "hoot convert --${DEBUG_LEVEL} -C RemoveReview2Pre.conf ${HOOT_OPTIONS} ${INPUT_PATH} ${OUTPUT_PATH} --trans ${TRANSLATION_PATH} ";

        super.configureCommand(command, substitutionMap, caller);
    }

    private void appendToFGDB() {
        //Appends data to a blank fgdb. The template is stored with the fouo translations.

        //$(HOOT_HOME)/translations-local/template
        File templateHome = new File(new File(HOME_FOLDER, "translations-local"), "template");

        String translation = params.getTranslation();
        File tdsTemplate = null;

        if (translation.equalsIgnoreCase("translations/TDSv61.js")) {
            tdsTemplate = new File(templateHome, "tds61.tgz");
        }
        else if (translation.equalsIgnoreCase("translations/TDSv40.js")) {
            tdsTemplate = new File(templateHome, "tds40.tgz");
        }

        if ((tdsTemplate != null) && tdsTemplate.exists()) {
            File outputDir = new File(this.getWorkFolder(), params.getOutputName() + "."     + params.getOutputType().toLowerCase());
            try {
                FileUtils.forceMkdir(outputDir);
            }
            catch (IOException ioe) {
                throw new RuntimeException("Error creating directory: " + outputDir.getAbsolutePath(), ioe);
            }

            ExternalCommand untarFileCommand = new UnTARFileCommand(tdsTemplate, outputDir, this.getClass());
            untarFileCommand.execute();
        }
    }

    List<String> getCommonExportHootOptions() {
        List<String> options = new LinkedList<>();
        options.add("convert.ops=hoot::DecomposeBuildingRelationsVisitor");
        options.add("hootapi.db.writer.overwrite.map=true");
        options.add("hootapi.db.writer.create.user=true");
        options.add("api.db.email=" + params.getUserEmail());

        //# Add the option to have status tags as text with "Input1" instead of "1" or "Unknown1"
        if (params.getTextStatus()) {
            options.add("writer.text.status=true");
        }

        if (!params.getTagOverrides().isEmpty()) {
            options.add("translation.override=" + params.getTagOverrides() );
        }

        //# Add the option to append
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
            outputFile = new File(outputFolder, getJobId() + "." + params.getOutputType());
        }

        return outputFile.getAbsolutePath();
    }

    String getInput() {
        if (! params.getInputType().equalsIgnoreCase("file")) {
            return HOOTAPI_DB_URL + "/" + params.getInput();
        }

        return params.getInput();
    }

    static Map getConflatedMap(Long mapId) {
        Map conflatedMap = new Map(mapId);
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
        return new File(this.getWorkFolder(), "changeset-" + getJobId() + ".osc.sql").getAbsolutePath();
    }

    private File getWorkFolder() {
        return new File(TEMP_OUTPUT_PATH, getJobId());
    }
}
