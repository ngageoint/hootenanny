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

import static hoot.services.HootProperties.*;

import java.io.File;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import hoot.services.command.ExternalCommand;


class IngestBasemapCommand extends ExternalCommand {

    IngestBasemapCommand(String jobId, String groupId, String inputFileName, String projection, String basemapName, Class<?> caller) {
        JSONArray commandArgs = new JSONArray();

        // from scripts/makebasemaprastertotiles.sh:
        //   make -f $HOOT_HOME/scripts/submakebasemaprastertotiles INPUT="$1" INPUT_NAME="$2" RASTER_OUTPUT_DIR="$3" PROJECTION="$4" JOB_PROCESSOR_DIR="$5" jobid="$6"

        // $1
        JSONObject arg = new JSONObject();
        arg.put("INPUT", UPLOAD_FOLDER + File.separator + groupId + File.separator + inputFileName);
        commandArgs.add(arg);

        // $2
        arg = new JSONObject();
        arg.put("INPUT_NAME", basemapName);
        commandArgs.add(arg);

        // $3
        arg = new JSONObject();
        arg.put("RASTER_OUTPUT_DIR", BASEMAPS_TILES_FOLDER);
        commandArgs.add(arg);

        // $4
        arg = new JSONObject();
        if ((projection != null) && (!projection.isEmpty())) {
            arg.put("PROJECTION", projection);
        }
        else {
            arg.put("PROJECTION", "auto");
        }
        commandArgs.add(arg);

        // $5
        arg = new JSONObject();
        arg.put("JOB_PROCESSOR_DIR", BASEMAPS_FOLDER);
        commandArgs.add(arg);

        // $6
        arg = new JSONObject();
        arg.put("jobid", jobId);
        commandArgs.add(arg);

        super.configureAsBashCommand(BASEMAP_RASTER_TO_TILES, caller, commandArgs);
    }
}
