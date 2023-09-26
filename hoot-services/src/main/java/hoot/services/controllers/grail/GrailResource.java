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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016-2023 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.grail;

import static hoot.services.HootProperties.CHANGESETS_FOLDER;
import static hoot.services.HootProperties.CHANGESET_OPTIONS;
import static hoot.services.HootProperties.DIFFERENTIAL_CHANGESET_TWOSTEP;
import static hoot.services.HootProperties.GRAIL_OVERPASS_LABEL;
import static hoot.services.HootProperties.GRAIL_RAILS_LABEL;
import static hoot.services.HootProperties.HOME_FOLDER;
import static hoot.services.HootProperties.HOOTAPI_DB_URL;
import static hoot.services.HootProperties.OVERPASS_QUERY_MAXSIZE;
import static hoot.services.HootProperties.OVERPASS_QUERY_TIMEOUT;
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
import java.net.URLDecoder;
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
import javax.ws.rs.ServiceUnavailableException;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.ResponseBuilder;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.apache.commons.io.FileUtils;
import org.apache.commons.io.IOUtils;
import org.apache.commons.io.input.ReversedLinesFileReader;
import org.apache.commons.lang3.CharSet;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import hoot.services.command.Command;
import hoot.services.command.ExternalCommand;
import hoot.services.command.InternalCommand;
import hoot.services.command.common.ZIPFileCommand;
import hoot.services.controllers.conflation.ConflateCommandFactory;
import hoot.services.controllers.conflation.ConflateParams;
import hoot.services.controllers.ingest.ImportCommand;
import hoot.services.controllers.ingest.ImportCommandFactory;
import hoot.services.controllers.ingest.RemoveFilesCommandFactory;
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
    private ImportCommandFactory importCommandFactory;

    @Autowired
    private UpdateParentCommandFactory updateParentCommandFactory;

    @Autowired
    private PullConnectedWaysCommandFactory connectedWaysCommandFactory;

    @Autowired
    private RemoveFilesCommandFactory removeFilesCommandFactory;

    @Autowired
    private ConflateCommandFactory conflateCommandFactory;

    public GrailResource() {}

    /**
     * If the PRIVATE_OVERPASS_URL variable is set to a value then return true
      */
    public static boolean isPrivateOverpassActive() {
        return !replaceSensitiveData(PRIVATE_OVERPASS_URL).equals(PRIVATE_OVERPASS_URL);
    }

    private APICapabilities railsOnlineCheck() {
        APICapabilities railsPortCapabilities = getCapabilities(RAILSPORT_CAPABILITIES_URL);
        logger.info("ApplyChangeset: railsPortAPI status = " + railsPortCapabilities.getApiStatus());
        if (railsPortCapabilities.getApiStatus() == null || railsPortCapabilities.getApiStatus().equals("offline")) {
            throw new ServiceUnavailableException(Response.status(Response.Status.SERVICE_UNAVAILABLE).type(MediaType.TEXT_PLAIN).entity("The reference OSM API server is offline or unreachable.").build());
        }

        return railsPortCapabilities;
    }

    private ImportCommand getRailsPortApiCommand(String jobId, GrailParams params, String debugLevel) throws UnavailableException {
        // Checks to see that the sensitive data was actually replaced meaning there was a value
        if (isPrivateOverpassActive()) {
            params.setPullUrl(PRIVATE_OVERPASS_URL);
        } else {
            APICapabilities railsPortCapabilities = railsOnlineCheck();

            params.setMaxBoundsSize(railsPortCapabilities.getMaxArea());
            params.setPullUrl(RAILSPORT_PULL_URL);
        }

        if (params.getOutput() == null) {
            File referenceOSMFile = new File(params.getWorkDir(), REFERENCE + ".osm");
            params.setOutput(referenceOSMFile.getAbsolutePath());
            if (referenceOSMFile.exists()) referenceOSMFile.delete();
            params.setWorkDir(null);
        }

        ImportCommand command = importCommandFactory.build(jobId, params, debugLevel, this.getClass());
        return command;
    }

    private InternalCommand getConnectedWaysApiCommand(String jobId, GrailParams params) throws UnavailableException {
        params.setPullUrl(RAILSPORT_PULL_URL);

        InternalCommand command = connectedWaysCommandFactory.build(jobId, params, this.getClass());
        return command;
    }

    private ExternalCommand getPublicOverpassCommand(String jobId, File workDir, GrailParams params, String debugLevel) {
        params.setPullUrl(PUBLIC_OVERPASS_URL);

        if (params.getOutput() == null) {
            File overpassFile = new File(workDir, SECONDARY + ".osm");
            params.setOutput(overpassFile.getAbsolutePath());
            if (overpassFile.exists()) overpassFile.delete();
            params.setWorkDir(null);
        }

        return importCommandFactory.build(jobId, params, debugLevel, getClass());
    }

    /**
     * Pull the Public Overpass and Private Rails Port data for a bounding box and run differential on it
     *
     * Takes in a json object
     * POST hoot-services/grail/createchangeset
     *
     * {
     *   //Can be in the form of 4 points or multiple points
     *   "bounds": "-77.279039,39.177650,-77.275970,39.178958",
     *   OR
     *   "bounds": "-77.283282,39.185101;-77.291479,39.172060;-77.279806,39.166770;-77.267661,39.181209;-77.283282,39.185101"
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
    @Path("/createchangeset")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response createChangeset(@Context HttpServletRequest request,
            GrailParams reqParams,
            @QueryParam("deriveType") @DefaultValue("") String deriveType,
            @QueryParam("replacement") @DefaultValue("false") Boolean replacement,
            @QueryParam("uploadResult") @DefaultValue("false") Boolean uploadResult,
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
            logger.error("createChangeset: Error creating folder: {} ", workDir.getAbsolutePath(), ioe);
            return Response.status(Response.Status.INTERNAL_SERVER_ERROR).entity(ioe.getMessage()).build();
        }
        reqParams.setWorkDir(workDir);

        Class<? extends GrailCommand> grailCommandClass;
        GrailParams differentialParams = new GrailParams(reqParams);
        List<Command> workflow = new LinkedList<>();
        String input1 = reqParams.getInput1() == null ? null : HOOTAPI_DB_URL + "/" + reqParams.getInput1();
        String input2 = reqParams.getInput2() == null ? null : HOOTAPI_DB_URL + "/" + reqParams.getInput2();

        GrailParams getRailsParams = new GrailParams(reqParams);
        GrailParams getOverpassParams = new GrailParams(reqParams);

        String outputFileName = "diff.osc";

        if (deriveType.equals("Adds only")) {
            if (input1 == null) {
                ExternalCommand getOverpassCommand = getPublicOverpassCommand(jobId, workDir, getOverpassParams, debugLevel);
                workflow.add(getOverpassCommand);
                input1 = getOverpassParams.getOutput();
            }

            if (input2 == null) {
                input2 = "\"\"";
            }

            grailCommandClass = DeriveChangesetCommand.class;
        } else if (deriveType.equals("Cut & Replace")) {
            if (input1 == null) {
                // can't have the cut and replace options for hoot convert. used for DeriveChangesetReplacementCommand
                getRailsParams.setAdvancedOptions(null);
                File tempDir = new File(TEMP_OUTPUT_PATH, "grail_" + jobId);
                File referenceOSMFile = new File(tempDir, REFERENCE + ".osm");
                getRailsParams.setOutput(referenceOSMFile.getAbsolutePath());
                if (referenceOSMFile.exists()) referenceOSMFile.delete();
                // hide the changeset workDir so it doesn't get deleted by ImportCommand
                getRailsParams.setWorkDir(null);
                try {
                    workflow.addAll(setupRailsPull(jobId, getRailsParams, null, debugLevel));
                } catch (UnavailableException ex) {
                    return Response.status(Response.Status.SERVICE_UNAVAILABLE).entity(ex.getMessage()).build();
                }
                input1 = getRailsParams.getOutput();
            }

            if (input2 == null) {
                ExternalCommand getOverpassCommand = getPublicOverpassCommand(jobId, workDir, getOverpassParams, debugLevel);
                workflow.add(getOverpassCommand);

                input2 = getOverpassParams.getOutput();
            }

            grailCommandClass = DeriveChangesetReplacementCommand.class;
        } else {
            if (input1 == null) {
                try {
                    workflow.add(getRailsPortApiCommand(jobId, getRailsParams, debugLevel));
                } catch (UnavailableException ex) {
                    return Response.status(Response.Status.SERVICE_UNAVAILABLE).entity(ex.getMessage()).build();
                }
                input1 = getRailsParams.getOutput();
            }

            if (input2 == null) {
                ExternalCommand getOverpassCommand = getPublicOverpassCommand(jobId, workDir, getOverpassParams, debugLevel);
                workflow.add(getOverpassCommand);

                input2 = getOverpassParams.getOutput();
            }

            if (deriveType.toLowerCase().startsWith("diff")) {
                //check for one (conflate -> osc) or two step (conflate -> osm -> derive changeset -> osc) preference
                if (DIFFERENTIAL_CHANGESET_TWOSTEP) {
                    String outputOsm = new File(workDir, "diff.osm").getAbsolutePath();
                    ConflateParams conflateParams = new ConflateParams();
                    conflateParams.setConflationCommand(deriveType.toLowerCase().contains("tags") ? "differential-tags" : "differential");
                    conflateParams.setConflationType("Differential");
                    conflateParams.setHoot2(true);
                    conflateParams.setHoot2AdvOptions(reqParams.getAdvancedOptions());
                    conflateParams.setCollectStats(false);
                    conflateParams.setBounds(reqParams.getBounds());
                    conflateParams.setInput1(input1);
                    conflateParams.setInputType1("");
                    conflateParams.setInput2(input2);
                    conflateParams.setInputType2("");
                    conflateParams.setOutputName(outputOsm);
                    conflateParams.setOutputType("");
                    ExternalCommand conflateCommand = conflateCommandFactory.build(jobId, conflateParams, debugLevel, this.getClass(), user);
                    workflow.add(conflateCommand);
                    input2 = outputOsm;
                    grailCommandClass = DeriveChangesetCommand.class;
                } else {
                    grailCommandClass = RunDiffCommand.class;
                }
            } else {
                if (deriveType.equals("JOSM .osm")) {
                    String name = reqParams.getOutput();
                    outputFileName = ((name != null) ? name : "diff") + ".osm";
                }
                grailCommandClass = DeriveChangesetCommand.class;
            }
        }

        // create output file
        File geomDiffFile = new File(workDir, outputFileName);
        if (geomDiffFile.exists()) geomDiffFile.delete();
        try {
            geomDiffFile.createNewFile();
        }
        catch(IOException exc) {
            logger.error("createChangeset: Error creating file: {} ", geomDiffFile.getAbsolutePath(), exc);
            return Response.status(Response.Status.INTERNAL_SERVER_ERROR).entity(exc.getMessage()).build();
        }

        differentialParams.setWorkDir(workDir);
        differentialParams.setInput1(input1);
        differentialParams.setInput2(input2);
        differentialParams.setOutput(geomDiffFile.getAbsolutePath());

        ExternalCommand makeDiff = grailCommandFactory.build(jobId, differentialParams, debugLevel, grailCommandClass, this.getClass());
        workflow.add(makeDiff);

        JobType jobType = JobType.DERIVE_CHANGESET;
        // Check to see if changeset should be uploaded
        if (uploadResult) {
            GrailParams pushParams = new GrailParams(differentialParams);
            workflow.add(createApplyWorkflow(jobId, pushParams, debugLevel));

            if (deriveType.equals("Adds only")) {
                jobType = JobType.BULK_ADD;
            } else if (deriveType.equals("Cut & Replace")) {
                jobType = JobType.BULK_REPLACE;
            } else {
                if (reqParams.getApplyTags()) {
                    jobType = JobType.BULK_DIFFWTAGS;
                } else {
                    jobType = JobType.BULK_DIFFERENTIAL;
                }
            }

            // Wait to detect overpass 'Last element pushed'
            if (isPrivateOverpassActive()) {
                GrailParams waitParams = new GrailParams(reqParams);
                workflow.add(grailCommandFactory.build(jobId, waitParams, "info", WaitOverpassUpdate.class, this.getClass()));
            }

            // Clean up pulled files
            ArrayList<File> deleteFiles = new ArrayList<>();
            deleteFiles.add(workDir);
            // commented out because we don't want to delete if error occurs so user can download error file
            // InternalCommand cleanFolders = removeFilesCommandFactory.build(jobId, deleteFiles);
            // workflow.add(cleanFolders);
        } else {
            //zip up result for possible download
            //ZIPFileCommand(File targetZIP, File workDir, String file, Class<?> caller)
            Command zipCommand = new ZIPFileCommand(new File(workDir, outputFileName + ".zip"), workDir, outputFileName, this.getClass());
            workflow.add(zipCommand);
        }

        Map<String, Object> jobStatusTags = new HashMap<>();
        jobStatusTags.put("bounds", reqParams.getBounds());
        jobStatusTags.put("parentId", reqParams.getParentId());
        jobStatusTags.put("taskInfo", reqParams.getTaskInfo());
        jobStatusTags.put("deriveType", deriveType);
        if (reqParams.getInput1() != null) jobStatusTags.put("input1", reqParams.getInput1());
        if (reqParams.getInput2() != null) jobStatusTags.put("input2", reqParams.getInput2());
        if (reqParams.getOutput() != null) jobStatusTags.put("output", reqParams.getOutput());

        jobProcessor.submitAsync(new Job(jobId, user.getId(), workflow.toArray(new Command[workflow.size()]), jobType, jobStatusTags));

        JSONObject jobInfo = new JSONObject();
        jobInfo.put("jobid", jobId);

        return Response.ok(jobInfo.toJSONString()).build();
    }

    /**
     * GET hoot-services/grail/gettimeouttasks
     *
     * @param request
     * @param projectId
     * @return
     */
    @GET
    @Path("/gettimeouttasks")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getTimeoutTasks(@Context HttpServletRequest request,
            @QueryParam("projectId") String projectId) {
        Users user = Users.fromRequest(request);
        advancedUserCheck(user);

        List<Long> tasks = DbUtils.getTimeoutTasks(projectId);

        return Response.ok(tasks).build();
    }

    /**
     * GET hoot-services/grail/overpasssynccheck
     *
     * @return
     */
    @GET
    @Path("/overpasssynccheck")
    @Produces(MediaType.APPLICATION_JSON)
    public Response overpassSyncCheck(@Context HttpServletRequest request,
            @QueryParam("projectTaskInfo") @DefaultValue("") String projectTaskInfo) {
        Users user = Users.fromRequest(request);
        advancedUserCheck(user);

        String jobId = "grail_" + UUID.randomUUID().toString().replace("-", "");
        String id = DbUtils.getJobIdByTask(projectTaskInfo);
        if (id == null) {
            String errorMsg = "Error during overpass sync check! Error retrieving taskInfo for the task.";
            throw new WebApplicationException(Response.status(Response.Status.NOT_FOUND).entity(errorMsg).build());
        }

        String bounds = DbUtils.getJobBounds(id);
        if (bounds == null) {
            String errorMsg = "Error during overpass sync check! Error retrieving bounds for the task";
            throw new WebApplicationException(Response.status(Response.Status.NOT_FOUND).entity(errorMsg).build());
        }

        // Wait to detect overpass 'Last element pushed'
        GrailParams waitParams = new GrailParams();
        waitParams.setUser(user);
        waitParams.setTaskInfo(projectTaskInfo);
        waitParams.setBounds(bounds);

        List<Command> workflow = new LinkedList<>();
        workflow.add(grailCommandFactory.build(id, waitParams, "info", WaitOverpassUpdate.class, this.getClass()));

        jobProcessor.submitAsync(new Job(jobId, user.getId(), workflow.toArray(new Command[workflow.size()]), JobType.SYNC_WAIT));

        JSONObject jobInfo = new JSONObject();
        jobInfo.put("jobid", jobId);

        return Response.ok(jobInfo.toJSONString()).build();
    }

    private ExternalCommand createApplyWorkflow(String jobId, GrailParams pushParams, String debugLevel) {
        pushParams.setPushUrl(RAILSPORT_PUSH_URL);

        try {
            railsOnlineCheck();

            ExternalCommand applyGeomChange = grailCommandFactory.build(jobId, pushParams, debugLevel, ApplyChangesetCommand.class, this.getClass());
            return applyGeomChange;
        }
        catch (ServiceUnavailableException exc) {
            String msg = "Error during changeset push! Error connecting to railsport";
            throw new WebApplicationException(exc, Response.serverError().entity(msg).build());
        }
        catch (Exception exc) {
            String msg = "Error during changeset push! Params: " + pushParams;
            throw new WebApplicationException(exc, Response.serverError().entity(msg).build());
        }
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
            //check if diff.osc has translated tags by scanning lines in reverse
            //looking for FCODE or F_CODE until just a single feature has been read
            //we will assume all features in a translated schema will have this tag
            ReversedLinesFileReader fr = new ReversedLinesFileReader(FileUtils.getFile(workDir, "diff.osc"), StandardCharsets.UTF_8);
            String ln;
            boolean hasFcode = false;
            do {
                ln = fr.readLine();
                if (ln != null && ln.matches("\\s*<tag k=\"F_?CODE\" .*")) {
                    hasFcode = true;
                }
            } while (ln != null && !ln.matches("\\s*<(node|way|relation) .*"));
            fr.close();

            if (hasFcode) {
                jobInfo.put("error", "non-OSM tags detected in changeset");
            }

            JSONParser parser = new JSONParser();
            String jsonDoc = FileUtils.readFileToString(FileUtils.getFile(workDir, "stats.json"), "UTF-8");
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
            railsOnlineCheck();

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

            if (changesetFile.exists() && isPrivateOverpassActive()) {
                // Wait to detect overpass 'Last element pushed'
                GrailParams waitParams = new GrailParams(params);
                workflow.add(grailCommandFactory.build(jobId, waitParams, "info", WaitOverpassUpdate.class, this.getClass()));
            }

            Map<String, Object> jobStatusTags = new HashMap<>();
            jobStatusTags.put("bounds", reqParams.getBounds());
            jobStatusTags.put("parentId", reqParams.getParentId());
            jobStatusTags.put("taskInfo", reqParams.getTaskInfo());

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
     * @param reqParams Contains info such as bounds, layerName, and if the
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
            @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel,
            GrailParams reqParams) {

        Users user = Users.fromRequest(request);
        advancedUserCheck(user);
        reqParams.setUser(user);

        String bounds = reqParams.getBounds();
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

        GrailParams getOverpassParams = new GrailParams(reqParams);

        // If clipExtent is true, split the import into two commands
        // using an OGR format like GPKG as an intermediate file so that true geometry clipping can occur
        if (reqParams.getClipExtent()) {

            String translation = new File(HOME_FOLDER, "translations/RenderDb.js").getAbsolutePath();
            getOverpassParams.setTranslation(translation);
            File overpassFile = new File(workDir, SECONDARY + ".gpkg");
            getOverpassParams.setOutput(overpassFile.getAbsolutePath());
            if (overpassFile.exists()) overpassFile.delete();
            workflow.add(getPublicOverpassCommand(jobId, workDir, getOverpassParams, debugLevel));

            GrailParams dbOverpassParams = new GrailParams(reqParams);
            dbOverpassParams.setIntermediateFile(overpassFile.getAbsolutePath());
            dbOverpassParams.setOutput(HOOTAPI_DB_URL + "/" + layerName);
            dbOverpassParams.setTranslation(translation);
            workflow.add(getPublicOverpassCommand(jobId, workDir, dbOverpassParams, debugLevel));

        } else {
            getOverpassParams.setOutput(HOOTAPI_DB_URL + "/" + layerName);
            workflow.add(getPublicOverpassCommand(jobId, workDir, getOverpassParams, debugLevel));
        }

        GrailParams params = new GrailParams(reqParams);

        // Set map tags marking dataset as eligible for derive changeset
        Map<String, String> tags = new HashMap<>();
        tags.put("bounds", params.getBounds());
        if (params.getTaskInfo() != null) { tags.put("taskInfo", params.getTaskInfo()); }
        InternalCommand setMapTags = setMapTagsCommandFactory.build(tags, jobId);
        workflow.add(setMapTags);

        // Move the data to the folder
        InternalCommand setFolder = updateParentCommandFactory.build(jobId, folderId, layerName, user, this.getClass());
        workflow.add(setFolder);

        // Clean up pulled files
        ArrayList<File> deleteFiles = new ArrayList<>();
        deleteFiles.add(workDir);
        InternalCommand cleanFolders = removeFilesCommandFactory.build(jobId, deleteFiles);
        workflow.add(cleanFolders);

        Map<String, Object> jobStatusTags = new HashMap<>();
        jobStatusTags.put("bounds", bounds);
        jobStatusTags.put("taskInfo", reqParams.getTaskInfo());

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
        jobInfo.put("railsLabel", railsLabel);
        jobInfo.put("overpassLabel", overpassLabel);

        return Response.ok(jobInfo.toJSONString()).build();
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

    @GET
    @Path("/getDefaultOverpassQuery")
    @Produces(MediaType.TEXT_PLAIN)
    public Response getDefaultOverpassQuery() {
        return Response.ok(PullOverpassCommand.getDefaultOverpassQuery()).build();
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
            overpassQuery = PullOverpassCommand.getDefaultOverpassQuery();
        } else {
            overpassQuery = customQuery;

            if (overpassQuery.contains("out:xml")) {
                overpassQuery = overpassQuery.replace("out:xml", "out:json");
            }
        }

        // first line that lists columns which are counts for each feature type
        overpassQuery = overpassQuery.replaceAll(PullApiCommand.overpassqlFormatPattern,
                String.format("[out:csv(::count, ::\"count:nodes\", ::\"count:ways\", ::\"count:relations\")][maxsize:%s][timeout:%s];",
                OVERPASS_QUERY_MAXSIZE, OVERPASS_QUERY_TIMEOUT)
                );

        // overpass query can have multiple "out *" lines so need to replace all
        overpassQuery = overpassQuery.replaceAll("out [\\s\\w]+;", "out count;");

        // polygon contains coordinates separated by ';'
        if (reqParams.getBounds().contains(";")) {
            // replace "[bbox:{{bbox}}]" if exists
            overpassQuery = overpassQuery.replace("[bbox:{{bbox}}]", "");

            // We need to reverse the coordinates from lon,lat to lat,long for overpass
            String polyBounds = PullOverpassCommand.boundsToOverpassPolyString(reqParams.getBounds());
            //replace the {{bbox}} from the overpass query with the poly
            overpassQuery = overpassQuery.replace("{{bbox}}", "poly:\"" + polyBounds + "\"");
        } else {
            //replace the {{bbox}} from the overpass query with the actual coordinates and encode the query
            overpassQuery = overpassQuery.replace("{{bbox}}", new BoundingBox(reqParams.getBounds()).toOverpassString());
        }

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
        if (isPrivateOverpassActive()) {
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
     * @param reqParams Contains info such as bounds, layerName, and if the
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
            @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel,
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
        params.setInput1(layerName);
        params.setOutput(HOOTAPI_DB_URL + "/" + layerName);


        List<Command> workflow;
        try {
            workflow = setupRailsPull(jobId, params, folderId, debugLevel);
        }
        catch(UnavailableException exc) {
            return Response.status(Response.Status.SERVICE_UNAVAILABLE).entity(exc.getMessage()).build();
        }

        Map<String, Object> jobStatusTags = new HashMap<>();
        jobStatusTags.put("bounds", reqParams.getBounds());
        jobStatusTags.put("taskInfo", reqParams.getTaskInfo());

        jobProcessor.submitAsync(new Job(jobId, user.getId(), workflow.toArray(new Command[workflow.size()]), JobType.IMPORT, jobStatusTags));

        return Response.ok(json.toJSONString()).build();
    }

    private List<Command> setupRailsPull(String jobId, GrailParams params, Long parentFolderId, String debugLevel) throws UnavailableException {
        List<Command> workflow = new LinkedList<>();
        boolean usingPrivateOverpass = isPrivateOverpassActive();
        String layerName = params.getInput1();

        // Pull data from the reference OSM API or private Overpass API
        GrailParams getRailsParams = new GrailParams(params);

        // have to add the query for getting connected ways before calling getRailsPortApiCommand
        if (usingPrivateOverpass) {
            String queryWithConnectedWays = PullApiCommand.connectedWaysQuery(getRailsParams.getCustomQuery());
            getRailsParams.setCustomQuery(queryWithConnectedWays);
        }

        try {
            // If clipExtent is true, split the import into two commands
            // using an OGR format like GPKG as an intermediate file so that true geometry clipping can occur
            if (params.getClipExtent()) {

                String translation = new File(HOME_FOLDER, "translations/RenderDb.js").getAbsolutePath();
                getRailsParams.setTranslation(translation);
                File overpassFile = new File(params.getWorkDir(), REFERENCE + ".gpkg");
                getRailsParams.setOutput(overpassFile.getAbsolutePath());
                if (overpassFile.exists()) overpassFile.delete();
                getRailsParams.setWorkDir(null);
                workflow.add(getRailsPortApiCommand(jobId, getRailsParams, debugLevel));

                GrailParams dbRailsParams = new GrailParams(params);
                dbRailsParams.setIntermediateFile(overpassFile.getAbsolutePath());
                dbRailsParams.setTranslation(translation);
                workflow.add(getRailsPortApiCommand(jobId, dbRailsParams, debugLevel));

            } else {
                workflow.add(getRailsPortApiCommand(jobId, getRailsParams, debugLevel));
            }
        } catch (UnavailableException exc) {
            throw new UnavailableException("The Rails port API is offline.");
        }

        // if not using private overpass then this will be changed to the merge file
        File ingestFile = new File(getRailsParams.getOutput());

        // private overpass query result file should handle getting the connected ways so just use that as the ingest file
        if (!usingPrivateOverpass) {
            GrailParams connectedWaysParams = new GrailParams(params);
            connectedWaysParams.setInput1(ingestFile.getAbsolutePath());
            File cropFile = new File(params.getWorkDir(), "crop.osm");
            connectedWaysParams.setOutput(cropFile.getAbsolutePath());
            // Do an invert crop of this data to get nodes outside bounds
            workflow.add(grailCommandFactory.build(jobId, connectedWaysParams, "info", InvertCropCommand.class, this.getClass()));

            // read node ids, pull connected ways, pull entire ways, remove cropfile
            workflow.add(getConnectedWaysApiCommand(jobId, connectedWaysParams));

            // merge OOB connected ways osm files and add 'hoot:change:exclude:delete' tag to each
            GrailParams mergeOobWaysParams = new GrailParams(params);
            File mergeOobWaysFile = new File(params.getWorkDir(), "oobways.osm");
            mergeOobWaysParams.setOutput(mergeOobWaysFile.getAbsolutePath());
            mergeOobWaysParams.setInput1("\\d+\\.osm"); //this is the file filter
            workflow.add(grailCommandFactory.build(jobId, mergeOobWaysParams, "info", MergeOsmFilesCommand.class, this.getClass()));

            // merge OOB connected ways merge file and the reference osm file
            GrailParams mergeRefParams = new GrailParams(params);
            mergeRefParams.setInput1("(" + mergeOobWaysFile.getName() + "|" + ingestFile.getName() + ")"); //this is the file filter. ingestFile here points to reference.osm
            ingestFile = new File(params.getWorkDir(), "merge.osm");
            mergeRefParams.setOutput(ingestFile.getAbsolutePath());
            workflow.add(grailCommandFactory.build(jobId, mergeRefParams, "info", MergeOsmFilesCommand.class, this.getClass()));
            // set output so the output file can be used outside this function
            params.setOutput(ingestFile.getAbsolutePath());
        }

        // let parentFolderId decide if data should be written to database
        if ( parentFolderId != null ) {
            if (!usingPrivateOverpass) {
                // Write the data to the hoot db
                GrailParams pushParams = new GrailParams(params);
                pushParams.setInput1(ingestFile.getAbsolutePath());
                pushParams.setOutput(layerName);
                ExternalCommand importRailsPort = grailCommandFactory.build(jobId, pushParams, "info", PushToDbCommand.class, this.getClass());
                workflow.add(importRailsPort);
            }

            // Set map tags marking dataset as eligible for derive changeset
            Map<String, String> tags = new HashMap<>();
            tags.put("grailReference", "true");
            tags.put("bounds", params.getBounds());
            if (params.getTaskInfo() != null) { tags.put("taskInfo", params.getTaskInfo()); }
            InternalCommand setMapTags = setMapTagsCommandFactory.build(tags, jobId);
            workflow.add(setMapTags);

            // Move the data to the folder
            InternalCommand setFolder = updateParentCommandFactory.build(jobId, parentFolderId, layerName, params.getUser(), this.getClass());
            workflow.add(setFolder);

            if (!usingPrivateOverpass) {
                // Clean up pulled files
                ArrayList<File> deleteFiles = new ArrayList<>();
                deleteFiles.add(params.getWorkDir());
                InternalCommand cleanFolders = removeFilesCommandFactory.build(jobId, deleteFiles);
                workflow.add(cleanFolders);
            }
        }

        return workflow;
    }

    // throws forbidden exception is user does not have advanced privileges
    public static void advancedUserCheck(Users user) {
        HashMap privileges = ((HashMap) user.getPrivileges());
        if(privileges == null || !privileges.get("advanced").equals("true")) {
            throw new ForbiddenException(Response.status(Response.Status.FORBIDDEN).type(MediaType.TEXT_PLAIN).entity("You do not have access to this operation.").build());
        }
    }

    //Used for self-signed SSL certs
    //still requires import of cert into server java keystore
    //e.g. sudo keytool -import -alias <CertAlias> -keystore $JAVA_HOME/jre/lib/security/cacerts -file /tmp/CertFile.der
    public static InputStream getUrlInputStreamWithNullHostnameVerifier(String urlString) throws IOException {
        InputStream inputStream = null;
        String[] splitUrl = urlString.split("(?=data)"); // prevents removal of 'data' text
        URL url = new URL(splitUrl[0]);
        HttpURLConnection conn = (HttpURLConnection) url.openConnection();
        conn.setRequestMethod(urlString.contains(replaceSensitiveData(RAILSPORT_PUSH_URL)) ? "GET" : "POST"); // Need to use GET for rails port
        conn.setDoOutput(true);

        if (url.getProtocol().equalsIgnoreCase("https")) {
            ((HttpsURLConnection) conn).setHostnameVerifier(new HostnameVerifier() {
                @Override
                public boolean verify(String hostname, SSLSession sslSession) {
                    return true;
                }
            });
        }
        // Just a safety check but splitUrl[1] should be the query data
        if (splitUrl.length == 2) {
            logger.info(splitUrl[0]);
            logger.info(URLDecoder.decode(splitUrl[1], "UTF-8"));
            conn.getOutputStream().write(splitUrl[1].getBytes(StandardCharsets.UTF_8));
        }

        try {
            inputStream = conn.getInputStream();
        } catch(IOException e) {
            //read the error response body
            InputStream errIs = conn.getErrorStream();

            String err;
            if (errIs != null) {
                err = IOUtils.toString(errIs, StandardCharsets.UTF_8);
            } else {
                err = "Remote server was unreachable, check the Hoot config";
            }
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
            //The api capabilities request timed out or returned an error or malformed response, logged elsewhere;
            //return an empty params object which should report to the UI that api is offline
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

}
