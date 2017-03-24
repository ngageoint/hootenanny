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
package hoot.services.controllers.conflation;

import static hoot.services.HootProperties.OSM_API_DB_ENABLED;
import static hoot.services.HootProperties.RPT_STORE_PATH;

import java.io.File;
import java.io.IOException;
import java.nio.charset.Charset;
import java.util.UUID;

import javax.ws.rs.Consumes;
import javax.ws.rs.DefaultValue;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;

import org.apache.commons.io.FileUtils;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.command.Command;
import hoot.services.command.CommandResult;
import hoot.services.command.ExternalCommand;
import hoot.services.command.ExternalCommandManager;
import hoot.services.command.InternalCommand;
import hoot.services.command.InternalCommandManager;
import hoot.services.controllers.ExportRenderDBCommandFactory;
import hoot.services.geo.BoundingBox;
import hoot.services.job.Job;
import hoot.services.job.JobProcessor;
import hoot.services.models.osm.Map;
import hoot.services.utils.JsonUtils;


@Controller
@Path("/conflation")
@Transactional
public class ConflationResource {
    private static final Logger logger = LoggerFactory.getLogger(ConflationResource.class);

    @Autowired
    private JobProcessor jobProcessor;

    @Autowired
    private ExternalCommandManager externalCommandManager;

    @Autowired
    private InternalCommandManager internalCommandManager;

    @Autowired
    private ConflateCommandFactory conflateCommandFactory;

    @Autowired
    private ExportRenderDBCommandFactory exportRenderDBCommandFactory;

    @Autowired
    private UpdateTagsCommandFactory updateTagsCommandFactory;


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
     *
     *     INPUT1_TYPE: Conflation input type [OSM] | [OGR] | [DB] | [OSM_API_DB]
     *     INPUT2_TYPE: Conflation input type [OSM] | [OGR] | [DB]
     *     INPUT1: Conlfation input 1
     *     INPUT2: Conlfation input 2
     *     OUTPUT_NAME: Conflation operation output name
     *     CONFLATION_TYPE: [Average] | [Reference]
     *     REFERENCE_LAYER:
     *         The reference layer which will be dominant tags. Default is 1 and if 2 selected, layer 2
     *         tags will be dominant with layer 1 as geometry snap layer.
     *
     *     COLLECT_STATS: true to collect conflation statistics
     *     ADV_OPTIONS: Advanced options list for hoot-core command
     *
     *     GENERATE_REPORT: Not used.  true to generate conflation report
     *     TIME_STAMP: Not used   Time stamp used in generated report if GENERATE_REPORT is true
     *     USER_EMAIL: Not used.  Email address of the user requesting the conflation job
     *     AUTO_TUNNING: Not used. Always false
     *
     * @return Job ID
     */
    @POST
    @Path("/execute")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response conflate(String params,
                             @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {
        String jobId = UUID.randomUUID().toString();

        try {
            java.util.Map<String, String> paramMap = JsonUtils.jsonToMap(params);

            boolean conflatingOsmApiDbData = oneLayerIsOsmApiDb(paramMap);

            //Since we're not returning the osm api db layer to the hoot ui, this exception
            //shouldn't actually ever occur, but will leave this check here anyway.
            if (conflatingOsmApiDbData && !OSM_API_DB_ENABLED) {
                throw new IllegalArgumentException("Attempted to conflate an OSM API database data source but OSM " +
                        "API database support is disabled.");
            }

            BoundingBox bbox;

            // osm api db related input params have already been validated by
            // this point, so just check to see if any osm api db input is present
            if (conflatingOsmApiDbData && OSM_API_DB_ENABLED) {
                validateOsmApiDbConflateParams(paramMap);

                String secondaryParameterKey = (firstLayerIsOsmApiDb(paramMap)) ? "INPUT2" : "INPUT1";

                //Record the aoi of the conflation job (equal to that of the secondary layer), as
                //we'll need it to detect conflicts at export time.
                long secondaryMapId = Long.parseLong(paramMap.get(secondaryParameterKey));
                if (!Map.mapExists(secondaryMapId)) {
                    String msg = "No secondary map exists with ID: " + secondaryMapId;
                    throw new WebApplicationException(Response.status(Status.BAD_REQUEST).entity(msg).build());
                }

                if (paramMap.containsKey("TASK_BBOX")) {
                    bbox = new BoundingBox(paramMap.get("TASK_BBOX"));
                }
                else {
                    Map secondaryMap = new Map(secondaryMapId);
                    bbox = secondaryMap.getBounds();
                }
            }
            else {
                bbox = null;
            }

            String confOutputName = paramMap.get("OUTPUT_NAME");

            Command[] commands = {
                () -> {
                    ExternalCommand conflateCommand = conflateCommandFactory.build(paramMap, bbox, debugLevel, this.getClass());
                    CommandResult commandResult = externalCommandManager.exec(jobId, conflateCommand);

                    if (Boolean.valueOf(paramMap.get("COLLECT_STATS"))) {
                        File statsFile = new File(RPT_STORE_PATH, confOutputName + "-stats.csv");
                        try {
                            FileUtils.write(statsFile, commandResult.getStdout(), Charset.defaultCharset());
                        }
                        catch (IOException ioe) {
                            throw new RuntimeException("Error writing to " + statsFile.getAbsolutePath(), ioe);
                        }
                    }

                    return commandResult;
                },

                () -> {
                    InternalCommand updateTagsCommand = updateTagsCommandFactory.build(jobId, params, confOutputName, this.getClass());
                    return internalCommandManager.exec(jobId, updateTagsCommand);
                },

                () -> {
                    ExternalCommand exportRenderDBCommand = exportRenderDBCommandFactory.build(confOutputName, this.getClass());
                    return externalCommandManager.exec(jobId, exportRenderDBCommand);
                }
            };

            jobProcessor.process(new Job(jobId, commands));
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception e) {
            String msg = "Error during process call!  Params: " + params;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        JSONObject json = new JSONObject();
        json.put("jobid", jobId);

        return Response.ok(json.toJSONString()).build();
    }

    static boolean oneLayerIsOsmApiDb(java.util.Map<String, String> inputParams) {
        return firstLayerIsOsmApiDb(inputParams) || secondLayerIsOsmApiDb(inputParams);
    }

    private static boolean firstLayerIsOsmApiDb(java.util.Map<String, String> inputParams) {
        return inputParams.get("INPUT1_TYPE").equalsIgnoreCase("OSM_API_DB");
    }

    private static boolean secondLayerIsOsmApiDb(java.util.Map<String, String> inputParams) {
        return inputParams.get("INPUT2_TYPE").equalsIgnoreCase("OSM_API_DB");
    }

    private static void validateOsmApiDbConflateParams(java.util.Map<String, String> inputParams) {
        // default REFERENCE_LAYER = 1
        if (inputParams.containsKey("REFERENCE_LAYER")) {
            if ((firstLayerIsOsmApiDb(inputParams) && inputParams.get("REFERENCE_LAYER").equalsIgnoreCase("2"))
                    || ((secondLayerIsOsmApiDb(inputParams)) && inputParams.get("REFERENCE_LAYER").equalsIgnoreCase("1"))) {
                String msg = "OSM_API_DB not allowed as secondary input type.";
                throw new WebApplicationException(Response.status(Status.BAD_REQUEST).entity(msg).build());
            }
        }
        else if (secondLayerIsOsmApiDb(inputParams)) {
            String msg = "OSM_API_DB not allowed as secondary input type.";
            throw new WebApplicationException(Response.status(Status.BAD_REQUEST).entity(msg).build());
        }
    }
}