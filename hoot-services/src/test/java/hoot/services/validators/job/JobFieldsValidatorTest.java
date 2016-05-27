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
package hoot.services.validators.job;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.junit.Assert;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;


public class JobFieldsValidatorTest {
    @Ignore
    @Test
    @Category(UnitTest.class)
    public void testValidateField() throws Exception {
        hoot.services.validators.job.JobFieldsValidator validator = new hoot.services.validators.job.JobFieldsValidator(
                "ConflationResource");
        boolean actual = validator.validateField("INPUT1_TYPE", "OSM");
        boolean expected = true;
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("INPUT1", "Conf1");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("INPUT2_TYPE", "OGR");
        Assert.assertEquals(actual, expected);

        // actual = validator.validateField( "INPUT2", "Input2");
        // Assert.assertEquals(actual, expected);

        actual = validator.validateField("OUTPUT_NAME", "someName");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("CONFLATION_TYPE", "reference");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("MATCH_THRESHOLD", "0.3");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("MISS_THRESHOLD", "0.3");
        Assert.assertEquals(actual, expected);

        expected = false;

        actual = validator.validateField("INPUT1_TYPE", "");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("INPUT1", "");
        Assert.assertEquals(actual, expected);

        // actual = validator.validateField( "INPUT2_TYPE", "UNK");
        // Assert.assertEquals(actual, expected);

        // actual = validator.validateField( "INPUT2", "");
        // Assert.assertEquals(actual, expected);

        actual = validator.validateField("OUTPUT_NAME", "");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("CONFLATION_TYPE", "myreference");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("MATCH_THRESHOLD", "1.3");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("MISS_THRESHOLD", "2.3");
        Assert.assertEquals(actual, expected);

        validator = new hoot.services.validators.job.JobFieldsValidator("CookieCutterResource");
        actual = validator.validateField("INPUT1_TYPE", "OSM");
        expected = true;
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("INPUT1", "Conf1");
        Assert.assertEquals(actual, expected);

        // actual = validator.validateField( "INPUT2_TYPE", "OGR");
        // Assert.assertEquals(actual, expected);

        // actual = validator.validateField( "INPUT2", "Input2");
        // Assert.assertEquals(actual, expected);

        actual = validator.validateField("OUTPUT_NAME", "someName");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("CONFLATION_TYPE", "reference");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("MATCH_THRESHOLD", "0.3");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("MISS_THRESHOLD", "0.3");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("cuttershape", "osmcut");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("alpha", "500");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("buffer", "500");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("inputtype", "DB");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("input", "someinput");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("outputbuffer", "-600");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("crop", "true");
        Assert.assertEquals(actual, expected);

        expected = false;

        actual = validator.validateField("INPUT1_TYPE", "");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("INPUT1", "");
        Assert.assertEquals(actual, expected);

        // actual = validator.validateField( "INPUT2_TYPE", "UNK");
        // Assert.assertEquals(actual, expected);

        actual = validator.validateField("OUTPUT_NAME", "");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("CONFLATION_TYPE", "myreference");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("MATCH_THRESHOLD", "1.3");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("MISS_THRESHOLD", "2.3");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("cuttershape", "");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("inputtype", "DB2");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("input", "");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("crop", "DONTKNOW");
        Assert.assertEquals(actual, expected);

        validator = new hoot.services.validators.job.JobFieldsValidator("ETLResource");

        expected = true;

        actual = validator.validateField("TRANSLATION", "translations/MGCP.js");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("INPUT_TYPE", "OGR");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("INPUT", "test-files/ToyTestA.osm");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("INPUT_NAME", "ToyTest");
        Assert.assertEquals(actual, expected);

        expected = false;

        actual = validator.validateField("TRANSLATION", "");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("INPUT_TYPE", "UNK");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("INPUT", "");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("INPUT_NAME", "");
        Assert.assertEquals(actual, expected);

        validator = new hoot.services.validators.job.JobFieldsValidator("ExportResource");

        expected = true;

        actual = validator.validateField("translation", "MGCP.js");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("inputtype", "db");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("input", "ToyTest");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("outputtype", "gdb");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("removereview", "false");
        Assert.assertEquals(actual, expected);

        expected = false;

        actual = validator.validateField("translation", "LTDS.js");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("inputtype", "mydb");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("input", "");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("outputtype", "fgdb");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("removereview", "unk");
        Assert.assertEquals(actual, expected);

        validator = new hoot.services.validators.job.JobFieldsValidator("TranslatorResource");

        expected = true;

        actual = validator.validateField("input", "<xml></xml>");
        Assert.assertEquals(actual, expected);

        expected = false;

        actual = validator.validateField("input", "");
        Assert.assertEquals(actual, expected);

        validator = new hoot.services.validators.job.JobFieldsValidator("ConflateProcesslet");
        actual = validator.validateField("INPUT1_TYPE", "OSM");
        expected = true;
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("INPUT1", "Conf1");
        Assert.assertEquals(actual, expected);

        // actual = validator.validateField( "INPUT2_TYPE", "OGR");
        // Assert.assertEquals(actual, expected);

        // actual = validator.validateField( "INPUT2", "Input2");
        // Assert.assertEquals(actual, expected);

        actual = validator.validateField("OUTPUT_NAME", "someName");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("CONFLATION_TYPE", "reference");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("MATCH_THRESHOLD", "0.3");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("MISS_THRESHOLD", "0.3");
        Assert.assertEquals(actual, expected);

        expected = false;

        actual = validator.validateField("INPUT1_TYPE", "");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("INPUT1", "");
        Assert.assertEquals(actual, expected);

        // actual = validator.validateField( "INPUT2_TYPE", "UNK");
        // Assert.assertEquals(actual, expected);

        // actual = validator.validateField( "INPUT2", "");
        // Assert.assertEquals(actual, expected);

        actual = validator.validateField("OUTPUT_NAME", "");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("CONFLATION_TYPE", "myreference");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("MATCH_THRESHOLD", "1.3");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("MISS_THRESHOLD", "2.3");
        Assert.assertEquals(actual, expected);

        validator = new hoot.services.validators.job.JobFieldsValidator("CookieCutterProcesslet");
        actual = validator.validateField("INPUT1_TYPE", "OSM");
        expected = true;
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("INPUT1", "Conf1");
        Assert.assertEquals(actual, expected);

        // actual = validator.validateField( "INPUT2_TYPE", "OGR");
        // Assert.assertEquals(actual, expected);

        // actual = validator.validateField( "INPUT2", "Input2");
        // Assert.assertEquals(actual, expected);

        actual = validator.validateField("OUTPUT_NAME", "someName");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("CONFLATION_TYPE", "reference");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("MATCH_THRESHOLD", "0.3");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("MISS_THRESHOLD", "0.3");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("cuttershape", "osmcut");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("alpha", "500");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("buffer", "500");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("inputtype", "DB");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("input", "someinput");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("outputbuffer", "-600");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("crop", "true");
        Assert.assertEquals(actual, expected);

        expected = false;

        actual = validator.validateField("INPUT1_TYPE", "");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("INPUT1", "");
        Assert.assertEquals(actual, expected);

        // actual = validator.validateField( "INPUT2_TYPE", "UNK");
        // Assert.assertEquals(actual, expected);

        actual = validator.validateField("OUTPUT_NAME", "");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("CONFLATION_TYPE", "myreference");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("MATCH_THRESHOLD", "1.3");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("MISS_THRESHOLD", "2.3");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("cuttershape", "");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("inputtype", "DB2");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("input", "");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("crop", "DONTKNOW");
        Assert.assertEquals(actual, expected);

        validator = new hoot.services.validators.job.JobFieldsValidator("ETLProcesslet");

        expected = true;

        actual = validator.validateField("TRANSLATION", "translations/MGCP.js");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("INPUT_TYPE", "OGR");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("INPUT", "test-files/ToyTestA.osm");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("INPUT_NAME", "ToyTest");
        Assert.assertEquals(actual, expected);

        expected = false;

        actual = validator.validateField("TRANSLATION", "");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("INPUT_TYPE", "UNK");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("INPUT", "");
        Assert.assertEquals(actual, expected);

        actual = validator.validateField("INPUT_NAME", "");
        Assert.assertEquals(actual, expected);

        validator = new hoot.services.validators.job.JobFieldsValidator("LTDSTranslateProcesslet");

        expected = true;

        actual = validator.validateField("input", "<xml></xml>");
        Assert.assertEquals(actual, expected);

        expected = false;

        actual = validator.validateField("input", "");
        Assert.assertEquals(actual, expected);

    }

    @Ignore
    @Test
    @Category(UnitTest.class)
    public void testValidateRequiredExists() throws Exception {
        hoot.services.validators.job.JobFieldsValidator validator = new hoot.services.validators.job.JobFieldsValidator(
                "ConflationResource");

        boolean expected = true;

        List<String> missingList = new ArrayList<String>();
        Map<String, String> fieldsMap = new HashMap<String, String>();
        fieldsMap.put("INPUT1_TYPE", "DB");
        fieldsMap.put("INPUT1", "412836ae-26c4-4964-99d2-049e5c30178b");
        // fieldsMap.put("INPUT2_TYPE","DB");
        // fieldsMap.put("INPUT2","fbebefb4-ab39-4b9a-96ce-a82d32e65353");
        fieldsMap.put("OUTPUT_NAME", "Test1");
        fieldsMap.put("CONFLATION_TYPE", "reference");
        fieldsMap.put("MATCH_THRESHOLD", "0.6");
        fieldsMap.put("MISS_THRESHOLD", "0.6");
        boolean actual = validator.validateRequiredExists(fieldsMap, missingList);

        Assert.assertEquals(actual, expected);

        expected = false;

        missingList = new ArrayList<String>();
        fieldsMap = new HashMap<String, String>();
        fieldsMap.put("INPUT1_TYPE", "DB");
        // fieldsMap.put("INPUT2","fbebefb4-ab39-4b9a-96ce-a82d32e65353");
        fieldsMap.put("OUTPUT_NAME", "Test1");
        fieldsMap.put("CONFLATION_TYPE", "reference");
        fieldsMap.put("MATCH_THRESHOLD", "0.6");
        fieldsMap.put("MISS_THRESHOLD", "0.6");
        actual = validator.validateRequiredExists(fieldsMap, missingList);

        String missing = missingList.toString();
        Assert.assertEquals(missing, "[INPUT1]");
        Assert.assertEquals(actual, expected);

        validator = new hoot.services.validators.job.JobFieldsValidator("CookieCutterResource");

        expected = true;

        missingList = new ArrayList<String>();
        fieldsMap = new HashMap<String, String>();
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

        Assert.assertEquals(actual, expected);

        expected = false;

        missingList = new ArrayList<String>();
        fieldsMap = new HashMap<String, String>();
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
        Assert.assertEquals(missing, "[cuttershapetype, INPUT1]");
        Assert.assertEquals(actual, expected);

        validator = new hoot.services.validators.job.JobFieldsValidator("ETLResource");

        expected = true;

        missingList = new ArrayList<String>();
        fieldsMap = new HashMap<String, String>();
        fieldsMap.put("TRANSLATION", "translations/MGCP.js");
        fieldsMap.put("INPUT_TYPE", "OSM");
        fieldsMap.put("INPUT", "test-files/ToyTestA.osm");
        fieldsMap.put("INPUT_NAME", "ToyTestA");
        actual = validator.validateRequiredExists(fieldsMap, missingList);

        Assert.assertEquals(actual, expected);

        expected = false;

        missingList = new ArrayList<String>();
        fieldsMap = new HashMap<String, String>();
        fieldsMap.put("TRANSLATION", "translations/MGCP.js");
        fieldsMap.put("INPUT", "test-files/ToyTestA.osm");
        fieldsMap.put("INPUT_NAME", "ToyTestA");
        actual = validator.validateRequiredExists(fieldsMap, missingList);

        missing = missingList.toString();
        Assert.assertEquals(missing, "[INPUT_TYPE]");
        Assert.assertEquals(actual, expected);

        validator = new hoot.services.validators.job.JobFieldsValidator("ExportResource");

        expected = true;

        missingList = new ArrayList<String>();
        fieldsMap = new HashMap<String, String>();
        fieldsMap.put("translation", "MGCP.js");
        fieldsMap.put("inputtype", "db");
        fieldsMap.put("input", "ToyTestA");
        fieldsMap.put("outputtype", "gdb");
        fieldsMap.put("removereview", "false");
        actual = validator.validateRequiredExists(fieldsMap, missingList);

        Assert.assertEquals(actual, expected);

        expected = false;

        missingList = new ArrayList<String>();
        fieldsMap = new HashMap<String, String>();
        fieldsMap.put("inputtype", "db");
        fieldsMap.put("input", "ToyTestA");
        actual = validator.validateRequiredExists(fieldsMap, missingList);

        missing = missingList.toString();
        Assert.assertEquals(missing, "[translation, outputtype]");
        Assert.assertEquals(actual, expected);

        validator = new hoot.services.validators.job.JobFieldsValidator("TranslatorResource");

        expected = true;

        missingList = new ArrayList<String>();
        fieldsMap = new HashMap<String, String>();
        fieldsMap.put("input", "ToyTestA");
        actual = validator.validateRequiredExists(fieldsMap, missingList);

        Assert.assertEquals(actual, expected);

        expected = false;

        missingList = new ArrayList<String>();
        fieldsMap = new HashMap<String, String>();
        actual = validator.validateRequiredExists(fieldsMap, missingList);

        missing = missingList.toString();
        Assert.assertEquals(missing, "[input]");
        Assert.assertEquals(actual, expected);

        //////////////////////////////////////
        validator = new hoot.services.validators.job.JobFieldsValidator("ConflateProcesslet");

        expected = true;

        missingList = new ArrayList<String>();
        fieldsMap = new HashMap<String, String>();
        fieldsMap.put("INPUT1_TYPE", "DB");
        fieldsMap.put("INPUT1", "412836ae-26c4-4964-99d2-049e5c30178b");
        // fieldsMap.put("INPUT2_TYPE","DB");
        // fieldsMap.put("INPUT2","fbebefb4-ab39-4b9a-96ce-a82d32e65353");
        fieldsMap.put("OUTPUT_NAME", "Test1");
        fieldsMap.put("CONFLATION_TYPE", "reference");
        fieldsMap.put("MATCH_THRESHOLD", "0.6");
        fieldsMap.put("MISS_THRESHOLD", "0.6");
        actual = validator.validateRequiredExists(fieldsMap, missingList);

        Assert.assertEquals(actual, expected);

        expected = false;

        missingList = new ArrayList<String>();
        fieldsMap = new HashMap<String, String>();
        fieldsMap.put("INPUT1_TYPE", "DB");
        // fieldsMap.put("INPUT2","fbebefb4-ab39-4b9a-96ce-a82d32e65353");
        fieldsMap.put("OUTPUT_NAME", "Test1");
        fieldsMap.put("CONFLATION_TYPE", "reference");
        fieldsMap.put("MATCH_THRESHOLD", "0.6");
        fieldsMap.put("MISS_THRESHOLD", "0.6");
        actual = validator.validateRequiredExists(fieldsMap, missingList);

        missing = missingList.toString();
        Assert.assertEquals(missing, "[INPUT1]");
        Assert.assertEquals(actual, expected);

        validator = new hoot.services.validators.job.JobFieldsValidator("CookieCutterProcesslet");

        expected = true;

        missingList = new ArrayList<String>();
        fieldsMap = new HashMap<String, String>();
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

        Assert.assertEquals(actual, expected);

        expected = false;

        missingList = new ArrayList<String>();
        fieldsMap = new HashMap<String, String>();
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
        Assert.assertEquals(missing, "[cuttershapetype, INPUT1]");
        Assert.assertEquals(actual, expected);

        validator = new hoot.services.validators.job.JobFieldsValidator("ETLProcesslet");

        expected = true;

        missingList = new ArrayList<String>();
        fieldsMap = new HashMap<String, String>();
        fieldsMap.put("TRANSLATION", "translations/MGCP.js");
        fieldsMap.put("INPUT_TYPE", "OSM");
        fieldsMap.put("INPUT", "test-files/ToyTestA.osm");
        fieldsMap.put("INPUT_NAME", "ToyTestA");
        actual = validator.validateRequiredExists(fieldsMap, missingList);

        Assert.assertEquals(actual, expected);

        expected = false;

        missingList = new ArrayList<String>();
        fieldsMap = new HashMap<String, String>();
        fieldsMap.put("TRANSLATION", "translations/MGCP.js");
        fieldsMap.put("INPUT", "test-files/ToyTestA.osm");
        fieldsMap.put("INPUT_NAME", "ToyTestA");
        actual = validator.validateRequiredExists(fieldsMap, missingList);

        missing = missingList.toString();
        Assert.assertEquals(missing, "[INPUT_TYPE]");
        Assert.assertEquals(actual, expected);

        validator = new hoot.services.validators.job.JobFieldsValidator("LTDSTranslateProcesslet");

        expected = true;

        missingList = new ArrayList<String>();
        fieldsMap = new HashMap<String, String>();
        fieldsMap.put("input", "ToyTestA");
        actual = validator.validateRequiredExists(fieldsMap, missingList);

        Assert.assertEquals(actual, expected);

        expected = false;

        missingList = new ArrayList<String>();
        fieldsMap = new HashMap<String, String>();
        actual = validator.validateRequiredExists(fieldsMap, missingList);

        missing = missingList.toString();
        Assert.assertEquals(missing, "[input]");
        Assert.assertEquals(actual, expected);

    }
}
