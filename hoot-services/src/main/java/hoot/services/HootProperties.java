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
package hoot.services;

import java.io.IOException;
import java.io.InputStream;
import java.util.Collections;
import java.util.Enumeration;
import java.util.Map;
import java.util.Properties;
import java.util.TreeMap;


/**
 * Services configuration file
 */
public final class HootProperties {
    private static final Properties properties;

    static {
        try {
            Properties appProperties = new Properties();
            try (InputStream propsStrm = HootProperties.class.getClassLoader()
                    .getResourceAsStream("conf/hoot-services.conf")) {
                appProperties.load(propsStrm);
            }

            // This block of code checks for the local.conf and if there is one
            // then it overrides the global properties.
            Properties localProperties = new Properties();
            try (InputStream in = HootProperties.class.getClassLoader().getResourceAsStream("conf/local.conf")) {
                if (in != null) {
                    localProperties.load(in);
                    Enumeration<?> enumeration = localProperties.propertyNames();
                    while (enumeration.hasMoreElements()) {
                        String key = (String) enumeration.nextElement();
                        String value = localProperties.getProperty(key);
                        appProperties.setProperty(key, value);
                    }
                }
            }

            properties = appProperties;
        }
        catch (IOException ioe) {
            throw new RuntimeException("Error loading Hootenanny's configuation!", ioe);
        }
    }

    private HootProperties() {
    }

    /**
     * Helper function to add property reference. It looks for property from the
     * local properties and if it does not find it then uses environmental
     * variable.
     *
     * @param key
     */
    public static String getProperty(String key) {
        StringBuilder sFullProp = new StringBuilder();
        String sProp = properties.getProperty(key, "");
        String[] parts = sProp.split("\\$");
        if (parts.length > 1) {
            for (String part : parts) {
                if (part.startsWith("(")) {
                    int iClosure = part.indexOf(')');
                    String token = part.substring(1, iClosure);

                    // check to see if the local key exists
                    String sToken = properties.getProperty(token, "");

                    // nested call of property so resolve it.
                    if (sToken.contains("$")) {
                        sToken = HootProperties.getProperty(token);
                    }

                    if ((sToken == null) || (sToken.isEmpty())) {
                        // if not get it from env
                        Map<String, String> env = System.getenv();
                        sToken = env.get(token);
                    }

                    if ((sToken != null) && (!sToken.isEmpty())) {
                        sFullProp.append(sToken);
                    }

                    if ((iClosure + 1) < part.length()) {
                        sFullProp.append(part.substring(iClosure + 1));
                    }
                }
                else {
                    sFullProp.append(part);
                }
            }
        }
        else {
            sFullProp.append(sProp);
        }

        return sFullProp.toString();
    }

    public static String getPropertyOrDefault(String key) {
        String value = getProperty(key);
        if ((value == null) || value.isEmpty()) {
            value = getDefault(key);
        }
        return value;
    }

    /**
     * Determines reasonable default properties in case the server config file
     * is missing
     *
     * @param key
     *            property key
     * @return a property value
     */
    public static String getDefault(String key) {
        switch (key) {
            case "osmVersion":
                return "0.6";
            case "generator":
                return "Hootenanny server";
            case "copyright":
                return "?";
            case "attribution":
                return "?";
            case "license":
                return "?";
            case "maxQueryAreaDegrees":
                return "0.25";
            case "maxQueryNodes":
                return "50000";
            case "mapQueryDimensions":
                return "2";
            case "maximumChangesetElements":
                return "50000";
            case "maximumWayNodes":
                return "2000";
            case "changesetBoundsExpansionFactorDeegrees":
                return "0.1";
            case "changesetIdleTimeoutMinutes":
                return "60";
            case "changesetMaxOpenTimeHours":
                return "24";
            case "debugSql":
                return "false";
            case "testChangesetAutoClose":
                return "false";
            case "coreScriptPath":
                return "/project/hoot/scripts";
            case "coreScriptOutputPath":
                return "";
            case "coreJobServerUrl":
                return "http://localhost:8080";
            case "ETLMakefile":
                return "makeetl";
            case "grizzlyPort":
                return "9998";
            case "maxRecordBatchSize":
                return "5000";
            case "testJobStatusPollerTimeout":
                return "250";
            case "servicesTestClearEntireDb":
                return "false";
            case "logPropsDynamicChangeScanInterval":
                return "1";
            case "autoScanForLogPropsChanges":
                return "true";
            case "maxWarningsDisplayed":
                return "10";
            case "seedRandomQueries":
                return "false";
            case "randomQuerySeed":
                return "0.1";
            default:
                return null;
        }
    }

    public static Map<String, String> getProperties() {
        Map<String, String> props = new TreeMap<>();

        for (Map.Entry<Object, Object> property : properties.entrySet()) {
            props.put((String) property.getKey(), (String) property.getValue());
        }

        return Collections.unmodifiableMap(props);
    }
}
