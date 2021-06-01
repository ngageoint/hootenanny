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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.ogr;

import static hoot.services.HootProperties.UPLOAD_FOLDER;

import java.io.File;
import java.util.Collection;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;
import java.util.UUID;

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

import org.apache.commons.io.FileUtils;
import org.apache.commons.io.FilenameUtils;
import org.apache.commons.io.filefilter.FileFilterUtils;
import org.apache.commons.io.filefilter.IOFileFilter;
import org.apache.commons.io.filefilter.TrueFileFilter;
import org.glassfish.jersey.media.multipart.FormDataMultiPart;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;

import hoot.services.command.Command;
import hoot.services.command.ExternalCommand;
import hoot.services.command.ExternalCommandManager;
import hoot.services.command.common.UnZIPFileCommand;
import hoot.services.job.Job;
import hoot.services.job.JobProcessor;
import hoot.services.job.JobType;
import hoot.services.models.db.Users;
import hoot.services.utils.MultipartSerializer;


@Controller
@Path("/info")
public class ExtractAttributesResource {
    private static final Logger logger = LoggerFactory.getLogger(ExtractAttributesResource.class);

    @Autowired
    private JobProcessor jobProcessor;

    @Autowired
    private ExtractAttributesCommandFactory getAttributesCommandFactory;

    @Autowired
    private ExternalCommandManager externalCommandManager;


    /**
     * This REST endpoint uploads multipart data from UI and then generates attribute output.
     *
     * @param inputType
     *            : [FILE | DIR] where FILE type should represents ZIP, SHP or OMS and DIR represents FGDB.
     * @param debugLevel
     *            debug level that can be configured for some of the commands that support it.
     */
    @POST
    @Path("/upload")
    @Consumes(MediaType.MULTIPART_FORM_DATA)
    @Produces(MediaType.APPLICATION_JSON)
    public Response processUpload(@Context HttpServletRequest request,
                                @QueryParam("INPUT_TYPE") String inputType,
                                @QueryParam("DEBUG_LEVEL") @DefaultValue("error") String debugLevel,
                                FormDataMultiPart multiPart) {
        Users user = Users.fromRequest(request);
        String jobId = UUID.randomUUID().toString();

        try {
            File workDir = new File(UPLOAD_FOLDER, jobId);
            FileUtils.forceMkdir(workDir);

            List<File> files = processFormDataMultiPart(multiPart, workDir, jobId, inputType);

            ExternalCommand getAttributesCommand = getAttributesCommandFactory.build(jobId, workDir, files, debugLevel, this.getClass());

            Command[] workflow = { getAttributesCommand };

            jobProcessor.submitAsync(new Job(jobId, user.getId(), workflow, JobType.ATTRIBUTES));
        }
        catch (IllegalArgumentException iae) {
            throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
        }
        catch (Exception e) {
            String msg = "Upload failed for job with id = " + jobId + ".  Cause: " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        JSONObject response = new JSONObject();
        response.put("jobId", jobId);

        return Response.ok(response.toJSONString()).build();
    }

    /**
     * This REST end point is for getting the result of get attribute upload operation
     *
     * @param jobId
     *            : The jobid from upload
     *
     * @param doDelete
     *            : [true | false] if true then it deletes the output resource after getting the result
     *
     * Example:http://localhost:8080/hoot-services/ogr/info/e43feae4-0644-47fd-a23c-6249e6 e7f7fb output: JSON of attributes
     *
     * @return JSON object of requested attribute
     */
    @GET
    @Path("/{id}")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getAttributes(@PathParam("id") String jobId,
                                  @QueryParam("deleteoutput") @DefaultValue("false") Boolean doDelete) {
        String json = null;
        try {
            File fileWithAttributes = ExtractAttributesUtils.getAttributesOutputFile(jobId);
            if (fileWithAttributes.exists()) {
                json = FileUtils.readFileToString(fileWithAttributes, "UTF-8");

                if (doDelete) {
                    FileUtils.deleteQuietly(fileWithAttributes);
                }
            }
        }
        catch (Exception e) {
            String msg = "Error retrieving attributes for job with ID = " + jobId;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return Response.ok(json).build();
    }

    private List<File> processFormDataMultiPart(FormDataMultiPart multiPart, File workDir, String jobId, String inputType) {
        List<File> uploadedFiles = MultipartSerializer.serializeUpload(multiPart, workDir);

        List<File> files = new LinkedList<>();

        if (!uploadedFiles.isEmpty()) {
            if (inputType.equalsIgnoreCase("DIR")) {
                // Assumption: all of the uploaded files are under the same folder in this case
                files.addAll(processFolder(uploadedFiles.get(0).getParentFile()));
            }
            else {
                for (File uploadedFile : uploadedFiles) {
                    String filename = uploadedFile.getName();
                    String extension = FilenameUtils.getExtension(filename);
                    String basename = FilenameUtils.getBaseName(filename);

                    //files can be one or more shapefiles, consisting of .shp, .shx, and .dbf components at a minimum;
                    //or a zip file containing one or more shapefiles, or a folder that is a file geodatabase
                    if (extension.equalsIgnoreCase("zip")) {
                        File folderWithUnzippedContents = new File(workDir, basename);

                        ExternalCommand unZIPFileCommand = new UnZIPFileCommand(uploadedFile, folderWithUnzippedContents, this.getClass());
                        unZIPFileCommand.setTrackable(Boolean.FALSE);
                        externalCommandManager.exec(jobId, unZIPFileCommand);

                        files.addAll(processFolder(folderWithUnzippedContents));
                    }
                    else {
                        //if we have shapefiles, only include shp refs
                        if (extension.equalsIgnoreCase("shp") || extension.equalsIgnoreCase("gpkg")) {
                            files.add(uploadedFile);
                        }
                    }
                }
            }
        }

        return files;
    }

    private static Collection<File> processFolder(File folder) {
        // Check if the folder represents an FGDB.
        IOFileFilter fileFilter = FileFilterUtils.nameFileFilter("gdb");
        Collection<File> gdbFiles = FileUtils.listFiles(folder, fileFilter, TrueFileFilter.INSTANCE);
        if (!gdbFiles.isEmpty()) {
            return Collections.singletonList(gdbFiles.iterator().next().getParentFile());
        }

        // Could be just a folder full of SHP files with their support files: .dbx and .shx
        fileFilter = FileFilterUtils.suffixFileFilter("shp");
        Collection<File> shpFiles = FileUtils.listFiles(folder, fileFilter, TrueFileFilter.INSTANCE);
        if (!shpFiles.isEmpty()) {
            return shpFiles;
        }

        return Collections.emptyList();
    }
}
