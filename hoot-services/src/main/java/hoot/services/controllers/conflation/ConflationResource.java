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

import java.util.UUID;

import javax.ws.rs.Consumes;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;

import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.command.Command;
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
    public Response conflate(String params) {
        String jobId = UUID.randomUUID().toString();

        try {
            JSONParser parser = new JSONParser();
            JSONObject oParams = (JSONObject) parser.parse(params);

            oParams.put("IS_BIG", "false");

            boolean conflatingOsmApiDbData = oneLayerIsOsmApiDb(oParams);
            String confOutputName = oParams.get("OUTPUT_NAME").toString();
            String userEmail = (oParams.get("USER_EMAIL") == null) ? null : oParams.get("USER_EMAIL").toString();

            //Since we're not returning the osm api db layer to the hoot ui, this exception
            //shouldn't actually ever occur, but will leave this check here anyway.
            if (conflatingOsmApiDbData && !OSM_API_DB_ENABLED) {
                String msg = "Attempted to conflate an OSM API database data source but OSM " +
                        "API database support is disabled.";
                throw new WebApplicationException(Response.serverError().entity(msg).build());
            }

            BoundingBox bbox;

            // osm api db related input params have already been validated by
            // this point, so just check to see if any osm api db input is present
            if (conflatingOsmApiDbData && OSM_API_DB_ENABLED) {
                validateOsmApiDbConflateParams(oParams);

                String secondaryParameterKey = (firstLayerIsOsmApiDb(oParams)) ? "INPUT2" : "INPUT1";

                //Record the aoi of the conflation job (equal to that of the secondary layer), as
                //we'll need it to detect conflicts at export time.
                long secondaryMapId = Long.parseLong(JsonUtils.getParameterValue(secondaryParameterKey, oParams));
                if (!Map.mapExists(secondaryMapId)) {
                    String msg = "No secondary map exists with ID: " + secondaryMapId;
                    throw new WebApplicationException(Response.status(Status.BAD_REQUEST).entity(msg).build());
                }

                if (oParams.get("TASK_BBOX") != null) {
                    bbox = new BoundingBox(oParams.get("TASK_BBOX").toString());
                }
                else {
                    Map secondaryMap = new Map(secondaryMapId);
                    bbox = secondaryMap.getBounds();
                }
            }
            else {
                bbox = null;
            }

            Command[] commands = {
                () -> {
                    ExternalCommand conflateCommand = conflateCommandFactory.build(oParams.toJSONString(), bbox, this.getClass());
                    return externalCommandManager.exec(jobId, conflateCommand);
                },

                () -> {
                    InternalCommand updateTagsCommand = updateTagsCommandFactory.build(oParams.toJSONString(), confOutputName, jobId);
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

    static boolean oneLayerIsOsmApiDb(JSONObject inputParams) {
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
}