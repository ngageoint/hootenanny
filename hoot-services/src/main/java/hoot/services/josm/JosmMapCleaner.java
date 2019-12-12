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

import hoot.services.josm.JosmMapValidator;
import hoot.services.josm.JosmUtils;

import java.util.List;
import java.util.ArrayList;
import java.util.Set;
import java.util.HashSet;
import java.util.Map;
import java.util.HashMap;
import java.util.Collection;
import java.lang.Exception;
import java.awt.AWTException;

import org.apache.commons.lang3.StringUtils;

import com.google.common.collect.Multimap;
import com.google.common.collect.LinkedHashMultimap;

import org.openstreetmap.josm.data.osm.AbstractPrimitive;
import org.openstreetmap.josm.data.osm.OsmPrimitive;;
import org.openstreetmap.josm.data.validation.TestError;
import org.openstreetmap.josm.tools.Logging;
import org.openstreetmap.josm.command.Command;
import org.openstreetmap.josm.data.osm.DataSet;

/**
 * Cleans a map using JOSM validators
 *
 * The interface for this class is kept purposefully coarse, so as to result in the smallest number
 * of JNI calls from clients as possible.
 *
 * @see JosmMapCleaner in hoot-core
 */
public class JosmMapCleaner extends JosmMapValidator
{
  public JosmMapCleaner(
    String logLevel, String userCertPath, String userCertPassword) throws Exception
  {
    super(logLevel, userCertPath, userCertPassword);
  }

  public Set<String> getDeletedElementIds()
  {
    if (deletedElementIds != null)
    {
      return deletedElementIds;
    }
    return new HashSet<String>();
  }
  public int getNumDeletedElements()
  {
    if (deletedElementIds != null)
    {
      return deletedElementIds.size();
    }
    return 0;
  }
  public int getNumGroupsOfElementsCleaned() { return numGroupsOfElementsCleaned; }
  public int getNumFailedCleaningOperations()
  {
    if (failedCleaningOps != null)
    {
      return failedCleaningOps.size();
    }
    return 0;
  }

  /**
   * Returns the counts of elements that were cleaned, organized by validation error type, during
   * map validation
   *
   * @return a delimited string of the form:
   * <validation error 1 name>:<cleaned element count for validation error 1>;
   * <validation error 2 name>:<cleaned element count for validation error 2>...
   */
  public Map<String, Integer> getValidationErrorFixCountsByType()
  {
    return validationErrorFixesByType;
  }

  /**
   * Runs selected JOSM validation routines on a map and cleans a subset of the elements that fail
   * validation with
   *
   * @param validatorsStr semicolon delimited string with the full Java class names of the
   * validators to be used
   * @param elementsXml input OSM map to be cleaned as XML
   * @param addDetailTags if true, elements failing validation are tagged with validation error
   * descriptions and an indication of whether they were cleaned or not; TODO: The IDs of deleted
   * elements are recorded in theoutput  map's base tags.
   */
  public String clean(List<String> validators, String elementsXml, boolean addDetailTags)
    throws Exception
  {
    Logging.debug("Cleaning map with " + validators.size() + " validators...");
    Logging.debug("addDetailTags: " + addDetailTags);

    // clear out existing data and stats
    clear();

    // validate the elements
    /*outputElements = */parseAndValidateElements(validators, elementsXml);

    // check for any validation errors

    int cleanedMapSize = -1;
    if (validationErrors.size() > 0)
    {
      try
      {
        // clean the validated features and record that those that were successfully cleaned

        outputElements = cleanValidatedElements(validationErrors);

        if (failedCleaningOps.size() > 0)
        {
          Logging.warn(
            "The following JOSM cleaning operations failed: " + failedCleaningOps.keySet());
        }

        if (outputElements == null)
        {
          if (deletedElementIds == null || deletedElementIds.size() == 0)
          {
            Logging.debug("No elements cleaned. Using original input data for output...");
          }
          else
          {
            Logging.debug(
              deletedElementIds.size() +
              " elements cleaned. Modifying original input data for output...");
          }
          outputElements = new ArrayList<AbstractPrimitive>();
          outputElements.addAll(inputElements);
        }
        Logging.debug("cleanedElements size: " + outputElements.size());

        // optionally add validation/fix message tags and also remove deleted elements

        outputElements = getReturnElements(outputElements, addDetailTags);
        Logging.debug("outputElements size: " + outputElements.size());
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
      outputElements = new ArrayList<AbstractPrimitive>();
      outputElements.addAll(inputElements);
      Logging.debug("outputElements size: " + outputElements.size());
    }

    Logging.debug(
      "Found " + getNumValidationErrors() + " validation errors, cleaned " +
      numGroupsOfElementsCleaned + " groups of elements, and deleted " + getNumDeletedElements() +
      " elements.");

    return convertOutputElementsToXml();
  }

  /////////////////////////////////////////////////////////////////////////////////////////////

  /*
   * @see JosmMapValidator
   */
  protected void clear()
  {
    super.clear();

    numGroupsOfElementsCleaned = 0;
    if (elementCleanings != null)
    {
      elementCleanings.clear();
    }
    if (validationErrorFixesByType != null)
    {
      validationErrorFixesByType.clear();
    }
    if (deletedElementIds != null)
    {
      deletedElementIds.clear();
    }
  }

  /////////////////////////////////////////////////////////////////////////////////////////////

  /*
   * Attempts to clean all elements involved in a collection of validation errors
   */
  private Collection<AbstractPrimitive> cleanValidatedElements(List<TestError> errors)
    throws Exception
  {
    Logging.info("Cleaning elements with JOSM...");
    long startTime = System.currentTimeMillis();

    Collection<AbstractPrimitive> cleanedElements = null;

    DataSet affectedData = null;
    deletedElementIds = new HashSet<String>();
    for (TestError error : errors)
    {
      Collection<? extends OsmPrimitive> elementGroupWithError = error.getPrimitives();

      boolean cleanSuccess = false;
      Logging.trace("error cleanable?: " + error.isFixable());
      if (error.isFixable())
      {
        cleanSuccess = cleanValidatedElementsForError(error, affectedData);
      }

      Logging.trace("Recording cleaned elements...");
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

        // record the elements cleaned by ID
        elementCleanings.put(JosmUtils.getElementMapKey(element), cleanStatusToString(cleanStatus));

        // record the elements cleaned by validation error type
        if (cleanSuccess)
        {
          if (validationErrorFixesByType.containsKey(error.getTester().getName()))
          {
            int currentFixCountForType = validationErrorFixesByType.get(error.getTester().getName());
            validationErrorFixesByType.put(error.getTester().getName(), currentFixCountForType + 1);
          }
          else
          {
            validationErrorFixesByType.put(error.getTester().getName(), 1);
          }
        }
        else if (!validationErrorFixesByType.containsKey(error.getTester().getName()))
        {
          validationErrorFixesByType.put(error.getTester().getName(), 0);
        }
      }
    }
    Logging.debug("elementCleanings size: " + elementCleanings.size());

    // record the updated state of the map after cleaning
    boolean affectedDataNull = (affectedData == null);
    Logging.trace("affectedData == null: " + affectedDataNull);
    if (affectedData != null)
    {
      cleanedElements = new ArrayList<AbstractPrimitive>();
      cleanedElements.addAll(affectedData.getAllPrimitives());
    }

    Logging.debug(
      "Cleaned " + elementCleanings.size() + " elements in: " +
      String.valueOf((System.currentTimeMillis() - startTime) / 1000) + " seconds.");

    return cleanedElements;
  }

  /*
   * Cleans a collection of elements associated with a single validation test error
   */
  private boolean cleanValidatedElementsForError(TestError error, DataSet affectedData)
    throws Exception
  {
    Logging.trace(
      "Cleaning " + error.getPrimitives().size() + " elements for error: \"" + error.getMessage() +
      "\" found by test: " + error.getTester().getName() + "...");
    //Logging.trace("error.getPrimitives(): " + JosmUtils.elementsToString(error.getPrimitives()));

    boolean cleanSuccess = false;
    try
    {
      // get the command to use for cleaning; certain commands will try to create a window when
      // getFix is called, so make sure we trap that since we're headless
      Command cleanCmd = error.getFix();
      Logging.trace("cleanCmd: " + JosmUtils.commandToString(cleanCmd, true));
      // clean associated features based on the error found
      cleanSuccess = cleanCmd.executeCommand();
      Logging.trace("Success executing fix command: " + cleanCmd.getDescriptionText());
      // record any elements that were deleted
      deletedElementIds.addAll(JosmUtils.getDeletedElementIds(cleanCmd));
      // grab the actual data from the command so we can update our return map
      affectedData = cleanCmd.getAffectedDataSet();
    }
    catch (Exception e)
    {
      failedCleaningOps.put(error.getTester().getName(), getErrorMessage(error.getTester(), e));
      cleanSuccess = false;
    }

    return cleanSuccess;
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

  private Collection<AbstractPrimitive> getReturnElements(Collection<AbstractPrimitive> elements,
    boolean addDetailTags) throws Exception
  {
    Logging.debug("Updating tags on up to " + elements.size() + " elements...");

    Collection<AbstractPrimitive> returnElements = new ArrayList<AbstractPrimitive>();

    int numValidationTagsAdded = 0;
    int numDeletedElements = 0;
    for (AbstractPrimitive element : elements)
    {
      OsmPrimitive osmElement = (OsmPrimitive)element;
      String elementKey = JosmUtils.getElementMapKey(osmElement);

      // don't add the feature to the output map if it was deleted
      if (deletedElementIds == null || !deletedElementIds.contains(elementKey))
      {
        // elementValidations and elementCleanings should always have the same element keys.
        Logging.trace(
          "elementValidations.containsKey(elementKey): " +
          elementValidations.containsKey(elementKey));
        Logging.trace("addDetailTags: " + addDetailTags);
        Logging.trace("elementCleanings.get(elementKey): " + elementCleanings.get(elementKey));
        if (elementValidations.containsKey(elementKey) && addDetailTags)
        {
          Logging.trace("Adding validation tags to element: " + elementKey + "...");

          Collection<String> errorMessages = elementValidations.get(elementKey);
          // cleaningMessagesArr's ordering will match that of errorMessagesArr
          String[] errorMessagesArr = errorMessages.toArray(new String[errorMessages.size()]);
          Collection<String> cleaningMessages = elementCleanings.get(elementKey);
          String[] cleaningMessagesArr =
            cleaningMessages.toArray(new String[cleaningMessages.size()]);

          int errorCtr = 1;
          for (int i = 0; i < errorMessagesArr.length; i++)
          {
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
      "Added " + numValidationTagsAdded + " validation error/fix status tags. " +
      numDeletedElements + " deleted elements were skipped. Total return elements: " +
      returnElements.size());
    return returnElements;
  }

  //////////////////////////////////////////////////////////////////////////////////////////////

  // matches corresponding entry in the hoot-core MetadataTags class
  private static final String VALIDATION_FIX_STATUS_TAG_KEY_BASE =
    "hoot:validation:error:clean:status";

  private enum CleanStatus
  {
    NONE_AVAILABLE, FAILED, SUCCEEDED;
  }

  // element keys to validation error cleaning statuses; see related notes for
  // JosmMapValidator::elementValidations
  // e.g. key=Way:1, value1="Duplicated way nodes=fixed", value2="Unclosed way=none available"
  private Multimap<String, String> elementCleanings = LinkedHashMultimap.create();
  // a list of names of validators that threw an error during cleaning
  private Map<String, String> failedCleaningOps = new HashMap<String, String>();

  // validation error types (validator names) mapped to successful cleaning counts
  private Map<String, Integer> validationErrorFixesByType = new HashMap<String, Integer>();

  // list of IDs for all elements deleted during cleaning
  private Set<String> deletedElementIds = null;
  // each command attempts to clean a group of elements associated with a validation error
  private int numGroupsOfElementsCleaned = 0;
}
