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
package hoot.services.utils;

import java.io.IOException;
import java.io.StringWriter;
import java.util.HashMap;
import java.util.Map;

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
     */
    public static String objectToJson(Object obj) {
        StringWriter writer = new StringWriter();
        try {
            try (JsonGenerator jsonGenerator = new JsonFactory().createGenerator(writer)) {
                jsonGenerator.setCodec(new ObjectMapper());
                jsonGenerator.writeObject(obj);
            }
        }
        catch (IOException ioe) {
            throw new RuntimeException("Error converting object to JSON!", ioe);
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
     */
    public static String getTopLevelValueAsString(String fieldName, String json, boolean retainQuotes) {
        JsonNode root;
        try {
            root = (new ObjectMapper()).readTree(json).path(fieldName);
        }
        catch (IOException ioe) {
            throw new RuntimeException("Error reading JSON : " + json, ioe);
        }

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
    public static String escapeJson(String input) {
        JSONParser parser = new JSONParser();
        JSONObject json;
        try {
            json = (JSONObject) parser.parse(input);
        }
        catch (ParseException pe) {
            throw new RuntimeException("Error parsing JSON : " + input, pe);
        }

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

            JSONObject obj = null;
            try {
                obj = (JSONObject) parser.parse(cleanup);
            }
            catch (ParseException pe) {
                throw new RuntimeException("Error parsing JSON: " + cleanup, pe);
            }
            json.put(key, obj);
        }

        return JSONObject.escape(json.toString());
    }

    public static Map<String, String> jsonToMap(String json) {
        JSONParser parser = new JSONParser();
        JSONObject command;
        try {
            command = (JSONObject) parser.parse(json);
        }
        catch (ParseException pe) {
            throw new RuntimeException("Error parsing JSON: " + json, pe);
        }

        Map<String, String> paramsMap = new HashMap<>();
        for (Object o : command.entrySet()) {
            Map.Entry<Object, Object> mEntry = (Map.Entry<Object, Object>) o;
            String key = (String) mEntry.getKey();
            String val = (String) mEntry.getValue();
            paramsMap.put(key, val);
        }

        return paramsMap;
    }

    public static String getParameterValue(String key, JSONObject jsonObject) {
        return (jsonObject.get(key) != null) ? jsonObject.get(key).toString() : null;
    }
}