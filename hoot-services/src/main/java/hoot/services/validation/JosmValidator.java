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

import com.google.common.collect.Multimap;
import com.google.common.collect.LinkedHashMultimap;

import org.openstreetmap.josm.data.osm.AbstractPrimitive;
import org.openstreetmap.josm.data.osm.OsmPrimitive;
import org.openstreetmap.josm.data.validation.OsmValidator;
import org.openstreetmap.josm.data.validation.Test;
import org.openstreetmap.josm.data.validation.TestError;
import org.openstreetmap.josm.tools.Logging;
import org.openstreetmap.josm.io.OsmApi;
import org.openstreetmap.josm.command.Command;
import org.openstreetmap.josm.data.osm.DataSet;

/**
 * TODO
 */
public class JosmValidator
{
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
    numGroupsOfElementsFixed = 0;

    // verify inputs

    Logging.debug("validatorsStr: " + validatorsStr);
    Logging.debug("fixFeatures: " + fixFeatures);

    // TODO: check for empty input map too?
    if (featuresXml == null || featuresXml.trim().isEmpty())
    {
      throw new Exception("No features passed to validation.");
    }
    Logging.trace("featuresXml: " + featuresXml);

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

    Collection<OsmPrimitive> elementsToValidate = null;
    int originalMapSize = -1;
    try
    {
      Logging.debug("Converting input elements from xml...");;
      elementsToValidate =
        HootOsmReader.parseDataSet(new ByteArrayInputStream(featuresXml.getBytes()))
          .getAllPrimitives();
      originalMapSize = elementsToValidate.size();
      Logging.debug("originalMapSize: " + originalMapSize);
      Logging.trace("input elements: " + JosmUtils.elementsToString(elementsToValidate));
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

    // check for any validation errors

    Collection<AbstractPrimitive> validatedElements = null;
    int validatedMapSize = -1;
    if (errors.size() > 0)
    {
      try
      {
        // record the features that were validated
        updateElementValidations(errors);

        if (fixFeatures)
        {
          // If we're fixing features, fix the features, record those that were fixed, and get back
          // the fixed features.

          validatedElements = fixElements(errors);

          if (validatedElements == null)
          {
            if (deletedElementIds == null || deletedElementIds.size() == 0)
            {
              Logging.trace("No elements fixed. Using original input data for output...");
            }
            else
            {
              Logging.debug(
                deletedElementIds.size() +
                " elements fixed. Modifying original input data for output...");
            }
            validatedElements = new ArrayList<AbstractPrimitive>();
            validatedElements.addAll(elementsToValidate);
          }
          Logging.debug("validatedElements size: " + validatedElements.size());
        }
        else
        {
          // Otherwise, just use the unmodified input elements.

          validatedElements = new ArrayList<AbstractPrimitive>();
          validatedElements.addAll(elementsToValidate);
        }
        Logging.debug("validatedElements size: " + validatedElements.size());

        // add validation/fix message tags for use in hoot and remove deleted elements

        validatedElements = getTaggedReturnElements(validatedElements);
        Logging.debug("validatedElements size: " + validatedElements.size());
      }
      catch (Exception e)
      {
        String msg = "Error during collecting";
        if (fixFeatures)
        {
          msg += " and fixing of";
        }
        msg += " validated elements: ";
        Logging.error(msg + e.getMessage());
        throw e;
      }
    }
    else
    {
      // If there weren't any validation errors, just use the unmodified input elements.

      Logging.trace("No elements fixed. Using original input data for output...");
      validatedElements = new ArrayList<AbstractPrimitive>();
      validatedElements.addAll(elementsToValidate);
      Logging.debug("validatedElements size: " + validatedElements.size());
    }
    validatedMapSize = validatedElements.size();

    int mapSizeDiff = 0;
    if (!fixFeatures || errors.size() == 0 || originalMapSize == validatedMapSize)
    {
      Logging.debug("The validated map has the same number of elements as the input map.");
    }
    else if (originalMapSize < validatedMapSize)
    {
      mapSizeDiff = validatedMapSize - originalMapSize;
      Logging.debug(
        "The validated map has " + mapSizeDiff + " more elements than the input map.");
    }
    else if (validatedMapSize < originalMapSize)
    {
      mapSizeDiff = originalMapSize - validatedMapSize;
      Logging.debug(
        "The validated map has " + mapSizeDiff + " fewer elements than the input map.");
    }

    Logging.trace("output elements: " + JosmUtils.elementsToString(elementsToValidate));

    // convert any validated elements back to xml; we'll just end up returning empty xml if no
    // errors were found

    // TODO: make sure if all elements get deleted here that empty map xml is returned and not an
    // empty xml string

    try
    {
      Logging.debug("Converting validated elements to xml...");
      validatedFeaturesXmlStr =
        OsmApi.getOsmApi("http://localhost").toBulkXml(validatedElements, true);
      Logging.trace("validatedFeaturesXmlStr: " + validatedFeaturesXmlStr);
    }
    catch (Exception e)
    {
      Logging.error("Error converting validated elements back to XML: " + e.getMessage());
      throw e;
    }

    Logging.info(
      "Found " + numValidationErrors + " validation errors, fixed " + numGroupsOfElementsFixed +
      " groups of elements, and deleted " + getNumDeletedElements() + " features.");

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
  private void updateElementValidations(List<TestError> errors)
  {
    Logging.debug("Recording validated elements...");

    for (TestError error : errors)
    {
      Collection<? extends OsmPrimitive> elementGroupWithError = error.getPrimitives();
      Logging.trace(
        "Processing validation results for " + error.getPrimitives().size() + " elements for " +
        " error: \"" + error.getMessage() + "\" found by test: " + error.getTester().getName() +
        "...");
      Logging.trace("error.getPrimitives(): " + JosmUtils.elementsToString(error.getPrimitives()));

      for (OsmPrimitive element : elementGroupWithError)
      {
        elementValidations.put(JosmUtils.getElementMapKey(element), error.getMessage());
      }
    }

    Logging.debug("elementValidations size: " + elementValidations.size());
  }

  /*
   * TODO
   */
  private Collection<AbstractPrimitive> fixElements(List<TestError> errors) throws Exception
  {
    Logging.debug("Fixing and recording fixed elements...");

    Collection<AbstractPrimitive> fixedElements = null;

    DataSet affectedData = null;
    deletedElementIds = new ArrayList<String>();
    for (TestError error : errors)
    {
      Collection<? extends OsmPrimitive> elementGroupWithError = error.getPrimitives();
      Logging.trace(
        "Processing " + error.getPrimitives().size() + " elements to fix for error: \"" +
        error.getMessage() + "\" found by test: " + error.getTester().getName() + "...");
      Logging.trace("error.getPrimitives(): " + JosmUtils.elementsToString(error.getPrimitives()));

      boolean fixSuccess = false;
      Logging.trace("error fixable?: " + error.isFixable());
      if (error.isFixable())
      {
        fixSuccess = fixValidatedElement(error, affectedData);
      }

      Logging.debug("Recording fixed elements...");
      for (OsmPrimitive element : elementGroupWithError)
      {
        FixStatus fixStatus = FixStatus.NONE_AVAILABLE;
        if (error.isFixable())
        {
          if (fixSuccess)
          {
            fixStatus = fixStatus.SUCCEEDED;
            numGroupsOfElementsFixed++;
          }
          else
          {
            fixStatus = fixStatus.FAILED;
          }
        }

        String elementKey = JosmUtils.getElementMapKey(element);
        assert(elementValidations.containsKey(elementKey));
        elementFixes.put(elementKey, error.getMessage() + "=" + fixStatusToString(fixStatus));
      }
    }

    Logging.debug("elementFixes size: " + elementFixes.size());

    boolean affectedDataNull = (affectedData == null);
    Logging.trace("affectedData == null: " + affectedDataNull);
    if (affectedData != null)
    {
      fixedElements = new ArrayList<AbstractPrimitive>();
      fixedElements.addAll(affectedData.getAllPrimitives());
    }

    return fixedElements;
  }

  /*
   * TODO
   */
  private boolean fixValidatedElement(TestError error, DataSet affectedData) throws Exception
  {
    // fix features based on error found
    Logging.trace(
      "Fixing " + error.getPrimitives().size() + " elements for error: \"" + error.getMessage() +
      "\" found by test: " + error.getTester().getName() + "...");
    Logging.trace("error.getPrimitives(): " + JosmUtils.elementsToString(error.getPrimitives()));

    Command fixCmd = error.getFix();
    Logging.trace("fixCmd: " + JosmUtils.commandToString(fixCmd, true));

    deletedElementIds.addAll(JosmUtils.getDeletedElementIds(fixCmd));
    affectedData = fixCmd.getAffectedDataSet();

    boolean fixSuccess = fixCmd.executeCommand();
    String fixStatus = fixSuccess ? "Success" : "Failure";
    Logging.trace(fixStatus + " executing fix command: " + fixCmd.getDescriptionText());
    return fixSuccess;
  }

  /*
   * TODO
   */
  private Collection<AbstractPrimitive> getTaggedReturnElements(
    Collection<AbstractPrimitive> finalElements) throws Exception
  {
    Logging.debug("Updating tags on up to " + finalElements.size() + " elements...");

    Collection<AbstractPrimitive> returnElements = new ArrayList<AbstractPrimitive>();

    int numValidationTagsAdded = 0;
    int numFixTagsAdded = 0;
    int numDeletedElements = 0;
    for (AbstractPrimitive element : finalElements)
    {
      OsmPrimitive osmElement = (OsmPrimitive)element;
      String elementKey = JosmUtils.getElementMapKey(osmElement);

      if (deletedElementIds == null || !deletedElementIds.contains(elementKey))
      {
        if (elementValidations.containsKey(elementKey))
        {
          Logging.trace("Adding validation tag to element: " + elementKey + "...");
          Collection<String> errorMessages = elementValidations.get(elementKey);
          String[] errorMessagesArr = errorMessages.toArray(new String[errorMessages.size()]);
          osmElement.put(VALIDATED_TAG_KEY, String.join(";", errorMessagesArr));
          numValidationTagsAdded++;

          Logging.trace("Adding fix tag to element: " + elementKey + "...");
          String fixStatusStr = "";
          if (elementFixes.containsKey(elementKey))
          {
            Collection<String> fixMessages = elementFixes.get(elementKey);
            fixStatusStr =
              String.join(
                ";", elementFixes.get(elementKey).toArray(new String[fixMessages.size()]));
          }
          else
          {
            for (int i = 0; i < errorMessagesArr.length; i++)
            {
              fixStatusStr +=
                errorMessagesArr[i] + "=" + fixStatusToString(FixStatus.NOT_ATTEMPTED);
              if (i < errorMessagesArr.length - 1)
              {
                fixStatusStr += ";";
              }
            }
          }
          osmElement.put(FIXED_TAG_KEY, fixStatusStr);
          numFixTagsAdded++;
        }
        Logging.trace("Adding return element: " + elementKey + "...");
        returnElements.add(osmElement);
      }
      else
      {
        numDeletedElements++;
      }
    }

    Logging.debug(
      "Added " + numValidationTagsAdded + " validation tags and " + numFixTagsAdded +
      " fix tags. " + numDeletedElements + " deleted elements were skipped. Total return " +
      "elements: " + returnElements.size());
    return returnElements;
  }

  private String fixStatusToString(FixStatus fixStatus) throws Exception
  {
    switch (fixStatus)
    {
      case NOT_ATTEMPTED:
        return "no fix attempted";
      case NONE_AVAILABLE:
        return "no fix available";
      case FAILED:
        return "fix failed";
      case SUCCEEDED:
        return "fix succeeded";
      default:
        throw new Exception("Invalid fix status: " + fixStatus);
    }
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

  public String getDeletedElementIds()
  {
    if (deletedElementIds != null)
    {
      return String.join(";", deletedElementIds.toArray(new String[deletedElementIds.size()]));
    }
    return "";
  }
  public int getNumDeletedElements()
  {
    if (deletedElementIds != null)
    {
      return deletedElementIds.size();
    }
    return 0;
  }
  public int getNumValidationErrors() { return numValidationErrors; }
  public int getNumGroupsOfElementsFixed() { return numGroupsOfElementsFixed; }

  // these match corresponding entries in core MetadataTags class
  private static final String VALIDATED_TAG_KEY = "hoot:validation:error";
  private static final String FIXED_TAG_KEY = "hoot:validation:error:fixed";

  private enum FixStatus
  {
    NOT_ATTEMPTED, NONE_AVAILABLE, FAILED, SUCCEEDED;
  }

  // Not using PrimitiveId as keys here but don't know how to get it directly from OsmPrimitive;
  // using LinkedHashMultimap to preserve the value orderings between the various multimaps

  // element keys to validation error messages
  // e.g. key=Way:1, value1="Duplicated way nodes", value2="Unclosed way"
  private Multimap<String, String> elementValidations = LinkedHashMultimap.create();
  // element keys to validation error fix messages
  // e.g. key=Way:1, value1="Duplicated way nodes=fixed", value2="Unclosed way=none available"
  private Multimap<String, String> elementFixes = LinkedHashMultimap.create();

  // TODO
  private List<String> deletedElementIds = null;
  // TODO
  private int numValidationErrors = 0;
  // TODO
  private int numGroupsOfElementsFixed = 0;
}
