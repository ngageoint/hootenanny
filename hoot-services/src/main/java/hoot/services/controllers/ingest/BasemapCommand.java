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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016, 2017, 2021, 2022 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.ingest;

import static hoot.services.HootProperties.BASEMAPS_FOLDER;

import java.io.File;
import java.io.IOException;
import java.nio.charset.Charset;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.io.FileUtils;
import org.apache.commons.lang3.StringUtils;
import org.json.simple.JSONObject;

import hoot.services.command.CommandResult;
import hoot.services.command.ExternalCommand;


class BasemapCommand extends ExternalCommand {

    private final File tileOutputDir;
    private final String basemapName;

    BasemapCommand(String jobId, String basemapName, File inputFile, String projection, File tileOutputDir, boolean verboseOutput, Class<?> caller) {
        super(jobId);
        this.tileOutputDir = tileOutputDir;
        this.basemapName = basemapName;

        Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("VERBOSE", verboseOutput ? "-v" : "");
        substitutionMap.put("WEBVIEWER", "none");
        substitutionMap.put("INPUT_FILE", inputFile.getAbsolutePath());
        substitutionMap.put("ZOOM", "0-20");
        substitutionMap.put("TILE_OUTPUT_DIR", tileOutputDir.getAbsolutePath());
        substitutionMap.put("TITLE", inputFile.getAbsolutePath());

        String command;

        if (!StringUtils.isBlank(projection)) {
            substitutionMap.put("PROJECTION", projection);
            command = "gdal2tiles.py ${VERBOSE} -s ${PROJECTION} -w ${WEBVIEWER} -t ${TITLE} -z ${ZOOM} ${INPUT_FILE} ${TILE_OUTPUT_DIR}";
        }
        else {
            command = "gdal2tiles.py ${VERBOSE} -w ${WEBVIEWER} -t ${TITLE} -z ${ZOOM} ${INPUT_FILE} ${TILE_OUTPUT_DIR}";
        }

        super.configureCommand(command, substitutionMap, caller);
    }

    @Override
    public CommandResult execute() {
        String ext = ".processing";
        File file = new File(BASEMAPS_FOLDER, basemapName + ext);

        try {
            JSONObject json = new JSONObject();
            json.put("jobid", getJobId());
            json.put("path", tileOutputDir.getAbsolutePath());

            FileUtils.writeStringToFile(file, json.toJSONString(), Charset.defaultCharset());
        }
        catch (IOException ioe) {
            throw new RuntimeException("Error creating " + file, ioe);
        }

        CommandResult commandResult = super.execute();

        ext = (commandResult.failed() ? ".failed" : ".disabled");
        File newFileName = new File(BASEMAPS_FOLDER, basemapName + ext);

        try {
            FileUtils.moveFile(file, newFileName);
        }
        catch (IOException ioe) {
            throw new RuntimeException("Error moving " + file + " to " + newFileName.getAbsolutePath(), ioe);
        }

        return commandResult;
    }
}
