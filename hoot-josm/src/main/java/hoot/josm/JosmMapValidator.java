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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.josm;

import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;
import java.util.Collection;
import java.util.Collections;
import java.lang.Exception;
import java.io.ByteArrayInputStream;
import java.lang.Class;
import java.io.FileInputStream;
import java.io.File;
import java.io.InputStream;
import java.io.IOException;
import java.util.Set;
import java.util.HashSet;
import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.lang.reflect.Method;
import java.lang.reflect.Constructor;

import org.openstreetmap.josm.data.osm.AbstractPrimitive;
import org.openstreetmap.josm.data.osm.OsmPrimitive;
//import org.openstreetmap.josm.data.validation.OsmValidator;
import hoot.josm.OsmValidator;
import hoot.josm.TagChecker2;
import org.openstreetmap.josm.data.validation.Test;
import org.openstreetmap.josm.data.validation.TestError;
import org.openstreetmap.josm.tools.Logging;
import org.openstreetmap.josm.io.OsmApi;
import org.openstreetmap.josm.io.OsmWriter;
import org.openstreetmap.josm.io.OsmWriterFactory;
import org.openstreetmap.josm.data.osm.DataSet;
import org.openstreetmap.josm.spi.preferences.Config;
import org.openstreetmap.josm.data.preferences.sources.ValidatorPrefHelper;
import org.openstreetmap.josm.data.Preferences;
import org.openstreetmap.josm.tools.ListenableWeakReference;

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
  public JosmMapValidator(String logLevel) throws Exception
  {
    JosmUtils.initJosm(logLevel);
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

  /**
   * Returns the IDs of all elements deleted if the map was also cleaned during validation
   */
  public Set<String> getDeletedElementIds()
  {
    if (cleaner == null)
    {
      return new HashSet<String>();
    }
    return cleaner.getDeletedElementIds();
  }

  /**
   * Returns the number of elements deleted if the map was also cleaned during validation
   */
  public int getNumDeletedElements()
  {
    if (cleaner == null)
    {
      return 0;
    }
    return cleaner.getNumDeletedElements();
  }

  /**
   * Returns the the number of elements cleaned if the map was also cleaned during validation
   */
  public int getNumElementsCleaned()
  {
    if (cleaner == null)
    {
      return 0;
    }
    return cleaner.getNumElementsCleaned();
  }

  /**
   * Returns the number of failed cleaning operations if the map was also cleaned during validation
   */
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
   * "<validator1ClassName>:<validator1Description>;<validator2ClassName>:<validator2Description>..."
   */
  public Map<String, String> getAvailableValidators() throws Exception
  {
    //Logging.debug("test4");
    Logging.debug("Retrieving available validators...");
    Map<String, String> validators = new HashMap<String, String>();
    try
    {
      Field modifiersField = Field.class.getDeclaredField("modifiers");
      modifiersField.setAccessible(true);

      //OsmValidator.initialize();
      //OsmValidator.resetErrorList();
      //OsmValidator.initializeErrorLayer();
      //OsmValidator.resetErrorLayer();
      //OsmValidator.initializeTests();

      //Collection<Test> validationTests = OsmValidator.getTests();

      /*Field field15 = OsmValidator.class.getDeclaredField("allTests");
      modifiersField.setInt(field15, field15.getModifiers() & ~Modifier.FINAL);
      field15.setAccessible(true);
      Collection<Class<? extends Test>> validationTests =
        (Collection<Class<? extends Test>>)field15.get(null);*/
      /*Field field16 = OsmValidator.class.getDeclaredField("allTestsMap");
      modifiersField.setInt(field16, field16.getModifiers() & ~Modifier.FINAL);
      field16.setAccessible(true);
      Map<String, Test> validationTestsMap = (Map<String, Test>)field16.get(null);
      Collection<Test> validationTests = validationTestsMap.values();*/

      /*Collection<Class<? extends Test>> validationTestClasses =
        OsmValidator.getAllAvailableTestClasses();
      Collection<Test> validationTests = new ArrayList<Test>();
      for (Class<? extends Test> testClass : validationTestClasses)
      {
        validationTests.add(testClass.getConstructor().newInstance());
      }*/

      /*
      bad:

      TagChecker
      */

      Collection<Test> validationTests = new ArrayList<Test>();
      Field field17 = hoot.josm.OsmValidator.class.getDeclaredField("CORE_TEST_CLASSES");
      modifiersField.setInt(field17, field17.getModifiers() & ~Modifier.FINAL);
      field17.setAccessible(true);
      Class<Test>[] validationTestClasses = (Class<Test>[])field17.get(null);
      for (int i = 0; i < validationTestClasses.length; i++)
      {
        Logging.debug(validationTestClasses[i].getName());
        Class<Test> cls = validationTestClasses[i];
        if (cls.getName().endsWith("TagChecker"))
        {
          cls = TagChecker2.class;
        }
        validationTests.add(cls.getConstructor().newInstance());
      }

      /*Constructor<OsmValidator> constructor = OsmValidator.class.getDeclaredConstructor();
      constructor.setAccessible(true);
      OsmValidator osmValidator = constructor.newInstance();
      Collection<Test> validationTests = osmValidator.getTests();
      osmValidator = null;*/

      //Collection<Test> validationTests = new ArrayList<Test>();

      //Collection<Test> validationTests = OsmValidator.getTests().clone();

      //ArrayList<Test> validationTests = new ArrayList<Test>(OsmValidator.getTests());

      //List<Test> validationTests = new ArrayList<Test>();
      //Collections.copy(validationTests, OsmValidator.getTests());

      for (Test validationTest : validationTests)
      //for (Class<? extends Test> validationTest : validationTests)
      {
        if (validationTest != null)
        {
          /*Logging.debug(validationTest.getClass().getName());
          if (!validationTest.getClass().getName().endsWith("BarriersEntrances"))
          {
            continue;
          }*/

          String testName = validationTest.toString().split("@")[0];
          // converting inner class '$' char over to '.' for readability
          testName = testName.replace("$", ".");
          // not returning the namespace, so remove it
          testName = testName.replace(VALIDATORS_NAMESPACE + ".", "");
          Logging.trace("testName: " + testName);
          String testDescription = validationTest.getName();
          validators.put(testName, testDescription);

          //if (testName.endsWith("TagChecker"))
          //{

          //}

          /*Field field1 = Test.class.getDeclaredField("name");
          modifiersField.setInt(field1, field1.getModifiers() & ~Modifier.FINAL);
          field1.setAccessible(true);
          field1.set(validationTest, null);
          Field field2 =Test.class.getDeclaredField("description");
          modifiersField.setInt(field2, field2.getModifiers() & ~Modifier.FINAL);
          field2.setAccessible(true);
          field2.set(validationTest, null);
          Field field3 = Test.class.getDeclaredField("checkEnabled");
          field3.setAccessible(true);
          field3.set(validationTest, null);
          Field field4 = Test.class.getDeclaredField("checkBeforeUpload");
          field4.setAccessible(true);
          field4.set(validationTest, null);
          Field field5 = Test.class.getDeclaredField("errors");
          field5.setAccessible(true);
          field5.set(validationTest, null);
          Field field6 = Test.class.getDeclaredField("progressMonitor");
          field6.setAccessible(true);
          field6.set(validationTest, null);
          Field field7 = Test.class.getDeclaredField("stopwatch");
          field7.setAccessible(true);
          field7.set(validationTest, null);
          Field field8 = Test.class.getDeclaredField("IN_DOWNLOADED_AREA");
          modifiersField.setInt(field8, field8.getModifiers() & ~Modifier.FINAL);
          field8.setAccessible(true);
          field8.set(null, null);
          Field field9 = Test.class.getDeclaredField("IN_DOWNLOADED_AREA_STRICT");
          modifiersField.setInt(field9, field9.getModifiers() & ~Modifier.FINAL);
          field9.setAccessible(true);
          field9.set(null, null);

          validationTest = null;*/
        }
      }
      /*validationTests.clear();
      validationTests = null;

      OsmValidator.resetErrorLayer();
      OsmValidator.resetErrorList();
      Method method1 = OsmValidator.class.getDeclaredMethod("removeLegacyEntries", Boolean.TYPE);
      method1.setAccessible(true);
      method1.invoke(null, true);
      Method method2 = OsmValidator.class.getDeclaredMethod("cleanupIgnoredErrors", null);
      method2.setAccessible(true);
      method2.invoke(null, null);
      Method method3 = OsmValidator.class.getDeclaredMethod("clearIgnoredErrors", null);
      method3.setAccessible(true);
      method3.invoke(null, null);

      Field field1 = OsmValidator.class.getDeclaredField("allTestsMap");
      modifiersField.setInt(field1, field1.getModifiers() & ~Modifier.FINAL);
      field1.setAccessible(true);

      Map<String, Test> allTestsMap = (Map<String, Test>)field1.get(null);
      for (Map.Entry<String, Test> entry : allTestsMap.entrySet())
      {
        Test test = entry.getValue();
        test = null;
      }
      allTestsMap.clear();

      field1.set(null, null);
      field1 = null;
      Field field2 = OsmValidator.class.getDeclaredField("allTests");
      modifiersField.setInt(field2, field2.getModifiers() & ~Modifier.FINAL);
      field2.setAccessible(true);

      Collection<Class<? extends Test>> allTests =
        (Collection<Class<? extends Test>>)field2.get(null);
      for (Class<? extends Test> validationTest : allTests)
      {
        validationTest = null;
      }
      allTests.clear();

      field2.set(null, null);
      field2 = null;
      Field field3 = OsmValidator.class.getDeclaredField("errorLayer");
      modifiersField.setInt(field3, field3.getModifiers() & ~Modifier.FINAL);
      field3.setAccessible(true);
      field3.set(null, null);
      field3 = null;
      Field field4 = OsmValidator.class.getDeclaredField("ignoredErrors");
      modifiersField.setInt(field4, field4.getModifiers() & ~Modifier.FINAL);
      field4.setAccessible(true);
      field4.set(null, null);
      field4 = null;
      Field field5 = OsmValidator.class.getDeclaredField("CORE_TEST_CLASSES");
      modifiersField.setInt(field5, field5.getModifiers() & ~Modifier.FINAL);
      field5.setAccessible(true);
      field5.set(null, null);
      field5 = null;
      modifiersField = null;*/

      //Thread.currentThread().stop();

      /*if (Thread.currentThread().isAlive()) {
          ThreadGroup group = Thread.currentThread().getThreadGroup();
          while (group != null) {
              group.interrupt();
              group = Thread.currentThread().getThreadGroup();
          }
          Thread.currentThread().interrupt();
      }*/

      //Thread.currentThread().interrupt();

      //Config.setPreferencesInstance(null);

      /*Field field10 = Config.class.getDeclaredField("preferences");
      field10.setAccessible(true);
      field10.set(null, null);
      Field field11 = Config.class.getDeclaredField("baseDirectories");
      field11.setAccessible(true);
      field11.set(null, null);
      Field field12 = Config.class.getDeclaredField("urls");
      field12.setAccessible(true);
      field12.set(null, null);*/
      //Field field13 = ValidatorPrefHelper.class.getDeclaredField("INSTANCE");
      //modifiersField.setInt(field13, field13.getModifiers() & ~Modifier.FINAL);
      //field13.setAccessible(true);
      //field13.set(null, null);

      //Preferences pref = Preferences.main();
      //pref.resetToInitialState();

      /*Field field14 = ListenableWeakReference.class.getDeclaredField("GLOBAL_QUEUE");
      modifiersField.setInt(field14, field14.getModifiers() & ~Modifier.FINAL);
      field14.setAccessible(true);
      field14.set(null, null);
      Field field15 = ListenableWeakReference.class.getDeclaredField("thread");
      field15.setAccessible(true);
      field15.set(null, null);*/
      /*Method method4 = ListenableWeakReference.class.getDeclaredMethod("clean", null);
      method4.setAccessible(true);
      method4.invoke(null, null);*/
      /*Method method5 = ListenableWeakReference.class.getDeclaredMethod("clear", null);
      method5.setAccessible(true);
      method5.invoke(null, null);*/
    }
    catch (Exception e)
    {
      System.out.println(e.getMessage());
      throw e;
    }

    Logging.debug("validators size: " + String.valueOf(validators.size()));
    return validators;
  }

  /**
   * Runs JOSM validators against an XML map string and tags elements that fail validation
   *
   * @param validators list of simple class names of the validators to be used
   * @param elementsXml map to be validated as an XML string
   * @param cleanValidated if true, elements failing validation are cleaned where possible
   * @return modified OSM map XML string if validation errors were found; otherwise a map identical
   * to the input map
   */
  public String validate(
    List<String> validators, String elementsXml, boolean cleanValidated,
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

    String mapXml = null;
    try
    {
      Logging.debug("Writing map to xml...");
      long startTime = System.currentTimeMillis();
      mapXml = JosmUtils.writeMapToXml(map);
      Logging.debug(
        "Map written to xml in: " +
        String.valueOf((System.currentTimeMillis() - startTime) / 1000) + " seconds.");
    }
    catch (Exception e)
    {
      Logging.error("Error writing output to xml: " + e.getMessage());
      throw e;
    }

    return mapXml;
  }

  /**
   * Runs JOSM validators against a map
   *
   * @param validators list of simple class names of the validators to be used
   * @param elementsFileInputPath file path to the map to be validated
   * @param elementsFileOutputPath file path for the validated output map
   * @param cleanValidated if true, elements failing validation are cleaned where possible
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

    try
    {
      Logging.debug("Writing map to file...");
      long startTime = System.currentTimeMillis();
      JosmUtils.writeMapToFile(map, new File(elementsFileOutputPath));
      Logging.debug(
        "Map written to file in: " +
        String.valueOf((System.currentTimeMillis() - startTime) / 1000) + " seconds.");
    }
    catch (Exception e)
    {
      Logging.error("Error writing output to file: " + e.getMessage());
      throw e;
    }
  }

  // these match corresponding entries in the hoot-core MetadataTags class
  private static final String VALIDATION_ERROR_TAG_KEY_BASE = "hoot:validation:error";
  private static final String VALIDATION_SOURCE_TAG_KEY_BASE = "hoot:validation:error:source";

  private static final String VALIDATORS_NAMESPACE = "org.openstreetmap.josm.data.validation.tests";

  // determines whether a cleaning attempt will be performed on elements failing validation if
  // applicable
  boolean cleanValidated = false;

  // determine whether tags are added to elements failing validation
  boolean addTags = false;

  // the size of the input map
  int originalMapSize = 0;

  // maps error names to a running count
  private Map<String, Integer> elementErrorCounts = new HashMap<String, Integer>();

  // maps element keys to a running count of the number of errors the element is involved in
  private Map<String, Integer> elementErrorIndexes = new HashMap<String, Integer>();

  // maps validation error names to error occurrence counts
  private Map<String, Integer> validationErrorCountsByType = new HashMap<String, Integer>();

  // a list of names of validators that threw an error during validation
  private Map<String, String> failingValidators = new HashMap<String, String>();

  // optionally attempts to clean features failing validation
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
    validators = updateValidators(validators);

    // read in the input element map xml
    DataSet map = null;
    try
    {
      Logging.debug("Converting input elements from xml...");
      long startTime = System.currentTimeMillis();
      map = HootOsmReader.parseDataSet(elementsStream);
      Logging.debug(
        "Input elements converted from xml in: " +
        String.valueOf((System.currentTimeMillis() - startTime) / 1000) + " seconds.");
      originalMapSize = map.allPrimitives().size();
      Logging.debug("originalMapSize: " + originalMapSize);

      if (originalMapSize == 0)
      {
        throw new Exception("No features passed to validation.");
      }

      // NOTE: Unlike hoot core's logging, JOSM's will still execute any code in the logging
      // statement despite the log level and simply not log the statement. So, you definitely don't
      // want anything like this making its way into a production environment that wil cause
      // performance issues.
      //Logging.trace(
      // "input elements: " + JosmUtils.elementsToString(inputDataset.allPrimitives()));
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
      validator.visit(map.allPrimitives());
      validator.endTest();

      errors = validator.getErrors();

      Logging.info(
        "Validator: " + validator.getName() + " found " + errors.size() + " errors with " +
        getNumElementsInvolvedInErrors(errors) + " total involved elements."); 
    }
    catch (Exception e)
    {
      failingValidators.put(validator.getName(), JosmUtils.getErrorMessage(validator, e));
    }

    parseValidationErrors(validator.getName(), errors, map);

    // This will clear out any errors, so call it last.
    validator.clear();
  }

  /*
   * Records any validation errors and performs cleaning if cleaning was specified
   */
  private void parseValidationErrors(
    String validatorName, List<TestError> validationErrors, DataSet map) throws Exception
  {
    String cleanMsgStr = "will not";
    if (cleanValidated)
    {
      cleaner = new JosmMapCleaner(addTags);
      cleanMsgStr = "will";
    }
    Logging.info(
      "Parsing validation errors for: " + validatorName + ". Cleaning " + cleanMsgStr +
      " be attempted...");
    //Logging.trace("validationErrors size: " + validationErrors.size());

    int numCleaned = 0;
    for (TestError error : validationErrors)
    {
      Collection<? extends OsmPrimitive> elementGroupWithError = error.getPrimitives();
      Logging.trace(
        "Processing validation results for " + error.getPrimitives().size() + " elements for " +
        " error: \"" + error.getMessage() + "\" found by test: " + validatorName + "...");
      //Logging.trace("error.getPrimitives(): " + JosmUtils.elementsToString(error.getPrimitives()));

      if (validationErrorCountsByType.containsKey(validatorName))
      {
        int currentErrorCountForType = validationErrorCountsByType.get(validatorName);
        validationErrorCountsByType.put(validatorName, currentErrorCountForType + 1);
      }
      else
      {
        validationErrorCountsByType.put(validatorName, 1);
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
        numCleaned += cleaner.clean(error);
      }
    }

    if (cleanValidated)
    {
      Logging.info("Cleaned " + numCleaned + " features for error: " + validatorName);
      // Apparently, any JOSM fixes resulting in deletes don't actually delete the elements (tried
      // accessing the affected dataset from the command itself). So, we'll manually delete them
      // here.
      cleaner.removeDeletedElements(map);
    }
  }

  private void logMapStats(DataSet map)
  {
    // gather some stats on the modified map

    Collection<OsmPrimitive> elements = map.allPrimitives();
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
}
