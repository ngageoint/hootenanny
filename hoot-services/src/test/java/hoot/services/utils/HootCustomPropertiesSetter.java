package hoot.services.utils;

import java.lang.reflect.Field;
import java.util.Properties;

import hoot.services.HootProperties;


/**
 * Created by dmylov on 5/10/16.
 */
public class HootCustomPropertiesSetter {

    public static void setProperties(Properties newProperties) throws Exception {
        Properties currentProperties = getHootProperties();
        currentProperties.clear();
        currentProperties.putAll(newProperties);
    }

    public static void setProperty(String key, String value) throws Exception {
        Properties currentProperties = getHootProperties();
        currentProperties.setProperty(key, value);
    }

    private static Properties getHootProperties() throws Exception {
        Field propertiesField = HootProperties.class.getDeclaredField("properties");
        propertiesField.setAccessible(true);

        Properties currentProperties = (Properties) propertiesField.get(null);
        return currentProperties;
    }
}
