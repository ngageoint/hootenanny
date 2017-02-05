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
package hoot.services.controllers.nonblocking.conflation;

import static hoot.services.HootProperties.*;

import java.io.File;
import java.time.LocalDateTime;
import java.util.HashMap;
import java.util.UUID;

import javax.ws.rs.Consumes;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;

import org.apache.commons.io.FileUtils;
import org.joda.time.DateTime;
import org.joda.time.DateTimeZone;
import org.joda.time.format.DateTimeFormat;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.command.Command;
import hoot.services.command.CommandResult;
import hoot.services.command.ExternalCommand;
import hoot.services.command.InternalCommand;
import hoot.services.command.RasterToTilesCommandFactory;
import hoot.services.job.ChainJob;
import hoot.services.controllers.nonblocking.AsynchronousJobResource;
import hoot.services.controllers.nonblocking.JobId;
import hoot.services.geo.BoundingBox;
import hoot.services.models.osm.Map;
import hoot.services.utils.DbUtils;
import hoot.services.utils.JsonUtils;


@Controller
@Path("/conflation")
@Transactional
public class ConflationResource extends AsynchronousJobResource {
    private static final Logger logger = LoggerFactory.getLogger(ConflationResource.class);

    @Autowired
    private RasterToTilesCommandFactory rasterToTilesCommandFactory;

    private static class UpdateTagsCommand implements InternalCommand {
        private final java.util.Map<String, String> tags;
        private final String mapName;
        private final String jobId;

        UpdateTagsCommand(java.util.Map<String, String> tags, String mapName, String jobId) {
            this.tags = tags;
            this.mapName = mapName;
            this.jobId = jobId;
        }

        @Override
        public CommandResult execute() {
            CommandResult commandResult = new CommandResult();
            commandResult.setJobId(jobId);
            commandResult.setCommand("updateTagsDirect()");
            commandResult.setStart(LocalDateTime.now());

            updateTagsDirect();

            commandResult.setFinish(LocalDateTime.now());
            commandResult.setExitCode(CommandResult.SUCCESS);

            return commandResult;
        }

        private void updateTagsDirect() {
            try {
                // Currently we do not have any way to get map id directly from hoot
                // core command when it runs so for now we need get the all the map ids matching name and pick
                // first one..
                // THIS WILL NEED TO CHANGE when we implement handle map by Id instead of name..

                Long mapId = DbUtils.getMapIdByName(mapName);
                if (mapId != null) {
                    // Hack alert!
                    // Add special handling of stats tag key
                    // We need to read the file in here, because the file doesn't
                    // exist at the time the updateMapsTagsCommand job is created in ConflationResource.java
                    String statsKey = "stats";
                    if (tags.containsKey(statsKey)) {
                        String statsName = tags.get(statsKey);
                        File statsFile = new File(statsName);
                        if (statsFile.exists()) {
                            String stats = FileUtils.readFileToString(statsFile, "UTF-8");
                            tags.put(statsKey, stats);

                            if (!statsFile.delete()) {
                                logger.error("Error deleting {} file", statsFile.getAbsolutePath());
                            }
                        }
                        else {
                            logger.error("Can't find {}", statsName);
                            tags.remove(statsKey);
                        }
                    }

                    DbUtils.updateMapsTableTags(tags, mapId);
                }
            }
            catch (Exception ex) {
                String msg = "Failure update map tags resource" + ex.getMessage();
                throw new RuntimeException(msg, ex);
            }
        }
    }


    public ConflationResource() {
        super(CONFLATE_MAKEFILE_PATH);
    }

    /**
     * Conflate service operates like a standard ETL service. The conflate
     * service specifies the input files, conflation type, match threshold, miss
     * threshold, and output file name. The conflation type can be specified as
     * the average of the two input datasets or based on a single input file
     * that is intended to be the reference dataset. It has two fronts, WPS and
     * standard rest end point.
     *
     * that is intended to be the reference dataset.
     *
     * POST hoot-services/conflation/execute
     *
     * @param params
     *            parameters in json format :
     *            <INPUT1_TYPE>Conflation input type [OSM] | [OGR] | [DB] | [OSM_API_DB]</INPUT1_TYPE>
     *            <INPUT1>Conlfation input 1</INPUT1>
     *            <INPUT2_TYPE>Conflation input type [OSM] | [OGR] | [DB] </INPUT2_TYPE>
     *            <INPUT2>Conlfation input 2</INPUT2>
     *            <OUTPUT_NAME>Conflation operation output name</OUTPUT_NAME>
     *            <CONFLATION_TYPE>[Average] | [Reference]</CONFLATION_TYPE>
     *            <REFERENCE_LAYER>
     *                      The reference layer which will be dominant tags. Default is 1 and if 2 selected, layer 2
     *                      tags will be dominant with layer 1 as geometry snap layer.
     *            </REFERENCE_LAYER>
     *            <AUTO_TUNNING> Not used. Always false</AUTO_TUNNING>
     *            <COLLECT_STATS> true to collect conflation statistics</COLLECT_STATS>
     *            <GENERATE_REPORT> true to generate conflation report</GENERATE_REPORT>
     *            <TIME_STAMP> Time stamp used in generated report if GENERATE_REPORT is true</TIME_STAMP>
     *            <USER_EMAIL> Email address of the user requesting the conflation job.</USER_EMAIL>
     *            <ADV_OPTIONS>Advanced options list for hoot-core command </ADV_OPTIONS>
     * @return Job ID
     */
    @POST
    @Path("/execute")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public JobId process(String params) {
        logger.debug("Conflation resource raw request: {}", params);

        String jobId = UUID.randomUUID().toString();

        try {
            JSONParser pars = new JSONParser();
            JSONObject oParams = (JSONObject) pars.parse(params);

            boolean osmApiDbEnabled = Boolean.parseBoolean(OSM_API_DB_ENABLED);
            boolean conflatingOsmApiDbData = oneLayerIsOsmApiDb(oParams);

            //Since we're not returning the osm api db layer to the hoot ui, this exception
            //shouldn't actually ever occur, but will leave this check here anyway.
            if (conflatingOsmApiDbData && !osmApiDbEnabled) {
                String msg = "Attempted to conflate an OSM API database data source but OSM " +
                        "API database support is disabled.";
                throw new WebApplicationException(Response.serverError().entity(msg).build());
            }

            oParams.put("IS_BIG", "false");
            String confOutputName = oParams.get("OUTPUT_NAME").toString();
            String input1Name = oParams.get("INPUT1").toString();
            String input2Name = oParams.get("INPUT2").toString();

            JSONArray commandArgs = parseParams(oParams.toJSONString());

            // add map tags
            // WILL BE DEPRECATED WHEN CORE IMPLEMENTS THIS
            java.util.Map<String, String> tags = new HashMap<>();
            tags.put("input1", input1Name);
            tags.put("input2", input2Name);

            // Need to reformat the list of hoot command options to json properties
            tags.put("params", JsonUtils.escapeJson(params));

            // Hack alert!
            // Write stats file name to tags, if the file exists when this updateMapsTagsCommand job is run, the
            // file will be read and its contents placed in the stats tag.
            if ((oParams.get("COLLECT_STATS") != null)
                    && oParams.get("COLLECT_STATS").toString().equalsIgnoreCase("true")) {
                String statsName = HOME_FOLDER + "/" + RPT_STORE_PATH + "/" + confOutputName + "-stats.csv";
                tags.put("stats", statsName);
            }

            // osm api db related input params have already been validated by
            // this point, so just check to see if any osm api db input is present
            if (conflatingOsmApiDbData && osmApiDbEnabled) {
                validateOsmApiDbConflateParams(oParams);

                String secondaryParameterKey = (firstLayerIsOsmApiDb(oParams)) ? "INPUT2" : "INPUT1";

                //Record the aoi of the conflation job (equal to that of the secondary layer), as
                //we'll need it to detect conflicts at export time.
                long secondaryMapId = Long.parseLong(getParameterValue(secondaryParameterKey, commandArgs));
                if (!mapExists(secondaryMapId)) {
                    String msg = "No secondary map exists with ID: " + secondaryMapId;
                    throw new WebApplicationException(Response.status(Status.BAD_REQUEST).entity(msg).build());
                }

                BoundingBox bbox;
                if (oParams.get("TASK_BBOX") != null) {
                    bbox = new BoundingBox(oParams.get("TASK_BBOX").toString());
                }
                else {
                    Map secondaryMap = new Map(secondaryMapId);
                    bbox = getMapBounds(secondaryMap);
                }
                setAoi(bbox, commandArgs);

                // write a timestamp representing the time the osm api db data was queried out
                // from the source; to be used conflict detection during export of conflated
                // data back into the osm api db at a later time; timestamp must be 24 hour utc
                // to match rails port
                String now = DateTimeFormat
                        .forPattern(DbUtils.OSM_API_TIMESTAMP_FORMAT)
                        .withZone(DateTimeZone.UTC)
                        .print(new DateTime());

                tags.put("osm_api_db_export_time", now);
            }

            Object oUserEmail = oParams.get("USER_EMAIL");
            String userEmail = (oUserEmail == null) ? null : oUserEmail.toString();

            JSONObject hootDBURL = new JSONObject();
            hootDBURL.put("DB_URL", HOOT_APIDB_URL);
            commandArgs.add(hootDBURL);

            JSONObject osmAPIDBURL = new JSONObject();
            osmAPIDBURL.put("OSM_API_DB_URL", OSM_APIDB_URL);
            commandArgs.add(osmAPIDBURL);

            Command[] commands = {
                    () -> {
                        ExternalCommand conflationCommand = super.createMakeScriptJobReq(commandArgs);
                        return externalCommandManager.exec(jobId, conflationCommand);
                    },
                    () -> {
                        InternalCommand updateTagsCommand = new UpdateTagsCommand(tags, confOutputName, jobId);
                        return internalCommandManager.exec(jobId, updateTagsCommand);
                    },
                    () -> {
                        ExternalCommand rasterToTilesCommand = rasterToTilesCommandFactory.createExternalCommand(confOutputName, userEmail);
                        return externalCommandManager.exec(jobId, rasterToTilesCommand);
                    }
            };

            ChainJob chainJob = new ChainJob();
            chainJob.setJobId(jobId);
            chainJob.setCommands(commands);

            super.processChainJob(chainJob);
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception e) {
            String msg = "Error during process call!  Params: " + params;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return new JobId(jobId);
    }

    private static boolean oneLayerIsOsmApiDb(JSONObject inputParams) {
        return firstLayerIsOsmApiDb(inputParams) || secondLayerIsOsmApiDb(inputParams);
    }

    private static boolean firstLayerIsOsmApiDb(JSONObject inputParams) {
        return inputParams.get("INPUT1_TYPE").toString().toUpperCase().equals("OSM_API_DB");
    }

    private static boolean secondLayerIsOsmApiDb(JSONObject inputParams) {
        return inputParams.get("INPUT2_TYPE").toString().toUpperCase().equals("OSM_API_DB");
    }

    private static void validateOsmApiDbConflateParams(JSONObject inputParams) {
        // default REFERENCE_LAYER = 1
        if (inputParams.get("REFERENCE_LAYER") != null) {
            if ((firstLayerIsOsmApiDb(inputParams) && inputParams.get("REFERENCE_LAYER").toString().toUpperCase().equals("2"))
                    || ((secondLayerIsOsmApiDb(inputParams)) && inputParams.get("REFERENCE_LAYER").toString().toUpperCase().equals("1"))) {
                String msg = "OSM_API_DB not allowed as secondary input type.";
                throw new WebApplicationException(Response.status(Status.BAD_REQUEST).entity(msg).build());
            }
        }
        else if (secondLayerIsOsmApiDb(inputParams)) {
            String msg = "OSM_API_DB not allowed as secondary input type.";
            throw new WebApplicationException(Response.status(Status.BAD_REQUEST).entity(msg).build());
        }
    }

    // adding this to satisfy the mock
    BoundingBox getMapBounds(Map map) {
        return map.getBounds();
    }

    // adding this to satisfy the mock
    boolean mapExists(long id) {
        return Map.mapExists(id);
    }

    private static void setAoi(BoundingBox bounds, JSONArray commandArgs) {
        JSONObject arg = new JSONObject();
        arg.put("conflateaoi", bounds.getMinLon() + "," + bounds.getMinLat() + "," + bounds.getMaxLon() + "," + bounds.getMaxLat());
        commandArgs.add(arg);
    }
}