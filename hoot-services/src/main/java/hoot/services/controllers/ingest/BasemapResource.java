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
package hoot.services.controllers.ingest;

import static hoot.services.HootProperties.*;

import java.io.File;
import java.io.FileFilter;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.Charset;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.UUID;

import javax.ws.rs.Consumes;
import javax.ws.rs.DefaultValue;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.apache.commons.io.FileUtils;
import org.apache.commons.io.FilenameUtils;
import org.apache.commons.io.filefilter.WildcardFileFilter;
import org.glassfish.jersey.media.multipart.BodyPart;
import org.glassfish.jersey.media.multipart.FormDataMultiPart;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import hoot.services.command.Command;
import hoot.services.command.CommandResult;
import hoot.services.command.ExternalCommand;
import hoot.services.command.ExternalCommandManager;
import hoot.services.job.Job;
import hoot.services.job.JobProcessor;
import hoot.services.utils.JsonUtils;


@Controller
@Path("/basemap")
@Transactional
public class BasemapResource {
    private static final Logger logger = LoggerFactory.getLogger(BasemapResource.class);

    @Autowired
    private JobProcessor jobProcessor;

    @Autowired
    private ExternalCommandManager externalCommandManager;

    @Autowired
    private IngestBasemapCommandFactory ingestBasemapCommandFactory;


    /**
     * Upload dataset file and create TMS tiles.
     * 
     * POST hoot-services/ingest/basemap/upload?INPUT_NAME=MyBasemap
     * 
     * @param inputName
     *            Name of basemap
     * @param projection
     *            projection to apply. defaults to EPSG:4326
     * @param verboseOutput
     *            controls level of status output
     * @param multiPart
     *            multipart data
     * @return JSON Array containing JSON of job id
     */
    @POST
    @Path("/upload")
    @Consumes(MediaType.MULTIPART_FORM_DATA)
    @Produces(MediaType.APPLICATION_JSON)
    public Response processUpload(@QueryParam("INPUT_NAME") String inputName,
                                  @QueryParam("PROJECTION") String projection,
                                  @QueryParam("VERBOSE_OUTPUT") @DefaultValue("true") Boolean verboseOutput,
                                  FormDataMultiPart multiPart) {
        JSONArray jobsArr = new JSONArray();

        try {
            String jobId = UUID.randomUUID().toString();
            File workingDir = new File(UPLOAD_FOLDER, jobId);
            FileUtils.forceMkdir(workingDir);

            Map<String, String> uploadedFiles = new HashMap<>();
            Map<String, String> uploadedFilesPaths = new HashMap<>();
            List<BodyPart> fileItems = multiPart.getBodyParts();

            for (BodyPart fileItem : fileItems) {
                String fileName = fileItem.getContentDisposition().getFileName();

                try (InputStream fileStream = fileItem.getEntityAs(InputStream.class)) {
                    File file = new File(workingDir, fileName);
                    FileUtils.copyInputStreamToFile(fileStream, file);
                }

                String extension = FilenameUtils.getExtension(fileName).toLowerCase();
                String baseName = FilenameUtils.getBaseName(fileName);

                if (BASEMAP_RASTER_EXTENSIONS.contains(extension)) {
                    uploadedFiles.put(baseName, extension);
                    uploadedFilesPaths.put(baseName, fileName);
                    logger.debug("Saving uploaded:{}", fileName);
                }
                else {
                    logger.warn("Extension {} is not supported.  Skipping upload of {} file", extension, baseName);
                }
            }

            for (Map.Entry<String, String> pairs : uploadedFiles.entrySet()) {
                String fileName = pairs.getKey();

                logger.debug("Preparing Basemap Ingest for :{}", fileName);

                String basemapName = ((inputName == null) || (inputName.isEmpty())) ? fileName : inputName;
                String inputFileName = uploadedFilesPaths.get(fileName);

                File tileOutputDir = new File(BASEMAPS_TILES_FOLDER, basemapName);
                FileUtils.forceMkdir(tileOutputDir);

                Command[] workflow = {
                    () -> {
                        String extension = ".processing";
                        File file = new File(BASEMAPS_FOLDER, basemapName + extension);

                        try {
                            String json = "{\"jobid\":\"" + jobId + "\",\"path\":\"" + tileOutputDir + "\"}";
                            FileUtils.writeStringToFile(file, json, Charset.defaultCharset());
                        }
                        catch (IOException ioe) {
                            throw new RuntimeException("Error creating " + file, ioe);
                        }

                        File inputFile = new File(workingDir, inputFileName);

                        ExternalCommand ingestBasemapCommand = ingestBasemapCommandFactory.build(inputFile,
                                projection, tileOutputDir, verboseOutput, this.getClass());

                        CommandResult commandResult = externalCommandManager.exec(jobId, ingestBasemapCommand);

                        extension = (commandResult.failed() ? ".failed" : ".disabled");
                        File newFileName = new File(BASEMAPS_FOLDER, basemapName + extension);

                        try {
                            FileUtils.moveFile(file, newFileName);
                        }
                        catch (IOException ioe) {
                            throw new RuntimeException("Error moving " + file + " to " + newFileName.getAbsolutePath(), ioe);
                        }

                        return commandResult;
                    }
                };

                jobProcessor.process(new Job(jobId, workflow));

                JSONObject response = new JSONObject();
                response.put("jobid", jobId);
                response.put("name", basemapName);

                jobsArr.add(response);
            }
        }
        catch (Exception e) {
            String msg = "Error processing upload for: " + inputName + ".  Cause: " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return Response.ok(jobsArr.toJSONString()).build();
    }

    /**
     * Service method endpoint for retrieving the uploaded basemaps list.
     * 
     * GET hoot-services/ingest/basemap/getlist
     * 
     * @return JSON Array containing basemap information
     */
    @GET
    @Path("/getlist")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getBasemapList() {
        JSONArray basemaps;

        try {
            basemaps = getBasemapListHelper();
        }
        catch (Exception e) {
            String message = "Error getting basemap list.  Cause: " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(message).build());
        }

        // sort the list
        Map<String, JSONObject> sortedBasemaps = new TreeMap<>();
        for (Object basemap : basemaps) {
            JSONObject json = (JSONObject) basemap;
            String basemapName = json.get("name").toString();
            sortedBasemaps.put(basemapName.toUpperCase(), json);
        }

        JSONArray basemapList = new JSONArray();
        basemapList.addAll(sortedBasemaps.values());

        return Response.ok(basemapList.toJSONString()).build();
    }

    /**
     * Service method endpoint for enabling a basemap.
     * 
     * GET hoot-services/ingest/basemap/enable?NAME=abc&ENABLE=true
     * 
     * @param basemap
     *            Name of a basemap
     * @param enable
     *            true/false
     * @return JSON containing enable state
     */
    @GET
    @Path("/enable")
    @Produces(MediaType.APPLICATION_JSON)
    public Response enableBasemap(@QueryParam("NAME") String basemap, @QueryParam("ENABLE") Boolean enable) {
        try {
            toggleBaseMap(basemap, enable);
        }
        catch (Exception e) {
            String msg = "Error toggling basemap: " + basemap + ".  Cause: " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        JSONObject entity = new JSONObject();
        entity.put("name", basemap);
        entity.put("isenabled", String.valueOf(enable));

        return Response.ok(entity.toJSONString()).build();
    }

    /**
     * Service method endpoint for deleting a basemap.
     * 
     * GET hoot-services/ingest/basemap/delete?NAME=abc
     * 
     * //TODO: this should be an HTTP DELETE
     * 
     * @param basemap
     *            Name of a basemap
     * @return JSON containing enable state
     */
    @GET
    @Path("/delete")
    @Produces(MediaType.APPLICATION_JSON)
    public Response deleteBasemap(@QueryParam("NAME") String basemap) {
        try {
            deleteBaseMapHelper(basemap);
        }
        catch (Exception e) {
            String msg = "Error deleting base map: " + basemap + ".  Cause: " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        JSONObject entity = new JSONObject();
        entity.put("name", basemap);

        return Response.ok(entity.toJSONString()).build();
    }

    private static void toggleBaseMap(String basemapName, boolean enable) {
        String srcExt = "enabled";
        String destExt = "disabled";

        if (enable) {
            srcExt = "disabled";
            destExt = "enabled";
        }

        File srcFile = new File(BASEMAPS_FOLDER, basemapName + "." + srcExt);
        File destFile = new File(BASEMAPS_FOLDER, basemapName + "." + destExt);

        try {
            FileUtils.moveFile(srcFile, destFile);
        }
        catch (IOException ioe) {
            throw new RuntimeException("Error moving " + srcFile.getAbsolutePath() + " to " + destFile.getAbsolutePath(), ioe);
        }
    }

    private static void deleteBaseMapHelper(String basemapName) throws IOException {
        File tilesDir = new File(BASEMAPS_TILES_FOLDER, basemapName);
        if (tilesDir.exists()) {
            FileUtils.forceDelete(tilesDir);
        }

        FileFilter fileFilter = new WildcardFileFilter(basemapName + ".*");
        File[] files = new File(BASEMAPS_FOLDER).listFiles(fileFilter);
        if (files != null) {
            for (File file : files) {
                FileUtils.forceDelete(file);
            }
        }
    }

    private static JSONArray getBasemapListHelper() throws IOException {
        JSONArray filesList = new JSONArray();

        String[] exts = { "processing", "enabled", "disabled", "failed" };

        List<File> files = (List<File>) FileUtils.listFiles(new File(BASEMAPS_FOLDER), exts, false);

        for (File file : files) {
            if (file.isFile()) {
                String basemapName = file.getName();
                String ext = FilenameUtils.getExtension(basemapName);
                String name = FilenameUtils.getBaseName(basemapName);

                String metadata = FileUtils.readFileToString(file, Charset.defaultCharset());
                Map<String, String> map = JsonUtils.jsonToMap(metadata);
                String jobId = map.get("jobid");

                JSONObject jsonExtent = null;

                File xmlFile = new File(new File(BASEMAPS_TILES_FOLDER, name), "tilemapresource.xml");

                if (xmlFile.exists()) {
                    try {
                        DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
                        DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
                        Document doc = dBuilder.parse(xmlFile);
                        doc.getDocumentElement().normalize();
                        NodeList nodeList = doc.getElementsByTagName("BoundingBox");
                        Node prop = nodeList.item(0);
                        NamedNodeMap attr = prop.getAttributes();

                        if (attr != null) {
                            jsonExtent = new JSONObject();
                            jsonExtent.put("minx", Double.parseDouble(attr.getNamedItem("minx").getNodeValue()));
                            jsonExtent.put("miny", Double.parseDouble(attr.getNamedItem("miny").getNodeValue()));
                            jsonExtent.put("maxx", Double.parseDouble(attr.getNamedItem("maxx").getNodeValue()));
                            jsonExtent.put("maxy", Double.parseDouble(attr.getNamedItem("maxy").getNodeValue()));
                        }
                    }
                    catch (Exception e) {
                        logger.warn("Error parsing {} ", xmlFile, e);
                    }
                }

                JSONObject basemap = new JSONObject();
                basemap.put("name", name);
                basemap.put("status", ext);
                basemap.put("jobid", jobId);
                basemap.put("extent", jsonExtent);
                filesList.add(basemap);
            }
        }

        return filesList;
    }
}
