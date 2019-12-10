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
//import org.openstreetmap.josm.data.osm.DataSet;

/**
 * Validates a map using JOSM validators
 *
 * @see JosmMapValidator in hoot-core
 */
public class JosmMapValidator
{
  public JosmMapValidator(String logLevel) throws Exception
  {
    JosmUtils.initJosm(logLevel);
  }

  public int getNumValidationErrors()
  {
    if (validationErrors != null)
    {
      return validationErrors.size();
    }
    return 0;
  }

  /**
   * Returns the validation error counts, organized by validation error type, found during map
   * validation
   *
   * @return a delimited string of the form:
   * <validation error 1 name>:<validation error 1 count>;
   * <validation error 2 name>:<validation error 2 count>...
   */
  public Map<String, Integer> getValidationErrorCountsByType()
  {
    return validationErrorCountsByType;
  }

  /**
   * Returns the available JOSM validators
   *
   * @return a delimited string of the form:
   * <validator 1 class name>:<validator 1 description>;<validator 2 class name>:<validator 2 description>
   */
  public Map<String, String> getAvailableValidators() throws Exception
  {
    Logging.debug("Retrieving available validators...");
    Map<String, String> validators = new HashMap<String, String>();
    try
    {
      Collection<Test> validationTests = OsmValidator.getTests();
      for (Test validationTest : validationTests)
      {
        if (validationTest != null)
        {
          String testName = validationTest.toString().split("@")[0];
          // converting inner class '$' char over to '.' for readability
          testName = testName.replace("$", ".");
          // not returning the namespace, so remove it
          testName = testName.replace(VALIDATORS_NAMESPACE + ".", "");
          Logging.trace("testName: " + testName);
          String testDescription = validationTest.getName();
          validators.put(testName, testDescription);
        }
      }
    }
    catch (Exception e)
    {
      System.out.println(e.getMessage());
      throw e;
    }
    return validators;
  }

  /**
   * Runs selected JOSM validation routines on a map and marks elements that fail validation with
   * custom tags
   *
   * @param validatorsStr semicolon delimited string with the full Java class names of the
   * validators to be used
   * @param elementsXml input OSM map to be validated as XML
   * @return modified OSM map XML string if validation errors were found; otherwise a map identical
   * to the input map
   */
  public String validate(List<String> validators, String elementsXml) throws Exception
  {
    Logging.info("Validating map with " + validators.size() + " validators...");

    // clear out existing data and stats
    clear();

    // validate the elements
    outputElements = parseAndValidateElements(validators, elementsXml);

    // update modified map
    outputElements = getReturnElements(outputElements);
    Logging.debug("outputElements size: " + outputElements.size());

    // return the modified map as xml
    return convertOutputElementsToXml();
  }

  /////////////////////////////////////////////////////////////////////////////////////////////

  /*
   * Clear out member data
   */
  protected void clear()
  {
    //numValidationErrors = 0;
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
    failingValidators.clear();
  }

  protected Collection<AbstractPrimitive> parseAndValidateElements(
    List<String> validators, String elementsXml) throws Exception
  {
    long startTime = -1;

    // verify inputs

    validators = parseValidatorsInput(validators);
    //Logging.trace("elementsXml: " + elementsXml);
    if (elementsXml == null || elementsXml.trim().isEmpty())
    {
      throw new Exception("No elements passed to validation.");
    }

    Collection<AbstractPrimitive> validatedElements = null;

    // read in the input element map xml

    try
    {
      Logging.info("Converting input elements from xml...");
      startTime = System.currentTimeMillis();
      //ByteArrayInputStream bytes = new ByteArrayInputStream(elementsXml.getBytes());
      //DataSet dataset = HootOsmReader.parseDataSet(new ByteArrayInputStream(elementsXml.getBytes()));
      inputElements =
        HootOsmReader.parseDataSet(new ByteArrayInputStream(elementsXml.getBytes()))
          .getAllPrimitives();
      Logging.info(
        "Input elements converted from xml in: " +
        String.valueOf((System.currentTimeMillis() - startTime) / 1000) + " seconds.");
      Logging.debug("inputElements.size(): " + inputElements.size());

      if (inputElements.size() == 0)
      {
        throw new Exception("No features passed to validation.");
      }

      originalMapSize = inputElements.size();
      Logging.debug("originalMapSize: " + originalMapSize);
      //Logging.trace("input elements: " + JosmUtils.elementsToString(inputElements));
    }
    catch (Exception e)
    {
      Logging.error("Error converting validation input features to XML: " + e.getMessage());
      throw e;
    }

    // run the specified validators against the elements

    Logging.info("Running validation tests...");
    try
    {
      startTime = System.currentTimeMillis();
      validationErrors = runValidation(validators, inputElements);
      Logging.info(
        "Found " + validationErrors.size() + " validation errors in: " +
        String.valueOf((System.currentTimeMillis() - startTime) / 1000) + " seconds.");
      if (failingValidators.size() > 0)
      {
        Logging.error("The following validators failed: " + failingValidators);
      }
    }
    catch (Exception e)
    {
      Logging.error("Error running validation tests: " + e.getMessage());
      throw e;
    }

    if (validationErrors.size() > 0)
    {
      // record the features that were validated

      try
      {
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
      // If there weren't any validation errors, just return the unmodified input map.

      Logging.trace("No elements validated. Using original input data for output...");
      validatedElements = new ArrayList<AbstractPrimitive>();
      validatedElements.addAll(inputElements);
      Logging.debug("validatedElements size: " + validatedElements.size());
    }

    return validatedElements;
  }

  protected String convertOutputElementsToXml()
  {
    Logging.info("Converting output elements to XML...");
    long startTime = System.currentTimeMillis();

    // gather some stats on the modified map

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

    // convert any validated elements back to xml

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

    Logging.info(
      "Output elements converted to xml in: " +
      String.valueOf((System.currentTimeMillis() - startTime) / 1000) + " seconds.");

    return outputElementsXml;
  }

  /////////////////////////////////////////////////////////////////////////////////////////////

  private List<String> parseValidatorsInput(List<String> validators) throws Exception
  {
    Logging.debug("input validators: " + validators);
    // we're always expecting populated validators
    if (validators.size() == 0)
    {
      throw new Exception("No validators specified.");
    }
    List<String> validatorsOut = new ArrayList<String>();
    // Incoming validators don't have the Java namespace prefixed, so add it here.
    for (String validator : validators)
    {
      String validatorName = validator;
      Logging.trace("validatorName: " + validatorName);
      // convert '.' back to '$' to handle inner classes; inner classes are the only reason a '.'
      // should be in the validator input
      validatorName = validatorName.replace(".", "$");
      if (validatorName.contains("."))
      {
        throw new Exception("Validators must not have a Java namespace prefixed.");
      }
      // prepend the Java namespace to the validator
      validatorName = VALIDATORS_NAMESPACE + "." + validatorName;
      Logging.trace("validatorName: " + validatorName);
      validatorsOut.add(validatorName);
    }
    Logging.debug("validatorsOut: " + validatorsOut);
    return validatorsOut;
  }

  /*
   * Runs a set of validators against specified input elements
   */
  private List<TestError> runValidation(List<String> validators, Collection<OsmPrimitive> elements)
    throws Exception
  {
    List<TestError> errors = new ArrayList<TestError>();
    for (String validatorStr : validators)
    {
      Logging.trace("validatorStr: " + validatorStr);
      Test validator = (Test)Class.forName(validatorStr).newInstance();
      errors.addAll(runValidation(validator, elements));
    }
    return errors;
  }

  /*
   * Runs a set of validators against specified input elements
   */
  private List<TestError> runValidation(
    Collection<Test> validators, Collection<OsmPrimitive> elements) throws Exception
  {
    List<TestError> errors = new ArrayList<TestError>();
    for (Test validator : validators)
    {
      errors.addAll(runValidation(validator, elements));
    }
    return errors;
  }

  /*
   * Runs a single validator against specified input elements
   */
  private List<TestError> runValidation(Test validator, Collection<OsmPrimitive> elements)
    throws Exception
  {
    Logging.debug("Running validator: " + validator.getName() + "...");

    List<TestError> errors = new ArrayList<TestError>();

    try
    {
      validator.initialize();
      validator.setPartialSelection(false);
      validator.startTest(null);
      validator.visit(elements);
      validator.endTest();

      errors.addAll(validator.getErrors());
      validator.clear();

      Logging.info(
        "Validator: " + validator.getName() + " found " + errors.size() + " errors with " +
        getNumElementsInvolvedInErrors(errors) + " total involved elements.");
    }
    catch (Exception e)
    {
      Logging.error("Error running validator: " + validator.getName());
      failingValidators.put(validator.getName(), e.getMessage());
    }

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
    Logging.info("Recording validated elements...");

    for (TestError error : errors)
    {
      Collection<? extends OsmPrimitive> elementGroupWithError = error.getPrimitives();
      Logging.trace(
        "Processing validation results for " + error.getPrimitives().size() + " elements for " +
        " error: \"" + error.getMessage() + "\" found by test: " + error.getTester().getName() +
        "...");
      Logging.trace("error.getPrimitives(): " + JosmUtils.elementsToString(error.getPrimitives()));

      // map validated elements by ID and error that occurred
      for (OsmPrimitive element : elementGroupWithError)
      {
        elementValidations.put(JosmUtils.getElementMapKey(element), error.getMessage());
      }

      // map validation errors by type
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

  /*
   * Mark elements which failed validation with tags
   *
   * @param elements the elements to tag
   * @return an updated collection of elements
   */
  private Collection<AbstractPrimitive> getReturnElements(Collection<AbstractPrimitive> elements)
    throws Exception
  {
    Logging.info("Updating tags on up to " + elements.size() + " elements...");

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

        // Add a separate tag for each validation error this element was involved in.
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

    Logging.info(
      "Added " + numValidationTagsAdded + " validation error tags. Total return " +
      "elements: " + returnElements.size());
    return returnElements;
  }

  //////////////////////////////////////////////////////////////////////////////////////////////

  // these match corresponding entries in the hoot-core MetadataTags class
  protected static final String VALIDATION_ERROR_TAG_KEY_BASE = "hoot:validation:error";
  protected static final String VALIDATION_SOURCE_TAG_KEY_BASE = "hoot:validation:error:source";

  // the elements passed into the validate routine
  protected Collection<OsmPrimitive> inputElements;
  // the validated elements
  protected Collection<AbstractPrimitive> outputElements;

  // validation errors found on the input elements
  protected List<TestError> validationErrors;

  // element keys mapped to one or more validation error messages; not using PrimitiveId as element
  // keys here but don't know how to get it directly from OsmPrimitive; using LinkedHashMultimap to
  // preserve the value orderings between the various multimaps
  // e.g. key=Way:1, value1="Duplicated way nodes", value2="Unclosed way"
  protected Multimap<String, String> elementValidations = LinkedHashMultimap.create();

  ///////////////////////////////////////////////////////////////

  private static final String VALIDATORS_NAMESPACE = "org.openstreetmap.josm.data.validation.tests";

  // a list of names of validators that threw an error during validation
  private Map<String, String> failingValidators = new HashMap<String, String>();

  // size of the map input to the validation routine
  private int originalMapSize = 0;

  // maps validation error names to occurrence counts
  private Map<String, Integer> validationErrorCountsByType = new HashMap<String, Integer>();
}
