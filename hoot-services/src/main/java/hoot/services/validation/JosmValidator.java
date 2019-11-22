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
import java.lang.Class;

import org.openstreetmap.josm.data.osm.AbstractPrimitive;
import org.openstreetmap.josm.data.osm.OsmPrimitive;
import org.openstreetmap.josm.data.validation.OsmValidator;
import org.openstreetmap.josm.data.validation.Test;
import org.openstreetmap.josm.data.validation.TestError;
import org.openstreetmap.josm.tools.Logging;
import org.openstreetmap.josm.spi.preferences.Config;
import org.openstreetmap.josm.data.Preferences;
import org.openstreetmap.josm.io.OsmReader;
import org.openstreetmap.josm.io.OsmApi;
import org.openstreetmap.josm.command.Command;
import org.openstreetmap.josm.data.preferences.JosmBaseDirectories;
import org.openstreetmap.josm.data.preferences.JosmUrls;

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
    initJosm();
  }

  /*
   * TODO
   */
  private void initJosm()
  {
    Logging.setLogLevel(Logging.LEVEL_WARN); // LEVEL_DEBUG
    Logging.debug("Initializing JOSM...");
    Preferences pref = Preferences.main();
    Config.setPreferencesInstance(pref);
    Config.setBaseDirectoriesProvider(JosmBaseDirectories.getInstance());
    Config.setUrlsProvider(JosmUrls.getInstance());
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
    // will try passing features around as xml for the first draft; if too slow can try
    // an OsmMap --> OsmPrimitive conversion

    String validatedFeaturesStr = "";
    numValidationErrors = 0;
    numFeatureFixesMade = 0;
    try
    {
      Logging.trace("validators: " + validators);
      Logging.trace("fixFeatures: " + fixFeatures);

      // read input element xml
      Logging.debug("Converting input elements from xml...");
      Collection<OsmPrimitive> elementsToValidate =
        OsmReader.parseDataSet(
          new ByteArrayInputStream(featuresXml.getBytes()), null).getAllPrimitives();
      Logging.trace("elementsToValidate size: " + elementsToValidate.size());

      // run the specified validation tests against the elements
      Logging.debug("Running validators...");
      List<TestError> errors = runValidators(validators.split(";"), elementsToValidate);
      Logging.trace("errors size: " + errors.size());

      if (errors.size() > 0)
      {
        // optionally fix features failing validation and add validation/fix msg tags for use in hoot
        Logging.debug("Parsing validated elements...");
        Collection<AbstractPrimitive> validatedElements =
          collectValidatedElements(errors, fixFeatures);
        Logging.trace("validatedElements size: " + validatedElements.size());

        // convert the validated elements back to xml
        Logging.debug("Converting validated elements to xml...");
        validatedFeaturesStr =
          OsmApi.getOsmApi("http://localhost").toBulkXml(validatedElements, true);
        Logging.trace("validatedFeaturesStr: " + validatedFeaturesStr);
      }
    }
    catch (Exception e)
    {
      System.out.println(e.getMessage());
      throw e;
    }
    return validatedFeaturesStr; // empty string returned means no features had validation issues
  }

  /*
   * TODO
   */
  private List<TestError> runValidators(String[] validators, Collection<OsmPrimitive> elements)
    throws Exception
  {
    List<TestError> errors = new ArrayList<TestError>();
    for (int i = 0; i < validators.length; i++)
    {
      String validatorStr = validators[i];
      Logging.trace("validatorStr: " + validatorStr);
      Test validator = (Test)Class.forName(validatorStr).newInstance();

      validator.initialize();
      validator.setPartialSelection(false);
      validator.startTest(null);
      validator.visit(elements);
      validator.endTest();

      errors.addAll(validator.getErrors());
      validator.clear();
    }
    numValidationErrors = errors.size();
    return errors;
  }

  /*
   * TODO
   */
  private Collection<AbstractPrimitive> collectValidatedElements(
    List<TestError> errors, boolean fixFeatures)
  {
    Collection<AbstractPrimitive> validatedElements = new ArrayList<AbstractPrimitive>();
    for (TestError error : errors)
    {
      Collection<? extends OsmPrimitive> elementsWithErrors = error.getPrimitives();
      Logging.trace("elementsWithErrors size: " + elementsWithErrors.size());

      boolean fixSuccess = false;
      if (fixFeatures && error.isFixable())
      {
        // fix features based on error found
        Command fixCmd = error.getFix();
        fixSuccess = fixCmd.executeCommand();
        if (!fixSuccess)
        {
          Logging.trace("Failure executing fix command.");
        }
      }

      for (OsmPrimitive element : elementsWithErrors)
      {
        // mark the validated/fixed elements for use in hoot
        element.put("hoot:validation", error.getMessage());
        if (fixFeatures)
        {
          String fixStatus = "false";
          if (fixSuccess)
          {
            fixStatus = "true";
            numFeatureFixesMade++;
          }
          element.put("hoot:validation:fixed", fixStatus);
        }
        validatedElements.add(element);
      }
    }
    return validatedElements;
  }

  public int getNumValidationErrors() { return numValidationErrors; }
  public int getNumFeatureFixesMade() { return numFeatureFixesMade; }

  // TODO
  private int numValidationErrors = 0;
  // TODO
  private int numFeatureFixesMade = 0;
}
