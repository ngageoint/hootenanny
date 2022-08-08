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
import static org.junit.Assert.*;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.lang.reflect.Method;
import java.nio.charset.Charset;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Scanner;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.client.Entity;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;

import org.apache.commons.io.FileUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.junit.runner.RunWith;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.support.AnnotationConfigContextLoader;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.UnitTest;
import hoot.services.jerseyframework.HootServicesJerseyTestAbstract;
import hoot.services.jerseyframework.HootServicesSpringTestConfig;
import hoot.services.utils.HootCustomPropertiesSetter;


/*
 * For the save/delete multiple tests, was unable to use the Jersey test container due to
 * BaseMapResource and FileUploadResource use of HttpServletRequest in their Jersey methods.  There
 * is a way around the issue, but was unable to get it to work so far.  So, will remove the Jersey
 * resource from those tests.
 */
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = HootServicesSpringTestConfig.class, loader = AnnotationConfigContextLoader.class)
@Transactional
public class CustomScriptResourceTest extends HootServicesJerseyTestAbstract {
    private static File homefolder;
    private static File customScriptFolder;
    private static String original_HOME_FOLDER;
    private static String original_SCRIPT_FOLDER;


    @BeforeClass
    public static void beforeClass() throws Exception {
        original_HOME_FOLDER = HOME_FOLDER;
        homefolder = new File(FileUtils.getTempDirectory(), "CustomScriptResourceTest");
        FileUtils.forceMkdir(homefolder);
        assertTrue(homefolder.exists());
        HootCustomPropertiesSetter.setProperty("HOME_FOLDER", homefolder.getAbsolutePath());

        original_SCRIPT_FOLDER = SCRIPT_FOLDER;
        customScriptFolder = new File(homefolder, "customscript");
        FileUtils.forceMkdir(customScriptFolder);
        assertTrue(customScriptFolder.exists());
        HootCustomPropertiesSetter.setProperty("SCRIPT_FOLDER", customScriptFolder.getAbsolutePath());

        File scriptsFolder = new File(homefolder, "scripts");
        FileUtils.forceMkdir(scriptsFolder);
        assertTrue(scriptsFolder.exists());
    }

    @AfterClass
    public static void afterClass() throws Exception {
        FileUtils.deleteDirectory(homefolder);
        HootCustomPropertiesSetter.setProperty("HOME_FOLDER", original_HOME_FOLDER);
        HootCustomPropertiesSetter.setProperty("SCRIPT_FOLDER", original_SCRIPT_FOLDER);
    }

    @Before
    public void beforeTest() {}

    @After
    public void afterTest() throws IOException {
        if (customScriptFolder.exists()) {
            FileUtils.cleanDirectory(customScriptFolder);
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testProcessSave() throws Exception {
        Response response = target("/customscript/save")
                .queryParam("scriptName", "testName")
                .queryParam("scriptDescription", "Test Description")
                .queryParam("folderId", 0)
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.entity("test", MediaType.TEXT_PLAIN), Response.class);

        JSONParser parser = new JSONParser();
        JSONArray actualObj = (JSONArray) parser.parse(response.readEntity(String.class));
        JSONArray expectedObj = (JSONArray) parser.parse("[{\"name\":\"testName\",\"description\":\"Test Description\",\"canExport\":false}]");

        assertEquals(expectedObj, actualObj);
        assertEquals(200, response.getStatus());

        File file = new File(customScriptFolder + "/" + "testName.js");
        assertTrue(file.exists());

        String expectedContent = "/*<<<" + actualObj.get(0) + ">>>*/" + System.lineSeparator() + "test";
        String actualContent = FileUtils.readFileToString(file, "UTF-8");

        assertEquals(expectedContent, actualContent);
    }

    @Test
    @Category(UnitTest.class)
    public void testSaveBadSyntax() throws Exception {
        CustomScriptResource res = new CustomScriptResource();
        try {
            Response response = target("/customscript/save")
                    .queryParam("scriptName", "testName")
                    .queryParam("scriptDescription", "Test Description")
                    .queryParam("folderId", 0)
                    .request(MediaType.APPLICATION_JSON)
                    .post(Entity.entity("{ test", MediaType.TEXT_PLAIN), Response.class);
        }
        catch (WebApplicationException e) {
            Response response = e.getResponse();
            assertEquals(Status.INTERNAL_SERVER_ERROR.getStatusCode(), response.getStatus());
            //Assert.assertTrue(response.readEntity(String.class).contains("missing } in compound statement"));
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testSaveMultiple() throws Exception {
        ModifyScriptsRequest request = new ModifyScriptsRequest();
        request.setScripts(new Script[] { new Script("testName1", "Test Description 1", "test1"),
                                          new Script("testName2", "Test Description 2", "test2") });

        CustomScriptResource res = new CustomScriptResource();
        ScriptsModifiedResponse response = res.saveScripts(request);

        assertEquals(2, response.getScriptsModified().length);

        List<String> modifiedScripts = Arrays.asList(response.getScriptsModified());
        Collections.sort(modifiedScripts);

        assertEquals("testName1", modifiedScripts.get(0));
        File file = new File(customScriptFolder, "testName1.js");
        assertTrue(file.exists());

        JSONParser parser = new JSONParser();
        JSONObject expectedObj = (JSONObject) parser.parse("{\"name\":\"testName1\",\"description\":\"Test Description 1\",\"canExport\":false}");

        String expectedStr = "/*<<<" + expectedObj + ">>>*/" + System.lineSeparator() + "test1";
        String actualStr = FileUtils.readFileToString(file, "UTF-8");
        assertEquals(expectedStr, actualStr);

        assertEquals("testName2", modifiedScripts.get(1));
        file = new File(customScriptFolder, "testName2.js");
        assertTrue(file.exists());

        expectedObj = (JSONObject) parser.parse("{\"name\":\"testName2\",\"description\":\"Test Description 2\",\"canExport\":false}");

        expectedStr = "/*<<<" + expectedObj + ">>>*/" + System.lineSeparator() + "test2";
        actualStr = FileUtils.readFileToString(file, "UTF-8");
        assertEquals(expectedStr, actualStr);
    }

    @Test
    @Category(UnitTest.class)
    public void testSaveMultipleDirDoesntExist() throws Exception {
        /*
         * The upload directory should be created if it doesn't exist.
         */

        FileUtils.deleteDirectory(customScriptFolder);

        ModifyScriptsRequest request = new ModifyScriptsRequest();
        request.setScripts(new Script[] { new Script("testName1", "Test Description 1", "test1"),
                new Script("testName2", "Test Description 2", "test2") });

        CustomScriptResource res = new CustomScriptResource();
        ScriptsModifiedResponse response = res.saveScripts(request);

        assertEquals(2, response.getScriptsModified().length);

        List<String> modifiedScripts = Arrays.asList(response.getScriptsModified());
        Collections.sort(modifiedScripts);

        assertEquals("testName1", modifiedScripts.get(0));
        File file = new File(customScriptFolder, "testName1.js");
        assertTrue(file.exists());

        JSONParser parser = new JSONParser();
        JSONObject expectedObj = (JSONObject) parser.parse("{\"name\":\"testName1\",\"description\":\"Test Description 1\",\"canExport\":false}");

        String expectedStr = "/*<<<" + expectedObj + ">>>*/" + System.lineSeparator() + "test1";
        String actualStr = FileUtils.readFileToString(file, "UTF-8");
        assertEquals(expectedStr, actualStr);

        assertEquals("testName2", modifiedScripts.get(1));
        file = new File(customScriptFolder, "testName2.js");
        assertTrue(file.exists());

        expectedObj = (JSONObject) parser.parse("{\"name\":\"testName2\",\"description\":\"Test Description 2\",\"canExport\":false}");

        expectedStr = "/*<<<" + expectedObj + ">>>*/" + System.lineSeparator() + "test2";
        actualStr = FileUtils.readFileToString(file, "UTF-8");
        assertEquals(expectedStr, actualStr);

        actualStr = FileUtils.readFileToString(file, "UTF-8");
        assertEquals(expectedStr, actualStr);
    }

    @Test
    @Category(UnitTest.class)
    public void testSaveMultipleEmptyName() throws Exception {
        /*
         * The server will just log that an error occurred when saving the
         * script with no name rather than erroring out of the operation. This
         * seems better to me than failing in the middle of a batch. However,
         * the client will need to match the input saved scripts against the
         * saved scripts listed in the response.
         */

        ModifyScriptsRequest request = new ModifyScriptsRequest();
        request.setScripts(new Script[] { new Script("testName1", "Test Description 1", "test1"),
                new Script(" ", "Test Description 2", "test2") });

        CustomScriptResource res = new CustomScriptResource();
        ScriptsModifiedResponse response = res.saveScripts(request);

        assertEquals(1, response.getScriptsModified().length);

        assertEquals("testName1", response.getScriptsModified()[0]);
        File file = new File(customScriptFolder, "testName1.js");
        assertTrue(file.exists());

        JSONParser parser = new JSONParser();
        JSONObject expectedObj = (JSONObject) parser.parse("{\"name\":\"testName1\",\"description\":\"Test Description 1\",\"canExport\":false}");

        String expectedStr = "/*<<<" + expectedObj + ">>>*/" + System.lineSeparator() + "test1";
        String actualStr = FileUtils.readFileToString(file, "UTF-8");
        assertEquals(expectedStr, actualStr);

        file = new File(customScriptFolder + "/" + "testName2.js");
        assertFalse(file.exists());
    }

    @Test
    @Category(UnitTest.class)
    public void testSaveMultipleEmptyDescription() throws Exception {
        /*
         * description is optional
         */

        ModifyScriptsRequest request = new ModifyScriptsRequest();
        request.setScripts(new Script[] { new Script("testName1", "Test Description 1", "test1"),
                                          new Script("testName2", " ", "test2") });

        CustomScriptResource res = new CustomScriptResource();
        ScriptsModifiedResponse response = res.saveScripts(request);

        assertEquals(2, response.getScriptsModified().length);

        List<String> modifiedScripts = Arrays.asList(response.getScriptsModified());
        Collections.sort(modifiedScripts);

        assertEquals("testName1", modifiedScripts.get(0));
        File file = new File(customScriptFolder, "testName1.js");
        assertTrue(file.exists());

        JSONParser parser = new JSONParser();
        JSONObject expectedObj = (JSONObject) parser.parse("{\"name\":\"testName1\",\"description\":\"Test Description 1\",\"canExport\":false}");

        String expectedStr = "/*<<<" + expectedObj + ">>>*/" + System.lineSeparator() + "test1";
        String actualStr = FileUtils.readFileToString(file, "UTF-8");
        assertEquals(expectedStr, actualStr);

        assertEquals("testName2", modifiedScripts.get(1));
        file = new File(customScriptFolder, "testName2.js");
        assertTrue(file.exists());

        expectedObj = (JSONObject) parser.parse("{\"name\":\"testName2\",\"description\":\" \",\"canExport\":false}");

        expectedStr = "/*<<<" + expectedObj + ">>>*/" + System.lineSeparator() + "test2";
        actualStr = FileUtils.readFileToString(file, "UTF-8");
        assertEquals(expectedStr, actualStr);
    }

    @Test
    @Category(UnitTest.class)
    public void testSaveMultipleEmptyContent() throws Exception {
        /*
         * See explanation in testSaveMultipleEmptyName for why no error occurs here
         */
        ModifyScriptsRequest request = new ModifyScriptsRequest();
        request.setScripts(new Script[] { new Script("testName1", "Test Description 1", "test1"),
                                          new Script("testName2", "Test Description 2", " ") });

        CustomScriptResource res = new CustomScriptResource();
        ScriptsModifiedResponse response = res.saveScripts(request);

        assertEquals(1, response.getScriptsModified().length);
        assertEquals("testName1", response.getScriptsModified()[0]);

        File file = new File(customScriptFolder, "testName1.js");
        assertTrue(file.exists());

        JSONParser parser = new JSONParser();
        String actualStr = FileUtils.readFileToString(file, "UTF-8");
        JSONObject expectedObj = (JSONObject) parser.parse("{\"name\":\"testName1\",\"description\":\"Test Description 1\",\"canExport\":false}");

        String expectedStr = "/*<<<" + expectedObj + ">>>*/" + System.lineSeparator() + "test1";

        assertEquals(expectedStr, actualStr);

        file = new File(customScriptFolder, "testName2.js");
        assertFalse(file.exists());
    }

    @Test
    @Category(UnitTest.class)
    public void testGetScriptsList() throws Exception {
        Response response = target("/customscript/save")
                .queryParam("scriptName", "testName3")
                .queryParam("scriptDescription", "Test3 Description")
                .queryParam("folderId", 0)
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.entity("test3", MediaType.TEXT_PLAIN), Response.class);
        assertEquals(200, response.getStatus());

        response = target("/customscript/save")
                .queryParam("scriptName", "testName4")
                .queryParam("scriptDescription", "Test4 Description")
                .queryParam("folderId", 0)
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.entity("test1", MediaType.TEXT_PLAIN), Response.class);
        assertEquals(200, response.getStatus());

        File file = new File(customScriptFolder, "testName3.js");
        assertTrue(file.exists());

        file = new File(customScriptFolder, "testName4.js");
        assertTrue(file.exists());

        response = target("/customscript/getlist")
                .request(MediaType.APPLICATION_JSON)
                .get();
        String strList = response.readEntity(String.class);

        JSONParser parser = new JSONParser();
        JSONArray arr = (JSONArray) parser.parse(strList);

        assertTrue(!arr.isEmpty());

        for (Object jo : arr) {
            JSONObject o = (JSONObject) jo;
            if (o.get("name") != null) {
                if (o.get("name").toString().equals("testName3")) {
                    assertTrue(o.get("name").toString().startsWith("testName"));
                    assertTrue(o.get("description").toString().startsWith("Test"));
                }
            }
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testGetScript() throws Exception {
        CustomScriptResource res = new CustomScriptResource();

        Response resp = target("/customscript/save")
                .queryParam("scriptName", "testName5")
                .queryParam("scriptDescription", "Test5 Description")
                .queryParam("folderId", 0)
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.entity("test5", MediaType.TEXT_PLAIN), Response.class);
        assertEquals(200, resp.getStatus());

        resp = target("/customscript/save")
                .queryParam("scriptName", "testName6")
                .queryParam("scriptDescription", "Test6 Description")
                .queryParam("folderId", 0)
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.entity("test6", MediaType.TEXT_PLAIN), Response.class);
        assertEquals(200, resp.getStatus());

        File file = new File(customScriptFolder, "testName5.js");
        assertTrue(file.exists());

        file = new File(customScriptFolder, "testName6.js");
        assertTrue(file.exists());

        resp = target("/customscript/getscript")
                .queryParam("scriptName", "testName5")
                .request(MediaType.TEXT_PLAIN)
                .get();
        String scriptStr = resp.readEntity(String.class);

        assertEquals("test5", scriptStr);

        resp = target("/customscript/getscript")
                .queryParam("scriptName", "testName6")
                .request(MediaType.TEXT_PLAIN)
                .get();
        scriptStr = resp.readEntity(String.class);

        assertEquals("test6", scriptStr);
    }

    @Test
    @Category(UnitTest.class)
    public void testDeleteScript() throws Exception {
        Response resp = target("/customscript/save")
                .queryParam("scriptName", "testName9")
                .queryParam("scriptDescription", "Test9 Description")
                .queryParam("folderId", 0)
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.entity("test9", MediaType.TEXT_PLAIN), Response.class);
        assertEquals(200, resp.getStatus());

        File file = new File(customScriptFolder, "testName9.js");
        assertTrue(file.exists());

        resp = target("/customscript/deletescript")
                .queryParam("scriptInfo", "testName9")
                .request(MediaType.APPLICATION_JSON)
                .get();
        String deletedStr = resp.readEntity(String.class);

        JSONParser parser = new JSONParser();
        JSONArray actualObj = (JSONArray) parser.parse(deletedStr);
        JSONArray expectedObj = (JSONArray) parser.parse("[{\"name\":\"testName9\",\"description\":\"Test9 Description\",\"canExport\":false}]");

        assertEquals(expectedObj, actualObj);
    }

    @Test
    @Category(UnitTest.class)
    public void testDeleteMultiple() throws SQLException {
        Response saveResponse = target("/customscript/save")
                .queryParam("scriptName", "testName9")
                .queryParam("scriptDescription", "Test9 Description")
                .queryParam("folderId", 0)
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.entity("test9", MediaType.TEXT_PLAIN), Response.class);
        assertEquals(200, saveResponse.getStatus());
        File file = new File(customScriptFolder, "testName9.js");
        assertTrue(file.exists());

        saveResponse = target("/customscript/save")
                .queryParam("scriptName", "testName10")
                .queryParam("scriptDescription", "Test10 Description")
                .queryParam("folderId", 0)
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.entity("test10", MediaType.TEXT_PLAIN), Response.class);
        assertEquals(200, saveResponse.getStatus());
        file = new File(customScriptFolder, "testName10.js");
        assertTrue(file.exists());

        ModifyScriptsRequest request = new ModifyScriptsRequest();
        List<Script> scriptsToDelete = new ArrayList<>();
        Script script = new Script("testName9", "Test9 Description", "test9");
        scriptsToDelete.add(script);
        script = new Script("testName10", "Test10 Description", "test10");
        scriptsToDelete.add(script);
        request.setScripts(scriptsToDelete.toArray(new Script[scriptsToDelete.size()]));

        CustomScriptResource res = new CustomScriptResource();
        ScriptsModifiedResponse deleteResponse = res.deleteScripts(request);

        assertEquals(2, deleteResponse.getScriptsModified().length);
        List<String> modifiedScripts = Arrays.asList(deleteResponse.getScriptsModified());
        Collections.sort(modifiedScripts);
        assertEquals("testName10", modifiedScripts.get(0));
        file = new File(customScriptFolder, "testName10.js");
        assertFalse(file.exists());
        assertEquals("testName9", modifiedScripts.get(1));
        file = new File(customScriptFolder, "testName9.js");
        assertFalse(file.exists());
    }

    @Test
    @Category(UnitTest.class)
    public void testDeleteMultipleScriptsDirDoesntExist() throws IOException {
        /*
         * See explanation in testSaveMultipleEmptyName why no failure occurs here
         */
        ModifyScriptsRequest request = new ModifyScriptsRequest();
        List<Script> scriptsToDelete = new ArrayList<>();
        Script script = new Script("testName9", "Test9 Description", "test9");
        scriptsToDelete.add(script);
        script = new Script("testName10", "Test10 Description", "test10");
        scriptsToDelete.add(script);
        request.setScripts(scriptsToDelete.toArray(new Script[scriptsToDelete.size()]));

        FileUtils.deleteDirectory(customScriptFolder);

        CustomScriptResource res = new CustomScriptResource();
        ScriptsModifiedResponse deleteResponse = res.deleteScripts(request);
        assertEquals(0, deleteResponse.getScriptsModified().length);
    }

    @Test
    @Category(UnitTest.class)
    public void testDeleteMultipleScriptToBeDeletedDoesntExist() throws Exception {
        /*
         * See explanation in testSaveMultipleEmptyName why no failure occurs here
         */
        Response saveResponse = target("/customscript/save")
                .queryParam("scriptName", "testName9")
                .queryParam("scriptDescription", "Test9 Description")
                .queryParam("folderId", 0)
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.entity("test9", MediaType.TEXT_PLAIN), Response.class);
        assertEquals(200, saveResponse.getStatus());
        File file = new File(customScriptFolder, "testName9.js");
        assertTrue(file.exists());

        saveResponse = target("/customscript/save")
                .queryParam("scriptName", "testName10")
                .queryParam("scriptDescription", "Test10 Description")
                .queryParam("folderId", 0)
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.entity("test10", MediaType.TEXT_PLAIN), Response.class);
        assertEquals(200, saveResponse.getStatus());
        file = new File(customScriptFolder + "/" + "testName10.js");
        assertTrue(file.exists());

        ModifyScriptsRequest request = new ModifyScriptsRequest();
        List<Script> scriptsToDelete = new ArrayList<>();
        Script script = new Script("testName9", "Test9 Description", "test9");
        scriptsToDelete.add(script);
        // this doesn't exist
        script = new Script("testName11", "Test11 Description", "test11");
        scriptsToDelete.add(script);
        request.setScripts(scriptsToDelete.toArray(new Script[scriptsToDelete.size()]));

        CustomScriptResource res = new CustomScriptResource();
        ScriptsModifiedResponse deleteResponse = res.deleteScripts(request);

        assertEquals(1, deleteResponse.getScriptsModified().length);
        assertEquals("testName9", deleteResponse.getScriptsModified()[0]);
        file = new File(customScriptFolder, "testName9.js");
        assertFalse(file.exists());

        file = new File(customScriptFolder, "testName10.js");
        assertTrue(file.exists());
    }

    private static void removeFirstLineFromFile(File file) throws IOException {
        try (Scanner fileScanner = new Scanner(file)) {
            fileScanner.nextLine();
            try (FileWriter fileStream = new FileWriter(file.getAbsolutePath())) {
                try (BufferedWriter out = new BufferedWriter(fileStream)) {
                    while (fileScanner.hasNextLine()) {
                        String next = fileScanner.nextLine();
                        if (next.equals(System.lineSeparator())) {
                            out.newLine();
                        }
                        else {
                            out.write(next);
                        }

                        out.newLine();
                    }
                }
            }
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testDeleteMultipleScriptToBeDeletedHasNoHeader() throws Exception {
        Response saveResponse = target("/customscript/save")
                .queryParam("scriptName", "testName9")
                .queryParam("scriptDescription", "Test9 Description")
                .queryParam("folderId", 0)
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.entity("test9", MediaType.TEXT_PLAIN), Response.class);
        assertEquals(200, saveResponse.getStatus());
        File file = new File(customScriptFolder, "testName9.js");
        assertTrue(file.exists());

        saveResponse = target("/customscript/save")
                .queryParam("scriptName", "testName10")
                .queryParam("scriptDescription", "Test10 Description")
                .queryParam("folderId", 0)
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.entity("test10", MediaType.TEXT_PLAIN), Response.class);
        assertEquals(200, saveResponse.getStatus());
        file = new File(customScriptFolder, "testName10.js");
        assertTrue(file.exists());

        // remove the header (on the first line) from the second script
        removeFirstLineFromFile(file);

        String content = FileUtils.readFileToString(file, "UTF-8");
        assertFalse(content.contains("*<<<"));
        assertFalse(content.contains(">>>*"));

        ModifyScriptsRequest request = new ModifyScriptsRequest();
        List<Script> scriptsToDelete = new ArrayList<>();
        Script script = new Script("testName9", "Test9 Description", "test9");
        scriptsToDelete.add(script);
        script = new Script("testName10", "Test10 Description", "test10");
        scriptsToDelete.add(script);
        request.setScripts(scriptsToDelete.toArray(new Script[scriptsToDelete.size()]));

        CustomScriptResource res = new CustomScriptResource();
        ScriptsModifiedResponse deleteResponse = res.deleteScripts(request);

        assertEquals(1, deleteResponse.getScriptsModified().length);
        assertEquals("testName9", deleteResponse.getScriptsModified()[0]);
        file = new File(customScriptFolder, "testName9.js");
        assertFalse(file.exists());
        file = new File(customScriptFolder, "testName10.js");
        assertTrue(file.exists());
    }

    @Test
    @Category(UnitTest.class)
    public void testValidateExport() throws Exception {
        Method validateExportMethod = CustomScriptResource.class.getDeclaredMethod("validateExport", String.class);
        validateExportMethod.setAccessible(true);

        String noExportScript = "";
        noExportScript += "function translateToOsm(attrs, layerName){}";
        boolean canExport = (Boolean) validateExportMethod.invoke(null, noExportScript);
        assertTrue(!canExport);

        String exportScript = "";
        exportScript += "function getDbSchema(){}";
        exportScript += "function translateToOgr(){}";
        canExport = (Boolean) validateExportMethod.invoke(null, exportScript);
        assertTrue(canExport);
    }

    @Test
    @Category(UnitTest.class)
    public void testGetDefaultList() throws Exception {
        // Try to test current DefaultTranslations.json setup and the mechanism.
        // The value in DefaultTranslations.json should be valid at build time..

        Method validateExportMethod = CustomScriptResource.class.getDeclaredMethod("validateExport", String.class);
        validateExportMethod.setAccessible(true);

        Method getDefaultListMethod = CustomScriptResource.class.getDeclaredMethod("getDefaultList", List.class);
        getDefaultListMethod.setAccessible(true);

        List<String> configFiles = new ArrayList<>();
        configFiles.add(DEFAULT_TRANSLATIONS_CONFIG);
        JSONArray trans = (JSONArray) getDefaultListMethod.invoke(null, configFiles);

        for (Object o : trans) {
            JSONObject jsTrans = (JSONObject) o;

            Object oName = jsTrans.get("name");
            assertNotNull(oName);
            assertTrue(!oName.toString().isEmpty());
            assertNotNull(jsTrans.get("description"));

            if (jsTrans.containsKey("path")) {
                Object oPath = jsTrans.get("path");
                assertNotNull(oPath);
                assertTrue(!oPath.toString().isEmpty());

                File fScript = new File(original_HOME_FOLDER, (String) oPath);
                assertTrue(fScript.exists());

                String sScript = FileUtils.readFileToString(fScript, Charset.defaultCharset());
                validateExportMethod.invoke(null, sScript);
            }
            else {
                if (jsTrans.containsKey("importPath")) {
                    Object oPath = jsTrans.get("importPath");
                    assertNotNull(oPath);
                    assertTrue(!oPath.toString().isEmpty());

                    File fScript = new File(original_HOME_FOLDER, (String) oPath);
                    assertTrue(fScript.exists());

                    String sScript = FileUtils.readFileToString(fScript, Charset.defaultCharset());
                    validateExportMethod.invoke(null, sScript);
                }

                if (jsTrans.containsKey("exportPath")) {
                    Object oPath = jsTrans.get("exportPath");
                    assertNotNull(oPath);
                    assertTrue(!oPath.toString().isEmpty());

                    File fScript = new File(original_HOME_FOLDER, (String) oPath);
                    assertTrue(fScript.exists());

                    String sScript = FileUtils.readFileToString(fScript, Charset.defaultCharset());
                    validateExportMethod.invoke(null, sScript);
                }
            }

            if (jsTrans.get("fouoPath") != null) {
                File fouo = new File(original_HOME_FOLDER, (String) jsTrans.get("fouoPath"));
                assertTrue(fouo.exists());
            }
        }
    }
}
