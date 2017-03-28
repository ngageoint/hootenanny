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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.export;

import static hoot.services.HootProperties.*;

import java.io.File;
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
import hoot.services.command.ExternalCommandManager;
import hoot.services.command.common.UnTARFileCommand;
import hoot.services.command.common.ZIPDirectoryCommand;
import hoot.services.job.Job;
import hoot.services.job.JobProcessor;
import hoot.services.utils.DbUtils;
import hoot.services.utils.JsonUtils;
import hoot.services.utils.XmlDocumentBuilder;
import hoot.services.wfs.WFSManager;


@Controller
@Path("/export")
@Transactional
public class ExportResource {
    private static final Logger logger = LoggerFactory.getLogger(ExportResource.class);

    @Autowired
    private JobProcessor jobProcessor;

    @Autowired
    private ExternalCommandManager externalCommandManager;

    @Autowired
    private ExportCommandFactory exportCommandFactory;


    public ExportResource() {}

    /**
     * Asynchronous export service.
     *
     * POST hoot-services/job/export/execute
     *
     * example request:
     *
     * {
     *   "translation":"MGCP.js", //Translation script name.
     *   "inputtype":"db",        //[db | file] db means input from hoot db will be used. file mean
     *                            //a file path will be specified.
     *   "input":"ToyTestA",      //Input name. for inputtype = db then specify name from hoot db.
     *                            //For inputtype=file, specify full path to a file.
     *   "outputtype":"gdb",      //[gdb | shp | wfs | osm_api_db | osc]. gdb will produce file gdb,
     *                            //shp will output shapefile. if outputtype = wfs then a wfs
     *                            //front end will be created. osm_api_db will derive and apply a
     *                            //changeset to an OSM API database, osc will derive changeset
     *                            //xml computing the the difference between the configured
     *                            //OSM API database and the specified input layer
     *   "removereview" : "false"
     * }
     *
     * @param params
     * @return Job ID
     */
    @POST
    @Path("/execute")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response export(String params,
                           @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {
        String jobId = "ex_" + UUID.randomUUID().toString().replace("-", "");

        try {
            Map<String, String> paramMap = JsonUtils.jsonToMap(params);

            String outputType = paramMap.get("outputtype");

            // Created scratch area for each export request.
            // This is where downloadable files will be stored and other intermediate artifacts.
            File workDir = new File(TEMP_OUTPUT_PATH, jobId);
            FileUtils.forceMkdir(workDir);

            List<Command> workflow = new LinkedList<>();

            if (outputType.equalsIgnoreCase("osm") || outputType.equalsIgnoreCase("osm.pbf")) {
                workflow.add(
                    () -> {
                        ExternalCommand exportOSMCommand = exportCommandFactory.build(jobId, paramMap,
                                debugLevel, ExportOSMCommand.class, this.getClass());

                        return externalCommandManager.exec(jobId, exportOSMCommand);
                    }
                );

                if (outputType.equalsIgnoreCase("osm")) {
                    Command zipCommand = getZIPCommand(jobId, workDir, outputType);
                    workflow.add(zipCommand);
                }
            }
            else if (outputType.equalsIgnoreCase("osc")) {
                workflow.add(
                    () -> {
                        ExternalCommand exportOSCCommand = exportCommandFactory.build(jobId, paramMap,
                                debugLevel, ExportOSCCommand.class, this.getClass());
                        return externalCommandManager.exec(jobId, exportOSCCommand);
                    }
                );
            }
            //TODO outputtype=osm_api_db may end up being obsolete with the addition of osc
            else if (outputType.equalsIgnoreCase("osm_api_db")) {
                workflow.add(
                    () -> {
                        ExternalCommand osmAPIDBDeriveChangesetCommand = exportCommandFactory.build(jobId, paramMap,
                                debugLevel, OSMAPIDBDeriveChangesetCommand.class, this.getClass());
                        return externalCommandManager.exec(jobId, osmAPIDBDeriveChangesetCommand);
                    }
                );

                workflow.add(
                    () -> {
                        ExternalCommand osmAPIDBApplyChangesetCommand = exportCommandFactory.build(jobId, paramMap,
                                debugLevel, OSMAPIDBApplyChangesetCommand.class, this.getClass());
                        return externalCommandManager.exec(jobId, osmAPIDBApplyChangesetCommand);
                    }
                );
            }
            else { //else Shape/FGDB
                if (outputType.equalsIgnoreCase("shp")) {
                    Command zipCommand = getZIPCommand(jobId, workDir, outputType);
                    workflow.add(zipCommand);
                }

                //TEMPLATE_PATH=$(HOOT_HOME)/translations-local/template
                //TDS61_TEMPLATE=$(TEMPLATE_PATH)/tds61.tgz
                //TDS40_TEMPLATE=$(TEMPLATE_PATH)/tds40.tgz

                // ifeq "$(append)" "true"
                //     ifeq "$(translation)" "translations/TDSv61.js"
                //         ifneq ("$(wildcard $(TDS61_TEMPLATE))","")
                //             mkdir -p $(OP_OUTPUT)
                //             tar -zxf $(TDS61_TEMPLATE) -C $(OP_OUTPUT)
                //         endif # Template Path
                //    else
                //       ifeq "$(translation)" "translations/TDSv40.js"
                //           ifneq ("$(wildcard $(TDS40_TEMPLATE))","")
                //               mkdir -p $(OP_OUTPUT)
                //               tar -zxf $(TDS40_TEMPLATE) -C $(OP_OUTPUT)
                //           endif # Template Path
                //       endif # Translations TDSv40
                //    endif # Else
                // endif # Append

                String translation = paramMap.get("translation");
                if (Boolean.valueOf(paramMap.get("append"))) {
                    //Appends data to a blank fgdb. The template is stored with the fouo translations.
                    File templateHome = new File(new File(HOME_FOLDER, "translations-local"), "template");
                    if (translation.equalsIgnoreCase("translations/TDSv61.js")) {
                        File tds61TemplatePath = new File(templateHome, "tds61.tgz");
                        if (tds61TemplatePath.exists()) {
                            workflow.add(
                                () -> {
                                    //tar -zxf $(TDS61_TEMPLATE) -C $(OP_OUTPUT)
                                    ExternalCommand untarFileCommand = new UnTARFileCommand(tds61TemplatePath, workDir, this.getClass());
                                    return externalCommandManager.exec(jobId, untarFileCommand);
                                }
                            );
                        }
                    }
                    else if (translation.equalsIgnoreCase("translations/TDSv40.js")) {
                        File tds40TemplatePath = new File(templateHome, "tds40.tgz");
                        if (tds40TemplatePath.exists()) {
                            workflow.add(
                                () -> {
                                    //OP_OUTPUT=$(outputfolder)/$(outputname).$(outputtype)
                                    //tar -zxf $(TDS40_TEMPLATE) -C $(OP_OUTPUT)
                                    ExternalCommand untarFileCommand = new UnTARFileCommand(tds40TemplatePath, workDir, this.getClass());
                                    return externalCommandManager.exec(jobId, untarFileCommand);
                                }
                            );
                        }
                    }
                }

                workflow.add(
                    () -> {
                        // ExportCommand
                        ExternalCommand exportCommand = exportCommandFactory.build(jobId, paramMap,
                                debugLevel, ExportCommand.class, this.getClass());
                        return externalCommandManager.exec(jobId, exportCommand);
                    }
                );

                Command zipCommand = getZIPCommand(jobId, workDir, outputType);
                workflow.add(zipCommand);
            }

            jobProcessor.process(new Job(jobId, workflow.toArray(new Command[workflow.size()])));
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception e) {
            String msg = "Error exporting data!  Params: " + params;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        JSONObject json = new JSONObject();
        json.put("jobid", jobId);

        return Response.ok(json.toJSONString()).build();
    }

    /**
     * To retrieve the output from job make Get request.
     *
     * GET hoot-services/job/export/[job id from export job]?outputname=[user
     * defined name]&removecache=[true | false]&ext=[file extension override
     * from zip]
     *
     * @param jobId
     *            job id
     * @param outputname
     *            parameter overrides the output file name with the user defined
     *            name. If not specified then defaults to job id as name.
     * @param removeCache
     *            parameter controls if the output file from export job should
     *            be deleted when Get request completes. - DISABLED
     * @param ext
     *            parameter overrides the file extension of the file being
     *            downloaded
     * @return Octet stream
     */
    @GET
    @Path("/{id}")
    @Produces(MediaType.APPLICATION_OCTET_STREAM)
    public Response exportFile(@PathParam("id") String jobId, @QueryParam("outputname") String outputname,
                               @QueryParam("removeCache") Boolean removeCache, @QueryParam("ext") String ext) {
        Response response;

        try {
            String fileExt = StringUtils.isEmpty(ext) ? "zip" : ext;
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
        finally {
            //TODO: removeCache didn't seem to be supported at all when the changes for #1263 were
            //made.  So, to keep from breaking some UI tests this will be left disabled for now.
//            if (removeCache) {
//                FileUtils.deleteQuietly(out);
//            }
        }

        return response;
    }

    /**
     * Returns the contents of an XML job output file
     *
     * * GET hoot-services/job/export/xml/[job id from export
     * job]?removeCache=[true | false]&ext=[file extension override from xml]
     *
     * @param jobId
     *            job id
     * @param removeCache
     *            parameter controls if the output file from export job should
     *            be deleted when Get request completes. - DISABLED
     * @param ext
     *            parameter overrides the file extension of the file being
     *            downloaded
     * @return job output XML
     * @throws WebApplicationException
     *             if the job with ID = id does not exist, the referenced job
     *             output file no longer exists, or the changeset is made up of
     *             multiple changeset files.
     */
    @GET
    @Path("/xml/{id}")
    @Produces(MediaType.TEXT_XML)
    public Response getXmlOutput(@PathParam("id") String jobId, @QueryParam("removeCache") Boolean removeCache,
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
        finally {
            //TODO: removeCache didn't seem to be supported at all when the changes for #1263 were
            //made.  So, to keep from breaking some UI tests this will be left disabled for now.
//            if (removeCache) {
//                FileUtils.deleteQuietly(out);
//            }
        }

        return response;
    }

    /**
     * Removes specified WFS resource.
     *
     * GET
     * hoot-services/job/export/wfs/remove/ex_eed379c0b9f7469d80ab32c71550883b
     *
     * //TODO: should be an HTTP DELETE
     *
     * @param id
     *            id of the wfs resource to remove
     * @return Removed id
     */
    @GET
    @Path("/wfs/remove/{id}")
    @Produces(MediaType.APPLICATION_JSON)
    public Response removeWfsResource(@PathParam("id") String id) {
        try {
            WFSManager.removeWfsResource(id);
            List<String> tbls = DbUtils.getTablesList(id);
            DbUtils.deleteTables(tbls);
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception e) {
            String msg = "Error removing WFS resource with id = " + id;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        JSONObject entity = new JSONObject();
        entity.put("id", id);

        return Response.ok(entity.toJSONString()).build();
    }

    /**
     * Lists all wfs resources.
     *
     * GET hoot-services/job/export/wfs/resources
     *
     * @return List of wfs resources
     */
    @GET
    @Path("/wfs/resources")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getWfsResources() {
        JSONArray wfsResources = new JSONArray();
        try {
            List<String> wfsServices = WFSManager.getAllWfsServices();

            if (wfsServices != null) {
                for (String wfsService : wfsServices) {
                    JSONObject resource = new JSONObject();
                    resource.put("id", wfsService);
                    wfsResources.add(resource);
                }
            }
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception ex) {
            String msg = "Error retrieving WFS resource list!";
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        return Response.ok(wfsResources.toJSONString()).build();
    }

    /**
     * Based on the existence of translation script extension, it will send the
     * list of available translations script for export.
     *
     * GET hoot-services/job/export/resources
     *
     * @return List of translation script resources
     */
    @GET
    @Path("/resources")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getExportResources() {
        String transExtPath = HOME_FOLDER + "/" + "/plugins-local/script/utp";
        if ((TRANSLATION_EXT_PATH != null) && (!TRANSLATION_EXT_PATH.isEmpty())) {
            transExtPath = TRANSLATION_EXT_PATH;
        }

        JSONArray exportResources = new JSONArray();
        try {
            JSONObject o = new JSONObject();
            o.put("name", "TDS");
            o.put("description", "LTDS 4.0");
            exportResources.add(o);

            o = new JSONObject();
            o.put("name", "MGCP");
            o.put("description", "MGCP");
            exportResources.add(o);

            File f = new File(transExtPath);
            if (f.exists() && f.isDirectory()) {
                o = new JSONObject();
                o.put("name", "UTP");
                o.put("description", "UTP");
                exportResources.add(o);
            }
        }
        catch (Exception e) {
            String msg = "Error retrieving exported resource list!";
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return Response.ok(exportResources.toJSONString()).build();
    }

    private static File getExportFile(String jobId, String outputname, String fileExt) {
        File exportFile = new File(new File(TEMP_OUTPUT_PATH, jobId), outputname + "." + fileExt);

        if (!exportFile.exists()) {
            String errorMsg = "Error exporting data.  Missing output file.";
            throw new WebApplicationException(Response.status(Status.NOT_FOUND).entity(errorMsg).build());
        }

        return exportFile;
    }

    private Command getZIPCommand(String jobId, File workDir, String outputType) {
        return () -> {
            // ZIP_OUTPUT = $(outputname).zip
            File targetZip = new File(workDir, jobId + ".zip");
            File sourceFolder = new File(workDir, jobId + "." + outputType);

            // OP_OUTPUT_FILE=$(outputname).$(outputtype)
            // cd "$(outputfolder)" && zip -r "$(ZIP_OUTPUT)" "$(OP_OUTPUT_FILE)"
            ExternalCommand zipDirectoryCommand = new ZIPDirectoryCommand(targetZip, sourceFolder, this.getClass());
            return externalCommandManager.exec(jobId, zipDirectoryCommand);
        };
    }
}
