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

import static hoot.services.HootProperties.ETL_MAKEFILE;
import static hoot.services.HootProperties.HOOTAPI_DB_URL;
import static hoot.services.HootProperties.UPLOAD_FOLDER;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import org.apache.commons.lang3.StringUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.ParseException;

import hoot.services.command.ExternalCommand;
import hoot.services.utils.JsonUtils;


class FileETLCommand extends ExternalCommand {

    FileETLCommand(JSONArray reqList, int zipCnt, int shpZipCnt, int fgdbZipCnt, int osmZipCnt, int geonamesZipCnt,
                   int shpCnt, int fgdbCnt, int osmCnt, int geonamesCnt, List<String> zipList, String translation,
                   String jobId, String etlName, List<String> inputsList, String userEmail,
                   String isNoneTranslation, String fgdbFeatureClasses, Class<?> caller) {

        String inputs = "";
        for (Object r : reqList) {
            JSONObject rr = (JSONObject) r;
            inputs += "\"" + rr.get("name") + "\" ";
        }

        JSONObject param = new JSONObject();

        // if fgdb zip > 0 then all becomes fgdb so it can be uzipped first
        // if fgdb zip == 0 and shp zip > then it is standard zip.
        // if fgdb zip == 0 and shp zip == 0 and osm zip > 0 then it is osm zip
        String curInputType = "";
        if (zipCnt > 0) {
            if (fgdbZipCnt > 0) {
                String mergedZipList = StringUtils.join(zipList.toArray(), ';');
                param.put("UNZIP_LIST", mergedZipList);
                curInputType = "OGR";
            }
            else {
                // Mix of shape and zip then we will unzip and treat it like OGR
                if (shpCnt > 0) { // One or more all ogr zip + shape
                    curInputType = "OGR";
                    String mergedZipList = StringUtils.join(zipList.toArray(), ';');
                    param.put("UNZIP_LIST", mergedZipList);
                }
                else if (osmCnt > 0) { // Mix of One or more all osm zip + osm
                    curInputType = "OSM";
                    String mergedZipList = StringUtils.join(zipList.toArray(), ';');
                    param.put("UNZIP_LIST", mergedZipList);
                }
                else if (geonamesCnt > 0) { // Mix of One or more all osm zip + osm
                    curInputType = "GEONAMES";
                    String mergedZipList = StringUtils.join(zipList.toArray(), ';');
                    param.put("UNZIP_LIST", mergedZipList);
                }
                else { // One or more zip (all ogr) || One or more zip (all osm)

                    // If contains zip of just shape or osm then we will etl zip directly
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

        String translationPath = "translations" + File.separator + translation;

        if (translation.contains("/")) {
            translationPath = translation;
        }

        // Formulate request parameters

        param.put("NONE_TRANSLATION", isNone.toString());
        param.put("TRANSLATION", translationPath);
        param.put("INPUT_TYPE", curInputType);
        param.put("INPUT_PATH", UPLOAD_FOLDER + File.separator + jobId);
        param.put("INPUT", inputs);
        param.put("INPUT_NAME", etlName);
        param.put("USER_EMAIL", userEmail);
        param.put("DB_URL", HOOTAPI_DB_URL);

        if (curInputType.equalsIgnoreCase("FGDB") && (fgdbFeatureClasses != null) && (!fgdbFeatureClasses.isEmpty())) {
            Object oRq = reqList.get(0);

            if (oRq != null) {
                JSONObject jsonReq = (JSONObject) oRq;
                String rawInput = jsonReq.get("name").toString();
                List<String> fgdbInputs = new ArrayList<>();
                String[] cls = fgdbFeatureClasses.split(",");

                for (String cl : cls) {
                    fgdbInputs.add(rawInput + "\\;" + cl);
                }

                String fgdbInput = StringUtils.join(fgdbInputs.toArray(), ' ');
                param.put("INPUT", fgdbInput);
            }
        }

        JSONArray commandArgs;
        try {
            commandArgs = JsonUtils.parseParams(param.toJSONString());
        }
        catch (ParseException pe) {
            throw new RuntimeException("Error parsing: " + param.toJSONString(), pe);
        }

        super.configureAsMakeCommand(ETL_MAKEFILE, caller, commandArgs);
    }
}
