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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.grail;

import static hoot.services.HootProperties.*;

import java.io.File;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.UUID;

import javax.ws.rs.Consumes;
import javax.ws.rs.DefaultValue;
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
import javax.xml.transform.dom.DOMSource;

import org.apache.commons.io.FileUtils;
import org.apache.commons.lang3.StringUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.command.Command;
import hoot.services.command.ExternalCommand;
import hoot.services.command.common.ZIPDirectoryContentsCommand;
import hoot.services.command.common.ZIPFileCommand;
import hoot.services.controllers.osm.map.MapResource;
import hoot.services.geo.BoundingBox;
import hoot.services.job.Job;
import hoot.services.job.JobProcessor;
import hoot.services.utils.DbUtils;
import hoot.services.utils.XmlDocumentBuilder;


@Controller
@Path("")
@Transactional
public class GrailResource {
    private static final Logger logger = LoggerFactory.getLogger(GrailResource.class);

    @Autowired
    private JobProcessor jobProcessor;

    @Autowired
    private PullOSMDataCommandFactory OSMDataCommandFactory;

    @Autowired
    private RunDiffCommandFactory runDiffCommandFactory;

    @Autowired
    private ApplyChangesetCommandFactory applyChangesetCommandFactory;

    /**
     * The purpose of this service is to take a bounding box and then:
     * Pull the native OSM data for the area
     * Pull the same area from a specified DB
     * Run Differential Conflation on the two
     * Push the result to the specified DB
     * Return anything that cannot be pushed to the user as an osc file.
     *
     * POST hoot-services/grail/bybox?BBOX=left,bottom,right,top&USER_NAME=JSmith
     *
     * @param BBOX
     *        left is the longitude of the left (west) side of the bounding box  
     *        bottom is the latitude of the bottom (south) side of the bounding box  
     *        right is the longitude of the right (east) side of the bounding box  
     *        top is the latitude of the top (north) side of the bounding box  
     *
     * @param USER_NAME
     *        left is the longitude of the left (west) side of the bounding box  

     * @return Job ID
     */
    @POST
    @Path("/bybox")
    //@Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response grailByBox(@QueryParam("bbox") String bbox,
                          @QueryParam("user_name") String userName,
                          @QueryParam("tag_diff") @DefaultValue("false") Boolean tagDiff,
                          @QueryParam("split_output") @DefaultValue("false") Boolean splitOutput,
                          @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {

        // String jobId = "grail_" + UUID.randomUUID().toString().replace("-", "");
        String jobId = "grail_01";

        BoundingBox boundingBox = new BoundingBox(bbox);
        double bboxArea = boundingBox.getArea();

        // TODO:  Pull <area maximum="0.25"> from the capabilities of the OSM API DB's
        // Also, pull the server status as well. Throw errors if they are not available.
        double maxBboxArea = 0.25;

        if (bboxArea > maxBboxArea) {
            String msg = "The bounding box area (" + bboxArea + ") is too large. It must be less than " + maxBboxArea + " degrees";
            throw new WebApplicationException(Response.status(Response.Status.BAD_REQUEST).entity(msg).build());
        }

        try {
            File workDir = new File(TEMP_OUTPUT_PATH, jobId);
            FileUtils.forceMkdir(workDir);

            // Pull OSM data from the local OSM API Db
            File localOSMFile = new File(workDir,"local.osm");
            ExternalCommand getLocalOSM = OSMDataCommandFactory.build(jobId,boundingBox,RAILSPORT_PULL_URL,localOSMFile, this.getClass());

            // Pull OSM data from the real, internet, OSM API Db
            File internetOSMFile = new File(workDir,"internet.osm");
            ExternalCommand getInternetOSM = OSMDataCommandFactory.build(jobId,boundingBox,MAIN_OSMAPI_URL,internetOSMFile, this.getClass());

            // Run the diff command.
            // TODO: We could possibly use ConflateCommand.java for this. If we setup the params for it 
            File diffFile = new File(workDir,"diff.osc");
            ExternalCommand makeDiff = runDiffCommandFactory.build(jobId,localOSMFile,internetOSMFile,diffFile,debugLevel, this.getClass());

            // Push to the local OSM API Db
            // TODO: The export/ApplyChangesetCommand.java command is hardcoded to push to OSMAPI_DB_URL. We could refactor it to 
            // take the DB URL as a parameter. 
            ExternalCommand applyChange = applyChangesetCommandFactory.build(jobId,diffFile,RAILSPORT_PUSH_URL,userName, debugLevel, this.getClass());

            Command[] workflow = { getLocalOSM, getInternetOSM, makeDiff, applyChange };

            jobProcessor.submitAsync(new Job(jobId, workflow));
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (IllegalArgumentException iae) {
            throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
        }
        catch (Exception e) {
            String msg = "Error during grail! ";
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        JSONObject json = new JSONObject();
        json.put("jobid", jobId);

        return Response.ok(json.toJSONString()).build();
    }


    // Taken from ExportResource.java and modified
    /**
     * To retrieve the diff file from job make a GET request.
     *
     * GET hoot-services/grail/diff/[job id from export job]
     *
     * @param jobId
     *            job id
     *
     * @return Octet stream
     */
    @GET
    @Path("diff/{id}")
    @Produces(MediaType.APPLICATION_OCTET_STREAM)
    public Response exportDiffFile(@PathParam("id") String jobId) {
        Response response;

        try {
            File exportFile = getExportFile(jobId, "diff", "osc");

            ResponseBuilder responseBuilder = Response.ok(exportFile);
            responseBuilder.header("Content-Disposition", "attachment; filename=diff.osc");

            response = responseBuilder.build();
        }
        catch (WebApplicationException e) {
            throw e;
        }
        catch (Exception e) {
            throw new WebApplicationException(e);
        }

        return response;
    }

    // Taken from ExportResource.java and modified
    /**
     * To retrieve the error file from job make a GET request.
     *
     * GET hoot-services/grail/error/[job id from export job]
     *
     * @param jobId
     *            job id
     *
     * @return Octet stream
     */
    @GET
    @Path("error/{id}")
    @Produces(MediaType.APPLICATION_OCTET_STREAM)
    public Response exportErrorFile(@PathParam("id") String jobId) {
        Response response;

        try {
            // NOTE: This will be changed so that it grabs the error.osc file after we figure out
            // what it is called.
            File exportFile = getExportFile(jobId, "diff", "osc");

            ResponseBuilder responseBuilder = Response.ok(exportFile);
            responseBuilder.header("Content-Disposition", "attachment; filename=diff.osc");

            response = responseBuilder.build();
        }
        catch (WebApplicationException e) {
            throw e;
        }
        catch (Exception e) {
            throw new WebApplicationException(e);
        }

        return response;
    }

    // Test the BoundingBox methods. This will be deleted soon
    @GET
    @Path("boxtest")
    @Produces(MediaType.APPLICATION_JSON)
    public Response boxTest(@QueryParam("bbox") String bbox)  {
        Response response;

        logger.info("##### Starting boxtest command");
        logger.info("BBOX: " + bbox);

        BoundingBox boundingBox = new BoundingBox(bbox);

        double bboxArea = boundingBox.getArea();
        String bboxSvc = boundingBox.toServicesString();
        String bboxString = boundingBox.toString();
        double height = boundingBox.getHeight();
        double width = boundingBox.getWidth();

        Boolean isVaild = bboxArea < 0.25;

        double maxBboxArea = 0.25;

        if (bboxArea > maxBboxArea) {
            String msg = "The bounding box area (" + bboxArea + ") is too large. It must be less than " + maxBboxArea + " degrees";
            throw new WebApplicationException(Response.status(Response.Status.BAD_REQUEST).entity(msg).build());
        }

        JSONObject json = new JSONObject();

        json.put("bboxSvc", bboxSvc);
        json.put("bboxString", bboxString);
        json.put("Height", height);
        json.put("Width", width);
        json.put("Area", bboxArea);
        json.put("isVaild", isVaild);

        ResponseBuilder responseBuilder = Response.ok(json.toJSONString());
        responseBuilder.header("Test", "File");

        response = responseBuilder.build();

        return response;
    }

    // Taken from ExportResource.java
    private static File getExportFile(String jobId, String outputName, String fileExt) {
        File exportFile = new File(new File(TEMP_OUTPUT_PATH, jobId), outputName + "." + fileExt);

        if (!exportFile.exists()) {
            String errorMsg = "Error exporting data.  Missing output file.";
            throw new WebApplicationException(Response.status(Status.NOT_FOUND).entity(errorMsg).build());
        }

        return exportFile;
    }

    // Testing: Just runn the diff
    @GET
    @Path("/rundiff/{dir}")
    @Produces(MediaType.APPLICATION_JSON)
    public Response runDiffTest(@PathParam("dir") String jobDir) {
        Response response;

        String jobId = "grail_" + UUID.randomUUID().toString().replace("-", "");
        File workDir = new File(TEMP_OUTPUT_PATH, jobDir);

        try {
            File diffFile = new File(workDir,"diff.osc");
            File localOSMFile = new File(workDir,"local.osm");
            File internetOSMFile = new File(workDir,"internet.osm");
            ExternalCommand makeDiff = runDiffCommandFactory.build(jobId,localOSMFile,internetOSMFile,diffFile,"info", this.getClass());

            Command[] workflow = { makeDiff };

            jobProcessor.submitAsync(new Job(jobId, workflow));
        
            JSONObject json = new JSONObject();
            json.put("jobId", jobId);
            ResponseBuilder responseBuilder = Response.ok(json.toJSONString());
            response = responseBuilder.build();
        }
        catch (WebApplicationException e) {
            throw e;
        }
        catch (Exception e) {
            throw new WebApplicationException(e);
        }

        return response;
    }

    // Testing: Just run the Apply
    @GET
    @Path("/runapply/{dir}")
    @Produces(MediaType.APPLICATION_JSON)
    public Response runApplyTest(@PathParam("dir") String jobDir) {
        Response response;

        String jobId = "grail_" + UUID.randomUUID().toString().replace("-", "");
        File workDir = new File(TEMP_OUTPUT_PATH, jobDir);

        try {
            File diffFile = new File(workDir,"diff.osc");

            ExternalCommand applyChange = applyChangesetCommandFactory.build(jobId,diffFile,RAILSPORT_PUSH_URL,"Papa Smurf", "debug", this.getClass());

            Command[] workflow = { applyChange };

            jobProcessor.submitAsync(new Job(jobId, workflow));
        
            JSONObject json = new JSONObject();
            json.put("jobId", jobId);
            ResponseBuilder responseBuilder = Response.ok(json.toJSONString());
            response = responseBuilder.build();
        }
        catch (WebApplicationException e) {
            throw e;
        }
        catch (Exception e) {
            throw new WebApplicationException(e);
        }

        return response;
    }
}