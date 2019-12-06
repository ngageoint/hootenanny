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

import org.apache.commons.lang3.StringUtils;

import com.google.common.collect.Multimap;
import com.google.common.collect.LinkedHashMultimap;

import org.openstreetmap.josm.data.osm.AbstractPrimitive;
import org.openstreetmap.josm.data.osm.OsmPrimitive;
import org.openstreetmap.josm.data.validation.OsmValidator;
import org.openstreetmap.josm.data.validation.Test;
import org.openstreetmap.josm.data.validation.TestError;
import org.openstreetmap.josm.tools.Logging;
import org.openstreetmap.josm.io.OsmApi;

/**
 * TODO
 */
public class JosmMapValidator
{
  public JosmMapValidator(String logLevel) throws Exception
  {
    JosmUtils.initJosm(logLevel);
  }

  public int getNumValidationErrors() { return numValidationErrors; }

  public String getValidationErrorCountsByType()
  {
    Logging.trace("Retrieving validation error counts by type...");
    Logging.trace("validationErrorCountsByType size: " + validationErrorCountsByType.size());
    String returnStr = "";
    for (Map.Entry<String, Integer> entry : validationErrorCountsByType.entrySet())
    {
      returnStr += entry.getKey() + ":" + String.valueOf(entry.getValue()) + ";";
    }
    returnStr = StringUtils.chop(returnStr);
    Logging.trace("returnStr: " + returnStr);
    return returnStr;
  }

  /**
   * TODO - change to return Map<String, String>?
   */
  public String getAvailableValidators() throws Exception
  {
    Logging.debug("Getting available validators...");

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
  public String validate(String validatorsStr, String elementsXml) throws Exception
  {
    clear();

    // check for any validation errors
    outputElements = parseAndValidateElements(validatorsStr, elementsXml);
    Logging.info("Found " + numValidationErrors + " validation errors.");

    outputElements = getReturnElements(outputElements);
    Logging.debug("outputElements size: " + outputElements.size());

    return convertOutputElementsToXml();
  }

  /////////////////////////////////////////////////////////////////////////////////////////////

  protected void clear()
  {
    numValidationErrors = 0;
    originalMapSize = 0;
    if (inputElements != null)
    {
      inputElements.clear();
    }
    if (outputElements != null)
    {
      outputElements.clear();
    }
    if (validationErrors != null)
    {
      validationErrors.clear();
    }
    if (elementValidations != null)
    {
      elementValidations.clear();
    }
    if (validationErrorCountsByType != null)
    {
      validationErrorCountsByType.clear();
    }
  }

  protected Collection<AbstractPrimitive> parseAndValidateElements(
    String validatorsStr, String elementsXml) throws Exception
  {
    // verify inputs

    Logging.debug("validatorsStr: " + validatorsStr);
    Logging.trace("elementsXml: " + elementsXml);

    // we're always expecting populated validators
    if (validatorsStr == null || validatorsStr.trim().isEmpty())
    {
      throw new Exception("No validators specified.");
    }
    if (elementsXml == null || elementsXml.trim().isEmpty())
    {
      throw new Exception("No elements passed to validation.");
    }

    String[] validators = validatorsStr.split(";");

    Logging.info("Validating elements with " + validators.length + " validators...");
    Logging.debug("validators: " + Arrays.toString(validators));

    Collection<AbstractPrimitive> validatedElements = null;

    // read in the input element xml

    try
    {
      Logging.debug("Converting input elements from xml...");
      inputElements =
        HootOsmReader.parseDataSet(new ByteArrayInputStream(elementsXml.getBytes()))
          .getAllPrimitives();

      if (inputElements.size() == 0)
      {
        throw new Exception("No features passed to validation.");
      }

      originalMapSize = inputElements.size();
      Logging.debug("originalMapSize: " + originalMapSize);
      Logging.trace("input elements: " + JosmUtils.elementsToString(inputElements));
    }
    catch (Exception e)
    {
      Logging.error("Error converting validation input features to XML: " + e.getMessage());
      throw e;
    }

    // run the specified validation tests against the elements

    Logging.debug("Running validation tests...");
    try
    {
      validationErrors = runValidation(validators, inputElements);
      Logging.debug("validationErrors size: " + validationErrors.size());
    }
    catch (Exception e)
    {
      Logging.error("Error running validation tests: " + e.getMessage());
      throw e;
    }

    if (validationErrors.size() > 0)
    {
      try
      {
        // record the features that were validated
        updateElementValidations(validationErrors);

        validatedElements = new ArrayList<AbstractPrimitive>();
        validatedElements.addAll(inputElements);
        Logging.debug("validatedElements size: " + validatedElements.size());
      }
      catch (Exception e)
      {
        Logging.error("Error during validation of elements: " + e.getMessage());
        throw e;
      }
    }
    else
    {
      // If there weren't any validation errors, just use the unmodified input elements.

      Logging.trace("No elements validated. Using original input data for output...");
      validatedElements = new ArrayList<AbstractPrimitive>();
      validatedElements.addAll(inputElements);
      Logging.debug("validatedElements size: " + validatedElements.size());
    }

    return validatedElements;
  }

  protected String convertOutputElementsToXml()
  {
    int validatedMapSize = outputElements.size();
    int mapSizeDiff = 0;
    if (validationErrors.size() == 0 || originalMapSize == validatedMapSize)
    {
      Logging.debug("The output map has the same number of elements as the input map.");
    }
    else if (originalMapSize < validatedMapSize)
    {
      mapSizeDiff = validatedMapSize - originalMapSize;
      Logging.debug(
        "The output map has " + mapSizeDiff + " more elements than the input map.");
    }
    else if (validatedMapSize < originalMapSize)
    {
      mapSizeDiff = originalMapSize - validatedMapSize;
      Logging.debug(
        "The output map has " + mapSizeDiff + " fewer elements than the input map.");
    }

    Logging.trace("output elements: " + JosmUtils.elementsToString(outputElements));

    // convert any validated elements back to xml; we'll just end up returning empty xml if no
    // errors were found

    // TODO: make sure if all elements get deleted here that empty map xml is returned and not an
    // empty xml string

    String outputElementsXml = null;
    try
    {
      Logging.debug("Converting validated elements to xml...");
      outputElementsXml =
        OsmApi.getOsmApi("http://localhost").toBulkXml(outputElements, true);
      Logging.trace("outputElementsXml: " + outputElementsXml);
    }
    catch (Exception e)
    {
      Logging.error("Error converting output elements to XML: " + e.getMessage());
      throw e;
    }

    return outputElementsXml;
  }

  /////////////////////////////////////////////////////////////////////////////////////////////

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

  private int getNumElementsInvolvedInErrors(List<TestError> errors)
  {
    int numElementsInvolved = 0;
    for (TestError error : errors)
    {
      numElementsInvolved += error.getPrimitives().size();
    }
    return numElementsInvolved;
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

      if (validationErrorCountsByType.containsKey(error.getTester().getName()))
      {
        int currentErrorCountForType = validationErrorCountsByType.get(error.getTester().getName());
        validationErrorCountsByType.put(error.getTester().getName(), currentErrorCountForType + 1);
      }
      else
      {
        validationErrorCountsByType.put(error.getTester().getName(), 1);
      }
    }

    Logging.debug("elementValidations size: " + elementValidations.size());
  }

  private Collection<AbstractPrimitive> getReturnElements(Collection<AbstractPrimitive> elements)
    throws Exception
  {
    Logging.debug("Updating tags on up to " + elements.size() + " elements...");

    Collection<AbstractPrimitive> returnElements = new ArrayList<AbstractPrimitive>();

    int numValidationTagsAdded = 0;
    int numDeletedElements = 0;
    for (AbstractPrimitive element : elements)
    {
      OsmPrimitive osmElement = (OsmPrimitive)element;
      String elementKey = JosmUtils.getElementMapKey(osmElement);

      if (elementValidations.containsKey(elementKey))
      {
        Logging.trace("Adding validation tags to element: " + elementKey + "...");

        Collection<String> errorMessages = elementValidations.get(elementKey);
        String[] errorMessagesArr = errorMessages.toArray(new String[errorMessages.size()]);

        int errorCtr = 1;
        for (int i = 0; i < errorMessagesArr.length; i++)
        {
          osmElement.put(
            VALIDATION_ERROR_TAG_KEY_BASE + ":" + String.valueOf(errorCtr), errorMessagesArr[i]);
          osmElement.put(VALIDATION_SOURCE_TAG_KEY_BASE + ":" + String.valueOf(errorCtr), "JOSM");
          numValidationTagsAdded++;
          errorCtr++;
        }
      }
      Logging.trace("Adding return element: " + elementKey + "...");
      returnElements.add(osmElement);
    }

    Logging.debug(
      "Added " + numValidationTagsAdded + " validation error tags. Total return " +
      "elements: " + returnElements.size());
    return returnElements;
  }

  //////////////////////////////////////////////////////////////////////////////////////////////

  // these match corresponding entries in the hoot-core MetadataTags class
  protected static final String VALIDATION_ERROR_TAG_KEY_BASE = "hoot:validation:error";
  protected static final String VALIDATION_SOURCE_TAG_KEY_BASE = "hoot:validation:error:source";

  protected Collection<OsmPrimitive> inputElements;
  protected Collection<AbstractPrimitive> outputElements;

  protected List<TestError> validationErrors;
  // TODO
  protected int numValidationErrors = 0;

  // Not using PrimitiveId as keys here but don't know how to get it directly from OsmPrimitive;
  // using LinkedHashMultimap to preserve the value orderings between the various multimaps

  // element keys to validation error messages
  // e.g. key=Way:1, value1="Duplicated way nodes", value2="Unclosed way"
  protected Multimap<String, String> elementValidations = LinkedHashMultimap.create();

  ///////////////////////////////////////////////////////////////

  private int originalMapSize = 0;

  private Map<String, Integer> validationErrorCountsByType = new HashMap<String, Integer>();
}
