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
package hoot.services.controllers.ingest;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import javax.ws.rs.Consumes;
import javax.ws.rs.DELETE;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;

import org.apache.commons.io.FileUtils;
import org.apache.commons.lang3.StringUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.mozilla.javascript.Context;
import org.mozilla.javascript.EvaluatorException;
import org.mozilla.javascript.Function;
import org.mozilla.javascript.ScriptableObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.HootProperties;
import hoot.services.ingest.ModifyScriptsRequest;
import hoot.services.ingest.Script;
import hoot.services.ingest.ScriptsModifiedResponse;
import hoot.services.utils.CaseInsensitiveStringList;
import hoot.services.utils.ResourceErrorHandler;


@Path("/customscript")
public class CustomScriptResource {
    private static final Logger logger = LoggerFactory.getLogger(CustomScriptResource.class);
    private static final String scriptFolder;
    private static final String homeFolder;
    private static final String jsHeaderScriptPath;
    private static final String defaultTranslationsConfig;
    private static final String defaultFOUOTranslationsConfig;
    private static final boolean fouoTranslationsExist;
    private static final String headerStart = "/*<<<";
    private static final String headerEnd = ">>>*/" + System.lineSeparator();

    static {
        homeFolder = HootProperties.getProperty("homeFolder");
        jsHeaderScriptPath = HootProperties.getProperty("dummyjsHeaderScriptPath");
        scriptFolder = HootProperties.getProperty("customScriptPath");
        defaultTranslationsConfig = HootProperties.getProperty("defaultTranslationsConfig");
        defaultFOUOTranslationsConfig = HootProperties.getProperty("defaultFOUOTranslationsConfig");

        if ((new File(defaultFOUOTranslationsConfig)).exists()) {
            fouoTranslationsExist = true;
            logger.info("FOUO translations are present.");
        }
        else {
            fouoTranslationsExist = false;
            logger.info("FOUO translations are not present.");
        }
    }
    /**
     * Returns the directory the scripts are stored in
     *
     * @return a directory
     */
    private File getUploadDir() {
        return new File(scriptFolder);
    }

    private boolean uploadDirExists() {
        return getUploadDir().exists();
    }

    public CustomScriptResource() {
    }

    /**
     * Create or update user provided script into file.
     * 
     * // A non-standard extension to include additional js files within the
     * same dir // sub-tree. require("example")
     *
     * // an optional initialize function that gets called once before any //
     * translateAttribute calls. function initialize() { // The print method
     * simply prints the string representation to stdout
     * //print("Initializing.") }
     *
     * // an optional finalize function that gets called once after all //
     * translateAttribute calls. function finalize() { // the debug method
     * prints to stdout when --debug has been specified on // the hoot command
     * line. (DEBUG logger level) debug("Finalizing."); }
     *
     * // A translateAttributes method that is very similar to the python
     * translate // attributes function translateAttributes(attrs, layerName) {
     * tags = {}; //print(layerName); for (var key in attrs) { k =
     * key.toLowerCase() //print(k + ": " + attrs[key]); tags[k] = attrs[key] }
     * return tags; }
     * 
     * @param script
     *            Name of script. If exists then it will be updated
     * @param scriptName
     *            Name of script. If exists then it will be updated
     * @param scriptDescription
     *            Script description.
     * @return Script
     */
    @POST
    @Path("/save")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.TEXT_PLAIN)
    public Response processSave(String script,
                                @QueryParam("SCRIPT_NAME") String scriptName,
                                @QueryParam("SCRIPT_DESCRIPTION") String scriptDescription) {
        JSONArray saveArr = new JSONArray();
        try {
            saveArr.add(saveScript(scriptName, scriptDescription, script));
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError(
                    "Error processing script save for: " + scriptName + " Error: " + ex.getMessage(),
                    Status.INTERNAL_SERVER_ERROR, logger);
        }
        return Response.ok(saveArr.toString(), MediaType.TEXT_PLAIN).build();
    }

    @POST
    @Path("/saveMultiple")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public ScriptsModifiedResponse saveScripts(ModifyScriptsRequest saveMultipleScriptsRequest) {
        ScriptsModifiedResponse response = null;
        List<String> scriptsModified = new ArrayList<>();
        try {
            response = new ScriptsModifiedResponse();
            for (Script script : saveMultipleScriptsRequest.getScripts()) {
                if (saveScript(script.getName(), script.getDescription(), script.getContent()) != null) {
                    scriptsModified.add(script.getName());
                }
            }
            response.setScriptsModified(scriptsModified.toArray(new String[scriptsModified.size()]));
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error processing script save.  Error: " + ex.getMessage(),
                    Status.INTERNAL_SERVER_ERROR, logger);
        }
        return response;
    }

    /**
     * Gets the list of available scripts.
     * 
     * GET hoot-services/ingest/customscript/getlist
     * 
     * @return JSON Array containing JSON of name and description of all
     *         available scripts
     */
    @GET
    @Path("/getlist")
    @Produces(MediaType.TEXT_PLAIN)
    public Response getScriptsList() {
        JSONArray retList = new JSONArray();
        Map<String, JSONObject> sortedScripts = new TreeMap<>();
        JSONArray filesList = new JSONArray();

        try {
            File scriptsDir = new File(scriptFolder);
            if (scriptsDir.exists()) {
                String[] exts = { "js" };
                List<File> files = (List<File>) FileUtils.listFiles(scriptsDir, exts, false);

                for (File file : files) {
                    String content = FileUtils.readFileToString(file, "UTF-8");
                    JSONObject oScript = getScriptObject(content);

                    if (oScript != null) {
                        JSONObject header = (JSONObject) oScript.get("HEADER");
                        if (header.get("CANEXPORT") == null) {
                            boolean canExport = validateExport(oScript.get("BODY").toString());
                            header.put("CANEXPORT", canExport);
                        }
                        filesList.add(header);
                    }
                }
            }

            List<String> configFiles = new ArrayList<>();
            configFiles.add(defaultTranslationsConfig);

            if (fouoTranslationsExist) {
                configFiles.add(defaultFOUOTranslationsConfig);
            }

            filesList.addAll(getDefaultList(configFiles));

            // sort the list
            for (Object o : filesList) {
                JSONObject cO = (JSONObject) o;
                String sName = cO.get("NAME").toString();
                sortedScripts.put(sName.toUpperCase(), cO);
            }

            retList.addAll(sortedScripts.values());
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error getting scripts list: " + ex.getMessage(),
                    Status.INTERNAL_SERVER_ERROR, logger);
        }

        return Response.ok(retList.toString(), MediaType.TEXT_PLAIN).build();
    }

    /**
     * getDefaultList reads the DefaultTranslations.json and passes default
     * translations list. The list is used by UI to distinguish between custom
     * and default translations.
     *
     * @return JSONArray of translation objects
     * @throws Exception
     */
    private static JSONArray getDefaultList(List<String> configFiles) throws Exception {
        JSONArray filesList = new JSONArray();
        
        for (String configFile : configFiles) {
            File file = new File(configFile);
            if (file.exists()) {
                try (FileReader reader = new FileReader(configFile)) {
                    JSONParser jsonParser = new JSONParser();
                    JSONArray defTranslations = (JSONArray) jsonParser.parse(reader);
                    for (Object defTranslation : defTranslations) {
                        JSONObject oTrans = (JSONObject) defTranslation;
                        oTrans.put("DEFAULT", true);
                        String desc = oTrans.get("DESCRIPTION").toString();
                        if (desc.isEmpty()) {
                            desc = oTrans.get("NAME").toString();
                        }
                        oTrans.put("DESCRIPTION", desc);

                        Object oCanExport = oTrans.get("CANEXPORT");

                        // If the CANEXPORT is not available then try to determine
                        if (oCanExport == null) {
                            // Get the script
                            if (oTrans.get("PATH") != null) {
                                File fScript = new File(homeFolder + "/" + oTrans.get("PATH"));
                                if (fScript.exists()) {
                                    String sScript = FileUtils.readFileToString(fScript);
                                    boolean canExport = validateExport(sScript);
                                    oTrans.put("CANEXPORT", canExport);
                                }
                            }
                        }
                    }

                    // validate FOUO support
                    if (!defTranslations.isEmpty()) {
                        for (Object oTrans : defTranslations) {
                            JSONObject jsTrans = (JSONObject) oTrans;
                            if (jsTrans.get("FOUO_PATH") != null) {
                                // See if FOUO folder exists
                                File fouoPath = new File(homeFolder + "/" + jsTrans.get("FOUO_PATH"));
                                if (fouoPath.exists()) {
                                    filesList.add(jsTrans);
                                }
                            }
                            else {
                                // If there is no FOUO_PATH then assume it is
                                // not FOUO translation
                                filesList.add(jsTrans);
                            }
                        }
                    }
                }
            }
        }

        return filesList;
    }

    /**
     * Returns the specified script.
     * 
     * GET hoot-services/ingest/customscript/getscript?SCRIPT_NAME=MyTest
     * 
     * // A non-standard extension to include additional js files within the
     * same dir // sub-tree. require("example")
     *
     * // an optional initialize function that gets called once before any //
     * translateAttribute calls. function initialize() { // The print method
     * simply prints the string representation to stdout
     * //print("Initializing.") }
     *
     * // an optional finalize function that gets called once after all //
     * translateAttribute calls. function finalize() { // the debug method
     * prints to stdout when --debug has been specified on // the hoot command
     * line. (DEBUG logger level) debug("Finalizing."); }
     *
     * // A translateAttributes method that is very similar to the python
     * translate // attributes function translateAttributes(attrs, layerName) {
     * tags = {}; //print(layerName); for (var key in attrs) { k =
     * key.toLowerCase() //print(k + ": " + attrs[key]); tags[k] = attrs[key] }
     * return tags; }
     * 
     * @param scriptName
     *            Name of the script to retrieve.
     * @return Requested translation script
     */
    @GET
    @Path("/getscript")
    @Produces(MediaType.TEXT_PLAIN)
    public Response getScript(@QueryParam("SCRIPT_NAME") String scriptName) {
        String script = "";
        try {
            File scriptsDir = new File(scriptFolder);

            if (scriptsDir.exists()) {
                String[] exts = { "js" };
                List<File> files = (List<File>) FileUtils.listFiles(scriptsDir, exts, false);

                for (File file : files) {
                    try {
                        String content = FileUtils.readFileToString(file, "UTF-8");
                        JSONObject oScript = getScriptObject(content);

                        if (oScript != null) {
                            JSONObject header = (JSONObject) oScript.get("HEADER");
                            if (header.get("NAME").toString().equalsIgnoreCase(scriptName)) {
                                script = oScript.get("BODY").toString();
                                break;
                            }
                        }
                    }
                    catch (Exception e) {
                        logger.error("Failed to read file header: {}", e.getMessage());
                    }
                }
            }
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error getting script: " + scriptName + " Error: " + ex.getMessage(),
                    Status.INTERNAL_SERVER_ERROR, logger);
        }

        return Response.ok(script, MediaType.TEXT_PLAIN).build();
    }

    /**
     * Returns the default script.
     * 
     * GET hoot-services/ingest/customscript/getscript?SCRIPT_PATH=customscript/
     * testdefault.js
     * 
     * // A non-standard extension to include additional js files within the
     * same dir // sub-tree. require("example")
     *
     * // an optional initialize function that gets called once before any //
     * translateAttribute calls. function initialize() { // The print method
     * simply prints the string representation to stdout
     * //print("Initializing.") }
     *
     * // an optional finalize function that gets called once after all //
     * translateAttribute calls. function finalize() { // the debug method
     * prints to stdout when --debug has been specified on // the hoot command
     * line. (DEBUG logger level) debug("Finalizing."); }
     *
     * // A translateAttributes method that is very similar to the python
     * translate // attributes function translateAttributes(attrs, layerName) {
     * tags = {}; //print(layerName); for (var key in attrs) { k =
     * key.toLowerCase() //print(k + ": " + attrs[key]); tags[k] = attrs[key] }
     * return tags; }
     * 
     * @param scriptPath
     *            Relative path of default translation script. (Relative to hoot
     *            home path)
     * @return Requested translation script
     */
    @GET
    @Path("/getdefaultscript")
    @Produces(MediaType.TEXT_PLAIN)
    public Response getDefaultScript(@QueryParam("SCRIPT_PATH") String scriptPath) {
        String script = "";
        try {
            // See Bug #6483 Read vulnerability in services script API
            boolean bPathValidated = false;
            List<String> configFiles = new ArrayList<>();

            configFiles.add(defaultTranslationsConfig);
            if (fouoTranslationsExist) {
                configFiles.add(defaultFOUOTranslationsConfig);
            }

            JSONArray defList = getDefaultList(configFiles);

            for (Object aDefList : defList) {
                JSONObject item = (JSONObject) aDefList;

                Object oPath = item.get("PATH");
                if ((oPath != null) && scriptPath.equals(oPath.toString())) {
                    bPathValidated = true;
                    break;
                }

                Object oFouoPath = item.get("FOUO_PATH");
                if ((oFouoPath != null) && scriptPath.equals(oFouoPath.toString())) {
                    bPathValidated = true;
                    break;
                }
            }

            if (bPathValidated) {
                File scriptFile = new File(homeFolder, scriptPath);
                if (scriptFile.exists()) {
                    script = FileUtils.readFileToString(scriptFile);
                }
            }
            else {
                throw new Exception("Invalid script path.");
            }
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error getting script: " + scriptPath + " Error: " + ex.getMessage(),
                    Status.INTERNAL_SERVER_ERROR, logger);
        }
        return Response.ok(script, MediaType.TEXT_PLAIN).build();
    }

    /**
     * Deletes the specified script.
     * 
     * GET hoot-services/ingest/customscript/deletescript?SCRIPT_NAME=My Test6
     * 
     * //TODO: should be an HTTP DELETE
     * 
     * @param scriptName
     *            Name of the script to delete.
     * @return JSON Array containing JSON of name and description of deleted
     *         scripts
     */
    @GET
    @Path("/deletescript")
    @Produces(MediaType.TEXT_PLAIN)
    public Response deleteScript(@QueryParam("SCRIPT_NAME") String scriptName) {
        JSONArray delArr = new JSONArray();
        try {
            List<File> files = getFilesInScriptDir();
            if (files == null) {
                throw new Exception("Script directory does not exist.");
            }

            for (File file : files) {
                try {
                    String content = FileUtils.readFileToString(file, "UTF-8");
                    JSONObject oScript = getScriptObject(content);

                    if (oScript != null) {
                        JSONObject header = (JSONObject) oScript.get("HEADER");
                        if (header.get("NAME").toString().equalsIgnoreCase(scriptName)) {
                            delArr.add(header);
                            if (!file.delete()) {
                                logger.error("Error deleting {} file!", file.getAbsolutePath());
                            }
                            break;
                        }
                    }
                }
                catch (Exception e) {
                    logger.error("Failed to read file header: {}", e.getMessage());
                }
            }
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error deleting script: " + scriptName + " Error: " + ex.getMessage(),
                    Status.INTERNAL_SERVER_ERROR, logger);
        }

        return Response.ok(delArr.toString(), MediaType.TEXT_PLAIN).build();
    }

    /**
     * Deletes requested script
     * 
     * @param deleteScriptsRequest
     * @return ScriptsModifiedResponse
     */
    @DELETE
    @Path("/deletescripts")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public ScriptsModifiedResponse deleteScripts(ModifyScriptsRequest deleteScriptsRequest) {
        ScriptsModifiedResponse response = null;
        try {
            CaseInsensitiveStringList scriptNames = new CaseInsensitiveStringList();
            for (Script script : deleteScriptsRequest.getScripts()) {
                scriptNames.add(script.getName());
            }

            List<File> files = getFilesInScriptDir();

            response = new ScriptsModifiedResponse();
            List<String> scriptsDeleted = new ArrayList<>();
            for (File file : files) {
                try {
                    String content = FileUtils.readFileToString(file, "UTF-8");
                    JSONObject oScript = getScriptObject(content);
                    if (oScript != null) {
                        JSONObject header = (JSONObject) oScript.get("HEADER");
                        String foundScriptName = header.get("NAME").toString();
                        if (scriptNames.contains(foundScriptName)) {
                            scriptsDeleted.add(foundScriptName);
                            if (file.delete()) {
                                logger.debug("Deleted script: {}", foundScriptName);
                            }
                            else {
                                logger.error("Error deleting {} script!", file.getAbsolutePath());
                            }
                        }
                    }
                }
                catch (Exception e) {
                    assert (file != null);
                    logger.error("Failed to read file header for script: {}{}", file.getName(), e.getMessage());
                }
            }
            response.setScriptsModified(scriptsDeleted.toArray(new String[scriptsDeleted.size()]));
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error deleting scripts:  Error: " + ex.getMessage(),
                    Status.INTERNAL_SERVER_ERROR, logger);
        }

        return response;
    }

    private JSONObject getScriptObject(String content) throws Exception {
        JSONObject script = new JSONObject();
        if (content.startsWith(headerStart)) {
            int iHeader = content.indexOf(headerEnd);
            if (iHeader > 0) {
                String header = content.substring(0, iHeader);
                header = header.replace(headerStart, "");
                header = header.replace(headerEnd, "");

                String body = content.substring(iHeader + headerEnd.length());

                JSONParser parser = new JSONParser();
                JSONObject jHeader = (JSONObject) parser.parse(header);
                if (jHeader != null) {
                    script.put("HEADER", jHeader);
                    script.put("BODY", body);
                    return script;
                }
            }
        }
        return null;
    }

    private List<File> getFilesInScriptDir() throws IOException {
        if (!uploadDirExists()) {
            FileUtils.forceMkdir(getUploadDir());
        }
        String[] exts = { "js" };
        File scriptsDir = new File(scriptFolder);
        return (List<File>) FileUtils.listFiles(scriptsDir, exts, false);
    }

    private JSONObject saveScript(String name, String description, String content) throws Exception {
        if (StringUtils.trimToNull(name) == null) {
            logger.error("Invalid input script name: {}", name);
            return null;
        }

        if (StringUtils.trimToNull(content) == null) {
            logger.error("Invalid input script content.");
            return null;
        }

        boolean canExport = validateExport(content);

        if (!uploadDirExists()) {
            FileUtils.forceMkdir(getUploadDir());
        }

        if (!hoot.services.utils.FileUtils.validateFilePath(scriptFolder, scriptFolder + "/" + name + ".js")) {
            throw new Exception("Script name can not contain path.");
        }

        File fScript = new File(scriptFolder, name + ".js");
        if (!fScript.exists()) {
            if (!fScript.createNewFile()) {
                logger.error("File {} should not have existed before we tried to create it!", fScript.getAbsolutePath());
            }
        }

        JSONObject oHeader = new JSONObject();
        oHeader.put("NAME", name);
        oHeader.put("DESCRIPTION", description);
        oHeader.put("CANEXPORT", canExport);

        String header = headerStart;
        header += oHeader.toString();
        header += headerEnd;

        FileUtils.writeStringToFile(fScript, header + content);

        logger.debug("Saved script: {}", name);

        return oHeader;
    }

    // This function checks to see if the script has both getDbSchema and
    // translateToOgr which indicates if it can export
    private static boolean validateExport(String script) throws Exception {
        boolean canExport = false;
        Context context = Context.enter();
        try {
            // initialize Rhino
            ScriptableObject scope = context.initStandardObjects();
            context.setOptimizationLevel(-1);

            scope.put("context", scope, context);
            scope.put("scope", scope, scope);
            scope.put("APP_ROOT", scope, homeFolder);

            try (FileReader frHeader = new FileReader(jsHeaderScriptPath);
                 BufferedReader jsHeader = new BufferedReader(frHeader)) {
                context.evaluateReader(scope, jsHeader, "jsHeader", 1, null);
            }

            try (StringReader sr = new StringReader(script);
                 BufferedReader translation_script = new BufferedReader(sr)) {
                context.evaluateReader(scope, translation_script, "translation_script", 1, null);
            }

            // call getDbSchema call any required preloading functions
            Object getSchemaObj = scope.get("getDbSchema", scope);
            Object translateToOgr = scope.get("translateToOgr", scope);

            // If not exist then will return Tag instead of function
            boolean getDbSchemaExist = (getSchemaObj instanceof Function);

            // If not exist then will return Tag instead of function
            boolean translateToOgrExist = (translateToOgr instanceof Function);

            canExport = getDbSchemaExist && translateToOgrExist;
        }
        catch (Exception ex) {
            logger.error(ex.getMessage());
            if (ex instanceof EvaluatorException) {
                throw ex;
            }
        }
        finally {
            Context.exit();
        }

        return canExport;
    }
}
