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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.ingest;

import static hoot.services.HootProperties.HOME_FOLDER;
import static hoot.services.HootProperties.IMPORT_OPTIONS;
import static hoot.services.HootProperties.UPLOAD_FOLDER;
import static hoot.services.controllers.ingest.UploadClassification.FGDB;
import static hoot.services.controllers.ingest.UploadClassification.FGDB_ZIP;
import static hoot.services.controllers.ingest.UploadClassification.GEOJSON;
import static hoot.services.controllers.ingest.UploadClassification.GEOJSON_ZIP;
import static hoot.services.controllers.ingest.UploadClassification.GEONAMES;
import static hoot.services.controllers.ingest.UploadClassification.GEONAMES_ZIP;
import static hoot.services.controllers.ingest.UploadClassification.GPKG;
import static hoot.services.controllers.ingest.UploadClassification.GPKG_ZIP;
import static hoot.services.controllers.ingest.UploadClassification.OSM;
import static hoot.services.controllers.ingest.UploadClassification.OSM_ZIP;
import static hoot.services.controllers.ingest.UploadClassification.OTHER;
import static hoot.services.controllers.ingest.UploadClassification.SHAPE_ZIP;
import static hoot.services.controllers.ingest.UploadClassification.SHP;
import static hoot.services.controllers.ingest.UploadClassification.TXT;
import static hoot.services.controllers.ingest.UploadClassification.ZIP;

import java.io.File;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.EnumMap;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.UUID;

import javax.servlet.http.HttpServletRequest;
import javax.ws.rs.Consumes;
import javax.ws.rs.DefaultValue;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.apache.commons.io.FilenameUtils;
import org.apache.commons.lang3.StringUtils;
import org.glassfish.jersey.media.multipart.FormDataMultiPart;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.command.Command;
import hoot.services.command.ExternalCommand;
import hoot.services.command.InternalCommand;
import hoot.services.controllers.osm.map.UpdateParentCommandFactory;
import hoot.services.job.Job;
import hoot.services.job.JobProcessor;
import hoot.services.job.JobType;
import hoot.services.models.db.Users;
import hoot.services.utils.MultipartSerializer;


@Controller
@Path("/ingest")
@Transactional
public class ImportResource {
    @Autowired
    private JobProcessor jobProcessor;

    @Autowired
    private ImportCommandFactory fileETLCommandFactory;

    @Autowired
    private UpdateParentCommandFactory updateParentCommandFactory;

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
     * @param advUploadOpts
                  advanced options for importing shapefiles
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
    public Response processFileUpload(@Context HttpServletRequest request,
                                      @QueryParam("TRANSLATION") String translation,
                                      @QueryParam("INPUT_TYPE") String inputType,
                                      @QueryParam("INPUT_NAME") String inputName,
                                      @QueryParam("USER_EMAIL") String userEmail,
                                      @QueryParam("NONE_TRANSLATION") Boolean noneTranslation,
                                      @QueryParam("ADV_UPLOAD_OPTS") String advUploadOpts,
                                      @QueryParam("FOLDER_ID") String folderId,
                                      @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel,
                                      FormDataMultiPart multiPart) {
        Users user = Users.fromRequest(request);
        List<Map<String,Object>> results = new ArrayList<Map<String,Object>>();

        try {
            String jobId = UUID.randomUUID().toString();
            File workDir = new File(UPLOAD_FOLDER, jobId);

            List<File> uploadedFiles = MultipartSerializer.serializeUpload(multiPart, workDir);

            int shpCnt = 0, osmCnt = 0, fgdbCnt = 0, geojsonCnt = 0, geonamesCnt = 0, gpkgCnt = 0, zipCnt = 0;
            int shpZipCnt = 0, osmZipCnt = 0, geojsonZipCnt = 0, geonamesZipCnt = 0, gpkgZipCnt = 0, fgdbZipCnt = 0;

            List<File> filesToImport = new LinkedList<>();
            List<String> fileNames = new ArrayList<>();
            List<File> zipsToImport = new ArrayList<>();
            String etlName = inputName;

            for (File uploadedFile : uploadedFiles) {
                String uploadedFileName = uploadedFile.getName();
                String uploadedFileBasename = FilenameUtils.getBaseName(uploadedFileName);
                String uploadedFileExtension = FilenameUtils.getExtension(uploadedFileName);
                UploadClassification initialUploadClassification;

                try {
                    initialUploadClassification = UploadClassification.valueOf(uploadedFileExtension.toUpperCase());
                }
                catch (IllegalArgumentException iea) {
                    initialUploadClassification = OTHER;
                }

                if (StringUtils.isBlank(etlName)) {
                    etlName = uploadedFileBasename;
                }

                fileNames.add(uploadedFileName);

                Map<UploadClassification, Integer> counts = new EnumMap<>(UploadClassification.class);

                filesToImport.addAll(ImportResourceUtils.handleUploadedFile(initialUploadClassification, uploadedFile,
                        counts, workDir, inputType));

                if (initialUploadClassification == ZIP) {
                    zipsToImport.add(uploadedFile);
                    zipCnt++;
                }

                // Update the counts
                shpCnt += counts.get(SHP);
                fgdbCnt += counts.get(FGDB);
                osmCnt += counts.get(OSM);
                geojsonCnt +=  counts.get(GEOJSON);
                geonamesCnt += counts.get(GEONAMES);
                gpkgCnt += counts.get(GPKG);
                shpZipCnt += counts.get(SHAPE_ZIP);
                fgdbZipCnt += counts.get(FGDB_ZIP);
                osmZipCnt += counts.get(OSM_ZIP);
                geojsonZipCnt += counts.get(GEOJSON_ZIP);
                geonamesZipCnt += counts.get(GEONAMES_ZIP);
                gpkgZipCnt += counts.get(GPKG_ZIP);


                if ((geonamesCnt == 1) && (initialUploadClassification == TXT)) {
                    ImportResourceUtils.handleGEONAMESWithTxtExtension(workDir, uploadedFile, fileNames, filesToImport);
                }
            }

            if (((shpZipCnt + fgdbZipCnt + shpCnt + fgdbCnt) > 0) && ((osmZipCnt + osmCnt) > 0)) {
                throw new IllegalArgumentException("Can not mix osm and ogr type.");
            }

            if (osmZipCnt > 1) {
                throw new IllegalArgumentException("Hootennany does not support zip files containing multiple .osm data files.");
            }

            if ((osmZipCnt == 1) && ((shpZipCnt + fgdbZipCnt + shpCnt + fgdbCnt + osmCnt) == 0)) {
                ImportResourceUtils.handleOSMZip(workDir, zipsToImport, filesToImport, fileNames);

                // massage some variables to make it look like an osm file was uploaded
                osmCnt = 1;
                shpCnt = fgdbCnt = geonamesCnt = shpZipCnt = geonamesZipCnt = fgdbZipCnt = zipCnt = osmZipCnt = 0;
            }

            UploadClassification finalUploadClassification = ImportResourceUtils.finalizeUploadClassification(zipCnt,
                    shpZipCnt, fgdbZipCnt, osmZipCnt, geojsonZipCnt, geonamesZipCnt, gpkgZipCnt, shpCnt, fgdbCnt, osmCnt,
                    geojsonCnt, geonamesCnt, gpkgCnt);

            List<Command> workflow = new LinkedList<>();

            ExternalCommand importCommand = fileETLCommandFactory.build(jobId, workDir, filesToImport, zipsToImport, translation, advUploadOpts,
                    etlName, noneTranslation, debugLevel, finalUploadClassification, this.getClass(), user);
            workflow.add(importCommand);

            if(folderId != null) {
                InternalCommand setFolderCommand = updateParentCommandFactory.build(jobId, Long.parseLong(folderId), etlName, user, this.getClass());
                workflow.add(setFolderCommand);
            }

            jobProcessor.submitAsync(new Job(jobId, user.getId(), workflow.toArray(new Command[workflow.size()]), JobType.IMPORT));

            Map<String, Object> res = new HashMap<String, Object>();
            res.put("jobid", jobId);
            res.put("input", StringUtils.join(fileNames.toArray(), ';'));
            res.put("output", etlName);
            res.put("status", "success");
            results.add(res);
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (IllegalArgumentException iae) {
            throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
        }
        catch (Exception ex) {
            String msg = "File import failed: " + ex.getMessage();
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        return Response.ok(results).build();
    }

    @GET
    @Path("/getoptions")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getOptions() {
        JSONObject template;
        JSONParser parser = new JSONParser();
        try (FileReader fileReader = new FileReader(new File(HOME_FOLDER, IMPORT_OPTIONS))) {
            template = (JSONObject) parser.parse(fileReader);
        }
        catch (Exception e) {
            String msg = "Error getting import options!  Cause: " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }
        return Response.ok(template).build();
    }
}
