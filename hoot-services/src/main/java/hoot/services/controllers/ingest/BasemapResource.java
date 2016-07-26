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
import java.nio.charset.Charset;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.UUID;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.disk.DiskFileItemFactory;
import org.apache.commons.fileupload.servlet.ServletFileUpload;
import org.apache.commons.io.FileUtils;
import org.apache.commons.io.FilenameUtils;
import org.apache.commons.io.filefilter.WildcardFileFilter;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import hoot.services.controllers.job.JobControllerBase;


@Path("/basemap")
public class BasemapResource extends JobControllerBase {
    private static final Logger logger = LoggerFactory.getLogger(BasemapResource.class);
    private static final Map<String, String> basemapRasterExt;

    static {
        basemapRasterExt = new HashMap<>();
        String extStr = BASEMAP_RASTER_EXTENSIONS;
        String[] extList = extStr.toLowerCase().split(",");

        for (String ext : extList) {
            basemapRasterExt.put(ext, ext);
        }
    }

    public BasemapResource() {
        super(BASEMAP_RASTER_TO_TILES);
    }

    public static void createTileServerPath() throws IOException {
        File file = new File(TILE_SERVER_PATH);
        if (!file.exists()) {
            FileUtils.forceMkdir(file);
        }
    }

    /**
     * Upload dataset file and create TMS tiles.
     * 
     * POST hoot-services/ingest/basemap/upload?INPUT_NAME=MyBasemap
     * 
     * @param inputName
     *            Name of basemap
     * @param projection
     *            rojection to apply. defaults to EPSG:4326
     * @param request
     *            multipart data
     * @return JSON Array containing JSON of job id
     */
    @POST
    @Path("/upload")
    @Produces(MediaType.APPLICATION_JSON)
    public Response processUpload(@QueryParam("INPUT_NAME") String inputName,
                                  @QueryParam("PROJECTION") String projection,
                                  @Context HttpServletRequest request) {
        String groupId = UUID.randomUUID().toString();
        JSONArray jobsArr = new JSONArray();
        try {
            File uploadDir = new File(HOME_FOLDER + "/upload/");
            if (!uploadDir.exists()) {
                if (!uploadDir.mkdir()) {
                    throw new IOException("Error creating " + uploadDir.getAbsolutePath() + " directory!");
                }
            }

            String repFolderPath = HOME_FOLDER + "/upload/" + groupId;
            File dir = new File(repFolderPath);
            if (!dir.exists()) {
                if (!dir.mkdir()) {
                    throw new IOException("Error creating " + dir.getAbsolutePath() + " directory!");
                }
            }

            if (!ServletFileUpload.isMultipartContent(request)) {
                throw new ServletException("Content type is not multipart/form-data");
            }

            DiskFileItemFactory fileFactory = new DiskFileItemFactory();
            File filesDir = new File(repFolderPath);
            fileFactory.setRepository(filesDir);
            ServletFileUpload uploader = new ServletFileUpload(fileFactory);

            Map<String, String> uploadedFiles = new HashMap<>();
            Map<String, String> uploadedFilesPaths = new HashMap<>();
            List<FileItem> fileItemsList = uploader.parseRequest(request);

            for (FileItem fileItem : fileItemsList) {
                String fileName = fileItem.getName();

                String uploadedPath = repFolderPath + "/" + fileName;
                File file = new File(uploadedPath);
                fileItem.write(file);

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
                postJobRquest(jobId, argStr);

                JSONObject res = new JSONObject();
                res.put("jobid", jobId);
                res.put("name", bmName);

                jobsArr.add(res);
            }
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception ex) {
            String msg = "Error processing upload for: " + inputName;
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        return Response.ok(jobsArr.toJSONString(), MediaType.APPLICATION_JSON).build();
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
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception ex) {
            String message = "Error getting basemap list!";
            throw new WebApplicationException(ex, Response.serverError().entity(message).build());
        }

        // sort the list
        Map<String, JSONObject> sortedScripts = new TreeMap<>();
        for (Object file : filesList) {
            JSONObject cO = (JSONObject) file;
            String sName = cO.get("name").toString();
            sortedScripts.put(sName.toUpperCase(), cO);
        }

        basemapList.addAll(sortedScripts.values());

        return Response.ok(basemapList.toJSONString(), MediaType.APPLICATION_JSON).build();
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
    public Response enableBasemap(@QueryParam("NAME") String basemap, @QueryParam("ENABLE") String enable) {
        boolean doEnable = true;

        if ((enable != null) && (!enable.isEmpty())) {
            doEnable = Boolean.parseBoolean(enable);
        }

        try {
            toggleBaseMap(basemap, doEnable);
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception ex) {
            String msg = "Error enabling basemap: " + basemap;
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        JSONObject entity = new JSONObject();
        entity.put("name", basemap);
        entity.put("isenabled", doEnable);

        return Response.ok(entity.toJSONString(), MediaType.APPLICATION_JSON).build();
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
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception ex) {
            String msg = "Error deleting base map: " + basemap;
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        JSONObject entity = new JSONObject();
        entity.put("name", basemap);

        return Response.ok(entity.toJSONString(), MediaType.APPLICATION_JSON).build();
    }
}
