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

import static hoot.services.HootProperties.HOME_FOLDER;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.UUID;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import javax.ws.rs.Consumes;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

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
import hoot.services.command.ExternalCommandManager;
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
                                      @QueryParam("NONE_TRANSLATION") String noneTranslation,
                                      @QueryParam("FGDB_FC") String fgdbFeatureClasses,
                                      FormDataMultiPart multiPart) {
        JSONArray response = new JSONArray();

        try {
            // Save multipart data into file
            logger.debug("Starting ETL Process for:{}", inputName);

            Map<String, String> uploadedFiles = new HashMap<>();
            Map<String, String> uploadedFilesPaths = new HashMap<>();

            String jobId = UUID.randomUUID().toString();

            MultipartSerializer.serializeUpload(jobId, inputType, uploadedFiles, uploadedFilesPaths, multiPart);

            int shpCnt = 0;
            int osmCnt = 0;
            int fgdbCnt = 0;
            int geonamesCnt = 0;
            int zipCnt = 0;
            int shpZipCnt = 0;
            int osmZipCnt = 0;
            int geonamesZipCnt = 0;
            int fgdbZipCnt = 0;

            List<String> zipList = new ArrayList<>();

            JSONArray reqList = new JSONArray();
            List<String> inputsList = new ArrayList<>();

            String etlName = inputName;
            for (Map.Entry<String, String> pairs : uploadedFiles.entrySet()) {
                String fName = pairs.getKey();
                String ext = pairs.getValue();

                if ((etlName == null) || (etlName.isEmpty())) {
                    etlName = fName;
                }

                String inputFileName = uploadedFilesPaths.get(fName);
                inputsList.add(inputFileName);

                // for osm
                // for shp
                // for zip - can not mix different types in zip
                // -- for fgdb zip
                // for fgdb

                JSONObject zipStat = new JSONObject();
                buildNativeRequest(jobId, fName, ext, inputFileName, reqList, zipStat);

                if (ext.equalsIgnoreCase("zip")) {
                    shpZipCnt += (Integer) zipStat.get("shpzipcnt");
                    fgdbZipCnt += (Integer) zipStat.get("fgdbzipcnt");
                    osmZipCnt += (Integer) zipStat.get("osmzipcnt");
                    geonamesZipCnt += (Integer) zipStat.get("geonameszipcnt");

                    zipList.add(fName);
                    zipCnt++;
                }
                else {
                    shpCnt += (Integer) zipStat.get("shpcnt");
                    fgdbCnt += (Integer) zipStat.get("fgdbcnt");
                    osmCnt += (Integer) zipStat.get("osmcnt");
                    geonamesCnt += (Integer) zipStat.get("geonamescnt");
                }

                if (inputType.equalsIgnoreCase("geonames") && ext.equalsIgnoreCase("txt") && (geonamesCnt == 1)) {
                    inputFileName = fName + ".geonames";
                    String directory = HOME_FOLDER + "/upload/" + jobId;
                    // we need to rename the file for hoot to ingest
                    new File(directory + File.separator + inputsList.get(0)).renameTo(new File(directory + File.separator + inputFileName));
                    inputsList.set(0, inputFileName);
                    reqList = new JSONArray();

                    buildNativeRequest(jobId, fName, "geonames", inputFileName, reqList, zipStat);
                }
            }

            if (((shpZipCnt + fgdbZipCnt + shpCnt + fgdbCnt) > 0) && ((osmZipCnt + osmCnt) > 0)) {
                throw new IllegalStateException("Can not mix osm and ogr type.");
            }

            if (osmZipCnt > 1) {
                // #6027
                throw new IllegalArgumentException("Hootennany does not support zip files containing multiple .osm data files.");
            }

            if ((osmZipCnt == 1) && ((shpZipCnt + fgdbZipCnt + shpCnt + fgdbCnt + osmCnt) == 0)) {
                // we want to unzip the file and modify any necessary parameters for the ensuing makefile
                String zipFilePath = HOME_FOLDER + "/upload/" + jobId + File.separator + inputsList.get(0);

                try (ZipInputStream zis = new ZipInputStream(new FileInputStream(zipFilePath))) {
                    ZipEntry ze = zis.getNextEntry();

                    byte[] buffer = new byte[2048];
                    while (ze != null) {
                        String entryName = ze.getName();
                        File file = new File(HOME_FOLDER + "/upload/" + jobId + File.separator + entryName);
                        // for now assuming no subdirectories
                        try (FileOutputStream fOutput = new FileOutputStream(file)) {
                            int count = 0;
                            while ((count = zis.read(buffer)) > 0) {
                                // write 'count' bytes to the file output stream
                                fOutput.write(buffer, 0, count);
                            }
                        }

                        zipList = new ArrayList<>();
                        reqList = new JSONArray();
                        inputsList = new ArrayList<>();
                        JSONObject zipStat = new JSONObject();
                        buildNativeRequest(jobId, inputName, "OSM", entryName, reqList, zipStat);
                        ze = zis.getNextEntry();
                    }
                }
                // massage some variables to make it look like an osm file was uploaded
                zipCnt = 0;
                osmZipCnt = 0;
                osmCnt = 1;
            }

            // TODO: move creation of FileETLCommand instance inside of the lambda function below.
            // Presently, it's not easy to do since lots of fileETLCommandFactory.build() takes lots of
            // variables that are not final or effectively final.
            FileETLCommand etlCommand = fileETLCommandFactory.build(reqList, zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt, geonamesZipCnt,
                    shpCnt, fgdbCnt, osmCnt, geonamesCnt, zipList, translation, jobId, etlName, inputsList, userEmail,
                    noneTranslation, fgdbFeatureClasses, this.getClass());

            String mapDisplayName = etlName;

            Command[] commands = {
                    // Clip to a bounding box
                    () -> {
                        return externalCommandManager.exec(jobId, etlCommand);
                    },

                    () -> {
                        ExternalCommand exportRenderDBCommand = exportRenderDBCommandFactory.build(mapDisplayName, this.getClass());
                        return externalCommandManager.exec(jobId, exportRenderDBCommand);
                    }
            };

            jobProcessor.process(new Job(jobId, commands));

            String mergedInputList = StringUtils.join(inputsList.toArray(), ';');
            JSONObject res = new JSONObject();
            res.put("jobid", jobId);
            res.put("input", mergedInputList);
            res.put("output", etlName);

            String batchJobReqStatus = "success";
            res.put("status", batchJobReqStatus);

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

    private static void buildNativeRequest(String jobId, String fName, String ext, String inputFileName,
            JSONArray reqList, JSONObject zipStat) throws IOException {
        int shpZipCnt = 0;
        Object oShpStat = zipStat.get("shpzipcnt");
        if (oShpStat == null) {
            zipStat.put("shpzipcnt", 0);
        }
        else {
            shpZipCnt = (Integer) oShpStat;
        }

        int fgdbZipCnt = 0;
        Object oFgdbStat = zipStat.get("fgdbzipcnt");
        if (oFgdbStat == null) {
            zipStat.put("fgdbzipcnt", 0);
        }
        else {
            fgdbZipCnt = (Integer) oFgdbStat;
        }

        int osmZipCnt = 0;
        Object oOsmStat = zipStat.get("osmzipcnt");
        if (oOsmStat == null) {
            zipStat.put("osmzipcnt", 0);
        }
        else {
            osmZipCnt = (Integer) oOsmStat;
        }

        int geonamesZipCnt = 0;
        Object oGeonamesStat = zipStat.get("geonameszipcnt");
        if (oGeonamesStat == null) {
            zipStat.put("geonameszipcnt", 0);
        }
        else {
            geonamesZipCnt = (Integer) oGeonamesStat;
        }

        int osmCnt = 0;
        int shpCnt = 0;
        int fgdbCnt = 0;
        int geonamesCnt = 0;

        if (ext.equalsIgnoreCase("osm") || ext.equalsIgnoreCase("pbf")) {
            JSONObject reqType = new JSONObject();
            reqType.put("type", "OSM");
            reqType.put("name", inputFileName);
            reqList.add(reqType);
            osmCnt++;
        }
        else if (ext.equalsIgnoreCase("geonames") || ext.equalsIgnoreCase("txt")) {
            JSONObject reqType = new JSONObject();
            reqType.put("type", "GEONAMES");
            reqType.put("name", inputFileName);
            reqList.add(reqType);
            geonamesCnt++;
        }
        else if (ext.equalsIgnoreCase("shp")) {
            JSONObject reqType = new JSONObject();
            reqType.put("type", "OGR");
            reqType.put("name", inputFileName);
            reqList.add(reqType);
            shpCnt++;
        }
        else if (ext.equalsIgnoreCase("zip")) {
            // Check to see the type of zip (osm, ogr or fgdb)
            String zipFilePath = HOME_FOLDER + "/upload/" + jobId + "/" + inputFileName;

            JSONObject res = getZipContentType(zipFilePath, reqList, fName);

            shpZipCnt += (Integer) res.get("shpcnt");
            fgdbZipCnt += (Integer) res.get("fgdbcnt");
            osmZipCnt += (Integer) res.get("osmcnt");
            geonamesZipCnt += (Integer) res.get("geonamescnt");

            // We do not allow mix of ogr and osm in zip
            if (((shpZipCnt + fgdbZipCnt) > 0) && (osmZipCnt > 0)) {
                throw new IllegalStateException("Zip should not contain both osm and ogr types.");
            }

            zipStat.put("shpzipcnt", shpZipCnt);
            zipStat.put("fgdbzipcnt", fgdbZipCnt);
            zipStat.put("osmzipcnt", osmZipCnt);
            zipStat.put("geonameszipcnt", geonamesZipCnt);
        }
        else if (ext.equalsIgnoreCase("gdb")) {
            JSONObject reqType = new JSONObject();
            reqType.put("type", "FGDB");
            reqType.put("name", inputFileName);
            reqList.add(reqType);
            fgdbCnt++;
        }

        zipStat.put("shpcnt", shpCnt);
        zipStat.put("fgdbcnt", fgdbCnt);
        zipStat.put("osmcnt", osmCnt);
        zipStat.put("geonamescnt", geonamesCnt);
    }

    // returns the type of file in zip
    // throws error if there are mix of osm and ogr
    // zip does not allow fgdb so it needs to be expanded out
    private static JSONObject getZipContentType(String zipFilePath, JSONArray contentTypes, String fName)
            throws IOException {
        JSONObject resultStat = new JSONObject();
        String[] extList = { "gdb", "osm", "shp", "geonames", "pbf" };

        try (ZipInputStream zis = new ZipInputStream(new FileInputStream(zipFilePath))) {
            ZipEntry ze = zis.getNextEntry();

            int shpCnt = 0;
            int osmCnt = 0;
            int geonamesCnt = 0;
            int fgdbCnt = 0;
            while (ze != null) {
                String zipName = ze.getName();
                // check to see if zipName ends with slash and remove
                if (zipName.endsWith("/")) {
                    zipName = zipName.substring(0, zipName.length() - 1);
                }

                String[] fileNameParts = zipName.split("\\.");
                String ext = null;

                int partsLen = fileNameParts.length;
                if (partsLen > 1) {
                    ext = fileNameParts[partsLen - 1];
                }

                // See if there is extension and if none then throw error
                if (ext == null) {
                    throw new IOException("Unknown file type.");
                }

                // for each type of extensions
                for (String anExtList : extList) {
                    if (ext.equalsIgnoreCase(anExtList)) {
                        if (ze.isDirectory()) {
                            if (ext.equals("gdb")) {
                                JSONObject contentType = new JSONObject();
                                contentType.put("type", "FGDB_ZIP");
                                contentType.put("name", fName + "/" + zipName);
                                contentTypes.add(contentType);
                                fgdbCnt++;
                            }
                            else {
                                throw new IOException("Unknown folder type. Only gdb folder type is supported.");
                            }
                        }
                        else // file
                        {
                            switch (ext) {
                                case "shp": {
                                    JSONObject contentType = new JSONObject();
                                    contentType.put("type", "OGR_ZIP");
                                    contentType.put("name", fName + "/" + zipName);
                                    contentTypes.add(contentType);
                                    shpCnt++;
                                    break;
                                }
                                case "osm": {
                                    JSONObject contentType = new JSONObject();
                                    contentType.put("type", "OSM_ZIP");
                                    contentType.put("name", fName + "/" + zipName);
                                    contentTypes.add(contentType);
                                    osmCnt++;
                                    break;
                                }
                                case "geonames": {
                                    JSONObject contentType = new JSONObject();
                                    contentType.put("type", "GEONAMES_ZIP");
                                    contentType.put("name", fName + "/" + zipName);
                                    contentTypes.add(contentType);
                                    geonamesCnt++;
                                    break;
                                }
                                default:
                                    // We will not throw error here since shape file
                                    // can contain mutiple types of support files.
                                    // We will let hoot-core decide if it can handle
                                    // the zip.
                                    break;
                            }
                        }
                    }

                    // We do not allow mix of ogr and osm in zip
                    if (((shpCnt + fgdbCnt) > 0) && (osmCnt > 0)) {
                        throw new IOException("Zip should not contain both osm and ogr types.");
                    }
                }

                ze = zis.getNextEntry();
            }

            resultStat.put("shpcnt", shpCnt);
            resultStat.put("fgdbcnt", fgdbCnt);
            resultStat.put("osmcnt", osmCnt);
            resultStat.put("geonamescnt", geonamesCnt);

            return resultStat;
        }
    }
}
