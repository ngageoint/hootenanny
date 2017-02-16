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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.export;

import static hoot.services.HootProperties.*;

import java.io.File;
import java.io.IOException;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;

import org.apache.commons.lang3.StringUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import hoot.services.command.ExternalCommand;
import hoot.services.geo.BoundingBox;
import hoot.services.models.osm.Map;
import hoot.services.utils.DbUtils;
import hoot.services.utils.JsonUtils;


class ExportCommand extends ExternalCommand {

    //TODO outputtype=osm_api_db may end up being obsolete with the addition of osc
    ExportCommand(String jobId, String params, Class<?> caller) {
        JSONArray commandArgs;
        JSONObject oParams;
        try {
            commandArgs = JsonUtils.parseParams(params);
            JSONParser parser = new JSONParser();
            oParams = (JSONObject) parser.parse(params);
        }
        catch (ParseException pe) {
            throw new RuntimeException("Error parsing: " + params, pe);
        }

        JSONObject outputfolder = new JSONObject();
        outputfolder.put("outputfolder", TEMP_OUTPUT_PATH + "/" + jobId);
        commandArgs.add(outputfolder);

        JSONObject output = new JSONObject();
        output.put("output", jobId);
        commandArgs.add(output);

        JSONObject hootDBURL = new JSONObject();
        hootDBURL.put("DB_URL", HOOT_APIDB_URL);
        commandArgs.add(hootDBURL);

        JSONObject osmAPIDBURL = new JSONObject();
        osmAPIDBURL.put("OSM_API_DB_URL", OSM_APIDB_URL);
        commandArgs.add(osmAPIDBURL);

        String type = JsonUtils.getParameterValue("outputtype", oParams);

        if ("wfs".equalsIgnoreCase(type)) {
            JSONObject arg = new JSONObject();
            arg.put("outputname", jobId);
            commandArgs.add(arg);

            String pgUrl = "host='" + DB_HOST + "' port='" + DB_PORT + "' user='" + DB_USER_ID
                    + "' password='" + DB_PASSWORD + "' dbname='" + WFS_STORE_DB + "'";

            arg = new JSONObject();
            arg.put("PG_URL", pgUrl);
            commandArgs.add(arg);
        }
        else if ("osm_api_db".equalsIgnoreCase(type)) {
            addExportToOsmApiDbCommandArgs(commandArgs, oParams);
        }
        else if ("osc".equalsIgnoreCase(type)) {
            addExportToChangesetCommandArgs(commandArgs, oParams);
        }
        else {
            // replace with with getParameterValue
            boolean paramFound = false;
            for (Object commandArg : commandArgs) {
                JSONObject json = (JSONObject) commandArg;
                Object oo = json.get("outputname");
                if (oo != null) {
                    String strO = (String) oo;
                    if (!strO.isEmpty()) {
                        paramFound = true;
                        break;
                    }
                }
            }

            if (!paramFound) {
                JSONObject arg = new JSONObject();
                arg.put("outputname", jobId);
                commandArgs.add(arg);
            }
        }

        super.configureAsMakeCommand(EXPORT_SCRIPT, caller, commandArgs);
    }

    private JSONArray addExportToOsmApiDbCommandArgs(JSONArray commandArgs, JSONObject oParams) {
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

        // This logic is a little out of line with the rest of
        // ExportJobResource, but since
        // this export option will probably go away completely soon, no point in
        // changing
        // it now.

        // ignoring outputname, since we're only going to have a single mapedit
        // connection configured in the core for now configured in the core for
        // now
        JSONObject arg = new JSONObject();
        File tempOutputDir = new File(TEMP_OUTPUT_PATH);
        if (!tempOutputDir.exists()) {
            tempOutputDir.mkdir();
        }

        // services currently always write changeset with sql
        File tempFile = null;
        try {
            tempFile = File.createTempFile("changeset-", ".osc.sql", tempOutputDir);
        }
        catch (IOException ioe) {
            throw new RuntimeException("Error occurred!", ioe);
        }

        arg.put("changesetoutput", tempFile.getAbsolutePath());
        commandArgs.add(arg);

        // This option allows the job executor return std out to the client.
        // This is the only way
        // I've found to get the conflation summary text back from hoot command
        // line to the UI.
        arg = new JSONObject();
        arg.put("writeStdOutToStatusDetail", "true");
        commandArgs.add(arg);

        Map conflatedMap = getConflatedMap(oParams);

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
            bbox = getMapBounds(conflatedMap);
        }

        setAoi(bbox, commandArgs);

        // put the osm userid in the command args
        if (oParams.get("USER_ID") != null) {
            JSONObject uid = new JSONObject();
            uid.put("userid", oParams.get("USER_ID"));
            commandArgs.add(uid);
        }

        return commandArgs;
    }

    private void addExportToChangesetCommandArgs(JSONArray commandArgs, JSONObject oParams) {
        //handling these inputs a little differently than the rest of ExportJobResource as makes it
        //it possible to test osm2ogrscript with file inputs

        JSONObject commandArg = new JSONObject();
        commandArg.put("input1", OSM_APIDB_URL);
        commandArgs.add(commandArg);

        commandArg = new JSONObject();
        commandArg.put("input2", HOOT_APIDB_URL + "/" + oParams.get("input"));
        commandArgs.add(commandArg);

        if (oParams.get("TASK_BBOX") == null) {
            String msg = "When exporting to a changeset, TASK_BBOX must be specified.";
            throw new WebApplicationException(Response.status(Response.Status.BAD_REQUEST).entity(msg).build());
        }
        BoundingBox bbox = new BoundingBox(oParams.get("TASK_BBOX").toString());
        setAoi(bbox, commandArgs);

        // put the osm userid in the command args
        if (oParams.get("USER_ID") != null) {
            JSONObject uid = new JSONObject();
            uid.put("userid", oParams.get("USER_ID"));
            commandArgs.add(uid);
        }
    }

    private Map getConflatedMap(JSONObject oParams) {
        String mapName = JsonUtils.getParameterValue("input", oParams);
        Long mapId = getMapIdByName(mapName);

        // this may be checked somewhere else down the line...not sure
        if (mapId == null) {
            String msg = "Error exporting data.  No map exists with name: " + mapName;
            throw new WebApplicationException(Response.status(Response.Status.BAD_REQUEST).entity(msg).build());
        }

        Map conflatedMap = new Map(mapId);
        conflatedMap.setDisplayName(mapName);

        return conflatedMap;
    }

    // adding this to satisfy the mock
    private Long getMapIdByName(String conflatedMapName) {
        return DbUtils.getMapIdByName(conflatedMapName);
    }

    // adding this to satisfy the mock
    private java.util.Map<String, String> getMapTags(long mapId) {
        return DbUtils.getMapsTableTags(mapId);
    }

    // adding this to satisfy the mock
    private BoundingBox getMapBounds(Map map) {
        return map.getBounds();
    }

    private void addMapForExportTag(Map map, JSONArray commandArgs) {
        java.util.Map<String, String> tags = getMapTags(map.getId());

        if (!tags.containsKey("osm_api_db_export_time")) {
            String msg = "Error exporting data.  Map with ID: " + map.getId() + " and name: " + map.getDisplayName()
                    + " has no osm_api_db_export_time tag.";
            throw new WebApplicationException(Response.status(Response.Status.CONFLICT).entity(msg).build());
        }

        JSONObject arg = new JSONObject();
        arg.put("changesetsourcedatatimestamp", tags.get("osm_api_db_export_time"));
        commandArgs.add(arg);
    }

    private void setAoi(BoundingBox bounds, JSONArray commandArgs) {
        JSONObject arg = new JSONObject();
        arg.put("aoi", bounds.getMinLon() + "," + bounds.getMinLat() + "," + bounds.getMaxLon() + "," + bounds.getMaxLat());
        commandArgs.add(arg);
    }
}
