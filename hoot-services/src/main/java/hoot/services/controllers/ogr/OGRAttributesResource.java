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

import static hoot.services.HootProperties.GET_OGR_ATTRIBUTE_SCRIPT;
import static hoot.services.HootProperties.HOME_FOLDER;
import static hoot.services.HootProperties.TEMP_OUTPUT_PATH;

import java.io.File;
import java.io.FileInputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.UUID;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.apache.commons.io.FileUtils;
import org.apache.commons.lang3.StringUtils;
import org.glassfish.jersey.media.multipart.FormDataMultiPart;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;

import hoot.services.controllers.job.JobControllerBase;
import hoot.services.utils.MultipartSerializer;

@Controller
@Path("/info")
public class OGRAttributesResource extends JobControllerBase {
    private static final Logger logger = LoggerFactory.getLogger(OGRAttributesResource.class);

    public OGRAttributesResource() {
        super(GET_OGR_ATTRIBUTE_SCRIPT);
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
    @Consumes(MediaType.MULTIPART_FORM_DATA)
    @Produces(MediaType.APPLICATION_JSON)
    public Response processUpload(@QueryParam("INPUT_TYPE") String inputType, FormDataMultiPart multiPart) {
        JSONObject response = new JSONObject();
        String jobId = UUID.randomUUID().toString();

        try {
            Map<String, String> uploadedFiles = new HashMap<>();
            Map<String, String> uploadedFilesPaths = new HashMap<>();

            MultipartSerializer.serializeUpload(jobId, inputType, uploadedFiles, uploadedFilesPaths, multiPart);

            List<String> filesList = new ArrayList<>();
            List<String> zipList = new ArrayList<>();

            for (Map.Entry<String, String> pairs : uploadedFiles.entrySet()) {
                String fName = pairs.getKey();
                String ext = pairs.getValue();

                String inputFileName = uploadedFilesPaths.get(fName);

                // If it is zip file then we crack open to see if it contains
                // FGDB. If so then we add the folder location and desired output name
                // which is fgdb name in the zip
                if (ext.equalsIgnoreCase("ZIP")) {
                    zipList.add(fName);
                    String zipFilePath = HOME_FOLDER + "/upload/" + jobId + "/" + inputFileName;
                    try (FileInputStream in = new FileInputStream(zipFilePath)) {
                        try (ZipInputStream zis = new ZipInputStream(in)) {
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

            postJobRequest(jobId, argStr);
        }
        catch (Exception e) {
            String msg = "Upload failed for job with id = " + jobId + ".  Cause: " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        response.put("jobId", jobId);

        return Response.ok(response.toJSONString()).build();
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
        String script;
        try {
            File file = new File(TEMP_OUTPUT_PATH, id + ".out");
            script = FileUtils.readFileToString(file, "UTF-8");

            if ("true".equalsIgnoreCase(doDelete)) {
                FileUtils.deleteQuietly(file);
            }
        }
        catch (Exception e) {
            String msg = "Error getting attribute: " + id + ".  Cause : " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return Response.ok(script).build();
    }
}
