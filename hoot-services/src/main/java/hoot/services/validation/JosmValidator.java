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

import hoot.services.validation.HootOsmReader;
import hoot.services.validation.JosmUtils;

import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;
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
import org.openstreetmap.josm.io.OsmApi;
import org.openstreetmap.josm.command.Command;

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
    JosmUtils.initJosm(logLevel);
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
  public String validate(String validatorsStr, String featuresXml, boolean fixFeatures)
    throws Exception
  {
    String validatedFeaturesXmlStr = "";

    numValidationErrors = 0;
    numElementsFixed = 0;

    // verify inputs

    Logging.debug("validatorsStr: " + validatorsStr);
    Logging.debug("fixFeatures: " + fixFeatures);

    // TODO: check for empty map too?
    if (featuresXml == null || featuresXml.trim().isEmpty())
    {
      throw new Exception("No features passed to validation.");
    }
    //Logging.trace("featuresXml: " + featuresXml);

    String[] validators = null;
    // empty or null input validators means we'll run them all
    if (validatorsStr != null && !validatorsStr.trim().isEmpty())
    {
      validators = validatorsStr.split(";");
    }
    if (validators != null)
    {
      Logging.info("Validating elements with " + validators.length + " validators...");
    }
    else
    {
      Logging.info("Validating elements with all validators...");
    }
    Logging.debug("validators: " + Arrays.toString(validators));

    // read in the input element xml

    // TODO: clone initial map?

    Collection<OsmPrimitive> elementsToValidate = null;
    try
    {
      Logging.debug("Converting input elements from xml...");
      elementsToValidate =
        HootOsmReader.parseDataSet(
          new ByteArrayInputStream(featuresXml.getBytes())).getAllPrimitives();
      Logging.debug("elementsToValidate size: " + elementsToValidate.size());
      //Logging.trace("elementsToValidate: " + JosmUtils.elementsToString(elementsToValidate));
    }
    catch (Exception e)
    {
      Logging.error("Error converting validation input features to XML: " + e.getMessage());
      throw e;
    }

    // run the specified validation tests against the elements

    List<TestError> errors = null;
    Logging.debug("Running validators...");
    try
    {
      if (validators == null)
      {
        errors = runValidation(OsmValidator.getTests(), elementsToValidate);
      }
      else
      {
        errors = runValidation(validators, elementsToValidate);
      }
      Logging.debug("errors size: " + errors.size());
    }
    catch (Exception e)
    {
      Logging.error("Error running validation tests: " + e.getMessage());
      throw e;
    }

    // if any validation issues were found, collect, optionally fix features failing validation,
    // and add validation/fix msg tags for use in hoot

    Collection<AbstractPrimitive> validatedElements = null;
    if (errors.size() > 0)
    {
      try
      {
        Logging.debug("Parsing validated elements...");
        validatedElements = collectAndOptionallyFixValidatedElements(errors, fixFeatures);
        Logging.debug("validatedElements size: " + validatedElements.size());
        //Logging.trace("validatedElements: " + JosmUtils.elementsToString(validatedElements));
      }
      catch (Exception e)
      {
        String msg = "Error during gathering";
        if (fixFeatures)
        {
          msg += " and fixing of";
        }
        msg += " validated elements: ";
        Logging.error(msg + e.getMessage());
        throw e;
      }
    }

    // convert any validated elements back to xml; we'll just end up returning empty xml if no
    // errors were found
    // TODO: combine original copied input map back with validated features

    if (validatedElements != null)
    {
      try
      {
        Logging.debug("Converting validated elements to xml...");
        validatedFeaturesXmlStr =
          OsmApi.getOsmApi("http://localhost").toBulkXml(validatedElements, true);
        //Logging.trace("validatedFeaturesXmlStr: " + validatedFeaturesXmlStr);
      }
      catch (Exception e)
      {
        Logging.error("Error converting validated elements back to XML: " + e.getMessage());
        throw e;
      }
    }

    Logging.info(
      "Found " + numValidationErrors + " validation errors and fixed " + numElementsFixed +
      " elements.");

    return validatedFeaturesXmlStr;
  }

  /*
   * TODO
   */
  private List<TestError> runValidation(String[] validators, Collection<OsmPrimitive> elements)
    throws Exception
  {
    List<TestError> errors = new ArrayList<TestError>();
    for (int i = 0; i < validators.length; i++)
    {
      String validatorStr = validators[i];
      Logging.trace("validatorStr: " + validatorStr);
      Test validator = (Test)Class.forName(validatorStr).newInstance();
      errors.addAll(runValidation(validator, elements));
    }
    numValidationErrors = errors.size();
    return errors;
  }

  /*
   * TODO
   */
  private List<TestError> runValidation(
    Collection<Test> validators, Collection<OsmPrimitive> elements) throws Exception
  {
    List<TestError> errors = new ArrayList<TestError>();
    for (Test validator : validators)
    {
      errors.addAll(runValidation(validator, elements));
    }
    numValidationErrors = errors.size();
    return errors;
  }

  /*
   * TODO
   */
  private List<TestError> runValidation(Test validator, Collection<OsmPrimitive> elements)
    throws Exception
  {
    List<TestError> errors = new ArrayList<TestError>();

    validator.initialize();
    validator.setPartialSelection(false);
    validator.startTest(null);
    validator.visit(elements);
    validator.endTest();

    errors.addAll(validator.getErrors());
    validator.clear();

    Logging.trace(
      "Validator: " + validator.getName() + " found " + errors.size() + " errors with " +
      getNumElementsInvolvedInErrors(errors) + " total involved elements.");
    return errors;
  }

  /*
   * TODO
   */
  private int getNumElementsInvolvedInErrors(List<TestError> errors)
  {
    int numElementsInvolved = 0;
    for (TestError error : errors)
    {
      numElementsInvolved += error.getPrimitives().size();
    }
    return numElementsInvolved;
  }

  /*
   * TODO
   */
  private Collection<AbstractPrimitive> collectAndOptionallyFixValidatedElements(
    List<TestError> errors, boolean fixFeatures)
  {
    // <element type>:<element id> mapped to element; would like to use PrimitiveId here but don't
    // know how to get it directly from the OsmPrimitive
    Map<String, AbstractPrimitive> validatedElements =
      new HashMap<String, AbstractPrimitive>();

    for (TestError error : errors)
    {
      Collection<? extends OsmPrimitive> elementsWithErrors = error.getPrimitives();
      Logging.trace(
        "Processing " + error.getPrimitives().size() + " elements for error: \"" +
        error.getMessage() + "\" found by test: " + error.getTester().getName() + "...");
      Logging.trace("error.getPrimitives(): " + JosmUtils.elementsToString(error.getPrimitives()));

      boolean fixSuccess = false;
      Logging.trace("error fixable?: " + error.isFixable());
      if (fixFeatures && error.isFixable())
      {
        fixSuccess = fixValidatedElement(error);
      }

      // TODO: if the element was deleted by the fix, we need to ensure its not returned in the
      // output

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
        Logging.trace("validationVal: " + validationVal);
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
          Logging.trace("fixedVal: " + fixedVal);
          element.put(FIXED_TAG_KEY, fixedVal);
        }

        // we need to add all children elements of elements in the validated map; map won't allow
        // duplicate elements to be added
        validatedElements.putAll(JosmUtils.hydrate(element));
        Logging.trace("validatedElements size: " + validatedElements.size());
      }
    }

    return validatedElements.values();
  }

  /*
   * TODO
   */
  private boolean fixValidatedElement(TestError error)
  {
    // fix features based on error found
    Logging.trace(
      "Fixing " + error.getPrimitives().size() + " elements for error: \"" + error.getMessage() +
      "\" found by test: " + error.getTester().getName() + "...");
    Logging.trace("error.getPrimitives(): " + JosmUtils.elementsToString(error.getPrimitives()));

    Command fixCmd = error.getFix();
    Logging.trace("fixCmd: " + JosmUtils.commandToString(fixCmd, true));

    boolean fixSuccess = fixCmd.executeCommand();
    String fixStatus = fixSuccess ? "Success" : "Failure";
    Logging.trace(fixStatus + " executing fix command: " + fixCmd.getDescriptionText());
    return fixSuccess;
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
