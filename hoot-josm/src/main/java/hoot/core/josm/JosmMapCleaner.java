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
package hoot.core.josm;

import java.util.Set;
import java.util.HashSet;
import java.util.Map;
import java.util.HashMap;
import java.util.Collection;
import java.lang.Exception;

import org.openstreetmap.josm.data.osm.OsmPrimitive;;
import org.openstreetmap.josm.data.validation.TestError;
import org.openstreetmap.josm.tools.Logging;
import org.openstreetmap.josm.command.Command;
import org.openstreetmap.josm.data.osm.DataSet;
import org.openstreetmap.josm.data.osm.PrimitiveId;

/**
 * Cleans a map using JOSM validators
 *
 * @see JosmMapCleaner in hoot-core
 */
public class JosmMapCleaner
{  
  /**
   * Constructor
   * @param addTags if true, elements being cleaned will be marked with custom tags
   */
  public JosmMapCleaner(boolean addTags) throws Exception
  {
    this.addTags = addTags;
  }

  /**
   * Clears member data
   */
  public void clear()
  {
    numElementsCleaned = 0;
    validationErrorFixesByType.clear();
    deletedElementIds.clear();
    failedCleaningOps.clear();
    addTags = false;
    if (elementErrorIndexes != null)
    {
      elementErrorIndexes.clear();
    }
  }

  public Set<String> getDeletedElementIds()
  {
    return deletedElementIds;
  }

  public int getNumDeletedElements()
  {
    return deletedElementIds.size();
  }

  public int getNumElementsCleaned() { return numElementsCleaned; }

  public int getNumFailedCleaningOperations()
  {
    return failedCleaningOps.size();
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

  public void setElementErrorIndexes(Map<String, Integer> elementErrorIndexes) throws Exception
  {
    if (addTags && (elementErrorIndexes == null || elementErrorIndexes.size() == 0))
    {
      throw new Exception("No element error indexes specified.");
    }
    this.elementErrorIndexes = elementErrorIndexes;
  }

  /**
   * Attempt to clean all features involved in a validation test error
   *
   * @param validationError a validation error identified by a validation test
   * @return the number of features cleaned
   */
  public int clean(TestError validationError) throws Exception
  {
    Logging.info("Cleaning element(s) with JOSM...");
    long startTime = System.currentTimeMillis();

    Collection<? extends OsmPrimitive> elementGroupWithError = validationError.getPrimitives();

    boolean cleanSuccess = false;
    //Logging.trace("error cleanable?: " + validationError.isFixable());
    CleanStatus cleanStatus = CleanStatus.NONE_AVAILABLE;
    int numCleaned = 0;
    if (validationError.isFixable())
    {
      Logging.trace(
        "Cleaning " + validationError.getPrimitives().size() + " elements for error: \"" +
        validationError.getMessage() + "\" found by test: " +
        validationError.getTester().getName() + "...");
      //Logging.trace(
      // "validationError.getPrimitives(): " +
      // JosmUtils.elementsToString(validationError.getPrimitives()));

      try
      {
        // get the command to use for cleaning; certain commands will try to create a window when
        // getFix is called, so make sure we trap that since we're headless
        Command cleanCmd = validationError.getFix();
        Logging.trace("cleanCmd: " + JosmUtils.commandToString(cleanCmd, true));

        // clean associated features based on the error found
        cleanSuccess = cleanCmd.executeCommand();
        Logging.trace("Success executing fix command: " + cleanCmd.getDescriptionText());

        // record any elements that were deleted
        Set<String> deletedElementIdsForCmd = JosmUtils.getDeletedElementIds(cleanCmd);
        Logging.trace("deletedElementIdsForCmd: " + deletedElementIdsForCmd);
        numElementsCleaned += deletedElementIdsForCmd.size();
        deletedElementIds.addAll(deletedElementIdsForCmd);
      }
      catch (Exception e)
      {
        failedCleaningOps.put(
          validationError.getTester().getName(),
          JosmUtils.getErrorMessage(validationError.getTester(), e));
        cleanSuccess = false;
      }

      if (cleanSuccess)
      {
        cleanStatus = CleanStatus.SUCCEEDED;
        numCleaned += elementGroupWithError.size();
      }
      else
      {
        cleanStatus = CleanStatus.FAILED;
      }
    }

    recordCleaningStatus(elementGroupWithError, cleanStatus, validationError);

    return numCleaned;
  }

  /**
   * Removes all elements whose fix was to be deleted as determined during cleaning
   *
   * @param map the map to remove deleted elements from
   */
  public void removeDeletedElements(DataSet map)
  {
    Logging.debug("Removing " + deletedElementIds.size() + " deleted elements...");
    Collection<OsmPrimitive> elements = map.allPrimitives();
    Collection<OsmPrimitive> outputElements = elements;
    for (OsmPrimitive element : elements)
    {
      String key = JosmUtils.getElementMapKey(element);
      if (deletedElementIds.contains(key))
      {
        map.removePrimitive(element.getPrimitiveId());
      }
    }
  }

  // matches corresponding entry in the hoot-core MetadataTags class
  private static final String VALIDATION_FIX_STATUS_TAG_KEY_BASE =
    "hoot:validation:error:clean:status";

  private enum CleanStatus
  {
    NONE_AVAILABLE, FAILED, SUCCEEDED;
  }

  // determines whether debug tags are added to cleaned elements
  boolean addTags = false;

  // @see JosmMapValidator.elementErrorIndexes
  private Map<String, Integer> elementErrorIndexes = null;

  // validators that threw an error during cleaning; validator name mapped to exception message
  private Map<String, String> failedCleaningOps = new HashMap<String, String>();

  // validation error types (validator names) mapped to successful cleaning counts
  private Map<String, Integer> validationErrorFixesByType = new HashMap<String, Integer>();

  // IDs for all elements deleted during cleaning
  private Set<String> deletedElementIds = new HashSet<String>();
  // each command attempts to clean a group of elements associated with a validation error
  private int numElementsCleaned = 0;

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

  /*
   * Record cleaning statuses and mark cleaned elements with tags
   */
  private void recordCleaningStatus(
    Collection<? extends OsmPrimitive> elementGroupWithError, CleanStatus cleanStatus,
    TestError validationError) throws Exception
  {
    String testName = validationError.getTester().getName();
    Logging.trace("Cleaning status for " + testName + ": " + cleanStatusToString(cleanStatus));

    if (cleanStatus == CleanStatus.SUCCEEDED)
    {
      numElementsCleaned += elementGroupWithError.size();
      Logging.trace("numElementsCleaned: " + numElementsCleaned);

      if (validationErrorFixesByType.containsKey(testName))
      {
        int currentFixCountForType = validationErrorFixesByType.get(testName);
        validationErrorFixesByType.put(
          testName, currentFixCountForType + elementGroupWithError.size());
      }
      else
      {
        validationErrorFixesByType.put(testName, elementGroupWithError.size());
      }
    }
    else if (!validationErrorFixesByType.containsKey(testName))
    {
      validationErrorFixesByType.put(testName, 0);
    }

    if (addTags)
    {
      //Logging.debug("Adding cleaning tags...");

      assert(elementErrorIndexes.size() > 0);
      for (OsmPrimitive element : elementGroupWithError)
      {
        String elementKey = JosmUtils.getElementMapKey(element);
        if (!deletedElementIds.contains(elementKey))
        {
          int errorIndex = elementErrorIndexes.get(elementKey + ";" + validationError.getMessage());
          element.put(
            VALIDATION_FIX_STATUS_TAG_KEY_BASE + ":" + String.valueOf(errorIndex),
            cleanStatusToString(cleanStatus));
        }
      }
    }
  }
}
