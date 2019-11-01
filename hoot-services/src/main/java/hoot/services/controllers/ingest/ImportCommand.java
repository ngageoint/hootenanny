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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.ingest;

import static hoot.services.HootProperties.HOME_FOLDER;
import static hoot.services.HootProperties.HOOTAPI_DB_URL;
import static hoot.services.HootProperties.SCRIPT_FOLDER;
import static hoot.services.controllers.ingest.UploadClassification.FGDB;
import static hoot.services.controllers.ingest.UploadClassification.GEOJSON;
import static hoot.services.controllers.ingest.UploadClassification.GEONAMES;
import static hoot.services.controllers.ingest.UploadClassification.GPKG;
import static hoot.services.controllers.ingest.UploadClassification.OSM;
import static hoot.services.controllers.ingest.UploadClassification.SHP;
import static hoot.services.controllers.ingest.UploadClassification.ZIP;

import java.io.File;
import java.io.IOException;
import java.util.Arrays;
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
import hoot.services.models.db.Users;


class ImportCommand extends ExternalCommand {
    private static final Logger logger = LoggerFactory.getLogger(ImportCommand.class);

    private final File workDir;

    ImportCommand(String jobId, File workDir, List<File> filesToImport, List<File> zipsToImport, String translation, String advUploadOpts,
                  String etlName, Boolean isNoneTranslation, String debugLevel, UploadClassification classification,
                  Class<?> caller, Users user) {
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
        //TODO: always set remap ids to false??
        options.add("hootapi.db.writer.overwrite.map=true");
        options.add("job.id=" + jobId);
        options.add("api.db.email=" + user.getEmail());

        //if (((classification == OSM) && !isNoneTranslation) || (classification == GEONAMES)) {
            //options.add("convert.ops=hoot::SchemaTranslationVisitor");
            //options.add("schema.translation.script=" + translationPath);
        //}

        if (!isNoneTranslation) {
          options.add("schema.translation.script=" + translationPath);
        }

        if (advUploadOpts != null && !advUploadOpts.isEmpty()) {
            List<String> getAdvOpts = Arrays.asList(advUploadOpts.split(","));
            for (String option: getAdvOpts) {
                String[] opt = option.split("=");
                String key = opt[0];
                String value = (opt.length == 2) ? "=" + opt[1] : "";

                if (configOptions.containsKey(key)) { // if option key in possible values, add new option command
                    Map<String, String> optionConfig = configOptions.get(key);
                    options.add(optionConfig.get("key") + value);
                }
            }
        }

        List<String> hootOptions = toHootOptions(options);

        String inputName = HOOTAPI_DB_URL + "/" + etlName;

        Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("HOOT_OPTIONS", hootOptions);
        substitutionMap.put("INPUT_NAME", inputName);
        substitutionMap.put("INPUTS", inputs);

        String hootConvertCommand = "hoot convert --${DEBUG_LEVEL} -C Import.conf ${HOOT_OPTIONS} ${INPUTS} ${INPUT_NAME}";

        String command = null;
        if ((classification == SHP) || (classification == FGDB) || (classification == ZIP)) {
            if ((classification == ZIP) && !zipsToImport.isEmpty()) {
                //Reading a GDAL dataset in a .gz file or a .zip archive
                inputs = zipsToImport.stream().map(zip -> "/vsizip/" + zip.getAbsolutePath()).collect(Collectors.toList());
                substitutionMap.put("INPUTS", inputs);
            }
            command = hootConvertCommand;
        }
        else if ((classification == OSM) || (classification == GEOJSON) || (classification == GEONAMES) || (classification == GPKG)) {
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
