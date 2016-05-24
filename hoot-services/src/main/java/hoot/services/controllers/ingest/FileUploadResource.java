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
package hoot.services.controllers.ingest;

import java.io.FileInputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.UUID;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import javax.servlet.http.HttpServletRequest;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;

import org.apache.commons.lang3.StringUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.HootProperties;
import hoot.services.ingest.MultipartSerializer;
import hoot.services.utils.ResourceErrorHandler;


@Path("/ingest")
public class FileUploadResource extends hoot.services.controllers.job.JobControllerBase {
    private static final Logger log = LoggerFactory.getLogger(FileUploadResource.class);
    private String homeFolder = null;

    public FileUploadResource() {
        try {
            if (processScriptName == null) {
                processScriptName = HootProperties.getProperty("ETLMakefile");
            }

            homeFolder = HootProperties.getProperty("homeFolder");
        }
        catch (Exception ex) {
            log.error(ex.getMessage());
        }
    }

    /**
     * Purpose of this service is to provide ingest service for uploading shape
     * and osm file and performing ETL operation on the uploaded file(s). This
     * service is multipart post service which accepts single or multiple files
     * sent by multipart client.
     * 
     * POST
     * hoot-services/ingest/ingest/upload?TRANSLATION=NFDD.js&INPUT_TYPE=OSM&
     * INPUT_NAME=ToyTest
     * 
     * @param translation
     *            Translation script used during OGR ETL process.
     * @param inputType
     *            [OSM | OGR ] OSM for osm file and OGR for shapefile.
     * @param inputName
     *            optional input name which is used in hoot db. Defaults to the
     *            file name.
     * @param userEmail
     *            mail address of the user requesting job
     * @param noneTranslation
     *            ?
     * @param request
     *            ?
     * @return Array of job status
     */
    @POST
    @Path("/upload")
    @Produces(MediaType.TEXT_PLAIN)
    public Response processUpload2(@QueryParam("TRANSLATION") final String translation,
            @QueryParam("INPUT_TYPE") final String inputType, @QueryParam("INPUT_NAME") final String inputName,
            @QueryParam("USER_EMAIL") final String userEmail,
            @QueryParam("NONE_TRANSLATION") final String noneTranslation,
            @QueryParam("FGDB_FC") final String fgdbFeatureClasses, @Context HttpServletRequest request) {
        String etlName = inputName;
        String jobId = UUID.randomUUID().toString();
        JSONArray resA = new JSONArray();

        try {
            // Save multipart data into file
            log.debug("Starting ETL Process for:" + inputName);
            Map<String, String> uploadedFiles = new HashMap<String, String>();
            Map<String, String> uploadedFilesPaths = new HashMap<String, String>();

            MultipartSerializer ser = new MultipartSerializer();
            ser.serializeUpload(jobId, inputType, uploadedFiles, uploadedFilesPaths, request);

            int shpCnt = 0;
            int osmCnt = 0;
            int fgdbCnt = 0;
            int geonamesCnt = 0;

            int zipCnt = 0;
            int shpZipCnt = 0;
            int osmZipCnt = 0;
            int geonamesZipCnt = 0;
            int fgdbZipCnt = 0;
            List<String> zipList = new ArrayList<String>();

            JSONArray reqList = new JSONArray();
            List<String> inputsList = new ArrayList<String>();

            Iterator it = uploadedFiles.entrySet().iterator();
            while (it.hasNext()) {
                Map.Entry pairs = (Map.Entry) it.next();
                String fName = pairs.getKey().toString();
                String ext = pairs.getValue().toString();

                if (etlName == null || etlName.length() == 0) {
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
                _buildNativeRequest(jobId, fName, ext, inputFileName, reqList, zipStat);
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
            }

            if ((shpZipCnt + fgdbZipCnt + shpCnt + fgdbCnt) > 0 && (osmZipCnt + osmCnt) > 0) {
                throw new Exception("Can not mix osm and ogr type.");
            }
            if (osmZipCnt > 0) {
                // #6027
                throw new Exception("Hootennany does not support zip files containing .osm data.");
            }

            String batchJobReqStatus = "success";
            String batchJobId = UUID.randomUUID().toString();
            JSONArray jobArgs = _createNativeRequest(reqList, zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt, geonamesZipCnt,
                    shpCnt, fgdbCnt, osmCnt, geonamesCnt, zipList, translation, jobId, etlName, inputsList, userEmail,
                    noneTranslation, fgdbFeatureClasses);

            // userEmail

            log.debug("Posting Job Request for Job :" + batchJobId + " With Args: " + jobArgs.toJSONString());
            postChainJobRquest(batchJobId, jobArgs.toJSONString());

            String mergedInputList = StringUtils.join(inputsList.toArray(), ';');
            JSONObject res = new JSONObject();
            res.put("jobid", batchJobId);
            res.put("input", mergedInputList);
            res.put("output", etlName);
            res.put("status", batchJobReqStatus);

            resA.add(res);
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Failed upload: " + ex.toString(), Status.INTERNAL_SERVER_ERROR, log);
        }
        return Response.ok(resA.toJSONString(), MediaType.APPLICATION_JSON).build();
    }

    /*
     * final int shpZipCnt, final int osmZipCnt, final int geonamesZipCnt, final
     * List<String> inputsList,
     */
    protected JSONArray _createNativeRequest(final JSONArray reqList, final int zipCnt, final int shpZipCnt,
            final int fgdbZipCnt, final int osmZipCnt, final int geonamesZipCnt, final int shpCnt, final int fgdbCnt,
            final int osmCnt, final int geonamesCnt, final List<String> zipList, final String translation,
            final String jobId, final String etlName, final List<String> inputsList, final String userEmail,
            final String isNoneTranslation, final String fgdbFeatureClasses) throws Exception {
        JSONArray jobArgs = new JSONArray();
        String curInputType = "";

        String inputs = "";
        for (Object r : reqList) {
            JSONObject rr = (JSONObject) r;
            inputs += "\"" + rr.get("name").toString() + "\" ";
        }

        JSONObject param = new JSONObject();

        // if fgdb zip > 0 then all becomes fgdb so it can be uzipped first
        // if fgdb zip == 0 and shp zip > then it is standard zip.
        // if fgdb zip == 0 and shp zip == 0 and osm zip > 0 then it is osm zip
        if (zipCnt > 0) {
            if (fgdbZipCnt > 0) {
                String mergedZipList = StringUtils.join(zipList.toArray(), ';');
                param.put("UNZIP_LIST", mergedZipList);
                curInputType = "OGR";
            }
            else {
                // Mix of shape and zip then we will unzip and treat it like OGR
                if (shpCnt > 0) // One or more all ogr zip + shape
                {
                    curInputType = "OGR";
                    String mergedZipList = StringUtils.join(zipList.toArray(), ';');
                    param.put("UNZIP_LIST", mergedZipList);
                }
                else if (osmCnt > 0) // Mix of One or more all osm zip + osm
                {
                    curInputType = "OSM";
                    String mergedZipList = StringUtils.join(zipList.toArray(), ';');
                    param.put("UNZIP_LIST", mergedZipList);
                }
                else if (geonamesCnt > 0) // Mix of One or more all osm zip +
                                          // osm
                {
                    curInputType = "GEONAMES";
                    String mergedZipList = StringUtils.join(zipList.toArray(), ';');
                    param.put("UNZIP_LIST", mergedZipList);
                }
                else // One or more zip (all ogr) || One or more zip (all osm)
                {
                    // If contains zip of just shape or osm then we will etl zip
                    // directly
                    curInputType = "ZIP";
                    // add zip extension

                    for (int j = 0; j < zipList.size(); j++) {
                        zipList.set(j, zipList.get(j) + ".zip");
                    }
                    inputs = StringUtils.join(zipList.toArray(), ';');
                }
            }
        }
        else if (shpCnt > 0) {
            curInputType = "OGR";
        }
        else if (osmCnt > 0) {
            curInputType = "OSM";
        }
        else if (fgdbCnt > 0) {
            curInputType = "FGDB";
        }
        else if (geonamesCnt > 0) {
            curInputType = "GEONAMES";
        }

        Boolean isNone = false;
        if (isNoneTranslation != null) {
            isNone = isNoneTranslation.equals("true");
        }

        String translationPath = "translations/" + translation;

        if (translation.contains("/")) {
            translationPath = translation;
        }
        log.debug("Using Translation for ETL :" + translationPath);

        // Formulate request parameters

        param.put("NONE_TRANSLATION", isNone.toString());
        param.put("TRANSLATION", translationPath);
        param.put("INPUT_TYPE", curInputType);
        param.put("INPUT_PATH", "upload/" + jobId);
        param.put("INPUT", inputs);
        param.put("INPUT_NAME", etlName);
        param.put("USER_EMAIL", userEmail);
        if (curInputType.equalsIgnoreCase("FGDB") && fgdbFeatureClasses != null && fgdbFeatureClasses.length() > 0) {
            Object oRq = reqList.get(0);

            if (oRq != null) {
                JSONObject jsonReq = (JSONObject) oRq;
                String rawInput = jsonReq.get("name").toString();
                String fgdbInput = "";
                List<String> fgdbInputs = new ArrayList<>();
                String[] cls = fgdbFeatureClasses.split(",");

                for (int i = 0; i < cls.length; i++) {
                    String cl = cls[i];
                    fgdbInputs.add(rawInput + "\\;" + cl);
                }

                fgdbInput = StringUtils.join(fgdbInputs.toArray(), ' ');
                param.put("INPUT", fgdbInput);
            }
        }

        JSONArray commandArgs = parseParams(param.toJSONString());

        JSONObject etlCommand = _createMakeScriptJobReq(commandArgs);

        // Density Raster
        String internalJobId = UUID.randomUUID().toString();
        JSONArray rasterTilesArgs = new JSONArray();
        JSONObject rasterTilesparam = new JSONObject();
        rasterTilesparam.put("value", etlName);
        rasterTilesparam.put("paramtype", String.class.getName());
        rasterTilesparam.put("isprimitivetype", "false");
        rasterTilesArgs.add(rasterTilesparam);

        rasterTilesparam = new JSONObject();
        rasterTilesparam.put("value", userEmail);
        rasterTilesparam.put("paramtype", String.class.getName());
        rasterTilesparam.put("isprimitivetype", "false");
        rasterTilesArgs.add(rasterTilesparam);

        JSONObject ingestOSMResource = _createReflectionJobReq(rasterTilesArgs,
                "hoot.services.controllers.ingest.RasterToTilesService", "ingestOSMResourceDirect", internalJobId);

        jobArgs.add(etlCommand);
        jobArgs.add(ingestOSMResource);

        return jobArgs;
    }

    protected void _buildNativeRequest(final String jobId, final String fName, final String ext,
            final String inputFileName, JSONArray reqList, JSONObject zipStat) throws Exception {
        // get zip stat is not exist then create one
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

        if (ext.equalsIgnoreCase("osm")) {
            JSONObject reqType = new JSONObject();
            reqType.put("type", "OSM");
            reqType.put("name", inputFileName);
            reqList.add(reqType);
            osmCnt++;
        }
        else if (ext.equalsIgnoreCase("geonames")) {
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
            String zipFilePath = homeFolder + "/upload/" + jobId + "/" + inputFileName;

            JSONObject res = _getZipContentType(zipFilePath, reqList, fName);

            shpZipCnt += (Integer) res.get("shpcnt");
            fgdbZipCnt += (Integer) res.get("fgdbcnt");
            osmZipCnt += (Integer) res.get("osmcnt");
            geonamesZipCnt += (Integer) res.get("geonamescnt");

            // We do not allow mix of ogr and osm in zip
            if ((shpZipCnt + fgdbZipCnt) > 0 && osmZipCnt > 0) {
                throw new Exception("Zip should not contain both osm and ogr types.");
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
    protected JSONObject _getZipContentType(final String zipFilePath, JSONArray contentTypes, String fName)
            throws Exception {
        JSONObject resultStat = new JSONObject();
        String[] extList = { "gdb", "osm", "shp", "geonames" };

        int shpCnt = 0;
        int osmCnt = 0;
        int geonamesCnt = 0;
        int fgdbCnt = 0;
        ZipInputStream zis = null;
        try {
            zis = new ZipInputStream(new FileInputStream(zipFilePath));
            ZipEntry ze = zis.getNextEntry();

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
                    throw new Exception("Unknown file type.");
                }

                // for each type of extensions
                for (int i = 0; i < extList.length; i++) {
                    if (ext.equalsIgnoreCase(extList[i])) {
                        if (ze.isDirectory()) {
                            if (ext.equals("gdb")) {
                                JSONObject contentType = new JSONObject();
                                contentType.put("type", "FGDB_ZIP");
                                contentType.put("name", fName + "/" + zipName);
                                contentTypes.add(contentType);
                                fgdbCnt++;
                            }
                            else {
                                throw new Exception("Unknown folder type. Only gdb folder type is supported.");
                            }
                        }
                        else // file
                        {
                            if (ext.equals("shp")) {
                                JSONObject contentType = new JSONObject();
                                contentType.put("type", "OGR_ZIP");
                                contentType.put("name", fName + "/" + zipName);
                                contentTypes.add(contentType);
                                shpCnt++;
                            }
                            else if (ext.equals("osm")) {
                                JSONObject contentType = new JSONObject();
                                contentType.put("type", "OSM_ZIP");
                                contentType.put("name", fName + "/" + zipName);
                                contentTypes.add(contentType);
                                osmCnt++;
                            }
                            else if (ext.equals("geonames")) {
                                JSONObject contentType = new JSONObject();
                                contentType.put("type", "GEONAMES_ZIP");
                                contentType.put("name", fName + "/" + zipName);
                                contentTypes.add(contentType);
                                geonamesCnt++;
                            }
                            else {
                                // We will not throw error here since shape file
                                // can contain mutiple types of support files.
                                // We will let hoot-core decide if it can handle
                                // the zip.
                            }
                        }
                    }
                    // We do not allow mix of ogr and osm in zip
                    if ((shpCnt + fgdbCnt) > 0 && osmCnt > 0) {
                        throw new Exception("Zip should not contain both osm and ogr types.");
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
}
