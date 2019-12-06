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
import java.util.Map;
import java.util.HashMap;
import java.util.Collection;
import java.lang.Exception;

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
 * TODO
 */
public class JosmMapCleaner extends JosmMapValidator
{
  public JosmMapCleaner(String logLevel) throws Exception
  {
    super(logLevel);
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
  public int getNumGroupsOfElementsCleaned() { return numGroupsOfElementsCleaned; }
  public String getValidationErrorFixCountsByType()
  {
    Logging.trace("Retrieving validation error fix counts by type...");
    Logging.trace("validationErrorFixesByType size: " + validationErrorFixesByType.size());

    String returnStr = "";
    for (Map.Entry<String, Integer> entry : validationErrorFixesByType.entrySet())
    {
      returnStr += entry.getKey() + ":" + String.valueOf(entry.getValue()) + ";";
    }
    returnStr = StringUtils.chop(returnStr);
    return returnStr;
  }

  /**
   * TODO - change to string array for cleaners?
   */
  public String clean(String validatorsStr, String elementsXml, boolean addDebugTags)
    throws Exception
  {
    Logging.debug("addDebugTags: " + addDebugTags);

    clear();

    /*outputElements = */parseAndValidateElements(validatorsStr, elementsXml);

    // check for any validation errors

    int cleanedMapSize = -1;
    if (validationErrors.size() > 0)
    {
      try
      {
        // If we're fixing features, fix the features, record those that were fixed, and get back
        // the fixed features.

        outputElements = cleanElements(validationErrors);

        if (outputElements == null)
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
          outputElements = new ArrayList<AbstractPrimitive>();
          outputElements.addAll(inputElements);
        }
        Logging.debug("cleanedElements size: " + outputElements.size());

        // add validation/fix message tags for use in hoot and remove deleted elements

        outputElements = getReturnElements(outputElements, addDebugTags);
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

    Logging.info(
      "Found " + numValidationErrors + " validation errors, cleaned " + numGroupsOfElementsCleaned +
      " groups of elements, and deleted " + getNumDeletedElements() + " elements.");

    return convertOutputElementsToXml();
  }

  /////////////////////////////////////////////////////////////////////////////////////////////

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

  private Collection<AbstractPrimitive> cleanElements(List<TestError> errors) throws Exception
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
        Logging.trace(
          "elementValidations.containsKey(elementKey): " +
          elementValidations.containsKey(elementKey));
        Logging.trace("addDebugTags: " + addDebugTags);
        Logging.trace("elementCleanings.get(elementKey): " + elementCleanings.get(elementKey));
        if (elementValidations.containsKey(elementKey) && addDebugTags)
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

  // Not using PrimitiveId as keys here but don't know how to get it directly from OsmPrimitive;
  // using LinkedHashMultimap to preserve the value orderings between the various multimaps

  // element keys to validation error fix messages
  // e.g. key=Way:1, value1="Duplicated way nodes=fixed", value2="Unclosed way=none available"
  private Multimap<String, String> elementCleanings = LinkedHashMultimap.create();

  private Map<String, Integer> validationErrorFixesByType = new HashMap<String, Integer>();

  // TODO
  private List<String> deletedElementIds = null;
  // TODO
  private int numGroupsOfElementsCleaned = 0;
}
