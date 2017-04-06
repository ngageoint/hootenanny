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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.ogr;

import static hoot.services.HootProperties.TEMP_OUTPUT_PATH;
import static hoot.services.HootProperties.UPLOAD_FOLDER;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.UUID;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

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

import org.apache.commons.io.FileUtils;
import org.apache.commons.io.FilenameUtils;
import org.glassfish.jersey.media.multipart.FormDataMultiPart;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;

import hoot.services.command.Command;
import hoot.services.command.CommandResult;
import hoot.services.command.ExternalCommand;
import hoot.services.command.ExternalCommandManager;
import hoot.services.command.InternalCommand;
import hoot.services.command.InternalCommandManager;
import hoot.services.command.common.DeleteFolderCommand;
import hoot.services.command.common.UnZIPFileCommand;
import hoot.services.job.Job;
import hoot.services.job.JobProcessor;
import hoot.services.utils.MultipartSerializer;


@Controller
@Path("/info")
public class OGRAttributesResource {
    private static final Logger logger = LoggerFactory.getLogger(OGRAttributesResource.class);

    @Autowired
    private JobProcessor jobProcessor;

    @Autowired
    private ExternalCommandManager externalCommandManager;

    @Autowired
    private InternalCommandManager internalCommandManager;

    @Autowired
    private GetAttributesCommandFactory getAttributesCommandFactory;


    /**
     * This REST endpoint uploads multipart data from UI and then generates attribute output.
     *
     * @param inputType
     *            : [FILE | DIR] where FILE type should represents zip,shp or OMS and DIR represents FGDB
     * @param debugLevel
     *            debug level of some of the commands that are executed during the method's execution
     */
    @POST
    @Path("/upload")
    @Consumes(MediaType.MULTIPART_FORM_DATA)
    @Produces(MediaType.APPLICATION_JSON)
    public Response processUpload(@QueryParam("INPUT_TYPE") String inputType,
                                  @QueryParam("DEBUG_LEVEL") @DefaultValue("error") String debugLevel,
                                  FormDataMultiPart multiPart) {
        String jobId = UUID.randomUUID().toString();

        try {
            File workFolder = new File(UPLOAD_FOLDER, jobId);
            FileUtils.forceMkdir(workFolder);

            List<File> fileList = new ArrayList<>();
            List<File> zipList = new ArrayList<>();

            processFormDataMultiPart(fileList, zipList, inputType, multiPart, workFolder);

            List<Command> workflow = new LinkedList<>();

            for (File zip : zipList) {
                workflow.add(
                    () -> {
                        // OP_INPUT=$(HOOT_HOME)/userfiles/tmp/upload/$(jobid)
                        // bash $(HOOT_HOME)/scripts/util/unzipfiles.sh "$(INPUT_ZIPS)" "$(OP_INPUT)"
                        File targetFolder = new File(workFolder, FilenameUtils.getBaseName(zip.getName()));
                        ExternalCommand unZIPFileCommand = new UnZIPFileCommand(zip, targetFolder, this.getClass());
                        return externalCommandManager.exec(jobId, unZIPFileCommand);
                    }
                );
            }

            workflow.add(
                () -> {
                    // OP_OUTPUT=$(HOOT_HOME)/userfiles/tmp/$(jobid).out
                    //File outputFile = new File(TEMP_OUTPUT_PATH, jobId + ".out");
                    ExternalCommand getAttributesCommand = getAttributesCommandFactory.build(fileList, debugLevel, this.getClass());
                    CommandResult commandResult = externalCommandManager.exec(jobId, getAttributesCommand);

                    File outputFile = getAttributesOutputFile(jobId);
                    try {
                        FileUtils.write(outputFile, commandResult.getStdout(), Charset.defaultCharset());
                    }
                    catch (IOException ioe) {
                        throw new RuntimeException("Error writing attributes to: " + outputFile.getAbsolutePath(), ioe);
                    }

                    return commandResult;
                }
            );

            workflow.add(
                () -> {
                    // cd .. && rm -rf "$(OP_INPUT)"
                    InternalCommand deleteFolderCommand = new DeleteFolderCommand(jobId, workFolder);
                    return internalCommandManager.exec(jobId, deleteFolderCommand);
                }
            );

            jobProcessor.process(new Job(jobId, workflow.toArray(new Command[workflow.size()])));
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
    public Response getAttributes(@PathParam("id") String jobId, @QueryParam("deleteoutput") String doDelete) {
        String json = null;
        try {
            File fileWithAttributes = getAttributesOutputFile(jobId);
            if (fileWithAttributes.exists()) {
                json = FileUtils.readFileToString(fileWithAttributes, "UTF-8");

                if (Boolean.valueOf(doDelete)) {
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

    private static void processFormDataMultiPart(List<File> fileList, List<File> zipList,
                                                 String inputType, FormDataMultiPart multiPart, File workFolder)
            throws IOException {
        Map<File, String> uploadedFiles = MultipartSerializer.serializeUpload(inputType, multiPart, workFolder);

        for (Map.Entry<File, String> pairs : uploadedFiles.entrySet()) {
            File inputFile = pairs.getKey();
            String baseName = FilenameUtils.getBaseName(inputFile.getName());
            String extension = pairs.getValue();

            // If it is zip file then we crack open to see if it contains FGDB.
            // If so then we add the folder location and desired output name which is fgdb name in the zip.
            if (extension.equalsIgnoreCase("ZIP")) {
                File zipFile = inputFile;
                zipList.add(zipFile);

                try (FileInputStream fin = new FileInputStream(zipFile)) {
                    try (ZipInputStream zis = new ZipInputStream(fin)) {
                        ZipEntry zipEntry = zis.getNextEntry();
                        while (zipEntry != null) {
                            String zipEntryName = zipEntry.getName();

                            if (zipEntry.isDirectory()) {
                                if (zipEntryName.toLowerCase().endsWith(".gdb/")
                                        || zipEntryName.toLowerCase(Locale.ENGLISH).endsWith(".gdb")) {
                                    String fgdbZipName = zipEntryName;
                                    if (zipEntryName.toLowerCase(Locale.ENGLISH).endsWith(".gdb/")) {
                                        fgdbZipName = zipEntryName.substring(0, zipEntryName.length() - 1);
                                    }
                                    fileList.add(new File(new File(workFolder, fgdbZipName), fgdbZipName));
                                }
                            }
                            else {
                                if (zipEntryName.toLowerCase(Locale.ENGLISH).endsWith(".shp")) {
                                    fileList.add(new File(new File(workFolder, baseName), zipEntryName));
                                }
                            }

                            zipEntry = zis.getNextEntry();
                        }
                    }
                }
            }
            else {
                fileList.add(inputFile);
            }
        }
    }

    private static File getAttributesOutputFile(String jobId) {
        return new File(TEMP_OUTPUT_PATH, jobId + ".out");
    }
}
