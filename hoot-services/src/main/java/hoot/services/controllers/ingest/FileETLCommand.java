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
package hoot.services.controllers.ingest;

import static hoot.services.HootProperties.*;

import java.io.File;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

import org.apache.commons.lang3.StringUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import hoot.services.command.ExternalCommand;

/*
    #
    #  ETL Make file
    #

    HOOT_OPTS+= -D osm2ogr.ops=hoot::DecomposeBuildingRelationsVisitor
    HOOT_OPTS+= -D hootapi.db.writer.overwrite.map=true -D hootapi.db.writer.create.user=true
    HOOT_OPTS+= -D api.db.email=test@test.com

    OP_INPUT=$(INPUT)
    OP_TRANSLATION=$(HOOT_HOME)/$(TRANSLATION)
    OP_INPUT_PATH=$(INPUT_PATH)

    # This replaces semicolon with vsizip and path
    ifeq "$(INPUT_TYPE)" "ZIP"
        #OP_INPUT="/vsizip/$(OP_INPUT_PATH)/$(subst ;,/" "/vsizip/$(OP_INPUT_PATH)/,$(INPUT))/"
        OP_INPUT="/vsizip/$(OP_INPUT_PATH)/$(subst ;," "/vsizip/$(OP_INPUT_PATH),$(INPUT))"
    endif

    ifeq "$(INPUT_TYPE)" "GEONAMES"
        HOOT_OPTS+= -D convert.ops=hoot::TranslationOp
        HOOT_OPTS+= -D translation.script="$(OP_TRANSLATION)"
    endif

    ifeq "$(INPUT_TYPE)" "OSM"
        ifneq "$(NONE_TRANSLATION)" "true"
            HOOT_OPTS+= -D convert.ops=hoot::TranslationOp
            HOOT_OPTS+= -D translation.script="$(OP_TRANSLATION)"
        endif
    endif

    ###
    # Transform and load data
    ###
    step1:

    # Unzip when semicolon separated lists are provided
    ifneq ($(strip $(UNZIP_LIST)), )
        bash $(HOOT_HOME)/scripts/util/unzipfiles.sh "$(UNZIP_LIST)" "$(OP_INPUT_PATH)"
    endif

    ifeq "$(INPUT_TYPE)" "OGR"
        cd "$(OP_INPUT_PATH)" && hoot ogr2osm $(HOOT_OPTS) "$(OP_TRANSLATION)" "$(DB_URL)/$(INPUT_NAME)" $(OP_INPUT)
    endif

    ifeq "$(INPUT_TYPE)" "OSM"
        cd "$(OP_INPUT_PATH)" && hoot convert $(HOOT_OPTS) $(OP_INPUT) "$(DB_URL)/$(INPUT_NAME)"
    endif

    ifeq "$(INPUT_TYPE)" "ZIP"
        cd "$(OP_INPUT_PATH)" && hoot ogr2osm $(HOOT_OPTS) "$(OP_TRANSLATION)" "$(DB_URL)/$(INPUT_NAME)" $(OP_INPUT)
    endif

    ifeq "$(INPUT_TYPE)" "FGDB"
        cd "$(OP_INPUT_PATH)" && hoot ogr2osm $(HOOT_OPTS) "$(OP_TRANSLATION)" "$(DB_URL)/$(INPUT_NAME)" $(OP_INPUT)
    endif

    ifeq "$(INPUT_TYPE)" "GEONAMES"
        cd "$(OP_INPUT_PATH)" && hoot convert $(HOOT_OPTS) $(OP_INPUT) "$(DB_URL)/$(INPUT_NAME)"
    endif

    rm -rf "$(OP_INPUT_PATH)"
*/


class FileETLCommand extends ExternalCommand {

    FileETLCommand(JSONArray reqList, int zipCnt, int shpZipCnt, int fgdbZipCnt,
                   int osmZipCnt, int geonamesZipCnt, int shpCnt, int fgdbCnt,
                   int osmCnt, int geonamesCnt, List<String> zipList, String translation,
                   String jobId, String etlName, List<String> inputsList, String userEmail,
                   Boolean isNoneTranslation, String fgdbFeatureClasses, String debugLevel, Class<?> caller) {

        String inputs = "";
        for (Object r : reqList) {
            JSONObject rr = (JSONObject) r;
            inputs += "\"" + rr.get("name") + "\" ";
        }

        JSONObject param = new JSONObject();

        // if fgdb zip > 0 then all becomes fgdb so it can be uzipped first
        // if fgdb zip == 0 and shp zip > then it is standard zip.
        // if fgdb zip == 0 and shp zip == 0 and osm zip > 0 then it is osm zip
        String inputType = "";
        if (zipCnt > 0) {
            if (fgdbZipCnt > 0) {
                String mergedZipList = StringUtils.join(zipList.toArray(), ';');
                param.put("UNZIP_LIST", mergedZipList);
                inputType = "OGR";
            }
            else {
                // Mix of shape and zip then we will unzip and treat it like OGR
                if (shpCnt > 0) { // One or more all ogr zip + shape
                    inputType = "OGR";
                    String mergedZipList = StringUtils.join(zipList.toArray(), ';');
                    param.put("UNZIP_LIST", mergedZipList);
                }
                else if (osmCnt > 0) { // Mix of One or more all osm zip + osm
                    inputType = "OSM";
                    String mergedZipList = StringUtils.join(zipList.toArray(), ';');
                    param.put("UNZIP_LIST", mergedZipList);
                }
                else if (geonamesCnt > 0) { // Mix of One or more all osm zip + osm
                    inputType = "GEONAMES";
                    String mergedZipList = StringUtils.join(zipList.toArray(), ';');
                    param.put("UNZIP_LIST", mergedZipList);
                }
                else { // One or more zip (all ogr) || One or more zip (all osm)

                    // If contains zip of just shape or osm then we will etl zip directly
                    inputType = "ZIP";

                    // add zip extension
                    for (int j = 0; j < zipList.size(); j++) {
                        zipList.set(j, zipList.get(j) + ".zip");
                    }

                    inputs = StringUtils.join(zipList.toArray(), ';');
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

        //if (translation.contains("/")) {
        //    translationPath = translation;
        //}

        //HOOT_OPTS+= -D osm2ogr.ops=hoot::DecomposeBuildingRelationsVisitor
        //HOOT_OPTS+= -D hootapi.db.writer.overwrite.map=true -D hootapi.db.writer.create.user=true
        //HOOT_OPTS+= -D api.db.email=test@test.com

        List<String> hootOptions = new LinkedList<>();
        hootOptions.add("-D osm2ogr.ops=hoot::DecomposeBuildingRelationsVisitor");
        hootOptions.add("-D hootapi.db.writer.overwrite.map=true");
        hootOptions.add("-D hootapi.db.writer.create.user=true");
        hootOptions.add("-D api.db.email=test@test.com");

        //ifeq "$(INPUT_TYPE)" "GEONAMES"
        //    HOOT_OPTS+= -D convert.ops=hoot::TranslationOp
        //    HOOT_OPTS+= -D translation.script="$(OP_TRANSLATION)"
        //endif

        // OP_TRANSLATION=$(HOOT_HOME)/$(TRANSLATION)

        String translationPath = new File(new File(HOME_FOLDER, "translations"), translation).getAbsolutePath();
        if ("GEONAMES".equals(inputType)) {
            hootOptions.add("-D convert.ops=hoot::TranslationOp");
            hootOptions.add("-D translation.script=\"" + translationPath + "\"");
        }

        //ifeq "$(INPUT_TYPE)" "OSM"
        //    ifneq "$(NONE_TRANSLATION)" "true"
        //        HOOT_OPTS+= -D convert.ops=hoot::TranslationOp
        //       HOOT_OPTS+= -D translation.script="$(OP_TRANSLATION)"
        //    endif
        //endif

        if ("OSM".equals(inputType) && !isNoneTranslation) {
            hootOptions.add("-D convert.ops=hoot::TranslationOp");
            hootOptions.add("-D translation.script=\"" + translation + "\"");
        }

        // OP_INPUT_PATH (INPUT_PATH)
        File workingDir = new File(UPLOAD_FOLDER, jobId);

        //# This replaces semicolon with vsizip and path
        //ifeq "$(INPUT_TYPE)" "ZIP"
        //   #OP_INPUT="/vsizip/$(OP_INPUT_PATH)/$(subst ;,/" "/vsizip/$(OP_INPUT_PATH)/,$(INPUT))/"
        //   OP_INPUT="/vsizip/$(OP_INPUT_PATH)/$(subst ;," "/vsizip/$(OP_INPUT_PATH),$(INPUT))"
        //endif

        if ("ZIP".equals(inputType)) {
            //Reading a GDAL dataset in a .gz file or a .zip archive
            inputs.replace(";", "/vsizip/" + workingDir.getAbsolutePath());
        }

        // Formulate request parameters

        //param.put("NONE_TRANSLATION", isNone.toString());
        //param.put("TRANSLATION", translationPath);
        //param.put("INPUT_TYPE", inputType);
        //param.put("INPUT_PATH", UPLOAD_FOLDER + File.separator + jobId);
        //param.put("INPUT", inputs);
        //param.put("INPUT_NAME", etlName);
        //param.put("USER_EMAIL", userEmail);
        //param.put("DB_URL", HOOTAPI_DB_URL);

        if (inputType.equalsIgnoreCase("FGDB") && !StringUtils.isBlank(fgdbFeatureClasses)) {
            Object oRq = reqList.get(0);

            if (oRq != null) {
                JSONObject json = (JSONObject) oRq;
                String rawInput = json.get("name").toString();
                List<String> fgdbInputs = new ArrayList<>();
                String[] classes = fgdbFeatureClasses.split(",");

                for (String clazz : classes) {
                    fgdbInputs.add(rawInput + "\\;" + clazz);
                }

                String fgdbInput = StringUtils.join(fgdbInputs.toArray(), ' ');
                inputs = fgdbInput;
                //param.put("INPUT", fgdbInput);
            }
        }

        JSONArray commandArgs = new JSONArray();
        //try {
        //    commandArgs = JsonUtils.parseParams(param.toJSONString());
        //}
        //catch (ParseException pe) {
        //    throw new RuntimeException("Error parsing: " + param.toJSONString(), pe);
        //}

        //ifeq "$(INPUT_TYPE)" "OGR"
        //    cd "$(OP_INPUT_PATH)" && hoot ogr2osm $(HOOT_OPTS) "$(OP_TRANSLATION)" "$(DB_URL)/$(INPUT_NAME)" $(OP_INPUT)
        //endif

        //ifeq "$(INPUT_TYPE)" "FGDB"
        //    cd "$(OP_INPUT_PATH)" && hoot ogr2osm $(HOOT_OPTS) "$(OP_TRANSLATION)" "$(DB_URL)/$(INPUT_NAME)" $(OP_INPUT)
        //endif

        //ifeq "$(INPUT_TYPE)" "ZIP"
        //    cd "$(OP_INPUT_PATH)" && hoot ogr2osm $(HOOT_OPTS) "$(OP_TRANSLATION)" "$(DB_URL)/$(INPUT_NAME)" $(OP_INPUT)
        //endif

        //ifeq "$(INPUT_TYPE)" "OSM"
        //    cd "$(OP_INPUT_PATH)" && hoot convert $(HOOT_OPTS) $(OP_INPUT) "$(DB_URL)/$(INPUT_NAME)"
        //endif

        //ifeq "$(INPUT_TYPE)" "GEONAMES"
        //    cd "$(OP_INPUT_PATH)" && hoot convert $(HOOT_OPTS) $(OP_INPUT) "$(DB_URL)/$(INPUT_NAME)"
        //endif

        JSONObject arg = new JSONObject();
        arg.put("DEBUG_LEVEL", "--" + debugLevel);
        commandArgs.add(arg);

        arg = new JSONObject();
        arg.put("HOOT_OPTIONS", StringUtils.join(hootOptions, " "));
        commandArgs.add(arg);

        if ("OGR".equals(inputType) || "FGDB".equals(inputType) || "ZIP".equals(inputType)) {
            arg = new JSONObject();
            arg.put("TRANSLATION", translationPath);
            commandArgs.add(arg);

            arg = new JSONObject();
            arg.put("INPUT_NAME", HOOTAPI_DB_URL + "/" + etlName);
            commandArgs.add(arg);

            arg = new JSONObject();
            arg.put("INPUT", inputs);
            commandArgs.add(arg);

            super.configureAsHootCommand("ogr2osm", caller, commandArgs);
        }
        else if ("OSM".equals(inputType) || "GEONAMES".equals(inputType)) {
            arg = new JSONObject();
            arg.put("INPUT", inputs);
            commandArgs.add(arg);

            arg = new JSONObject();
            arg.put("INPUT_NAME", HOOTAPI_DB_URL + "/" + etlName);
            commandArgs.add(arg);

            super.configureAsHootCommand("convert", caller, commandArgs);
        }

        // override working directory set during super.configureAsHootCommand()
        this.put("workingDir", workingDir);
    }
}
