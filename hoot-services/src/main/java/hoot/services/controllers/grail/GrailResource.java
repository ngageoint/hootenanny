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
import hoot.services.job.Job;
import hoot.services.job.JobProcessor;
import hoot.services.utils.DbUtils;
import hoot.services.utils.XmlDocumentBuilder;


@Controller
@Path("/diff")
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
     * POST hoot-services/grail/bybox?BBOX=left,bottom,right,top&USER_NAME=JSmith&INPUT_NAME=ToyTest
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
                          @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {

        String jobId = "grail_" + UUID.randomUUID().toString().replace("-", "");

        logger.info("##### Starting command: " + jobId);
        logger.info("BBOX: " + bbox);
        logger.info("USER_NAME: " + userName);

        try {
            // This is where downloadable files will be stored and other intermediate artifacts.
            File workDir = new File(TEMP_OUTPUT_PATH, jobId);
            FileUtils.forceMkdir(workDir);

            //String tmpUrl = RAILSPORT_PULL_URL;

            File localOSMFile = new File(workDir,"local.osm");
            ExternalCommand getLocalOSM = OSMDataCommandFactory.build(jobId,bbox,RAILSPORT_PULL_URL,localOSMFile, this.getClass());

            File internetOSMFile = new File(workDir,"internet.osm");
            ExternalCommand getInternetOSM = OSMDataCommandFactory.build(jobId,bbox,MAIN_OSMAPI_URL,internetOSMFile, this.getClass());

            File diffFile = new File(workDir,"diff.osc");
            ExternalCommand makeDiff = runDiffCommandFactory.build(jobId,localOSMFile,internetOSMFile,diffFile,debugLevel, this.getClass());

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

    // Taken from ExportResource.java
    /**
     * To retrieve the output from job make GET request.
     *
     * GET hoot-services/job/export/[job id from export job]?outputname=[user defined name]&ext=[file extension override from zip]
     *
     * @param jobId
     *            job id
     * @param outputname
     *            parameter overrides the output file name with the user defined
     *            name. If not specified then defaults to job id as name.
     * @param ext
     *            parameter overrides the file extension of the file being
     *            downloaded
     *
     * @return Octet stream
     */
    @GET
    @Path("/{id}")
    @Produces(MediaType.APPLICATION_OCTET_STREAM)
    public Response exportFile(@PathParam("id") String jobId,
                               @QueryParam("outputname") String outputname,
                               @QueryParam("ext") String ext) {
        Response response;

        try {
            String fileExt = StringUtils.isEmpty(ext) ? "osc" : ext;
            File exportFile = getExportFile(jobId, outputname, fileExt);

            String outFileName = jobId;
            if (! StringUtils.isBlank(outputname)) {
                outFileName = outputname;
            }

            ResponseBuilder responseBuilder = Response.ok(exportFile);
            responseBuilder.header("Content-Disposition", "attachment; filename=" + outFileName + "." + fileExt);

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

    // Taken from ExportResource.java
    /**
     * Returns the contents of an XML job output file
     *
     * GET hoot-services/job/export/xml/[job id from exportjob]?ext=[file extension override from xml]
     *
     * @param jobId
     *            job id
     * @param ext
     *            parameter overrides the file extension of the file being downloaded
     * @return job output XML
     *
     * @throws WebApplicationException
     *             if the job with ID = id does not exist, the referenced job
     *             output file no longer exists, or the changeset is made up of
     *             multiple changeset files.
     */
    @GET
    @Path("/xml/{id}")
    @Produces(MediaType.TEXT_XML)
    public Response getXmlOutput(@PathParam("id") String jobId,
                                 @QueryParam("ext") String ext) {
        Response response;

        try {
            File out = getExportFile(jobId, jobId, StringUtils.isEmpty(ext) ? "xml" : ext);
            response = Response.ok(new DOMSource(XmlDocumentBuilder.parse(FileUtils.readFileToString(out, "UTF-8")))).build();
        }
        catch (WebApplicationException e) {
            throw e;
        }
        catch (Exception e) {
            throw new WebApplicationException(e);
        }

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

}