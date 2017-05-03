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
import static hoot.services.controllers.ingest.UploadClassification.*;

import java.io.File;
import java.util.ArrayList;
import java.util.EnumMap;
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
import hoot.services.command.ExternalCommand;
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

            int shpCnt = 0, osmCnt = 0, fgdbCnt = 0, geonamesCnt = 0, zipCnt = 0;
            int shpZipCnt = 0, osmZipCnt = 0, geonamesZipCnt = 0, fgdbZipCnt = 0;

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
                geonamesCnt += counts.get(GEONAMES);
                shpZipCnt += counts.get(SHAPE_ZIP);
                fgdbZipCnt += counts.get(FGDB_ZIP);
                osmZipCnt += counts.get(OSM_ZIP);
                geonamesZipCnt += counts.get(GEONAMES_ZIP);

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
                    shpZipCnt, fgdbZipCnt, osmZipCnt, geonamesZipCnt, shpCnt, fgdbCnt, osmCnt, geonamesCnt);

            ExternalCommand importCommand = fileETLCommandFactory.build(jobId, workDir, filesToImport, zipsToImport, translation,
                    etlName, noneTranslation, debugLevel, finalUploadClassification, this.getClass());

            ExternalCommand exportRenderDBCommand = exportRenderDBCommandFactory.build(jobId, etlName, this.getClass());

            Command[] workflow = { importCommand, exportRenderDBCommand };

            jobProcessor.submitAsync(new Job(jobId, workflow));

            JSONObject res = new JSONObject();
            res.put("jobid", jobId);
            res.put("input", StringUtils.join(fileNames.toArray(), ';'));
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
            String msg = "File import failed: " + ex.getMessage();
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        return Response.ok(response.toJSONString()).build();
    }
}
