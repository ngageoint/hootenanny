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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.utils;

import java.io.IOException;
import java.io.StringWriter;
import java.util.Map;

import org.codehaus.jackson.JsonFactory;
import org.codehaus.jackson.JsonGenerator;
import org.codehaus.jackson.JsonNode;
import org.codehaus.jackson.JsonParseException;
import org.codehaus.jackson.map.JsonMappingException;
import org.codehaus.jackson.map.ObjectMapper;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Various JSON utilities
 */
public class JsonUtils
{
  @SuppressWarnings("unused")
  private static final Logger log = LoggerFactory.getLogger(JsonUtils.class);
  
  public enum DelimitedStringType { FIELDS, VALUES, BOTH }
  
  /**
   * Converts a POJO to JSON
   * 
   * @param obj POJO
   * @return JSON string
   * @throws IOException
   */
  public static String objectToJson(final Object obj) throws IOException
  {
    StringWriter writer = new StringWriter();
    JsonGenerator jsonGenerator = new JsonFactory().createJsonGenerator(writer);
    jsonGenerator.setCodec(new ObjectMapper());
    jsonGenerator.writeObject(obj);
    return writer.toString();
  }
  
  /**
   * Returns the value of top level property in the specified JSON
   * 
   * @param fieldName name of the value to retrieve
   * @param json JSON to parse
   * @return POJO
   * @throws JsonParseException
   * @throws JsonMappingException
   * @throws IOException
   */
  public static Object getTopLevelValue(final String fieldName, final String json) 
    throws JsonParseException, JsonMappingException, IOException
  {
    return (new ObjectMapper()).readValue(json, Map.class).get(fieldName);
  }
  
  /**
   * Returns the value of the top level property in the specified JSON with all "'s removed.  Use
   * this to return non-JSON string values.
   * 
   * @param fieldName name of the value to retrieve
   * @param json JSON to parse
   * @return string value
   * @throws JsonParseException
   * @throws JsonMappingException
   * @throws IOException
   */
  public static String getTopLevelValueAsString(final String fieldName, final String json) 
    throws JsonParseException, JsonMappingException, IOException
  {
    return getTopLevelValueAsString(fieldName, json, false);
  }
  
  /**
   * Returns the value of the top level property in the specified JSON without "'s removed.  Use
   * this to return JSON string values.
   * 
   * @param fieldName name of the value to retrieve
   * @param json JSON to parse
   * @param retainQuotes if true, "'s are left in the parse value; otherwise they are removed
   * @return string value
   * @throws JsonParseException
   * @throws JsonMappingException
   * @throws IOException
   */
  public static String getTopLevelValueAsString(final String fieldName, final String json,
    final boolean retainQuotes) throws JsonParseException, JsonMappingException, IOException
  {
    JsonNode root = (new ObjectMapper()).readTree(json).path(fieldName);
    if (root != null)
    {
      String value = root.toString();
      if (!retainQuotes && value != null)
      {
        value = value.replaceAll("\"", "");
      }
      return value;
    }
    return null;
  }
  
  /**
   * Sets a top level property on a JSON string.  This can be used when ObjectMapper.readValue 
   * isn't an option because the object being modified isn't a bean.
   * 
   * @param json JSON string to modify
   * @param fieldName name of the field to modify
   * @param fieldValue value to modify the field with
   * @return the modified JSON
   * @throws Exception
   */
  public static String setTopLevelValueAsString(final String json, final String fieldName, 
    final String fieldValue) throws Exception
  {
    ObjectMapper jsonObjMapper = new ObjectMapper();
    @SuppressWarnings("unchecked")
    Map<String, String> props = jsonObjMapper.readValue(json, Map.class);
    if (!props.containsKey(fieldName))
    {
      throw new Exception("Field " + fieldName + " not contained in JSON: " + json);
    }
    props.put(fieldName, fieldValue);
    StringWriter writer = new StringWriter();
    jsonObjMapper.writeValue(writer, props);
    return writer.toString();
  }
}