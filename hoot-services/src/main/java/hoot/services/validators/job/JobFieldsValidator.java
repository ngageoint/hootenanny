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
package hoot.services.validators.job;

import java.io.FileReader;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.HootProperties;


public class JobFieldsValidator {
    private static final Logger logger = LoggerFactory.getLogger(JobFieldsValidator.class);
    private static final String metaDataPath;
    private static final JSONObject metaData;

    private String resourceName;

    static {
        metaDataPath = HootProperties.getProperty("homeFolder") + "/scripts/services_fields_metadata.json";
        try {
            try (FileReader fReader = new FileReader(metaDataPath)) {
                JSONParser parser = new JSONParser();
                metaData = (JSONObject) parser.parse(fReader);
            }
        }
        catch (Exception e) {
            throw new RuntimeException("Error loading services fields' metadata!", e);
        }
    }

    public JobFieldsValidator(String resourceName) {
        this.resourceName = resourceName;
    }

    public boolean validateRequiredExists(Map<String, String> fields, List<String> missingList) {
        boolean isValid = true;

        Object resMeta = metaData.get(this.resourceName);
        if (resMeta != null) {
            // get the list of required fields
            List<String> reqList = new ArrayList<>();
            JSONObject o = (JSONObject) resMeta;
            Iterator iter = o.entrySet().iterator();
            while (iter.hasNext()) {
                Map.Entry mEntry = (Map.Entry) iter.next();
                String key = (String) mEntry.getKey();
                JSONObject val = (JSONObject) mEntry.getValue();

                Object oReq = val.get("required");
                Boolean isReq = false;
                if (oReq != null) {
                    isReq = oReq.toString().equalsIgnoreCase("true");
                }

                if (isReq) {
                    reqList.add(key);
                }
            }

            // check for empty field val
            iter = fields.entrySet().iterator();
            while (iter.hasNext()) {
                Map.Entry mEntry = (Map.Entry) iter.next();
                Object key = mEntry.getKey();
                Object val = mEntry.getValue();

                if (val == null) {
                    return false;
                }

                if (key == null) {
                    return true;
                }
            }

            // now see if all required fields exists
            for (String key : reqList) {
                if (fields.get(key) == null) {
                    missingList.add(key);
                    isValid = false;
                }
            }
        }
        else {
            // nothing to check with so we pass
            isValid = true;
        }

        return isValid;
    }
}
