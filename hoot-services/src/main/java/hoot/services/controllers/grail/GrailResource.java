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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.grail;

import static hoot.services.HootProperties.HOOTAPI_DB_URL;
import static hoot.services.HootProperties.PUBLIC_OVERPASS_URL;
import static hoot.services.HootProperties.RAILSPORT_CAPABILITIES_URL;
import static hoot.services.HootProperties.RAILSPORT_PULL_URL;
import static hoot.services.HootProperties.RAILSPORT_PUSH_URL;
import static hoot.services.HootProperties.TEMP_OUTPUT_PATH;
import static hoot.services.HootProperties.replaceSensitiveData;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.URL;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.UUID;

import javax.servlet.UnavailableException;
import javax.servlet.http.HttpServletRequest;
import javax.ws.rs.Consumes;
import javax.ws.rs.DefaultValue;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.ResponseBuilder;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.TransformerException;

import org.apache.commons.io.FileUtils;
import org.apache.commons.io.filefilter.IOFileFilter;
import org.apache.commons.io.filefilter.RegexFileFilter;
import org.apache.commons.io.filefilter.WildcardFileFilter;
import org.apache.commons.lang3.StringUtils;
import org.apache.xpath.XPathAPI;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.oauth.common.signature.SharedConsumerSecret;
import org.springframework.security.oauth.consumer.ProtectedResourceDetails;
import org.springframework.security.oauth.consumer.client.OAuthRestTemplate;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import hoot.services.command.Command;
import hoot.services.command.ExternalCommand;
import hoot.services.command.InternalCommand;
import hoot.services.controllers.osm.map.SetMapTagsCommandFactory;
import hoot.services.controllers.osm.map.UpdateParentCommandFactory;
import hoot.services.geo.BoundingBox;
import hoot.services.job.Job;
import hoot.services.job.JobProcessor;
import hoot.services.job.JobType;
import hoot.services.models.db.Users;
import hoot.services.utils.DbUtils;
import hoot.services.utils.XmlDocumentBuilder;


@Controller
@Path("")
@Transactional
public class GrailResource {
    private static final Logger logger = LoggerFactory.getLogger(GrailResource.class);
    private static final String REFERENCE = "reference";
    private static final String SECONDARY = "secondary";

    @Autowired
    private JobProcessor jobProcessor;

    @Autowired
    private GrailCommandFactory grailCommandFactory;

    @Autowired
    private SetMapTagsCommandFactory setMapTagsCommandFactory;

    @Autowired
    private PullOverpassCommandFactory overpassCommandFactory;

    @Autowired
    private PullApiCommandFactory apiCommandFactory;

    @Autowired
    private UpdateDbCommandFactory updateDbCommandFactory;

    @Autowired
    private OAuthRestTemplate oauthRestTemplate;

    @Autowired
    private UpdateParentCommandFactory updateParentCommandFactory;

    public GrailResource() {}

    private Command getRailsPortApiCommand(String jobId, Users user, String bounds, String output) throws UnavailableException {
        APICapabilities railsPortCapabilities = getCapabilities(RAILSPORT_CAPABILITIES_URL);
        if (railsPortCapabilities.getApiStatus() == null
                || railsPortCapabilities.getApiStatus().equals("offline")) {
            throw new UnavailableException("The Rails port API is offline.");
        }

        GrailParams params = new GrailParams();
        params.setUser(user);
        params.setBounds(bounds);
        params.setMaxBBoxSize(railsPortCapabilities.getMaxArea());
        params.setPullUrl(RAILSPORT_PULL_URL);
        params.setOutput(output);
        InternalCommand command = apiCommandFactory.build(jobId, params, this.getClass());
        return command;
    }

    private Command getPublicOverpassCommand(String jobId, Users user, String bounds, String output) {
        //TODO: is there an availability check for overpass?
        GrailParams params = new GrailParams();
        params.setUser(user);
        params.setBounds(bounds);
        params.setPullUrl(PUBLIC_OVERPASS_URL);
        params.setOutput(output);
        InternalCommand command = overpassCommandFactory.build(jobId, params, this.getClass());
        return command;
    }

    /**
     * Pull the Public Overpass and Private Rails Port data for a bounding box and run differential on it
     *
     * Takes in a json object
     * POST hoot-services/grail/createdifferential
     *
     * {
     *   //The upper left (UL) and lower right (LR) of the bounding box to clip the dataset
     *   "BBOX" : "{"LR":[-77.04813267598544,38.89292259454q727],"UL":[-77.04315011486628,38.89958152667718]}",
     * }
     *
     * @param reqParams
     *      JSON input params; see description above
     *
     * @param debugLevel
     *      debug level
     *
     * @return Job ID Internally, this is the directory that the files are kept in
     */
    @POST
    @Path("/createdifferential")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response createDifferential(@Context HttpServletRequest request,
            GrailParams reqParams,
            @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {

        String jobId = "grail_" + UUID.randomUUID().toString().replace("-", "");

        File workDir = new File(TEMP_OUTPUT_PATH, jobId);
        try {
            FileUtils.forceMkdir(workDir);
        }
        catch (IOException ioe) {
            logger.error("createdifferential: Error creating folder: {} ", workDir.getAbsolutePath(), ioe);
            return Response.status(Response.Status.INTERNAL_SERVER_ERROR).entity(ioe.getMessage()).build();
        }

        Users user = Users.fromRequest(request);
        List<Command> workflow = new LinkedList<>();
        String bbox = reqParams.getBounds();

        JSONObject jobInfo = new JSONObject();
        jobInfo.put("jobid", jobId);

        // Pull reference data from Rails port OSM API
        File referenceOSMFile = new File(workDir, REFERENCE + ".osm");
        if (referenceOSMFile.exists()) referenceOSMFile.delete();
        try {
            workflow.add(getRailsPortApiCommand(jobId, user, bbox, referenceOSMFile.getAbsolutePath()));
        } catch (UnavailableException ex) {
            return Response.status(Response.Status.SERVICE_UNAVAILABLE).entity(ex.getMessage()).build();
        }

        // Pull secondary data from the Public Overpass API
        File secondaryOSMFile = new File(workDir, SECONDARY + ".osm");
        if (secondaryOSMFile.exists()) secondaryOSMFile.delete();
        workflow.add(getPublicOverpassCommand(jobId, user, bbox, secondaryOSMFile.getAbsolutePath()));

        // Run the differential conflate command.
        GrailParams params = new GrailParams();
        params.setUser(user);
        params.setInput1(referenceOSMFile.getAbsolutePath());
        params.setInput2(secondaryOSMFile.getAbsolutePath());

        File geomDiffFile = new File(workDir, "diff.osc");
        if (geomDiffFile.exists()) geomDiffFile.delete();

        try {
            geomDiffFile.createNewFile();
        }
        catch(IOException exc) {
            logger.error("createDifferential: Error creating file: {} ", geomDiffFile.getAbsolutePath(), exc);
            return Response.status(Response.Status.INTERNAL_SERVER_ERROR).entity(exc.getMessage()).build();
        }

        params.setOutput(geomDiffFile.getAbsolutePath());

        ExternalCommand makeDiff = grailCommandFactory.build(jobId, params, debugLevel, RunDiffCommand.class, this.getClass());
        workflow.add(makeDiff);

        jobProcessor.submitAsync(new Job(jobId, user.getId(), workflow.toArray(new Command[workflow.size()]), JobType.DERIVE_CHANGESET));

        return Response.ok(jobInfo.toJSONString()).build();
    }

    /**
     * Retrieve statistics on the specified differential
     *
     * GET hoot-services/grail/differentialstats/{jobId}
     *
     * @param jobDir
     *      Internally, this is the directory that the files are kept in. We expect that the directory
     *      has dest.osm & source.osm files to run the diff with.
     *
     * @param debugLevel
     *      debug level
     *
     * @return JSON object containing info about the differential such as
     *      the count of new nodes, ways, and relations
     */
    @GET
    @Path("/differentialstats")
    @Produces(MediaType.APPLICATION_JSON)
    public Response differentialStats(@Context HttpServletRequest request,
            @QueryParam("jobId") String jobDir,
            @QueryParam("includeTags") Boolean includeTags,
            @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {

        JSONObject jobInfo = new JSONObject();

        String fileDirectory = TEMP_OUTPUT_PATH + "/" + jobDir;
        File workDir = new File(fileDirectory);

        try {
            IOFileFilter fileFilter;
            if(includeTags) {
                fileFilter = new WildcardFileFilter("*.osc");
            } else {
                fileFilter = new RegexFileFilter("^(?!.*\\.tags\\.).*osc$");
            }

            List<File> oscFilesList = (List<File>) FileUtils.listFiles(workDir, fileFilter, null);

            for(File currentOsc : oscFilesList) {
                String xmlData = FileUtils.readFileToString(currentOsc, "UTF-8");
                Document changesetDiffDoc = XmlDocumentBuilder.parse(xmlData);
                logger.debug("Parsing changeset diff XML: {}", StringUtils.abbreviate(xmlData, 1000));

                for (DbUtils.EntityChangeType entityChangeType : DbUtils.EntityChangeType.values()) {
                    String changeTypeName = entityChangeType.toString().toLowerCase();

                    for (DbUtils.nwr_enum elementType : DbUtils.nwr_enum.values()) {
                        String elementTypeName = elementType.toString();

                        NodeList elementXmlNodes = XPathAPI.selectNodeList(changesetDiffDoc,
                                "//osmChange/" + changeTypeName +"/" + elementTypeName);

                        String key = changeTypeName + "-" + elementTypeName;
                        int count = jobInfo.get(key) == null ? elementXmlNodes.getLength() :
                                (int) jobInfo.get(key) + elementXmlNodes.getLength();

                        jobInfo.put(key, count);
                    }
                }
            }

            File tagDiffFile = new File(workDir, "diff.tags.osc");
            jobInfo.put("hasTags", tagDiffFile.exists());
        }
        catch (TransformerException e) {
            throw new RuntimeException("Error invoking XPathAPI!", e);
        }
        catch (Exception exc) {
            String msg = "Error during differential stats! jobid: " + jobDir;
            throw new WebApplicationException(exc, Response.serverError().entity(msg).build());
        }

        return Response.ok(jobInfo.toJSONString()).build();
    }

    /**
     * Pushes the specified differential changeset back to the
     * reference dataset API
     *
     * Takes in a json object
     * POST hoot-services/grail/differentialpush
     *
     * {
     *   //the job id. We make the directory for the differential with the name of the jobid
     *   "folder" : "grail_8e8c4681-ead9-4c27-aa2e-80746d627523",
     *
     *   //boolean if the user wants to apply the tags when pushing to reference API
     *   "APPLY_TAGS" : true
     * }
     *
     * @param reqParams
     *      JSON input params; see description above
     *
     * @param debugLevel
     *      debug level
     *
     * @return Job ID. Can be used to check status of the conflate push
     */
    @POST
    @Path("/differentialpush")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response differentialPush(@Context HttpServletRequest request,
            GrailParams reqParams,
            @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {

        Users user = Users.fromRequest(request);

        GrailParams params = new GrailParams();
        params.setUser(user);
        params.setPushUrl(RAILSPORT_PUSH_URL);

        ProtectedResourceDetails oauthInfo = oauthRestTemplate.getResource();
        params.setConsumerKey(oauthInfo.getConsumerKey());
        params.setConsumerSecret(((SharedConsumerSecret) oauthInfo.getSharedSecret()).getConsumerSecret());

        String jobId = "grail_" + UUID.randomUUID().toString().replace("-", "");

        JSONObject json = new JSONObject();
        json.put("jobid", jobId);
        String jobDir = reqParams.getFolder();
        File workDir = new File(TEMP_OUTPUT_PATH, jobDir);

        if (!workDir.exists()) {
            logger.error("ApplyDiff: jobDir {} does not exist.", workDir.getAbsolutePath());
            return Response.status(Response.Status.BAD_REQUEST).entity("Job " + jobDir + " does not exist.").build();
        }

        try {
            APICapabilities railsPortCapabilities = getCapabilities(RAILSPORT_CAPABILITIES_URL);
            logger.info("ApplyDiff: railsPortAPI status = " + railsPortCapabilities.getApiStatus());
            if (railsPortCapabilities.getApiStatus() == null || railsPortCapabilities.getApiStatus().equals("offline")) {
                return Response.status(Response.Status.SERVICE_UNAVAILABLE).entity("The reference OSM API server is offline. Try again later").build();
            }

            File geomDiffFile = new File(workDir, "diff.osc");
            List<Command> workflow = new LinkedList<>();

            if (geomDiffFile.exists()) {
                params.setOutput(geomDiffFile.getAbsolutePath());

                ExternalCommand applyGeomChange = grailCommandFactory.build(jobId, params, debugLevel, ApplyChangesetCommand.class, this.getClass());
                workflow.add(applyGeomChange);
            }
            else {
                String msg = "Error during differential push! Could not find differential file ";
                throw new WebApplicationException(new FileNotFoundException(), Response.serverError().entity(msg).build());
            }

            if (reqParams.getApplyTags()) {
                File tagDiffFile = new File(workDir, "diff.tags.osc");

                if (tagDiffFile.exists()) {
                    params.setOutput(tagDiffFile.getAbsolutePath());

                    ExternalCommand applyTagChange = grailCommandFactory.build(jobId, params, debugLevel, ApplyChangesetCommand.class, this.getClass());
                    workflow.add(applyTagChange);
                }
                else {
                    String msg = "Error during differential push! Could not find differential tags file ";
                    throw new WebApplicationException(new FileNotFoundException(), Response.serverError().entity(msg).build());
                }
            }

            jobProcessor.submitAsync(new Job(jobId, user.getId(), workflow.toArray(new Command[workflow.size()]), JobType.UPLOAD_CHANGESET));
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (IllegalArgumentException iae) {
            throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
        }
        catch (Exception e) {
            String msg = "Error during differential push! Params: " + params;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return Response.ok(json.toJSONString()).build();
    }

    /**
     * Runs changeset-derive on the two input layers
     *
     * Takes in a json object
     * POST hoot-services/grail/conflatedifferential
     *
     * {
     *   "input1" : // reference dataset name
     *
     *   "input2" : // secondary dataset name
     * }
     *
     * @param reqParams
     *      JSON input params; see description above
     *
     * @param debugLevel
     *      debug level
     *
     * @return Job ID. Can be used to check status of the conflate push
     */
    @POST
    @Path("/conflatedifferential")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response conflateDifferential(@Context HttpServletRequest request,
            GrailParams reqParams,
            @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {

        Users user = Users.fromRequest(request);

        String input1 = reqParams.getInput1();
        String input2 = reqParams.getInput2();

        JSONObject json = new JSONObject();
        String mainJobId = "grail_" + UUID.randomUUID().toString().replace("-", "");
        json.put("jobid", mainJobId);

        List<Command> workflow = new LinkedList<>();

        File workDir = new File(TEMP_OUTPUT_PATH, mainJobId);
        try {
            FileUtils.forceMkdir(workDir);
        }
        catch (IOException ioe) {
            logger.error("conflateDifferential: Error creating folder: {} ", workDir.getAbsolutePath(), ioe);
            return Response.status(Response.Status.INTERNAL_SERVER_ERROR).entity(ioe.getMessage()).build();
        }

        GrailParams params = new GrailParams();
        params.setUser(user);

        try {
            // Run changeset-derive
            params.setInput1(HOOTAPI_DB_URL + "/" + input1);
            params.setInput2(HOOTAPI_DB_URL + "/" + input2);

            File changeSet = new File(workDir, "diff.osc");
            if (changeSet.exists()) { changeSet.delete(); }

            params.setOutput(changeSet.getAbsolutePath());
            ExternalCommand makeChangeset = grailCommandFactory.build(mainJobId, params, debugLevel, DeriveChangesetCommand.class, this.getClass());
            workflow.add(makeChangeset);

            // Now roll the dice and run everything.....
            jobProcessor.submitAsync(new Job(mainJobId, user.getId(), workflow.toArray(new Command[workflow.size()]), JobType.DERIVE_CHANGESET));
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (IllegalArgumentException iae) {
            throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
        }
        catch (Exception e) {
            String msg = "Error during conflate differential! Params: " + params;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return Response.ok(json.toJSONString()).build();
    }

    /**
     * Pushes the conflation result back to the reference dataset API
     *
     * Takes in a json object
     * POST hoot-services/grail/conflatepush
     *
     * {
     *   "folder" : // jobId name
     * }
     *
     * @param reqParams
     *      JSON input params; see description above
     *
     * @param debugLevel
     *      debug level
     *
     * @return Job ID. Can be used to check status of the conflate push
     */
    @POST
    @Path("/conflatepush")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response conflatePush(@Context HttpServletRequest request,
            GrailParams reqParams,
            @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {

        String jobDir = reqParams.getFolder();
        File workDir = new File(TEMP_OUTPUT_PATH, jobDir);
        if (!workDir.exists()) {
            logger.error("conflatePush: jobDir {} does not exist.", workDir.getAbsolutePath());
            return Response.status(Response.Status.BAD_REQUEST).entity("Job " + jobDir + " does not exist.").build();
        }

        Users user = Users.fromRequest(request);

        JSONObject json = new JSONObject();
        String mainJobId = "grail_" + UUID.randomUUID().toString().replace("-", "");
        json.put("jobid", mainJobId);

        List<Command> workflow = new LinkedList<>();
        GrailParams params = new GrailParams();
        params.setUser(user);

        try {
            ProtectedResourceDetails oauthInfo = oauthRestTemplate.getResource();
            params.setConsumerKey(oauthInfo.getConsumerKey());
            params.setConsumerSecret(((SharedConsumerSecret) oauthInfo.getSharedSecret()).getConsumerSecret());

            File geomDiffFile = new File(workDir, "diff.osc");
            if (geomDiffFile.exists()) {
                params.setOutput(geomDiffFile.getAbsolutePath());

                // Apply changeset
                params.setPushUrl(RAILSPORT_PUSH_URL);
                ExternalCommand applyChange = grailCommandFactory.build(mainJobId, params, debugLevel, ApplyChangesetCommand.class, this.getClass());
                workflow.add(applyChange);

                jobProcessor.submitAsync(new Job(mainJobId, user.getId(), workflow.toArray(new Command[workflow.size()]), JobType.UPLOAD_CHANGESET));
            }
            else {
                String msg = "Error during conflate push! Could not find osc file ";
                throw new WebApplicationException(new FileNotFoundException(), Response.serverError().entity(msg).build());
            }
        }
        catch (IllegalArgumentException iae) {
            throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
        }
        catch (Exception e) {
            String msg = "Error during conflate push! Could not find osc file ";
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return Response.ok(json.toJSONString()).build();
    }

    /**
     * Pull the remote data for a bounding box
     * from public Overpass API
     * and write to a Hoot map dataset
     *
     * The data from Overpass will be named
     * the secondary layer for conflation
     *
     * The purpose of conflating these datasets is to update
     * a private OSM instance that has diverged from the public OSM
     * with private changes.
     *
     * @param bbox The bounding box
     *
     * @return Job ID
     * used by the client for polling job status
     * Internally, this provides the map dataset name suffix
     */
    @GET
    @Path("/pulloverpasstodb")
    @Produces(MediaType.APPLICATION_JSON)
    public Response pullOverpassToDb(@Context HttpServletRequest request,
            @QueryParam("bbox") String bbox) {

        Users user = Users.fromRequest(request);

        String jobId = UUID.randomUUID().toString().replace("-", "");
        String mapSuffix = jobId.substring(0, 7);
        String mapName = SECONDARY + "_" + mapSuffix;
        String folderName = "grail_" + bbox.replace(",", "_");

        Response response;
        JSONObject json = new JSONObject();
        json.put("jobid", jobId);

        List<Command> workflow = new LinkedList<>();


        // Create the folder if it doesn't exist
        Long folderId = DbUtils.createFolder(folderName, 0L, user.getId(), false);

        // Write the data to the hoot db
        GrailParams params = new GrailParams();
        params.setUser(user);
        String url = "'"
                + PUBLIC_OVERPASS_URL
                + "/api/interpreter?data=[out:json];(node("
                + new BoundingBox(bbox).toOverpassString()
                + ");<;>;);out%20meta%20qt;"
                + "'";
        params.setInput1(url);
        params.setOutput(mapName);
        ExternalCommand importOverpass = grailCommandFactory.build(jobId, params, "info", PushToDbCommand.class, this.getClass());
        workflow.add(importOverpass);

        // Move the data to the folder
        InternalCommand setFolder = updateParentCommandFactory.build(jobId, folderId, mapName, user, this.getClass());
        workflow.add(setFolder);

        jobProcessor.submitAsync(new Job(jobId, user.getId(), workflow.toArray(new Command[workflow.size()]), JobType.IMPORT));

        ResponseBuilder responseBuilder = Response.ok(json.toJSONString());
        response = responseBuilder.build();

        return response;
    }

    /**
     * Pull the remote data for a bounding box
     * from Rails Port API
     * and write to a Hoot map dataset
     *
     * The data from the Rails Port will be
     * named the reference layer for conflation
     *
     * The purpose of conflating these datasets is to update
     * a private OSM instance that has diverged from the public OSM
     * with private changes.
     *
     * @param bbox The bounding box
     *
     * @return Job ID
     * used by the client for polling job status
     * Internally, this provides the map dataset name suffix
     */
    @GET
    @Path("/pullrailsporttodb")
    @Produces(MediaType.APPLICATION_JSON)
    public Response pullRailsPortToDb(@Context HttpServletRequest request,
            @QueryParam("bbox") String bbox) {

        Users user = Users.fromRequest(request);

        String jobId = UUID.randomUUID().toString().replace("-", "");
        File workDir = new File(TEMP_OUTPUT_PATH, "grail_" + jobId);
        String mapSuffix = jobId.substring(0, 7);
        String mapName = REFERENCE + "_" + mapSuffix;
        String folderName = "grail_" + bbox.replace(",", "_");

        Response response;
        JSONObject json = new JSONObject();
        json.put("jobid", jobId);

        List<Command> workflow = new LinkedList<>();

        // Pull data from the reference OSM API
        // Until hoot can read API url directly, download to file first
        File referenceOSMFile = new File(workDir, REFERENCE +".osm");
        if (referenceOSMFile.exists()) { referenceOSMFile.delete(); }

        try {
            workflow.add(getRailsPortApiCommand(jobId, user, bbox, referenceOSMFile.getAbsolutePath()));
        } catch (UnavailableException ex) {
            return Response.status(Response.Status.SERVICE_UNAVAILABLE).entity(ex.getMessage()).build();
        }

        // Write the data to the hoot db
        GrailParams params = new GrailParams();
        params.setUser(user);
//        String url = RAILSPORT_PULL_URL +
//                "/mapfull?bbox=" + new BoundingBox(bbox).toServicesString();
//        params.setInput1(url);
        params.setInput1(referenceOSMFile.getAbsolutePath());
        params.setWorkDir(workDir);
        params.setOutput(mapName);
        ExternalCommand importRailsPort = grailCommandFactory.build(jobId, params, "info", PushToDbCommand.class, this.getClass());
        workflow.add(importRailsPort);

        // Set map tags marking dataset as eligible for derive changeset
        Map<String, String> tags = new HashMap<>();
        tags.put("source", "rails");
        tags.put("bbox", bbox);
        InternalCommand setMapTags = setMapTagsCommandFactory.build(tags, jobId);
        workflow.add(setMapTags);

        // Create the folder if it doesn't exist
        Long folderId = DbUtils.createFolder(folderName, 0L, user.getId(), false);

        // Move the data to the folder
        InternalCommand setFolder = updateParentCommandFactory.build(jobId, folderId, mapName, user, this.getClass());
        workflow.add(setFolder);

        jobProcessor.submitAsync(new Job(jobId, user.getId(), workflow.toArray(new Command[workflow.size()]), JobType.IMPORT));

        ResponseBuilder responseBuilder = Response.ok(json.toJSONString());
        response = responseBuilder.build();

        return response;
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
