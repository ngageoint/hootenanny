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
package hoot.services.josm;

import hoot.services.josm.HootOsmReader;
import hoot.services.josm.JosmUtils;

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

import org.openstreetmap.josm.data.osm.TagMap;
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
public class JosmMapCleaner
{
  public JosmMapCleaner(String logLevel) throws Exception
  {
    JosmUtils.initJosm(logLevel);
  }

  /**
   * TODO - change to return Map<String, String>?
   */
  public String getAvailableCleaners() throws Exception
  {
    Logging.debug("Getting available cleaners...");

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
   * TODO - change to string array for cleaners?
   */
  public String clean(String cleanersStr, String elementsXml, boolean addDebugTags) throws Exception
  {
    String cleanedElementsXml = "";

    numValidationErrors = 0;
    numGroupsOfElementsCleaned = 0;

    // verify inputs

    Logging.debug("cleanersStr: " + cleanersStr);
    Logging.trace("elementsXml: " + elementsXml);
    Logging.debug("addDebugTags: " + addDebugTags);

    // we're always expecting populated cleaners
    if (cleanersStr == null || cleanersStr.trim().isEmpty())
    {
      throw new Exception("No cleaners specified.");
    }
    if (elementsXml == null || elementsXml.trim().isEmpty())
    {
      throw new Exception("No elements passed to cleaning.");
    }

    String[] cleaners = cleanersStr.split(";");
    Logging.info("Cleaning elements with " + cleaners.length + " cleaners...");
    Logging.debug("cleaners: " + Arrays.toString(cleaners));

    // read in the input element xml

    Collection<OsmPrimitive> elementsToClean = null;
    int originalMapSize = -1;
    try
    {
      Logging.debug("Converting input elements from xml...");;
      elementsToClean =
        HootOsmReader.parseDataSet(new ByteArrayInputStream(elementsXml.getBytes()))
          .getAllPrimitives();

      if (elementsToClean.size() == 0)
      {
        throw new Exception("No features passed to cleaning.");
      }

      originalMapSize = elementsToClean.size();
      Logging.debug("originalMapSize: " + originalMapSize);
      Logging.trace("input elements: " + JosmUtils.elementsToString(elementsToClean));
    }
    catch (Exception e)
    {
      Logging.error("Error converting cleaning input features to XML: " + e.getMessage());
      throw e;
    }

    // run the specified validation tests against the elements

    List<TestError> errors = null;
    Logging.debug("Running validation tests...");
    try
    {
      errors = runValidation(cleaners, elementsToClean);
      Logging.debug("errors size: " + errors.size());
    }
    catch (Exception e)
    {
      Logging.error("Error running validation tests: " + e.getMessage());
      throw e;
    }

    // check for any validation errors

    Collection<AbstractPrimitive> cleanedElements = null;
    int cleanedMapSize = -1;
    if (errors.size() > 0)
    {
      try
      {
        // record the features that were validated
        updateElementValidations(errors);

        // If we're fixing features, fix the features, record those that were fixed, and get back
        // the fixed features.

        cleanedElements = fixElements(errors);

        if (cleanedElements == null)
        {
          if (deletedElementIds == null || deletedElementIds.size() == 0)
          {
            Logging.trace("No elements cleaned. Using original input data for output...");
          }
          else
          {
            Logging.debug(
              deletedElementIds.size() +
              " elements cleaned. Modifying original input data for output...");
          }
          cleanedElements = new ArrayList<AbstractPrimitive>();
          cleanedElements.addAll(elementsToClean);
        }
        Logging.debug("cleanedElements size: " + cleanedElements.size());

        // add validation/fix message tags for use in hoot and remove deleted elements

        cleanedElements = getReturnElements(cleanedElements, addDebugTags);
        Logging.debug("cleanedElements size: " + cleanedElements.size());
      }
      catch (Exception e)
      {
        Logging.error("Error during cleaning of elements: " + e.getMessage());
        throw e;
      }
    }
    else
    {
      // If there weren't any validation errors, just use the unmodified input elements.

      Logging.trace("No elements cleaned. Using original input data for output...");
      cleanedElements = new ArrayList<AbstractPrimitive>();
      cleanedElements.addAll(elementsToClean);
      Logging.debug("cleanedElements size: " + cleanedElements.size());
    }
    cleanedMapSize = cleanedElements.size();

    int mapSizeDiff = 0;
    if (errors.size() == 0 || originalMapSize == cleanedMapSize)
    {
      Logging.debug("The cleaned map has the same number of elements as the input map.");
    }
    else if (originalMapSize < cleanedMapSize)
    {
      mapSizeDiff = cleanedMapSize - originalMapSize;
      Logging.debug(
        "The cleaned map has " + mapSizeDiff + " more elements than the input map.");
    }
    else if (cleanedMapSize < originalMapSize)
    {
      mapSizeDiff = originalMapSize - cleanedMapSize;
      Logging.debug(
        "The cleaned map has " + mapSizeDiff + " fewer elements than the input map.");
    }

    Logging.trace("output elements: " + JosmUtils.elementsToString(cleanedElements));

    // convert any validated elements back to xml; we'll just end up returning empty xml if no
    // errors were found

    // TODO: make sure if all elements get deleted here that empty map xml is returned and not an
    // empty xml string

    try
    {
      Logging.debug("Converting cleaned elements to xml...");
      cleanedElementsXml =
        OsmApi.getOsmApi("http://localhost").toBulkXml(cleanedElements, true);
      Logging.trace("cleanedElementsXml: " + cleanedElementsXml);
    }
    catch (Exception e)
    {
      Logging.error("Error converting cleaned elements back to XML: " + e.getMessage());
      throw e;
    }

    Logging.info(
      "Found " + numValidationErrors + " validation errors, cleaned " + numGroupsOfElementsCleaned +
      " groups of elements, and deleted " + getNumDeletedElements() + " elements.");

    return cleanedElementsXml;
  }

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

  private Collection<AbstractPrimitive> fixElements(List<TestError> errors) throws Exception
  {
    Logging.debug("Fixing and recording fixed elements...");

    Collection<AbstractPrimitive> cleanedElements = null;

    DataSet affectedData = null;
    deletedElementIds = new ArrayList<String>();
    for (TestError error : errors)
    {
      Collection<? extends OsmPrimitive> elementGroupWithError = error.getPrimitives();
      Logging.trace(
        "Processing " + error.getPrimitives().size() + " elements to clean for error: \"" +
        error.getMessage() + "\" found by test: " + error.getTester().getName() + "...");
      Logging.trace("error.getPrimitives(): " + JosmUtils.elementsToString(error.getPrimitives()));

      boolean cleanSuccess = false;
      Logging.trace("error cleanable?: " + error.isFixable());
      if (error.isFixable())
      {
        cleanSuccess = cleanValidatedElement(error, affectedData);
      }

      Logging.debug("Recording cleaned elements...");
      for (OsmPrimitive element : elementGroupWithError)
      {
        CleanStatus cleanStatus = CleanStatus.NONE_AVAILABLE;
        if (error.isFixable())
        {
          if (cleanSuccess)
          {
            cleanStatus = CleanStatus.SUCCEEDED;
            numGroupsOfElementsCleaned++;
          }
          else
          {
            cleanStatus = CleanStatus.FAILED;
          }
        }

        elementCleanings.put(JosmUtils.getElementMapKey(element), cleanStatusToString(cleanStatus));
      }
    }

    Logging.debug("elementCleanings size: " + elementCleanings.size());

    boolean affectedDataNull = (affectedData == null);
    Logging.trace("affectedData == null: " + affectedDataNull);
    if (affectedData != null)
    {
      cleanedElements = new ArrayList<AbstractPrimitive>();
      cleanedElements.addAll(affectedData.getAllPrimitives());
    }

    return cleanedElements;
  }

  private boolean cleanValidatedElement(TestError error, DataSet affectedData) throws Exception
  {
    // fix features based on error found
    Logging.trace(
      "Cleaning " + error.getPrimitives().size() + " elements for error: \"" + error.getMessage() +
      "\" found by test: " + error.getTester().getName() + "...");
    Logging.trace("error.getPrimitives(): " + JosmUtils.elementsToString(error.getPrimitives()));

    Command cleanCmd = error.getFix();
    Logging.trace("cleanCmd: " + JosmUtils.commandToString(cleanCmd, true));

    deletedElementIds.addAll(JosmUtils.getDeletedElementIds(cleanCmd));
    affectedData = cleanCmd.getAffectedDataSet();

    boolean cleanSuccess = cleanCmd.executeCommand();
    String cleanStatusStr = cleanSuccess ? "Success" : "Failure";
    Logging.trace(cleanStatusStr + " executing fix command: " + cleanCmd.getDescriptionText());
    return cleanSuccess;
  }

  private Collection<AbstractPrimitive> getReturnElements(Collection<AbstractPrimitive> elements,
    boolean addDebugTags) throws Exception
  {
    Logging.debug("Updating tags on up to " + elements.size() + " elements...");

    Collection<AbstractPrimitive> returnElements = new ArrayList<AbstractPrimitive>();

    int numValidationTagsAdded = 0;
    int numDeletedElements = 0;
    for (AbstractPrimitive element : elements)
    {
      OsmPrimitive osmElement = (OsmPrimitive)element;
      String elementKey = JosmUtils.getElementMapKey(osmElement);

      if (deletedElementIds == null || !deletedElementIds.contains(elementKey))
      {
        // Always add tags if addDebugTags=true, otherwise only add them if a fix couldn't be made
        // after a validation error. elementValidations and elementCleanings should always have the
        // same element keys.
        if (elementValidations.containsKey(elementKey) &&
            (addDebugTags ||
             !elementCleanings.get(elementKey).equals(cleanStatusToString(CleanStatus.SUCCEEDED))))
        {
          Logging.trace("Adding validation tags to element: " + elementKey + "...");

          Collection<String> errorMessages = elementValidations.get(elementKey);
          String[] errorMessagesArr = errorMessages.toArray(new String[errorMessages.size()]);
          Collection<String> cleaningMessages = elementCleanings.get(elementKey);
          String[] cleaningMessagesArr =
            cleaningMessages.toArray(new String[cleaningMessages.size()]);

          int errorCtr = 1;
          for (int i = 0; i < errorMessagesArr.length; i++)
          {
            // cleaningMessagesArr's ordering will match that of errorMessagesArr
            osmElement.put(
              VALIDATION_ERROR_TAG_KEY_BASE + ":" + String.valueOf(errorCtr), errorMessagesArr[i]);
            osmElement.put(
              VALIDATION_FIX_STATUS_TAG_KEY_BASE + ":" + String.valueOf(errorCtr),
              cleaningMessagesArr[i]);
            osmElement.put(VALIDATION_SOURCE_TAG_KEY_BASE + ":" + String.valueOf(errorCtr), "JOSM");
            numValidationTagsAdded++;
            errorCtr++;
          }
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
      "Added " + numValidationTagsAdded + " validation tags. " + numDeletedElements +
      " deleted elements were skipped. Total return elements: " + returnElements.size());
    return returnElements;
  }

  private String cleanStatusToString(CleanStatus cleanStatus) throws Exception
  {
    switch (cleanStatus)
    {
      case NONE_AVAILABLE:
        return "no cleaning available";
      case FAILED:
        return "cleaning failed";
      case SUCCEEDED:
        return "cleaning succeeded";
      default:
        throw new Exception("Invalid clean status: " + cleanStatus);
    }
  }

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
  public int getNumGroupsOfElementsCleaned() { return numGroupsOfElementsCleaned; }

  // these match corresponding entries in the hoot-core MetadataTags class
  private static final String VALIDATION_ERROR_TAG_KEY_BASE = "hoot:validation:error";
  private static final String VALIDATION_FIX_STATUS_TAG_KEY_BASE =
    "hoot:validation:error:clean:status";
  private static final String VALIDATION_SOURCE_TAG_KEY_BASE = "hoot:validation:error:source";

  private enum CleanStatus
  {
    NONE_AVAILABLE, FAILED, SUCCEEDED;
  }

  // Not using PrimitiveId as keys here but don't know how to get it directly from OsmPrimitive;
  // using LinkedHashMultimap to preserve the value orderings between the various multimaps

  // element keys to validation error messages
  // e.g. key=Way:1, value1="Duplicated way nodes", value2="Unclosed way"
  private Multimap<String, String> elementValidations = LinkedHashMultimap.create();
  // element keys to validation error fix messages
  // e.g. key=Way:1, value1="Duplicated way nodes=fixed", value2="Unclosed way=none available"
  private Multimap<String, String> elementCleanings = LinkedHashMultimap.create();

  // TODO
  private List<String> deletedElementIds = null;
  // TODO
  private int numValidationErrors = 0;
  // TODO
  private int numGroupsOfElementsCleaned = 0;
}
