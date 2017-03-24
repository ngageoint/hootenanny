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
import java.util.HashMap;
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
     * This rest endpoint uploads multipart data from UI and then generates attribute output.
     * Example: http://localhost:8080//hoot-services/ogr/info/upload?INPUT_TYPE=DIR
     * Output: {"jobId":"e43feae4-0644-47fd-a23c-6249e6e7f7fb"}
     * 
     * After getting the jobId, one can track the progress through job status rest end point
     * Example: http://localhost:8080/hoot-services/job/status/e43feae4-0644-47fd-a23c-6249e6e7f7fb
     * Output: {"jobId":"e43feae4-0644-47fd-a23c-6249e6e7f7fb","statusDetail":null, "status":"complete"}
     * 
     * Once status is "complete", result attribute can be obtained through.
     * Example:http://localhost:8080/hoot-services/ogr/info/e43feae4-0644-47fd-a23c-6249e6e7f7fb
     * Output: JSON of attributes
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

            processFormDataMultiPart(fileList, zipList, jobId, inputType, multiPart, workFolder);

            List<Command> commands = new LinkedList<>();

            for (File zip : zipList) {
                commands.add(
                    () -> {
                        // OP_INPUT=$(HOOT_HOME)/userfiles/tmp/upload/$(jobid)
                        // bash $(HOOT_HOME)/scripts/util/unzipfiles.sh "$(INPUT_ZIPS)" "$(OP_INPUT)"
                        File targetFolder = new File(workFolder, FilenameUtils.getBaseName(zip.getName()));
                        ExternalCommand unZIPFileCommand = new UnZIPFileCommand(zip, targetFolder, this.getClass());
                        return externalCommandManager.exec(jobId, unZIPFileCommand);
                    }
                );
            }

            commands.add(
                () -> {
                    // OP_OUTPUT=$(HOOT_HOME)/userfiles/tmp/$(jobid).out
                    //File outputFile = new File(TEMP_OUTPUT_PATH, jobId + ".out");
                    ExternalCommand getAttributesCommand = getAttributesCommandFactory.build(jobId, fileList, debugLevel, this.getClass());
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

            commands.add(
                () -> {
                    InternalCommand command = () -> {
                        CommandResult commandResult = new CommandResult();
                        commandResult.setJobId(jobId);

                        try {
                            // cd .. && rm -rf "$(OP_INPUT)"
                            // Do cleanup
                            FileUtils.forceDelete(workFolder);
                            commandResult.setExitCode(CommandResult.SUCCESS);
                        }
                        catch(IOException ioe) {
                            commandResult.setExitCode(CommandResult.FAILURE);
                            logger.error("Error deleting {} directory!", workFolder, ioe);
                        }

                        return commandResult;
                    };

                    return internalCommandManager.exec(jobId, command);
                }
            );

            jobProcessor.process(new Job(jobId, commands.toArray(new Command[commands.size()])));
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
     * This rest end point is for getting the result of get attribute upload operation
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

    private static void processFormDataMultiPart(List<File> fileList, List<File> zipList, String jobId,
                                                 String inputType, FormDataMultiPart multiPart, File workFolder)
            throws IOException {
        Map<String, String> uploadedFiles = new HashMap<>();
        Map<String, String> uploadedFilesPaths = new HashMap<>();

        MultipartSerializer.serializeUpload(jobId, inputType, uploadedFiles, uploadedFilesPaths, multiPart, workFolder);

        for (Map.Entry<String, String> pairs : uploadedFiles.entrySet()) {
            String name = pairs.getKey();
            String ext = pairs.getValue();
            String inputFileName = uploadedFilesPaths.get(name);

            // If it is zip file then we crack open to see if it contains FGDB.
            // If so then we add the folder location and desired output name which is fgdb name in the zip.
            if (ext.equalsIgnoreCase("ZIP")) {
                File zipFile = new File(workFolder, inputFileName);
                zipList.add(zipFile);

                try (FileInputStream fin = new FileInputStream(zipFile)) {
                    try (ZipInputStream zis = new ZipInputStream(fin)) {
                        ZipEntry zipEntry = zis.getNextEntry();
                        while (zipEntry != null) {
                            String zeName = zipEntry.getName();
                            if (zipEntry.isDirectory()) {
                                if (zeName.toLowerCase(Locale.ENGLISH).endsWith(".gdb/")
                                        || zeName.toLowerCase(Locale.ENGLISH).endsWith(".gdb")) {
                                    String fgdbZipName = zeName;
                                    if (zeName.toLowerCase(Locale.ENGLISH).endsWith(".gdb/")) {
                                        fgdbZipName = zeName.substring(0, zeName.length() - 1);
                                    }
                                    fileList.add(new File(new File(workFolder, name), fgdbZipName));
                                }
                            }
                            else {
                                if (zeName.toLowerCase(Locale.ENGLISH).endsWith(".shp")) {
                                    fileList.add(new File(new File(workFolder, name), zeName));
                                }
                            }
                            zipEntry = zis.getNextEntry();
                        }
                    }
                }
            }
            else {
                fileList.add(new File(workFolder, inputFileName));
            }
        }
    }

    private static File getAttributesOutputFile(String jobId) {
        return new File(TEMP_OUTPUT_PATH, jobId + ".out");
    }
}
