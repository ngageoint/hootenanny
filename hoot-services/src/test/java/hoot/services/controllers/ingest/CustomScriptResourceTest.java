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

import static hoot.services.HootProperties.*;
import static org.junit.Assert.*;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.lang.reflect.Method;
import java.net.URL;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Scanner;

import javax.ws.rs.WebApplicationException;
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

import hoot.services.UnitTest;
import hoot.services.testsupport.HootCustomPropertiesSetter;


/*
 * For the save/delete multiple tests, was unable to use the Jersey test container due to
 * BaseMapResource and FileUploadResource use of HttpServletRequest in their Jersey methods.  There
 * is a way around the issue, but was unable to get it to work so far.  So, will remove the Jersey
 * resource from those tests.
 */
public class CustomScriptResourceTest {
    private static File homefolder;
    private static File customScriptFolder;
    private static String original_HOME_FOLDER;
    private static String original_SCRIPT_FOLDER;
    private static String original_JS_HEADER_SCRIPT_PATH;
    private static String original_DEFAULT_TRANSLATIONS_CONFIG;
    private static String original_TRANSLATION_SCRIPT_PATH;


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

        //dummyjsHeaderScriptPath=$(homeFolder)/scripts/empty_rh.js

        original_JS_HEADER_SCRIPT_PATH = JS_HEADER_SCRIPT_PATH;
        URL inputUrl = CustomScriptResourceTest.class.getResource("/hoot/services/controllers/ingest/empty_rh.js");
        File dest = new File(scriptsFolder.getAbsolutePath(), "empty_rh.js");
        FileUtils.copyURLToFile(inputUrl, dest);
        HootCustomPropertiesSetter.setProperty("JS_HEADER_SCRIPT_PATH", dest.getAbsolutePath());

        File confFolder = new File(homefolder, "conf");
        FileUtils.forceMkdir(confFolder);
        assertTrue(confFolder.exists());

        original_DEFAULT_TRANSLATIONS_CONFIG = DEFAULT_TRANSLATIONS_CONFIG;
        inputUrl = CustomScriptResourceTest.class.getResource("/hoot/services/controllers/ingest/DefaultTranslations.json");
        dest = new File(confFolder, "DefaultTranslations.json");
        FileUtils.copyURLToFile(inputUrl, dest);
        HootCustomPropertiesSetter.setProperty("DEFAULT_TRANSLATIONS_CONFIG", dest.getAbsolutePath());

        //translationScriptPath=$(homeFolder)/translations

        original_TRANSLATION_SCRIPT_PATH = TRANSLATION_SCRIPT_PATH;
        File translationsFolder = new File(homefolder, "translations");
        FileUtils.forceMkdir(translationsFolder);
        assertTrue(translationsFolder.exists());

        inputUrl = CustomScriptResourceTest.class.getResource("/hoot/services/controllers/ingest/translations");
        dest = new File(translationsFolder, "TDSv61.js");
        FileUtils.copyURLToFile(inputUrl, dest);

        inputUrl = CustomScriptResourceTest.class.getResource("/hoot/services/controllers/ingest/translations");
        dest = new File(translationsFolder, "TDSv40.js");
        FileUtils.copyURLToFile(inputUrl, dest);

        inputUrl = CustomScriptResourceTest.class.getResource("/hoot/services/controllers/ingest/translations");
        dest = new File(translationsFolder, "MGCP_TRD4.js");
        FileUtils.copyURLToFile(inputUrl, dest);

        inputUrl = CustomScriptResourceTest.class.getResource("/hoot/services/controllers/ingest/translations");
        dest = new File(translationsFolder, "OSM_Ingest.js");
        FileUtils.copyURLToFile(inputUrl, dest);

        inputUrl = CustomScriptResourceTest.class.getResource("/hoot/services/controllers/ingest/translations");
        dest = new File(translationsFolder, "GeoNames.js");
        FileUtils.copyURLToFile(inputUrl, dest);

        HootCustomPropertiesSetter.setProperty("TRANSLATION_SCRIPT_PATH", translationsFolder.getAbsolutePath());
    }

    @AfterClass
    public static void afterClass() throws Exception {
        FileUtils.deleteDirectory(homefolder);
        HootCustomPropertiesSetter.setProperty("HOME_FOLDER", original_HOME_FOLDER);
        HootCustomPropertiesSetter.setProperty("DEFAULT_TRANSLATIONS_CONFIG", original_DEFAULT_TRANSLATIONS_CONFIG);
        HootCustomPropertiesSetter.setProperty("JS_HEADER_SCRIPT_PATH", original_JS_HEADER_SCRIPT_PATH);
        HootCustomPropertiesSetter.setProperty("SCRIPT_FOLDER", original_SCRIPT_FOLDER);
        HootCustomPropertiesSetter.setProperty("TRANSLATION_SCRIPT_PATH", original_TRANSLATION_SCRIPT_PATH);
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
        CustomScriptResource res = new CustomScriptResource();
        Response resp = res.processSave("test", "testName", "Test Description");

        JSONParser parser = new JSONParser();
        JSONArray actualObj = (JSONArray) parser.parse(resp.getEntity().toString());
        JSONArray expectedObj = (JSONArray) parser.parse("[{\"NAME\":\"testName\",\"DESCRIPTION\":\"Test Description\",\"CANEXPORT\":false}]");

        assertEquals(expectedObj, actualObj);
        assertEquals(200, resp.getStatus());

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
            res.processSave("{ test", "testName", "Test Description");
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
        JSONObject expectedObj = (JSONObject) parser.parse("{\"NAME\":\"testName1\",\"DESCRIPTION\":\"Test Description 1\",\"CANEXPORT\":false}");

        String expectedStr = "/*<<<" + expectedObj + ">>>*/" + System.lineSeparator() + "test1";
        String actualStr = FileUtils.readFileToString(file, "UTF-8");
        assertEquals(expectedStr, actualStr);

        assertEquals("testName2", modifiedScripts.get(1));
        file = new File(customScriptFolder, "testName2.js");
        assertTrue(file.exists());

        expectedObj = (JSONObject) parser.parse("{\"NAME\":\"testName2\",\"DESCRIPTION\":\"Test Description 2\",\"CANEXPORT\":false}");

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
        JSONObject expectedObj = (JSONObject) parser.parse("{\"NAME\":\"testName1\",\"DESCRIPTION\":\"Test Description 1\",\"CANEXPORT\":false}");

        String expectedStr = "/*<<<" + expectedObj + ">>>*/" + System.lineSeparator() + "test1";
        String actualStr = FileUtils.readFileToString(file, "UTF-8");
        assertEquals(expectedStr, actualStr);

        assertEquals("testName2", modifiedScripts.get(1));
        file = new File(customScriptFolder, "testName2.js");
        assertTrue(file.exists());

        expectedObj = (JSONObject) parser.parse("{\"NAME\":\"testName2\",\"DESCRIPTION\":\"Test Description 2\",\"CANEXPORT\":false}");

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
        JSONObject expectedObj = (JSONObject) parser.parse("{\"NAME\":\"testName1\",\"DESCRIPTION\":\"Test Description 1\",\"CANEXPORT\":false}");

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
        JSONObject expectedObj = (JSONObject) parser.parse("{\"NAME\":\"testName1\",\"DESCRIPTION\":\"Test Description 1\",\"CANEXPORT\":false}");

        String expectedStr = "/*<<<" + expectedObj + ">>>*/" + System.lineSeparator() + "test1";
        String actualStr = FileUtils.readFileToString(file, "UTF-8");
        assertEquals(expectedStr, actualStr);

        assertEquals("testName2", modifiedScripts.get(1));
        file = new File(customScriptFolder, "testName2.js");
        assertTrue(file.exists());

        expectedObj = (JSONObject) parser.parse("{\"NAME\":\"testName2\",\"DESCRIPTION\":\" \",\"CANEXPORT\":false}");

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
        JSONObject expectedObj = (JSONObject) parser.parse("{\"NAME\":\"testName1\",\"DESCRIPTION\":\"Test Description 1\",\"CANEXPORT\":false}");

        String expectedStr = "/*<<<" + expectedObj + ">>>*/" + System.lineSeparator() + "test1";

        assertEquals(expectedStr, actualStr);

        file = new File(customScriptFolder, "testName2.js");
        assertFalse(file.exists());
    }

    @Test
    @Category(UnitTest.class)
    public void testGetScriptsList() throws Exception {
        CustomScriptResource customScriptResource = new CustomScriptResource();

        Response response = customScriptResource.processSave("test3", "testName3", "Test3 Description");
        assertEquals(200, response.getStatus());

        response = customScriptResource.processSave("test1", "testName4", "Test4 Description");
        assertEquals(200, response.getStatus());

        File file = new File(customScriptFolder, "testName3.js");
        assertTrue(file.exists());

        file = new File(customScriptFolder, "testName4.js");
        assertTrue(file.exists());

        response = customScriptResource.getScriptsList();
        String strList = response.getEntity().toString();

        JSONParser parser = new JSONParser();
        JSONArray arr = (JSONArray) parser.parse(strList);

        assertTrue(!arr.isEmpty());

        for (Object jo : arr) {
            JSONObject o = (JSONObject) jo;
            if (o.get("NAME") != null) {
                if (o.get("NAME").toString().equals("testName3")) {
                    assertTrue(o.get("NAME").toString().startsWith("testName"));
                    assertTrue(o.get("DESCRIPTION").toString().startsWith("Test"));
                }
            }
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testGetScript() throws Exception {
        CustomScriptResource res = new CustomScriptResource();
        Response resp = res.processSave("test5", "testName5", "Test5 Description");

        assertEquals(200, resp.getStatus());
        resp = res.processSave("test6", "testName6", "Test6 Description");
        assertEquals(200, resp.getStatus());

        File file = new File(customScriptFolder, "testName5.js");
        assertTrue(file.exists());

        file = new File(customScriptFolder, "testName6.js");
        assertTrue(file.exists());

        resp = res.getScript("testName5");
        String scriptStr = resp.getEntity().toString();

        assertEquals("test5", scriptStr);

        resp = res.getScript("testName6");
        scriptStr = resp.getEntity().toString();

        assertEquals("test6", scriptStr);
    }

    @Test
    @Category(UnitTest.class)
    public void testDeleteScript() throws Exception {
        CustomScriptResource res = new CustomScriptResource();
        Response resp = res.processSave("test9", "testName9", "Test9 Description");
        assertEquals(200, resp.getStatus());

        File file = new File(customScriptFolder, "testName9.js");
        assertTrue(file.exists());

        resp = res.deleteScript("testName9");
        String deletedStr = resp.getEntity().toString();

        JSONParser parser = new JSONParser();
        JSONArray actualObj = (JSONArray) parser.parse(deletedStr);
        JSONArray expectedObj = (JSONArray) parser.parse("[{\"NAME\":\"testName9\",\"DESCRIPTION\":\"Test9 Description\",\"CANEXPORT\":false}]");

        assertEquals(expectedObj, actualObj);
    }

    @Test
    @Category(UnitTest.class)
    public void testDeleteMultiple() {
        CustomScriptResource res = new CustomScriptResource();
        Response saveResponse = res.processSave("test9", "testName9", "Test9 Description");
        assertEquals(200, saveResponse.getStatus());
        File file = new File(customScriptFolder, "testName9.js");
        assertTrue(file.exists());

        saveResponse = res.processSave("test10", "testName10", "Test10 Description");
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
        CustomScriptResource res = new CustomScriptResource();
        Response saveResponse = res.processSave("test9", "testName9", "Test9 Description");
        assertEquals(200, saveResponse.getStatus());
        File file = new File(customScriptFolder, "testName9.js");
        assertTrue(file.exists());

        saveResponse = res.processSave("test10", "testName10", "Test10 Description");
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

        ScriptsModifiedResponse deleteResponse = res.deleteScripts(request);

        assertEquals(1, deleteResponse.getScriptsModified().length);
        assertEquals("testName9", deleteResponse.getScriptsModified()[0]);
        file = new File(customScriptFolder, "testName9.js");
        assertFalse(file.exists());

        file = new File(customScriptFolder, "testName10.js");
        assertTrue(file.exists());
    }

    /**
     * Removes the first line from a file
     *
     * @param file
     *            file to modify
     * @throws IOException
     */
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
        CustomScriptResource res = new CustomScriptResource();
        Response saveResponse = res.processSave("test9", "testName9", "Test9 Description");
        assertEquals(200, saveResponse.getStatus());
        File file = new File(customScriptFolder, "testName9.js");
        assertTrue(file.exists());

        saveResponse = res.processSave("test10", "testName10", "Test10 Description");
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

            Object oName = jsTrans.get("NAME");
            assertNotNull(oName);
            assertTrue(!oName.toString().isEmpty());
            assertNotNull(jsTrans.get("DESCRIPTION"));

            Object oPath = jsTrans.get("PATH");
            assertNotNull(oPath);
            assertTrue(!oPath.toString().isEmpty());

            File fScript = new File(homefolder, (String) oPath);
            assertTrue(fScript.exists());

            String sScript = FileUtils.readFileToString(fScript);
            validateExportMethod.invoke(null, sScript);

            if (jsTrans.get("FOUO_PATH") != null) {
                File fouo = new File(homefolder, (String) jsTrans.get("FOUO_PATH"));
                assertTrue(fouo.exists());
            }
        }
    }
}
