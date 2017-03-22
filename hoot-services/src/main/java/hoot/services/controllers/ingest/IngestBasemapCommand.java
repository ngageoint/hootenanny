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

import org.apache.commons.lang3.StringUtils;

import hoot.services.command.ExternalCommand;

/*
    make -f $HOOT_HOME/scripts/services/submakebasemaprastertotiles INPUT="$1" INPUT_NAME="$2" RASTER_OUTPUT_DIR="$3" PROJECTION="$4" JOB_PROCESSOR_DIR="$5" jobid="$6"
    if [ $? -eq 0 ]; then
        mv -f "$5/$2.processing" "$5/$2.disabled"
        exit 0
    else
        mv -f "$5/$2.processing" "$5/$2.failed"
        exit 10
    fi
*/

/*
    DQT="\""
    GDAL2TILES=/usr/local/bin/gdal2tiles.py
    OP_INPUT=$(INPUT)
    OP_TILE_OUTPUT_DIR=$(RASTER_OUTPUT_DIR)/$(INPUT_NAME)
    OP_JOB_PROCESSOR=$(JOB_PROCESSOR_DIR)/$(INPUT_NAME)
    OP_PROJECTION=

    ifneq "$(PROJECTION)" "auto"
        OP_PROJECTION=-s $(PROJECTION)
    endif

    OP_INFO={
    OP_INFO+=$(DQT)jobid$(DQT):$(DQT)$(jobid)$(DQT),
    OP_INFO+=$(DQT)path$(DQT):$(DQT)$(OP_TILE_OUTPUT_DIR)$(DQT)
    OP_INFO+=}

    step1:
        mkdir -p "$(OP_TILE_OUTPUT_DIR)"
        mkdir -p "$(JOB_PROCESSOR_DIR)"
        echo "$(OP_INFO)" > "$(OP_JOB_PROCESSOR).processing"

        "$(GDAL2TILES)" $(OP_PROJECTION)  -w none -t "$(OP_INPUT)" -z '0-20' "$(OP_INPUT)" "$(OP_TILE_OUTPUT_DIR)"
*/

class IngestBasemapCommand extends ExternalCommand {

    IngestBasemapCommand(String inputFile, String projection, String tileOutputDir, boolean verboseOutput, Class<?> caller) {
        String projectionSwitch = !StringUtils.isBlank(projection) ? ("-s " + projection) : "";
        String verboseSwitch = verboseOutput ? "-v" : "";
        String zoomSwitch = "-z '0-20'";
        String titleSwitch = "-t " + quote(inputFile);
        String webviewerSwitch = "-w none";

        //"$(GDAL2TILES)" $(OP_PROJECTION)  -w none -t "$(OP_INPUT)" -z '0-20' "$(OP_INPUT)" "$(OP_TILE_OUTPUT_DIR)"
        String command = "/usr/local/bin/gdal2tiles.py " + verboseSwitch + " " + projectionSwitch + " " +
                webviewerSwitch + " " + titleSwitch + " " + zoomSwitch + " " + quote(inputFile) + " " + quote(tileOutputDir);

        super.configureCommand(command, caller);
    }
}
