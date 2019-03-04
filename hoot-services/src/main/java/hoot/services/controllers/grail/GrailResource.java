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
import java.io.IOException;
import java.net.URL;
import java.util.LinkedList;
import java.util.List;
import java.util.UUID;

import javax.servlet.http.HttpServletRequest;
import javax.ws.rs.DefaultValue;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.ResponseBuilder;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import org.apache.commons.io.FileUtils;
import org.apache.commons.lang3.StringUtils;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.command.Command;
import hoot.services.command.ExternalCommand;
import hoot.services.command.InternalCommand;

import hoot.services.job.Job;
import hoot.services.job.JobProcessor;
import hoot.services.models.db.Users;


@Controller
@Path("")
@Transactional
public class GrailResource {
    private static final Logger logger = LoggerFactory.getLogger(GrailResource.class);

    @Autowired
    private JobProcessor jobProcessor;

    @Autowired
    private GrailCommandFactory grailCommandFactory;

    @Autowired
    private PullOverpassCommandFactory overpassCommandFactory;

    @Autowired
    private PullApiCommandFactory apiCommandFactory;

    @Autowired
    private UpdateDbCommandFactory updateDbCommandFactory;

    public GrailResource() {}

    /**
     * The purpose of this service is to take a bounding box and then:
     * Pull the native OSM data for the area
     * Pull the same area from a specified DB
     * Run Differential Conflation on the two
     * Push the result to the specified DB
     * Return anything that cannot be pushed to the user as an osc file.
     *
     * POST hoot-services/grail/everythingbybox?BBOX=left,bottom,right,top&user_name=JSmith&DEBUG_LEVEL=<error,info,debug,verbose,trace>
     *        left is the longitude of the left (west) side of the bounding box
     *        bottom is the latitude of the bottom (south) side of the bounding box
     *        right is the longitude of the right (east) side of the bounding box
     *        top is the latitude of the top (north) side of the bounding box
     *        left is the longitude of the left (west) side of the bounding box
     *
     * @param params
     *
     * @return Job ID
     */
    @POST
    @Path("/everythingbybox")
    @Produces(MediaType.APPLICATION_JSON)
    public Response everythingByBox(@Context HttpServletRequest request,
            @QueryParam("BBOX") String bbox,
            @QueryParam("APPLY_TAGS") @DefaultValue("false") Boolean applyTags,
            @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {

        Users user = Users.fromRequest(request);

        String mainJobId = "grail_" + UUID.randomUUID().toString().replace("-", "");

        JSONObject json = new JSONObject();
        json.put("jobid", mainJobId);

        List<Command> workflow = new LinkedList<>();

        GrailParams params = new GrailParams();
        params.setBounds(bbox);
        params.setUser(user);

        File workDir = new File(TEMP_OUTPUT_PATH, mainJobId);
        try {
            FileUtils.forceMkdir(workDir);
        }
        catch (IOException ioe) {
            logger.error("EverythingByBox: Error creating folder: {} ", workDir.getAbsolutePath(), ioe);
            return Response.status(Response.Status.INTERNAL_SERVER_ERROR).entity(ioe.getMessage()).build();
        }

        // IMPORTANT NOTE: This has NO incremental error checking!
        // It assumes that each step runs cleanly.
        try {
            // This is turned off since we are using Overpass
            // APICapabilities mainOsmApiCapabilities = getCapabilities(MAIN_OSMAPI_CAPABILITIES_URL);
            // logger.info("EverythingByBox: mainOSMAPI status = " + mainOsmApiCapabilities.getApiStatus());
            // if (mainOsmApiCapabilities.getApiStatus().equals("offline") | mainOsmApiCapabilities.getApiStatus() == null) {
            //     return Response.status(Response.Status.SERVICE_UNAVAILABLE).entity("The main OSM API server is offline. Try again later").build();
            // }

            APICapabilities railsPortCapabilities = getCapabilities(RAILSPORT_CAPABILITIES_URL);
            logger.info("EverythingByBox: railsPortAPI status = " + railsPortCapabilities.getApiStatus());
            if (railsPortCapabilities.getApiStatus() == null | railsPortCapabilities.getApiStatus().equals("offline")) {
                return Response.status(Response.Status.SERVICE_UNAVAILABLE).entity("The local OSM API server is offline. Try again later").build();
            }

            // Pull OSM data from the local OSM API Db
            File localOSMFile = new File(workDir, "local.osm");
            if (localOSMFile.exists()) localOSMFile.delete();

            GrailParams apiParams = new GrailParams();
            apiParams.setBounds(bbox);
            apiParams.setMaxBBoxSize(railsPortCapabilities.getMaxArea());
            apiParams.setOutput(localOSMFile.getAbsolutePath());
            apiParams.setPullUrl(RAILSPORT_PULL_URL);

            String jobId = "grail_" + UUID.randomUUID().toString().replace("-", "");
            json.put("jobid:LocalOSM", jobId);
            // ExternalCommand getLocalOSM = grailCommandFactory.build(jobId,params,debugLevel,PullOSMDataCommand.class,this.getClass());
            InternalCommand getLocalOSM = apiCommandFactory.build(jobId, apiParams, this.getClass());
            workflow.add(getLocalOSM);

            // Pull OSM data from the real, internet, OSM Db using overpass
            File internetOSMFile = new File(workDir, "internet.osm");
            if (internetOSMFile.exists()) internetOSMFile.delete();

            GrailParams overpassParams = new GrailParams();
            overpassParams.setBounds(bbox);
            overpassParams.setMaxBBoxSize(railsPortCapabilities.getMaxArea());
            overpassParams.setOutput(internetOSMFile.getAbsolutePath());
            overpassParams.setPullUrl(MAIN_OVERPASS_URL);

            jobId = "grail_" + UUID.randomUUID().toString().replace("-", "");
            json.put("jobid:InternetOSM", jobId);
            InternalCommand getOverpassOSM = overpassCommandFactory.build(jobId, overpassParams, this.getClass());
            workflow.add(getOverpassOSM);

            // Run the diff command.
            // TODO: We could possibly use ConflateCommand.java for this. If we setup the params for it - especially the 35 bazillion Hoot Options
            params.setInput1(localOSMFile.getAbsolutePath());
            params.setInput2(internetOSMFile.getAbsolutePath());

            File geomDiffFile = new File(workDir, "diff.osc");
            if (geomDiffFile.exists()) geomDiffFile.delete();
            params.setOutput(geomDiffFile.getAbsolutePath());

            ExternalCommand makeDiff = grailCommandFactory.build(mainJobId, params, debugLevel, RunDiffCommand.class, this.getClass());
            workflow.add(makeDiff);

            // Push to the local OSM API Db
            // TODO: The export/ApplyChangesetCommand.java command is hardcoded to push to OSMAPI_DB_URL. We could refactor it to
            // take the DB URL as a parameter.
            params.setPushUrl(RAILSPORT_PUSH_URL);
            ExternalCommand applyGeomChange = grailCommandFactory.build(mainJobId, params, debugLevel, ApplyChangesetCommand.class, this.getClass());
            workflow.add(applyGeomChange);

            if (applyTags) {
                File tagDiffFile = new File(workDir, "diff.tags.osc");
                params.setOutput(tagDiffFile.getAbsolutePath());
                ExternalCommand applyTagChange = grailCommandFactory.build(mainJobId, params, debugLevel, ApplyChangesetCommand.class, this.getClass());
                workflow.add(applyTagChange);
            }

            // Now roll the dice and run everything.....
            jobProcessor.submitAsync(new Job(mainJobId, workflow.toArray(new Command[workflow.size()])));
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (IllegalArgumentException iae) {
            throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
        }
        catch (Exception e) {
            String msg = "Error during everything by box! Params: " + params;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return Response.ok(json.toJSONString()).build();
    }

    @POST
    @Path("/everythingtest")
    @Produces(MediaType.APPLICATION_JSON)
    public Response everythingTest(@Context HttpServletRequest request,
            @QueryParam("BBOX") String bbox,
            @QueryParam("APPLY_TAGS") @DefaultValue("false") Boolean applyTags,
            @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {

        Users user = Users.fromRequest(request);

        JSONObject json = new JSONObject();
        String jobId = "grail_" + UUID.randomUUID().toString().replace("-", "");
        String userName = (user == null) ? null : user.getDisplayName();

        json.put("jobid", jobId);
        json.put("BBox", bbox);
        json.put("User_ID", userName);
        json.put("Debug", debugLevel);

        logger.info("BBOX:" + bbox);
        logger.info("User:" + userName);
        logger.info("applyTags:" + applyTags);
        logger.info("Debug_Level:" + debugLevel);
        logger.info("json:" + json.toJSONString());

        return Response.ok(json.toJSONString()).build();
    }

    @POST
    @Path("/conflatepush")
    @Produces(MediaType.APPLICATION_JSON)
    public Response conflatePush(@Context HttpServletRequest request,
            @QueryParam("INPUT1") String input1,
            @QueryParam("INPUT2") String input2,
            @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {

        Users user = Users.fromRequest(request);

        JSONObject json = new JSONObject();
        String mainJobId = "grail_" + UUID.randomUUID().toString().replace("-", "");
        json.put("jobid", mainJobId);

        List<Command> workflow = new LinkedList<>();

        File workDir = new File(TEMP_OUTPUT_PATH, mainJobId);
        try {
            FileUtils.forceMkdir(workDir);
        }
        catch (IOException ioe) {
            logger.error("GrailConflate: Error creating folder: {} ", workDir.getAbsolutePath(), ioe);
            return Response.status(Response.Status.INTERNAL_SERVER_ERROR).entity(ioe.getMessage()).build();
        }

        GrailParams params = new GrailParams();
        params.setUser(user);

        try {
            // Run changeset-derive
            params.setInput1(HOOTAPI_DB_URL + "/" + input1);
            params.setInput2(HOOTAPI_DB_URL + "/" + input2);

            File changeSet = new File(workDir, "diff.osc");
            if (changeSet.exists())
                changeSet.delete();
            params.setOutput(changeSet.getAbsolutePath());
            ExternalCommand makeChangeset = grailCommandFactory.build(mainJobId, params, debugLevel, DeriveChangesetCommand.class, this.getClass());
            workflow.add(makeChangeset);

            // Apply changeset
            params.setPushUrl(RAILSPORT_PUSH_URL);
            ExternalCommand applyChange = grailCommandFactory.build(mainJobId, params, debugLevel, ApplyChangesetCommand.class, this.getClass());
            workflow.add(applyChange);

            // Now roll the dice and run everything.....
            jobProcessor.submitAsync(new Job(mainJobId, workflow.toArray(new Command[workflow.size()])));
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (IllegalArgumentException iae) {
            throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
        }
        catch (Exception e) {
            String msg = "Error during grail conflate! Params: " + params;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return Response.ok(json.toJSONString()).build();
    }

    @POST
    @Path("/conflatetest")
    @Produces(MediaType.APPLICATION_JSON)
    public Response conflateTest(@Context HttpServletRequest request,
            @QueryParam("INPUT1") String input1,
            @QueryParam("INPUT2") String input2,
            @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {

        JSONObject json = new JSONObject();
        String jobId = "grail_" + UUID.randomUUID().toString().replace("-", "");

        Users user = Users.fromRequest(request);
        String userName = (user == null) ? null : user.getDisplayName();

        json.put("jobid", jobId);
        json.put("Input1", input1);
        json.put("Input2", input2);
        json.put("User_ID", userName);
        json.put("Debug", debugLevel);

        logger.info("User:" + userName);
        logger.info("Input1:" + input1);
        logger.info("Input2:" + input2);
        logger.info("Debug_Level:" + debugLevel);
        logger.info("json:" + json.toJSONString());

        return Response.ok(json.toJSONString()).build();
    }

    /**
     * Pull the OSM data for a bounding box.
     *
     * POST hoot-services/grail/pullosm?BBOX=left,bottom,right,top&DEBUG_LEVEL=<error,info,debug,verbose,trace>
     *        left is the longitude of the left (west) side of the bounding box
     *        bottom is the latitude of the bottom (south) side of the bounding box
     *        right is the longitude of the right (east) side of the bounding box
     *        top is the latitude of the top (north) side of the bounding box
     *
     * @param params
     *
     * @return Job ID Internally, this is the directory that the files are kept in
     */
    @POST
    @Path("/pullosm")
    @Produces(MediaType.APPLICATION_JSON)
    public Response pullOsm(@QueryParam("BBOX") String bbox,
            @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {

        String jobId = "grail_" + UUID.randomUUID().toString().replace("-", "");
        File workDir = new File(TEMP_OUTPUT_PATH, jobId);

        JSONObject json = new JSONObject();
        json.put("jobid", jobId);

        try {
            FileUtils.forceMkdir(workDir);
        }
        catch (IOException ioe) {
            logger.error("PullOSM: Error creating folder: {} ", workDir.getAbsolutePath(), ioe);
            return Response.status(Response.Status.INTERNAL_SERVER_ERROR).entity(ioe.getMessage()).build();
        }

        // Not sure how to check if the main Overpass server is alive.
        // APICapabilities mainOsmApiCapabilities = getCapabilities(MAIN_OSMAPI_CAPABILITIES_URL);
        // logger.info("PullOSM: mainOSMAPI status = " + mainOsmApiCapabilities.getApiStatus());
        // if (mainOsmApiCapabilities.getApiStatus() == "offline" | mainOsmApiCapabilities.getApiStatus() == null) {
        //     return Response.status(Response.Status.SERVICE_UNAVAILABLE).entity("The main OSM API server is offline. Try again later").build();
        // }

        APICapabilities railsPortCapabilities = getCapabilities(RAILSPORT_CAPABILITIES_URL);
        logger.info("PullOSM: railsPortAPI status = " + railsPortCapabilities.getApiStatus());
        if (railsPortCapabilities.getApiStatus() == null | railsPortCapabilities.getApiStatus().equals("offline")) {
            return Response.status(Response.Status.SERVICE_UNAVAILABLE).entity("The local OSM API server is offline. Try again later").build();
        }

        GrailParams apiParams = new GrailParams();
        GrailParams overpassParams = new GrailParams();

        apiParams.setBounds(bbox);
        apiParams.setMaxBBoxSize(railsPortCapabilities.getMaxArea());
        apiParams.setPullUrl(RAILSPORT_PULL_URL);

        overpassParams.setBounds(bbox);
        overpassParams.setMaxBBoxSize(railsPortCapabilities.getMaxArea());
        overpassParams.setPullUrl(MAIN_OVERPASS_URL);

        List<Command> workflow = new LinkedList<>();

        try {
            // Pull data from the local OSM API Db
            File localOSMFile = new File(workDir, "local.osm");
            if (localOSMFile.exists()) localOSMFile.delete();

            apiParams.setOutput(localOSMFile.getAbsolutePath());
            // ExternalCommand getLocalOSM = grailCommandFactory.build(jobId,params,debugLevel,PullOSMDataCommand.class,this.getClass());
            InternalCommand getLocalOSM = apiCommandFactory.build(jobId, apiParams, this.getClass());
            workflow.add(getLocalOSM);

            // Pull OSM data from the real, internet, OSM Db using overpass
            File internetOSMFile = new File(workDir,"internet.osm");
            if (internetOSMFile.exists()) internetOSMFile.delete();

            overpassParams.setOutput(internetOSMFile.getAbsolutePath());

            InternalCommand getOverpassOSM = overpassCommandFactory.build(jobId, overpassParams, this.getClass());
            workflow.add(getOverpassOSM);

            jobProcessor.submitAsync(new Job(jobId, workflow.toArray(new Command[workflow.size()])));
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (IllegalArgumentException iae) {
            throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
        }
        catch (Exception e) {
            // Fix This
            String msg = "Error while getting OSM data! Params: " + apiParams.toString();
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return Response.ok(json.toJSONString()).build();
    }

    /**
     * Push OSM files to the Hoot DB
     *
     * POST hoot-services/grail/pushtodb/[JobId]?DEBUG_LEVEL=<error,info,debug,verbose,trace>
     *
     * @param jobId
     *            Internally, this is the directory that the files are kept in. We expect that the directory
     *            has local.osm & internet.osm files to run the diff with.
     *
     * @return Job ID
     *            This is the id for the processing job. NOT the directory that the data is stored in.
     */
    @POST
    @Path("/pushtodb/{jobId}")
    @Produces(MediaType.APPLICATION_JSON)
    public Response pushtodb(@Context HttpServletRequest request,
            @PathParam("jobId") String jobDir,
            @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {
        Response response;
        JSONObject json = new JSONObject();

        Users user = Users.fromRequest(request);

        File workDir = new File(TEMP_OUTPUT_PATH, jobDir);
        File localOSMFile = new File(workDir, "local.osm");
        File internetOSMFile = new File(workDir, "internet.osm");

        // the first 10 digits of a random UUID _should_ be unique....
        String randomString = "_" + StringUtils.left(UUID.randomUUID().toString().replace("-", ""), 10);
        String localDbFile = "local" + randomString;
        String internetDbFile = "internet" + randomString;

        GrailParams apiParams = new GrailParams();
        GrailParams overpassParams = new GrailParams();
        GrailParams linkParams = new GrailParams();

        List<Command> workflow = new LinkedList<>();

        String mainJobId = "grail_" + UUID.randomUUID().toString().replace("-", "");
        json.put("jobId", mainJobId);

        try {
            if (!workDir.exists()) {
                logger.error("PushToDb: jobDir {} does not exist.", workDir.getAbsolutePath());
                return Response.status(Response.Status.BAD_REQUEST).entity("Job " + jobDir + " does not exist.").build();
            }

            if (!localOSMFile.exists() || !internetOSMFile.exists()) {
                logger.error("PushToDb: Missing OSM files in {} ", workDir.getAbsolutePath());
                return Response.status(Response.Status.BAD_REQUEST).entity("Missing OSM files in " + jobDir + ". Did you run pullosm?").build();
            }

            // We could use the existing Import Command to push the OSM files to the DB BUT it will delete the import directory
            // Till I figure out a better way to do this, we will use our version.
            apiParams.setUser(user);
            apiParams.setInput1(localOSMFile.getAbsolutePath());
            apiParams.setOutput(localDbFile);
            ExternalCommand pushApi = grailCommandFactory.build(mainJobId, apiParams, debugLevel, PushToDbCommand.class, this.getClass());
            workflow.add(pushApi);

            overpassParams.setInput1(internetOSMFile.getAbsolutePath());
            overpassParams.setOutput(internetDbFile);
            ExternalCommand pushOverpass = grailCommandFactory.build(mainJobId, overpassParams, debugLevel, PushToDbCommand.class, this.getClass());
            workflow.add(pushOverpass);

            // Now create a folder and link the uploaded layers to it
            linkParams.setFolder(jobDir);
            linkParams.setInput1(localDbFile);
            linkParams.setInput2(internetDbFile);
            InternalCommand updateDb = updateDbCommandFactory.build(mainJobId, linkParams, this.getClass());
            workflow.add(updateDb);

            jobProcessor.submitAsync(new Job(mainJobId, workflow.toArray(new Command[workflow.size()])));

            ResponseBuilder responseBuilder = Response.ok(json.toJSONString());
            response = responseBuilder.build();
        }
        catch (WebApplicationException e) {
            throw e;
        }
        catch (IllegalArgumentException iae) {
            throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
        }
        catch (Exception e) {
            throw new WebApplicationException(e);
        }

        return response;
    }

    /**
     * Pull the OSM data for a bounding box.abd put it in the DB
     *
     * This is not good but it works for a proof of concept
     *
     * POST hoot-services/grail/pullosmtodb?BBOX=left,bottom,right,top&DEBUG_LEVEL=<error,info,debug,verbose,trace>
     *        left is the longitude of the left (west) side of the bounding box
     *        bottom is the latitude of the bottom (south) side of the bounding box
     *        right is the longitude of the right (east) side of the bounding box
     *        top is the latitude of the top (north) side of the bounding box
     *
     * @param params
     *
     * @return Job ID
     *            Internally, this is the directory that the files are kept in
     */
    @POST
    @Path("/pullosmtodb")
    @Produces(MediaType.APPLICATION_JSON)
    public Response pullOsmToDb(@Context HttpServletRequest request,
            @QueryParam("BBOX") String bbox,
            @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {

        Users user = Users.fromRequest(request);

        String jobId = "grail_" + UUID.randomUUID().toString().replace("-", "");
        File workDir = new File(TEMP_OUTPUT_PATH, jobId);

        Response response;

        JSONObject json = new JSONObject();
        json.put("jobid", jobId);

        try {
            FileUtils.forceMkdir(workDir);
        }
        catch (IOException ioe) {
            logger.error("PullOsmToDb: Error creating folder: {} ", workDir.getAbsolutePath(), ioe);
            return Response.status(Response.Status.INTERNAL_SERVER_ERROR).entity(ioe.getMessage()).build();
        }

        APICapabilities railsPortCapabilities = getCapabilities(RAILSPORT_CAPABILITIES_URL);
        logger.info("PullOSM: railsPortAPI status = " + railsPortCapabilities.getApiStatus());
        if (railsPortCapabilities.getApiStatus() == null | railsPortCapabilities.getApiStatus().equals("offline")) {
            return Response.status(Response.Status.SERVICE_UNAVAILABLE).entity("The local OSM API server is offline. Try again later").build();
        }

        GrailParams apiParams = new GrailParams();
        GrailParams overpassParams = new GrailParams();
        apiParams.setUser(user);
        overpassParams.setUser(user);

        apiParams.setBounds(bbox);
        apiParams.setMaxBBoxSize(railsPortCapabilities.getMaxArea());
        apiParams.setPullUrl(RAILSPORT_PULL_URL);

        overpassParams.setBounds(bbox);
        overpassParams.setMaxBBoxSize(railsPortCapabilities.getMaxArea());
        overpassParams.setPullUrl(MAIN_OVERPASS_URL);

        List<Command> workflow = new LinkedList<>();

        try {
            // Pull data from the local OSM API Db
            File localOSMFile = new File(workDir, "local.osm");
            if (localOSMFile.exists()) localOSMFile.delete();

            apiParams.setOutput(localOSMFile.getAbsolutePath());
            // ExternalCommand getLocalOSM = grailCommandFactory.build(jobId,params,debugLevel,PullOSMDataCommand.class,this.getClass());
            InternalCommand getLocalOSM = apiCommandFactory.build(jobId, apiParams, this.getClass());
            workflow.add(getLocalOSM);

            // Pull OSM data from the real, internet, OSM Db using overpass
            File internetOSMFile = new File(workDir,"internet.osm");
            if (internetOSMFile.exists()) internetOSMFile.delete();

            overpassParams.setOutput(internetOSMFile.getAbsolutePath());

            InternalCommand getOverpassOSM = overpassCommandFactory.build(jobId, overpassParams, this.getClass());
            workflow.add(getOverpassOSM);

            // Now we paste in the "pushtodb"
            // the first 10 digits of a random UUID _should_ be unique....
            String randomString = "_" + StringUtils.left(UUID.randomUUID().toString().replace("-", ""), 10);
            String localDbFile = "local" + randomString;
            String internetDbFile = "internet" + randomString;

            GrailParams apiPushParams = new GrailParams();
            GrailParams overpassPushParams = new GrailParams();
            GrailParams linkParams = new GrailParams();

            apiPushParams.setUser(user);
            overpassPushParams.setUser(user);
            linkParams.setUser(user);

            // We could use the existing Import Command to push the OSM files to the DB BUT it will delete the import directory
            // Till I figure out a better way to do this, we will use our version.
            apiPushParams.setInput1(localOSMFile.getAbsolutePath());
            apiPushParams.setOutput(localDbFile);
            ExternalCommand pushApi = grailCommandFactory.build(jobId, apiPushParams, debugLevel, PushToDbCommand.class, this.getClass());
            workflow.add(pushApi);

            overpassPushParams.setInput1(internetOSMFile.getAbsolutePath());
            overpassPushParams.setOutput(internetDbFile);
            ExternalCommand pushOverpass = grailCommandFactory.build(jobId, overpassPushParams, debugLevel, PushToDbCommand.class, this.getClass());
            workflow.add(pushOverpass);

            // Now create a folder and link the uploaded layers to it
            linkParams.setFolder(jobId);
            linkParams.setInput1(localDbFile);
            linkParams.setInput2(internetDbFile);
            InternalCommand updateDb = updateDbCommandFactory.build(jobId, linkParams, this.getClass());
            workflow.add(updateDb);

            jobProcessor.submitAsync(new Job(jobId, workflow.toArray(new Command[workflow.size()])));

            ResponseBuilder responseBuilder = Response.ok(json.toJSONString());
            response = responseBuilder.build();
        }
        catch (WebApplicationException e) {
            throw e;
        }
        catch (IllegalArgumentException iae) {
            throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
        }
        catch (Exception e) {
            throw new WebApplicationException(e);
        }

        return response;
    }

    /**
     * Run Hootenanny differential conflation
     *
     * POST hoot-services/grail/rundiff/[JobId]?DEBUG_LEVEL=<error,info,debug,verbose,trace>
     *
     * @param jobId
     *            Internally, this is the directory that the files are kept in. We expect that the directory
     *            has local.osm & internet.osm files to run the diff with.
     *
     * @return Job ID
     *            This is the id for the processing job. NOT the directory that the data is stored in.
     */
    @POST
    @Path("/rundiff/{jobId}")
    @Produces(MediaType.APPLICATION_JSON)
    public Response runDiff(@Context HttpServletRequest request,
            @PathParam("jobId") String jobDir,
            @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {
        Response response;
        JSONObject json = new JSONObject();

        Users user = Users.fromRequest(request);

        File workDir = new File(TEMP_OUTPUT_PATH, jobDir);
        File localOSMFile = new File(workDir, "local.osm");
        File internetOSMFile = new File(workDir, "internet.osm");

        try {
            if (!workDir.exists()) {
                logger.error("RunDiff: jobDir {} does not exist.", workDir.getAbsolutePath());
                return Response.status(Response.Status.BAD_REQUEST).entity("Job " + jobDir + " does not exist.").build();
            }

            if (!localOSMFile.exists() || !internetOSMFile.exists()) {
                logger.error("RunDiff: Missing OSM files in {} ", workDir.getAbsolutePath());
                return Response.status(Response.Status.BAD_REQUEST).entity("Missing OSM files in " + jobDir + ". Did you run pullosm?").build();
            }

            String newJobId = "grail_" + UUID.randomUUID().toString().replace("-", "");
            GrailParams params = new GrailParams();

            File diffFile = new File(workDir, "diff.osc");
            if (diffFile.exists()) diffFile.delete();

            params.setUser(user);
            params.setOutput(diffFile.getAbsolutePath());
            params.setInput1(localOSMFile.getAbsolutePath());
            params.setInput2(internetOSMFile.getAbsolutePath());

            ExternalCommand makeDiff = grailCommandFactory.build(newJobId, params, debugLevel, RunDiffCommand.class, this.getClass());

            Command[] workflow = { makeDiff };

            jobProcessor.submitAsync(new Job(newJobId, workflow));

            json.put("jobId", newJobId);
            ResponseBuilder responseBuilder = Response.ok(json.toJSONString());
            response = responseBuilder.build();
        }
        catch (WebApplicationException e) {
            throw e;
        }
        catch (IllegalArgumentException iae) {
            throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
        }
        catch (Exception e) {
            throw new WebApplicationException(e);
        }

        return response;
    }

    /**
     * Apply changesets to the OSM API Db
     *
     * POST hoot-services/grail/applydiff/[JobId]?USER_ID="Papa Smurf"&APPLY_TAGS=falseDEBUG_LEVEL=<error,info,debug,verbose,trace>
     *
     * @param jobId
     *            Internally, this is the directory that the files are kept in. We expect that the directory
     *            has a diff.osc file and possibly a diff.tags.osc file.
     *
     * @param USER_ID
     *            A user ID string that gets added to the changeset comment.
     *
     * @param APPLY_TAGS
     *            If we have a diff.tags.osc file, apply that as well.
     *
     * @return  jobid:geomapply, jobid:tagapply or status text
     *            These are the id's for the processing jobs. NOT the directory that the data is stored in.
     */
    @POST
    @Path("/applydiff/{jobId}")
    @Produces(MediaType.APPLICATION_JSON)
    public Response runApply(@Context HttpServletRequest request,
            @PathParam("jobId") String jobDir,
            @DefaultValue("false") @QueryParam("APPLY_TAGS") Boolean applyTags,
            @DefaultValue("info") @QueryParam("DEBUG_LEVEL") String debugLevel) {

        Users user = Users.fromRequest(request);

        JSONObject json = new JSONObject();
        File workDir = new File(TEMP_OUTPUT_PATH, jobDir);

        if (!workDir.exists()) {
            logger.error("ApplyDiff: jobDir {} does not exist.", workDir.getAbsolutePath());
            return Response.status(Response.Status.BAD_REQUEST).entity("Job " + jobDir + " does not exist.").build();
        }

        APICapabilities railsPortCapabilities = getCapabilities(RAILSPORT_CAPABILITIES_URL);
        logger.info("ApplyDiff: railsPortAPI status = " + railsPortCapabilities.getApiStatus());
        if (railsPortCapabilities.getApiStatus() == null | railsPortCapabilities.getApiStatus().equals("offline")) {
            return Response.status(Response.Status.SERVICE_UNAVAILABLE).entity("The local OSM API server is offline. Try again later").build();
        }

        GrailParams params = new GrailParams();
        params.setUser(user);
        params.setPushUrl(RAILSPORT_PUSH_URL);

        File geomDiffFile = new File(workDir, "diff.osc");

        if (geomDiffFile.exists()) {
            params.setOutput(geomDiffFile.getAbsolutePath());

            try {
                String geomJobId = "grail_" + UUID.randomUUID().toString().replace("-", "");
                json.put("jobid:geometryapply", geomJobId);

                ExternalCommand applyGeomChange = grailCommandFactory.build(geomJobId, params, debugLevel, ApplyChangesetCommand.class, this.getClass());

                Command[] workflow = { applyGeomChange };
                jobProcessor.submitAsync(new Job(geomJobId, workflow));
            }
            catch (WebApplicationException wae) {
                throw wae;
            }
            catch (IllegalArgumentException iae) {
                throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
            }
            catch (Exception e) {
                String msg = "Error during apply geometry diff! Params: " + params;
                throw new WebApplicationException(e, Response.serverError().entity(msg).build());
            }
        } // End GeomDiff
        else {
            logger.error("RunApply: No diff.osc file in {}.", workDir.getAbsolutePath());
            json.put("geometryapply", "No geometry diff file (diff.osc) in " + jobDir);
        }

        File tagDiffFile = new File(workDir, "diff.tags.osc");

        if (applyTags) {
            if (tagDiffFile.exists()) {
                params.setOutput(tagDiffFile.getAbsolutePath());

                try {
                    String tagJobId = "grail_" + UUID.randomUUID().toString().replace("-", "");
                    json.put("jobid:tagapply", tagJobId);

                    ExternalCommand applyTagChange = grailCommandFactory.build(tagJobId, params, debugLevel, ApplyChangesetCommand.class, this.getClass());

                    Command[] workflow = { applyTagChange };
                    jobProcessor.submitAsync(new Job(tagJobId, workflow));
                }
                catch (WebApplicationException wae) {
                    throw wae;
                }
                catch (IllegalArgumentException iae) {
                    throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
                }
                catch (Exception e) {
                    String msg = "Error during apply tag diff! Params: " + params;
                    throw new WebApplicationException(e, Response.serverError().entity(msg).build());
                }
            }
            else {
                logger.error("RunApply: Requested APPLY_TAGS but no diff.tags.osc file in {}.", workDir.getAbsolutePath());
                json.put("tagapply", "Requested APPLY_TAGS but no tag diff file (diff.tags.osc) in " + jobDir);
            }
        }

        return Response.ok(json.toJSONString()).build();
    }

    /**
     * Apply just the geometry diff to the OSM API Db
     *
     * POST hoot-services/grail/applygeomdiff/[JobId]?USER_ID="Papa Smurf"&APPLY_TAGS=falseDEBUG_LEVEL=<error,info,debug,verbose,trace>
     *
     * @param jobId
     *            Internally, this is the directory that the files are kept in. We expect that the directory
     *            has a diff.osc file and possibly a diff.tags.osc file.
     *
     * @param USER_ID
     *            A user ID string that gets added to the changeset comment.
     *
     * @return   jobid or status text
     *            The id for the processing jobs. NOT the directory that the data is stored in.
     */
    @POST
    @Path("/applygeomdiff/{jobId}")
    @Produces(MediaType.APPLICATION_JSON)
    public Response runApplyGeom(@Context HttpServletRequest request,
            @PathParam("jobId") String jobDir,
            @DefaultValue("info") @QueryParam("DEBUG_LEVEL") String debugLevel) {

        Users user = Users.fromRequest(request);

        JSONObject json = new JSONObject();
        File workDir = new File(TEMP_OUTPUT_PATH, jobDir);

        if (!workDir.exists()) {
            logger.error("ApplyGeomDiff: jobDir {} does not exist.", workDir.getAbsolutePath());
            return Response.status(Response.Status.BAD_REQUEST).entity("Job " + jobDir + " does not exist.").build();
        }

        APICapabilities railsPortCapabilities = getCapabilities(RAILSPORT_CAPABILITIES_URL);
        logger.info("RunGeomApply: railsPortAPI status = " + railsPortCapabilities.getApiStatus());
        if (railsPortCapabilities.getApiStatus() == null | railsPortCapabilities.getApiStatus().equals("offline")) {
            return Response.status(Response.Status.SERVICE_UNAVAILABLE).entity("The local OSM API server is offline. Try again later").build();
        }

        GrailParams params = new GrailParams();
        params.setUser(user);
        params.setPushUrl(RAILSPORT_PUSH_URL);

        File geomDiffFile = new File(workDir, "diff.osc");

        if (geomDiffFile.exists()) {
            params.setOutput(geomDiffFile.getAbsolutePath());

            try {
                String geomJobId = "grail_" + UUID.randomUUID().toString().replace("-", "");
                json.put("jobid", geomJobId);

                ExternalCommand applyGeomChange = grailCommandFactory.build(geomJobId, params, debugLevel, ApplyChangesetCommand.class, this.getClass());

                Command[] workflow = { applyGeomChange };
                jobProcessor.submitAsync(new Job(geomJobId, workflow));
            }
            catch (WebApplicationException wae) {
                throw wae;
            }
            catch (IllegalArgumentException iae) {
                throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
            }
            catch (Exception e) {
                String msg = "Error during apply geometry diff! Params: " + params;
                throw new WebApplicationException(e, Response.serverError().entity(msg).build());
            }
        } // End GeomDiff
        else {
            logger.error("ApplyGeomDiff: No diff.osc file in {}.", workDir.getAbsolutePath());
            json.put("error", "No geometry diff file (diff.osc) in " + jobDir);
        }

        return Response.ok(json.toJSONString()).build();
    }

    /**
     * Apply just the tag diff to the OSM API Db
     *
     * POST hoot-services/grail/applytagdiff/[JobId]?USER_ID="Papa Smurf"&APPLY_TAGS=falseDEBUG_LEVEL=<error,info,debug,verbose,trace>
     *
     * @param jobId
     *            Internally, this is the directory that the files are kept in. We expect that the directory
     *            has a diff.osc file and possibly a diff.tags.osc file.
     *
     * @param USER_ID
     *            A user ID string that gets added to the changeset comment.
     *
     * @return  jobid or status text
     *            The id for the processing job. NOT the directory that the data is stored in.
     */
    @POST
    @Path("/applytagdiff/{jobId}")
    @Produces(MediaType.APPLICATION_JSON)
    public Response runApplyTag(@Context HttpServletRequest request,
            @PathParam("jobId") String jobDir,
            @DefaultValue("info") @QueryParam("DEBUG_LEVEL") String debugLevel) {

        Users user = Users.fromRequest(request);

        JSONObject json = new JSONObject();
        File workDir = new File(TEMP_OUTPUT_PATH, jobDir);

        if (!workDir.exists()) {
            logger.error("ApplyTagDiff: jobDir {} does not exist.", workDir.getAbsolutePath());
            return Response.status(Response.Status.BAD_REQUEST).entity("Job " + jobDir + " does not exist.").build();
        }

        APICapabilities railsPortCapabilities = getCapabilities(RAILSPORT_CAPABILITIES_URL);
        logger.info("ApplyTagDiff: railsPortAPI status = " + railsPortCapabilities.getApiStatus());
        if (railsPortCapabilities.getApiStatus() == null | railsPortCapabilities.getApiStatus().equals("offline")) {
            return Response.status(Response.Status.SERVICE_UNAVAILABLE).entity("The local OSM API server is offline. Try again later").build();
        }

        GrailParams params = new GrailParams();
        params.setUser(user);
        params.setPushUrl(RAILSPORT_PUSH_URL);

        File tagDiffFile = new File(workDir, "diff.tags.osc");

        if (tagDiffFile.exists()) {
            params.setOutput(tagDiffFile.getAbsolutePath());

            try {
                String tagJobId = "grail_" + UUID.randomUUID().toString().replace("-", "");
                json.put("jobid:", tagJobId);

                ExternalCommand applyTagChange = grailCommandFactory.build(tagJobId, params, debugLevel, ApplyChangesetCommand.class, this.getClass());

                Command[] workflow = { applyTagChange };
                jobProcessor.submitAsync(new Job(tagJobId, workflow));
            }
            catch (WebApplicationException wae) {
                throw wae;
            }
            catch (IllegalArgumentException iae) {
                throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
            }
            catch (Exception e) {
                String msg = "Error during apply tag diff! Params: " + params;
                throw new WebApplicationException(e, Response.serverError().entity(msg).build());
            }
        }
        else {
            logger.error("RunApply: Requested APPLY_TAGS but no diff.tags.osc file in {}.", workDir.getAbsolutePath());
            json.put("tagapply", "Requested APPLY_TAGS but no tag diff file (diff.tags.osc) in " + jobDir);
        }

        return Response.ok(json.toJSONString()).build();
    }

    /**
     * Get the geometry diff file from job.
     *
     * GET hoot-services/grail/geomdiff/[job id from pullosm job]
     *
     * @param jobId
     *            job id. Internally, this is the directory that the files are kept in
     *
     * @return Octet stream
     */
    @GET
    @Path("geomdiff/{id}")
    @Produces(MediaType.APPLICATION_OCTET_STREAM)
    public Response exportGeomDiffFile(@PathParam("id") String jobId) {
        Response response;

        try {
            File exportFile = getFile(jobId, "diff", "osc");

            ResponseBuilder responseBuilder = Response.ok(exportFile);
            responseBuilder.header("Content-Disposition", "attachment; filename=diff.osc");

            response = responseBuilder.build();
        }
        catch (WebApplicationException e) {
            logger.error("GeomDiff: File diff.osc does not exist in " + jobId);
            return Response.status(Response.Status.NOT_FOUND).entity(e.getMessage()).build();
        }
        catch (IllegalArgumentException iae) {
            throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
        }
        catch (Exception e) {
            throw new WebApplicationException(e);
        }

        return response;
    }

    /**
     * Get the tag diff file from job.
     *
     * GET hoot-services/grail/tagdiff/[job id from pullosm job]
     *
     * @param jobId
     *            job id. Internally, this is the directory that the files are kept in
     *
     * @return Octet stream
     */
    @GET
    @Path("tagdiff/{id}")
    @Produces(MediaType.APPLICATION_OCTET_STREAM)
    public Response exportTagDiffFile(@PathParam("id") String jobId) {
        Response response;

        try {
            File exportFile = getFile(jobId, "diff.tags", "osc");

            ResponseBuilder responseBuilder = Response.ok(exportFile);
            responseBuilder.header("Content-Disposition", "attachment; filename=diff.tags.osc");

            response = responseBuilder.build();
        }
        catch (WebApplicationException e) {
            logger.error("TagDiff: File diff.tags.osc does not exist in " + jobId);
            return Response.status(Response.Status.NOT_FOUND).entity(e.getMessage()).build();
        }
        catch (IllegalArgumentException iae) {
            throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
        }
        catch (Exception e) {
            throw new WebApplicationException(e);
        }

        return response;
    }

    /**
     * To retrieve the tag error diff file from job make a GET request.
     *
     * GET hoot-services/grail/tagdifferrror/[job id from export job]
     *
     * @param jobId
     *            job id. Internally, this is the directory that the files are kept in
     *
     * @return Octet stream
     */
    @GET
    @Path("tagdifferror/{id}")
    @Produces(MediaType.APPLICATION_OCTET_STREAM)
    public Response exportTagErrorFile(@PathParam("id") String jobId) {
        Response response;

        try {
            // NOTE: This will be changed so that it grabs the error.osc file after we figure out
            // what it is called.
            File exportFile = getFile(jobId, "diff-error.tags", "osc");

            ResponseBuilder responseBuilder = Response.ok(exportFile);
            responseBuilder.header("Content-Disposition", "attachment; filename=diff-error.tags.osc");

            response = responseBuilder.build();
        }
        catch (WebApplicationException e) {
            logger.error("ErrorDiff: File diff-error.tags.osc does not exist in " + jobId);
            return Response.status(Response.Status.NOT_FOUND).entity(e.getMessage()).build();
        }
        catch (IllegalArgumentException iae) {
            throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
        }
        catch (Exception e) {
            throw new WebApplicationException(e);
        }

        return response;
    }

    /**
     * To retrieve the geometry error diff file from job make a GET request.
     *
     * GET hoot-services/grail/geomdifferror/[job id from export job]
     *
     * @param jobId
     *            job id. Internally, this is the directory that the files are kept in
     *
     * @return Octet stream
     */
    @GET
    @Path("geomdifferror/{id}")
    @Produces(MediaType.APPLICATION_OCTET_STREAM)
    public Response exportGeomErrorFile(@PathParam("id") String jobId) {
        Response response;

        try {
            // NOTE: This will be changed so that it grabs the error.osc file after we figure out
            // what it is called.
            File exportFile = getFile(jobId, "diff-error", "osc");

            ResponseBuilder responseBuilder = Response.ok(exportFile);
            responseBuilder.header("Content-Disposition", "attachment; filename=diff-error.osc");

            response = responseBuilder.build();
        }
        catch (WebApplicationException e) {
            logger.error("ErrorDiff: File diff-error.osc does not exist in " + jobId);
            return Response.status(Response.Status.NOT_FOUND).entity(e.getMessage()).build();
        }
        catch (IllegalArgumentException iae) {
            throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
        }
        catch (Exception e) {
            throw new WebApplicationException(e);
        }

        return response;
    }

    // Get a file if it exists
    private static File getFile(String jobId, String outputName, String fileExt) {
        File tmpFile = new File(new File(TEMP_OUTPUT_PATH, jobId), outputName + "." + fileExt);

        if (!tmpFile.exists()) {
            throw new WebApplicationException("Error.  Missing file: " + outputName + "." + fileExt);
        }

        return tmpFile;
    }

    // Get Capabilities from an OSM API Db
    private static APICapabilities getCapabilities(String capabilitiesUrl) {
        APICapabilities params = new APICapabilities();

        try {
            DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
            DocumentBuilder db = dbf.newDocumentBuilder();
            Document doc = db.parse(new URL(replaceSensitiveData(capabilitiesUrl)).openStream());

            doc.getDocumentElement().normalize();

            NodeList nl = doc.getElementsByTagName("api");

            if (nl != null && nl.getLength() > 0) {
                Node nNode = nl.item(0);

                if (nNode.getNodeType() == Node.ELEMENT_NODE) {
                    Element eElement = (Element) nNode;

                    // OK. this is ugly. Apparently there is some way to parse XML to POJO
                    NodeList tList = eElement.getElementsByTagName("version");
                    String tValue = tList.item(0).getAttributes().getNamedItem("maximum").getNodeValue();
                    params.setMaxVersion(Double.parseDouble(tValue));

                    tValue = tList.item(0).getAttributes().getNamedItem("minimum").getNodeValue();
                    params.setMinVersion(Double.parseDouble(tValue));

                    tList = eElement.getElementsByTagName("area");
                    tValue = tList.item(0).getAttributes().getNamedItem("maximum").getNodeValue();
                    params.setMaxArea(Double.parseDouble(tValue));

                    tList = eElement.getElementsByTagName("waynodes");
                    tValue = tList.item(0).getAttributes().getNamedItem("maximum").getNodeValue();
                    params.setWaynodes(Long.parseLong(tValue));

                    tList = eElement.getElementsByTagName("changesets");
                    tValue = tList.item(0).getAttributes().getNamedItem("maximum_elements").getNodeValue();
                    params.setChangesets(Long.parseLong(tValue));

                    tList = eElement.getElementsByTagName("timeout");
                    tValue = tList.item(0).getAttributes().getNamedItem("seconds").getNodeValue();
                    params.setTimeout(Long.parseLong(tValue));

                    tList = eElement.getElementsByTagName("status");
                    tValue = tList.item(0).getAttributes().getNamedItem("database").getNodeValue();
                    params.setDatabaseStatus(tValue);

                    tValue = tList.item(0).getAttributes().getNamedItem("api").getNodeValue();
                    params.setApiStatus(tValue);

                    tValue = tList.item(0).getAttributes().getNamedItem("gpx").getNodeValue();
                    params.setGpxStatus(tValue);
                }
            }
        }
        catch (Exception e) {
            // throw new WebApplicationException(ioe, Response.status(Response.Status.BAD_REQUEST).entity(ioe.getMessage()).build());
             e.printStackTrace();
        }

        return params;
    }

}
