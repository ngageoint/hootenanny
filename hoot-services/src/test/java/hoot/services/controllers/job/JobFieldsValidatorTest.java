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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.job;

import static hoot.services.HootProperties.HOME_FOLDER;

import java.io.File;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.io.FileUtils;
import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;
import hoot.services.testsupport.HootCustomPropertiesSetter;


public class JobFieldsValidatorTest {
    private static final File testFolder = new File(FileUtils.getTempDirectory(), "JobFieldsValidatorTest");
    private static String originalHomeFolderDir;

    @BeforeClass
    public static void beforeClass() throws Exception {
        URL inputUrl = JobFieldsValidatorTest.class.getResource("/hoot.services.controllers.job/services_fields_metadata.json");
        File dest = new File(new File(testFolder, "scripts"), "services_fields_metadata.json");
        FileUtils.copyURLToFile(inputUrl, dest);

        originalHomeFolderDir = HOME_FOLDER;
        HootCustomPropertiesSetter.setProperty("HOME_FOLDER", testFolder.getAbsolutePath());
    }

    @AfterClass
    public static void afterClass() throws Exception {
        FileUtils.deleteQuietly(testFolder);
        HootCustomPropertiesSetter.setProperty("HOME_FOLDER", originalHomeFolderDir);
    }

    @Test
    @Category(UnitTest.class)
    public void testValidateRequiredExists() throws Exception {
        JobFieldsValidator validator = new JobFieldsValidator("ConflationResource");

        List<String> missingList = new ArrayList<>();

        Map<String, String> fieldsMap = new HashMap<>();
        fieldsMap.put("INPUT1_TYPE", "DB");
        fieldsMap.put("INPUT1", "412836ae-26c4-4964-99d2-049e5c30178b");
        // fieldsMap.put("INPUT2_TYPE","DB");
        // fieldsMap.put("INPUT2","fbebefb4-ab39-4b9a-96ce-a82d32e65353");
        fieldsMap.put("OUTPUT_NAME", "Test1");
        fieldsMap.put("CONFLATION_TYPE", "reference");
        fieldsMap.put("MATCH_THRESHOLD", "0.6");
        fieldsMap.put("MISS_THRESHOLD", "0.6");

        boolean actual = validator.validateRequiredExists(fieldsMap, missingList);

        boolean expected = true;
        Assert.assertEquals(expected, actual);

        expected = false;

        missingList = new ArrayList<>();
        fieldsMap = new HashMap<>();
        fieldsMap.put("INPUT1_TYPE", "DB");
        // fieldsMap.put("INPUT2","fbebefb4-ab39-4b9a-96ce-a82d32e65353");
        fieldsMap.put("OUTPUT_NAME", "Test1");
        fieldsMap.put("CONFLATION_TYPE", "reference");
        fieldsMap.put("MATCH_THRESHOLD", "0.6");
        fieldsMap.put("MISS_THRESHOLD", "0.6");
        actual = validator.validateRequiredExists(fieldsMap, missingList);

        String missing = missingList.toString();
        Assert.assertEquals("[INPUT1]", missing);
        Assert.assertFalse(actual);

        validator = new JobFieldsValidator("CookieCutterResource");

        expected = true;

        missingList = new ArrayList<>();
        fieldsMap = new HashMap<>();
        fieldsMap.put("INPUT1_TYPE", "DB");
        fieldsMap.put("INPUT1", "412836ae-26c4-4964-99d2-049e5c30178b");
        // fieldsMap.put("INPUT2_TYPE","DB");
        // fieldsMap.put("INPUT2","fbebefb4-ab39-4b9a-96ce-a82d32e65353");
        fieldsMap.put("OUTPUT_NAME", "Test1");
        fieldsMap.put("CONFLATION_TYPE", "reference");
        fieldsMap.put("MATCH_THRESHOLD", "0.6");
        fieldsMap.put("MISS_THRESHOLD", "0.6");
        fieldsMap.put("cuttershapetype", "DB");
        fieldsMap.put("cuttershape", "cutter");
        fieldsMap.put("alpha", "5000");
        fieldsMap.put("buffer", "0");
        fieldsMap.put("inputtype", "DB");
        fieldsMap.put("input", "dough");
        fieldsMap.put("outputbuffer", "-500");
        fieldsMap.put("crop", "false");
        fieldsMap.put("outputname", "test3");
        actual = validator.validateRequiredExists(fieldsMap, missingList);

        Assert.assertTrue(actual);

        expected = false;

        missingList = new ArrayList<>();
        fieldsMap = new HashMap<>();
        fieldsMap.put("INPUT1_TYPE", "DB");
        // fieldsMap.put("INPUT2","fbebefb4-ab39-4b9a-96ce-a82d32e65353");
        fieldsMap.put("OUTPUT_NAME", "Test1");
        fieldsMap.put("CONFLATION_TYPE", "reference");
        fieldsMap.put("MATCH_THRESHOLD", "0.6");
        fieldsMap.put("MISS_THRESHOLD", "0.6");
        fieldsMap.put("cuttershape", "cutter");
        fieldsMap.put("alpha", "5000");
        fieldsMap.put("buffer", "0");
        fieldsMap.put("inputtype", "DB");
        fieldsMap.put("input", "dough");
        fieldsMap.put("outputbuffer", "-500");
        fieldsMap.put("crop", "false");
        fieldsMap.put("outputname", "test3");
        actual = validator.validateRequiredExists(fieldsMap, missingList);

        missing = missingList.toString();
        Assert.assertEquals("[cuttershapetype, INPUT1]", missing);
        Assert.assertFalse(actual);

        validator = new JobFieldsValidator("ETLResource");

        expected = true;

        missingList = new ArrayList<>();
        fieldsMap = new HashMap<>();
        fieldsMap.put("TRANSLATION", "translations/MGCP.js");
        fieldsMap.put("INPUT_TYPE", "OSM");
        fieldsMap.put("INPUT", "test-files/ToyTestA.osm");
        fieldsMap.put("INPUT_NAME", "ToyTestA");
        actual = validator.validateRequiredExists(fieldsMap, missingList);

        Assert.assertTrue(actual);

        expected = false;

        missingList = new ArrayList<>();
        fieldsMap = new HashMap<>();
        fieldsMap.put("TRANSLATION", "translations/MGCP.js");
        fieldsMap.put("INPUT", "test-files/ToyTestA.osm");
        fieldsMap.put("INPUT_NAME", "ToyTestA");
        actual = validator.validateRequiredExists(fieldsMap, missingList);

        missing = missingList.toString();
        Assert.assertEquals("[INPUT_TYPE]", missing);
        Assert.assertFalse(actual);

        validator = new JobFieldsValidator("ExportResource");

        expected = true;

        missingList = new ArrayList<>();
        fieldsMap = new HashMap<>();
        fieldsMap.put("translation", "MGCP.js");
        fieldsMap.put("inputtype", "db");
        fieldsMap.put("input", "ToyTestA");
        fieldsMap.put("outputtype", "gdb");
        fieldsMap.put("removereview", "false");
        actual = validator.validateRequiredExists(fieldsMap, missingList);

        Assert.assertTrue(actual);

        expected = false;

        missingList = new ArrayList<>();
        fieldsMap = new HashMap<>();
        fieldsMap.put("inputtype", "db");
        fieldsMap.put("input", "ToyTestA");
        actual = validator.validateRequiredExists(fieldsMap, missingList);

        missing = missingList.toString();
        Assert.assertEquals("[translation, outputtype]", missing);
        Assert.assertFalse(actual);

        validator = new JobFieldsValidator("TranslatorResource");

        expected = true;

        missingList = new ArrayList<>();
        fieldsMap = new HashMap<>();
        fieldsMap.put("input", "ToyTestA");
        actual = validator.validateRequiredExists(fieldsMap, missingList);

        Assert.assertTrue(actual);

        expected = false;

        missingList = new ArrayList<>();
        fieldsMap = new HashMap<>();
        actual = validator.validateRequiredExists(fieldsMap, missingList);

        missing = missingList.toString();
        Assert.assertEquals("[input]", missing);
        Assert.assertFalse(actual);
    }
}
