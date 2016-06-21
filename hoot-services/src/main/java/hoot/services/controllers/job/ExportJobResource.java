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

import java.io.File;
import java.sql.Connection;
import java.util.List;
import java.util.UUID;

import javax.annotation.PreDestroy;
import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.ResponseBuilder;
import javax.ws.rs.core.Response.Status;

import org.apache.commons.io.FileUtils;
import org.apache.commons.lang3.StringUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.HootProperties;
import hoot.services.controllers.wfs.WfsManager;
import hoot.services.db.DataDefinitionManager;
import hoot.services.db.DbUtils;
import hoot.services.geo.BoundingBox;
import hoot.services.models.osm.Map;
import hoot.services.nativeInterfaces.NativeInterfaceException;
import hoot.services.utils.ResourceErrorHandler;


@Path("/export")
public class ExportJobResource extends JobControllerBase {
    private static final Logger log = LoggerFactory.getLogger(ExportJobResource.class);
    private static final String tempOutputPath = HootProperties.getProperty("tempOutputPath");
    private final String wfsStoreDb = HootProperties.getProperty("wfsStoreDb");
    private final String homeFolder = HootProperties.getProperty("homeFolder");
    private final String translationExtPath = HootProperties.getProperty("translationExtPath");

    private String delPath;

    public ExportJobResource() {
        super(HootProperties.getProperty("ExportScript"));
    }

    @PreDestroy
    public void preDestroy() {
        try {
            if (delPath != null) {
                File workfolder = new File(delPath);
                if (workfolder.exists() && workfolder.isDirectory()) {
                    FileUtils.deleteDirectory(workfolder);
                }
            }
        }
        catch (Exception ex) {
            log.error(ex.getMessage());
        }
    }

    /**
     * Asynchronous export service.
     *
     * POST hoot-services/job/export/execute
     *
     * { "translation":"MGCP.js", //Translation script name. "inputtype":"db",
     * //[db | file] db means input from hoot db will be used. file mean a file
     * path will be specified. "input":"ToyTestA", //Input name. for inputtype =
     * db then specify name from hoot db. For inputtype=file, specify full path
     * to a file. "outputtype":"gdb", //[gdb | shp | wfs | osm_api_db]. gdb will
     * produce file gdb, shp will output shapefile. if outputtype = wfs then a
     * wfs front end will be created. osm_api_db will derive and apply a
     * changeset to an OSM API database "removereview" : "false" //? }
     *
     * @param params
     * @return Job ID
     * @throws Exception
     */
    @POST
    @Path("/execute")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.TEXT_PLAIN)
    public Response process(String params) throws Exception {
        Connection conn = DbUtils.createConnection();
        String jobId = UUID.randomUUID().toString();
        jobId = "ex_" + jobId.replace("-", "");
        try {
            JSONArray commandArgs = parseParams(params);

            JSONObject arg = new JSONObject();
            arg.put("outputfolder", tempOutputPath + "/" + jobId);
            commandArgs.add(arg);

            arg = new JSONObject();
            arg.put("output", jobId);
            commandArgs.add(arg);

            String type = getParameterValue("outputtype", commandArgs);
            if ("wfs".equalsIgnoreCase(type)) {
                arg = new JSONObject();
                arg.put("outputname", jobId);
                commandArgs.add(arg);

                HootProperties.getProperty("dbName");
                String userid = HootProperties.getProperty("dbUserId");
                String pwd = HootProperties.getProperty("dbPassword");
                String host = HootProperties.getProperty("dbHost");
                String[] hostParts = host.split(":");

                String pgUrl = "host='" + hostParts[0] + "' port='" + hostParts[1] + "' user='" + userid
                        + "' password='" + pwd + "' dbname='" + wfsStoreDb + "'";

                arg = new JSONObject();
                arg.put("PG_URL", pgUrl);
                commandArgs.add(arg);

                JSONObject osm2orgCommand = _createPostBody(commandArgs);
                // this may need change in the future if we decided to use user
                // defined ouputname..
                String outname = jobId;

                JSONArray wfsArgs = new JSONArray();
                JSONObject param = new JSONObject();
                param.put("value", outname);
                param.put("paramtype", String.class.getName());
                param.put("isprimitivetype", "false");
                wfsArgs.add(param);

                JSONObject createWfsResCommand = createReflectionSycJobReq(wfsArgs,
                        "hoot.services.controllers.wfs.WfsManager", "createWfsResource");

                JSONArray jobArgs = new JSONArray();
                jobArgs.add(osm2orgCommand);
                jobArgs.add(createWfsResCommand);

                postChainJobRquest(jobId, jobArgs.toJSONString());
            }
            else if ("osm_api_db".equalsIgnoreCase(type)) {
                commandArgs = getExportToOsmApiDbCommandArgs(commandArgs, conn);
                postJobRquest(jobId, createPostBody(commandArgs));
            }
            else {
                // replace with with getParameterValue
                boolean paramFound = false;
                for (Object commandArg : commandArgs) {
                    JSONObject jo = (JSONObject) commandArg;
                    Object oo = jo.get("outputname");
                    if (oo != null) {
                        String strO = (String) oo;
                        if (!strO.isEmpty()) {
                            paramFound = true;
                            break;
                        }
                    }
                }

                if (!paramFound) {
                    arg = new JSONObject();
                    arg.put("outputname", jobId);
                    commandArgs.add(arg);
                }

                String argStr = createPostBody(commandArgs);
                postJobRquest(jobId, argStr);
            }
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error exporting data: " + ex, Status.INTERNAL_SERVER_ERROR, log);
        }
        finally {
            DbUtils.closeConnection(conn);
        }
        JSONObject res = new JSONObject();
        res.put("jobid", jobId);
        return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
    }

    protected JSONArray getExportToOsmApiDbCommandArgs(JSONArray inputCommandArgs, Connection conn)
            throws Exception {
        if (!Boolean.parseBoolean(HootProperties.getProperty("osmApiDbEnabled"))) {
            ResourceErrorHandler.handleError(
                    "Attempted to export to an OSM API database but OSM API database support is disabled",
                    Status.INTERNAL_SERVER_ERROR, log);
        }

        JSONArray commandArgs = new JSONArray();
        commandArgs.addAll(inputCommandArgs);

        if (!getParameterValue("inputtype", commandArgs).equalsIgnoreCase("db")) {
            ResourceErrorHandler.handleError(
                    "When exporting to an OSM API database, the input type must be a Hootenanny API database.",
                    Status.BAD_REQUEST, log);
        }

        String translation = getParameterValue("translation", commandArgs);
        if ((StringUtils.trimToNull(translation) != null) && !translation.toUpperCase().equals("NONE")) {
            ResourceErrorHandler.handleError("Custom translation not allowed when exporting to OSM API database.",
                    Status.BAD_REQUEST, log);
        }

        // ignoring outputname, since we're only going to have a single mapedit
        // connection configured in the core for now
        // configured in the core for now
        JSONObject arg = new JSONObject();
        arg.put("temppath", HootProperties.getProperty("tempOutputPath"));
        commandArgs.add(arg);

        // This option allows the job executor return std out to the client.  This is the only way
        // I've found to get the conflation summary text back from hoot command line to the UI.
        arg = new JSONObject();
        arg.put("writeStdOutToStatusDetail", "true");
        commandArgs.add(arg);

        Map conflatedMap = getConflatedMap(commandArgs, conn);
        //pass the export timestamp to the export bash script
        addMapForExportTag(conflatedMap, commandArgs, conn);
        //pass the export aoi to the export bash script
        setAoi(conflatedMap, commandArgs);

        return commandArgs;
    }

    private Map getConflatedMap(JSONArray commandArgs, Connection conn) throws Exception {
        String mapName = getParameterValue("input", commandArgs);
        List<Long> mapIds = getMapIdsByName(mapName, conn);

        // we don't expect the services to try to export a map that has multiple
        // name entries, but check for it anyway
        if (mapIds.size() > 1) {
            ResourceErrorHandler.handleError("Error exporting data.  Multiple maps with name: " + mapName,
                    Status.BAD_REQUEST, log);
        }
        // this may be checked somewhere else down the line...not sure
        else if (mapIds.isEmpty()) {
            ResourceErrorHandler.handleError("Error exporting data.  No map exists with name: " + mapName,
                    Status.BAD_REQUEST, log);
        }

        Map conflatedMap = new Map(mapIds.get(0), conn);
        conflatedMap.setDisplayName(mapName);
        return conflatedMap;
    }

    // adding this to satisfy the mock
    protected List<Long> getMapIdsByName(String conflatedMapName, Connection conn) throws Exception {
        return DbUtils.getMapIdsByName(conn, conflatedMapName);
    }

    // adding this to satisfy the mock
    protected java.util.Map<String, String> getMapTags(long mapId, Connection conn) throws Exception {
        return DbUtils.getMapsTableTags(mapId, conn);
    }

    // adding this to satisfy the mock
    protected BoundingBox getMapBounds(Map map) throws Exception {
        return map.getBounds();
    }

    private void addMapForExportTag(Map map, JSONArray commandArgs, Connection conn) throws Exception {
        java.util.Map<String, String> tags = getMapTags(map.getId(), conn);
        if (!tags.containsKey("osm_api_db_export_time")) {
            ResourceErrorHandler.handleError("Error exporting data.  Map with ID: " + map.getId()
                    + " and name: " + map.getDisplayName() + " has no osm_api_db_export_time tag.", Status.CONFLICT,
                    log);
        }
        JSONObject arg = new JSONObject();
        arg.put("changesetsourcedatatimestamp", tags.get("osm_api_db_export_time"));
        commandArgs.add(arg);
    }

    private void setAoi(Map conflatedMap, JSONArray commandArgs) throws Exception {
        BoundingBox bounds = getMapBounds(conflatedMap);
        JSONObject arg = new JSONObject();
        arg.put("changesetaoi", bounds.getMinLon() + "," + bounds.getMinLat() + "," + bounds.getMaxLon() + "," + bounds.getMaxLat());
        commandArgs.add(arg);
    }

    /**
     * To retrieve the output from job make Get request.
     *
     * GET hoot-services/job/export/[job id from export job]?outputname=[user
     * defined name]&removecache=[true | false]
     *
     * @param id
     *            ?
     * @param outputname
     *            parameter overrides the output file name with the user defined
     *            name. If not specified then defaults to job id as name.
     * @param remove
     *            parameter controls if the output file from export job should
     *            be delete when Get request completes.
     * @return Octet stream
     */
    @GET
    @Path("/{id}")
    @Produces(MediaType.APPLICATION_OCTET_STREAM)
    public Response exportFile(@PathParam("id") String id,
                               @QueryParam("outputname") String outputname,
                               @QueryParam("removecache") String remove) {
        File out = null;
        try {
            File folder = hoot.services.utils.FileUtils.getSubFolderFromFolder(tempOutputPath, id);

            if (folder != null) {
                String workingFolder = tempOutputPath + "/" + id;

                if (remove.equalsIgnoreCase("true")) {
                    delPath = workingFolder;
                }

                out = hoot.services.utils.FileUtils.getFileFromFolder(workingFolder, outputname, "zip");

                if ((out == null) || !out.exists()) {
                    throw new NativeInterfaceException("Missing output file",
                            NativeInterfaceException.HttpCode.SERVER_ERROR);
                }
            }
        }
        catch (NativeInterfaceException ne) {
            int nStat = ne.getExceptionCode().toInt();
            return Response.status(nStat).entity(ne.getMessage()).build();
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error exporting data: " + ex, Status.INTERNAL_SERVER_ERROR, log);
        }

        String outFileName = id;
        if ((outputname != null) && (!outputname.isEmpty())) {
            outFileName = outputname;
        }

        ResponseBuilder rBuild = Response.ok(out, MediaType.APPLICATION_OCTET_STREAM);
        rBuild.header("Content-Disposition", "attachment; filename=" + outFileName + ".zip");

        return rBuild.build();
    }

    /**
     * Removes specified WFS resource.
     *
     * GET
     * hoot-services/job/export/wfs/remove/ex_eed379c0b9f7469d80ab32c71550883b
     *
     * //TODO: should be an HTTP DELETE
     *
     * @param id
     *            id of the wfs resource to remove
     * @return Removed id
     */
    @GET
    @Path("/wfs/remove/{id}")
    @Produces(MediaType.TEXT_PLAIN)
    public Response removeWfsResource(@PathParam("id") String id) {
        JSONObject ret = new JSONObject();
        try {
            WfsManager wfsMan = new WfsManager();
            wfsMan.removeWfsResource(id);

            DataDefinitionManager dbMan = new DataDefinitionManager();
            List<String> tbls = dbMan.getTablesList(wfsStoreDb, id);
            dbMan.deleteTables(tbls, wfsStoreDb);
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error removing WFS resource: " + ex, Status.INTERNAL_SERVER_ERROR, log);
        }

        ret.put("id", id);
        return Response.ok(ret.toString(), MediaType.TEXT_PLAIN).build();
    }

    /**
     * Lists all wfs resources.
     *
     * GET hoot-services/job/export/wfs/resources
     *
     * @return List of wfs resources
     */
    @GET
    @Path("/wfs/resources")
    @Produces(MediaType.TEXT_PLAIN)
    public Response getWfsResources() {
        JSONArray srvList = new JSONArray();
        try {
            List<String> list = WfsManager.getAllWfsServices();

            for (String wfsResource : list) {
                JSONObject o = new JSONObject();
                o.put("id", wfsResource);
                srvList.add(o);
            }
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error retrieving WFS resource list: " + ex, Status.INTERNAL_SERVER_ERROR, log);
        }

        return Response.ok(srvList.toString(), MediaType.TEXT_PLAIN).build();
    }

    /**
     * Based on the existence of translation script extension, it will send the
     * list of available translations script for export.
     *
     * GET hoot-services/job/export/resources
     *
     * @return List of translation script resources
     */
    @GET
    @Path("/resources")
    @Produces(MediaType.TEXT_PLAIN)
    public Response getExportResources() {
        String transExtPath = homeFolder + "/" + "/plugins-local/script/utp";
        if ((translationExtPath != null) && (!translationExtPath.isEmpty())) {
            transExtPath = translationExtPath;
        }

        JSONArray srvList = new JSONArray();
        try {
            JSONObject o = new JSONObject();
            o.put("name", "TDS");
            o.put("description", "LTDS 4.0");
            srvList.add(o);

            o = new JSONObject();
            o.put("name", "MGCP");
            o.put("description", "MGCP");
            srvList.add(o);

            File f = new File(transExtPath);
            if (f.exists() && f.isDirectory()) {
                o = new JSONObject();
                o.put("name", "UTP");
                o.put("description", "UTP");
                srvList.add(o);
            }
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error retrieving exported resource list: " + ex, Status.INTERNAL_SERVER_ERROR, log);
        }

        return Response.ok(srvList.toString(), MediaType.APPLICATION_JSON).build();
    }
}