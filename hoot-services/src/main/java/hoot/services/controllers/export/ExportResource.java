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
import java.io.IOException;
import java.util.LinkedList;
import java.util.List;
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
import hoot.services.command.common.ZIPDirectoryContentsCommand;
import hoot.services.command.common.ZIPFileCommand;
import hoot.services.job.Job;
import hoot.services.job.JobProcessor;
import hoot.services.utils.XmlDocumentBuilder;


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
     *   "outputtype":"gdb",      //[gdb | shp | osm_api_db | osc]. gdb will produce file gdb,
     *                            //shp will output shapefile. osm_api_db will derive and apply a
     *                            //changeset to an OSM API database . osc will derive changeset
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
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response export(ExportParams params,
                           @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {
        String jobId = "ex_" + UUID.randomUUID().toString().replace("-", "");

        try {
            String outputType = params.getOutputType();
            String outputName = !StringUtils.isBlank(params.getOutputName()) ? params.getOutputName() : jobId;

            // Created scratch area for each export request.
            // This is where downloadable files will be stored and other intermediate artifacts.
            File workDir = new File(TEMP_OUTPUT_PATH, jobId);
            FileUtils.forceMkdir(workDir);

            List<Command> workflow = new LinkedList<>();

            if (outputType.equalsIgnoreCase("osm") || outputType.equalsIgnoreCase("osm.pbf")) {
                workflow.add(
                    () -> {
                        ExternalCommand exportOSMCommand = exportCommandFactory.build(jobId, params,
                                debugLevel, ExportOSMCommand.class, this.getClass());
                        return externalCommandManager.exec(jobId, exportOSMCommand);
                    }
                );

                if (outputType.equalsIgnoreCase("osm")) {
                    Command zipCommand = getZIPCommand(jobId, workDir, outputName, outputType);
                    workflow.add(zipCommand);
                }
            }
            // As of 04/03/2017, OSC support is not fully implemented yet.  This REST controller might not
            // even be the right place to host it.
            else if (outputType.equalsIgnoreCase("osc")) {
                workflow.add(
                    () -> {
                        ExternalCommand exportOSCCommand = exportCommandFactory.build(jobId, params,
                                debugLevel, ExportOSCCommand.class, this.getClass());
                        return externalCommandManager.exec(jobId, exportOSCCommand);
                    }
                );
            }
            //TODO outputtype=osm_api_db may end up being obsolete with the addition of osc
            else if (outputType.equalsIgnoreCase("osm_api_db")) {
                workflow.add(
                    () -> {
                        ExternalCommand osmAPIDBDeriveChangesetCommand = exportCommandFactory.build(jobId, params,
                                debugLevel, OSMAPIDBDeriveChangesetCommand.class, this.getClass());
                        return externalCommandManager.exec(jobId, osmAPIDBDeriveChangesetCommand);
                    }
                );

                workflow.add(
                    () -> {
                        ExternalCommand osmAPIDBApplyChangesetCommand = exportCommandFactory.build(jobId, params,
                                debugLevel, OSMAPIDBApplyChangesetCommand.class, this.getClass());
                        return externalCommandManager.exec(jobId, osmAPIDBApplyChangesetCommand);
                    }
                );
            }
            else { //else Shape/FGDB

                if (params.getAppend()) {
                    //Appends data to a blank fgdb. The template is stored with the fouo translations.

                    //$(HOOT_HOME)/translations-local/template
                    File templateHome = new File(new File(HOME_FOLDER, "translations-local"), "template");

                    String translation = params.getTranslation();
                    File tdsTemplate = null;

                    if (translation.equalsIgnoreCase("translations/TDSv61.js")) {
                        tdsTemplate = new File(templateHome, "tds61.tgz");
                    }
                    else if (translation.equalsIgnoreCase("translations/TDSv40.js")) {
                        tdsTemplate = new File(templateHome, "tds40.tgz");
                    }

                    if ((tdsTemplate != null) && tdsTemplate.exists()) {
                        File tdsTemplatePath = tdsTemplate;
                        workflow.add(
                            () -> {
                                File outputDir = new File(workDir, outputName + "."     + outputType.toLowerCase());
                                try {
                                    FileUtils.forceMkdir(outputDir);
                                }
                                catch (IOException ioe) {
                                    throw new RuntimeException("Error creating directory: " + outputDir.getAbsolutePath(), ioe);
                                }
                                ExternalCommand untarFileCommand = new UnTARFileCommand(tdsTemplatePath, outputDir, this.getClass());
                                return externalCommandManager.exec(jobId, untarFileCommand);
                            }
                        );
                    }
                }

                workflow.add(
                    () -> {
                        ExternalCommand exportCommand = exportCommandFactory.build(jobId, params,
                                debugLevel, ExportCommand.class, this.getClass());
                        return externalCommandManager.exec(jobId, exportCommand);
                    }
                );

                Command zipCommand = getZIPCommand(jobId, workDir, outputName, outputType);
                if (zipCommand != null) {
                    workflow.add(zipCommand);
                }
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

        return response;
    }

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
            JSONObject json = new JSONObject();
            json.put("name", "TDS");
            json.put("description", "LTDS 4.0");
            exportResources.add(json);

            json = new JSONObject();
            json.put("name", "MGCP");
            json.put("description", "MGCP");
            exportResources.add(json);

            File file = new File(transExtPath);
            if (file.exists() && file.isDirectory()) {
                json = new JSONObject();
                json.put("name", "UTP");
                json.put("description", "UTP");
                exportResources.add(json);
            }
        }
        catch (Exception e) {
            String msg = "Error retrieving exported resource list!";
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return Response.ok(exportResources.toJSONString()).build();
    }

    private Command getZIPCommand(String jobId, File workDir, String outputName, String outputType) {
        File targetZIP = new File(workDir, outputName + ".zip");

        if (outputType.equalsIgnoreCase("SHP")) {
            return () -> {
                // pwd = present working directory during execution of ZIPDirectoryContentsCommand
                File pwd = new File(workDir, outputName);

                ExternalCommand zipDirectoryCommand = new ZIPDirectoryContentsCommand(targetZIP, pwd, this.getClass());
                return externalCommandManager.exec(jobId, zipDirectoryCommand);
            };
        }
        else if (outputType.equalsIgnoreCase("OSM")) {
            return () -> {
                // pwd = present working directory during execution of ZIPFileCommand
                File pwd = workDir;

                String fileToCompress = outputName + "." + outputType;

                ExternalCommand zipFileCommand = new ZIPFileCommand(targetZIP, pwd, fileToCompress, this.getClass());
                return externalCommandManager.exec(jobId, zipFileCommand);
            };
        }
        else if (outputType.equalsIgnoreCase("GDB")) {
            return () -> {
                // pwd = present working directory during execution of ZIPDirectoryContentsCommand
                File pwd = workDir;

                ExternalCommand zipDirectoryCommand = new ZIPDirectoryContentsCommand(targetZIP, pwd, this.getClass());
                return externalCommandManager.exec(jobId, zipDirectoryCommand);
            };
        }
        else {
            return null;
        }
    }

    private static File getExportFile(String jobId, String outputName, String fileExt) {
        File exportFile = new File(new File(TEMP_OUTPUT_PATH, jobId), outputName + "." + fileExt);

        if (!exportFile.exists()) {
            String errorMsg = "Error exporting data.  Missing output file.";
            throw new WebApplicationException(Response.status(Status.NOT_FOUND).entity(errorMsg).build());
        }

        return exportFile;
    }
}
