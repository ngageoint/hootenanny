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
import hoot.services.controllers.nonblocking.AsynchronousJobResource;
import hoot.services.controllers.nonblocking.JobId;
import hoot.services.job.ChainJob;
import hoot.services.job.Job;
import hoot.services.utils.DbUtils;
import hoot.services.utils.FileUtils;
import hoot.services.utils.JsonUtils;
import hoot.services.wfs.WFSManager;


@Controller
@Path("/export")
@Transactional
public class ExportJobResource extends AsynchronousJobResource {
    private static final Logger logger = LoggerFactory.getLogger(ExportJobResource.class);

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
            JSONParser parser = new JSONParser();
            JSONObject oParams = (JSONObject) parser.parse(params);

            String type = JsonUtils.getParameterValue("outputtype", oParams);
            if ("wfs".equalsIgnoreCase(type)) {
                Command[] commands = {
                    () -> {
                        ExportCommand osm2orgCommand = new ExportCommand(jobId, params, this.getClass());
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

                super.processChainJob(new ChainJob(jobId, commands));
            }
            else if ("osm_api_db".equalsIgnoreCase(type)) {
                Command command = () -> {
                    ExternalCommand exportToOSMAPIDBCommand = new ExportCommand(jobId, params, this.getClass());
                    return externalCommandManager.exec(jobId, exportToOSMAPIDBCommand);
                };

                super.processJob(new Job(jobId, command));
            }
            else {
                Command command = () -> {
                    ExportCommand exportCommand = new ExportCommand(jobId, params, this.getClass());
                    return externalCommandManager.exec(jobId, exportCommand);
                };

                super.processJob(new Job(jobId, command));
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
