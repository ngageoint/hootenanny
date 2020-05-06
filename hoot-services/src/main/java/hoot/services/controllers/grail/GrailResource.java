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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.grail;

import static hoot.services.HootProperties.CHANGESETS_FOLDER;
import static hoot.services.HootProperties.CHANGESET_OPTIONS;
import static hoot.services.HootProperties.GRAIL_OVERPASS_LABEL;
import static hoot.services.HootProperties.GRAIL_OVERPASS_STATS_QUERY;
import static hoot.services.HootProperties.GRAIL_RAILS_LABEL;
import static hoot.services.HootProperties.HOME_FOLDER;
import static hoot.services.HootProperties.HOOTAPI_DB_URL;
import static hoot.services.HootProperties.MAX_OVERPASS_FEATURE_COUNT;
import static hoot.services.HootProperties.PRIVATE_OVERPASS_CERT_PATH;
import static hoot.services.HootProperties.PRIVATE_OVERPASS_URL;
import static hoot.services.HootProperties.PUBLIC_OVERPASS_URL;
import static hoot.services.HootProperties.RAILSPORT_CAPABILITIES_URL;
import static hoot.services.HootProperties.RAILSPORT_PULL_URL;
import static hoot.services.HootProperties.RAILSPORT_PUSH_URL;
import static hoot.services.HootProperties.TEMP_OUTPUT_PATH;
import static hoot.services.HootProperties.replaceSensitiveData;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.UUID;

import javax.net.ssl.HostnameVerifier;
import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.SSLSession;
import javax.servlet.UnavailableException;
import javax.servlet.http.HttpServletRequest;
import javax.ws.rs.BadRequestException;
import javax.ws.rs.Consumes;
import javax.ws.rs.DefaultValue;
import javax.ws.rs.ForbiddenException;
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

import org.apache.commons.io.FileUtils;
import org.apache.commons.io.IOUtils;
import org.apache.commons.io.filefilter.IOFileFilter;
import org.apache.commons.io.filefilter.RegexFileFilter;
import org.apache.commons.io.filefilter.WildcardFileFilter;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;
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
    private OAuthRestTemplate oauthRestTemplate;

    @Autowired
    private UpdateParentCommandFactory updateParentCommandFactory;

    @Autowired
    private PullConnectedWaysCommandFactory connectedWaysCommandFactory;

    public GrailResource() {}

    /**
     * If the PRIVATE_OVERPASS_URL variable is set to a value then return true
      */
    public static boolean isPrivateOverpassActive() {
        return !replaceSensitiveData(PRIVATE_OVERPASS_URL).equals(PRIVATE_OVERPASS_URL);
    }

    private Command getRailsPortApiCommand(String jobId, GrailParams params) throws UnavailableException {
        // Checks to see that the sensitive data was actually replaced meaning there was a value
        if (isPrivateOverpassActive()) {
            params.setPullUrl(PRIVATE_OVERPASS_URL);
        } else {
            APICapabilities railsPortCapabilities = getCapabilities(RAILSPORT_CAPABILITIES_URL);
            if (railsPortCapabilities.getApiStatus() == null
                    || railsPortCapabilities.getApiStatus().equals("offline")) {
                throw new UnavailableException("The Rails port API is offline.");
            }

            params.setMaxBBoxSize(railsPortCapabilities.getMaxArea());
            params.setPullUrl(RAILSPORT_PULL_URL);
        }

        InternalCommand command = apiCommandFactory.build(jobId, params, this.getClass());
        return command;
    }

    private Command getConnectedWaysApiCommand(String jobId, GrailParams params) throws UnavailableException {
        params.setPullUrl(RAILSPORT_PULL_URL);

        InternalCommand command = connectedWaysCommandFactory.build(jobId, params, this.getClass());
        return command;
    }

    private Command getPublicOverpassCommand(String jobId, GrailParams params) {
        params.setPullUrl(PUBLIC_OVERPASS_URL);

        InternalCommand command = overpassCommandFactory.build(jobId, params, this.getClass());
        return command;
    }

    /**
     * Pull the Public Overpass and Private Rails Port data for a bounding box and run differential on it
     *
     * Takes in a json object
     * POST hoot-services/grail/createdifferentialchangeset
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
    @Path("/createdifferentialchangeset")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response createDifferentialChangeset(@Context HttpServletRequest request,
            GrailParams reqParams,
            @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {

        Users user = Users.fromRequest(request);
        advancedUserCheck(user);
        reqParams.setUser(user);

        String jobId = "grail_" + UUID.randomUUID().toString().replace("-", "");

        File workDir = new File(CHANGESETS_FOLDER, jobId);
        try {
            FileUtils.forceMkdir(workDir);
        }
        catch (IOException ioe) {
            logger.error("createDifferentialChangeset: Error creating folder: {} ", workDir.getAbsolutePath(), ioe);
            return Response.status(Response.Status.INTERNAL_SERVER_ERROR).entity(ioe.getMessage()).build();
        }

        List<Command> workflow = new LinkedList<>();

        JSONObject jobInfo = new JSONObject();
        jobInfo.put("jobid", jobId);

        // Pull reference data from Rails port OSM API
        File referenceOSMFile = new File(workDir, REFERENCE + ".osm");
        GrailParams getRailsParams = new GrailParams(reqParams);
        getRailsParams.setOutput(referenceOSMFile.getAbsolutePath());

        if (referenceOSMFile.exists()) referenceOSMFile.delete();
        try {
            workflow.add(getRailsPortApiCommand(jobId, getRailsParams));
        } catch (UnavailableException ex) {
            return Response.status(Response.Status.SERVICE_UNAVAILABLE).entity(ex.getMessage()).build();
        }

        // Pull secondary data from the Public Overpass API
        File secondaryOSMFile = new File(workDir, SECONDARY + ".osm");
        GrailParams getOverpassParams = new GrailParams(reqParams);
        getOverpassParams.setOutput(secondaryOSMFile.getAbsolutePath());

        if (secondaryOSMFile.exists()) secondaryOSMFile.delete();
        workflow.add(getPublicOverpassCommand(jobId, getOverpassParams));

        // Run the differential conflate command.
        GrailParams params = new GrailParams(reqParams);
        params.setUser(user);
        params.setWorkDir(workDir);
        params.setInput1(referenceOSMFile.getAbsolutePath());
        params.setInput2(secondaryOSMFile.getAbsolutePath());

        File geomDiffFile = new File(workDir, "diff.osc");
        if (geomDiffFile.exists()) geomDiffFile.delete();

        try {
            geomDiffFile.createNewFile();
        }
        catch(IOException exc) {
            logger.error("createDifferentialChangeset: Error creating file: {} ", geomDiffFile.getAbsolutePath(), exc);
            return Response.status(Response.Status.INTERNAL_SERVER_ERROR).entity(exc.getMessage()).build();
        }

        params.setOutput(geomDiffFile.getAbsolutePath());

        ExternalCommand makeDiff = grailCommandFactory.build(jobId, params, debugLevel, RunDiffCommand.class, this.getClass());
        workflow.add(makeDiff);

        jobProcessor.submitAsync(new Job(jobId, user.getId(), workflow.toArray(new Command[workflow.size()]), JobType.DERIVE_CHANGESET));

        return Response.ok(jobInfo.toJSONString()).build();
    }

    /**
     * Retrieve statistics on the specified changeset
     *
     * GET hoot-services/grail/changesetstats/{jobId}
     *
     * @param jobDir
     *      Internally, this is the directory that the files are kept in.
     *
     * @param debugLevel
     *      debug level
     *
     * @return JSON object containing info about the changeset such as
     *      the count of new nodes, ways, and relations
     */
    @GET
    @Path("/changesetstats")
    @Produces(MediaType.APPLICATION_JSON)
    public Response changesetStats(@Context HttpServletRequest request,
            @QueryParam("jobId") String jobDir,
            @QueryParam("includeTags") Boolean includeTags,
            @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {

        Users user = Users.fromRequest(request);
        advancedUserCheck(user);

        JSONObject jobInfo = new JSONObject();

        String fileDirectory = CHANGESETS_FOLDER + "/" + jobDir;
        File workDir = new File(fileDirectory);
        if (!workDir.exists()) {
            logger.error("changesetStats: jobDir {} does not exist.", workDir.getAbsolutePath());
            return Response.status(Response.Status.BAD_REQUEST).entity("Job " + jobDir + " does not exist.").build();
        }

        try {
            IOFileFilter fileFilter;
            if (includeTags) {
                fileFilter = new WildcardFileFilter("*.json");
            } else {
                fileFilter = new RegexFileFilter("^(?!.*\\.tags\\.).*json$");
            }

            List<File> statFilesList = (List<File>) FileUtils.listFiles(workDir, fileFilter, null);

            // loop through each stats file based on the fileFilter. Should at most be 2 files, normals stats and if requested the tags stats.
            for (File currentOsc : statFilesList) {
                JSONParser parser = new JSONParser();
                String jsonDoc = FileUtils.readFileToString(currentOsc, "UTF-8");
                JSONObject statsJson = (JSONObject) parser.parse(jsonDoc);

                // loop for the change type name
                for (Object changeTypeKey: statsJson.keySet()) {
                    String changeTypeName = changeTypeKey.toString();
                    JSONArray valuesArray = (JSONArray) statsJson.get(changeTypeKey);

                    // loop for the element types
                    for (Object obj: valuesArray) {
                        JSONObject valueObject = (JSONObject) obj;
                        // will always be an object with single key -> value
                        String elementTypeName = valueObject.keySet().iterator().next().toString();
                        int elementTypeCount = Integer.parseInt(valueObject.values().iterator().next().toString());

                        String key = changeTypeName.toLowerCase() + "-" + elementTypeName.toLowerCase();
                        int count = jobInfo.get(key) == null ? elementTypeCount : (int) jobInfo.get(key) + elementTypeCount;

                        jobInfo.put(key, count);
                    }
                }
            }

            File tagDiffFile = new File(workDir, "diff.tags.osc");
            jobInfo.put("hasTags", tagDiffFile.exists());
        }
        catch (IOException exc) {
            throw new WebApplicationException(exc, Response.serverError().entity("Changeset file not found.").build());
        }
        catch (ParseException exc) {
            throw new WebApplicationException(exc, Response.serverError().entity("Changeset file is malformed.").build());
        }

        return Response.ok(jobInfo.toJSONString()).build();
    }

    /**
     * Pushes the specified changeset back to the
     * reference dataset API
     *
     * Takes in a json object
     * POST hoot-services/grail/changesetpush
     *
     * {
     *   //the job id. We make the directory for the changeset with the name of the jobid
     *   "folder" : "grail_8e8c4681-ead9-4c27-aa2e-80746d627523",
     *
     *   //boolean if the user wants to apply the tags when pushing to reference API
     *   "APPLY_TAGS" : true
     *   //this is only applicable if the changeset is the result of "create differential changeset"
     * }
     *
     * @param reqParams
     *      JSON input params; see description above
     *
     * @param debugLevel
     *      debug level
     *
     * @return Job ID. Can be used to check status of the changeset push
     */
    @POST
    @Path("/changesetpush")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response changesetPush(@Context HttpServletRequest request,
            GrailParams reqParams,
            @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {

        Users user = Users.fromRequest(request);
        advancedUserCheck(user);

        GrailParams params = new GrailParams(reqParams);
        params.setUser(user);
        params.setPushUrl(RAILSPORT_PUSH_URL);

        ProtectedResourceDetails oauthInfo = oauthRestTemplate.getResource();
        params.setConsumerKey(oauthInfo.getConsumerKey());
        params.setConsumerSecret(((SharedConsumerSecret) oauthInfo.getSharedSecret()).getConsumerSecret());

        String jobId = "grail_" + UUID.randomUUID().toString().replace("-", "");

        JSONObject json = new JSONObject();
        json.put("jobid", jobId);
        String jobDir = reqParams.getParentId();
        File workDir = new File(CHANGESETS_FOLDER, jobDir);
        params.setWorkDir(workDir);

        if (!workDir.exists()) {
            logger.error("ApplyChangeset: jobDir {} does not exist.", workDir.getAbsolutePath());
            return Response.status(Response.Status.BAD_REQUEST).entity("Job " + jobDir + " does not exist.").build();
        }

        try {
            APICapabilities railsPortCapabilities = getCapabilities(RAILSPORT_CAPABILITIES_URL);
            logger.info("ApplyChangeset: railsPortAPI status = " + railsPortCapabilities.getApiStatus());
            if (railsPortCapabilities.getApiStatus() == null || railsPortCapabilities.getApiStatus().equals("offline")) {
                return Response.status(Response.Status.SERVICE_UNAVAILABLE).entity("The reference OSM API server is offline.").build();
            }

            File changesetFile = new File(workDir, "diff.osc");
            List<Command> workflow = new LinkedList<>();

            if (changesetFile.exists()) {
                params.setOutput(changesetFile.getAbsolutePath());

                ExternalCommand applyGeomChange = grailCommandFactory.build(jobId, params, debugLevel, ApplyChangesetCommand.class, this.getClass());
                workflow.add(applyGeomChange);
            }
            else {
                String msg = "Error during changeset push! Could not find osc file ";
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
                    String msg = "Error during changset push! Could not find osc tags file ";
                    throw new WebApplicationException(new FileNotFoundException(), Response.serverError().entity(msg).build());
                }
            }

            Map<String, Object> jobStatusTags = new HashMap<>();
            jobStatusTags.put("bbox", reqParams.getBounds());
            jobStatusTags.put("parentId", reqParams.getParentId());

            jobProcessor.submitAsync(new Job(jobId, user.getId(), workflow.toArray(new Command[workflow.size()]), JobType.UPLOAD_CHANGESET, jobStatusTags));
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (IllegalArgumentException iae) {
            throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
        }
        catch (Exception e) {
            String msg = "Error during changeset push! Params: " + params;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return Response.ok(json.toJSONString()).build();
    }

    /**
     * Runs changeset-derive on the two input layers
     *
     * Takes in a json object
     * POST hoot-services/grail/derivechangeset
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
    @Path("/derivechangeset")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response deriveChangeset(@Context HttpServletRequest request,
            GrailParams reqParams,
            @QueryParam("replacement") @DefaultValue("false") Boolean replacement,
            @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {

        Users user = Users.fromRequest(request);
        advancedUserCheck(user);

        String input1 = reqParams.getInput1();
        String input2 = reqParams.getInput2();

        JSONObject json = new JSONObject();
        String mainJobId = "grail_" + UUID.randomUUID().toString().replace("-", "");
        json.put("jobid", mainJobId);

        List<Command> workflow = new LinkedList<>();

        File workDir = new File(CHANGESETS_FOLDER, mainJobId);
        try {
            FileUtils.forceMkdir(workDir);
        }
        catch (IOException ioe) {
            logger.error("deriveChangeset: Error creating folder: {} ", workDir.getAbsolutePath(), ioe);
            return Response.status(Response.Status.INTERNAL_SERVER_ERROR).entity(ioe.getMessage()).build();
        }

        GrailParams params = new GrailParams(reqParams);
        params.setUser(user);
        params.setWorkDir(workDir);

        try {
            params.setInput1(HOOTAPI_DB_URL + "/" + input1);

            //If not passed INPUT2 assume an adds only changeset using one input
            if(params.getInput2() != null) {
                params.setInput2(HOOTAPI_DB_URL + "/" + input2);
                params.setConflationType(DbUtils.getConflationType(Long.parseLong(input2)));
            } else {
                params.setInput2("\"\"");
            }

            File changeSet = new File(workDir, "diff.osc");
            if (changeSet.exists()) { changeSet.delete(); }

            params.setOutput(changeSet.getAbsolutePath());
            // Run changeset-derive
            ExternalCommand makeChangeset = grailCommandFactory.build(mainJobId, params, debugLevel, (replacement) ? DeriveChangesetReplacementCommand.class : DeriveChangesetCommand.class, this.getClass());
            workflow.add(makeChangeset);

            Map<String, Object> jobStatusTags = new HashMap<>();
            jobStatusTags.put("bbox", reqParams.getBounds());
            jobStatusTags.put("input1", input1);
            jobStatusTags.put("input2", input2);
            jobStatusTags.put("parentId", reqParams.getParentId());

            jobProcessor.submitAsync(new Job(mainJobId, user.getId(), workflow.toArray(new Command[workflow.size()]), JobType.DERIVE_CHANGESET, jobStatusTags));
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (IllegalArgumentException iae) {
            throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
        }
        catch (Exception e) {
            String msg = "Error during derive changeset! Params: " + params;
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
     * @param reqParams Contains info such as bbox, layerName, and if the
     * user provided one, a custom query for pulling overpass data
     *
     * @return Job ID
     * used by the client for polling job status
     * Internally, this provides the map dataset name suffix
     */
    @POST
    @Path("/pulloverpasstodb")
    @Produces(MediaType.APPLICATION_JSON)
    public Response pullOverpassToDb(@Context HttpServletRequest request,
            @QueryParam("folderId") Long folderId,
            GrailParams reqParams) {

        Users user = Users.fromRequest(request);
        advancedUserCheck(user);

        String bbox = reqParams.getBounds();
        String layerName = reqParams.getInput1();
        String jobId = UUID.randomUUID().toString().replace("-", "");
        File workDir = new File(TEMP_OUTPUT_PATH, "grail_" + jobId);

        if (DbUtils.mapExists(layerName)) {
            throw new BadRequestException("Record with name : " + layerName + " already exists.  Please try a different name.");
        }

        Response response;
        JSONObject json = new JSONObject();
        json.put("jobid", jobId);

        List<Command> workflow = new LinkedList<>();

        // Write the data to the hoot db
        GrailParams params = new GrailParams(reqParams);
        params.setUser(user);
        params.setPullUrl(PUBLIC_OVERPASS_URL);

        String url;
        try {
            String customQuery = reqParams.getCustomQuery();
            if (customQuery == null || customQuery.equals("")) {
                url = "'" + PullOverpassCommand.getOverpassUrl(bbox) + "'";
            } else {
                url = "'" + PullOverpassCommand.getOverpassUrl(replaceSensitiveData(params.getPullUrl()), bbox, "xml", customQuery) + "'";
            }

        } catch(IllegalArgumentException exc) {
            return Response.status(Response.Status.BAD_REQUEST).entity(exc.getMessage()).build();
        }


        File overpassOSMFile = new File(workDir, SECONDARY + ".osm");
        GrailParams getOverpassParams = new GrailParams(params);
        getOverpassParams.setOutput(overpassOSMFile.getAbsolutePath());
        if (overpassOSMFile.exists()) overpassOSMFile.delete();
        workflow.add(getPublicOverpassCommand(jobId, getOverpassParams));


        params.setInput1(overpassOSMFile.getAbsolutePath());
        params.setOutput(layerName);
        ExternalCommand importOverpass = grailCommandFactory.build(jobId, params, "info", PushToDbCommand.class, this.getClass());
        workflow.add(importOverpass);

        // Set map tags marking dataset as eligible for derive changeset
        Map<String, String> tags = new HashMap<>();
        tags.put("bbox", params.getBounds());
        InternalCommand setMapTags = setMapTagsCommandFactory.build(tags, jobId);
        workflow.add(setMapTags);

        // Move the data to the folder
        InternalCommand setFolder = updateParentCommandFactory.build(jobId, folderId, layerName, user, this.getClass());
        workflow.add(setFolder);

        Map<String, Object> jobStatusTags = new HashMap<>();
        jobStatusTags.put("bbox", bbox);

        jobProcessor.submitAsync(new Job(jobId, user.getId(), workflow.toArray(new Command[workflow.size()]), JobType.IMPORT, jobStatusTags));

        ResponseBuilder responseBuilder = Response.ok(json.toJSONString());
        response = responseBuilder.build();

        return response;
    }

    @GET
    @Path("/grailMetadataQuery")
    @Produces(MediaType.APPLICATION_JSON)
    public Response grailMetadata(@Context HttpServletRequest request) {

        Users user = Users.fromRequest(request);
        advancedUserCheck(user);

        String railsLabel = GRAIL_RAILS_LABEL;
        String overpassLabel = GRAIL_OVERPASS_LABEL;

        JSONObject jobInfo = new JSONObject();
        jobInfo.put("maxFeatureCount", MAX_OVERPASS_FEATURE_COUNT);
        jobInfo.put("railsLabel", railsLabel);
        jobInfo.put("overpassLabel", overpassLabel);

        return Response.ok(jobInfo.toJSONString()).build();
    }

    @POST
    @Path("/overpassStats")
    @Produces(MediaType.APPLICATION_JSON)
    public Response overpassStats(@Context HttpServletRequest request,
            GrailParams reqParams) {

        Users user = Users.fromRequest(request);
        advancedUserCheck(user);

        String customQuery = reqParams.getCustomQuery();

        // Get grail overpass query from the file and store it in a string
        String overpassQuery;
        if (customQuery == null || customQuery.equals("")) {
            File overpassQueryFile = new File(HOME_FOLDER, GRAIL_OVERPASS_STATS_QUERY);
            try {
                overpassQuery = FileUtils.readFileToString(overpassQueryFile, "UTF-8");
            } catch(Exception exc) {
                String msg = "Failed to poll overpass for stats query. Couldn't read overpass query file: " + overpassQueryFile.getName();
                throw new WebApplicationException(exc, Response.serverError().entity(msg).build());
            }
        } else {
            overpassQuery = customQuery;

            if (overpassQuery.contains("out:xml")) {
                overpassQuery = overpassQuery.replace("out:xml", "out:json");
            }

            // first line that lists columns which are counts for each feature type
            overpassQuery = overpassQuery.replace("[out:json]", "[out:csv(::count, ::\"count:nodes\", ::\"count:ways\", ::\"count:relations\")]");
        }

        // append the connected ways query
        overpassQuery = PullApiCommand.connectedWaysQuery(overpassQuery);

        // overpass query can have multiple "out *" lines so need to replace all
        overpassQuery = overpassQuery.replaceAll("out [\\s\\w]+;", "out count;");

        //replace the {{bbox}} from the overpass query with the actual coordinates and encode the query
        overpassQuery = overpassQuery.replace("{{bbox}}", new BoundingBox(reqParams.getBounds()).toOverpassString());
        try {
            overpassQuery = URLEncoder.encode(overpassQuery, "UTF-8").replace("+", "%20"); // need to encode url for the get
        } catch (UnsupportedEncodingException ignored) {} // Can be safely ignored because UTF-8 is always supported


        List<String> columns = new ArrayList<>();
        List<JSONObject> data = new ArrayList<>();
        JSONObject nodeObj = new JSONObject();
        nodeObj.put("label", "node");
        JSONObject wayObj = new JSONObject();
        wayObj.put("label", "way");
        JSONObject relationObj = new JSONObject();
        relationObj.put("label", "relation");
        JSONObject totalObj = new JSONObject();
        totalObj.put("label", "total");

        // Get public overpass data
        String publicUrl = replaceSensitiveData(PUBLIC_OVERPASS_URL) + "?data=" + overpassQuery;
        ArrayList<Double> publicStats = retrieveOverpassStats(publicUrl, false);
        if(publicStats.size() != 0) {
            columns.add(GRAIL_OVERPASS_LABEL);
            totalObj.put(GRAIL_OVERPASS_LABEL, publicStats.get(0));
            nodeObj.put(GRAIL_OVERPASS_LABEL, publicStats.get(1));
            wayObj.put(GRAIL_OVERPASS_LABEL, publicStats.get(2));
            relationObj.put(GRAIL_OVERPASS_LABEL, publicStats.get(3));
        }

        // Get private overpass data if private overpass url was provided
        if (!replaceSensitiveData(PRIVATE_OVERPASS_URL).equals(PRIVATE_OVERPASS_URL)) {
            String privateUrl = replaceSensitiveData(PRIVATE_OVERPASS_URL) + "?data=" + overpassQuery;
            ArrayList<Double> privateStats = retrieveOverpassStats(privateUrl, true);
            if(privateStats.size() != 0) {
                columns.add(GRAIL_RAILS_LABEL);
                totalObj.put(GRAIL_RAILS_LABEL, privateStats.get(0));
                nodeObj.put(GRAIL_RAILS_LABEL, privateStats.get(1));
                wayObj.put(GRAIL_RAILS_LABEL, privateStats.get(2));
                relationObj.put(GRAIL_RAILS_LABEL, privateStats.get(3));
            }
        }

        data.add(nodeObj);
        data.add(wayObj);
        data.add(relationObj);
        data.add(totalObj);
        /*
        * Example response object
            {
            columns: ["OSM", "NOME"],
            data: [
               {label: "node", NOME: 3, OSM: 5},
               {label: "way", NOME: 1, OSM: 2},
               {label: "relation", NOME: 0, OSM: 1},
               {label: "total", NOME: 4, OSM: 7}
            ]
            }
        *
        */
        JSONObject stats = new JSONObject();
        stats.put("columns", columns);
        stats.put("data", data);

        return Response.ok().entity(stats).build();
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
     * @param reqParams Contains info such as bbox, layerName, and if the
     * user provided one, a custom query for pulling overpass data
     *
     * @return Job ID
     * used by the client for polling job status
     * Internally, this provides the map dataset name suffix
     */
    @POST
    @Path("/pullrailsporttodb")
    @Produces(MediaType.APPLICATION_JSON)
    public Response pullRailsPortToDb(@Context HttpServletRequest request,
            @QueryParam("folderId") Long folderId,
            GrailParams reqParams) {

        Users user = Users.fromRequest(request);
        advancedUserCheck(user);

        String layerName = reqParams.getInput1();

        String jobId = UUID.randomUUID().toString().replace("-", "");
        File workDir = new File(TEMP_OUTPUT_PATH, "grail_" + jobId);

        JSONObject json = new JSONObject();
        json.put("jobid", jobId);

        GrailParams params = new GrailParams(reqParams);
        params.setUser(user);
        params.setWorkDir(workDir);
        params.setOutput(layerName);

        List<Command> workflow;
        try {
            workflow = setupRailsPull(jobId, params, folderId);
        }
        catch(UnavailableException exc) {
            return Response.status(Response.Status.SERVICE_UNAVAILABLE).entity(exc.getMessage()).build();
        }

        Map<String, Object> jobStatusTags = new HashMap<>();
        jobStatusTags.put("bbox", reqParams.getBounds());

        jobProcessor.submitAsync(new Job(jobId, user.getId(), workflow.toArray(new Command[workflow.size()]), JobType.IMPORT, jobStatusTags));

        return Response.ok(json.toJSONString()).build();
    }

    private List<Command> setupRailsPull(String jobId, GrailParams params, Long parentFolderId) throws UnavailableException {
        List<Command> workflow = new LinkedList<>();
        Users user = params.getUser();
        boolean usingPrivateOverpass = isPrivateOverpassActive();

        // Pull data from the reference OSM API or private Overpass API
        // Until hoot can read API url directly, download to file first
        File referenceOSMFile = new File(params.getWorkDir(), REFERENCE +".osm");
        if (referenceOSMFile.exists()) { referenceOSMFile.delete(); }

        GrailParams getRailsParams = new GrailParams(params);
        getRailsParams.setOutput(referenceOSMFile.getAbsolutePath());

        // have to add the query for getting connected ways before calling getRailsPortApiCommand
        if (usingPrivateOverpass) {
            String queryWithConnectedWays = PullApiCommand.connectedWaysQuery(getRailsParams.getCustomQuery());
            getRailsParams.setCustomQuery(queryWithConnectedWays);
        }

        try {
            workflow.add(getRailsPortApiCommand(jobId, getRailsParams));
        } catch (UnavailableException exc) {
            throw new UnavailableException("The Rails port API is offline.");
        }

        // if not using private overpass then this will be changed to the merge file
        File ingestFile = referenceOSMFile;

        // private overpass query result file should handle getting the connected ways so just use that as the ingest file
        if (!usingPrivateOverpass) {
            GrailParams connectedWaysParams = new GrailParams(params);
            connectedWaysParams.setInput1(referenceOSMFile.getAbsolutePath());
            File cropFile = new File(params.getWorkDir(), "crop.osm");
            connectedWaysParams.setOutput(cropFile.getAbsolutePath());
            // Do an invert crop of this data to get nodes outside bounds
            workflow.add(grailCommandFactory.build(jobId, connectedWaysParams, "info", InvertCropCommand.class, this.getClass()));

            //read node ids
            //pull connected ways
            //pull entire ways
            //remove cropfile
            workflow.add(getConnectedWaysApiCommand(jobId, connectedWaysParams));

            // merge OOB connected ways osm files and add 'hoot:change:exclude:delete' tag to each
            GrailParams mergeOobWaysParams = new GrailParams(params);
            File mergeOobWaysFile = new File(params.getWorkDir(), "oobways.osm");
            mergeOobWaysParams.setOutput(mergeOobWaysFile.getAbsolutePath());
            // Map<String, String> opts = new HashMap<>();
            // opts.put("convert.ops", "hoot::SetTagValueVisitor");
            // opts.put("set.tag.value.visitor.element.criteria", "hoot::WayCriterion");
            // opts.put("set.tag.value.visitor.keys", "hoot:change:exclude:delete");
            // opts.put("set.tag.value.visitor.values", "yes");
            // mergeOobWaysParams.setAdvancedOptions(opts);
            mergeOobWaysParams.setInput1("\\d+\\.osm"); //this is the file filter
            workflow.add(grailCommandFactory.build(jobId, mergeOobWaysParams, "info", MergeOsmFilesCommand.class, this.getClass()));

            // merge OOB connected ways merge file and the reference osm file
            GrailParams mergeRefParams = new GrailParams(params);
            ingestFile = new File(params.getWorkDir(), "merge.osm");
            mergeRefParams.setInput1("(" + mergeOobWaysFile.getName() + "|" + referenceOSMFile.getName() + ")"); //this is the file filter
            mergeRefParams.setOutput(ingestFile.getAbsolutePath());
            workflow.add(grailCommandFactory.build(jobId, mergeRefParams, "info", MergeOsmFilesCommand.class, this.getClass()));
        }

        // Write the data to the hoot db
        GrailParams pushParams = new GrailParams(params);
        pushParams.setInput1(ingestFile.getAbsolutePath());
        ExternalCommand importRailsPort = grailCommandFactory.build(jobId, pushParams, "info", PushToDbCommand.class, this.getClass());
        workflow.add(importRailsPort);

        // Set map tags marking dataset as eligible for derive changeset
        Map<String, String> tags = new HashMap<>();
        tags.put("grailReference", "true");
        tags.put("bbox", params.getBounds());
        InternalCommand setMapTags = setMapTagsCommandFactory.build(tags, jobId);
        workflow.add(setMapTags);

        // Move the data to the folder
        InternalCommand setFolder = updateParentCommandFactory.build(jobId, parentFolderId, params.getOutput(), user, this.getClass());
        workflow.add(setFolder);

        return workflow;
    }

    // throws forbidden exception is user does not have advanced privileges
    private static void advancedUserCheck(Users user) {
        HashMap privileges = ((HashMap) user.getPrivileges());
        if(privileges == null || !privileges.get("advanced").equals("true")) {
            throw new ForbiddenException(Response.status(Response.Status.FORBIDDEN).type(MediaType.TEXT_PLAIN).entity("You do not have access to this operation.").build());
        }
    }

    //Used for self-signed SSL certs
    //still requires import of cert into server java keystore
    //e.g. sudo keytool -import -alias <CertAlias> -keystore /usr/lib/jvm/java-1.8.0-openjdk-1.8.0.191.b12-1.el7_6.x86_64/jre/lib/security/cacerts -file /tmp/CertFile.der
    public static InputStream getUrlInputStreamWithNullHostnameVerifier(String urlString) throws IOException {
        InputStream inputStream = null;
        URL url = new URL(urlString);
        HttpURLConnection conn = (HttpURLConnection) url.openConnection();

        if (url.getProtocol().equalsIgnoreCase("https")) {
            ((HttpsURLConnection) conn).setHostnameVerifier(new HostnameVerifier() {
                @Override
                public boolean verify(String hostname, SSLSession sslSession) {
                    return true;
                }
            });
        }

        try {
            inputStream = conn.getInputStream();
        } catch(IOException e) {
            //read the error response body
            String err = IOUtils.toString(conn.getErrorStream(), StandardCharsets.UTF_8);
            logger.error(err);
            throw new IOException(err);
        }

        return inputStream;
    }


    // Get Capabilities from an OSM API Db
    private static APICapabilities getCapabilities(String capabilitiesUrl) {
        APICapabilities params = new APICapabilities();

        try {
            InputStream inputStream = getUrlInputStreamWithNullHostnameVerifier(replaceSensitiveData(capabilitiesUrl));

            DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
            DocumentBuilder db = dbf.newDocumentBuilder();
            Document doc = db.parse(inputStream);
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

    /**
     *
     * @param url
     * @param usePrivateOverpass
     *          If true and the cert path is set then we know to use the cert for the overpass request
     *          If false then no cert will need to be used for the request
     * @return
     */
    private static ArrayList<Double> retrieveOverpassStats(String url, boolean usePrivateOverpass) {
        ArrayList<Double> statCounts = new ArrayList<>();

        try {
            InputStream inputStream;

            // if cert path is specified then we assume to use them for the request
            // Both need to be true because in the case of using this function for public overpass we want it to skip immediately even if
            // the cert path is specified.
            if (usePrivateOverpass && !replaceSensitiveData(PRIVATE_OVERPASS_CERT_PATH).equals(PRIVATE_OVERPASS_CERT_PATH)) {
                inputStream = PullApiCommand.getHttpResponseWithSSL(url);
            } else {
                inputStream = PullOverpassCommand.getOverpassInputStream(url);
            }

            BufferedReader br = new BufferedReader(new InputStreamReader(inputStream));

            String inputLine;

            boolean firstLine = true;
            while ((inputLine = br.readLine()) != null) {
                //After the first line it is all stat numbers
                if(!firstLine){
                    String[] rowCounts = inputLine.split("\t");
                    for(int i = 0; i < rowCounts.length; i++) {
                        statCounts.set(i, statCounts.get(i) + Double.parseDouble(rowCounts[i]));
                    }
                } else {
                    // This else is only entered for the first line which contains the column names so we initialize arraylist with that many elements
                    int numColumns = inputLine.split("\t").length;
                    for(int i = 0; i < numColumns; i++) {
                        statCounts.add(0.0);
                    }
                }

                firstLine = false;
            }

            br.close();
        }
        catch (Exception exc) {
            String msg = "Error retrieving overpass stats!  Cause: " + exc.getMessage();
            throw new WebApplicationException(exc, Response.status(Response.Status.NOT_FOUND).entity(msg).build());
        }

        return statCounts;
    }

    @GET
    @Path("/getChangesetOptions")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getOptions() {
        JSONObject template;
        JSONParser parser = new JSONParser();
        try (FileReader fileReader = new FileReader(new File(HOME_FOLDER, CHANGESET_OPTIONS))) {
            template = (JSONObject) parser.parse(fileReader);
        }
        catch (Exception e) {
            String msg = "Error getting changeset options!  Cause: " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }
        return Response.ok(template).build();
    }

}
