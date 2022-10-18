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
 * @copyright Copyright (C) 2015, 2016, 2017, 2021, 2022 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.ingest;

import static hoot.services.HootProperties.*;
import static hoot.services.models.db.QTranslationFolders.translationFolders;
import static hoot.services.models.db.QTranslations.translations;
import static hoot.services.utils.DbUtils.createQuery;
import static hoot.services.utils.DbUtils.getConnection;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.StringReader;
import java.nio.charset.Charset;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.servlet.http.HttpServletRequest;
import javax.ws.rs.BadRequestException;
import javax.ws.rs.Consumes;
import javax.ws.rs.DELETE;
import javax.ws.rs.ForbiddenException;
import javax.ws.rs.GET;
import javax.ws.rs.NotFoundException;
import javax.ws.rs.POST;
import javax.ws.rs.PUT;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import com.querydsl.core.Tuple;
import com.querydsl.core.types.dsl.Expressions;
import com.querydsl.sql.dml.SQLUpdateClause;
import org.apache.commons.io.FileUtils;
import org.apache.commons.lang3.StringUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;
import org.mozilla.javascript.EvaluatorException;
import org.mozilla.javascript.Function;
import org.mozilla.javascript.ScriptableObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.controllers.osm.user.UserResource;
import hoot.services.models.db.TranslationFolder;
import hoot.services.models.db.Translations;
import hoot.services.models.db.Users;
import hoot.services.utils.DbUtils;


@Controller
@Path("/customscript")
@Transactional
public class CustomScriptResource {
    private static final Logger logger = LoggerFactory.getLogger(CustomScriptResource.class);
    private static final String HEADER_START = "/*<<<";
    private static final String HEADER_END = ">>>*/" + System.lineSeparator();

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
    @Produces(MediaType.APPLICATION_JSON)
    public Response processSave(@Context HttpServletRequest request,
                                String script,
                                @QueryParam("scriptName") String scriptName,
                                @QueryParam("scriptDescription") String scriptDescription,
                                @QueryParam("folderId") Long folderId) {
        Users user = Users.fromRequest(request);
        JSONArray saveArr = new JSONArray();

        if (folderId == null) {
            folderId = 0L;
        }

        TranslationFolder folder = getTranslationFolderForUser(user, folderId);
        String path = folder.getPath();

        try {
            saveArr.add(saveScript(scriptName, scriptDescription, script, path));
        }
        catch (Exception e) {
            String msg = "Error processing script save for: " + scriptName + ".  Cause: " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        long translationExists = createQuery()
                .select(translations)
                .from(translations)
                .where(translations.folderId.eq(folderId).and(translations.displayName.eq(scriptName)))
                .fetchCount();

        if (translationExists == 0) {
            DbUtils.addTranslation(scriptName, user.getId(), folderId);
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
    public Response getScriptsList(@Context HttpServletRequest request) {
        Users user = Users.fromRequest(request);
        JSONArray retList = new JSONArray();
        JSONArray filesList = new JSONArray();

        try {
            filesList.addAll(getDefaultList());

            for (Object o : filesList) {
                JSONObject cO = (JSONObject) o;
                cO.put("displayPath", cO.get("name"));
                retList.add(cO);
            }

            List<Tuple> mappings = createQuery()
                .select(translations.id,
                        translations.displayName,
                        translations.userId,
                        translations.createdAt,
                        translations.folderId,
                        translationFolders.path,
                        translationFolders.publicCol)
                .from(translations)
                .leftJoin(translationFolders).on(translationFolders.id.eq(translations.folderId))
                .fetch();

            Set<Long> visibleFolders = DbUtils.getTranslationFolderIdsForUser(user.getId());
            for (Tuple translationRecord: mappings) {
                String translationName = translationRecord.get(translations.displayName);
                Long folderId = translationRecord.get(translations.folderId);
                Boolean parentFolderIsPublic = translationRecord.get(translationFolders.publicCol);

                String path = translationRecord.get(translationFolders.path);
                String translationPath = File.separator + translationName;
                translationPath = path != null ? path + translationPath : translationPath;
                File translationFile = new File(SCRIPT_FOLDER, translationPath + ".js");

                if (translationFile.exists()) {
                    String content = FileUtils.readFileToString(translationFile, "UTF-8");
                    JSONObject oScript = getScriptObject(content);

                    JSONObject json = new JSONObject();
                    json.put("id", translationRecord.get(translations.id));
                    json.put("name", translationName);
                    json.put("userId", translationRecord.get(translations.userId));
                    json.put("date", translationRecord.get(translations.createdAt).toString());
                    json.put("folderId", folderId);
                    json.put("displayPath", translationPath);

                    if (parentFolderIsPublic == null) {
                        parentFolderIsPublic = true;
                    }
                    if (folderId == null || folderId.equals(0L) || visibleFolders.contains(folderId)) {
                        json.put("public", parentFolderIsPublic);
                    }

                    if (oScript != null) {
                        JSONObject header = normalizeHeader((JSONObject) oScript.get("HEADER"));
                        json.put("description", header.get("description"));
                        json.put("canExport", header.get("canExport"));
                    }

                    retList.add(json);
                }
            }
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
    private static JSONArray getDefaultList() throws IOException, ParseException {
        JSONArray filesList = new JSONArray();
        List<File> configFiles = new ArrayList<>();

        // List<File> fileList = File.listFiles(HOME_FOLDER,new String[] {},false);
        File dir = new File(HOME_FOLDER);
        for (File file : dir.listFiles()) {
            if (file.isDirectory() && file.getName().startsWith("translations")) {
                File confFile = new File(file,TRANSLATION_CONFIG);

                // Sanity check
                if (!confFile.exists()) {
                    logger.error("Missing translation conf file: " + confFile.getName());
                    continue;
                }

                try (FileReader reader = new FileReader(confFile)) {
                    JSONParser jsonParser = new JSONParser();
                    JSONArray defTranslations = (JSONArray) jsonParser.parse(reader);
                    for (Object defTranslation : defTranslations) {
                        JSONObject oTrans = (JSONObject) defTranslation;
                        oTrans.put("default", true);
                        String desc = oTrans.get("description").toString();
                        if (desc.isEmpty()) {
                            desc = oTrans.get("name").toString();
                        }
                        oTrans.put("description", desc);

                        Object oCanExport = oTrans.get("canExport");

                        // If the canExport is not available then try to determine
                        if (oCanExport == null) {
                            // Get the script
                            if (oTrans.get("path") != null) {
                                File fScript = new File(HOME_FOLDER + "/" + oTrans.get("path"));
                                if (fScript.exists()) {
                                    String sScript = FileUtils.readFileToString(fScript, Charset.defaultCharset());
                                    boolean canExport = validateExport(sScript);
                                    oTrans.put("canExport", canExport);
                                }
                            }
                        }
                    }

                    // validate FOUO support
                    if (!defTranslations.isEmpty()) {
                        for (Object oTrans : defTranslations) {
                            JSONObject jsTrans = (JSONObject) oTrans;
                            filesList.add(jsTrans);
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
     * GET hoot-services/ingest/customscript/getscript?scriptName=MyTest
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
    public Response getScript(@Context HttpServletRequest request, @QueryParam("scriptName") String identifier) {
        Users user = Users.fromRequest(request);
        String script = "";

        long translationId = -1;

        try {
            translationId = Long.parseLong(identifier);
        } catch (NumberFormatException exc) {}

        try {
            if (translationId > -1) {
                Translations folderMapInfo = DbUtils.getTranslation(translationId);
                String translationName = folderMapInfo.getDisplayName();

                // get full directory path for file being deleted
                TranslationFolder folder = getTranslationFolderForUser(user, folderMapInfo.getFolderId());
                String getPath = folder.getPath();

                String translationPath = File.separator + translationName;
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
                                JSONObject header = normalizeHeader((JSONObject) oScript.get("HEADER"));
                                Object scriptName = header.get("name");
                                if (scriptName.toString().equalsIgnoreCase(identifier)) {
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
    public Response getDefaultScript(@QueryParam("scriptPath") String scriptPath) {
        String script = "";
        try {
            JSONArray defList = getDefaultList();

            // See Bug #6483 Read vulnerability in services script API
            boolean pathValidated = false;
            for (Object aDefList : defList) {
                JSONObject item = (JSONObject) aDefList;

                Object path = item.get("path");
                if ((path != null) && scriptPath.equals(path.toString())) {
                    pathValidated = true;
                    break;
                }

                Object importPath = item.get("importPath");
                Object exportPath = item.get("exportPath");
                if ((importPath != null) && scriptPath.equals(importPath.toString())) {
                    pathValidated = true;
                    scriptPath = importPath.toString();
                    break;
                } else if ((exportPath != null) && scriptPath.equals(exportPath.toString())) {
                    pathValidated = true;
                    scriptPath = exportPath.toString();
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
     * GET hoot-services/ingest/customscript/deletescript?scriptInfo=My Test6
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
    public Response deleteScript(@Context HttpServletRequest request, @QueryParam("scriptInfo") String identifier) {
        Users user = Users.fromRequest(request);
        JSONArray delArr = new JSONArray();

        long translationId = -1;

        try {
            translationId = Long.parseLong(identifier);
        } catch (NumberFormatException exc) {}

        try {
            if (translationId > -1) {
                // get the display name because that's the file to delete. folder id is to get path for the file
                Translations folderMapInfo = getTranslationForUser(user, translationId, true /*editable*/);
                String translationName = folderMapInfo.getDisplayName();

                // get full directory path for file being deleted
                TranslationFolder folder = getTranslationFolderForUser(user, folderMapInfo.getFolderId());
                String getPath = folder.getPath();

                String translationPath = File.separator + translationName;
                translationPath = getPath != null ? getPath + translationPath : translationPath;
                File translationFile = new File(SCRIPT_FOLDER, translationPath + ".js");

                String content = FileUtils.readFileToString(translationFile, "UTF-8");
                JSONObject oScript = getScriptObject(content);
                delArr.add(normalizeHeader((JSONObject) oScript.get("HEADER")));

                translationFile.delete();

                createQuery().delete(translations)
                    .where(translations.id.eq(translationId))
                    .execute();
            } else {

                List<File> files = getFilesInScriptDir();
                if (files == null) {
                    throw new IOException("Script directory does not exist.");
                }

                for (File file : files) {
                    String content = FileUtils.readFileToString(file, "UTF-8");
                    JSONObject oScript = getScriptObject(content);

                    if (oScript != null) {
                        JSONObject header = normalizeHeader((JSONObject) oScript.get("HEADER"));
                        Object scriptName = header.get("name");
                        if (scriptName.toString().equalsIgnoreCase(identifier)) {
                            delArr.add(header);
                            if (!file.delete()) {
                                throw new IOException("Error deleting script: " + file.getAbsolutePath());
                            }
                            break;
                        }
                    }
                }
            }
        }
        catch (Exception ex) {
            throw new WebApplicationException(ex, Response.serverError().entity(ex.getMessage()).build());
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
                    JSONObject header = normalizeHeader((JSONObject) oScript.get("HEADER"));
                    String foundScriptName = header.get("name").toString();
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

    /**
     * Creates translations folder
     *
     * POST hoot-services/ingest/customscript/createfolder
     *
     * @param request
     * @param parentId
     * @param folderName
     * @param isPublic
     * @return
     * @throws SQLException
     */
    @POST
    @Path("/createfolder")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response addFolder(@Context HttpServletRequest request,
            @QueryParam("parentId") Long parentId,
            @QueryParam("folderName") String folderName,
            @QueryParam("isPublic") Boolean isPublic) {

        Map<String, Object> resp = new HashMap<>();
        Users user = Users.fromRequest(request);
        Long userid = -1L;
        if(user != null) {
            userid = user.getId();
        }

        // get folder path
        TranslationFolder folder = getTranslationFolderForUser(user, parentId);
        String folderPath = folder.getPath();

        if(folder.getId() != 0 && !folder.getUserId().equals(userid)) {
            throw new ForbiddenException(Response.status(Response.Status.FORBIDDEN).type(MediaType.TEXT_PLAIN).entity("You must be the owner of this folder to create a folder inside it.").build());
        }

        String newFolder = File.separator + folderName;
        folderPath = folderPath != null ? folderPath + newFolder : newFolder;

        if(isPublic == null) {
            isPublic = folder.getPublicCol();
            // If the user did specify verify visibility:
        } else {
            if(isPublic && !folder.getPublicCol()) {
                throw new BadRequestException("public folders cannot be created under private folders");
            }
        }

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

    @DELETE
    @Path("/deleteFolder")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response deleteFolder(@Context HttpServletRequest request, @QueryParam("folderId") Long folderId) {

        Users user = Users.fromRequest(request);
        if(folderId.equals(0L) || user == null) {
            throw new BadRequestException();
        }

        TranslationFolder folder = getTranslationFolderForUser(user, folderId);

        if(!UserResource.adminUserCheck(user) && !folder.getUserId().equals(user.getId())) {
            throw new ForbiddenException(Response.status(Response.Status.FORBIDDEN).type(MediaType.TEXT_PLAIN).entity("You must own the folder to delete it").build());
        }

        long translationsInFolder = createQuery()
            .select(translations)
            .from(translations)
            .where(translations.folderId.eq(folderId))
            .fetchCount();

        if (translationsInFolder == 0) {
            // get file path
            String path = folder.getPath() != null ? folder.getPath() : "";
            File translationFile = new File(SCRIPT_FOLDER, path);
            translationFile.delete();

            createQuery()
                    .delete(translationFolders)
                    .where(translationFolders.id.eq(folderId))
                    .execute();
        } else {
            throw new ForbiddenException(Response.status(Response.Status.FORBIDDEN).type(MediaType.TEXT_PLAIN).entity("You must clear files inside the folder before deleting it").build());
        }

        return Response.ok().build();
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
    public List<TranslationFolder> getFolders(@Context HttpServletRequest request) {
        Users user = Users.fromRequest(request);

        List<TranslationFolder> folderRecordSet = DbUtils.getTranslationFoldersForUser(user.getId());
        List<TranslationFolder> visibleFolders = new ArrayList<>();
        for(TranslationFolder f : folderRecordSet) {
            if(folderIsPublic(folderRecordSet, f, user)) {
                visibleFolders.add(f);
            }
        }

        return visibleFolders;
    }

    @PUT
    @Path("/changeVisibility")
    @Produces(MediaType.APPLICATION_JSON)
    public Response changeVisibility(@Context HttpServletRequest request,
            @QueryParam("folderId") Long folderId,
            @QueryParam("visibility") String visibility) throws SQLException {
        Users user = Users.fromRequest(request);

        // handle some ACL logic:
        TranslationFolder folder = getTranslationFolderForUser(user, folderId);
        // User must also -own- the folder:
        if(!folder.getUserId().equals(user.getId())) {
            throw new ForbiddenException(Response.status(Response.Status.FORBIDDEN).type(MediaType.TEXT_PLAIN).entity("You must own the folder to set/view it's attributes").build());
        }

        // If a folder is changed to public, it will recurse up the parents and set those folders to public
        // If a folder is changed private, it will recurse down that folder and set everything to private
        String query = String.format("with recursive related_folders as (" +
                "     select id, display_name, parent_id, user_id, public, created_at from translation_folders where id = %d" +
                "     union" +
                "     select f.id, f.display_name, f.parent_id, f.user_id, f.public, f.created_at from translation_folders f" +
                "     inner join related_folders rf on (" +
                "          f.id != 0 AND (" +
                (visibility.equals("public") ? "f.id = rf.parent_id" : "f.parent_id = rf.id") +
                "          )" +
                "     )" +
                ")" +
                "update translation_folders x set public = %s " +
                "where x.id in (select id from related_folders)", folderId, visibility.equals("public"));
        long updated = 0;
        try(Connection conn = getConnection() ) {
            Statement stmt = conn.createStatement();
            stmt.execute(query);
            updated = stmt.getUpdateCount();

            if(!conn.getAutoCommit()) {
                conn.commit();
            }
        }

        java.util.Map<String, Object> r = new HashMap<>();
        r.put("updated", updated);
        return Response.status(Response.Status.OK).entity(r).build();
    }

    /**
     * Moves translation folder to new folder
     *
     * @param folderId
     *            ID of folder
     * @param targetFolderId
     *            ID of target folder
     *
     * @return jobId Success = True/False
     */
    @PUT
    @Path("/moveFolder")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response moveFolder(@Context HttpServletRequest request,
            @QueryParam("folderId") Long folderId,
            @QueryParam("targetFolderId") Long targetFolderId) {
        if(folderId.equals(0L)) {
            throw new BadRequestException();
        } else if(folderId.equals(targetFolderId)) {
            return Response.status(Response.Status.BAD_REQUEST).entity("The new parent folder cannot be the current folder").build();
        }

        Users user = Users.fromRequest(request);

        TranslationFolder currentTranslationFolder = getTranslationFolderForUser(user, folderId);
        String currentPath = currentTranslationFolder.getPath() != null ? currentTranslationFolder.getPath() : "";
        File currentFolder = new File(SCRIPT_FOLDER, currentPath);

        TranslationFolder targetTranslationFolder = getTranslationFolderForUser(user, targetFolderId);
        String targetPath = targetTranslationFolder.getPath() != null ? targetTranslationFolder.getPath() : "";
        File targetFolder = new File(SCRIPT_FOLDER, targetPath);

        if (user != null && targetTranslationFolder.getId() != 0 && (
                !currentTranslationFolder.getUserId().equals(user.getId())
                ||
                !targetTranslationFolder.getUserId().equals(user.getId())
        )) {
            return Response.status(Response.Status.FORBIDDEN).type(MediaType.TEXT_PLAIN).entity("You must own both folders to move it.").build();
        } else if (currentTranslationFolder.getPublicCol() && !targetTranslationFolder.getPublicCol()) {
            // dont allow moving public folder into private folder
            return Response.status(Response.Status.FORBIDDEN).type(MediaType.TEXT_PLAIN).entity("Can't move public folder into private folder.").build();
        }

        Map<String, Object> ret = new HashMap<>();
        try {
            FileUtils.moveDirectoryToDirectory(currentFolder, targetFolder, true);
            String newPath = targetPath + File.separator + currentTranslationFolder.getDisplayName();

            createQuery()
                .update(translationFolders)
                .where(translationFolders.id.eq(folderId))
                .set(translationFolders.parentId, targetTranslationFolder.getId())
                .execute();

            // Recurses down the folder and changes the paths of all sub folders as well as target folder
            DbUtils.changeTranslationFoldersPath(folderId, newPath);

            ret.put("success", true);
        }
        catch (IOException exc) {
            ret.put("success", false);
        }

        return Response.ok().entity(ret).build();
    }

    /**
     * Moves translation file to new folder
     *
     * @param translationId
     *            ID of translation
     * @param folderId
     *            ID of folder
     *
     * @return jobId Success = True/False
     */
    @PUT
    @Path("/modifyTranslation")
    @Produces(MediaType.APPLICATION_JSON)
    public Response modifyTranslation(@Context HttpServletRequest request,
            @QueryParam("name") String name,
            @QueryParam("description") String description,
            @QueryParam("translationId") Long translationId,
            @QueryParam("folderId") Long folderId,
            String script) {

        Users user = Users.fromRequest(request);

        Translations translation = getTranslationForUser(user, translationId, true /*editable*/);
        TranslationFolder currentFolder = getTranslationFolderForUser(user, translation.getFolderId());
        File currentFile = getTranslationFile(currentFolder.getPath(), translation.getDisplayName());

        TranslationFolder targetFolder = getTranslationFolderForUser(user, folderId);
        File targetFile = getTranslationFile(targetFolder.getPath(), name);

        Map<String, Object> ret = new HashMap<>();
        ret.put("success", true);

        // rename and move file
        if (currentFile.renameTo(targetFile)) {
            SQLUpdateClause query = createQuery()
                .update(translations)
                .where(translations.id.eq(translationId))
                .set(translations.folderId, targetFolder.getId())
                .set(translations.displayName, name);

            // if translation doesn't have an owner, assign it to user that moved it
            if (translation.getUserId() == -1) {
                query.set(translations.userId, user.getId());
            }

            query.execute();
        } else {
            ret.put("success", false);
        }

        try {
            saveScript(name, description, script, targetFolder.getPath());
        }
        catch (Exception e) {
            String msg = "Error processing script save for: " + name + ".  Cause: " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return Response.ok().entity(ret).build();
    }

    public static File getTranslationFile(String folderPath, String translationName) {
        String translationPath = File.separator + translationName;
        translationPath = folderPath != null ? folderPath + translationPath : translationPath;

        return new File(SCRIPT_FOLDER, translationPath + ".js");
    }

    public static TranslationFolder getTranslationFolderForUser(Users user, Long folderId) {
        TranslationFolder folder = DbUtils.getTranslationFolder(folderId);

        if(folder == null) {
            throw new NotFoundException();
        }

        if(UserResource.adminUserCheck(user) || user.getId().equals(folder.getUserId()) || folder.getPublicCol()) {
            return folder;
        }
        throw new ForbiddenException("You do not have access to this folder");
    }

    public static Translations getTranslationForUser(Users user, Long translationId, boolean editable) throws WebApplicationException {
        Translations translation = DbUtils.getTranslation(translationId);

        if(translation == null) {
            throw new NotFoundException("No translation with that id exists");
        }

        if(user != null && !isVisible(user, translationId)) {
            throw new ForbiddenException("This translation is not visible to you");
        }

        // For write operations (delete/modify)
        // Check if owner of translation isn't the user, user isn't admin, and there isn't an owner of the translation
        if(editable && user != null && !translation.getUserId().equals(user.getId()) && !UserResource.adminUserCheck(user) && translation.getUserId() != -1) {
            throw new ForbiddenException("You must own the translation to modify it");
        }
        return translation;
    }

    // Check if user is admin, owner of the translation, nobody is owner of translation, or folder that translation is inside is public
    public static boolean isVisible(Users user, Long translationId) {
        Tuple t = createQuery()
            .select(translations.userId, translationFolders.publicCol)
            .from(translations)
            .leftJoin(translationFolders).on(translationFolders.id.eq(translations.folderId))
            .where(translations.id.eq(translationId))
            .fetchFirst();

        Long ownerId = t.get(translations.userId);
        Boolean isPublic = t.get(translationFolders.publicCol);

        if (UserResource.adminUserCheck(user)) return true;

        return user.getId().equals(ownerId) || ownerId == null || isPublic == null || isPublic;
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
                JSONObject jHeader = normalizeHeader((JSONObject) parser.parse(header));
                if (jHeader != null) {
                    script.put("HEADER", jHeader);
                    script.put("BODY", body);
                    return script;
                }
            }
        }

        return null;
    }

    // This is used so that the header for old translations can be lowercase like new translations
    private static JSONObject normalizeHeader(JSONObject headerObj) {
        if (headerObj.get("NAME") != null) {
            headerObj.put("name", headerObj.get("NAME"));
            headerObj.remove("NAME");
        }

        if (headerObj.get("DESCRIPTION") != null) {
            headerObj.put("description", headerObj.get("DESCRIPTION"));
            headerObj.remove("DESCRIPTION");
        }

        if (headerObj.get("CANEXPORT") != null) {
            headerObj.put("canExport", headerObj.get("CANEXPORT"));
            headerObj.remove("CANEXPORT");
        }

        return headerObj;
    }

    public static void scanTranslations() {
        List<File> files;
        ResultSet queryResult;

        try (Connection conn = getConnection(); Statement dbQuery = conn.createStatement()) {
            files = getFilesInScriptDir();

            for (File file: files) {
                String fileName = file.getName();
                fileName = fileName.substring(0, fileName.lastIndexOf(".js"));

                queryResult = dbQuery.executeQuery(String.format("SELECT id FROM translations WHERE display_name = '%s'", fileName));

                if (!queryResult.next()) {
                    Timestamp created = new Timestamp(System.currentTimeMillis());

                    String queryInsert = String.format(
                            "INSERT INTO translations(display_name, user_id, public, created_at, folder_id) " +
                            "VALUES('%s', '%d', '%s', '%s', '%d') ", fileName, -1, true, created, 0
                            );

                    dbQuery.executeUpdate(queryInsert);
                }
            }
        }
        catch (Exception exc) {
            String msg = "Error retrieving files in script directory!";
            throw new WebApplicationException(exc, Response.serverError().entity(msg).build());
        }
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

        String translationPath = File.separator + name;
        translationPath = scriptPath != null ? scriptPath + translationPath : translationPath;

        File fScript = new File(SCRIPT_FOLDER, translationPath + ".js");
        if (!fScript.exists()) {
            if (!fScript.createNewFile()) {
                logger.error("File {} should not have existed before we tried to create it!", fScript.getAbsolutePath());
            }
        }

        JSONObject oHeader = new JSONObject();
        oHeader.put("name", name);
        oHeader.put("description", description);
        oHeader.put("canExport", canExport);

        String header = HEADER_START;
        header += oHeader.toString();
        header += HEADER_END;

        FileUtils.writeStringToFile(fScript, header + content, Charset.defaultCharset());

        logger.debug("Saved script: {}", name);

        return oHeader;
    }

    // This function checks to see if the script has both getDbSchema and
    // translateToOgr which indicates if it can export
    private static boolean validateExport(String script) throws IOException {
        boolean canExport = false;
        org.mozilla.javascript.Context context = org.mozilla.javascript.Context.enter();
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
            org.mozilla.javascript.Context.exit();
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

    public static boolean folderIsPublic(List<TranslationFolder> folders, TranslationFolder f, Users user) {
        // If the user is an admin
        if(UserResource.adminUserCheck(user)) {
            return true;
        }
        // If its public & attached to root (0)
        if(f.getPublicCol() && f.getParentId() != null && f.getParentId().equals(0L)) {
            return true;
        }
        // if we have a user, and folder is private, but its owned by this user:
        if(user != null && !f.getPublicCol() && f.getUserId().equals(user.getId())) {
            return true;
        }

        // Look for this folder's parent in the list:
        TranslationFolder parentFolder = null;
        for(TranslationFolder currentFolder : folders) {
            // if we find it, stop:
            if(f.getParentId() != null && f.getParentId().equals(currentFolder.getId())) {
                parentFolder = currentFolder;
                break;
            }
        }
        // We found the parent in the list, recurse down:
        if(parentFolder != null) {
            return folderIsPublic(folders, parentFolder, user);
        }

        // We did not find the parent, this means that the parent
        // was not visible to us -so- neither should this folder be:
        return false;
    }


}
