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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2017, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.ingest;

import static hoot.services.HootProperties.*;
import static hoot.services.models.db.QTranslationFolders.translationFolders;
import static hoot.services.models.db.QTranslationFolderMappings.translationFolderMappings;
import static hoot.services.utils.DbUtils.createQuery;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.StringReader;
import java.nio.charset.Charset;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import javax.servlet.http.HttpServletRequest;
import javax.ws.rs.Consumes;
import javax.ws.rs.DELETE;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import com.querydsl.core.Tuple;
import com.querydsl.core.types.dsl.Expressions;
import org.apache.commons.io.FileUtils;
import org.apache.commons.io.FilenameUtils;
import org.apache.commons.lang3.StringUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;
import org.mozilla.javascript.Context;
import org.mozilla.javascript.EvaluatorException;
import org.mozilla.javascript.Function;
import org.mozilla.javascript.ScriptableObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.controllers.osm.map.FolderRecords;
import hoot.services.controllers.osm.map.FolderResource;
import hoot.services.models.db.Folders;
import hoot.services.models.db.TranslationFolderMappings;
import hoot.services.models.db.Users;
import hoot.services.utils.DbUtils;


@Controller
@Path("/customscript")
@Transactional
public class CustomScriptResource {
    private static final Logger logger = LoggerFactory.getLogger(CustomScriptResource.class);
    private static final boolean FOUO_TRANSLATIONS_EXIST;
    private static final String HEADER_START = "/*<<<";
    private static final String HEADER_END = ">>>*/" + System.lineSeparator();

    static {
        if ((new File(DEFAULT_FOUO_TRANSLATIONS_CONFIG)).exists()) {
            FOUO_TRANSLATIONS_EXIST = true;
            logger.info("FOUO translations are present.");
        }
        else {
            FOUO_TRANSLATIONS_EXIST = false;
            logger.info("FOUO translations are not present.");
        }
    }
    /**
     * Returns the directory the scripts are stored in
     *
     * @return a directory
     */
    private static File getUploadDir() {
        return new File(SCRIPT_FOLDER);
    }

    private static boolean uploadDirExists() {
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
     * POST hoot-services/ingest/customscript/save
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
    @Produces(MediaType.APPLICATION_JSON)
    public Response processSave(String script,
                                @QueryParam("SCRIPT_NAME") String scriptName,
                                @QueryParam("SCRIPT_DESCRIPTION") String scriptDescription,
                                @QueryParam("folderId") Long folderId) {
        JSONArray saveArr = new JSONArray();

        // get full directory path for file being deleted
        String getPath = getFolderPath(folderId);

        String translationPath = "/" + scriptName;
        translationPath = getPath != null ? getPath + translationPath : translationPath;

        try {
            saveArr.add(saveScript(scriptName, scriptDescription, script, translationPath));
        }
        catch (Exception e) {
            String msg = "Error processing script save for: " + translationPath + ".  Cause: " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        if (folderId != null) {
            createQuery()
                .insert(translationFolderMappings)
                .columns(translationFolderMappings.folderId, translationFolderMappings.displayName)
                .values(folderId, scriptName)
                .execute();
        }

        return Response.ok(saveArr.toString()).build();
    }

    @POST
    @Path("/saveMultiple")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public ScriptsModifiedResponse saveScripts(ModifyScriptsRequest saveMultipleScriptsRequest) {
        ScriptsModifiedResponse response;

        try {
            response = new ScriptsModifiedResponse();
            List<String> scriptsModified = new ArrayList<>();
            for (Script script : saveMultipleScriptsRequest.getScripts()) {
                if (saveScript(script.getName(), script.getDescription(), script.getContent(), null) != null) {
                    scriptsModified.add(script.getName());
                }
            }

            response.setScriptsModified(scriptsModified.toArray(new String[scriptsModified.size()]));
        }
        catch (Exception e) {
            String msg = "Error processing save scripts request.  Cause: " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
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
    @Produces(MediaType.APPLICATION_JSON)
    public Response getScriptsList() {
        JSONArray retList = new JSONArray();
        JSONArray filesList = new JSONArray();

        try {
            List<String> configFiles = new ArrayList<>();
            configFiles.add(DEFAULT_TRANSLATIONS_CONFIG);

            if (FOUO_TRANSLATIONS_EXIST) {
                configFiles.add(DEFAULT_FOUO_TRANSLATIONS_CONFIG);
            }

            filesList.addAll(getDefaultList(configFiles));

            // sort the list
            Map<String, JSONObject> sortedScripts = new TreeMap<>();
            for (Object o : filesList) {
                JSONObject cO = (JSONObject) o;
                String sName = cO.get("NAME").toString();
                sortedScripts.put(sName.toUpperCase(), cO);
            }

            List<Tuple> mappings = createQuery()
                    .select(translationFolderMappings.id, translationFolderMappings.displayName, translationFolderMappings.folderId, translationFolders.path)
                    .from(translationFolderMappings)
                    .leftJoin(translationFolders).on(translationFolders.id.eq(translationFolderMappings.folderId))
                    .fetch();

            for (Tuple folder: mappings) {
                JSONObject json = new JSONObject();
                String translationName = folder.get(translationFolderMappings.displayName);

                json.put("id", folder.get(translationFolderMappings.id));
                json.put("name", translationName);
                json.put("folderId", folder.get(translationFolderMappings.folderId));

                String path = folder.get(translationFolders.path);
                String translationPath = "/" + translationName;
                translationPath = path != null ? path + translationPath : translationPath;
                File translationFile = new File(SCRIPT_FOLDER, translationPath + ".js");

                String content = FileUtils.readFileToString(translationFile, "UTF-8");
                JSONObject oScript = getScriptObject(content);

                if (oScript != null) {
                    JSONObject header = (JSONObject) oScript.get("HEADER");
                    json.put("DESCRIPTION", header.get("DESCRIPTION"));
                    json.put("CANEXPORT", header.get("CANEXPORT"));
                }

                sortedScripts.put(translationName, json);
            }

            retList.addAll(sortedScripts.values());
        }
        catch (Exception e) {
            String msg = "Error getting scripts list.  Cause: " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return Response.ok(retList.toString()).build();
    }

    /**
     * getDefaultList reads the DefaultTranslations.json and passes default
     * translations list. The list is used by UI to distinguish between custom
     * and default translations.
     *
     * @return JSONArray of translation objects
     * @throws IOException, ParseException
     */
    private static JSONArray getDefaultList(List<String> configFiles) throws IOException, ParseException {
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
                                File fScript = new File(HOME_FOLDER + "/" + oTrans.get("PATH"));
                                if (fScript.exists()) {
                                    String sScript = FileUtils.readFileToString(fScript, Charset.defaultCharset());
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
                                File fouoPath = new File(HOME_FOLDER + "/" + jsTrans.get("FOUO_PATH"));
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
     * @param identifier
     *            Name of the script to retrieve.
     * @return Requested translation script
     */
    @GET
    @Path("/getscript")
    @Produces(MediaType.TEXT_PLAIN)
    public Response getScript(@QueryParam("SCRIPT_NAME") String identifier) {
        String script = "";

        long translationId = -1;

        try {
            translationId = Long.parseLong(identifier);
        } catch (NumberFormatException exc) {}

        try {
            if (translationId > -1) {
                Tuple folderMapInfo = createQuery().select(translationFolderMappings.displayName, translationFolderMappings.folderId)
                        .from(translationFolderMappings)
                        .where(translationFolderMappings.id.eq(translationId))
                        .fetchFirst();

                String translationName = folderMapInfo.get(translationFolderMappings.displayName);
                Long translationFolderId = folderMapInfo.get(translationFolderMappings.folderId);

                // get full directory path for file being deleted
                String getPath = getFolderPath(translationFolderId);

                String translationPath = "/" + translationName;
                translationPath = getPath != null ? getPath + translationPath : translationPath;
                File translationFile = new File(SCRIPT_FOLDER, translationPath + ".js");

                if (translationFile.exists()) {
                    try {
                        String content = FileUtils.readFileToString(translationFile, "UTF-8");
                        JSONObject oScript = getScriptObject(content);

                        if (oScript != null) {
                            script = oScript.get("BODY").toString();
                        }
                    }
                    catch (Exception e) {
                        logger.error("Failed to read file header: {}", e.getMessage(), e);
                    }
                }
            } else {
                File scriptsDir = new File(SCRIPT_FOLDER);

                if (scriptsDir.exists()) {
                    String[] exts = { "js" };
                    List<File> files = (List<File>) FileUtils.listFiles(scriptsDir, exts, false);

                    for (File file : files) {
                        try {
                            String content = FileUtils.readFileToString(file, "UTF-8");
                            JSONObject oScript = getScriptObject(content);

                            if (oScript != null) {
                                JSONObject header = (JSONObject) oScript.get("HEADER");
                                if (header.get("NAME").toString().equalsIgnoreCase(identifier)) {
                                    script = oScript.get("BODY").toString();
                                    break;
                                }
                            }
                        }
                        catch (Exception e) {
                            logger.error("Failed to read file header: {}", e.getMessage(), e);
                        }
                    }
                }
            }
        }
        catch (Exception ex) {
            String msg = "Error getting script: " + identifier;
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        return Response.ok(script).build();
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
     *            Relative path of default translation script. (Relative to hoot home path)
     * @return Requested translation script
     */
    @GET
    @Path("/getdefaultscript")
    @Produces(MediaType.TEXT_PLAIN)
    public Response getDefaultScript(@QueryParam("SCRIPT_PATH") String scriptPath) {
        String script = "";
        try {
            List<String> configFiles = new ArrayList<>();

            configFiles.add(DEFAULT_TRANSLATIONS_CONFIG);
            if (FOUO_TRANSLATIONS_EXIST) {
                configFiles.add(DEFAULT_FOUO_TRANSLATIONS_CONFIG);
            }

            JSONArray defList = getDefaultList(configFiles);

            // See Bug #6483 Read vulnerability in services script API
            boolean pathValidated = false;
            for (Object aDefList : defList) {
                JSONObject item = (JSONObject) aDefList;

                Object path = item.get("PATH");
                if ((path != null) && scriptPath.equals(path.toString())) {
                    pathValidated = true;
                    break;
                }

                Object fouoPath = item.get("FOUO_PATH");
                if ((fouoPath != null) && scriptPath.equals(fouoPath.toString())) {
                    pathValidated = true;
                    break;
                }
            }

            if (pathValidated) {
                File scriptFile = new File(HOME_FOLDER, scriptPath);
                if (scriptFile.exists()) {
                    script = FileUtils.readFileToString(scriptFile, Charset.defaultCharset());
                }
            }
            else {
                throw new IOException("Invalid script path: " + scriptPath);
            }
        }
        catch (Exception ex) {
            String msg = "Error getting script: " + scriptPath;
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        return Response.ok(script).build();
    }

    /**
     * Deletes the specified script.
     *
     * GET hoot-services/ingest/customscript/deletescript?SCRIPT_INFO=My Test6
     *
     * //TODO: should be an HTTP DELETE
     *
     * @param identifier
     *            Name of the script to delete.
     * @return JSON Array containing JSON of name and description of deleted scripts
     */
    @GET
    @Path("/deletescript")
    @Produces(MediaType.APPLICATION_JSON)
    public Response deleteScript(@QueryParam("SCRIPT_INFO") String identifier) {
        JSONArray delArr = new JSONArray();

        long translationId = -1;

        try {
            translationId = Long.parseLong(identifier);
        } catch (NumberFormatException exc) {}

        if (translationId > -1) {
            // get the display name because thats the file to delete. folder id is to get path for the file
            Tuple folderMapInfo = createQuery().select(translationFolderMappings.displayName, translationFolderMappings.folderId)
                .from(translationFolderMappings)
                .where(translationFolderMappings.id.eq(translationId))
                .fetchFirst();

            String translationName = folderMapInfo.get(translationFolderMappings.displayName);
            Long translationFolderId = folderMapInfo.get(translationFolderMappings.folderId);

            // get full directory path for file being deleted
            String getPath = getFolderPath(translationFolderId);

            String translationPath = "/" + translationName;
            translationPath = getPath != null ? getPath + translationPath : translationPath;
            File translationFile = new File(SCRIPT_FOLDER, translationPath + ".js");
            translationFile.delete();

            createQuery().delete(translationFolderMappings)
                .where(translationFolderMappings.id.eq(translationId))
                .execute();
        } else {
            try {
                List<File> files = getFilesInScriptDir();
                if (files == null) {
                    throw new IOException("Script directory does not exist.");
                }

                for (File file : files) {
                    String content = FileUtils.readFileToString(file, "UTF-8");
                    JSONObject oScript = getScriptObject(content);

                    if (oScript != null) {
                        JSONObject header = (JSONObject) oScript.get("HEADER");
                        if (header.get("NAME").toString().equalsIgnoreCase(identifier)) {
                            delArr.add(header);
                            if (!file.delete()) {
                                throw new IOException("Error deleting script: " + file.getAbsolutePath());
                            }
                            break;
                        }
                    }
                }
            }
            catch (Exception ex) {
                String msg = "Error deleting script: " + identifier;
                throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
            }
        }

        return Response.ok(delArr.toString()).build();
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
        ScriptsModifiedResponse response;
        try {
            CaseInsensitiveStringList scriptNames = new CaseInsensitiveStringList();
            for (Script script : deleteScriptsRequest.getScripts()) {
                scriptNames.add(script.getName());
            }

            List<File> files = getFilesInScriptDir();

            response = new ScriptsModifiedResponse();
            List<String> scriptsDeleted = new ArrayList<>();
            for (File file : files) {
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
                            throw new IOException("Error deleting script: " + file.getAbsolutePath());
                        }
                    }
                }
            }

            response.setScriptsModified(scriptsDeleted.toArray(new String[scriptsDeleted.size()]));
        }
        catch (Exception ex) {
            String msg = "Error deleting scripts!";
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        return response;
    }

    @POST
    @Path("/createfolder")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response addFolder(@javax.ws.rs.core.Context HttpServletRequest request,
            @QueryParam("parentId") Long parentId,
            @QueryParam("folderName") String folderName,
            @QueryParam("isPublic") Boolean isPublic) throws SQLException {

        Map<String, Object> resp = new HashMap<>();
        Users user = Users.fromRequest(request);
        Long userid = -1L;
        if(user != null) {
            userid = user.getId();
        }

        // get folder path
        String folderPath = createQuery()
                .select(translationFolders.path)
                .from(translationFolders)
                .where(translationFolders.id.eq(parentId))
                .fetchFirst();
        String newFolder = "/" + folderName;
        folderPath = folderPath != null ? folderPath + newFolder : newFolder;

        File workDir = new File(SCRIPT_FOLDER, folderPath);

        if (!workDir.exists()) {
            try {
                FileUtils.forceMkdir(workDir);
            }
            catch (IOException exc) {
                String msg = "Error creating folder!";
                throw new WebApplicationException(exc, Response.serverError().entity(msg).build());
            }
        } else {
            return Response.status(Response.Status.CONFLICT).entity("Unable to create folder because it already exists").build();
        }

        Long newId = createQuery()
                .select(Expressions.numberTemplate(Long.class, "nextval('translation_folders_id_seq')"))
                .from()
                .fetchOne();

        Timestamp now = new Timestamp(System.currentTimeMillis());

        createQuery()
            .insert(translationFolders)
            .columns(translationFolders.id, translationFolders.displayName, translationFolders.parentId, translationFolders.userId, translationFolders.publicCol, translationFolders.createdAt, translationFolders.path)
            .values(newId, folderName, parentId, userid, isPublic, now, folderPath)
            .execute();

        resp.put("success", true);
        resp.put("folderId", 1);

        return Response.ok().entity(resp).build();
    }

    /**
     * Returns a list of all translation folders in the services database.
     *
     * GET hoot-services/osm/api/0.6/map/folders
     *
     * @return a JSON object containing a list of folders
     */
    @GET
    @Path("/getFolders")
    @Produces(MediaType.APPLICATION_JSON)
    public FolderRecords getFolders(@javax.ws.rs.core.Context HttpServletRequest request) {
        Users user = Users.fromRequest(request);

        return FolderResource.mapFolderRecordsToFolders(getTranslationFoldersForUser(user));
    }

    /**
     * Returns a list of all translation folders in the services database.
     *
     * GET hoot-services/osm/api/0.6/map/folders
     *
     * @return a JSON object containing a list of folders
     */
    @GET
    @Path("/getMappings")
    @Produces(MediaType.APPLICATION_JSON)
    public List<TranslationFolderMappings> getMappings(@javax.ws.rs.core.Context HttpServletRequest request) {
        Users user = Users.fromRequest(request);

        List<TranslationFolderMappings> mappings = createQuery()
            .select(translationFolderMappings)
            .from(translationFolderMappings)
            .fetch();

        return mappings;
    }

    private static List<Folders> getTranslationFoldersForUser(Users user) {
        List<Folders> folderRecordSet = DbUtils.getTranslationFoldersForUser(user.getId());
        List<Folders> out = new ArrayList<Folders>();
        for(Folders f : folderRecordSet) {
            if(FolderResource.folderIsPublic(folderRecordSet, f, user)) {
                out.add(f);
            }
        }
        return out;
    }

    private static JSONObject getScriptObject(String content) throws ParseException {
        JSONObject script = new JSONObject();

        if (content.startsWith(HEADER_START)) {
            int iHeader = content.indexOf(HEADER_END);
            if (iHeader > 0) {
                String header = content.substring(0, iHeader);
                header = header.replace(HEADER_START, "");
                header = header.replace(HEADER_END, "");

                String body = content.substring(iHeader + HEADER_END.length());

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

    private static List<File> getFilesInScriptDir() throws IOException {
        if (!uploadDirExists()) {
            FileUtils.forceMkdir(getUploadDir());
        }

        String[] exts = { "js" };
        File scriptsDir = new File(SCRIPT_FOLDER);

        return (List<File>) FileUtils.listFiles(scriptsDir, exts, false);
    }

    private static JSONObject saveScript(String name, String description, String content, String scriptPath) throws IOException {
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

        File fScript = new File(SCRIPT_FOLDER, scriptPath + ".js");
        if (!fScript.exists()) {
            if (!fScript.createNewFile()) {
                logger.error("File {} should not have existed before we tried to create it!", fScript.getAbsolutePath());
            }
        }

        JSONObject oHeader = new JSONObject();
        oHeader.put("NAME", name);
        oHeader.put("DESCRIPTION", description);
        oHeader.put("CANEXPORT", canExport);

        String header = HEADER_START;
        header += oHeader.toString();
        header += HEADER_END;

        FileUtils.writeStringToFile(fScript, header + content, Charset.defaultCharset());

        logger.debug("Saved script: {}", name);

        return oHeader;
    }

    private static String getFolderPath(Long translationFolderId) {
        // get full directory path for file being deleted
        String path = createQuery().select(translationFolders.path)
                .from(translationFolders)
                .where(translationFolders.id.eq(translationFolderId))
                .fetchOne();

        return path;
    }

    // This function checks to see if the script has both getDbSchema and
    // translateToOgr which indicates if it can export
    private static boolean validateExport(String script) throws IOException {
        boolean canExport = false;
        Context context = Context.enter();
        try {
            // initialize Rhino
            ScriptableObject scope = context.initStandardObjects();
            context.setOptimizationLevel(-1);

            scope.put("context", scope, context);
            scope.put("scope", scope, scope);
            scope.put("APP_ROOT", scope, HOME_FOLDER);

            try (FileReader frHeader = new FileReader(JS_HEADER_SCRIPT_PATH)) {
                try (BufferedReader jsHeader = new BufferedReader(frHeader)) {
                    context.evaluateReader(scope, jsHeader, "jsHeader", 1, null);
                }
            }

            try (StringReader sr = new StringReader(script)) {
                try (BufferedReader translation_script = new BufferedReader(sr)) {
                    context.evaluateReader(scope, translation_script, "translation_script", 1, null);
                }
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

    /**
     * A case insensitive string list
     */
    private static class CaseInsensitiveStringList extends ArrayList<String> {

        @Override
        public boolean contains(Object o) {
            String paramStr = (String) o;
            for (String s : this) {
                if (paramStr.equalsIgnoreCase(s)) {
                    return true;
                }
            }
            return false;
        }
    }

    private static boolean validateFilePath(String expectedPath, String actualPath) {
        String path = FilenameUtils.getFullPathNoEndSeparator(actualPath);
        return expectedPath.equals(path);
    }
}
