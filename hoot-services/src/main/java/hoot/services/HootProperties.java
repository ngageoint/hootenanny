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
package hoot.services;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.Collections;
import java.util.Enumeration;
import java.util.Map;
import java.util.Properties;
import java.util.TreeMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


/**
 * Services configuration file
 */
public final class HootProperties {
    private static final Logger logger = LoggerFactory.getLogger(HootProperties.class);

    private static final Properties properties;

    public static final String HOME_FOLDER;
    public static final String ASCIIDOC_PATH;
    public static final String TEMPLATE_PATH;
    public static final String REF_OVERRIDE_PATH;
    public static final String HORZ_OVERRIDE_PATH;
    public static final String AVE_OVERRIDE_PATH;
    public static final String DOC_NAME;
    public static final String RPT_STORE_PATH;
    public static final String TILE_SERVER_PATH;
    public static final String INGEST_STAGING_PATH;
    public static final String JS_HEADER_SCRIPT_PATH;
    public static final String SCRIPT_FOLDER;
    public static final String DEFAULT_TRANSLATIONS_CONFIG;
    public static final String DEFAULT_FOUO_TRANSLATIONS_CONFIG;
    public static final String CORE_SCRIPT_PATH;
    public static final String ERROR_LOG_PATH;
    public static final String TEMP_OUTPUT_PATH;
    public static final String WFS_STORE_CONN_NAME;
    public static final String WFS_STORE_DB;
    public static final String ELEMENT_MERGE_SERVER_PORT;
    public static final String ELEMENT_MERGE_SERVER_THREAD_COUNT;
    public static final String ELEMENT_MERGE_SERVER_SCRIPT;
    public static final String TRANSLATION_SERVER_PORT;
    public static final String TRANSLATION_SERVER_THREAD_COUNT;
    public static final String TRANSLATION_SERVER_SCRIPT;
    public static final String TRANSLATION_EXT_PATH;
    public static final String DB_NAME;
    public static final String DB_USER_ID;
    public static final String DB_PASSWORD;
    public static final String DB_HOST;
    public static final String DB_PORT;
    public static final String HGIS_FILTER_SCRIPT;
    public static final String CONFLATE_SIZE_THRESHOLD;
    public static final String INGEST_SIZE_THRESHOLD;
    public static final String EXPORT_SIZE_THRESHOLD;
    public static final String ETL_MAKEFILE;
    public static final String OSM_API_DB_NAME;
    public static final String EXPORT_SCRIPT;
    public static final Boolean OSM_API_DB_ENABLED;
    public static final String MAP_QUERY_DIMENSIONS;
    public static final String MAP_QUERY_AREA_DEGREES;
    public static final String MAX_QUERY_NODES;
    public static final String HGIS_PREPARE_FOR_VALIDATION_SCRIPT;
    public static final String CLIP_DATASET_MAKEFILE_PATH;
    public static final String GET_OGR_ATTRIBUTE_SCRIPT;
    public static final String RASTER_TO_TILES;
    public static final String BASEMAP_RASTER_EXTENSIONS;
    public static final String BASEMAP_RASTER_TO_TILES;
    public static final String CONFLATE_MAKEFILE_PATH;
    public static final String COPYRIGHT;
    public static final String ATTRIBUTION;
    public static final String LICENSE;
    public static final String OSM_VERSION;
    public static final String GENERATOR;
    public static final String MAX_QUERY_AREA_DEGREES;
    public static final String MAXIMUM_WAY_NODES;
    public static final String MAXIMUM_CHANGESET_ELEMENTS;
    public static final String CHANGESET_IDLE_TIMEOUT_MINUTES;
    public static final String CHANGESET_MAX_OPEN_TIME_HOURS;
    public static final String CHANGESET_BOUNDS_EXPANSION_FACTOR_DEEGREES;
    public static final String MAX_RECORD_BATCH_SIZE;
    public static final String TEST_CHANGESET_AUTO_CLOSE;
    public static final String RANDOM_QUERY_SEED;
    public static final String SEED_RANDOM_QUERIES;
    public static final String INTERNAL_JOB_THREAD_SIZE;
    public static final String TEST_JOB_STATUS_POLLER_TIMEOUT;
    public static final String TRANSLATION_SCRIPT_PATH;
    public static final String UPLOAD_FOLDER;
    public static final String DERIVE_CHANGESET_SCRIPT;
    public static final String HOOT_APIDB_URL;
    public static final String OSM_APIDB_URL;

    private static final String OSM_API_DB_USER_ID;
    private static final String OSM_API_DB_PASSWORD;
    private static final String OSM_API_DB_HOST;
    private static final String OSM_API_DB_PORT;

    static {
        try {
            Properties appProperties = new Properties();
            try (InputStream propsStrm = HootProperties.class.getClassLoader()
                    .getResourceAsStream("conf/hoot-services.conf")) {
                appProperties.load(propsStrm);
            }

            // This block of code checks for the local.conf and if there is one
            // then it overrides the global properties.
            try (InputStream in = HootProperties.class.getClassLoader().getResourceAsStream("conf/local.conf")) {
                if (in != null) {
                    Properties localProperties = new Properties();
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

        HOME_FOLDER = getProperty("homeFolder");
        ASCIIDOC_PATH = getProperty("configAsciidocPath");
        TEMPLATE_PATH = getProperty("advOptTemplate");
        REF_OVERRIDE_PATH = getProperty("advOptRefOverride");
        HORZ_OVERRIDE_PATH = getProperty("advOptHorizontalOverride");
        AVE_OVERRIDE_PATH = getProperty("advOptAverageOverride");
        DOC_NAME = getProperty("documentName");
        RPT_STORE_PATH = getProperty("reportDataPath");
        TILE_SERVER_PATH = getProperty("tileServerPath");
        INGEST_STAGING_PATH = getProperty("ingestStagingPath");
        JS_HEADER_SCRIPT_PATH = getProperty("dummyjsHeaderScriptPath");
        SCRIPT_FOLDER = getProperty("customScriptPath");
        DEFAULT_TRANSLATIONS_CONFIG = getProperty("defaultTranslationsConfig");
        DEFAULT_FOUO_TRANSLATIONS_CONFIG = getProperty("defaultFOUOTranslationsConfig");
        CORE_SCRIPT_PATH = getProperty("coreScriptPath");
        ERROR_LOG_PATH = getProperty("ErrorLogPath");
        TEMP_OUTPUT_PATH = getProperty("tempOutputPath");
        WFS_STORE_CONN_NAME = getProperty("wfsStoreConnName");
        WFS_STORE_DB = getProperty("wfsStoreDb");
        ELEMENT_MERGE_SERVER_PORT = getProperty("ElementMergeServerPort");
        ELEMENT_MERGE_SERVER_THREAD_COUNT = getProperty("ElementMergeServerThreadCount");
        ELEMENT_MERGE_SERVER_SCRIPT = getProperty("ElementMergeServerScript");
        TRANSLATION_SERVER_PORT = getProperty("translationServerPort");
        TRANSLATION_SERVER_THREAD_COUNT = getProperty("translationServerThreadCount");
        TRANSLATION_SERVER_SCRIPT = getProperty("translationServerScript");
        TRANSLATION_EXT_PATH = getProperty("translationExtPath");
        HGIS_FILTER_SCRIPT = getProperty("hgisFilterScript");
        CONFLATE_SIZE_THRESHOLD = getProperty("conflateSizeThreshold");
        INGEST_SIZE_THRESHOLD = getProperty("ingestSizeThreshold");
        EXPORT_SIZE_THRESHOLD = getProperty("exportSizeThreshold");
        ETL_MAKEFILE = getProperty("ETLMakefile");
        EXPORT_SCRIPT = getProperty("ExportScript");
        OSM_API_DB_ENABLED = Boolean.parseBoolean(getProperty("osmApiDbEnabled"));
        MAP_QUERY_DIMENSIONS = getProperty("mapQueryDimensions");
        MAP_QUERY_AREA_DEGREES = getProperty("maxQueryAreaDegrees");
        MAX_QUERY_NODES = getProperty("maxQueryNodes");
        HGIS_PREPARE_FOR_VALIDATION_SCRIPT = getProperty("hgisPrepareForValidationScript");
        CLIP_DATASET_MAKEFILE_PATH = getProperty("ClipDatasetMakefilePath");
        GET_OGR_ATTRIBUTE_SCRIPT = getProperty("GetOgrAttributeScript");
        RASTER_TO_TILES = getProperty("RasterToTiles");
        BASEMAP_RASTER_EXTENSIONS = getProperty("BasemapRasterExtensions");
        BASEMAP_RASTER_TO_TILES = getProperty("BasemapRasterToTiles");
        CONFLATE_MAKEFILE_PATH = getProperty("ConflateMakefilePath");
        COPYRIGHT = getProperty("copyright");
        ATTRIBUTION = getProperty("attribution");
        LICENSE = getProperty("license");
        OSM_VERSION = getProperty("osmVersion");
        GENERATOR = getProperty("generator");
        MAX_QUERY_AREA_DEGREES = getProperty("maxQueryAreaDegrees");
        MAXIMUM_WAY_NODES = getProperty("maximumWayNodes");
        MAXIMUM_CHANGESET_ELEMENTS = getProperty("maximumChangesetElements");
        CHANGESET_IDLE_TIMEOUT_MINUTES = getProperty("changesetIdleTimeoutMinutes");
        CHANGESET_MAX_OPEN_TIME_HOURS = getProperty("changesetMaxOpenTimeHours");
        CHANGESET_BOUNDS_EXPANSION_FACTOR_DEEGREES = getProperty("changesetBoundsExpansionFactorDeegrees");
        MAX_RECORD_BATCH_SIZE = getProperty("maxRecordBatchSize");
        TEST_CHANGESET_AUTO_CLOSE = getProperty("testChangesetAutoClose");
        RANDOM_QUERY_SEED = getProperty("randomQuerySeed");
        SEED_RANDOM_QUERIES = getProperty("seedRandomQueries");
        INTERNAL_JOB_THREAD_SIZE = getProperty("internalJobThreadSize");
        TEST_JOB_STATUS_POLLER_TIMEOUT = getProperty("testJobStatusPollerTimeout");
        TRANSLATION_SCRIPT_PATH = getProperty("translationScriptPath");
        UPLOAD_FOLDER = HOME_FOLDER + File.separator + "upload";
        DERIVE_CHANGESET_SCRIPT = getProperty("deriveChangesetScript");

        // Adding another layer of indirection for "sensitive" properties.
        // They should be resolved just before being used to minimize any unintended exposure (f.e. logging).
        DB_NAME = "${dbName}";
        DB_USER_ID = "${dbUserId}";
        DB_PASSWORD = "${dbPassword}";
        DB_HOST = "${dbHost}";
        DB_PORT = "${dbPort}";
        OSM_API_DB_NAME = "${osmApiDbName}";
        OSM_API_DB_USER_ID = "${osmApiDbUserId}";
        OSM_API_DB_PASSWORD = "${osmApiDbPassword}";
        OSM_API_DB_HOST = "${osmApiDbHost}";
        OSM_API_DB_PORT = "${osmApiDbPort}";
        HOOT_APIDB_URL = "hootapidb://" + DB_USER_ID + ":" + DB_PASSWORD + "@" + DB_HOST + ":" + DB_PORT + "/" + DB_NAME;
        OSM_APIDB_URL = "osmapidb://" + OSM_API_DB_USER_ID + ":" + OSM_API_DB_PASSWORD + "@" + OSM_API_DB_HOST + ":" + OSM_API_DB_PORT + "/" + OSM_API_DB_NAME;
    }

    private HootProperties() {}

    /**
     * Helper function to add property reference. It looks for property from the
     * local properties and if it does not find it then uses environmental
     * variable.
     *
     * @param key
     */
    private static String getProperty(String key) {
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
                        sToken = getProperty(token);
                    }

                    if (sToken.isEmpty()) {
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

    public static String replaceSensitiveData(String text) {
        Pattern pattern = Pattern.compile("\\$\\{(.*?)\\}"); // matches ${} pattern
        Matcher matcher = pattern.matcher(text);

        StringBuilder result = new StringBuilder();
        int i = 0;
        while (matcher.find()) {
            String token = matcher.group(1);
            String replacement = getProperty(token);
            if (StringUtils.isBlank(replacement)) {
                throw new RuntimeException("Could not resolve " + token + " token to a valid value!");
            }
            result.append(text.substring(i, matcher.start()));
            result.append(replacement);
            i = matcher.end();
        }

        result.append(text.substring(i, text.length()));

        return result.toString();
    }

    private static Map<String, String> getProperties() {
        Map<String, String> props = new TreeMap<>();

        for (Map.Entry<Object, Object> property : properties.entrySet()) {
            props.put((String) property.getKey(), (String) property.getValue());
        }

        return Collections.unmodifiableMap(props);
    }

    static void init() {
        //logger.debug("Hoot Properties - {}", getProperties());
    }
}
