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
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.UUID;

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
import org.apache.commons.io.filefilter.FileFilterUtils;
import org.apache.commons.io.filefilter.IOFileFilter;
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
import hoot.services.command.ExternalCommand;
import hoot.services.command.common.UnZIPFileCommand;
import hoot.services.controllers.common.ExportRenderDBCommandFactory;
import hoot.services.job.Job;
import hoot.services.job.JobProcessor;
import hoot.services.utils.MultipartSerializer;


@Controller
@Path("/ingest")
@Transactional
public class ImportResource {
    private static final Logger logger = LoggerFactory.getLogger(ImportResource.class);

    @Autowired
    private JobProcessor jobProcessor;

    @Autowired
    private ImportCommandFactory fileETLCommandFactory;

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
                                      @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel,
                                      FormDataMultiPart multiPart) {
        JSONArray response = new JSONArray();

        try {
            String jobId = UUID.randomUUID().toString();
            File workDir = new File(UPLOAD_FOLDER, jobId);

            List<File> uploadedFiles = MultipartSerializer.serializeUpload(multiPart, workDir);

            int shpCnt = 0;
            int osmCnt = 0;
            int fgdbCnt = 0;
            int geonamesCnt = 0;
            int zipCnt = 0;
            int shpZipCnt = 0;
            int osmZipCnt = 0;
            int geonamesZipCnt = 0;
            int fgdbZipCnt = 0;

            List<Map<String, String>> etlRequests = new LinkedList<>();
            List<String> inputsList = new ArrayList<>();
            List<File> zipList = new ArrayList<>();
            String etlName = inputName;

            for (File uploadedFile : uploadedFiles) {
                String uploadedFileName = uploadedFile.getName();
                String uploadedFileBasename = FilenameUtils.getBaseName(uploadedFileName);
                String uploadedFileExtension = FilenameUtils.getExtension(uploadedFileName);

                if (StringUtils.isBlank(etlName)) {
                    etlName = uploadedFileBasename;
                }

                inputsList.add(uploadedFileName);

                Map<String, Integer> zipStat = new HashMap<>();

                etlRequests.addAll(handleUploadedFile(uploadedFileExtension, uploadedFile, zipStat, workDir, inputType));

                if (uploadedFileExtension.equalsIgnoreCase("ZIP")) {
                    zipList.add(uploadedFile);
                    zipCnt++;
                }

                // Update the counters
                shpCnt += zipStat.get("shpcnt");
                fgdbCnt += zipStat.get("fgdbcnt");
                osmCnt += zipStat.get("osmcnt");
                geonamesCnt += zipStat.get("geonamescnt");
                shpZipCnt += zipStat.get("shpzipcnt");
                fgdbZipCnt += zipStat.get("fgdbzipcnt");
                osmZipCnt += zipStat.get("osmzipcnt");
                geonamesZipCnt += zipStat.get("geonameszipcnt");

                if (inputType.equalsIgnoreCase("GEONAMES") &&
                        uploadedFileExtension.equalsIgnoreCase("TXT") && (geonamesCnt == 1)) {
                    uploadedFileName = uploadedFileBasename + "." + "geonames";
                    File srcFile = new File(workDir, inputsList.get(0));
                    File destFile = new File(workDir, uploadedFileName);

                    // we need to rename the file for hoot to ingest
                    FileUtils.moveFile(srcFile, destFile);

                    inputsList.set(0, uploadedFileName);

                    etlRequests.clear();

                    etlRequests.addAll(handleUploadedFile("GEONAMES", destFile, zipStat, workDir, inputType));
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
                File zipFolder = new File(workDir, FilenameUtils.getBaseName(zipFile.getName()));

                zipList.clear();
                etlRequests.clear();
                inputsList.clear();

                IOFileFilter fileFilter = FileFilterUtils.suffixFileFilter("osm");
                Collection<File> osmFiles = FileUtils.listFiles(zipFolder, fileFilter, null);

                Map<String, Integer> zipStat = new HashMap<>();
                for (File osmFile : osmFiles) {
                    etlRequests.addAll(handleUploadedFile("OSM", osmFile, zipStat, workDir, inputType));
                }

                // massage some variables to make it look like an osm file was uploaded
                shpCnt = 0;
                fgdbCnt = 0;
                geonamesCnt = 0;
                shpZipCnt = 0;
                geonamesZipCnt = 0;
                fgdbZipCnt = 0;
                zipCnt = 0;
                osmZipCnt = 0;
                osmCnt = 1;
            }

            String uploadedFileClassification = classifyUploadedFile(zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt,
                                                                     geonamesZipCnt, shpCnt, fgdbCnt, osmCnt,
                                                                     geonamesCnt);

            ExternalCommand etlCommand = fileETLCommandFactory.build(jobId, workDir, etlRequests, zipList, translation, etlName,
                    noneTranslation, debugLevel, uploadedFileClassification, this.getClass());

            ExternalCommand exportRenderDBCommand = exportRenderDBCommandFactory.build(jobId, etlName, this.getClass());

            Command[] workflow = { etlCommand, exportRenderDBCommand };

            jobProcessor.submitAsync(new Job(jobId, workflow));

            JSONObject res = new JSONObject();
            res.put("jobid", jobId);
            res.put("input", StringUtils.join(inputsList.toArray(), ';'));
            res.put("output", etlName);
            res.put("status", "success");

            response.add(res);
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (IllegalArgumentException iae) {
            throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
        }
        catch (Exception ex) {
            String msg = "Failed upload: " + ex.getMessage();
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        return Response.ok(response.toJSONString()).build();
    }

    private static String classifyUploadedFile(int zipCnt, int shpZipCnt, int fgdbZipCnt, int osmZipCnt,
                                               int geonamesZipCnt, int shpCnt, int fgdbCnt, int osmCnt,
                                               int geonamesCnt) {
        // if fgdb zip > 0 then all becomes fgdb so it can be uzipped first
        // if fgdb zip == 0 and shp zip > then it is standard zip.
        // if fgdb zip == 0 and shp zip == 0 and osm zip > 0 then it is osm zip
        String classification = null;

        if (zipCnt > 0) {
            if (fgdbZipCnt > 0) {
                classification = "OGR";
            }
            else {
                // Mix of shape and zip then we will unzip and treat it like OGR
                if (shpCnt > 0) { // One or more all ogr zip + shape
                    classification = "OGR";
                }
                else if (osmCnt > 0) { // Mix of One or more all osm zip + osm
                    classification = "OSM";
                }
                else if (geonamesCnt > 0) { // Mix of One or more all osm zip + osm
                    classification = "GEONAMES";
                }
                else {
                    // One or more zip (all ogr) || One or more zip (all osm)
                    // If contains zip of just shape or osm then we will etl zip directly
                    classification = "ZIP";
                }
            }
        }
        else if (shpCnt > 0) {
            classification = "OGR";
        }
        else if (osmCnt > 0) {
            classification = "OSM";
        }
        else if (fgdbCnt > 0) {
            classification = "FGDB";
        }
        else if (geonamesCnt > 0) {
            classification = "GEONAMES";
        }
        else {
            throw new RuntimeException("Error during classification: unable to classify uploaded file!");
        }

        return classification;
    }

    private List<Map<String, String>> handleUploadedFile(String ext, File inputFile, Map<String, Integer> stats, File workDir, String inputType) {
        List<Map<String, String>> etlRequests = new LinkedList<>();

        int osmZipCnt = stats.getOrDefault("osmzipcnt", 0);
        int shpZipCnt = stats.getOrDefault("shpzipcnt", 0);
        int fgdbZipCnt = stats.getOrDefault("fgdbzipcnt", 0);
        int geonamesZipCnt = stats.getOrDefault("geonameszipcnt", 0);
        int osmCnt = stats.getOrDefault("osmcnt", 0);
        int shpCnt = stats.getOrDefault("shpcnt", 0);
        int fgdbCnt = stats.getOrDefault("fgdbcnt", 0);
        int geonamesCnt = stats.getOrDefault("geonamescnt", 0);

        if (ext.equalsIgnoreCase("OSM") || ext.equalsIgnoreCase("PBF")) {
            Map<String, String> etlRequest = new HashMap<>();
            etlRequest.put("type", "OSM");
            etlRequest.put("name", inputFile.getAbsolutePath());
            etlRequests.add(etlRequest);
            osmCnt++;
        }
        else if (ext.equalsIgnoreCase("GEONAMES") || ext.equalsIgnoreCase("TXT")) {
            Map<String, String> etlRequest = new HashMap<>();
            etlRequest.put("type", "GEONAMES");
            etlRequest.put("name", inputFile.getAbsolutePath());
            etlRequests.add(etlRequest);
            geonamesCnt++;
        }
        else if (ext.equalsIgnoreCase("SHP")) {
            Map<String, String> etlRequest = new HashMap<>();
            etlRequest.put("type", "OGR");
            etlRequest.put("name", inputFile.getAbsolutePath());
            etlRequests.add(etlRequest);
            shpCnt++;
        }
        else if (ext.equalsIgnoreCase("ZIP")) {
            // Check to see the type of zip (osm, ogr or fgdb)
            Map<String, Integer> results = specialHandleWhenZIP(inputFile, etlRequests, workDir);

            shpZipCnt += results.get("shpzipcnt");
            fgdbZipCnt += results.get("fgdbzipcnt");
            osmZipCnt += results.get("osmzipcnt");
            geonamesZipCnt += results.get("geonameszipcnt");
        }
        else if (inputFile.getName().equalsIgnoreCase("GDB") && inputType.equalsIgnoreCase("DIR")) {
            Map<String, String> etlRequest = new HashMap<>();
            etlRequest.put("type", "FGDB");
            etlRequest.put("name", inputFile.getParentFile().getAbsolutePath());
            etlRequests.add(etlRequest);
            fgdbCnt++;
        }

        stats.put("shpzipcnt", shpZipCnt);
        stats.put("fgdbzipcnt", fgdbZipCnt);
        stats.put("osmzipcnt", osmZipCnt);
        stats.put("geonameszipcnt", geonamesZipCnt);
        stats.put("shpcnt", shpCnt);
        stats.put("fgdbcnt", fgdbCnt);
        stats.put("osmcnt", osmCnt);
        stats.put("geonamescnt", geonamesCnt);

        return etlRequests;
    }

    /**
     * Look inside of the zip and decide how to classify what's inside.
     *
     * @param zip archive to analyse
     * @param etlRequests
     * @return Map of counters after looking inside of the ZIP
     */
    private Map<String, Integer> specialHandleWhenZIP(File zip, List<Map<String, String>> etlRequests, File workDir) {
        String basename = FilenameUtils.getBaseName(zip.getName());

        File targetFolder = new File(workDir, FilenameUtils.getBaseName(basename));

        ExternalCommand unZIPFileCommand = new UnZIPFileCommand(zip, targetFolder, this.getClass());
        unZIPFileCommand.execute();

        IOFileFilter fileFilter = FileFilterUtils.or(
                       FileFilterUtils.suffixFileFilter("shp"),
                       FileFilterUtils.suffixFileFilter("osm"),
                       FileFilterUtils.suffixFileFilter("geonames"),
                       FileFilterUtils.suffixFileFilter("pbf"),
                       FileFilterUtils.nameFileFilter("gdb"));

        Collection<File> files = FileUtils.listFiles(targetFolder, fileFilter, null);

        int shpCnt = 0;
        int osmCnt = 0;
        int geonamesCnt = 0;
        int fgdbCnt = 0;

        for (File file : files) {
            String ext = FilenameUtils.getExtension(file.getName());

            if (file.getName().equalsIgnoreCase("GDB") && StringUtils.isBlank(ext) &&
                    FilenameUtils.getExtension(targetFolder.getName()).equalsIgnoreCase("GDB")) {
                Map<String, String> contentType = new HashMap<>();
                contentType.put("type", "FGDB_ZIP");
                contentType.put("name", file.getParentFile().getAbsolutePath());
                etlRequests.add(contentType);
                fgdbCnt++;
            }
            else if (ext.equalsIgnoreCase("SHP")) {
                Map<String, String> contentType = new HashMap<>();
                contentType.put("type", "OGR_ZIP");
                contentType.put("name", file.getAbsolutePath());
                etlRequests.add(contentType);
                shpCnt++;
            }
            else if (ext.equalsIgnoreCase("OSM")) {
                Map<String, String> contentType = new HashMap<>();
                contentType.put("type", "OSM_ZIP");
                contentType.put("name", file.getAbsolutePath());
                etlRequests.add(contentType);
                osmCnt++;
            }
            else if (ext.equalsIgnoreCase("GEONAMES")) {
                Map<String, String> contentType = new HashMap<>();
                contentType.put("type", "GEONAMES_ZIP");
                contentType.put("name", file.getAbsolutePath());
                etlRequests.add(contentType);
                geonamesCnt++;
            }
        }

        // We do not allow mix of ogr and osm in zip
        if (((shpCnt + fgdbCnt) > 0) && (osmCnt > 0)) {
            throw new IllegalArgumentException(zip.getAbsolutePath() + " cannot contain both OSM and OGR types.");
        }

        Map<String, Integer> stats = new HashMap<>();

        stats.put("shpzipcnt", shpCnt);
        stats.put("fgdbzipcnt", fgdbCnt);
        stats.put("osmzipcnt", osmCnt);
        stats.put("geonameszipcnt", geonamesCnt);

        return stats;
    }
}
