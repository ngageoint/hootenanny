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
import java.util.Iterator;
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
    private static final Logger log = LoggerFactory.getLogger(BasemapResource.class);
    protected static String _tileServerPath = null;
    protected static String _ingestStagingPath = null;
    private static String homeFolder = null;

    protected static Map<String, String> _basemapRasterExt = null;

    static {
        try {
            homeFolder = HootProperties.getProperty("homeFolder");
            _tileServerPath = HootProperties.getProperty("tileServerPath");
            _ingestStagingPath = HootProperties.getProperty("ingestStagingPath");

            _basemapRasterExt = new HashMap<String, String>();
            String extStr = HootProperties.getProperty("BasemapRasterExtensions");
            String[] extList = extStr.toLowerCase().split(",");
            for (String ext : extList) {
                _basemapRasterExt.put(ext, ext);
            }

        }
        catch (Exception ex) {
            log.error(ex.getMessage());
        }
    }

    public BasemapResource() {
        try {
            if (processScriptName == null) {
                processScriptName = HootProperties.getProperty("BasemapRasterToTiles");
            }
        }
        catch (Exception ex) {
            log.error(ex.getMessage());
        }
    }

    public void createTileServerPath() {
        try {
            File f = new File(_tileServerPath);
            if (!f.exists()) {
                FileUtils.forceMkdir(f);
            }
        }
        catch (IOException iex) {
            log.error(iex.getMessage());
        }
        catch (Exception ex) {
            log.error(ex.getMessage());
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
    public Response processUpload(@QueryParam("INPUT_NAME") final String inputName,
            @QueryParam("PROJECTION") final String projection, @Context HttpServletRequest request) {
        String groupId = UUID.randomUUID().toString();
        JSONArray jobsArr = new JSONArray();
        try {
            File uploadDir = new File(homeFolder + "/upload/");
            uploadDir.mkdir();

            String repFolderPath = homeFolder + "/upload/" + groupId;
            File dir = new File(repFolderPath);
            dir.mkdir();

            if (!ServletFileUpload.isMultipartContent(request)) {
                throw new ServletException("Content type is not multipart/form-data");
            }
            DiskFileItemFactory fileFactory = new DiskFileItemFactory();
            File filesDir = new File(repFolderPath);
            fileFactory.setRepository(filesDir);
            ServletFileUpload uploader = new ServletFileUpload(fileFactory);

            Map<String, String> uploadedFiles = new HashMap<String, String>();
            Map<String, String> uploadedFilesPaths = new HashMap<String, String>();
            List<FileItem> fileItemsList = uploader.parseRequest(request);
            Iterator<FileItem> fileItemsIterator = fileItemsList.iterator();

            while (fileItemsIterator.hasNext()) {
                FileItem fileItem = fileItemsIterator.next();
                String fileName = fileItem.getName();

                String uploadedPath = repFolderPath + "/" + fileName;
                File file = new File(uploadedPath);
                fileItem.write(file);

                String[] nameParts = fileName.split("\\.");
                if (nameParts.length > 1) {
                    String extension = nameParts[nameParts.length - 1].toLowerCase();
                    String filename = nameParts[0];

                    if (_basemapRasterExt.get(extension) != null) {
                        uploadedFiles.put(filename, extension);
                        uploadedFilesPaths.put(filename, fileName);
                        log.debug("Saving uploaded:" + filename);
                    }

                }

            }

            Iterator it = uploadedFiles.entrySet().iterator();
            while (it.hasNext()) {
                String jobId = UUID.randomUUID().toString();
                Map.Entry pairs = (Map.Entry) it.next();
                String fName = pairs.getKey().toString();
                pairs.getValue().toString();

                log.debug("Preparing Basemap Ingest for :" + fName);
                String inputFileName = "";
                String bmName = inputName;

                if (bmName == null || bmName.length() == 0) {
                    bmName = fName;
                }

                inputFileName = uploadedFilesPaths.get(fName);

                try {
                    JSONArray commandArgs = new JSONArray();
                    JSONObject arg = new JSONObject();
                    arg.put("INPUT", "upload/" + groupId + "/" + inputFileName);
                    commandArgs.add(arg);

                    arg = new JSONObject();
                    arg.put("INPUT_NAME", bmName);
                    commandArgs.add(arg);

                    arg = new JSONObject();
                    arg.put("RASTER_OUTPUT_DIR", _tileServerPath + "/BASEMAP");
                    commandArgs.add(arg);

                    arg = new JSONObject();
                    if (projection != null && projection.length() > 0) {
                        arg.put("PROJECTION", projection);
                    }
                    else {
                        arg.put("PROJECTION", "auto");
                    }
                    commandArgs.add(arg);

                    arg = new JSONObject();
                    arg.put("JOB_PROCESSOR_DIR", _ingestStagingPath + "/BASEMAP");
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
                            Status.INTERNAL_SERVER_ERROR, log);
                }

            }

        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error processing upload: " + ex.getMessage(),
                    Status.INTERNAL_SERVER_ERROR, log);
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
        Map<String, JSONObject> sortedScripts = new TreeMap<String, JSONObject>();
        JSONArray filesList = new JSONArray();

        try {
            filesList = _getBasemapList();
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error getting base map list: " + ex.getMessage(),
                    Status.INTERNAL_SERVER_ERROR, log);
        }

        // sort the list
        for (Object o : filesList) {
            JSONObject cO = (JSONObject) o;
            String sName = cO.get("name").toString();
            sortedScripts.put(sName.toUpperCase(), cO);
        }

        retList.addAll(sortedScripts.values());

        return Response.ok(retList.toString(), MediaType.TEXT_PLAIN).build();
    }

    protected JSONArray _getBasemapList() throws Exception {
        JSONArray filesList = new JSONArray();
        File basmapDir = new File(_ingestStagingPath + "/BASEMAP");
        if (basmapDir.exists()) {

            String[] exts = new String[4];
            exts[0] = "processing";
            exts[1] = "enabled";
            exts[2] = "disabled";
            exts[3] = "failed";
            List<File> files = (List<File>) FileUtils.listFiles(basmapDir, exts, false);

            for (int i = 0; i < files.size(); i++) {
                File f = files.get(i);
                if (f.isFile()) {
                    String basemapName = f.getName();
                    String ext = FilenameUtils.getExtension(basemapName);
                    String name = FilenameUtils.getBaseName(basemapName);

                    String meta = FileUtils.readFileToString(f, Charset.defaultCharset());
                    JSONParser parser = new JSONParser();
                    JSONObject jsonMeta = (JSONObject) parser.parse(meta);
                    String jobId = jsonMeta.get("jobid").toString();

                    // Check for tilemapresource.xml in processed folder
                    JSONObject jsonExtent = new JSONObject();

                    String XmlPath = _tileServerPath + "/BASEMAP/" + name + "/tilemapresource.xml";
                    File fXmlFile = new File(XmlPath);
                    if (fXmlFile.exists()) {
                        try {
                            DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
                            DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
                            Document doc = dBuilder.parse(fXmlFile);
                            doc.getDocumentElement().normalize();
                            NodeList l = doc.getElementsByTagName("BoundingBox");
                            Node prop = l.item(0);
                            NamedNodeMap attr = prop.getAttributes();
                            if (null != attr) {
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

    protected void _toggleBaseMap(String bmName, boolean enable) throws Exception {
        // See ticket#6760
        // for file path manipulation
        String fileExt = "enabled";
        String targetExt = ".disabled";
        if (enable) {
            fileExt = "disabled";
            targetExt = ".enabled";
        }

        // We first verify that file exits in the folder first and then try to
        // get the source file
        File sourceFile = hoot.services.utils.FileUtils.getFileFromFolder(_ingestStagingPath + "/BASEMAP/", bmName,
                fileExt);

        if (sourceFile != null && sourceFile.exists()) {
            // if the source file exist then just swap the extension
            boolean res = sourceFile.renameTo(new File(_ingestStagingPath + "/BASEMAP/", bmName + targetExt));
            if (res == false) {
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
    public Response enableBasemap(@QueryParam("NAME") final String bmName, @QueryParam("ENABLE") final String enable) {

        boolean doEnable = true;
        try {

            if (enable != null && enable.length() > 0) {
                doEnable = Boolean.parseBoolean(enable);
            }

            _toggleBaseMap(bmName, doEnable);
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error enabling base map: " + bmName + " Error: " + ex.getMessage(),
                    Status.INTERNAL_SERVER_ERROR, log);
        }

        JSONObject resp = new JSONObject();
        resp.put("name", bmName);
        resp.put("isenabled", doEnable);
        return Response.ok(resp.toString(), MediaType.TEXT_PLAIN).build();
    }

    protected void _deleteBaseMap(String bmName) throws Exception {
        String controlFolder = _ingestStagingPath + "/BASEMAP/";

        File tileDir = hoot.services.utils.FileUtils.getSubFolderFromFolder(_tileServerPath + "/BASEMAP/", bmName);
        if (tileDir != null && tileDir.exists()) {
            FileUtils.forceDelete(tileDir);
        }

        File dir = new File(controlFolder);
        FileFilter fileFilter = new WildcardFileFilter(bmName + ".*");
        File[] files = dir.listFiles(fileFilter);
        for (int i = 0; i < files.length; i++) {
            File curFile = files[i];
            FileUtils.forceDelete(curFile);
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
    public Response deleteBasemap(@QueryParam("NAME") final String bmName) {
        try {
            _deleteBaseMap(bmName);
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error deleting base map: " + bmName + " Error: " + ex.getMessage(),
                    Status.INTERNAL_SERVER_ERROR, log);
        }

        JSONObject resp = new JSONObject();
        resp.put("name", bmName);

        return Response.ok(resp.toString(), MediaType.TEXT_PLAIN).build();
    }

}
