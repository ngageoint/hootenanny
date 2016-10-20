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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
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
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import hoot.services.controllers.job.JobControllerBase;

@Controller
@Path("/basemap")
@Transactional
public class BasemapResource extends JobControllerBase {
    private static final Logger logger = LoggerFactory.getLogger(BasemapResource.class);
    private static final Map<String, String> basemapRasterExt;

    static {
        basemapRasterExt = new HashMap<>();
        String[] extList = BASEMAP_RASTER_EXTENSIONS.toLowerCase().split(",");

        for (String ext : extList) {
            basemapRasterExt.put(ext, ext);
        }
    }

    public BasemapResource() {
        super(BASEMAP_RASTER_TO_TILES);
    }

    /**
     * Upload dataset file and create TMS tiles.
     * 
     * POST hoot-services/ingest/basemap/upload?INPUT_NAME=MyBasemap
     * 
     * @param inputName
     *            Name of basemap
     * @param projection
     *            projection to apply. defaults to EPSG:4326
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
                                  FormDataMultiPart multiPart) {
        String groupId = UUID.randomUUID().toString();
        JSONArray jobsArr = new JSONArray();

        try {
            String repFolderPath = UPLOAD_FOLDER + "/" + groupId;
            File dir = new File(repFolderPath);
            if (!dir.exists()) {
                if (!dir.mkdir()) {
                    throw new IOException("Error creating " + dir.getAbsolutePath() + " directory!");
                }
            }

            Map<String, String> uploadedFiles = new HashMap<>();
            Map<String, String> uploadedFilesPaths = new HashMap<>();
            List<BodyPart> fileItems = multiPart.getBodyParts();

            for (BodyPart fileItem : fileItems) {
                String fileName = fileItem.getContentDisposition().getFileName();

                try (InputStream fileStream = fileItem.getEntityAs(InputStream.class)) {
                    String uploadedPath = repFolderPath + "/" + fileName;
                    File file = new File(uploadedPath);
                    FileUtils.copyInputStreamToFile(fileStream, file);
                }

                String[] nameParts = fileName.split("\\.");
                if (nameParts.length > 1) {
                    String extension = nameParts[nameParts.length - 1].toLowerCase();
                    String filename = nameParts[0];

                    if (basemapRasterExt.get(extension) != null) {
                        uploadedFiles.put(filename, extension);
                        uploadedFilesPaths.put(filename, fileName);
                        logger.debug("Saving uploaded:{}", filename);
                    }
                }
            }

            for (Map.Entry<String, String> pairs : uploadedFiles.entrySet()) {
                String jobId = UUID.randomUUID().toString();
                String fName = pairs.getKey();

                logger.debug("Preparing Basemap Ingest for :{}", fName);
                String bmName = inputName;

                if ((bmName == null) || (bmName.isEmpty())) {
                    bmName = fName;
                }

                String inputFileName = uploadedFilesPaths.get(fName);

                JSONArray commandArgs = new JSONArray();
                JSONObject arg = new JSONObject();
                arg.put("INPUT", "upload/" + groupId + "/" + inputFileName);
                commandArgs.add(arg);

                arg = new JSONObject();
                arg.put("INPUT_NAME", bmName);
                commandArgs.add(arg);

                arg = new JSONObject();
                arg.put("RASTER_OUTPUT_DIR", TILE_SERVER_PATH + "/BASEMAP");
                commandArgs.add(arg);

                arg = new JSONObject();
                if ((projection != null) && (!projection.isEmpty())) {
                    arg.put("PROJECTION", projection);
                }
                else {
                    arg.put("PROJECTION", "auto");
                }
                commandArgs.add(arg);

                arg = new JSONObject();
                arg.put("JOB_PROCESSOR_DIR", INGEST_STAGING_PATH + "/BASEMAP");
                commandArgs.add(arg);

                String argStr = createBashPostBody(commandArgs);
                postJobRequest(jobId, argStr);

                JSONObject res = new JSONObject();
                res.put("jobid", jobId);
                res.put("name", bmName);

                jobsArr.add(res);
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
        JSONArray basemapList = new JSONArray();
        JSONArray filesList;

        try {
            filesList = getBasemapListHelper();
        }
        catch (Exception e) {
            String message = "Error getting basemap list.  Cause: " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(message).build());
        }

        // sort the list
        Map<String, JSONObject> sortedScripts = new TreeMap<>();
        for (Object file : filesList) {
            JSONObject cO = (JSONObject) file;
            String sName = cO.get("name").toString();
            sortedScripts.put(sName.toUpperCase(), cO);
        }

        basemapList.addAll(sortedScripts.values());

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
            String msg = "Error enabling basemap: " + basemap + ".  Cause: " + e.getMessage();
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

    private static void toggleBaseMap(String bmName, boolean enable) throws IOException {
        // See ticket#6760
        // for file path manipulation
        String fileExt = "enabled";
        String targetExt = ".disabled";

        if (enable) {
            fileExt = "disabled";
            targetExt = ".enabled";
        }

        // We first verify that file exits in the folder first and then try to get the source file
        File sourceFile = hoot.services.utils.FileUtils.getFileFromFolder(INGEST_STAGING_PATH + "/BASEMAP/", bmName, fileExt);

        if ((sourceFile != null) && sourceFile.exists()) {
            // if the source file exist then just swap the extension
            boolean renamed = sourceFile.renameTo(new File(INGEST_STAGING_PATH + "/BASEMAP/", bmName + targetExt));

            if (!renamed) {
                throw new IOException("Failed to rename file:" + bmName + fileExt + " to " + bmName + targetExt);
            }
        }
        else {
            throw new IOException("Can not enable file:" + bmName + targetExt + ". It does not exist.");
        }
    }

    private static void deleteBaseMapHelper(String bmName) throws IOException {
        File tileDir = hoot.services.utils.FileUtils.getSubFolderFromFolder(TILE_SERVER_PATH + "/BASEMAP/", bmName);
        if ((tileDir != null) && tileDir.exists()) {
            FileUtils.forceDelete(tileDir);
        }

        String controlFolder = INGEST_STAGING_PATH + "/BASEMAP/";
        File dir = new File(controlFolder);
        FileFilter fileFilter = new WildcardFileFilter(bmName + ".*");
        File[] files = dir.listFiles(fileFilter);
        if (files != null) {
            for (File curFile : files) {
                FileUtils.forceDelete(curFile);
            }
        }
    }

    private static JSONArray getBasemapListHelper() throws IOException, ParseException {
        JSONArray filesList = new JSONArray();
        File basmapDir = new File(INGEST_STAGING_PATH + "/BASEMAP");

        if (basmapDir.exists()) {
            String[] exts = {"processing", "enabled", "disabled", "failed"};

            List<File> files = (List<File>) FileUtils.listFiles(basmapDir, exts, false);

            for (File file : files) {
                if (file.isFile()) {
                    String basemapName = file.getName();
                    String ext = FilenameUtils.getExtension(basemapName);
                    String name = FilenameUtils.getBaseName(basemapName);

                    String meta = FileUtils.readFileToString(file, Charset.defaultCharset());
                    JSONParser parser = new JSONParser();
                    JSONObject jsonMeta = (JSONObject) parser.parse(meta);
                    String jobId = jsonMeta.get("jobid").toString();

                    // Check for tilemapresource.xml in processed folder
                    JSONObject jsonExtent = new JSONObject();

                    String XmlPath = TILE_SERVER_PATH + "/BASEMAP/" + name + "/tilemapresource.xml";
                    File fXmlFile = new File(XmlPath);
                    if (fXmlFile.exists()) {
                        try {
                            DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
                            DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
                            Document doc = dBuilder.parse(fXmlFile);
                            doc.getDocumentElement().normalize();
                            NodeList nodeList = doc.getElementsByTagName("BoundingBox");
                            Node prop = nodeList.item(0);
                            NamedNodeMap attr = prop.getAttributes();
                            if (attr != null) {
                                jsonExtent.put("minx", Double.parseDouble(attr.getNamedItem("minx").getNodeValue()));
                                jsonExtent.put("miny", Double.parseDouble(attr.getNamedItem("miny").getNodeValue()));
                                jsonExtent.put("maxx", Double.parseDouble(attr.getNamedItem("maxx").getNodeValue()));
                                jsonExtent.put("maxy", Double.parseDouble(attr.getNamedItem("maxy").getNodeValue()));
                            }
                            else {
                                jsonExtent = null;
                            }
                        }
                        catch (Exception ignored) {
                            jsonExtent = null;
                        }
                    }

                    JSONObject oBaseMap = new JSONObject();
                    oBaseMap.put("name", name);
                    oBaseMap.put("status", ext);
                    oBaseMap.put("jobid", jobId);
                    oBaseMap.put("extent", jsonExtent);
                    filesList.add(oBaseMap);
                }
            }
        }

        return filesList;
    }
}
