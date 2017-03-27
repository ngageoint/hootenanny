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

import static hoot.services.HootProperties.UPLOAD_FOLDER;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.UUID;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import javax.ws.rs.Consumes;
import javax.ws.rs.DefaultValue;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.apache.commons.io.FileUtils;
import org.apache.commons.io.FilenameUtils;
import org.apache.commons.lang3.StringUtils;
import org.glassfish.jersey.media.multipart.FormDataMultiPart;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.command.Command;
import hoot.services.command.CommandResult;
import hoot.services.command.ExternalCommand;
import hoot.services.command.ExternalCommandManager;
import hoot.services.command.InternalCommand;
import hoot.services.command.InternalCommandManager;
import hoot.services.command.common.UnZIPFileCommand;
import hoot.services.controllers.ExportRenderDBCommandFactory;
import hoot.services.job.Job;
import hoot.services.job.JobProcessor;
import hoot.services.utils.MultipartSerializer;


@Controller
@Path("/ingest")
@Transactional
public class FileUploadResource {
    private static final Logger logger = LoggerFactory.getLogger(FileUploadResource.class);

    @Autowired
    private JobProcessor jobProcessor;

    @Autowired
    private ExternalCommandManager externalCommandManager;

    @Autowired
    private InternalCommandManager internalCommandManager;

    @Autowired
    private FileETLCommandFactory fileETLCommandFactory;

    @Autowired
    private ExportRenderDBCommandFactory exportRenderDBCommandFactory;

    /**
     * Purpose of this service is to provide ingest service for uploading shape
     * and osm file and performing ETL operation on the uploaded file(s). This
     * service is multipart post service which accepts single or multiple files
     * sent by multipart client.
     *
     * POST hoot-services/ingest/upload?TRANSLATION=NFDD.js&INPUT_TYPE=OSM&INPUT_NAME=ToyTest
     *
     * @param translation
     *            Translation script used during OGR ETL process.
     * @param inputType
     *            [OSM | OGR ] OSM for osm file and OGR for shapefile.
     * @param inputName
     *            optional input name which is used in hoot db. Defaults to the file name.
     * @param userEmail
     *            mail address of the user requesting job
     * @param noneTranslation
     *            ?
     * @param multiPart
     *            uploaded files
     * @return Array of job status
     */
    @POST
    @Path("/upload")
    @Consumes(MediaType.MULTIPART_FORM_DATA)
    @Produces(MediaType.APPLICATION_JSON)
    public Response processFileUpload(@QueryParam("TRANSLATION") String translation,
                                      @QueryParam("INPUT_TYPE") String inputType,
                                      @QueryParam("INPUT_NAME") String inputName,
                                      @QueryParam("USER_EMAIL") String userEmail,
                                      @QueryParam("NONE_TRANSLATION") Boolean noneTranslation,
                                      @QueryParam("FGDB_FC") String fgdbFeatureClasses,
                                      @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel,
                                      FormDataMultiPart multiPart) {
        JSONArray response = new JSONArray();

        try {
            // Save multipart data into file
            logger.debug("Starting ETL Process for : {}", inputName);

            String jobId = UUID.randomUUID().toString();
            File workDir = new File(UPLOAD_FOLDER, jobId);

            Map<File, String> uploadedFiles = MultipartSerializer.serializeUpload(inputType, multiPart, workDir);

            int shpCnt = 0;
            int osmCnt = 0;
            int fgdbCnt = 0;
            int geonamesCnt = 0;
            int zipCnt = 0;
            int shpZipCnt = 0;
            int osmZipCnt = 0;
            int geonamesZipCnt = 0;
            int fgdbZipCnt = 0;

            List<Map<String, String>> reqList = new LinkedList<>();
            List<String> inputsList = new ArrayList<>();
            List<String> zipList = new ArrayList<>();

            String etlName = inputName;
            for (Map.Entry<File, String> pairs : uploadedFiles.entrySet()) {
                String inputFileName = pairs.getKey().getName();
                String basename = FilenameUtils.getBaseName(inputFileName);
                String ext = pairs.getValue();

                if (StringUtils.isBlank(etlName)) {
                    etlName = basename;
                }

                inputsList.add(inputFileName);

                // for osm
                // for shp
                // for zip - can not mix different types in zip
                // -- for fgdb zip
                // for fgdb

                Map<String, Integer> zipStat = new HashMap<>();

                analyzeUploadedFile(basename, ext, inputFileName, reqList, zipStat, workDir);

                if (ext.equalsIgnoreCase("ZIP")) {
                    shpZipCnt += zipStat.get("shpzipcnt");
                    fgdbZipCnt += zipStat.get("fgdbzipcnt");
                    osmZipCnt += zipStat.get("osmzipcnt");
                    geonamesZipCnt += zipStat.get("geonameszipcnt");

                    zipList.add(inputFileName);
                    zipCnt++;
                }
                else {
                    shpCnt += zipStat.get("shpcnt");
                    fgdbCnt += zipStat.get("fgdbcnt");
                    osmCnt += zipStat.get("osmcnt");
                    geonamesCnt += zipStat.get("geonamescnt");
                }

                if (inputType.equalsIgnoreCase("GEONAMES") &&
                        ext.equalsIgnoreCase("TXT") && (geonamesCnt == 1)) {
                    inputFileName = basename + "." + "geonames";
                    File srcFile = new File(workDir, inputsList.get(0));
                    File destFile = new File(workDir, inputFileName);

                    // we need to rename the file for hoot to ingest
                    FileUtils.moveFile(srcFile, destFile);

                    inputsList.set(0, inputFileName);
                    reqList.clear();

                    analyzeUploadedFile(basename, "GEONAMES", inputFileName, reqList, zipStat, workDir);
                }
            }

            if (((shpZipCnt + fgdbZipCnt + shpCnt + fgdbCnt) > 0) && ((osmZipCnt + osmCnt) > 0)) {
                throw new IllegalArgumentException("Can not mix osm and ogr type.");
            }

            if (osmZipCnt > 1) {
                throw new IllegalArgumentException("Hootennany does not support zip files containing multiple .osm data files.");
            }

            if ((osmZipCnt == 1) && ((shpZipCnt + fgdbZipCnt + shpCnt + fgdbCnt + osmCnt) == 0)) {
                // we want to unzip the file and modify any necessary parameters
                File zipFile = new File(workDir, inputsList.get(0));

                try (ZipInputStream zis = new ZipInputStream(new FileInputStream(zipFile))) {
                    ZipEntry zipEntry = zis.getNextEntry();

                    byte[] buffer = new byte[2048];
                    while (zipEntry != null) {
                        String entryName = zipEntry.getName();
                        File file = new File(workDir, entryName);

                        // for now assuming no subdirectories
                        try (FileOutputStream fos = new FileOutputStream(file)) {
                            int count;
                            while ((count = zis.read(buffer)) > 0) {
                                // write 'count' bytes to the file output stream
                                fos.write(buffer, 0, count);
                            }
                        }

                        zipList.clear();
                        reqList.clear();
                        inputsList.clear();

                        Map<String, Integer> zipStat = new HashMap<>();

                        analyzeUploadedFile(inputName, "OSM", entryName, reqList, zipStat, workDir);

                        zipEntry = zis.getNextEntry();
                    }
                }

                // massage some variables to make it look like an osm file was uploaded
                zipCnt = 0;
                osmZipCnt = 0;
                osmCnt = 1;
            }

            String finalInputType = determineInputType(zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt, geonamesZipCnt,
                                                       shpCnt, fgdbCnt, osmCnt, geonamesCnt);

            String finalETLName = etlName;

            List<Command> commands = new LinkedList<>();

            //# Unzip when semicolon separated lists are provided
            //ifneq ($(strip $(UNZIP_LIST)), )
            //    bash $(HOOT_HOME)/scripts/util/unzipfiles.sh "$(UNZIP_LIST)" "$(OP_INPUT_PATH)"
            //endif
            if (!zipList.isEmpty()) {
                for (String zip : zipList) {
                    File file = new File(workDir, zip);
                    commands.add(() -> {
                        ExternalCommand unzipCommand = new UnZIPFileCommand(file, workDir, this.getClass());
                        return externalCommandManager.exec(jobId, unzipCommand);
                    });
                }
            }

            commands.add(() -> {
                ExternalCommand etlCommand = fileETLCommandFactory.build(reqList, zipList, translation, jobId, finalETLName,
                        noneTranslation, fgdbFeatureClasses, debugLevel, finalInputType, this.getClass());

                return externalCommandManager.exec(jobId, etlCommand);
            });

            commands.add(() -> {
                InternalCommand command = () -> {
                    CommandResult commandResult = new CommandResult();
                    commandResult.setJobId(jobId);

                    // cleanup
                    try {
                        //rm -rf "$(OP_INPUT_PATH)"
                        FileUtils.forceDelete(workDir);
                    }
                    catch (IOException ioe) {
                        logger.error("Error deleting {} ", workDir.getAbsolutePath(), ioe);
                    }

                    return commandResult;
                };

                return internalCommandManager.exec(jobId, command);
            });

            String mapDisplayName = etlName;

            commands.add(
                () -> {
                    ExternalCommand exportRenderDBCommand = exportRenderDBCommandFactory.build(mapDisplayName, this.getClass());
                    return externalCommandManager.exec(jobId, exportRenderDBCommand);
            });

            jobProcessor.process(new Job(jobId, commands.toArray(new Command[commands.size()])));

            String mergedInputList = StringUtils.join(inputsList.toArray(), ';');

            JSONObject res = new JSONObject();
            res.put("jobid", jobId);
            res.put("input", mergedInputList);
            res.put("output", etlName);
            res.put("status", "success");

            response.add(res);
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception ex) {
            String msg = "Failed upload: " + ex.getMessage();
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        return Response.ok(response.toJSONString()).build();
    }

    private static String determineInputType(int zipCnt, int shpZipCnt, int fgdbZipCnt, int osmZipCnt,
                                             int geonamesZipCnt, int shpCnt, int fgdbCnt, int osmCnt,
                                             int geonamesCnt) {
        // if fgdb zip > 0 then all becomes fgdb so it can be uzipped first
        // if fgdb zip == 0 and shp zip > then it is standard zip.
        // if fgdb zip == 0 and shp zip == 0 and osm zip > 0 then it is osm zip
        String inputType = "";

        if (zipCnt > 0) {
            if (fgdbZipCnt > 0) {
                inputType = "OGR";
            }
            else {
                // Mix of shape and zip then we will unzip and treat it like OGR
                if (shpCnt > 0) { // One or more all ogr zip + shape
                    inputType = "OGR";
                }
                else if (osmCnt > 0) { // Mix of One or more all osm zip + osm
                    inputType = "OSM";
                }
                else if (geonamesCnt > 0) { // Mix of One or more all osm zip + osm
                    inputType = "GEONAMES";
                }
                else { // One or more zip (all ogr) || One or more zip (all osm)
                    // If contains zip of just shape or osm then we will etl zip directly
                    inputType = "ZIP";
                }
            }
        }
        else if (shpCnt > 0) {
            inputType = "OGR";
        }
        else if (osmCnt > 0) {
            inputType = "OSM";
        }
        else if (fgdbCnt > 0) {
            inputType = "FGDB";
        }
        else if (geonamesCnt > 0) {
            inputType = "GEONAMES";
        }

        return inputType;
    }

    private static void analyzeUploadedFile(String basename, String ext, String inputFileName,
            List<Map<String, String>> requests, Map<String, Integer> zipStat, File workDir) throws IOException {

        int osmZipCnt = zipStat.getOrDefault("osmzipcnt", 0);
        int shpZipCnt = zipStat.getOrDefault("shpzipcnt", 0);
        int fgdbZipCnt = zipStat.getOrDefault("fgdbzipcnt", 0);
        int geonamesZipCnt = zipStat.getOrDefault("geonameszipcnt", 0);
        int osmCnt = zipStat.getOrDefault("osmcnt", 0);
        int shpCnt = zipStat.getOrDefault("shpcnt", 0);
        int fgdbCnt = zipStat.getOrDefault("fgdbcnt", 0);
        int geonamesCnt = zipStat.getOrDefault("geonamescnt", 0);

        if (ext.equalsIgnoreCase("OSM") || ext.equalsIgnoreCase("PBF")) {
            Map<String, String> request = new HashMap<>();
            request.put("type", "OSM");
            request.put("name", inputFileName);
            requests.add(request);
            osmCnt++;
        }
        else if (ext.equalsIgnoreCase("GEONAMES") || ext.equalsIgnoreCase("TXT")) {
            Map<String, String> request = new HashMap<>();
            request.put("type", "GEONAMES");
            request.put("name", inputFileName);
            requests.add(request);
            geonamesCnt++;
        }
        else if (ext.equalsIgnoreCase("SHP")) {
            Map<String, String> request = new HashMap<>();
            request.put("type", "OGR");
            request.put("name", inputFileName);
            requests.add(request);
            shpCnt++;
        }
        else if (ext.equalsIgnoreCase("ZIP")) {
            // Check to see the type of zip (osm, ogr or fgdb)
            String zipFilePath = new File(workDir, inputFileName).getAbsolutePath();
                Map<String, Integer> results = getZipContentType(zipFilePath, requests, basename);

            shpZipCnt += results.get("shpcnt");
            fgdbZipCnt += results.get("fgdbcnt");
            osmZipCnt += results.get("osmcnt");
            geonamesZipCnt += results.get("geonamescnt");

            // We do not allow mix of ogr and osm in zip
            if (((shpZipCnt + fgdbZipCnt) > 0) && (osmZipCnt > 0)) {
                throw new IllegalArgumentException("Zip should not contain both osm and ogr types.");
            }
        }
        else if (ext.equalsIgnoreCase("GDB")) {
            Map<String, String> request = new HashMap<>();
            request.put("type", "FGDB");
            request.put("name", inputFileName);
            requests.add(request);
            fgdbCnt++;
        }

        zipStat.put("shpzipcnt", shpZipCnt);
        zipStat.put("fgdbzipcnt", fgdbZipCnt);
        zipStat.put("osmzipcnt", osmZipCnt);
        zipStat.put("geonameszipcnt", geonamesZipCnt);
        zipStat.put("shpcnt", shpCnt);
        zipStat.put("fgdbcnt", fgdbCnt);
        zipStat.put("osmcnt", osmCnt);
        zipStat.put("geonamescnt", geonamesCnt);
    }

    // returns the type of file in zip
    // throws error if there are mix of osm and ogr
    // zip does not allow fgdb so it needs to be expanded out
    private static Map<String, Integer> getZipContentType(String zipFilePath, List<Map<String, String>> contentTypes, String basename)
            throws IOException {

        try (ZipInputStream zis = new ZipInputStream(new FileInputStream(zipFilePath))) {
            ZipEntry zipEntry = zis.getNextEntry();

            int shpCnt = 0;
            int osmCnt = 0;
            int geonamesCnt = 0;
            int fgdbCnt = 0;

            while (zipEntry != null) {
                String zipEntryName = zipEntry.getName();

                // check to see if zipName ends with slash and remove
                if (zipEntryName.endsWith("/")) {
                    zipEntryName = zipEntryName.substring(0, zipEntryName.length() - 1);
                }

                String ext = null;
                String[] fileNameParts = zipEntryName.split("\\.");

                int partsLen = fileNameParts.length;
                if (partsLen > 1) {
                    ext = fileNameParts[partsLen - 1];
                }

                // See if there is extension and if none then throw error
                if (StringUtils.isBlank(ext)) {
                    throw new IllegalArgumentException("Unknown file type: " + zipEntryName);
                }

                String[] extList = { "GDB", "OSM", "SHP", "GEONAMES", "PBF" };

                // for each type of extensions
                for (String anExtList : extList) {
                    if (ext.equalsIgnoreCase(anExtList)) {
                        if (zipEntry.isDirectory()) {
                            if (ext.equalsIgnoreCase("GDB")) {
                                Map<String, String> contentType = new HashMap<>();
                                contentType.put("type", "FGDB_ZIP");
                                contentType.put("name", basename + File.separator + zipEntryName);
                                contentTypes.add(contentType);
                                fgdbCnt++;
                            }
                            else {
                                throw new IllegalArgumentException("Unknown folder type. Only GDB folder type is supported.");
                            }
                        }
                        else { // file
                            switch (ext.toUpperCase()) {
                                case "SHP": {
                                    Map<String, String> contentType = new HashMap<>();
                                    contentType.put("type", "OGR_ZIP");
                                    contentType.put("name", basename + File.separator + zipEntryName);
                                    contentTypes.add(contentType);
                                    shpCnt++;
                                    break;
                                }
                                case "OSM": {
                                    Map<String, String> contentType = new HashMap<>();
                                    contentType.put("type", "OSM_ZIP");
                                    contentType.put("name", basename + File.separator + zipEntryName);
                                    contentTypes.add(contentType);
                                    osmCnt++;
                                    break;
                                }
                                case "GEONAMES": {
                                    Map<String, String> contentType = new HashMap<>();
                                    contentType.put("type", "GEONAMES_ZIP");
                                    contentType.put("name", basename + File.separator + zipEntryName);
                                    contentTypes.add(contentType);
                                    geonamesCnt++;
                                    break;
                                }
                                default:
                                    // We will not throw error here since shape file
                                    // can contain mutiple types of support files.
                                    // We will let hoot-core decide if it can handle the zip.
                                    break;
                            }
                        }
                    }

                    // We do not allow mix of ogr and osm in zip
                    if (((shpCnt + fgdbCnt) > 0) && (osmCnt > 0)) {
                        throw new IllegalArgumentException("Zip should not contain both OSM and OGR types.");
                    }
                }

                zipEntry = zis.getNextEntry();
            }

            Map<String, Integer> resultStat = new HashMap<>();
            resultStat.put("shpcnt", shpCnt);
            resultStat.put("fgdbcnt", fgdbCnt);
            resultStat.put("osmcnt", osmCnt);
            resultStat.put("geonamescnt", geonamesCnt);

            return resultStat;
        }
    }
}
