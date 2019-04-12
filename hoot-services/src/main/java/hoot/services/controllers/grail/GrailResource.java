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
import static hoot.services.HootProperties.MAIN_OVERPASS_URL;
import static hoot.services.HootProperties.RAILSPORT_CAPABILITIES_URL;
import static hoot.services.HootProperties.RAILSPORT_PULL_URL;
import static hoot.services.HootProperties.RAILSPORT_PUSH_URL;
import static hoot.services.HootProperties.TEMP_OUTPUT_PATH;
import static hoot.services.HootProperties.replaceSensitiveData;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.URL;
import java.util.LinkedList;
import java.util.List;
import java.util.UUID;

import javax.servlet.http.HttpServletRequest;
import javax.ws.rs.Consumes;
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
import javax.xml.transform.TransformerException;

import org.apache.commons.io.FileUtils;
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
import hoot.services.job.Job;
import hoot.services.job.JobProcessor;
import hoot.services.job.JobType;
import hoot.services.models.db.Users;
import hoot.services.utils.XmlDocumentBuilder;


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

    @Autowired
    private OAuthRestTemplate oauthRestTemplate;

    public GrailResource() {}

    /**
     * Pull the OSM and Mapedit data for a bounding box and run differential on it
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

        String mainJobId = "grail_" + UUID.randomUUID().toString().replace("-", "");

        File workDir = new File(TEMP_OUTPUT_PATH, mainJobId);
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
        jobInfo.put("jobid", mainJobId);

        GrailParams params = new GrailParams();
        params.setBounds(bbox);
        params.setUser(user);

        APICapabilities railsPortCapabilities = getCapabilities(RAILSPORT_CAPABILITIES_URL);
        logger.info("differentialStats: railsPortAPI status = " + railsPortCapabilities.getApiStatus());
        if (railsPortCapabilities.getApiStatus() == null | railsPortCapabilities.getApiStatus().equals("offline")) {
            return Response.status(Response.Status.SERVICE_UNAVAILABLE).entity("The dest OSM API server is offline. Try again later").build();
        }

        // Pull OSM data from the dest OSM API Db
        File destOSMFile = new File(workDir, "dest.osm");
        if (destOSMFile.exists()) destOSMFile.delete();

        GrailParams sourceParams = new GrailParams();
        sourceParams.setUser(user);
        sourceParams.setBounds(bbox);
        sourceParams.setMaxBBoxSize(railsPortCapabilities.getMaxArea());
        sourceParams.setOutput(destOSMFile.getAbsolutePath());
        sourceParams.setPullUrl(RAILSPORT_PULL_URL);

        String jobId = "grail_" + UUID.randomUUID().toString().replace("-", "");
        jobInfo.put("jobid:DestOSM", jobId);
        // ExternalCommand getDestOSM = grailCommandFactory.build(jobId,params,debugLevel,PullOSMDataCommand.class,this.getClass());
        InternalCommand getDestOSM = apiCommandFactory.build(jobId, sourceParams, this.getClass());
        workflow.add(getDestOSM);


        // Pull OSM data from the source OSM Db using overpass
        File sourceOSMFile = new File(workDir, "source.osm");
        if (sourceOSMFile.exists()) sourceOSMFile.delete();

        GrailParams overpassParams = new GrailParams();
        overpassParams.setUser(user);
        overpassParams.setBounds(bbox);
        overpassParams.setMaxBBoxSize(railsPortCapabilities.getMaxArea());
        overpassParams.setOutput(sourceOSMFile.getAbsolutePath());
        overpassParams.setPullUrl(MAIN_OVERPASS_URL);

        jobId = "grail_" + UUID.randomUUID().toString().replace("-", "");
        jobInfo.put("jobid:SourceOSM", jobId);
        InternalCommand getOverpassOSM = overpassCommandFactory.build(jobId, overpassParams, this.getClass());
        workflow.add(getOverpassOSM);

        // Run the diff command.
        params.setInput1(destOSMFile.getAbsolutePath());
        params.setInput2(sourceOSMFile.getAbsolutePath());

        File geomDiffFile = new File(workDir, "diff.osc");
        if (geomDiffFile.exists()) geomDiffFile.delete();

        try {
            geomDiffFile.createNewFile();
        }
        catch(IOException exc) {
            logger.error("createDifferential: Error creating folder: {} ", geomDiffFile.getAbsolutePath(), exc);
            return Response.status(Response.Status.INTERNAL_SERVER_ERROR).entity(exc.getMessage()).build();
        }

        params.setOutput(geomDiffFile.getAbsolutePath());

        ExternalCommand makeDiff = grailCommandFactory.build(mainJobId, params, debugLevel, RunDiffCommand.class, this.getClass());
        workflow.add(makeDiff);

        jobProcessor.submitAsync(new Job(mainJobId, user.getId(), workflow.toArray(new Command[workflow.size()]), JobType.DERIVE_CHANGESET));

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
    @Path("/differentialstats/{jobId}")
    @Produces(MediaType.APPLICATION_JSON)
    public Response differentialStats(@Context HttpServletRequest request,
            @PathParam("jobId") String jobDir,
            @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {

        JSONObject jobInfo = new JSONObject();

        String fileDirectory = TEMP_OUTPUT_PATH + "/" + jobDir + "/diff.osc";
        File workDir = new File(fileDirectory);

        if(!workDir.exists()) {
            String message = "Could not find diff.osc file to calculate diff stats from.";
            logger.error("differentialStats: Error finding diff file: {} ", workDir.getAbsolutePath());
            return Response.status(Response.Status.NOT_FOUND).entity(message).build();
        }

        try {
            String xmlData = FileUtils.readFileToString(workDir, "UTF-8");
            Document changesetDiffDoc = XmlDocumentBuilder.parse(xmlData);
            logger.debug("Parsing changeset diff XML: {}", StringUtils.abbreviate(xmlData, 1000));

            NodeList elementXmlNodes = XPathAPI.selectNodeList(changesetDiffDoc, "//osmChange/create/node");
            jobInfo.put("nodeCount", elementXmlNodes.getLength());

            elementXmlNodes = XPathAPI.selectNodeList(changesetDiffDoc, "//osmChange/create/way");
            jobInfo.put("wayCount", elementXmlNodes.getLength());

            elementXmlNodes = XPathAPI.selectNodeList(changesetDiffDoc, "//osmChange/create/relation");
            jobInfo.put("relationCount", elementXmlNodes.getLength());

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
     * Pushes the specified differential to MapEdit
     *
     * Takes in a json object
     * POST hoot-services/grail/differentialpush
     *
     * {
     *   //the job id. We make the directory for the differential with the name of the jobid
     *   "folder" : "grail_8e8c4681-ead9-4c27-aa2e-80746d627523",
     *
     *   //boolean if the user wants to apply the tags when pushing to mapedit
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
            if (railsPortCapabilities.getApiStatus() == null | railsPortCapabilities.getApiStatus().equals("offline")) {
                return Response.status(Response.Status.SERVICE_UNAVAILABLE).entity("The dest OSM API server is offline. Try again later").build();
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
                    String msg = "Error during differential push! Could not find differential file ";
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
     * Runs changeset-derive and pushes the result to MapEdit
     *
     * Takes in a json object
     * POST hoot-services/grail/conflatepush
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
    @Path("/conflatepush")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response conflatePush(@Context HttpServletRequest request,
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
            logger.error("conflatePush: Error creating folder: {} ", workDir.getAbsolutePath(), ioe);
            return Response.status(Response.Status.INTERNAL_SERVER_ERROR).entity(ioe.getMessage()).build();
        }

        GrailParams params = new GrailParams();
        params.setUser(user);

        try {

            ProtectedResourceDetails oauthInfo = oauthRestTemplate.getResource();
            params.setConsumerKey(oauthInfo.getConsumerKey());
            params.setConsumerSecret(((SharedConsumerSecret) oauthInfo.getSharedSecret()).getConsumerSecret());

            // Run changeset-derive
            params.setInput1(HOOTAPI_DB_URL + "/" + input1);
            params.setInput2(HOOTAPI_DB_URL + "/" + input2);

            File changeSet = new File(workDir, "diff.osc");
            if (changeSet.exists()) { changeSet.delete(); }
            params.setOutput(changeSet.getAbsolutePath());
            ExternalCommand makeChangeset = grailCommandFactory.build(mainJobId, params, debugLevel, DeriveChangesetCommand.class, this.getClass());
            workflow.add(makeChangeset);

            // Apply changeset
            params.setPushUrl(RAILSPORT_PUSH_URL);
            ExternalCommand applyChange = grailCommandFactory.build(mainJobId, params, debugLevel, ApplyChangesetCommand.class, this.getClass());
            workflow.add(applyChange);

            // Now roll the dice and run everything.....
            jobProcessor.submitAsync(new Job(mainJobId, user.getId(), workflow.toArray(new Command[workflow.size()]), JobType.UPLOAD_CHANGESET));
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
     * @param
     *
     * @return Job ID
     *            Internally, this is the directory that the files are kept in
     */
    @POST
    @Path("/pullosmtodb")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response pullOsmToDb(@Context HttpServletRequest request,
            GrailParams reqParams,
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
            return Response.status(Response.Status.SERVICE_UNAVAILABLE).entity("The dest OSM API server is offline. Try again later").build();
        }

        GrailParams apiParams = new GrailParams();
        GrailParams overpassParams = new GrailParams();

        apiParams.setUser(user);
        apiParams.setBounds(reqParams.getBounds());
        apiParams.setMaxBBoxSize(railsPortCapabilities.getMaxArea());
        apiParams.setPullUrl(RAILSPORT_PULL_URL);

        overpassParams.setUser(user);
        overpassParams.setBounds(reqParams.getBounds());
        overpassParams.setMaxBBoxSize(railsPortCapabilities.getMaxArea());
        overpassParams.setPullUrl(MAIN_OVERPASS_URL);

        List<Command> workflow = new LinkedList<>();

        try {
            // Pull data from the dest OSM API Db
            File destOSMFile = new File(workDir, "dest.osm");
            if (destOSMFile.exists()) { destOSMFile.delete(); }

            apiParams.setOutput(destOSMFile.getAbsolutePath());
            // ExternalCommand getDestOSM = grailCommandFactory.build(jobId,params,debugLevel,PullOSMDataCommand.class,this.getClass());
            InternalCommand getDestOSM = apiCommandFactory.build(jobId, apiParams, this.getClass());
            workflow.add(getDestOSM);

            // Pull OSM data from the real source OSM Db using overpass
            File sourceOSMFile = new File(workDir, "source.osm");
            if (sourceOSMFile.exists()) { sourceOSMFile.delete(); }

            overpassParams.setOutput(sourceOSMFile.getAbsolutePath());

            InternalCommand getOverpassOSM = overpassCommandFactory.build(jobId, overpassParams, this.getClass());
            workflow.add(getOverpassOSM);

            // Now we paste in the "pushtodb"
            // the first 10 digits of a random UUID _should_ be unique....
            String randomString = "_" + StringUtils.left(UUID.randomUUID().toString().replace("-", ""), 10);
            String destDbFile = "dest" + randomString;
            String sourceDbFile = "source" + randomString;

            GrailParams apiPushParams = new GrailParams();
            GrailParams overpassPushParams = new GrailParams();
            GrailParams linkParams = new GrailParams();

            apiPushParams.setUser(user);
            overpassPushParams.setUser(user);
            linkParams.setUser(user);

            // We could use the existing Import Command to push the OSM files to the DB BUT it will delete the import directory
            // Till I figure out a better way to do this, we will use our version.
            apiPushParams.setInput1(destOSMFile.getAbsolutePath());
            apiPushParams.setOutput(destDbFile);
            ExternalCommand pushApi = grailCommandFactory.build(jobId, apiPushParams, debugLevel, PushToDbCommand.class, this.getClass());
            workflow.add(pushApi);

            overpassPushParams.setInput1(sourceOSMFile.getAbsolutePath());
            overpassPushParams.setOutput(sourceDbFile);
            ExternalCommand pushOverpass = grailCommandFactory.build(jobId, overpassPushParams, debugLevel, PushToDbCommand.class, this.getClass());
            workflow.add(pushOverpass);

            // Now create a folder and link the uploaded layers to it
            linkParams.setFolder(jobId);
            linkParams.setInput1(destDbFile);
            linkParams.setInput2(sourceDbFile);
            InternalCommand updateDb = updateDbCommandFactory.build(jobId, linkParams, this.getClass());
            workflow.add(updateDb);

            jobProcessor.submitAsync(new Job(jobId, user.getId(), workflow.toArray(new Command[workflow.size()]), JobType.IMPORT));

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
