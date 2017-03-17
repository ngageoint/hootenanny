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
import java.io.IOException;
import java.nio.file.Files;
import java.util.LinkedList;
import java.util.List;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;

import org.apache.commons.lang3.StringUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.command.ExternalCommand;
import hoot.services.geo.BoundingBox;
import hoot.services.models.osm.Map;
import hoot.services.utils.DbUtils;
import hoot.services.utils.JsonUtils;

/*
#
#  Export Make file
#

# Options for most exports
HOOT_OPTS+= -D osm2ogr.ops=hoot::DecomposeBuildingRelationsVisitor
HOOT_OPTS+= -D hootapi.db.writer.overwrite.map=true -D hootapi.db.writer.create.user=true
HOOT_OPTS+= -D api.db.email=test@test.com

# Add the option to append
ifeq "$(append)" "true"
HOOT_OPTS+= -D ogr.append.data=true
endif


# Options for osm & osm.pbf export
OSM_OPTS=-D hootapi.db.writer.create.user=true -D api.db.email=test@test.com

# Add the option to have status tags as text with "Input1" instead of "1" or "Unknown1"
ifeq "$(textstatus)" "true"
HOOT_OPTS+= -D writer.text.status=true
OSM_OPTS+=  -D writer.text.status=true
endif

#DB_URL=hootapidb://hoot:hoottest@localhost:5432/hoot
OP_PG_URL=PG:"$(PG_URL)"


OP_TRANSLATION=$(HOOT_HOME)/$(translation)
OP_OUTPUT_FILE=$(outputname).$(outputtype)
OP_OUTPUT=$(outputfolder)/$(outputname).$(outputtype)
ZIP_OUTPUT=$(outputname).zip
INPUT_PATH=$(DB_URL)/$(input)
REMOVE_REVIEW=-C RemoveReview2Pre.conf
OP_ZIP=cd "$(outputfolder)" && zip -r "$(ZIP_OUTPUT)" "$(OP_OUTPUT_FILE)"


# Hardcoded. There should be a better location for these
TEMPLATE_PATH=$(HOOT_HOME)/translations-local/template
TDS61_TEMPLATE=$(TEMPLATE_PATH)/tds61.tgz
TDS40_TEMPLATE=$(TEMPLATE_PATH)/tds40.tgz


ifeq "$(outputtype)" "shp"
	OP_ZIP=cd "$(outputfolder)/$(outputname)" && zip -r "$(outputfolder)/$(ZIP_OUTPUT)" *
endif

ifeq "$(inputtype)" "file"
	INPUT_PATH=$(input)
endif

#####

###
# Osm2Ogr
###
step1:
ifeq ("$(outputtype)","wfs")
	hoot osm2ogr $(REMOVE_REVIEW) -D ogr.writer.pre.layer.name=$(outputname)_ $(HOOT_OPTS) "$(OP_TRANSLATION)" "$(INPUT_PATH)" $(OP_PG_URL)
else ifeq ("$(outputtype)","osm")
	mkdir -p "$(outputfolder)"
	hoot convert $(OSM_OPTS) "$(INPUT_PATH)" "$(OP_OUTPUT)"
	cd "$(outputfolder)" && zip -r "$(ZIP_OUTPUT)" "$(OP_OUTPUT_FILE)"
else ifeq ("$(outputtype)","osm.pbf")
	mkdir -p "$(outputfolder)"
	hoot convert $(OSM_OPTS) "$(INPUT_PATH)" "$(OP_OUTPUT)"
else ifeq ("$(outputtype)","osm_api_db")
	hoot derive-changeset $(HOOT_OPTS) -D changeset.user.id=$(userid) -D convert.bounding.box=$(aoi) -D osm.changeset.sql.file.writer.generate.new.ids=false "$(OSM_API_DB_URL)" "$(INPUT_PATH)" $(changesetoutput) "$(OSM_API_DB_URL)"
	hoot apply-changeset $(HOOT_OPTS) $(changesetoutput) "$(OSM_API_DB_URL)" "$(aoi)" "$(changesetsourcedatatimestamp)"
else ifeq ("$(outputtype)","osc")
	mkdir -p "$(outputfolder)"
	hoot derive-changeset $(HOOT_OPTS) -D convert.bounding.box=$(aoi) -D osm.changeset.sql.file.writer.generate.new.ids=false $(input1) $(input2) "$(OP_OUTPUT)"
else
	mkdir -p "$(outputfolder)"
ifeq "$(append)" "true"
ifeq "$(translation)" "translations/TDSv61.js"
ifneq ("$(wildcard $(TDS61_TEMPLATE))","")
	mkdir -p $(OP_OUTPUT)
	tar -zxf $(TDS61_TEMPLATE) -C $(OP_OUTPUT)
endif # Template Path
else
ifeq "$(translation)" "translations/TDSv40.js"
ifneq ("$(wildcard $(TDS40_TEMPLATE))","")
	mkdir -p $(OP_OUTPUT)
	tar -zxf $(TDS40_TEMPLATE) -C $(OP_OUTPUT)
endif # Template Path
endif # Translations TDSv40
endif # Else
endif # Append
	hoot osm2ogr $(REMOVE_REVIEW) $(HOOT_OPTS) "$(OP_TRANSLATION)" "$(INPUT_PATH)" "$(OP_OUTPUT)"
	$(OP_ZIP)

endif # Shape/FGDB

 */

class ExportCommand extends ExternalCommand {

    private static final Logger logger = LoggerFactory.getLogger(ExportCommand.class);

    private String jobId;
    private java.util.Map<String, String> paramMap;
    private String outputType;
    private String input;

    //TODO outputtype=osm_api_db may end up being obsolete with the addition of osc
    ExportCommand(String jobId, java.util.Map<String, String> paramMap, String debugLevel, String outputType, String input, Class<?> caller) {
        this.jobId = jobId;
        this.paramMap = paramMap;
        this.outputType = outputType;
        this.input = input;

        List<String> hootOptions = this.getHootOptions();

        //ifeq "$(outputtype)" "shp"
        //    OP_ZIP=cd "$(outputfolder)/$(outputname)" && zip -r "$(outputfolder)/$(ZIP_OUTPUT)" *
        //endif
        if ("shp".equals(paramMap.get("outputtype"))) {
            //OP_ZIP=cd "$(outputfolder)/$(outputname)" && zip -r "$(outputfolder)/$(ZIP_OUTPUT)" *
        }

        //ifeq "$(inputtype)" "file"
        //    INPUT_PATH=$(input)
        //endif

        // mkdir -p "$(outputfolder)"
        // ifeq "$(append)" "true"
        //     ifeq "$(translation)" "translations/TDSv61.js"
        //         ifneq ("$(wildcard $(TDS61_TEMPLATE))","")
        //             mkdir -p $(OP_OUTPUT)
        //             tar -zxf $(TDS61_TEMPLATE) -C $(OP_OUTPUT)
        //         endif # Template Path
        //    else
        //       ifeq "$(translation)" "translations/TDSv40.js"
        //           ifneq ("$(wildcard $(TDS40_TEMPLATE))","")
        //               mkdir -p $(OP_OUTPUT)
        //               tar -zxf $(TDS40_TEMPLATE) -C $(OP_OUTPUT)
        //           endif # Template Path
        //       endif # Translations TDSv40
        //    endif # Else
        // endif # Append
        // hoot osm2ogr $(REMOVE_REVIEW) $(HOOT_OPTS) "$(OP_TRANSLATION)" "$(INPUT_PATH)" "$(OP_OUTPUT)"
        // $(OP_ZIP)
    }

    private static JSONArray addExportToOsmApiDbCommandArgs(String jobId, JSONArray commandArgs, JSONObject oParams) {
        if (!OSM_API_DB_ENABLED) {
            String msg = "Attempted to export to an OSM API database but OSM API database support is disabled";
            throw new WebApplicationException(Response.serverError().entity(msg).build());
        }

        if (!"db".equalsIgnoreCase(JsonUtils.getParameterValue("inputtype", oParams))) {
            String msg = "When exporting to an OSM API database, the input type must be a Hootenanny API database.";
            throw new WebApplicationException(Response.status(Response.Status.BAD_REQUEST).entity(msg).build());
        }

        String translation = JsonUtils.getParameterValue("translation", oParams);
        if ((StringUtils.trimToNull(translation) != null) && !translation.toUpperCase().equals("NONE")) {
            String msg = "Custom translation not allowed when exporting to OSM API database.";
            throw new WebApplicationException(Response.status(Response.Status.BAD_REQUEST).entity(msg).build());
        }

        // This logic is a little out of line with the rest of ExportResource, but since this export option will
        // probably go away completely soon, no point in changing it now.

        // ignoring outputname, since we're only going to have a single mapedit
        // connection configured in the core for now
        JSONObject arg = new JSONObject();

        // services currently always write changeset with sql
        File tempFile = null;
        try {
            File tempOutputDir = new File(TEMP_OUTPUT_PATH);
            tempFile = Files.createFile(new File(tempOutputDir, "changeset-" + jobId + ".osc.sql").toPath()).toFile();
            //tempFile = File.createTempFile("changeset-", ".osc.sql", tempOutputDir);
        }
        catch (IOException ioe) {
            throw new RuntimeException("Error occurred!", ioe);
        }

        arg.put("changesetoutput", tempFile.getAbsolutePath());
        commandArgs.add(arg);

        // This option allows the job executor return std out to the client.
        // This is the only way I've found to get the conflation summary text back from hoot command line to the UI.
        arg = new JSONObject();
        arg.put("writeStdOutToStatusDetail", "true");
        commandArgs.add(arg);

        String mapName = JsonUtils.getParameterValue("input", oParams);
        Map conflatedMap = getConflatedMap(mapName);

        // pass the export timestamp to the export bash script
        addMapForExportTag(conflatedMap, commandArgs);

        // pass the export aoi to the export bash script
        // if sent a bbox in the url (reflecting task grid bounds)
        // use that, otherwise use the bounds of the conflated output
        BoundingBox bbox;
            if (oParams.get("TASK_BBOX") != null) {
            bbox = new BoundingBox(oParams.get("TASK_BBOX").toString());
        }
        else {
            bbox = conflatedMap.getBounds();
        }

        setAoi(bbox, commandArgs);

        return commandArgs;
    }

    private static Map getConflatedMap(String mapName) {
        Long mapId = DbUtils.getMapIdByName(mapName);

        if (mapId == null) {
            String msg = "Error exporting data.  No map exists with name: " + mapName;
            throw new WebApplicationException(Response.status(Response.Status.BAD_REQUEST).entity(msg).build());
        }

        Map conflatedMap = new Map(mapId);
        conflatedMap.setDisplayName(mapName);
        conflatedMap.setTags(DbUtils.getMapsTableTags(mapId));

        return conflatedMap;
    }

    private static void addMapForExportTag(Map conflatedMap, JSONArray commandArgs) {
        java.util.Map<String, String> tags = (java.util.Map<String, String>) conflatedMap.getTags();
        if (!tags.containsKey("osm_api_db_export_time")) {
            String msg = "Error exporting data.  Map with ID: " + conflatedMap.getId() + " has no osm_api_db_export_time tag.";
            throw new WebApplicationException(Response.status(Response.Status.CONFLICT).entity(msg).build());
        }

        JSONObject arg = new JSONObject();
        arg.put("changesetsourcedatatimestamp", tags.get("osm_api_db_export_time"));
        commandArgs.add(arg);
    }

    private static void setAoi(BoundingBox bounds, JSONArray commandArgs) {
        JSONObject arg = new JSONObject();
        arg.put("aoi", bounds.getMinLon() + "," + bounds.getMinLat() + "," + bounds.getMaxLon() + "," + bounds.getMaxLat());
        commandArgs.add(arg);
    }

    List<String> getHootOptions() {
        //HOOT_OPTS+= -D osm2ogr.ops=hoot::DecomposeBuildingRelationsVisitor
        //HOOT_OPTS+= -D hootapi.db.writer.overwrite.map=true
        //HOOT_OPTS+= -D hootapi.db.writer.create.user=true
        //HOOT_OPTS+= -D api.db.email=test@test.com
        //HOOT_OPTS+= -D ogr.writer.pre.layer.name

        List<String> options = new LinkedList<>();
        options.add("-D osm2ogr.ops=hoot::DecomposeBuildingRelationsVisitor");
        options.add("-D hootapi.db.writer.overwrite.map=true");
        options.add("-D hootapi.db.writer.create.user=true");
        options.add("-D api.db.email=test@test.com");
        options.add("-D ogr.writer.pre.layer.name=" + jobId + "_");

        //# Add the option to append
        //ifeq "$(append)" "true"
        //    HOOT_OPTS+= -D ogr.append.data=true
        //endif
        if (Boolean.valueOf(paramMap.get("append"))) {
            options.add("-D ogr.append.data=true");
        }

        //# Add the option to have status tags as text with "Input1" instead of "1" or "Unknown1"
        //ifeq "$(textstatus)" "true"
        //    HOOT_OPTS+= -D writer.text.status=true
        //endif
        if (Boolean.valueOf(paramMap.get("textstatus"))) {
            options.add("-D writer.text.status=true");
        }

        return options;
    }

    String getOutputPath() {
        File outputFolder = new File(TEMP_OUTPUT_PATH, jobId);
        File outputFile = new File(outputFolder,jobId + "." + outputType);

        return "\"" + outputFile.getAbsolutePath() + "\"";
    }

    String getBoundingBox() {
        if (!paramMap.containsKey("TASK_BBOX")) {
            throw new IllegalArgumentException("When exporting to a changeset, TASK_BBOX must be specified.");
        }

        BoundingBox bounds = new BoundingBox(paramMap.get("TASK_BBOX"));

        return bounds.getMinLon() + "," + bounds.getMinLat() + "," + bounds.getMaxLon() + "," + bounds.getMaxLat();
    }
}
