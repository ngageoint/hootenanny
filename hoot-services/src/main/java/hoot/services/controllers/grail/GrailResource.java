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
import java.io.InputStream;
import java.net.URL;
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

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;

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
// import hoot.services.command.common.ZIPDirectoryContentsCommand;
// import hoot.services.command.common.ZIPFileCommand;
import hoot.services.controllers.osm.map.MapResource;
import hoot.services.job.Job;
import hoot.services.job.JobProcessor;
// import hoot.services.utils.DbUtils;
// import hoot.services.utils.XmlDocumentBuilder;


@Controller
@Path("")
@Transactional
public class GrailResource {
    private static final Logger logger = LoggerFactory.getLogger(GrailResource.class);

    @Autowired
    private JobProcessor jobProcessor;

    @Autowired
    private GrailCommandFactory grailCommandFactory;

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
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response everythingByBox(@QueryParam("bbox") String bbox,
                        @QueryParam("USER_ID") @DefaultValue("Hootenanny") String userId,
                        @QueryParam("APPLY_TAGS") @DefaultValue("false") Boolean applyTags,
                        @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {

        String jobId = "grail_" + UUID.randomUUID().toString().replace("-", "");
        File workDir = new File(TEMP_OUTPUT_PATH, jobId);

        List<Command> workflow = new LinkedList<>();

        GrailParams params = new GrailParams();
        params.setBounds(bbox);
        params.setUserId(userId);
        
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
            APICapabilities mainOsmApiCapabilities = getCapabilities(MAIN_OSMAPI_CAPABILITIES_URL);
            logger.info("EverythingByBox: mainOSMAPI status = " + mainOsmApiCapabilities.getApiStatus());
            if (mainOsmApiCapabilities.getApiStatus() == "offline" ) {
                return Response.status(Response.Status.SERVICE_UNAVAILABLE).entity("The main OSM API server is offline. Try again later").build();
            }

            APICapabilities railsPortCapabilities = getCapabilities(RAILSPORT_CAPABILITIES_URL);
            logger.info("EverythingByBox: railsPortAPI status = " + railsPortCapabilities.getApiStatus());
            if (railsPortCapabilities.getApiStatus() == "offline" ) {
                return Response.status(Response.Status.SERVICE_UNAVAILABLE).entity("The local OSM API server is offline. Try again later").build();
            }

            // Pull OSM data from the local OSM API Db
            File localOSMFile = new File(workDir,"local.osm");
            params.setOutput(localOSMFile.getAbsolutePath());
            params.setPullUrl(RAILSPORT_PULL_URL);
            
            ExternalCommand getLocalOSM = grailCommandFactory.build(jobId,params,debugLevel,PullOSMDataCommand.class,this.getClass());
            workflow.add(getLocalOSM);

            // Pull OSM data from the real, internet, OSM API Db
            File internetOSMFile = new File(workDir,"internet.osm");
            params.setOutput(internetOSMFile.getAbsolutePath());
            params.setPullUrl(MAIN_OSMAPI_PULL_URL);
            
            ExternalCommand getInternetOSM = grailCommandFactory.build(jobId,params,debugLevel,PullOSMDataCommand.class,this.getClass());
            workflow.add(getInternetOSM);

            // Run the diff command.
            // TODO: We could possibly use ConflateCommand.java for this. If we setup the params for it - especially the 35 bazillion Hoot Options 
            params.setInput1(localOSMFile.getAbsolutePath());
            params.setInput2(internetOSMFile.getAbsolutePath());

            File geomDiffFile = new File(workDir,"diff.osc");
            params.setOutput(geomDiffFile.getAbsolutePath());
            
            ExternalCommand makeDiff = grailCommandFactory.build(jobId,params,debugLevel,RunDiffCommand.class,this.getClass());
            workflow.add(makeDiff);

            // Push to the local OSM API Db
            // TODO: The export/ApplyChangesetCommand.java command is hardcoded to push to OSMAPI_DB_URL. We could refactor it to 
            // take the DB URL as a parameter.
            // ExternalCommand applyGeomChange = applyChangesetCommandFactory.build(jobId,geomDiffFile,RAILSPORT_PUSH_URL,userName, debugLevel, this.getClass());
            params.setPushUrl(RAILSPORT_PUSH_URL);
            ExternalCommand applyGeomChange = grailCommandFactory.build(jobId,params,debugLevel,ApplyChangesetCommand.class,this.getClass());
            workflow.add(applyGeomChange);

            if (applyTags) {
                File tagDiffFile = new File(workDir,"diff.tags.osc");
                params.setOutput(tagDiffFile.getAbsolutePath());
                ExternalCommand applyTagChange = grailCommandFactory.build(jobId,params,debugLevel,ApplyChangesetCommand.class,this.getClass());
                workflow.add(applyTagChange);
            }

            // Now roll the dice and run everything.....
            jobProcessor.submitAsync(new Job(jobId, workflow.toArray(new Command[workflow.size()])));
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

        JSONObject json = new JSONObject();
        json.put("jobid", jobId);

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
     * @return Job ID
     *            Internally, this is the directory that the files are kept in
     */
    @POST
    @Path("/pullosm")
    // @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    // public Response pullOsmTst(GrailParams params,
    public Response pullOsm(@QueryParam("bbox") String bbox,
                            @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {

        String jobId = "grail_" + UUID.randomUUID().toString().replace("-", "");
        File workDir = new File(TEMP_OUTPUT_PATH, jobId);
        JSONObject json = new JSONObject();

        try {
            FileUtils.forceMkdir(workDir);
        }
        catch (IOException ioe) {
            logger.error("PullOSM: Error creating folder: {} ", workDir.getAbsolutePath(), ioe);
            return Response.status(Response.Status.INTERNAL_SERVER_ERROR).entity(ioe.getMessage()).build();
            // throw new WebApplicationException(ioe, Response.status(Response.Status.BAD_REQUEST).entity(ioe.getMessage()).build());
        }

        APICapabilities mainOsmApiCapabilities = getCapabilities(MAIN_OSMAPI_CAPABILITIES_URL);
        logger.info("PullOSM: mainOSMAPI status = " + mainOsmApiCapabilities.getApiStatus());
        if (mainOsmApiCapabilities.getApiStatus() == "offline" ) {
            return Response.status(Response.Status.SERVICE_UNAVAILABLE).entity("The main OSM API server is offline. Try again later").build();
        }

        APICapabilities railsPortCapabilities = getCapabilities(RAILSPORT_CAPABILITIES_URL);
        logger.info("PullOSM: railsPortAPI status = " + railsPortCapabilities.getApiStatus());
        if (railsPortCapabilities.getApiStatus() == "offline" ) {
            return Response.status(Response.Status.SERVICE_UNAVAILABLE).entity("The local OSM API server is offline. Try again later").build();
        }

        GrailParams params = new GrailParams();
        params.setBounds(bbox);

        try {
            List<Command> workflow = new LinkedList<>();

            // Pull data from the local OSM API Db
            File localOSMFile = new File(workDir,"local.osm");
            params.setOutput(localOSMFile.getAbsolutePath());
            params.setPullUrl(RAILSPORT_PULL_URL);
            ExternalCommand getLocalOSM = grailCommandFactory.build(jobId,params,debugLevel,PullOSMDataCommand.class,this.getClass());
            workflow.add(getLocalOSM);

            // Pull OSM data from the real, internet, OSM API Db
            File internetOSMFile = new File(workDir,"internet.osm");
            params.setOutput(internetOSMFile.getAbsolutePath());
            params.setPullUrl(MAIN_OSMAPI_PULL_URL);
            ExternalCommand getInternetOSM = grailCommandFactory.build(jobId,params,debugLevel,PullOSMDataCommand.class,this.getClass());
            workflow.add(getInternetOSM);

            jobProcessor.submitAsync(new Job(jobId, workflow.toArray(new Command[workflow.size()])));
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (IllegalArgumentException iae) {
            throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
        }
        catch (Exception e) {
            String msg = "Error while getting OSM data! Params: " + params.toString();
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        json.put("jobid", jobId);

        return Response.ok(json.toJSONString()).build();
    }


    /**
     * Run Hootenanny differential conflation
     *
     * POST hoot-services/grail/runndiff/[JobId]?DEBUG_LEVEL=<error,info,debug,verbose,trace>
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
    public Response runDiff(@PathParam("jobId") String jobDir,
                                @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {
        Response response;
        JSONObject json = new JSONObject();

        File workDir = new File(TEMP_OUTPUT_PATH, jobDir);
        File localOSMFile = new File(workDir,"local.osm");
        File internetOSMFile = new File(workDir,"internet.osm");

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

            File diffFile = new File(workDir,"diff.osc");
            params.setOutput(diffFile.getAbsolutePath());
            params.setInput1(localOSMFile.getAbsolutePath()); 
            params.setInput2(internetOSMFile.getAbsolutePath());

            ExternalCommand makeDiff = grailCommandFactory.build(newJobId,params,debugLevel,RunDiffCommand.class,this.getClass());

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
     * POST hoot-services/grail/runapply/[JobId]?USER_ID="Papa Smurf"&APPLY_TAGS=falseDEBUG_LEVEL=<error,info,debug,verbose,trace>
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
    @Path("/runapply/{jobId}")
    @Produces(MediaType.APPLICATION_JSON)
    public Response runApply(@PathParam("jobId") String jobDir,
                                @QueryParam("USER_ID") @DefaultValue("Hootenanny") String userId,
                                @QueryParam("APPLY_TAGS") @DefaultValue("false") Boolean applyTags,
                                @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {

        JSONObject json = new JSONObject();
        File workDir = new File(TEMP_OUTPUT_PATH, jobDir);

        if (!workDir.exists()) {
            logger.error("RunApply: jobDir {} does not exist.", workDir.getAbsolutePath());
            return Response.status(Response.Status.BAD_REQUEST).entity("Job " + jobDir + " does not exist.").build();
        }

        APICapabilities railsPortCapabilities = getCapabilities(RAILSPORT_CAPABILITIES_URL);
        logger.info("RunApply: railsPortAPI status = " + railsPortCapabilities.getApiStatus());
        if (railsPortCapabilities.getApiStatus() == "offline" ) {
            return Response.status(Response.Status.SERVICE_UNAVAILABLE).entity("The local OSM API server is offline. Try again later").build();
        }

        GrailParams params = new GrailParams();
        params.setUserId(userId);
        params.setPushUrl(RAILSPORT_PUSH_URL);

        File geomDiffFile = new File(workDir,"diff.osc");
        
        if (geomDiffFile.exists()) {
            params.setOutput(geomDiffFile.getAbsolutePath());

            try {
                    String geomJobId = "grail_" + UUID.randomUUID().toString().replace("-", "");
                    json.put("jobid:geomapply",geomJobId);

                    ExternalCommand applyGeomChange = grailCommandFactory.build(geomJobId,params,debugLevel,ApplyChangesetCommand.class,this.getClass());

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
                json.put("warning","No geometry diff file (diff.osc) in " + jobDir);
        }

        File tagDiffFile = new File(workDir,"diff.tags.osc");
        if (applyTags) {
            if (tagDiffFile.exists()) {
                params.setOutput(tagDiffFile.getAbsolutePath());

                try {
                        String tagJobId = "grail_" + UUID.randomUUID().toString().replace("-", "");
                        json.put("jobid:tagapply",tagJobId);

                        ExternalCommand applyTagChange = grailCommandFactory.build(tagJobId,params,debugLevel,ApplyChangesetCommand.class,this.getClass());

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
                    json.put("warning","Requested APPLY_TAGS but no tag diff file (diff.tags.osc) in " + jobDir);
            }
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
     * To retrieve the error file from job make a GET request.
     *
     * GET hoot-services/grail/error/[job id from export job]
     *
     * @param jobId
     *            job id. Internally, this is the directory that the files are kept in
     *
     * @return Octet stream
     */
    @GET
    @Path("errordiff/{id}")
    @Produces(MediaType.APPLICATION_OCTET_STREAM)
    public Response exportErrorFile(@PathParam("id") String jobId) {
        Response response;

        try {
            // NOTE: This will be changed so that it grabs the error.osc file after we figure out
            // what it is called.
            File exportFile = getFile(jobId, "diff", "osc");

            ResponseBuilder responseBuilder = Response.ok(exportFile);
            responseBuilder.header("Content-Disposition", "attachment; filename=diff.osc");

            response = responseBuilder.build();
        }
        catch (WebApplicationException e) {
            logger.error("ErrorDiff: File diff.osc does not exist in " + jobId);
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
        catch (IOException ioe)
        {
            throw new WebApplicationException(ioe, Response.status(Response.Status.BAD_REQUEST).entity(ioe.getMessage()).build());
            // ioe.printStackTrace();
        }
        catch (ParserConfigurationException e) {
        }
        catch (SAXException se)
        {
            se.printStackTrace();
        }

    return params;
    }

}