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

import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;
import java.util.Collection;
import java.lang.Exception;
import java.io.ByteArrayInputStream;
import java.lang.Class;
import java.io.FileInputStream;
import java.io.File;
import java.io.InputStream;
import java.io.PrintWriter;
import java.io.IOException;
import java.util.Set;
import java.util.HashSet;

import org.openstreetmap.josm.data.osm.AbstractPrimitive;
import org.openstreetmap.josm.data.osm.OsmPrimitive;
import org.openstreetmap.josm.data.validation.OsmValidator;
import org.openstreetmap.josm.data.validation.Test;
import org.openstreetmap.josm.data.validation.TestError;
import org.openstreetmap.josm.tools.Logging;
import org.openstreetmap.josm.io.OsmApi;
import org.openstreetmap.josm.io.OsmWriter;
import org.openstreetmap.josm.io.OsmWriterFactory;
import org.openstreetmap.josm.data.osm.DataSet;

/**
 * Validates a map using JOSM validators
 *
 * The interface for this class is kept purposefully coarse, so as to result in the smallest number
 * of JNI calls from clients as possible.
 *
 * @see JosmMapValidator in hoot-core
 */
public class JosmMapValidator
{
  public JosmMapValidator(
    String logLevel, String userCertPath, String userCertPassword) throws Exception
  {
    JosmUtils.initJosm(logLevel, userCertPath, userCertPassword);
  }

  public int getNumValidationErrors()
  {
    Logging.debug("validationErrorCountsByType size: " + validationErrorCountsByType.size());
    int errorCount = 0;
    for (Map.Entry<String, Integer> entry : validationErrorCountsByType.entrySet())
    {
      errorCount += entry.getValue();
    }
    return errorCount;
  }

  public int getNumFailingValidators()
  {
    if (failingValidators != null)
    {
      return failingValidators.size();
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

  public Set<String> getDeletedElementIds()
  {
    if (cleaner == null)
    {
      return new HashSet<String>();
    }
    return cleaner.getDeletedElementIds();
  }
  public int getNumDeletedElements()
  {
    if (cleaner == null)
    {
      return 0;
    }
    return cleaner.getNumDeletedElements();
  }
  public int getNumElementsCleaned()
  {
    if (cleaner == null)
    {
      return 0;
    }
    return cleaner.getNumElementsCleaned();
  }
  public int getNumFailedCleaningOperations()
  {
    if (cleaner == null)
    {
      return 0;
    }
    return cleaner.getNumFailedCleaningOperations();
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
    if (cleaner == null)
    {
      return new HashMap<String, Integer>();
    }
    return cleaner.getValidationErrorFixCountsByType();
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
   * Runs JOSM validators against an XML map string and tags elements that fail validation
   *
   * @param validators list of simple class names of the validators to be used
   * @param elementsXml map to be validated as an XML string
   * @param cleanValidated TODO
   * @return modified OSM map XML string if validation errors were found; otherwise a map identical
   * to the input map
   */
  public String validate(List<String> validators, String elementsXml, boolean cleanValidated,
    boolean addTags) throws Exception
  {
    //Logging.trace("elementsXml: " + elementsXml);
    if (elementsXml == null || elementsXml.trim().isEmpty())
    {
      throw new Exception("No elements passed to validation.");
    }
    clear();
    this.cleanValidated = cleanValidated;
    Logging.trace("cleanValidated: " + this.cleanValidated);
    this.addTags = addTags;
    Logging.trace("addTags: " + this.addTags);

    DataSet map = parseAndValidate(validators, new ByteArrayInputStream(elementsXml.getBytes()));
    logMapStats(map);
    return writeMapToXml(map);
  }

  /**
   * Runs JOSM validators against an XML map file and tags elements that fail validation
   *
   * @param validators list of simple class names of the validators to be used
   * @param elementsFileInputPath file path to the map to be validated
   * @param elementsFileOutputPath file path for the validated output map
   * @param cleanValidated TODO
   */
  public void validate(
    List<String> validators, String elementsFileInputPath, String elementsFileOutputPath,
    boolean cleanValidated, boolean addTags) throws Exception
  {
   clear();
   this.cleanValidated = cleanValidated;
   Logging.trace("cleanValidated: " + this.cleanValidated);
   this.addTags = addTags;
   Logging.trace("addTags: " + this.addTags);

    DataSet map =
      parseAndValidate(validators, new FileInputStream(new File(elementsFileInputPath)));
    logMapStats(map);
    writeMapToFile(map, new File(elementsFileOutputPath));
  }

  // these match corresponding entries in the hoot-core MetadataTags class
  private static final String VALIDATION_ERROR_TAG_KEY_BASE = "hoot:validation:error";
  private static final String VALIDATION_SOURCE_TAG_KEY_BASE = "hoot:validation:error:source";

  private static final String VALIDATORS_NAMESPACE = "org.openstreetmap.josm.data.validation.tests";

  //
  boolean cleanValidated = false;

  //
  boolean addTags = false;

  //
  int originalMapSize = 0;

  //
  private Map<String, Integer> elementErrorCounts = new HashMap<String, Integer>();

  //
  private Map<String, Integer> elementErrorIndexes = new HashMap<String, Integer>();

  // maps validation error names to occurrence counts
  private Map<String, Integer> validationErrorCountsByType = new HashMap<String, Integer>();

  // a list of names of validators that threw an error during validation
  private Map<String, String> failingValidators = new HashMap<String, String>();

  //
  private JosmMapCleaner cleaner = null;

  /*
   * Clear out member data
   */
  private void clear()
  {
    validationErrorCountsByType.clear();
    failingValidators.clear();
    cleaner = null;
    elementErrorIndexes.clear();
    elementErrorCounts.clear();
    originalMapSize = 0;
    cleanValidated = false;
    addTags = false;
  }

  private List<String> updateValidators(List<String> validators) throws Exception
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
      // prepend the Java namespace to the validator; All the validation tests are in a single
      // namespace, so this works. Clearly, we'll need to refactor if that doesn't remain the case.
      validatorName = VALIDATORS_NAMESPACE + "." + validatorName;
      Logging.trace("validatorName: " + validatorName);
      validatorsOut.add(validatorName);
    }
    Logging.debug("validatorsOut: " + validatorsOut);
    return validatorsOut;
  }

  private DataSet parseAndValidate(List<String> validators, InputStream elementsStream)
    throws Exception
  {
    long startTime = -1;

    validators = updateValidators(validators);

    // read in the input element map xml
    DataSet map = null;
    try
    {
      Logging.debug("Converting input elements from xml...");
      startTime = System.currentTimeMillis();
      map = HootOsmReader.parseDataSet(elementsStream);
      Logging.debug(
        "Input elements converted from xml in: " +
        String.valueOf((System.currentTimeMillis() - startTime) / 1000) + " seconds.");
      originalMapSize = map.getAllPrimitives().size();
      Logging.debug("originalMapSize: " + originalMapSize);

      if (originalMapSize == 0)
      {
        throw new Exception("No features passed to validation.");
      }

      // NOTE: Unlike hoot core's logging, JOSM's will still execute any code in the logging
      // statement despite the log level and simply not log the statement. So, you definitely don't
      // want anything like this making its way into a production environment.
      //Logging.trace(
      // "input elements: " + JosmUtils.elementsToString(inputDataset.getAllPrimitives()));
    }
    catch (Exception e)
    {
      Logging.error("Error converting validation input features to XML: " + e.getMessage());
      throw e;
    }

    // run the specified validators against the elements
    try
    {
      runValidation(validators, map);
    }
    catch (Exception e)
    {
      Logging.error("Error running validation tests: " + e.getMessage());
      throw e;
    }

    return map;
  }

  /*
   * Runs a set of validators against specified input elements
   */
  private void runValidation(List<String> validators, DataSet map) throws Exception
  {
    Logging.debug("Running validation tests...");
    long startTime = System.currentTimeMillis();

    //Logging.trace("elements size: " + elements.size());
    for (String validator : validators)
    {
      runValidation((Test)Class.forName(validator).newInstance(), map);
    }

    Logging.debug(
      "Found " + getNumValidationErrors() + " validation errors in: " +
      String.valueOf((System.currentTimeMillis() - startTime) / 1000) + " seconds.");
    if (failingValidators.size() > 0)
    {
      Logging.warn("The following JOSM validators failed: " + failingValidators.keySet());
    }
  }

  /*
   * Runs a single validator against specified input elements
   */
  private void runValidation(Test validator, DataSet map) throws Exception
  {
    Logging.info("Running JOSM validator: " + validator.getName() + "...");
    //Logging.trace("elements size: " + elements.size());

    List<TestError> errors = null;
    try
    {
      validator.initialize();
      validator.setPartialSelection(false);
      validator.startTest(null);
      validator.visit(map.getAllPrimitives());
      validator.endTest();

      errors = validator.getErrors();

      Logging.debug(
        "Validator: " + validator.getName() + " found " + errors.size() + " errors with " +
        getNumElementsInvolvedInErrors(errors) + " total involved elements."); 
    }
    catch (Exception e)
    {
      failingValidators.put(validator.getName(), JosmUtils.getErrorMessage(validator, e));
    }

    parseValidationErrors(errors, map);

    // This will clear out any errors, so call it last.
    validator.clear();
  }

  private void parseValidationErrors(List<TestError> validationErrors, DataSet map) throws Exception
  {
    //Logging.debug("Parsing validation errors...");
    //Logging.trace("validationErrors size: " + validationErrors.size());

    if (cleanValidated)
    {
      cleaner = new JosmMapCleaner(addTags);
    }

    for (TestError error : validationErrors)
    {
      Collection<? extends OsmPrimitive> elementGroupWithError = error.getPrimitives();
      String testName = error.getTester().getName();
      Logging.trace(
        "Processing validation results for " + error.getPrimitives().size() + " elements for " +
        " error: \"" + error.getMessage() + "\" found by test: " + testName + "...");
      //Logging.trace("error.getPrimitives(): " + JosmUtils.elementsToString(error.getPrimitives()));

      if (validationErrorCountsByType.containsKey(testName))
      {
        int currentErrorCountForType = validationErrorCountsByType.get(testName);
        validationErrorCountsByType.put(testName, currentErrorCountForType + 1);
      }
      else
      {
        validationErrorCountsByType.put(testName, 1);
      }

      //Logging.trace("addTags: " + addTags);
      if (addTags)
      {
        //Logging.debug("Adding validation tags...");

        for (OsmPrimitive element : elementGroupWithError)
        {
          String elementKey = JosmUtils.getElementMapKey(element);
          int errorCount = 1;
          if (elementErrorCounts.containsKey(elementKey))
          {
            errorCount = elementErrorCounts.get(elementKey);
            errorCount++;
          }
          elementErrorCounts.put(elementKey, errorCount);
          elementErrorIndexes.put(elementKey + ";" + error.getMessage(), errorCount);

          element.put(
            VALIDATION_ERROR_TAG_KEY_BASE + ":" + String.valueOf(errorCount), error.getMessage());
          element.put(VALIDATION_SOURCE_TAG_KEY_BASE + ":" + String.valueOf(errorCount), "JOSM");
        }
      }

      if (cleanValidated)
      {
        //Logging.trace("elementErrorIndexes size: " + elementErrorIndexes.size());
        cleaner.setElementErrorIndexes(elementErrorIndexes);
        cleaner.clean(error);
      }
    }

    if (cleanValidated)
    {
      // Apparently, any JOSM fixes resulting in deletes don't actually delete the elements (tried
      // accessing the affected dataset from the command itself). So, we'll manually delete them
      // here.
      cleaner.removeDeletedElements(map);
    }
  }

  private void logMapStats(DataSet map)
  {
    // gather some stats on the modified map

    Collection<OsmPrimitive> elements = map.getAllPrimitives();
    int validatedMapSize = elements.size();
    int mapSizeDiff = 0;
    if (getNumValidationErrors() == 0 || originalMapSize == validatedMapSize)
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

  private String writeMapToXml(DataSet map)
  {
    Logging.debug("Writing map to XML...");
    //Logging.trace("elements: " + JosmUtils.elementsToString(elements));

    // convert any validated elements back to xml

    String outputElementsXml = null;
    try
    {
      long startTime = System.currentTimeMillis();
      Collection<AbstractPrimitive> elementsToWrite = new ArrayList<AbstractPrimitive>();
      elementsToWrite.addAll(map.getAllPrimitives());
      outputElementsXml =
        OsmApi.getOsmApi("http://localhost").toBulkXml(elementsToWrite, true);
      Logging.debug(
        "Output elements converted to xml in: " +
        String.valueOf((System.currentTimeMillis() - startTime) / 1000) + " seconds.");
      //Logging.trace("outputElementsXml: " + outputElementsXml);
    }
    catch (Exception e)
    {
      Logging.error("Error converting output elements to XML: " + e.getMessage());
      throw e;
    }

    return outputElementsXml;
  }

  private void writeMapToFile(DataSet map, File outFile) throws IOException
  {
    Logging.debug("Writing map to file: " + outFile.getName() + "...");

    PrintWriter writer = new PrintWriter(outFile);
    OsmWriterFactory.createOsmWriter(writer, true, "0.6").write(map);
    writer.flush();
    writer.close();
  }
}
