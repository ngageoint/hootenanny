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
    @Consumes(MediaType.MULTIPART_FORM_DATA)
    @Produces(MediaType.APPLICATION_JSON)
    public Response grailByBox(@QueryParam("BBOX") String bbox,
                          @QueryParam("USER_NAME") String userName,
                          @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {

        String jobId = "grail_" + UUID.randomUUID().toString().replace("-", "");

        logger.info("##### Starting command: " + jobId);
        logger.info("BBOX: " + bbox);
        logger.info("USER_NAME: " + userName);

        try {
            // This is where downloadable files will be stored and other intermediate artifacts.
            File workDir = new File(TEMP_OUTPUT_PATH, jobId);
            FileUtils.forceMkdir(workDir);

            String tmpUrl = RAILSPORT_PULL_URL;

            File localOutputFile = new File(workDir,"local.osm");
            ExternalCommand getLocalOSM = OSMDataCommandFactory.build(jobId, bbox,tmpUrl,localOutputFile, this.getClass());

            Command[] workflow = { getLocalOSM };

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
}