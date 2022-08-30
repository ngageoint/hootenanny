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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.export;

import static hoot.services.HootProperties.CHANGESETS_FOLDER;
import static hoot.services.HootProperties.HOME_FOLDER;
import static hoot.services.HootProperties.TEMP_OUTPUT_PATH;
import static hoot.services.HootProperties.TRANSLATION_EXT_PATH;
import static hoot.services.models.db.QMaps.maps;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.AbstractMap;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Stack;
import java.util.UUID;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

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
import javax.ws.rs.core.Response.Status;
import javax.ws.rs.core.StreamingOutput;
import javax.xml.transform.dom.DOMSource;

import org.apache.commons.io.FileUtils;
import org.apache.commons.io.IOUtils;
import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import com.querydsl.core.Tuple;

import hoot.services.command.Command;
import hoot.services.command.common.ZIPDirectoryContentsCommand;
import hoot.services.command.common.ZIPFileCommand;
import hoot.services.controllers.osm.map.FolderResource;
import hoot.services.controllers.osm.map.MapResource;
import hoot.services.job.Job;
import hoot.services.job.JobProcessor;
import hoot.services.job.JobType;
import hoot.services.models.db.Users;
import hoot.services.utils.DbUtils;
import hoot.services.utils.XmlDocumentBuilder;

@Controller
@Path("/export")
@Transactional
public class ExportResource {
    private static final Logger logger = LoggerFactory.getLogger(ExportResource.class);
    @Autowired
    private JobProcessor jobProcessor;

    @Autowired
    private UserAwareExportCommandFactory userAwareExportCommandFactory;

    public ExportResource() {}

    private Class<? extends ExportCommand> getCommand(String outputType) {
        Class<? extends ExportCommand> exportCommand = null;
        if (outputType.startsWith("tiles")) {
            exportCommand = CalculateTilesCommand.class;
        } else if (outputType.startsWith("alpha")) {
            exportCommand = AlphaShapeCommand.class;
        } else {
            exportCommand = ExportCommand.class;
        }
        return exportCommand;
    }

    private ExportCommand getCommand(Users user, String jobId, ExportParams params, String debugLevel) {
        return userAwareExportCommandFactory.build(jobId, params, debugLevel,
                getCommand(params.getOutputType()), this.getClass(), user);
    }

    /**
     * Asynchronous export service.
     *
     * POST hoot-services/job/export/execute
     *
     * example request:
     *
     * {
     *   "translation":"MGCP.js", //Translation script name.
     *   "inputtype":"db",        //[db | dbs | folder| file]
     *                            // - db means input from hoot db will be used.
     *                            // - dbs means list of maps in hoot db are used.
     *                            // - folder means all files in a hoot db folder are used.
     *                            // - file means a file path will be specified.
     *   "input":"ToyTestA",      //Input name. for inputtype = db then specify name from hoot db.
     *                            //For inputtype=file, specify full path to a file.
     *   "outputtype":"gdb",      //[gdb | shp | tiles]. gdb will produce an ESRI file
     *                             geodatabase, shp will output shapefile. tiles outputs a GeoJSON file
     *                            //containing partitioned concentrations of node tiles
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
            @Context HttpServletRequest request,
            @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {
        Users user = Users.fromRequest(request);
        params.setUserEmail(user.getEmail());
        String jobId = "ex_" + UUID.randomUUID().toString().replace("-", "");

        try {
            String inputType = params.getInputType();
            String outputName = !StringUtils.isBlank(params.getOutputName()) ? params.getOutputName() : jobId;

            // Created scratch area for each export request.
            // This is where downloadable files will be stored and other intermediate artifacts.
            File workDir = new File(TEMP_OUTPUT_PATH, jobId);
            FileUtils.forceMkdir(workDir);

            List<Command> workflow = new LinkedList<>();

            if (inputType.equalsIgnoreCase("folder")) {
                params.setInputType("db"); // make folder input really a db input...
                Long rootfolder = Long.parseLong(params.getInput());

                // create stack with the folder id and output path as values starting with main export folder
                // Having the output path is helpful for making the child directories since you can just take parent path
                // and append child directory name to it
                Stack<Map.Entry<Long, String>> folderList = new Stack<Map.Entry<Long, String>>(){{
                    push(new AbstractMap.SimpleEntry(rootfolder, workDir.getPath()));
                }};

                while(!folderList.empty()) {
                    Map.Entry<Long, String> currentFolder = folderList.pop();
                    Long currentId = currentFolder.getKey();

                    // get all maps in folder...
                    for (Tuple mapInfo: FolderResource.getFolderMaps(user, currentId)) {
                        params.setInput(Long.toString(mapInfo.get(maps.id)));
                        params.setOutputName(mapInfo.get(maps.displayName));
                        params.setOutputPath(currentFolder.getValue());
                        workflow.add(getCommand(user, jobId, params, debugLevel));
                    }

                    // get all children folders
                    for (long folderId : DbUtils.getChildrenFolders(currentId)) {
                        File childDir = new File(currentFolder.getValue(), FolderResource.getFolderName(folderId));
                        FileUtils.forceMkdir(childDir);
                        folderList.push(new AbstractMap.SimpleEntry(folderId, childDir.getPath()));
                    }
                }

                Command zipCommand = getZIPCommand(workDir, FolderResource.getFolderName(rootfolder), params.getOutputType());
                workflow.add(zipCommand);
                params.setInputType("folder");

            } else if (inputType.equalsIgnoreCase("dbs")) {
                params.setInputType("db");

                for (String mapid: Arrays.asList(params.getInput().split(","))) { // make list of all maps in input
                    // These functions ensure the map + containing folder are either owned by the user -or- public.
                    MapResource.getMapForUser(user, mapid, false, false);

                    params.setInput(mapid);
                    params.setOutputName(DbUtils.getDisplayNameById(Long.valueOf(mapid)));
                    workflow.add(getCommand(user, jobId, params, debugLevel)); // convert each map...
                }

                Command zipCommand = getZIPCommand(workDir, outputName, params.getOutputType()); // zip maps into single folder...
                workflow.add(zipCommand);

            //generates density tiles and alpha shape and clips the first with the second
            } else if (params.getOutputType().startsWith("alpha.tiles")) {
                params.setOutputType("tiles.geojson");
                workflow.add(getCommand(user, jobId, params, debugLevel));
                params.setOutputType("alpha.shp");
                workflow.add(getCommand(user, jobId, params, debugLevel));
                params.setInputType("file");
                params.setTagOverrides(params.getInput());
                params.setInput(workDir.getAbsolutePath() + "/" + params.getOutputName());
                Command ogrClipCommand = new OgrClipCommand(jobId, params, this.getClass());
                workflow.add(ogrClipCommand);
                Command ogrFormatCommand = new OgrFormatCommand(jobId, params, this.getClass());
                workflow.add(ogrFormatCommand);
                Command sedSourceCommand = new SedSourceCommand(jobId, params, this.getClass());
                workflow.add(sedSourceCommand);
                Command zipCommand = new ZIPFileCommand(new File(workDir, params.getOutputName() + ".zip"), workDir, outputName + ".alpha.tiles.geojson", this.getClass());
                workflow.add(zipCommand);

            } else if (inputType.equalsIgnoreCase("changesets")) {
                File jobDir = new File(CHANGESETS_FOLDER, params.getInput());

                Command zipCommand = getZIPCommand(workDir, jobDir, params.getInput()); // zip osc files in a changesets folder
                workflow.add(zipCommand);

            } else {
                // These functions ensure the map + containing folder are either owned by the user -or- public.
                MapResource.getMapForUser(user, params.getInput(), false, false);

                workflow.add(getCommand(user, jobId, params, debugLevel));
                Command zipCommand = getZIPCommand(workDir, outputName, params.getOutputType());
                workflow.add(zipCommand);
            }

            //Record output name in job status tag so output can be retrieved from job panel
            Map<String, Object> jobStatusTags = new HashMap<>();
            jobStatusTags.put("outputname", outputName);
            jobStatusTags.put("outputtype", params.getOutputType());

            jobProcessor.submitAsync(new Job(jobId, user.getId(), workflow.toArray(new Command[workflow.size()]), JobType.EXPORT, jobStatusTags));
        }
        catch (WebApplicationException wae) {
            logger.error(wae.getMessage(), wae);
            throw wae;
        }
        catch (IllegalArgumentException iae) {
            logger.error(iae.getMessage(), iae);
            throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
        }
        catch (Exception e) {
            logger.error(e.getMessage(), e);
            String msg = "Error exporting data!  Params: " + params;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        java.util.Map<String, Object> json = new HashMap<String, Object>();
        json.put("jobid", jobId);

        //Update last accessed timestamp for db datasets on export
        if (params.getInputType().equalsIgnoreCase("db")) {
            Long mapid;
            try { //Hoot2x sends mapid, Hoot1 still sends map name so handle both for now
                mapid = Long.parseLong(params.getInput());
            } catch (NumberFormatException ex) {
                mapid = DbUtils.getMapIdByName(params.getInput(), user.getId());
            }
            MapResource.updateLastAccessed(mapid);
        }

        return Response.ok(json).build();
    }

    /**
     * To retrieve the output from job make GET request.
     *
     * GET hoot-services/job/export/[job id from export job]?outputname=[file to download]&ext=[file extension if not zip]
     *
     * @param jobId
     *            job id
     * @param outputname
     *            the output file name to be downloaded if not the job id
     * @param ext
     *            the file extension of the file being zipped and
     *            downloaded (if not an existing zip file)
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
            ResponseBuilder responseBuilder;
            //Do some zipping if fileExt is not zip
            if (!fileExt.equalsIgnoreCase("zip")) {
                StreamingOutput stream = new StreamingOutput() {
                    @Override
                    public void write(OutputStream output) throws IOException {
                        try {
                            ZipOutputStream zip = new ZipOutputStream(new BufferedOutputStream(output));
                            zip.putNextEntry(new ZipEntry(exportFile.getName()));
                            FileInputStream fis = new FileInputStream(exportFile);
                            IOUtils.copy(fis, zip);
                            zip.closeEntry();
                            fis.close();
                            zip.close();
                        } finally {
                            if( output != null ) {
                                output.flush();
                                output.close();
                            }
                        }
                    }
                };
                responseBuilder = Response.ok(stream);
            } else {
                responseBuilder = Response.ok(exportFile);
            }
            responseBuilder.header("Content-Disposition", "attachment; filename=\""+ outFileName + ".zip\"");
            responseBuilder.header("Content-Type", "application/zip");
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
     * Returns the contents of a geojson job output file
     * This is used by Hoot1 UI when creating a conflation task project
     * using `hoot.bin task-grid --node-density` to build a k-d tree output geojson
     * of task areas with roughly equal numbers of features
     *
     * GET hoot-services/job/export/geojson/[job id from exportjob]
     *
     * @param jobId
     *            job id
     * @param ext
     *            parameter overrides the file extension of the file being downloaded
     *            but is also used to identify the export file on the server, so not really!
     * @return job output geojson
     *
     * @throws WebApplicationException
     *             if the job with ID = id does not exist, the referenced job
     *             output file no longer exists.
     */
    @GET
    @Path("/geojson/{id}")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getGeoJsonOutput(@PathParam("id") String jobId,
                                 @QueryParam("outputname") String outputname,
                                 @QueryParam("ext") String ext) {
        Response response;

        try {
            File out = getExportFile(jobId, StringUtils.isEmpty(outputname) ? jobId : outputname, StringUtils.isEmpty(ext) ? "geojson" : ext);
            response = Response.ok(FileUtils.readFileToString(out, "UTF-8")).build();
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
        String transExtPath = HOME_FOLDER + "/" + "/translations-local/script/utp";
        if ((TRANSLATION_EXT_PATH != null) && (!TRANSLATION_EXT_PATH.isEmpty())) {
            transExtPath = TRANSLATION_EXT_PATH;
        }

        List<java.util.Map<String,Object>> exportResources = new ArrayList<java.util.Map<String,Object>>(2);
        try {
            java.util.Map<String, Object> json = new HashMap<String, Object>();
            json.put("name", "TDS");
            json.put("description", "LTDS 4.0");
            exportResources.add(json);

            json = new HashMap<String, Object>();
            json.put("name", "MGCP");
            json.put("description", "MGCP");
            exportResources.add(json);

            File file = new File(transExtPath);
            if (file.exists() && file.isDirectory()) {
                json = new HashMap<String, Object>();
                json.put("name", "UTP");
                json.put("description", "UTP");
                exportResources.add(json);
            }
        }
        catch (Exception e) {
            String msg = "Error retrieving exported resource list!";
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return Response.ok(exportResources).build();
    }

    private Command getZIPCommand(File workDir, String outputName) {
        return getZIPCommand(workDir, workDir, outputName);
    }

    private Command getZIPCommand(File workDir, String outputName, String outputType) {
        return getZIPCommand(workDir, workDir, outputName, outputType);
    }

    private Command getZIPCommand(File workDir, File inputDir, String outputName) {
        File targetZIP = new File(workDir, outputName + ".zip");
        return new ZIPDirectoryContentsCommand(targetZIP, inputDir, this.getClass());
    }

    private Command getZIPCommand(File workDir, File inputDir, String outputName, String outputType) {
        File targetZIP = new File(workDir, outputName + "." + outputType + ".zip");
        return new ZIPDirectoryContentsCommand(targetZIP, inputDir, this.getClass());
    }

    private static File getExportFile(String jobId, String outputName, String fileExt) {
        String path = TEMP_OUTPUT_PATH;
        if (fileExt.equalsIgnoreCase("osc"))
            path = CHANGESETS_FOLDER;

        //check if the outputName already ends in the requested outputExt
        String outputExt = outputName.substring(outputName.lastIndexOf(".") + 1);
        if (!fileExt.equalsIgnoreCase(outputExt))
            outputName += "." + fileExt;

        File exportFile = new File(new File(path, jobId), outputName);

        if (!exportFile.exists()) {
            String errorMsg = "Error exporting data.  Missing output file.";
            throw new WebApplicationException(Response.status(Status.NOT_FOUND).entity(errorMsg).build());
        }

        return exportFile;
    }
}
