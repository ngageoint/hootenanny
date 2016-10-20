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
package hoot.services.utils;

import java.io.IOException;
import java.io.StringWriter;
import java.util.HashMap;
import java.util.Map;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.fasterxml.jackson.core.JsonFactory;
import com.fasterxml.jackson.core.JsonGenerator;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;


/**
 * Various JSON utilities
 */
public final class JsonUtils {

    private static final Logger logger = LoggerFactory.getLogger(JsonUtils.class);

    private JsonUtils() {
    }

    /**
     * Converts a POJO to JSON
     * 
     * @param obj
     *            POJO
     * @return JSON string
     * @throws IOException
     */
    public static String objectToJson(Object obj) throws IOException {
        StringWriter writer = new StringWriter();
        try (JsonGenerator jsonGenerator = new JsonFactory().createJsonGenerator(writer)) {
            jsonGenerator.setCodec(new ObjectMapper());
            jsonGenerator.writeObject(obj);
        }
        return writer.toString();
    }

    /**
     * Returns the value of the top level property in the specified JSON without
     * "'s removed. Use this to return JSON string values.
     * 
     * @param fieldName
     *            name of the value to retrieve
     * @param json
     *            JSON to parse
     * @param retainQuotes
     *            if true, "'s are left in the parse value; otherwise they are
     *            removed
     * @return string value
     * @throws IOException
     */
    public static String getTopLevelValueAsString(String fieldName, String json, boolean retainQuotes)
            throws IOException {
        JsonNode root = (new ObjectMapper()).readTree(json).path(fieldName);
        if (root != null) {
            String value = root.toString();
            if (!retainQuotes && (value != null)) {
                value = value.replaceAll("\"", "");
            }
            return value;
        }
        return null;
    }

    /**
     *
     *
     * @param input
     * @return String
     */
    public static String escapeJson(String input) throws ParseException {
        JSONParser parser = new JSONParser();
        JSONObject json = (JSONObject) parser.parse(input);

        // Special handling of ADV_OPTIONS
        String key = "ADV_OPTIONS";
        if (json.containsKey(key)) {
            String advopts = json.get(key).toString();
            String cleanup = advopts
                    .replaceAll("-D \"", "'")
                    .replaceAll("=", "': '")
                    .replaceAll("\"", "',")
                    .replaceAll("'", "\"");

            // wrap with curly braces and remove trailing comma
            cleanup = "{" + cleanup.substring(0, cleanup.length() - 1) + "}";

            JSONObject obj = (JSONObject) parser.parse(cleanup);
            json.put(key, obj);
        }

        return JSONObject.escape(json.toString());
    }

    public static Map<String, String> paramsToMap(JSONObject command) {
        JSONArray paramsList = (JSONArray) command.get("params");

        Map<String, String> paramsMap = new HashMap<>();
        for (Object aParamsList : paramsList) {
            JSONObject o = (JSONObject) aParamsList;
            for (Object o1 : o.entrySet()) {
                Map.Entry<Object, Object> mEntry = (Map.Entry<Object, Object>) o1;
                String key = (String) mEntry.getKey();
                String val = (String) mEntry.getValue();
                paramsMap.put(key, val);
            }
        }

        return paramsMap;
    }
}