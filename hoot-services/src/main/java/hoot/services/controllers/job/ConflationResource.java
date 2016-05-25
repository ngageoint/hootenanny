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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.job;

import java.sql.Connection;
import java.sql.Timestamp;
import java.util.Calendar;
import java.util.HashMap;
import java.util.List;
import java.util.UUID;

import javax.ws.rs.Consumes;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.HootProperties;
import hoot.services.db.DbUtils;
import hoot.services.geo.BoundingBox;
import hoot.services.models.osm.Map;
import hoot.services.utils.ResourceErrorHandler;


@Path("/conflation")
public class ConflationResource extends JobControllerBase {
    private static final Logger log = LoggerFactory.getLogger(ConflationResource.class);
    protected static String _tileServerPath = null;
    protected static String _homeFolder = null;
    protected static String _confAdvOptsScript = null;
    protected static String _rptStorePath = null;

    public ConflationResource() {
        try {
            if (processScriptName == null) {
                processScriptName = HootProperties.getProperty("ConflateMakefilePath");
            }
            if (_tileServerPath == null) {
                _tileServerPath = HootProperties.getProperty("tileServerPath");
            }
            if (_homeFolder == null) {
                _homeFolder = HootProperties.getProperty("homeFolder");
            }
            if (_confAdvOptsScript == null) {
                _confAdvOptsScript = HootProperties.getProperty("confAdvOptsScript");
            }
            if (_rptStorePath == null) {
                _rptStorePath = HootProperties.getProperty("reportDataPath");
            }
        }
        catch (Exception ex) {
            log.error(ex.getMessage());
        }
    }

    /**
     * Conflate service operates like a standard ETL service. The conflate
     * service specifies the input files, conflation type, match threshold, miss
     * threshold, and output file name. The conflation type can be specified as
     * the average of the two input datasets or based on a single input file
     * that is intended to be the reference dataset. It has two fronts, WPS and
     * standard rest end point.
     * 
     * POST hoot-services/conflation/execute
     * 
     * @param Conflation
     *            parameters in json format <INPUT1_TYPE> Conflation input type
     *            [OSM] | [OGR] | [DB] | [OSM_API_DB] </INPUT1_TYPE> <INPUT1>
     *            Conlfation input 1 </INPUT1> <INPUT2_TYPE> Conflation input
     *            type [OSM] | [OGR] | [DB] </INPUT2_TYPE> <INPUT2> Conlfation
     *            input 2 </INPUT2> <OUTPUT_NAME> Conflation operation output
     *            name </OUTPUT_NAME> <CONFLATION_TYPE> [Average] | [Reference]
     *            </CONFLATION_TYPE> <REFERENCE_LAYER> The reference layer which
     *            will be dominant tags. Default is 1 and if 2 selected, layer 2
     *            tags will be dominant with layer 1 as geometry snap layer.
     *            </REFERENCE_LAYER> <AUTO_TUNNING> Not used. Always false
     *            </AUTO_TUNNING> <COLLECT_STATS> true to collect conflation
     *            statistics </COLLECT_STATS> <GENERATE_REPORT> true to generate
     *            conflation report </GENERATE_REPORT> <TIME_STAMP> Time stamp
     *            used in generated report if GENERATE_REPORT is true
     *            </TIME_STAMP> <USER_EMAIL> Email address of the user
     *            requesting the conflation job. </USER_EMAIL> <ADV_OPTIONS>
     *            Advanced options list for hoot-core command </ADV_OPTIONS>
     * @return Job ID
     * @throws Exception
     */
    @POST
    @Path("/execute")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.TEXT_PLAIN)
    public Response process(String params) throws Exception {
        // log.debug("Conflation resource raw request: " + params);

        Connection conn = DbUtils.createConnection();
        String jobId = UUID.randomUUID().toString();
        try {
            JSONParser pars = new JSONParser();
            JSONObject oParams = (JSONObject) pars.parse(params);

            validateOsmApiDbConflateParams(oParams);

            oParams.put("IS_BIG", "false");
            String confOutputName = oParams.get("OUTPUT_NAME").toString();
            String input1Name = oParams.get("INPUT1").toString();
            String input2Name = oParams.get("INPUT2").toString();

            Object oTunn = oParams.get("AUTO_TUNNING");
            if (oTunn != null) {
                String autoTune = oTunn.toString();
                if (autoTune.equalsIgnoreCase("true")) {
                    javax.management.MBeanServer mBeanServer = java.lang.management.ManagementFactory
                            .getPlatformMBeanServer();
                    Object attribute = mBeanServer.getAttribute(new javax.management.ObjectName("java.lang", "type",
                            "OperatingSystem"), "TotalPhysicalMemorySize");
                    long totalMemSize = Long.parseLong(attribute.toString());

                    Long input1Size = Long.parseLong(oParams.get("INPUT1_ESTIMATE").toString());

                    Long input2Size = Long.parseLong(oParams.get("INPUT2_ESTIMATE").toString());

                    if ((input1Size + input2Size) > (totalMemSize / 2)) {
                        oParams.put("IS_BIG", "true");
                        processScriptName = "makebigconflate";
                    }
                }
            }

            JSONArray commandArgs = parseParams(oParams.toJSONString());
            JSONObject conflationCommand = _createMakeScriptJobReq(commandArgs);

            final boolean conflatingOsmApiDbData = oneLayerIsOsmApiDb(oParams);

            if (conflatingOsmApiDbData) {
                String secondaryParameterKey = null;
                if (firstLayerIsOsmApiDb(oParams)) {
                    secondaryParameterKey = "INPUT2";
                }
                else {
                    secondaryParameterKey = "INPUT1";
                }
                Map secondaryMap = getSecondaryMap(getParameterValue(secondaryParameterKey, commandArgs), conn);
                setAoi(secondaryMap, commandArgs);
            }

            // add map tags
            // WILL BE DEPRECATED WHEN CORE IMPLEMENTS THIS
            java.util.Map<String, String> tags = new HashMap<String, String>();
            tags.put("input1", input1Name);
            tags.put("input2", input2Name);
            // System.out.println(params);
            // Need to reformat the list of hoot command options to json
            // properties
            tags.put("params", DbUtils.escapeJson(params));
            // Hack alert!
            // Write stats file name to tags, if the file exists
            // when this updateMapsTagsCommand job is run, the
            // file will be read and its contents placed in the
            // stats tag.
            if (oParams.get("COLLECT_STATS") != null
                    && oParams.get("COLLECT_STATS").toString().equalsIgnoreCase("true")) {
                String statsName = _homeFolder + "/" + _rptStorePath + "/" + confOutputName + "-stats.csv";
                tags.put("stats", statsName);
            }

            // osm api db related input params have already been validated by
            // this
            // point, so just check to
            // see if any osm api db input is present
            if (conflatingOsmApiDbData) {
                // write a timestamp representing the time the osm api db data
                // was
                // queried out from the source;
                // to be used during export of conflated data back into the osm
                // api
                // db at a later time
                final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
                tags.put("osm_api_db_export_time", now.toString());
            }

            JSONArray mapTagsArgs = new JSONArray();
            JSONObject param = new JSONObject();
            param.put("value", tags);
            param.put("paramtype", java.util.Map.class.getName());
            param.put("isprimitivetype", "false");
            mapTagsArgs.add(param);

            param = new JSONObject();
            param.put("value", confOutputName);
            param.put("paramtype", String.class.getName());
            param.put("isprimitivetype", "false");
            mapTagsArgs.add(param);

            JSONObject updateMapsTagsCommand = _createReflectionJobReq(mapTagsArgs,
                    "hoot.services.controllers.osm.MapResource", "updateTagsDirect");

            Object oUserEmail = oParams.get("USER_EMAIL");
            String userEmail = (oUserEmail == null) ? null : oUserEmail.toString();
            // Density Raster
            JSONArray rasterTilesArgs = new JSONArray();
            JSONObject rasterTilesparam = new JSONObject();
            rasterTilesparam.put("value", confOutputName);
            rasterTilesparam.put("paramtype", String.class.getName());
            rasterTilesparam.put("isprimitivetype", "false");
            rasterTilesArgs.add(rasterTilesparam);

            rasterTilesparam = new JSONObject();
            rasterTilesparam.put("value", userEmail);
            rasterTilesparam.put("paramtype", String.class.getName());
            rasterTilesparam.put("isprimitivetype", "false");
            rasterTilesArgs.add(rasterTilesparam);

            JSONObject ingestOSMResource = _createReflectionJobReq(rasterTilesArgs,
                    "hoot.services.controllers.ingest.RasterToTilesService", "ingestOSMResourceDirect");

            JSONArray jobArgs = new JSONArray();
            jobArgs.add(conflationCommand);
            jobArgs.add(updateMapsTagsCommand);
            jobArgs.add(ingestOSMResource);

            log.debug(jobArgs.toJSONString());
            postChainJobRquest(jobId, jobArgs.toJSONString());
        }
        finally {
            DbUtils.closeConnection(conn);
        }

        JSONObject res = new JSONObject();
        res.put("jobid", jobId);
        return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
    }

    private static boolean oneLayerIsOsmApiDb(final JSONObject inputParams) {
        return inputParams.get("INPUT1_TYPE").toString().toUpperCase().equals("OSM_API_DB")
                || inputParams.get("INPUT2_TYPE").toString().toUpperCase().equals("OSM_API_DB");
    }

    private static boolean firstLayerIsOsmApiDb(final JSONObject inputParams) {
        return inputParams.get("INPUT1_TYPE").toString().toUpperCase().equals("OSM_API_DB");
    }

    private static void validateOsmApiDbConflateParams(final JSONObject inputParams) {
        // default REFERENCE_LAYER = 1
        if (inputParams.get("REFERENCE_LAYER") != null) {
            if ((inputParams.get("INPUT1_TYPE").toString().toUpperCase().equals("OSM_API_DB") && inputParams
                    .get("REFERENCE_LAYER").toString().toUpperCase().equals("2"))
                    || (inputParams.get("INPUT2_TYPE").toString().toUpperCase().equals("OSM_API_DB") && inputParams
                            .get("REFERENCE_LAYER").toString().toUpperCase().equals("1"))) {
                ResourceErrorHandler.handleError("OSM_API_DB not allowed as secondary input type.", Status.BAD_REQUEST,
                        log);
            }
        }
        else if (inputParams.get("INPUT2_TYPE").toString().toUpperCase().equals("OSM_API_DB")) {
            ResourceErrorHandler
                    .handleError("OSM_API_DB not allowed as secondary input type.", Status.BAD_REQUEST, log);
        }
    }

    private Map getSecondaryMap(final String mapName, Connection conn) throws Exception {
        List<Long> mapIds = getMapIdsByName(mapName, conn);
        // we don't expect the services to try to conflate a map that has
        // multiple
        // name entries, but check for it anyway
        if (mapIds.size() > 1) {
            ResourceErrorHandler.handleError("Error conflating data.  Multiple maps with name: " + mapName,
                    Status.BAD_REQUEST, log);
        }
        // this may be checked somewhere else down the line...not sure
        else if (mapIds.size() == 0) {
            ResourceErrorHandler.handleError("Error conflating data.  No map exists with name: " + mapName,
                    Status.BAD_REQUEST, log);
        }
        Map secondaryMap = new Map(mapIds.get(0), conn);
        secondaryMap.setDisplayName(mapName);
        return secondaryMap;
    }

    // adding this to satisfy the mock
    protected List<Long> getMapIdsByName(final String mapName, Connection conn) throws Exception {
        return DbUtils.getMapIdsByName(conn, mapName);
    }

    // adding this to satisfy the mock
    protected BoundingBox getMapBounds(final Map map) throws Exception {
        return map.getBounds();
    }

    private void setAoi(final Map secondaryMap, JSONArray commandArgs) throws Exception {
        final BoundingBox bounds = getMapBounds(secondaryMap);
        JSONObject arg = new JSONObject();
        arg.put("conflateaoi", String.valueOf(bounds.getMinLon()) + "," + String.valueOf(bounds.getMinLat()) + ","
                + String.valueOf(bounds.getMaxLon()) + "," + String.valueOf(bounds.getMaxLat()));
        commandArgs.add(arg);
    }
}