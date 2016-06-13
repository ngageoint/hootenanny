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

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;

import org.apache.commons.io.FileUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.junit.After;
import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.HootProperties;
import hoot.services.UnitTest;
import hoot.services.ingest.ModifyScriptsRequest;
import hoot.services.ingest.Script;
import hoot.services.ingest.ScriptsModifiedResponse;


/*
 * For the save/delete multiple tests, was unable to use the Jersey test container due to
 * BaseMapResource and FileUploadResource use of HttpServletRequest in their Jersey methods.  There
 * is a way around the issue, but was unable to get it to work so far.  So, will remove the Jersey
 * resource from those tests.
 */
public class CustomScriptResourceTest {
    private JSONParser _parser = new JSONParser();
    private static String homefolder = null;
    private static CustomScriptResource res = new CustomScriptResource();

    @BeforeClass
    public static void oneTimeSetup() throws IOException {
        // one-time setup code
        res.scriptFolder += "/test";
        String jsHeaderScriptPath = HootProperties.getProperty("dummyjsHeaderScriptPath");
        Assert.assertNotNull(jsHeaderScriptPath);
        File f = new File(jsHeaderScriptPath);
        Assert.assertTrue(f.exists());

        String defaultTranslationsConfig = HootProperties.getProperty("defaultTranslationsConfig");
        Assert.assertNotNull(defaultTranslationsConfig);
        f = new File(defaultTranslationsConfig);
        Assert.assertTrue(f.exists());

        homefolder = HootProperties.getProperty("homeFolder");
        Assert.assertNotNull(homefolder);
        f = new File(homefolder);
        Assert.assertTrue(f.exists());
    }

    @After
    public void afterTest() throws IOException {
        File scriptFolder = new File(res.scriptFolder);
        if (scriptFolder.exists()) {
            FileUtils.cleanDirectory(scriptFolder);
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testProcessSave() throws Exception {
        Response resp = res.processSave("test", "testName", "Test Description");

        String savedStr = resp.getEntity().toString();

        Assert.assertEquals("[{\"NAME\":\"testName\",\"DESCRIPTION\":\"Test Description\",\"CANEXPORT\":false}]", savedStr);

        Assert.assertEquals(200, resp.getStatus());
        File f = new File(res.scriptFolder + "/" + "testName.js");
        Assert.assertTrue(f.exists());

        String resStr = "/*<<<{\"NAME\":\"testName\",\"DESCRIPTION\":\"Test Description\",\"CANEXPORT\":false}>>>*/\n"
                + "test";
        String content = FileUtils.readFileToString(f, "UTF-8");
        Assert.assertEquals(content, resStr);
    }

    @Test
    @Category(UnitTest.class)
    public void testSaveBadSyntax() throws Exception {
        try {
            res.processSave("{ test", "testName", "Test Description");
        }
        catch (WebApplicationException e) {
            Response res = e.getResponse();
            Assert.assertEquals(Status.INTERNAL_SERVER_ERROR.getStatusCode(), res.getStatus());
            Assert.assertTrue(res.getEntity().toString().contains("missing } in compound statement"));
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testSaveMultiple() throws IOException {
        ModifyScriptsRequest request = new ModifyScriptsRequest();
        request.setScripts(new Script[] { new Script("testName1", "Test Description 1", "test1"),
                new Script("testName2", "Test Description 2", "test2") });

        ScriptsModifiedResponse response = res.saveScripts(request);

        Assert.assertEquals(2, response.getScriptsModified().length);

        List<String> modifiedScripts = Arrays.asList(response.getScriptsModified());
        Collections.sort(modifiedScripts);

        Assert.assertEquals("testName1", modifiedScripts.get(0));
        File f = new File(res.scriptFolder + "/" + "testName1.js");
        Assert.assertTrue(f.exists());
        String resStr = "/*<<<{\"NAME\":\"testName1\",\"DESCRIPTION\":\"Test Description 1\",\"CANEXPORT\":false}>>>*/\n"
                + "test1";
        String content = FileUtils.readFileToString(f, "UTF-8");
        Assert.assertTrue(content.equals(resStr));

        Assert.assertEquals("testName2", modifiedScripts.get(1));
        f = new File(res.scriptFolder + "/" + "testName2.js");
        Assert.assertTrue(f.exists());
        resStr = "/*<<<{\"NAME\":\"testName2\",\"DESCRIPTION\":\"Test Description 2\",\"CANEXPORT\":false}>>>*/\n"
                + "test2";
        content = FileUtils.readFileToString(f, "UTF-8");
        Assert.assertTrue(content.equals(resStr));
    }

    @Test
    @Category(UnitTest.class)
    public void testSaveMultipleDirDoesntExist() throws Exception {
        /*
         * The upload directory should be created if it doesn't exist.
         */

        FileUtils.deleteDirectory(new File(res.scriptFolder));

        ModifyScriptsRequest request = new ModifyScriptsRequest();
        request.setScripts(new Script[] { new Script("testName1", "Test Description 1", "test1"),
                new Script("testName2", "Test Description 2", "test2") });

        ScriptsModifiedResponse response = res.saveScripts(request);

        Assert.assertEquals(2, response.getScriptsModified().length);

        List<String> modifiedScripts = Arrays.asList(response.getScriptsModified());
        Collections.sort(modifiedScripts);

        Assert.assertEquals("testName1", modifiedScripts.get(0));
        File f = new File(res.scriptFolder + "/" + "testName1.js");
        Assert.assertTrue(f.exists());
        String resStr = "/*<<<{\"NAME\":\"testName1\",\"DESCRIPTION\":\"Test Description 1\",\"CANEXPORT\":false}>>>*/\n"
                + "test1";
        String content = FileUtils.readFileToString(f, "UTF-8");
        Assert.assertTrue(content.equals(resStr));

        Assert.assertEquals("testName2", modifiedScripts.get(1));
        f = new File(res.scriptFolder + "/" + "testName2.js");
        Assert.assertTrue(f.exists());
        resStr = "/*<<<{\"NAME\":\"testName2\",\"DESCRIPTION\":\"Test Description 2\",\"CANEXPORT\":false}>>>*/\n"
                + "test2";
        content = FileUtils.readFileToString(f, "UTF-8");
        Assert.assertTrue(content.equals(resStr));

        // PowerMock.verify(resource);
    }

    @Test
    @Category(UnitTest.class)
    public void testSaveMultipleEmptyName() throws IOException {
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

        ScriptsModifiedResponse response = res.saveScripts(request);

        Assert.assertEquals(1, response.getScriptsModified().length);

        Assert.assertEquals("testName1", response.getScriptsModified()[0]);
        File f = new File(res.scriptFolder + "/" + "testName1.js");
        Assert.assertTrue(f.exists());
        String resStr = "/*<<<{\"NAME\":\"testName1\",\"DESCRIPTION\":\"Test Description 1\",\"CANEXPORT\":false}>>>*/\n"
                + "test1";
        String content = FileUtils.readFileToString(f, "UTF-8");
        Assert.assertTrue(content.equals(resStr));

        f = new File(res.scriptFolder + "/" + "testName2.js");
        Assert.assertFalse(f.exists());
    }

    @Test
    @Category(UnitTest.class)
    public void testSaveMultipleEmptyDescription() throws IOException {
        /*
         * description is optional
         */

        ModifyScriptsRequest request = new ModifyScriptsRequest();
        request.setScripts(new Script[] { new Script("testName1", "Test Description 1", "test1"),
                new Script("testName2", " ", "test2") });

        ScriptsModifiedResponse response = res.saveScripts(request);

        Assert.assertEquals(2, response.getScriptsModified().length);

        List<String> modifiedScripts = Arrays.asList(response.getScriptsModified());
        Collections.sort(modifiedScripts);

        Assert.assertEquals("testName1", modifiedScripts.get(0));
        File f = new File(res.scriptFolder + "/" + "testName1.js");
        Assert.assertTrue(f.exists());
        String resStr = "/*<<<{\"NAME\":\"testName1\",\"DESCRIPTION\":\"Test Description 1\",\"CANEXPORT\":false}>>>*/\n"
                + "test1";
        String content = FileUtils.readFileToString(f, "UTF-8");
        Assert.assertTrue(content.equals(resStr));

        Assert.assertEquals("testName2", modifiedScripts.get(1));
        f = new File(res.scriptFolder + "/" + "testName2.js");
        Assert.assertTrue(f.exists());
        resStr = "/*<<<{\"NAME\":\"testName2\",\"DESCRIPTION\":\" \",\"CANEXPORT\":false}>>>*/\n" + "test2";
        content = FileUtils.readFileToString(f, "UTF-8");
        Assert.assertTrue(content.equals(resStr));
    }

    @Test
    @Category(UnitTest.class)
    public void testSaveMultipleEmptyContent() throws IOException {
        /*
         * See explanation in testSaveMultipleEmptyName for why no error occurs
         * here
         */

        ModifyScriptsRequest request = new ModifyScriptsRequest();
        request.setScripts(new Script[] { new Script("testName1", "Test Description 1", "test1"),
                new Script("testName2", "Test Description 2", " ") });

        ScriptsModifiedResponse response = res.saveScripts(request);

        Assert.assertEquals(1, response.getScriptsModified().length);

        Assert.assertEquals("testName1", response.getScriptsModified()[0]);
        File f = new File(res.scriptFolder + "/" + "testName1.js");
        Assert.assertTrue(f.exists());
        String resStr = "/*<<<{\"NAME\":\"testName1\",\"DESCRIPTION\":\"Test Description 1\",\"CANEXPORT\":false}>>>*/\n"
                + "test1";
        String content = FileUtils.readFileToString(f, "UTF-8");
        Assert.assertTrue(content.equals(resStr));

        f = new File(res.scriptFolder + "/" + "testName2.js");
        Assert.assertFalse(f.exists());
    }

    @Test
    @Category(UnitTest.class)
    public void testGetScriptsList() throws Exception {
        Response resp = res.processSave("test3", "testName3", "Test3 Description");
        Assert.assertTrue(resp.getStatus() == 200);
        resp = res.processSave("test1", "testName4", "Test4 Description");
        Assert.assertTrue(resp.getStatus() == 200);

        File f = new File(res.scriptFolder + "/" + "testName3.js");
        Assert.assertTrue(f.exists());

        f = new File(res.scriptFolder + "/" + "testName4.js");
        Assert.assertTrue(f.exists());

        resp = res.getScriptsList();
        String strList = resp.getEntity().toString();

        JSONArray arr = (JSONArray) _parser.parse(strList);

        Assert.assertTrue(arr.size() > 0);

        for (Object jo : arr) {
            JSONObject o = (JSONObject) jo;
            if (o.get("NAME") != null) {
                if (o.get("NAME").toString().equals("testName3")) {
                    Assert.assertTrue(o.get("NAME").toString().startsWith("testName"));
                    Assert.assertTrue(o.get("DESCRIPTION").toString().startsWith("Test"));
                }
            }
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testGetScript() throws Exception {
        Response resp = res.processSave("test5", "testName5", "Test5 Description");
        Assert.assertTrue(resp.getStatus() == 200);
        resp = res.processSave("test6", "testName6", "Test6 Description");
        Assert.assertTrue(resp.getStatus() == 200);

        File f = new File(res.scriptFolder + "/" + "testName5.js");
        Assert.assertTrue(f.exists());

        f = new File(res.scriptFolder + "/" + "testName6.js");
        Assert.assertTrue(f.exists());

        resp = res.getScript("testName5");
        String scriptStr = resp.getEntity().toString();

        Assert.assertTrue(scriptStr.equals("test5"));

        resp = res.getScript("testName6");
        scriptStr = resp.getEntity().toString();

        Assert.assertTrue(scriptStr.equals("test6"));
    }

    @Test
    @Category(UnitTest.class)
    public void testDeleteScript() throws Exception {
        Response resp = res.processSave("test9", "testName9", "Test9 Description");
        Assert.assertTrue(resp.getStatus() == 200);

        File f = new File(res.scriptFolder + "/" + "testName9.js");
        Assert.assertTrue(f.exists());

        resp = res.deleteScript("testName9");
        String deletedStr = resp.getEntity().toString();

        Assert.assertTrue(deletedStr
                .equals("[{\"NAME\":\"testName9\",\"DESCRIPTION\":\"Test9 Description\",\"CANEXPORT\":false}]"));
    }

    @Test
    @Category(UnitTest.class)
    public void testDeleteMultiple() {
        Response saveResponse = res.processSave("test9", "testName9", "Test9 Description");
        Assert.assertEquals(200, saveResponse.getStatus());
        File f = new File(res.scriptFolder + "/" + "testName9.js");
        Assert.assertTrue(f.exists());

        saveResponse = res.processSave("test10", "testName10", "Test10 Description");
        Assert.assertEquals(200, saveResponse.getStatus());
        f = new File(res.scriptFolder + "/" + "testName10.js");
        Assert.assertTrue(f.exists());

        ModifyScriptsRequest request = new ModifyScriptsRequest();
        List<Script> scriptsToDelete = new ArrayList<Script>();
        Script script = new Script("testName9", "Test9 Description", "test9");
        scriptsToDelete.add(script);
        script = new Script("testName10", "Test10 Description", "test10");
        scriptsToDelete.add(script);
        request.setScripts(scriptsToDelete.toArray(new Script[scriptsToDelete.size()]));

        ScriptsModifiedResponse deleteResponse = res.deleteScripts(request);

        Assert.assertEquals(2, deleteResponse.getScriptsModified().length);
        List<String> modifiedScripts = Arrays.asList(deleteResponse.getScriptsModified());
        Collections.sort(modifiedScripts);
        Assert.assertEquals("testName10", modifiedScripts.get(0));
        f = new File(res.scriptFolder + "/" + "testName10.js");
        Assert.assertFalse(f.exists());
        Assert.assertEquals("testName9", modifiedScripts.get(1));
        f = new File(res.scriptFolder + "/" + "testName9.js");
        Assert.assertFalse(f.exists());
    }

    @Test
    @Category(UnitTest.class)
    public void testDeleteMultipleScriptsDirDoesntExist() throws IOException {
        /*
         * See explanation in testSaveMultipleEmptyName why no failure occurs
         * here
         */

        ModifyScriptsRequest request = new ModifyScriptsRequest();
        List<Script> scriptsToDelete = new ArrayList<Script>();
        Script script = new Script("testName9", "Test9 Description", "test9");
        scriptsToDelete.add(script);
        script = new Script("testName10", "Test10 Description", "test10");
        scriptsToDelete.add(script);
        request.setScripts(scriptsToDelete.toArray(new Script[scriptsToDelete.size()]));

        FileUtils.deleteDirectory(new File(res.scriptFolder));

        ScriptsModifiedResponse deleteResponse = res.deleteScripts(request);
        Assert.assertEquals(0, deleteResponse.getScriptsModified().length);
    }

    @Test
    @Category(UnitTest.class)
    public void testDeleteMultipleScriptToBeDeletedDoesntExist() throws Exception {
        /*
         * See explanation in testSaveMultipleEmptyName why no failure occurs
         * here
         */

        Response saveResponse = res.processSave("test9", "testName9", "Test9 Description");
        Assert.assertEquals(200, saveResponse.getStatus());
        File f = new File(res.scriptFolder + "/" + "testName9.js");
        Assert.assertTrue(f.exists());

        saveResponse = res.processSave("test10", "testName10", "Test10 Description");
        Assert.assertEquals(200, saveResponse.getStatus());
        f = new File(res.scriptFolder + "/" + "testName10.js");
        Assert.assertTrue(f.exists());

        ModifyScriptsRequest request = new ModifyScriptsRequest();
        List<Script> scriptsToDelete = new ArrayList<Script>();
        Script script = new Script("testName9", "Test9 Description", "test9");
        scriptsToDelete.add(script);
        // this doesn't exist
        script = new Script("testName11", "Test11 Description", "test11");
        scriptsToDelete.add(script);
        request.setScripts(scriptsToDelete.toArray(new Script[scriptsToDelete.size()]));

        ScriptsModifiedResponse deleteResponse = res.deleteScripts(request);

        Assert.assertEquals(1, deleteResponse.getScriptsModified().length);
        Assert.assertEquals("testName9", deleteResponse.getScriptsModified()[0]);
        f = new File(res.scriptFolder + "/" + "testName9.js");
        Assert.assertFalse(f.exists());

        f = new File(res.scriptFolder + "/" + "testName10.js");
        Assert.assertTrue(f.exists());
    }

    @Test
    @Category(UnitTest.class)
    public void testDeleteMultipleScriptToBeDeletedHasNoHeader() throws Exception {
        Response saveResponse = res.processSave("test9", "testName9", "Test9 Description");
        Assert.assertTrue(saveResponse.getStatus() == 200);
        File f = new File(res.scriptFolder + "/" + "testName9.js");
        Assert.assertTrue(f.exists());

        saveResponse = res.processSave("test10", "testName10", "Test10 Description");
        Assert.assertTrue(saveResponse.getStatus() == 200);
        f = new File(res.scriptFolder + "/" + "testName10.js");
        Assert.assertTrue(f.exists());

        // remove the header (on the first line) from the second script
        hoot.services.utils.FileUtils.removeFirstLineFromFile(f);

        String content = FileUtils.readFileToString(f, "UTF-8");
        Assert.assertFalse(content.contains("*<<<"));
        Assert.assertFalse(content.contains(">>>*"));

        ModifyScriptsRequest request = new ModifyScriptsRequest();
        List<Script> scriptsToDelete = new ArrayList<Script>();
        Script script = new Script("testName9", "Test9 Description", "test9");
        scriptsToDelete.add(script);
        script = new Script("testName10", "Test10 Description", "test10");
        scriptsToDelete.add(script);
        request.setScripts(scriptsToDelete.toArray(new Script[scriptsToDelete.size()]));

        ScriptsModifiedResponse deleteResponse = res.deleteScripts(request);

        Assert.assertEquals(1, deleteResponse.getScriptsModified().length);
        Assert.assertEquals("testName9", deleteResponse.getScriptsModified()[0]);
        f = new File(res.scriptFolder + "/" + "testName9.js");
        Assert.assertFalse(f.exists());
        f = new File(res.scriptFolder + "/" + "testName10.js");
        Assert.assertTrue(f.exists());
    }

    @Test
    @Category(UnitTest.class)
    public void testValidateExport() throws Exception {

        String noExportScript = "";
        noExportScript += "function translateToOsm(attrs, layerName){}";
        boolean canExport = res.validateExport(noExportScript);
        Assert.assertTrue(!canExport);

        String exportScript = "";
        exportScript += "function getDbSchema(){}";
        exportScript += "function translateToOgr(){}";
        canExport = res.validateExport(exportScript);
        Assert.assertTrue(canExport);
    }

    @Test
    @Category(UnitTest.class)
    public void testGetDefaultList() throws Exception {
        // Try to test current DefaultTranslations.json setup and the mechanism.
        // The value in DefaultTranslations.json should be valid at build time..

        List<String> configFiles = new ArrayList<String>();
        configFiles.add(HootProperties.getProperty("defaultTranslationsConfig"));
        JSONArray trans = res.getDefaultList(configFiles);
        for (Object o : trans) {
            JSONObject jsTrans = (JSONObject) o;

            Object oName = jsTrans.get("NAME");
            Assert.assertNotNull(oName);
            Assert.assertTrue(!oName.toString().isEmpty());
            Assert.assertNotNull(jsTrans.get("DESCRIPTION"));

            Object oPath = jsTrans.get("PATH");
            Assert.assertNotNull(oPath);
            Assert.assertTrue(oPath.toString().length() > 0);

            File fScript = new File(homefolder + "/" + oPath.toString());
            Assert.assertTrue(fScript.exists());

            String sScript = FileUtils.readFileToString(fScript);
            res.validateExport(sScript);

            if (jsTrans.get("FOUO_PATH") != null) {
                File fouo = new File(homefolder + "/" + jsTrans.get("FOUO_PATH"));
                Assert.assertTrue(fouo.exists());
            }
        }
    }

    @AfterClass
    public static void oneTimeTearDown() throws IOException {
        File dir = new File(res.scriptFolder);
        FileUtils.deleteDirectory(dir);
    }
}
