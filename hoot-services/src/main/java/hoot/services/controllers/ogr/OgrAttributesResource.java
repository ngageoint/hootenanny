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
package hoot.services.controllers.ogr;

import java.io.File;
import java.io.FileInputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.UUID;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import javax.servlet.http.HttpServletRequest;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;

import org.apache.commons.io.FileUtils;
import org.apache.commons.lang3.StringUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.HootProperties;
import hoot.services.controllers.job.JobControllerBase;
import hoot.services.ingest.MultipartSerializer;
import hoot.services.utils.ResourceErrorHandler;


@Path("/info")
public class OgrAttributesResource extends JobControllerBase {
    private static final Logger log = LoggerFactory.getLogger(OgrAttributesResource.class);
    private String homeFolder = null;

    public OgrAttributesResource() {
        try {
            if (processScriptName == null) {
                processScriptName = HootProperties.getProperty("GetOgrAttributeScript");
            }

            homeFolder = HootProperties.getProperty("homeFolder");
        }
        catch (Exception ex) {
            log.error(ex.getMessage());
        }
    }

    /**
     * This rest endpoint uploads multipart data from UI and then generates
     * attribute output Example:
     * http://localhost:8080//hoot-services/ogr/info/upload?INPUT_TYPE=DIR
     * Output: {"jobId":"e43feae4-0644-47fd-a23c-6249e6e7f7fb"}
     * 
     * After getting the jobId, one can track the progress through job status
     * rest end point Example:
     * http://localhost:8080/hoot-services/job/status/e43feae4-0644-47fd-a23c-
     * 6249e6e7f7fb Output:
     * {"jobId":"e43feae4-0644-47fd-a23c-6249e6e7f7fb","statusDetail":null,
     * "status":"complete"}
     * 
     * Once status is "complete" Result attribute can be obtained through
     * Example:http://localhost:8080/hoot-services/ogr/info/e43feae4-0644-47fd-
     * a23c-6249e6e7f7fb output: JSON of attributes
     * 
     * @param inputType
     *            : [FILE | DIR] where FILE type should represents zip,shp or
     *            OMS and DIR represents FGDB
     */
    @POST
    @Path("/upload")
    @Produces(MediaType.TEXT_PLAIN)
    public Response processUpload(@QueryParam("INPUT_TYPE") final String inputType,
            @Context HttpServletRequest request) {
        JSONObject res = new JSONObject();
        String jobId = UUID.randomUUID().toString();

        try {
            log.debug("Starting file upload for ogr attribute Process");
            Map<String, String> uploadedFiles = new HashMap<String, String>();
            Map<String, String> uploadedFilesPaths = new HashMap<String, String>();

            MultipartSerializer ser = new MultipartSerializer();
            ser.serializeUpload(jobId, inputType, uploadedFiles, uploadedFilesPaths, request);

            List<String> filesList = new ArrayList<String>();
            List<String> zipList = new ArrayList<String>();

            Iterator it = uploadedFiles.entrySet().iterator();
            while (it.hasNext()) {

                Map.Entry pairs = (Map.Entry) it.next();
                String fName = pairs.getKey().toString();
                String ext = pairs.getValue().toString();

                String inputFileName = "";

                inputFileName = uploadedFilesPaths.get(fName);

                // If it is zip file then we crack open to see if it contains
                // FGDB.
                // If so then we add the folder location and desired output name
                // which is fgdb name in the
                // zip
                if (ext.equalsIgnoreCase("ZIP")) {
                    zipList.add(fName);
                    String zipFilePath = homeFolder + "/upload/" + jobId + "/" + inputFileName;
                    ZipInputStream zis = null;
                    try {
                        zis = new ZipInputStream(new FileInputStream(zipFilePath));
                        ZipEntry ze = zis.getNextEntry();

                        while (ze != null) {

                            String zipName = ze.getName();
                            if (ze.isDirectory()) {
                                if (zipName.toLowerCase(Locale.ENGLISH).endsWith(".gdb/")
                                        || zipName.toLowerCase(Locale.ENGLISH).endsWith(".gdb")) {
                                    String fgdbZipName = zipName;
                                    if (zipName.toLowerCase(Locale.ENGLISH).endsWith(".gdb/")) {
                                        fgdbZipName = zipName.substring(0, zipName.length() - 1);
                                    }
                                    filesList.add("\"" + fName + "/" + fgdbZipName + "\"");
                                }
                            }
                            else {
                                if (zipName.toLowerCase(Locale.ENGLISH).endsWith(".shp")) {
                                    filesList.add("\"" + fName + "/" + zipName + "\"");
                                }
                            }
                            ze = zis.getNextEntry();
                        }
                    }
                    finally {
                        if (zis != null) {
                            try {
                                zis.closeEntry();
                            }
                            finally {
                                zis.close();
                            }
                        }
                    }
                }
                else {
                    filesList.add("\"" + inputFileName + "\"");
                }
            }

            String mergeFilesList = StringUtils.join(filesList.toArray(), ' ');
            String mergedZipList = StringUtils.join(zipList.toArray(), ';');
            JSONArray params = new JSONArray();
            JSONObject param = new JSONObject();
            param.put("INPUT_FILES", mergeFilesList);
            params.add(param);
            param = new JSONObject();
            param.put("INPUT_ZIPS", mergedZipList);
            params.add(param);

            String argStr = createPostBody(params);
            postJobRquest(jobId, argStr);

        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Failed upload: " + ex.toString(), Status.INTERNAL_SERVER_ERROR, log);
        }
        res.put("jobId", jobId);
        return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
    }

    /**
     * This rest end point is for getting the result of get attribute upload
     * operation
     * 
     * @param id
     *            : The jobid from upload
     * @param doDelete
     *            : [true | false] if true then it deletes the output resource
     *            after getting the result
     * 
     *            Example:http://localhost:8080/hoot-services/ogr/info/e43feae4-
     *            0644-47fd-a23c-6249e6 e7f7fb output: JSON of attributes
     * 
     * @return JSON object of requested attribute
     */
    @GET
    @Path("/{id}")
    @Produces(MediaType.TEXT_PLAIN)
    public Response getAttributes(@PathParam("id") String id, @QueryParam("deleteoutput") String doDelete) {
        String script = "";
        try {
            File f = new File(homeFolder + "/tmp/" + id + ".out");
            script = FileUtils.readFileToString(f, "UTF-8");

            if (doDelete != null && doDelete.equalsIgnoreCase("true")) {
                FileUtils.deleteQuietly(f);
            }

        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error getting attribute: " + id + " Error: " + ex.getMessage(),
                    Status.INTERNAL_SERVER_ERROR, log);
        }
        return Response.ok(script, MediaType.TEXT_PLAIN).build();
    }

}
