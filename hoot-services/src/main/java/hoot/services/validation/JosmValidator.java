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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.validation;

import java.util.List;
import java.util.ArrayList;
import java.util.Collection;
import java.lang.Exception;
import java.io.ByteArrayInputStream;

import org.openstreetmap.josm.data.osm.AbstractPrimitive;
import org.openstreetmap.josm.data.osm.OsmPrimitive;
import org.openstreetmap.josm.data.osm.Node;
import org.openstreetmap.josm.data.validation.OsmValidator;
import org.openstreetmap.josm.data.validation.Test;
import org.openstreetmap.josm.data.validation.TestError;
import org.openstreetmap.josm.tools.Logging;
import org.openstreetmap.josm.spi.preferences.Config;
import org.openstreetmap.josm.data.Preferences;
import org.openstreetmap.josm.io.OsmReader;
//import org.openstreetmap.josm.data.osm.DataSet;
import org.openstreetmap.josm.io.OsmApi;

/**
 * TODO

 using delimited strings in place of containers here to keep the JNI code simpler initially; will
 replace with containers later if performance dictates it
 */
public class JosmValidator
{
  /**
   * TODO
   */
  public JosmValidator()
  {
    Logging.setLogLevel(Logging.LEVEL_DEBUG);
    Preferences pref = Preferences.main();
    Config.setPreferencesInstance(pref);
  }

  /**
   * TODO
   */
  public String getAvailableValidators() throws Exception
  {
    String testsInfo = "";

    try
    {
      Collection<Test> validationTests = OsmValidator.getTests();
      //System.out.println("validationTests size: " + validationTests.size());
      for (Test validationTest : validationTests)
      {
        if (validationTest != null)
        {
          String testName = validationTest.toString().split("@")[0];
          String testDescription = validationTest.getName();
          testsInfo += testName + "," + testDescription + ";";
        }
      }
    }
    catch (Exception e)
    {
      System.out.println(e.getMessage());
      throw e;
    }

    //System.out.println(testsInfo);
    return testsInfo;
  }

  /**
   * TODO
   */
  public String validate(String validators, String featuresXml, boolean fixFeatures)
    throws Exception
  {
    String validatedFeaturesStr = "";
    try
    {
      // will try passing features around as xml for the first draft; if too slow can try
      // an OsmMap --> OsmPrimitive conversion

      Collection<OsmPrimitive> elements =
        OsmReader.parseDataSet(
          new ByteArrayInputStream(featuresXml.getBytes()), null).getAllPrimitives();

      assert(validators.contains(";"));
      String[] validatorsToUse = validators.split(";");
      List<TestError> errors = new ArrayList<TestError>();
      for (int i = 0; i < validatorsToUse.length; i++)
      {
        String validator = validatorsToUse[i];

        // TODO: create and init collection of tests based on contents of validatorsParts via select
        // logic from OsmValidator
        Test validationTest = null;
        validationTest.initialize();
        validationTest.setPartialSelection(false);
        validationTest.visit(elements);
        validationTest.endTest();
        errors.addAll(validationTest.getErrors());
        validationTest.clear();
      }

      // add validation/fix msg tags

      Collection<AbstractPrimitive> validatedElements = new ArrayList<AbstractPrimitive>();
      if (fixFeatures)
      {
        // TODO: fix features based on errors found

      }
      else
      {
        for (TestError error : errors)
        {
          Collection<? extends OsmPrimitive> elementsWithErrors = error.getPrimitives();
          for (OsmPrimitive element : elementsWithErrors)
          {
            element.put("hoot:validation", error.getMessage());
            validatedElements.add(element);
          }
        }
      }

      // convert elements back to xml

      validatedFeaturesStr =
        OsmApi.getOsmApi("http://localhost").toBulkXml(validatedElements, true);
    }
    catch (Exception e)
    {
      System.out.println(e.getMessage());
      throw e;
    }

    return validatedFeaturesStr;
  }
}
