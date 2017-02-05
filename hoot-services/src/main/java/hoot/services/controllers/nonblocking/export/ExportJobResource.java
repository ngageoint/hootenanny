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
package hoot.services.controllers.nonblocking.export;

import static hoot.services.HootProperties.*;

import java.io.File;
import java.util.List;
import java.util.UUID;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.ResponseBuilder;
import javax.ws.rs.core.Response.Status;

import org.apache.commons.lang3.StringUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.command.Command;
import hoot.services.command.ExternalCommand;
import hoot.services.job.ChainJob;
import hoot.services.job.Job;
import hoot.services.controllers.nonblocking.AsynchronousJobResource;
import hoot.services.controllers.nonblocking.JobId;
import hoot.services.geo.BoundingBox;
import hoot.services.models.osm.Map;
import hoot.services.utils.DbUtils;
import hoot.services.utils.FileUtils;
import hoot.services.wfs.WFSManager;


@Controller
@Path("/export")
@Transactional
public class ExportJobResource extends AsynchronousJobResource {
    private static final Logger logger = LoggerFactory.getLogger(ExportJobResource.class);

    public ExportJobResource() {
        super(EXPORT_SCRIPT);
    }


    /**
     * Asynchronous export service.
     *
     * POST hoot-services/job/export/execute
     *
     * {
     *   "translation":"MGCP.js", //Translation script name.
     *   "inputtype":"db"         //[db | file] db means input from hoot db will be used. file mean a file path will be specified.
     *   "input":"ToyTestA",      //  Input name. for inputtype = db then specify name from hoot db. For inputtype=file, specify full path
     *                            //  to a file. "outputtype":"gdb", //[gdb | shp | wfs | osm_api_db]. gdb will
     *                            //  produce file gdb, shp will output shapefile. if outputtype = wfs then a
     *                            //  wfs front end will be created. osm_api_db will derive and apply a changeset to an
     *                            //  OSM API database "removereview" : "false" //?
     * }
     *
     * @param params
     * @return Job ID
     */
    @POST
    @Path("/execute")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public JobId process(String params) {
        String jobId = "ex_" + UUID.randomUUID().toString().replace("-", "");

        try {
            JSONArray commandArgs = AsynchronousJobResource.parseParams(params);
            JSONParser pars = new JSONParser();
            JSONObject oParams = (JSONObject) pars.parse(params);

            JSONObject arg = new JSONObject();
            arg.put("outputfolder", TEMP_OUTPUT_PATH + "/" + jobId);
            commandArgs.add(arg);

            arg = new JSONObject();
            arg.put("output", jobId);
            commandArgs.add(arg);

            JSONObject hootDBURL = new JSONObject();
            hootDBURL.put("DB_URL", HOOT_APIDB_URL);
            commandArgs.add(hootDBURL);

            JSONObject osmAPIDBURL = new JSONObject();
            osmAPIDBURL.put("OSM_API_DB_URL", OSM_APIDB_URL);
            commandArgs.add(osmAPIDBURL);

            String type = getParameterValue("outputtype", commandArgs);
            if ("wfs".equalsIgnoreCase(type)) {
                arg = new JSONObject();
                arg.put("outputname", jobId);
                commandArgs.add(arg);

                String pgUrl = "host='" + DB_HOST + "' port='" + DB_PORT + "' user='" + DB_USER_ID
                         + "' password='" + DB_PASSWORD + "' dbname='" + WFS_STORE_DB + "'";

                arg = new JSONObject();
                arg.put("PG_URL", pgUrl);
                commandArgs.add(arg);

                Command[] commands = {
                        () -> {
                            ExternalCommand osm2orgCommand = super.createMakeScriptJobReq(commandArgs);
                            return externalCommandManager.exec(jobId, osm2orgCommand);
                        },
                        () -> {
                            try {
                                return WFSManager.createWfsResource(jobId, jobId);
                            }
                            catch (Exception e) {
                                throw new RuntimeException("Error creating WFS resource!", e);
                            }
                        }
                };

                ChainJob chainJob = new ChainJob();
                chainJob.setJobId(jobId);
                chainJob.setCommands(commands);

                super.processChainJob(chainJob);
            }
            else if ("osm_api_db".equalsIgnoreCase(type)) {
                JSONArray args = getExportToOsmApiDbCommandArgs(commandArgs, oParams);

                Command command = () -> {
                    ExternalCommand exportToOSMCommand = super.createMakeScriptJobReq(args);
                    return externalCommandManager.exec(jobId, exportToOSMCommand);
                };

                Job job = new Job();
                job.setJobId(jobId);
                job.setCommand(command);

                super.processJob(job);
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
                    arg = new JSONObject();
                    arg.put("outputname", jobId);
                    commandArgs.add(arg);
                }

                Command command = () -> {
                    ExternalCommand exportCommand = super.createMakeScriptJobReq(commandArgs);
                    return externalCommandManager.exec(jobId, exportCommand);
                };

                Job job = new Job();
                job.setJobId(jobId);
                job.setCommand(command);

                super.processJob(job);
            }
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception e) {
            String msg = "Error exporting data! params = " + params;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return new JobId(jobId);
    }

    JSONArray getExportToOsmApiDbCommandArgs(JSONArray inputCommandArgs, JSONObject oParams) {
        if (!Boolean.parseBoolean(OSM_API_DB_ENABLED)) {
            String msg = "Attempted to export to an OSM API database but OSM API database support is disabled";
            throw new WebApplicationException(Response.serverError().entity(msg).build());
        }

        JSONArray commandArgs = new JSONArray();
        commandArgs.addAll(inputCommandArgs);

        if (!"db".equalsIgnoreCase(getParameterValue("inputtype", commandArgs))) {
            String msg = "When exporting to an OSM API database, the input type must be a Hootenanny API database.";
            throw new WebApplicationException(Response.status(Status.BAD_REQUEST).entity(msg).build());
        }

        String translation = getParameterValue("translation", commandArgs);
        if ((StringUtils.trimToNull(translation) != null) && !translation.toUpperCase().equals("NONE")) {
            String msg = "Custom translation not allowed when exporting to OSM API database.";
            throw new WebApplicationException(Response.status(Status.BAD_REQUEST).entity(msg).build());
        }

        // ignoring outputname, since we're only going to have a single mapedit
        // connection configured in the core for now configured in the core for now
        JSONObject arg = new JSONObject();
        arg.put("temppath", TEMP_OUTPUT_PATH);
        commandArgs.add(arg);

        // This option allows the job executor return std out to the client.  This is the only way
        // I've found to get the conflation summary text back from hoot command line to the UI.
        arg = new JSONObject();
        arg.put("writeStdOutToStatusDetail", "true");
        commandArgs.add(arg);

        Map conflatedMap = getConflatedMap(commandArgs);

        //pass the export timestamp to the export bash script
        addMapForExportTag(conflatedMap, commandArgs);

        //pass the export aoi to the export bash script
        //if sent a bbox in the url (reflecting task grid bounds)
        //use that, otherwise use the bounds of the conflated output
        BoundingBox bbox;
        if (oParams.get("TASK_BBOX") != null) {
            bbox = new BoundingBox(oParams.get("TASK_BBOX").toString());
        }
        else {
            bbox = getMapBounds(conflatedMap);
        }

        setAoi(bbox, commandArgs);

        //put the osm userid in the command args
        if (oParams.get("USER_ID") != null) {
            JSONObject uid = new JSONObject();
            uid.put("userid", oParams.get("USER_ID"));
            commandArgs.add(uid);
        }

        return commandArgs;
    }

    private Map getConflatedMap(JSONArray commandArgs) {
        String mapName = getParameterValue("input", commandArgs);
        Long mapId = getMapIdByName(mapName);

        // this may be checked somewhere else down the line...not sure
        if (mapId == null) {
            String msg = "Error exporting data.  No map exists with name: " + mapName;
            throw new WebApplicationException(Response.status(Status.BAD_REQUEST).entity(msg).build());
        }

        Map conflatedMap = new Map(mapId);
        conflatedMap.setDisplayName(mapName);

        return conflatedMap;
    }

    // adding this to satisfy the mock
    Long getMapIdByName(String conflatedMapName) {
        return DbUtils.getMapIdByName(conflatedMapName);
    }

    // adding this to satisfy the mock
    java.util.Map<String, String> getMapTags(long mapId) {
        return DbUtils.getMapsTableTags(mapId);
    }

    // adding this to satisfy the mock
    BoundingBox getMapBounds(Map map) {
        return map.getBounds();
    }

    private void addMapForExportTag(Map map, JSONArray commandArgs) {
        java.util.Map<String, String> tags = getMapTags(map.getId());

        if (!tags.containsKey("osm_api_db_export_time")) {
            String msg = "Error exporting data.  Map with ID: " + map.getId()
                    + " and name: " + map.getDisplayName() + " has no osm_api_db_export_time tag.";
            throw new WebApplicationException(Response.status(Status.CONFLICT).entity(msg).build());
        }

        JSONObject arg = new JSONObject();
        arg.put("changesetsourcedatatimestamp", tags.get("osm_api_db_export_time"));
        commandArgs.add(arg);
    }

    private static void setAoi(BoundingBox bounds, JSONArray commandArgs) {
        JSONObject arg = new JSONObject();
        arg.put("changesetaoi", bounds.getMinLon() + "," + bounds.getMinLat() + "," +
                bounds.getMaxLon() + "," + bounds.getMaxLat());
        commandArgs.add(arg);
    }

    /**
     * To retrieve the output from job make Get request.
     *
     * GET hoot-services/job/export/[job id from export job]?outputname=[user
     * defined name]&removecache=[true | false]&ext=[file extension override from zip]
     *
     * @param id
     *            ?
     * @param outputname
     *            parameter overrides the output file name with the user defined
     *            name. If not specified then defaults to job id as name.
     * @param remove
     *            parameter controls if the output file from export job should
     *            be delete when Get request completes.
     * @param ext
     *            parameter overrides the file extension of the file being downloaded
     * @return Octet stream
     */
    @GET
    @Path("/{id}")
    @Produces(MediaType.APPLICATION_OCTET_STREAM)
    public Response exportFile(@PathParam("id") String id,
                               @QueryParam("outputname") String outputname,
                               @QueryParam("removecache") String remove,
                               @QueryParam("ext") String ext) {
        String fileExt = StringUtils.isEmpty(ext) ? "zip" : ext;
        File folder = FileUtils.getSubFolderFromFolder(TEMP_OUTPUT_PATH, id);

        File out = null;
        if (folder != null) {
            String workingFolder = TEMP_OUTPUT_PATH + "/" + id;
            out = FileUtils.getFileFromFolder(workingFolder, outputname, fileExt);

            if ((out == null) || !out.exists()) {
                throw new IllegalStateException("Missing output file: " + outputname + " with extension " + fileExt);
            }
        }

        String outFileName = id;
        if ((outputname != null) && (!outputname.isEmpty())) {
            outFileName = outputname;
        }

        ResponseBuilder responseBuilder = Response.ok(out);
        responseBuilder.header("Content-Disposition", "attachment; filename=" + outFileName + "." + fileExt);

        return responseBuilder.build();
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
    @Produces(MediaType.APPLICATION_JSON)
    public Response removeWfsResource(@PathParam("id") String id) {

        try {
            WFSManager.removeWfsResource(id);
            List<String> tables = DbUtils.getTablesList(id);
            DbUtils.deleteTables(tables);
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception e) {
            String msg = "Error removing WFS resource with id = " + id;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        JSONObject resource = new JSONObject();
        resource.put("id", id);

        return Response.ok(resource.toJSONString()).build();
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
    @Produces(MediaType.APPLICATION_JSON)
    public Response getWfsResources() {
        JSONArray wfsResources = new JSONArray();
        try {
            List<String> list = WFSManager.getAllWfsServices();

            if (list != null) {
                for (String wfsResource : list) {
                    JSONObject resource = new JSONObject();
                    resource.put("id", wfsResource);
                    wfsResources.add(resource);
                }
            }
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception ex) {
            String msg = "Error retrieving WFS resource list!";
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        return Response.ok(wfsResources.toJSONString()).build();
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
    @Produces(MediaType.APPLICATION_JSON)
    public Response getExportResources() {
        String transExtPath = HOME_FOLDER + "/plugins-local/script/utp";
        if (!StringUtils.isBlank(TRANSLATION_EXT_PATH)) {
            transExtPath = TRANSLATION_EXT_PATH;
        }

        JSONArray exportResources = new JSONArray();
        try {
            JSONObject resource = new JSONObject();
            resource.put("name", "TDS");
            resource.put("description", "LTDS 4.0");
            exportResources.add(resource);

            resource = new JSONObject();
            resource.put("name", "MGCP");
            resource.put("description", "MGCP");
            exportResources.add(resource);

            File file = new File(transExtPath);
            if (file.exists() && file.isDirectory()) {
                resource = new JSONObject();
                resource.put("name", "UTP");
                resource.put("description", "UTP");
                exportResources.add(resource);
            }
        }
        catch (Exception e) {
            String msg = "Error retrieving export resources list!";
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return Response.ok(exportResources.toJSONString()).build();
    }
}
