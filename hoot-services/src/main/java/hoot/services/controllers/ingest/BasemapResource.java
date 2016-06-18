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
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;
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
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import hoot.services.HootProperties;
import hoot.services.controllers.job.JobControllerBase;
import hoot.services.utils.ResourceErrorHandler;


@Path("/basemap")
public class BasemapResource extends JobControllerBase {
    private static final Logger logger = LoggerFactory.getLogger(BasemapResource.class);
    private static final String tileServerPath;
    private static final String ingestStagingPath;
    private static final String homeFolder;
    private static final Map<String, String> basemapRasterExt;

    static {
        homeFolder = HootProperties.getProperty("homeFolder");
        tileServerPath = HootProperties.getProperty("tileServerPath");
        ingestStagingPath = HootProperties.getProperty("ingestStagingPath");

        basemapRasterExt = new HashMap<>();
        String extStr = HootProperties.getProperty("BasemapRasterExtensions");
        String[] extList = extStr.toLowerCase().split(",");

        for (String ext : extList) {
            basemapRasterExt.put(ext, ext);
        }
    }

    public BasemapResource() {
        super(HootProperties.getProperty("BasemapRasterToTiles"));
    }

    public static void createTileServerPath() {
        try {
            File file = new File(tileServerPath);
            if (!file.exists()) {
                FileUtils.forceMkdir(file);
            }
        }
        catch (IOException iex) {
            logger.error(iex.getMessage());
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
    @Produces(MediaType.TEXT_PLAIN)
    public Response processUpload(@QueryParam("INPUT_NAME") String inputName,
                                  @QueryParam("PROJECTION") String projection,
                                  @Context HttpServletRequest request) {
        String groupId = UUID.randomUUID().toString();
        JSONArray jobsArr = new JSONArray();
        try {
            File uploadDir = new File(homeFolder + "/upload/");
            if (!uploadDir.exists()) {
                if (!uploadDir.mkdir()) {
                    throw new IOException("Error creating " + uploadDir.getAbsolutePath() + " directory!");
                }
            }

            String repFolderPath = homeFolder + "/upload/" + groupId;
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

            for (Object o : uploadedFiles.entrySet()) {
                String jobId = UUID.randomUUID().toString();
                Map.Entry pairs = (Map.Entry) o;
                String fName = pairs.getKey().toString();
                pairs.getValue().toString();

                logger.debug("Preparing Basemap Ingest for :{}", fName);
                String bmName = inputName;

                if ((bmName == null) || (bmName.isEmpty())) {
                    bmName = fName;
                }

                String inputFileName = uploadedFilesPaths.get(fName);

                try {
                    JSONArray commandArgs = new JSONArray();
                    JSONObject arg = new JSONObject();
                    arg.put("INPUT", "upload/" + groupId + "/" + inputFileName);
                    commandArgs.add(arg);

                    arg = new JSONObject();
                    arg.put("INPUT_NAME", bmName);
                    commandArgs.add(arg);

                    arg = new JSONObject();
                    arg.put("RASTER_OUTPUT_DIR", tileServerPath + "/BASEMAP");
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
                    arg.put("JOB_PROCESSOR_DIR", ingestStagingPath + "/BASEMAP");
                    commandArgs.add(arg);

                    String argStr = createBashPostBody(commandArgs);
                    postJobRquest(jobId, argStr);

                    JSONObject res = new JSONObject();
                    res.put("jobid", jobId);
                    res.put("name", bmName);

                    jobsArr.add(res);
                }
                catch (Exception ex) {
                    ResourceErrorHandler.handleError("Error processing upload: " + ex.getMessage(),
                            Status.INTERNAL_SERVER_ERROR, logger);
                }
            }
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error processing upload: " + ex.getMessage(),
                    Status.INTERNAL_SERVER_ERROR, logger);
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
    @Produces(MediaType.TEXT_PLAIN)
    public Response getBasemapList() {
        JSONArray retList = new JSONArray();
        Map<String, JSONObject> sortedScripts = new TreeMap<>();
        JSONArray filesList = new JSONArray();

        try {
            filesList = getBasemapListHelper();
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error getting base map list: " + ex.getMessage(),
                    Status.INTERNAL_SERVER_ERROR, logger);
        }

        // sort the list
        for (Object file : filesList) {
            JSONObject cO = (JSONObject) file;
            String sName = cO.get("name").toString();
            sortedScripts.put(sName.toUpperCase(), cO);
        }

        retList.addAll(sortedScripts.values());

        return Response.ok(retList.toString(), MediaType.TEXT_PLAIN).build();
    }

    private static JSONArray getBasemapListHelper() throws Exception {
        JSONArray filesList = new JSONArray();
        File basmapDir = new File(ingestStagingPath + "/BASEMAP");

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

                    String XmlPath = tileServerPath + "/BASEMAP/" + name + "/tilemapresource.xml";
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
                        catch (Exception e) {
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

    private static void toggleBaseMap(String bmName, boolean enable) throws Exception {
        // See ticket#6760
        // for file path manipulation
        String fileExt = "enabled";
        String targetExt = ".disabled";

        if (enable) {
            fileExt = "disabled";
            targetExt = ".enabled";
        }

        // We first verify that file exits in the folder first and then try to get the source file
        File sourceFile = hoot.services.utils.FileUtils.getFileFromFolder(ingestStagingPath + "/BASEMAP/", bmName, fileExt);

        if ((sourceFile != null) && sourceFile.exists()) {
            // if the source file exist then just swap the extension
            boolean renamed = sourceFile.renameTo(new File(ingestStagingPath + "/BASEMAP/", bmName + targetExt));

            if (!renamed) {
                throw new Exception("Failed to rename file:" + bmName + fileExt + " to " + bmName + targetExt);
            }
        }
        else {
            throw new Exception("Can not enable file:" + bmName + targetExt + ". It does not exist.");
        }
    }

    /**
     * Service method endpoint for enabling a basemap.
     * 
     * GET hoot-services/ingest/basemap/enable?NAME=abc&ENABLE=true
     * 
     * @param bmName
     *            Name of a basemap
     * @param enable
     *            true/false
     * @return JSON containing enable state
     */
    @GET
    @Path("/enable")
    @Produces(MediaType.TEXT_PLAIN)
    public Response enableBasemap(@QueryParam("NAME") String bmName, @QueryParam("ENABLE") String enable) {
        boolean doEnable = true;

        try {
            if ((enable != null) && (!enable.isEmpty())) {
                doEnable = Boolean.parseBoolean(enable);
            }

            toggleBaseMap(bmName, doEnable);
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error enabling base map: " + bmName + " Error: " + ex.getMessage(),
                    Status.INTERNAL_SERVER_ERROR, logger);
        }

        JSONObject resp = new JSONObject();
        resp.put("name", bmName);
        resp.put("isenabled", doEnable);

        return Response.ok(resp.toString(), MediaType.TEXT_PLAIN).build();
    }

    private static void deleteBaseMap(String bmName) throws Exception {
        String controlFolder = ingestStagingPath + "/BASEMAP/";

        File tileDir = hoot.services.utils.FileUtils.getSubFolderFromFolder(tileServerPath + "/BASEMAP/", bmName);
        if ((tileDir != null) && tileDir.exists()) {
            FileUtils.forceDelete(tileDir);
        }

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
     * @param bmName
     *            Name of a basemap
     * @return JSON containing enable state
     */
    @GET
    @Path("/delete")
    @Produces(MediaType.TEXT_PLAIN)
    public Response deleteBasemap(@QueryParam("NAME") String bmName) {
        try {
            deleteBaseMap(bmName);
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error deleting base map: " + bmName + " Error: " + ex.getMessage(),
                    Status.INTERNAL_SERVER_ERROR, logger);
        }

        JSONObject resp = new JSONObject();
        resp.put("name", bmName);

        return Response.ok(resp.toString(), MediaType.TEXT_PLAIN).build();
    }
}
