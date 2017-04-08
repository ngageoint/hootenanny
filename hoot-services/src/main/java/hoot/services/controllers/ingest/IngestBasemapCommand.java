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
package hoot.services.controllers.ingest;

import java.io.File;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.lang3.StringUtils;

import hoot.services.command.ExternalCommand;


class IngestBasemapCommand extends ExternalCommand {

    IngestBasemapCommand(File inputFile, String projection, File tileOutputDir, boolean verboseOutput, Class<?> caller) {
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
            command = "/usr/local/bin/gdal2tiles.py ${VERBOSE} -s ${PROJECTION} -w ${WEBVIEWER} -t ${TITLE} -z ${ZOOM} ${INPUT_FILE} ${TILE_OUTPUT_DIR}";
        }
        else {
            command = "/usr/local/bin/gdal2tiles.py ${VERBOSE} -w ${WEBVIEWER} -t ${TITLE} -z ${ZOOM} ${INPUT_FILE} ${TILE_OUTPUT_DIR}";
        }

        super.configureCommand(command, substitutionMap, caller);
    }
}
