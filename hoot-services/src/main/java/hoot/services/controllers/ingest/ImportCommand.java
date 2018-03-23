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
package hoot.services.controllers.ingest;

import static hoot.services.HootProperties.HOME_FOLDER;
import static hoot.services.HootProperties.HOOTAPI_DB_URL;
import static hoot.services.HootProperties.SCRIPT_FOLDER;
import static hoot.services.controllers.ingest.UploadClassification.*;

import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import org.apache.commons.io.FileUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.command.CommandResult;
import hoot.services.command.ExternalCommand;


class ImportCommand extends ExternalCommand {
    private static final Logger logger = LoggerFactory.getLogger(ImportCommand.class);

    private final File workDir;

    ImportCommand(String jobId, File workDir, List<File> filesToImport, List<File> zipsToImport, String translation,
                  String etlName, Boolean isNoneTranslation, String debugLevel, UploadClassification classification,
                  Class<?> caller) {
        super(jobId);
        this.workDir = workDir;

        // TODO: Reconcile this logic with the UI.  Passing of translation script's name appears to be inconsistent!
        String translationPath;
        if (translation.startsWith("translations/")) {
            translationPath = new File(HOME_FOLDER, translation).getAbsolutePath();
        }
        else {
            translationPath = new File(new File(SCRIPT_FOLDER), translation).getAbsolutePath();
        }

        List<String> inputs = filesToImport.stream().map(File::getAbsolutePath).collect(Collectors.toList());

        List<String> options = new LinkedList<>();
        options.add("osm2ogr.ops=hoot::DecomposeBuildingRelationsVisitor");
        options.add("hootapi.db.writer.overwrite.map=true");
        options.add("hootapi.db.writer.create.user=true");
        options.add("api.db.email=test@test.com");

        if (((classification == OSM) && !isNoneTranslation) || (classification == GEONAMES)) {
            options.add("convert.ops=hoot::TranslationOp");
            options.add("translation.script=" + translationPath);
        }

        List<String> hootOptions = toHootOptions(options);

        String inputName = HOOTAPI_DB_URL + "/" + etlName;

        Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("HOOT_OPTIONS", hootOptions);
        substitutionMap.put("INPUT_NAME", inputName);
        substitutionMap.put("INPUTS", inputs);

        String hootConvertCommand = "hoot convert --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${INPUTS} ${INPUT_NAME}";
        String hootOGR2OSMCommand = "hoot convert-ogr2osm --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${TRANSLATION_PATH} ${INPUT_NAME} ${INPUTS}";

        String command = null;
        if ((classification == SHP) || (classification == FGDB) || (classification == ZIP)) {
            if ((classification == ZIP) && !zipsToImport.isEmpty()) {
                //Reading a GDAL dataset in a .gz file or a .zip archive
                inputs = zipsToImport.stream().map(zip -> "/vsizip/" + zip.getAbsolutePath()).collect(Collectors.toList());
                substitutionMap.put("INPUTS", inputs);
            }

            if (!isNoneTranslation) {
                substitutionMap.put("TRANSLATION_PATH", translationPath);
                command = hootOGR2OSMCommand;
            }
            else {
                command = hootConvertCommand;
            }
        }
        else if ((classification == OSM) || (classification == GEONAMES)) {
            command = hootConvertCommand;
        }

        super.configureCommand(command, substitutionMap, caller);
    }

    @Override
    public CommandResult execute() {
        CommandResult commandResult = super.execute();

        try {
            FileUtils.forceDelete(workDir);
        }
        catch (IOException ioe) {
            logger.error("Error deleting folder: {} ", workDir.getAbsolutePath(), ioe);
        }

        return commandResult;
    }
}
