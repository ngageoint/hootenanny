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

import static hoot.services.HootProperties.*;

import java.io.File;
import java.io.IOException;
import java.util.UUID;

import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.xml.transform.dom.DOMSource;

import org.apache.commons.io.FileUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.w3c.dom.Document;

import hoot.services.models.db.JobStatus;
import hoot.services.utils.XmlDocumentBuilder;


/**
 * Derives an OSM XML changeset (.osc)
 * 
 * Keeping this logic separate from that of ChangesetResource since it is not part of the official 
 * OSM API and ChangesetResource is made up of pure OSM API functions.
 */
@Controller
@Path("/changeset")
public class DeriveChangesetResource extends JobControllerBase {
    @SuppressWarnings("unused")
    private static final Logger logger = LoggerFactory.getLogger(DeriveChangesetResource.class);

    public DeriveChangesetResource() {
        super(DERIVE_CHANGESET_SCRIPT);
    }

    /**
     * Derives the difference between two OSM datasets, where the first dataset is a source and the 
     * other a target.  The difference is derived in the form of an OSM XML changeset.
     * 
     * @param input1 database layer name of the first input dataset
     * @param input2 database layer name of the second input dataset
     * @param aoi bounding box used to filter the changeset derivation
     * @return job ID of the changeset derivation job
     */
    @POST
    @Path("/derive")
    @Produces(MediaType.APPLICATION_JSON)
    public JobId derive(@QueryParam("input1") String input1, @QueryParam("input2") String input2, @QueryParam("aoi") String aoi) {
        
        String jobId = UUID.randomUUID().toString();
        jobId = "ex_" + jobId.replace("-", "");

        try {
            JSONArray commandArgs = new JSONArray();
            JSONObject arg = new JSONObject();
            
            //assuming that this would only be used with data layers already being loaded into the
            //database
            arg.put("input1", DB_URL + "/" + input1);
            arg.put("input2", DB_URL + "/" + input2);
            arg.put("aoi", aoi);
            //write the changeset to a temp file which can be retrieved through this class
            File tempOutputDir = new File(TEMP_OUTPUT_PATH);
            if (!tempOutputDir.exists())
            {
              tempOutputDir.mkdir();   
            }
            //don't auto-delete this temp file, since we want it to be available for future access;
            //users will be responsible for manually clearing this out of the hoot temp folder
            //services always write changeset with xml; no use case for the sql right now
            arg.put("output", File.createTempFile("changeset-", ".osc", tempOutputDir).getAbsolutePath());
            arg.put("jobId", jobId);
            commandArgs.add(arg);
            
            postJobRequest(jobId, createPostBody(commandArgs));
        }
        catch (Exception e) {
            throw new WebApplicationException(e, Response.serverError().entity("Error deriving changeset for inputs: " + input1 + ", " + input2).build());
        }

        return new JobId(jobId);
    }
    
    public String getJobStatusDetail(String jobId) throws Exception
    {
        JobStatus jobStatus = (new JobStatusManager()).getJobStatusObj(jobId);
        if (jobStatus == null)
        {
            throw new IOException("Job with ID: " + jobId + " does not exist.");
        }
        return jobStatus.getStatusDetail(); 
    }

    /**
     * Returns the contents of an XML changeset file (.osc)
     * 
     * @param jobId job ID the changeset file is associated with
     * @return changeset XML contents
     * @throws WebApplicationException if the job with ID = jobID does not exist, the referenced
     * temp changeset file no longer exists, or the changeset is made up of multiple changeset 
     * files.
     */
    @GET
    @Path("/getderived")
    @Produces(MediaType.TEXT_XML)
    public Response get(@PathParam("jobId") String jobId) {
        
        Document responseDoc = null;
        String changesetFileName = null;
        try {
            changesetFileName = getJobStatusDetail(jobId);
            if (changesetFileName.equals("<multiple files>"))
            {
                throw new Exception("Changeset requested is made up of multiple changesets in multiple files.  /getderived does not currently support this.");
            }
            responseDoc = XmlDocumentBuilder.parse(FileUtils.readFileToString(new File(changesetFileName), "UTF-8"));
        }
        catch (IOException e) {
            throw new WebApplicationException(Response.status(Response.Status.NOT_FOUND).entity("Error fetching changeset contents for job ID=" + jobId + ".  Unable to read changeset temp file at " + changesetFileName + ".").build());
        }
        catch (Exception e) {
            throw new WebApplicationException(e, Response.serverError().entity("Error fetching changeset contents for job ID=" + jobId + " " + e.getMessage()).build());
        }
        return Response.ok(new DOMSource(responseDoc)).build();
    }
}
