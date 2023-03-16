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
 * @copyright Copyright (C) 2016-2023 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.ingest;

import static hoot.services.HootProperties.GRAIL_OVERPASS_QUERY;
import static hoot.services.HootProperties.HOME_FOLDER;
import static hoot.services.HootProperties.HOOTAPI_DB_URL;
import static hoot.services.HootProperties.PRIVATE_OVERPASS_CERT_PATH;
import static hoot.services.HootProperties.PRIVATE_OVERPASS_CERT_PHRASE;
import static hoot.services.HootProperties.PRIVATE_OVERPASS_URL;
import static hoot.services.HootProperties.PUBLIC_OVERPASS_URL;
import static hoot.services.HootProperties.SCRIPT_FOLDER;
import static hoot.services.HootProperties.replaceSensitiveData;
import static hoot.services.controllers.ingest.UploadClassification.ZIP;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;

import org.apache.commons.io.FileUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.HootProperties;
import hoot.services.command.CommandResult;
import hoot.services.command.ExternalCommand;
import hoot.services.controllers.grail.GrailParams;
import hoot.services.geo.BoundingBox;
import hoot.services.models.db.Users;


public class ImportCommand extends ExternalCommand {
    private static final Logger logger = LoggerFactory.getLogger(ImportCommand.class);

    private final File workDir;

    /*
     * This is the import command to load data from a local file that has been uploaded
     */
    ImportCommand(String jobId, File workDir, List<File> filesToImport, List<File> zipsToImport, String translation, String advUploadOpts,
                  String etlName, Boolean isNoneTranslation, String debugLevel, UploadClassification classification,
                  Class<?> caller, Users user) {
        super(jobId);
        this.workDir = workDir;

        List<String> inputs = filesToImport.stream().map(File::getAbsolutePath).collect(Collectors.toList());

        List<String> hootOptions = buildHootOptions(user, translation, isNoneTranslation, advUploadOpts);

        String inputName = HOOTAPI_DB_URL + "/" + etlName;

        Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("HOOT_OPTIONS", hootOptions);
        substitutionMap.put("INPUT_NAME", inputName);
        substitutionMap.put("INPUTS", inputs);

        if ((classification == ZIP) && !zipsToImport.isEmpty()) {
            //Reading a GDAL dataset in a .gz file or a .zip archive
            inputs = zipsToImport.stream().map(zip -> "/vsizip/" + zip.getAbsolutePath()).collect(Collectors.toList());
            substitutionMap.put("INPUTS", inputs);
        }

        String command = "hoot.bin convert --${DEBUG_LEVEL} -C Import.conf ${HOOT_OPTIONS} ${INPUTS} ${INPUT_NAME}";

        super.configureCommand(command, substitutionMap, caller);
    }

    private List<String> buildHootOptions(Users user, String translation, Boolean isNoneTranslation, String advUploadOpts) {
        List<String> options = new LinkedList<>();
        //TODO: always set remap ids to false??
        options.add("hootapi.db.writer.overwrite.map=true");
        options.add("job.id=" + jobId);
        options.add("api.db.email=" + user.getEmail());

        //Built-in translations are passed with the translations/ or translations-local/ path in the name
        String translationPath;
        if (translation.matches("translations.*/.+")) {
            translationPath = new File(HOME_FOLDER, translation).getAbsolutePath();
        } else { //user submitted translations are in the customscript path
            translationPath = new File(new File(SCRIPT_FOLDER), translation).getAbsolutePath();
        }
        //A boolean gets passed if we don't mean to translate, but the UI has a dummy translation name of None
        if (!isNoneTranslation) {
          options.add("schema.translation.script=" + translationPath);
        } else {
            options.add("schema.translation.script=" + new File(HOME_FOLDER, "translations/quick.js").getAbsolutePath());
        }

        if (advUploadOpts != null && !advUploadOpts.isEmpty()) {
            List<String> getAdvOpts = Arrays.asList(advUploadOpts.split(","));
            for (String option: getAdvOpts) {
                String[] opt = option.split("=");
                String key = opt[0];
                String value = (opt.length == 2) ? "=" + opt[1] : "";

                if (configOptions.containsKey(key)) { // if option key in possible values, add new option command
                    Map<String, String> optionConfig = configOptions.get(key);
                    options.add(optionConfig.get("key") + value);
                }
            }
        }

        List<String> hootOptions = toHootOptions(options);
        return hootOptions;
    }

    /*
     * This is the import command for pulling remote data from a url
     */
    ImportCommand(String jobId, String url, String username, String password, String translation, String advUploadOpts,
                  String etlName, Boolean isNoneTranslation, String debugLevel, UploadClassification classification,
                  Class<?> caller, Users user) {
        super(jobId);
        this.workDir = null;

        String input;
        List<String> envVars = new ArrayList<>();
        envVars.add("OSM_USE_CUSTOM_INDEXING=NO"); //allows negative IDs on elements when read by OGR
        String[] urlParts = url.split("://");
        if (url.startsWith("s3")) {
            input = url.replace("s3://", "");
            input = "/vsis3/" + input;
            if (!username.isEmpty() && !password.isEmpty()) {
                envVars.add(String.format("AWS_SECRET_ACCESS_KEY=%s", password));
                envVars.add(String.format("AWS_ACCESS_KEY_ID=%s", username));
            }
        } else if (url.startsWith("http") || url.startsWith("ftp")) {
            if (!username.isEmpty() && !password.isEmpty()) {
                input = "/vsicurl/" + urlParts[0] + "://" + username + ":" + password + "@" + urlParts[1];
            } else {
                input = "/vsicurl/" + url;
            }
        } else {
            input = url;
        }

        if ((classification == ZIP)) {
            input = "/vsizip/" + input;
        }

        List<String> hootOptions = buildHootOptions(user, translation, isNoneTranslation, advUploadOpts);

        String inputName = HOOTAPI_DB_URL + "/" + etlName;
        java.util.Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("ENV_VARS", envVars);
        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("HOOT_OPTIONS", hootOptions);
        substitutionMap.put("INPUT_NAME", inputName);
        substitutionMap.put("INPUTS", input);


        String command = "hoot.bin convert --${DEBUG_LEVEL} -C Import.conf ${HOOT_OPTIONS} ${INPUTS} ${INPUT_NAME}";

        super.configureCommand(command, substitutionMap, caller);
    }

    /*
     * This is the import command to pull data from Overpass API
     */
    ImportCommand(String jobId, GrailParams params, String debugLevel, Class<?> caller) {
        super(jobId);

        if (params.getBounds() == null) {
            throw new IllegalArgumentException("bounds params cannot be null for Overpass ImportCommand");
        }

        if (params.getOutput() == null) {
            throw new IllegalArgumentException("outputPath param cannot be null for Overpass ImportCommand");
        }

        this.workDir = params.getWorkDir();

        boolean isPrivateOverpass = params.getPullUrl().equals(PRIVATE_OVERPASS_URL);
        boolean isOverpass = isPrivateOverpass || params.getPullUrl().equals(PUBLIC_OVERPASS_URL);
        String url = HootProperties.replaceSensitiveData(params.getPullUrl());
        Users user = params.getUser();
        String bounds = params.getBounds();
        BoundingBox bbox = new BoundingBox(params.getBounds());
        double bboxArea = bbox.getArea();

        List<String> options = new ArrayList<String>();
        options.add("job.id=" + jobId);

        if (isOverpass) {
            File grailOverpassQueryFile;
            try {
                if (params.getCustomQuery() == null) {
                    grailOverpassQueryFile = new File(HOME_FOLDER, GRAIL_OVERPASS_QUERY);
                } else {
                    grailOverpassQueryFile = createTempGrailOverpassQueryFile(jobId, params.getCustomQuery());
                }
            } catch (IOException e) {
                throw new RuntimeException("Unable to create temp overpass query file", e);
            }

            String overpassHost;
            try {
                overpassHost = new URL(url).getHost();
            } catch (MalformedURLException e) {
                throw new RuntimeException("Provided grail pull url is not a valid url.", e);
            }

            // add overpass specific options
            options.add("overpass.api.query.path=" + grailOverpassQueryFile.getAbsolutePath());
            options.add("overpass.api.host=" + overpassHost);
            options.add("reader.http.bbox.max.download.size=" + bboxArea);

            if (isPrivateOverpass) {
                options.add("hoot.pkcs12.key.path=" + replaceSensitiveData(PRIVATE_OVERPASS_CERT_PATH));
                options.add("hoot.pkcs12.key.phrase=" + replaceSensitiveData(PRIVATE_OVERPASS_CERT_PHRASE));
            }
        } else {
            double maxBboxArea = params.getMaxBoundsSize();
            if (bboxArea > maxBboxArea) {
                String errorMsg = "The bounding box area (" + bboxArea + ") is too large for OSM API. It must be less than " + maxBboxArea + " degrees";
                throw new WebApplicationException(Response.status(Response.Status.BAD_REQUEST).entity(errorMsg).build());
            }
        }

        if(user == null) {
            options.add("api.db.email=test@test.com");
        } else {
            options.add("api.db.email=" + user.getEmail());
        }

        if (!bounds.contains(";")) {
            bounds = bbox.getMinLon() + "," + bbox.getMinLat() + "," + bbox.getMaxLon() + "," + bbox.getMaxLat();
        }

        options.add("bounds=" + bounds);

        List<String> hootOptions = toHootOptions(options);

        java.util.Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("HOOT_OPTIONS", hootOptions);
        substitutionMap.put("INPUT_PATH", url);
        substitutionMap.put("OUTPUT_PATH", params.getOutput());

        String command = "hoot.bin convert --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${INPUT_PATH} ${OUTPUT_PATH}";
        super.configureCommand(command, substitutionMap, caller);
    }

    private File createTempGrailOverpassQueryFile(String jobId, String overpassQuery) throws IOException {
        File tempQueryFile = File.createTempFile(jobId + "_temp_query", ".oql");
        tempQueryFile.deleteOnExit();
        BufferedWriter queryFileWriter = new BufferedWriter(
            new OutputStreamWriter(new FileOutputStream(tempQueryFile.getAbsolutePath()), "utf-8"));
        queryFileWriter.write(overpassQuery);
        queryFileWriter.close();
        return tempQueryFile;
    }

    @Override
    public CommandResult execute() {
        CommandResult commandResult = super.execute();

        if (workDir != null) {
            try {
                FileUtils.forceDelete(workDir);
            }
            catch (IOException ioe) {
                logger.error("Error deleting folder: {} ", workDir.getAbsolutePath(), ioe);
            }
        }
        return commandResult;
    }
}
