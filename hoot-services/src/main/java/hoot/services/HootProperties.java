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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;
import java.util.Collections;
import java.util.Enumeration;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.commons.lang3.StringUtils;


public final class HootProperties {
    private static final Properties properties;
    public static final String HOME_FOLDER;
    public static final String ASCIIDOC_PATH;
    public static final String TEMPLATE_PATH;
    public static final String HOOT2_OVERRIDE_PATH;
    public static final String REF_OVERRIDE_PATH;
    public static final String HORZ_OVERRIDE_PATH;
    public static final String ATT_OVERRIDE_PATH;
    public static final String CONFLATION_TYPES_PATH;
    public static final String ATTRIBUTE_CONFLATION_PATH;
    public static final String DIFFERENTIAL_CONFLATION_PATH;
    public static final String HORIZONTAL_CONFLATION_PATH;
    public static final String NETWORK_CONFLATION_PATH;
    public static final String IMPORT_OPTIONS;
    public static final String CHANGESET_OPTIONS;
    public static final String CHANGESET_OPTION_KEYS;
    public static final String CONFIG_OPTIONS;
    public static final String DOC_NAME;
    public static final String RPT_STORE_PATH;
    public static final String BASEMAPS_TILES_FOLDER;
    public static final String BASEMAPS_FOLDER;
    public static final String JS_HEADER_SCRIPT_PATH;
    public static final String CHANGESETS_FOLDER;
    public static final String SCRIPT_FOLDER;
    public static final String DEFAULT_TRANSLATIONS_CONFIG;
    public static final String DEFAULT_FOUO_TRANSLATIONS_CONFIG;
    public static final String CORE_SCRIPT_PATH;
    public static final String ERROR_LOG_PATH;
    public static final String TEMP_OUTPUT_PATH;
    public static final String ELEMENT_MERGE_SERVER_PORT;
    public static final String ELEMENT_MERGE_SERVER_THREAD_COUNT;
    public static final String ELEMENT_MERGE_SERVER_SCRIPT;
    public static final String TRANSLATION_SERVER_PORT;
    public static final String TRANSLATION_SERVER_THREAD_COUNT;
    public static final String TRANSLATION_SERVER_SCRIPT;
    public static final String TRANSLATION_EXT_PATH;
    public static final String HOOTAPI_DB_NAME;
    public static final String HOOTAPI_DB_USER;
    public static final String HOOTAPI_DB_PWORD;
    public static final String HOOTAPI_DB_HOST;
    public static final String HOOTAPI_DB_PORT;
    public static final String HGIS_FILTER_SCRIPT;
    public static final String CONFLATE_SIZE_THRESHOLD;
    public static final String INGEST_SIZE_THRESHOLD;
    public static final String EXPORT_SIZE_THRESHOLD;
    public static final String HTTP_TIMEOUT;
    public static final String GRAIL_OVERPASS_QUERY;
    public static final String GRAIL_OVERPASS_STATS_QUERY;
    public static final String OSMAPI_DB_NAME;
    public static final String MAP_QUERY_DIMENSIONS;
    public static final String MAP_QUERY_AREA_DEGREES;
    public static final String MAX_QUERY_NODES;
    public static final String HGIS_PREPARE_FOR_VALIDATION_SCRIPT;
    public static final List<String> BASEMAP_RASTER_EXTENSIONS;
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
    public static final String TRANSLATION_SCRIPT_PATH;
    public static final String UPLOAD_FOLDER;
    public static final String HOOTAPI_DB_URL;
    public static final String OSMAPI_DB_URL;
    public static final String CHANGESET_DERIVE_BUFFER;
    public static final Boolean CHANGESET_APPLY_DEBUG;
    public static final Boolean CHANGESET_APPLY_SINGLETHREADED;
    public static final String HOOT_LANGUAGE_TRANSLATOR_APP;
    public static final String JOSHUA_CONNECTION_POOL_MAX_SIZE;
    public static final String JOSHUA_CONNECTION_TIMEOUT;
    public static final String JOSHUA_CONNECTION_MAX_WAIT;
    public static final String JOSHUA_LIBRARY;
    public static final String JOSHUA_MAX_MEMORY;
    public static final String OPEN_NLP_LANGUAGE_DETECTION_MODEL;

    private static final String USERFILES_FOLDER;
    private static final String OSMAPI_DB_USER;
    private static final String OSMAPI_DB_PWORD;
    private static final String OSMAPI_DB_HOST;
    private static final String OSMAPI_DB_PORT;

    public static final String PUBLIC_OSMAPI_PULL_URL;
    public static final String PUBLIC_OSMAPI_CAPABILITIES_URL;

    public static final String PUBLIC_OVERPASS_URL;
    public static final String PRIVATE_OVERPASS_URL;
    public static final String PRIVATE_OVERPASS_CERT_PATH;
    public static final String PRIVATE_OVERPASS_CERT_PHRASE;
    public static final String MAX_OVERPASS_FEATURE_COUNT;

    public static final String GRAIL_RAILS_LABEL;
    public static final String GRAIL_OVERPASS_LABEL;

    public static final String RAILSPORT_PUSH_URL;
    public static final String RAILSPORT_PULL_URL;
    public static final String RAILSPORT_CAPABILITIES_URL;

    private static final String PUBLIC_OSMAPI_URL;
    private static final String PUBLIC_OSMAPI_VERSION;

    private static final String RAILSPORT_PROTOCOL;
    private static final String RAILSPORT_HOST;
    private static final String RAILSPORT_PORT;
    private static final String RAILSPORT_API;

    public static final String OAUTH_REDIRECTURL;
    public static final String OAUTH_PROVIDERURL;
    public static final String OAUTH_PATHS_USER;

    static {
        try {
            Properties appProperties = new Properties();
            try (InputStream propsStrm = HootProperties.class.getClassLoader()
                    .getResourceAsStream("conf/hoot-services.conf")) {
                appProperties.load(propsStrm);
            }

            // Load DB properties
            try (InputStream in = HootProperties.class.getClassLoader().getResourceAsStream("db/db.properties")) {
                Properties dbProperties = new Properties();
                dbProperties.load(in);
                Enumeration<?> enumeration = dbProperties.propertyNames();
                while (enumeration.hasMoreElements()) {
                    String key = (String) enumeration.nextElement();
                    String value = dbProperties.getProperty(key);
                    appProperties.setProperty(key, value);
                }
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
        HOOT2_OVERRIDE_PATH = getProperty("advOptHoot2Override");
        REF_OVERRIDE_PATH = getProperty("advOptRefOverride");
        HORZ_OVERRIDE_PATH = getProperty("advOptHorizontalOverride");
        ATT_OVERRIDE_PATH = getProperty("advOptAttributeOverride");
        CONFLATION_TYPES_PATH = getProperty("conflationTypes");
        ATTRIBUTE_CONFLATION_PATH = getProperty("attributeConf");
        DIFFERENTIAL_CONFLATION_PATH = getProperty("differentialConf");
        HORIZONTAL_CONFLATION_PATH = getProperty("horizontalConf");
        NETWORK_CONFLATION_PATH = getProperty("networkConf");
        IMPORT_OPTIONS = getProperty("importOptions");
        CHANGESET_OPTION_KEYS = getProperty("changesetOptionKeys");
        CHANGESET_OPTIONS = getProperty("changesetOptions");
        CONFIG_OPTIONS = getProperty("configJsonPath");
        DOC_NAME = getProperty("documentName");
        JS_HEADER_SCRIPT_PATH = getProperty("dummyjsHeaderScriptPath");
        DEFAULT_TRANSLATIONS_CONFIG = getProperty("defaultTranslationsConfig");
        DEFAULT_FOUO_TRANSLATIONS_CONFIG = getProperty("defaultFOUOTranslationsConfig");
        CORE_SCRIPT_PATH = getProperty("coreScriptPath");
        ERROR_LOG_PATH = getProperty("ErrorLogPath");
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
        HTTP_TIMEOUT = getProperty("httpTimeout");
        MAX_OVERPASS_FEATURE_COUNT = getProperty("grailMaxFeatureCount");
        GRAIL_RAILS_LABEL =  getProperty("grailRailsLabel");
        GRAIL_OVERPASS_LABEL = getProperty("grailOverpassLabel");
        GRAIL_OVERPASS_QUERY = getProperty("grailOverpassQueryPath");
        GRAIL_OVERPASS_STATS_QUERY = getProperty("grailOverpassStatsQueryPath");
        MAP_QUERY_DIMENSIONS = getProperty("mapQueryDimensions");
        MAP_QUERY_AREA_DEGREES = getProperty("maxQueryAreaDegrees");
        MAX_QUERY_NODES = getProperty("maxQueryNodes");
        HGIS_PREPARE_FOR_VALIDATION_SCRIPT = getProperty("hgisPrepareForValidationScript");
        BASEMAP_RASTER_EXTENSIONS = Collections.unmodifiableList(Arrays.asList(getProperty("BasemapRasterExtensions").toLowerCase().split(",")));
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
        TRANSLATION_SCRIPT_PATH = getProperty("translationScriptPath");
        CHANGESET_DERIVE_BUFFER = getProperty("changesetDeriveBufferDegrees");
        CHANGESET_APPLY_DEBUG = Boolean.valueOf(getProperty("changesetApplyDebug"));
        CHANGESET_APPLY_SINGLETHREADED = Boolean.valueOf(getProperty("changesetApplySingleThreaded"));
        HOOT_LANGUAGE_TRANSLATOR_APP = getProperty("hootLanguageTranslatorApp");
        JOSHUA_CONNECTION_POOL_MAX_SIZE = getProperty("joshuaConnectionPoolMaxSize");
        JOSHUA_CONNECTION_TIMEOUT = getProperty("joshuaConnectionTimeout");
        JOSHUA_CONNECTION_MAX_WAIT= getProperty("joshuaConnectionMaxWait");
        JOSHUA_LIBRARY = getProperty("joshuaLibrary");
        JOSHUA_MAX_MEMORY = getProperty("joshuaMaxMemory");
        OPEN_NLP_LANGUAGE_DETECTION_MODEL = getProperty("openNlpLanguageDetectionModel");
        OAUTH_REDIRECTURL = getProperty("oauthRedirectURL");
        OAUTH_PROVIDERURL = getProperty("oauthProviderURL");
        OAUTH_PATHS_USER = getProperty("oauthPathsUser");

        // Root folder of tomcat writable locations
        USERFILES_FOLDER = HOME_FOLDER + File.separator + "userfiles";

        // User generated changesets location
        CHANGESETS_FOLDER = USERFILES_FOLDER + File.separator + "changesets";

        // User uploaded translation scripts go here
        SCRIPT_FOLDER = USERFILES_FOLDER + File.separator + "customscript";

        // Data source files to import and export
        TEMP_OUTPUT_PATH = USERFILES_FOLDER + File.separator + "tmp";

        // Files uploaded via HTTP for ingest
        UPLOAD_FOLDER = TEMP_OUTPUT_PATH + File.separator + "upload";

        // Conflation stats and reports go here
        RPT_STORE_PATH = USERFILES_FOLDER + File.separator + "reports";

        // Contains tiles of existing basemaps
        BASEMAPS_TILES_FOLDER = USERFILES_FOLDER + File.separator + "ingest" + File.separator + "processed" + File.separator + "BASEMAP";

        // Contains .enabled/.disabled control file
        BASEMAPS_FOLDER = USERFILES_FOLDER + File.separator + "ingest" + File.separator + "upload" + File.separator + "BASEMAP";

        // Adding another layer of indirection for "sensitive" properties.
        // They should be resolved just before being used to minimize any unintended exposure (f.e. logging).
        HOOTAPI_DB_NAME = "${HOOTAPI_DB_NAME}";
        HOOTAPI_DB_USER = "${HOOTAPI_DB_USER}";
        HOOTAPI_DB_PWORD = "${HOOTAPI_DB_PASSWORD}";
        HOOTAPI_DB_HOST = "${HOOTAPI_DB_HOST}";
        HOOTAPI_DB_PORT = "${HOOTAPI_DB_PORT}";
        OSMAPI_DB_NAME = "${OSMAPI_DB_NAME}";
        OSMAPI_DB_USER = "${OSMAPI_DB_USER}";
        OSMAPI_DB_PWORD = "${OSMAPI_DB_PASSWORD}";
        OSMAPI_DB_HOST = "${OSMAPI_DB_HOST}";
        OSMAPI_DB_PORT = "${OSMAPI_DB_PORT}";

        HOOTAPI_DB_URL = "hootapidb://" + HOOTAPI_DB_USER + ":" + HOOTAPI_DB_PWORD + "@" + HOOTAPI_DB_HOST + ":" + HOOTAPI_DB_PORT + "/" + HOOTAPI_DB_NAME;
        OSMAPI_DB_URL = "osmapidb://" + OSMAPI_DB_USER + ":" + OSMAPI_DB_PWORD + "@" + OSMAPI_DB_HOST + ":" + OSMAPI_DB_PORT + "/" + OSMAPI_DB_NAME;

        // The base URL for pulling OSM data
        PUBLIC_OSMAPI_URL = "${PUBLIC_OSMAPI_URL}";
        PUBLIC_OSMAPI_VERSION = "${PUBLIC_OSMAPI_VERSION}";
        PUBLIC_OSMAPI_PULL_URL = PUBLIC_OSMAPI_URL + "/api/" + PUBLIC_OSMAPI_VERSION;
        PUBLIC_OSMAPI_CAPABILITIES_URL = PUBLIC_OSMAPI_URL + "/api/capabilities";

        // The URL for the main Overpass server
        PUBLIC_OVERPASS_URL = "${PUBLIC_OVERPASS_URL}";

        // The URL for the private Overpass server
        PRIVATE_OVERPASS_URL = "${PRIVATE_OVERPASS_URL}";
        PRIVATE_OVERPASS_CERT_PATH = "${PRIVATE_OVERPASS_CERT_PATH}";
        PRIVATE_OVERPASS_CERT_PHRASE = "${PRIVATE_OVERPASS_CERT_PHRASE}";

        // The OSM Rails Port that we are going to pull and push data to.
        RAILSPORT_PROTOCOL = "${RAILSPORT_PROTOCOL}" + "://";
        RAILSPORT_HOST = "${RAILSPORT_HOST}";
        RAILSPORT_PORT = "${RAILSPORT_PORT}";
        RAILSPORT_API = "${RAILSPORT_API}";

        // Some sites don't need a port number
        // This _should_ goto https at some stage
        if (replaceSensitiveData(RAILSPORT_PORT).equals("XXX")) {
            RAILSPORT_PUSH_URL = RAILSPORT_PROTOCOL + RAILSPORT_HOST;
            RAILSPORT_PULL_URL = RAILSPORT_PROTOCOL + RAILSPORT_HOST + RAILSPORT_API;
            RAILSPORT_CAPABILITIES_URL = RAILSPORT_PROTOCOL + RAILSPORT_HOST + "/api/capabilities";
        } else {
            RAILSPORT_PUSH_URL = RAILSPORT_PROTOCOL + RAILSPORT_HOST + ":" + RAILSPORT_PORT;
            RAILSPORT_PULL_URL = RAILSPORT_PROTOCOL + RAILSPORT_HOST + ":" + RAILSPORT_PORT + RAILSPORT_API;
            RAILSPORT_CAPABILITIES_URL = RAILSPORT_PROTOCOL + RAILSPORT_HOST + ":" + RAILSPORT_PORT  + "/api/capabilities";
        }

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
            if (!StringUtils.isBlank(replacement)) {
                result.append(text.substring(i, matcher.start()));
                result.append(replacement);
                i = matcher.end();
            }
        }

        result.append(text.substring(i, text.length()));

        return result.toString();
    }
}
