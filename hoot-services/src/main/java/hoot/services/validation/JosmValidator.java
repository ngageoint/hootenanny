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
import java.util.Arrays;

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
 */
public class JosmValidator
{
  /**
   * TODO
   */
  public JosmValidator(String logLevel) throws Exception
  {
    initJosm(logLevel);
  }

  /*
   * TODO
   */
  private void initJosm(String logLevel) throws Exception
  {
    setLogLevel(logLevel);
    Logging.debug("Initializing JOSM...");
    Preferences pref = Preferences.main();
    Config.setPreferencesInstance(pref);
    Config.setBaseDirectoriesProvider(JosmBaseDirectories.getInstance());
    Config.setUrlsProvider(JosmUrls.getInstance());
  }

  public void setLogLevel(String logLevel) throws Exception
  {
    switch (logLevel.toUpperCase())
    {
      case "FATAL":
        Logging.setLogLevel(Logging.LEVEL_ERROR);
        break;
      case "ERROR":
        Logging.setLogLevel(Logging.LEVEL_ERROR);
        break;
      case "WARN":
        Logging.setLogLevel(Logging.LEVEL_WARN);
        break;
      case "STATUS":
        Logging.setLogLevel(Logging.LEVEL_INFO);
        break;
      case "INFO":
        Logging.setLogLevel(Logging.LEVEL_INFO);
        break;
      case "DEBUG":
        Logging.setLogLevel(Logging.LEVEL_DEBUG);
        break;
      case "TRACE":
        Logging.setLogLevel(Logging.LEVEL_TRACE);
        break;
      default:
        throw new Exception("Invalid log level: " + logLevel);
    }
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

     passing in xml for features in place of OsmPrimitive and delimited strings in place of
     containers to keep the JNI client code simpler and likely more performant

     @todo change validatorsStr to take in a list of strings
     @todo change featuresXml to take in a list of OsmPrimitive
     @todo change return type to a list of OsmPrimitive
   */
  public String validate(String validatorsStr, String featuresXml, boolean fixFeatures)
    throws Exception
  {
    String validatedFeaturesXmlStr = "";

    try
    {
      numValidationErrors = 0;
      numElementsFixed = 0;

      String[] validators = validatorsStr.split(";");
      Logging.info("Validating elements with " + validators.length + " validators...");
      Logging.trace("validators: " + Arrays.toString(validators));
      Logging.trace("featuresXml: " + featuresXml);
      Logging.trace("fixFeatures: " + fixFeatures);

      // read input element xml
      Logging.debug("Converting input elements from xml...");
      Collection<OsmPrimitive> elementsToValidate =
        OsmReader.parseDataSet(
          new ByteArrayInputStream(featuresXml.getBytes()), null).getAllPrimitives();
      Logging.trace("elementsToValidate size: " + elementsToValidate.size());
      Logging.trace("elementsToValidate: " + getElementsStr(elementsToValidate));

      // run the specified validation tests against the elements
      Logging.debug("Running validators...");
      List<TestError> errors = runValidators(validators, elementsToValidate);
      Logging.trace("errors size: " + errors.size());

      if (errors.size() > 0)
      {
        // optionally fix features failing validation and add validation/fix msg tags for use in
        // hoot
        Logging.debug("Parsing validated elements...");
        Collection<AbstractPrimitive> validatedElements =
          collectValidatedElements(errors, fixFeatures);
        Logging.trace("validatedElements size: " + validatedElements.size());

        // convert the validated elements back to xml
        Logging.debug("Converting validated elements to xml...");
        validatedFeaturesXmlStr =
          OsmApi.getOsmApi("http://localhost").toBulkXml(validatedElements, true);
        Logging.trace("validatedFeaturesStr: " + validatedFeaturesXmlStr);
      }

      Logging.info(
        "Found " + numValidationErrors + " validation errors and fixed " + numElementsFixed +
        " elements.");
    }
    catch (Exception e)
    {
      System.out.println(e.getMessage());
      throw e;
    }

    return validatedFeaturesXmlStr; // empty string returned means no features had validation issues
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
      Logging.trace("error fixable?: " + error.isFixable());
      if (fixFeatures && error.isFixable())
      {
        // fix features based on error found
        Logging.trace("Fixing features...");
        Command fixCmd = error.getFix();
        Logging.trace("cmd descr: " + fixCmd.getDescriptionText());
        fixSuccess = fixCmd.executeCommand();
        if (!fixSuccess)
        {
          Logging.trace("Failure executing fix command.");
        }
        else
        {
          Logging.trace("Success executing fix command.");
        }
      }

      for (OsmPrimitive element : elementsWithErrors)
      {
        // mark the validated/fixed elements for use in hoot

        // validated is a ';' separated list describing one or more validations performed
        String validationVal = "";
        if (element.hasKey(VALIDATED_TAG_KEY))
        {
          validationVal = element.get(VALIDATED_TAG_KEY) + ";" + error.getMessage();
        }
        else
        {
          validationVal = error.getMessage();
        }
        element.put(VALIDATED_TAG_KEY, validationVal);

        if (fixFeatures)
        {
          String fixStatus = "false";
          if (fixSuccess)
          {
            fixStatus = "true";
            numElementsFixed++;
          }

          // fixed is a ';' separated list describing one or more fixes made
          String fixedVal = "";
          if (element.hasKey(FIXED_TAG_KEY))
          {
            fixedVal = element.get(FIXED_TAG_KEY) + ";" + fixStatus;
          }
          else
          {
            fixedVal = fixStatus;
          }
          element.put(FIXED_TAG_KEY, fixedVal);
        }
        validatedElements.add(element);
      }
    }

    return validatedElements;
  }

  private static String getElementsStr(Collection<OsmPrimitive> elements)
  {
    String elementsStr = "";
    for (OsmPrimitive element : elements)
    {
      elementsStr += element.toString() + ";";
    }
    return elementsStr;
  }

  public int getNumValidationErrors() { return numValidationErrors; }
  public int getNumElementsFixed() { return numElementsFixed; }

  // these match corresponding entries in core MetadataTags class
  private static final String VALIDATED_TAG_KEY = "hoot:validation:error";
  private static final String FIXED_TAG_KEY = "hoot:validation:error:fixed";

  // TODO
  private int numValidationErrors = 0;
  // TODO
  private int numElementsFixed = 0;
}
