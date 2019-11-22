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

import java.util.List;
import java.util.ArrayList;
import java.util.Collection;
import java.lang.Exception;
import java.io.ByteArrayInputStream;
import java.lang.Class;

//import org.apache.commons.lang3.ClassUtils;
//import org.apache.commons.lang3.reflect.ConstructorUtils;

import org.openstreetmap.josm.data.osm.AbstractPrimitive;
import org.openstreetmap.josm.data.osm.OsmPrimitive;
import org.openstreetmap.josm.data.osm.Node;
import org.openstreetmap.josm.data.validation.OsmValidator;
import org.openstreetmap.josm.data.validation.Test;
import org.openstreetmap.josm.data.validation.TestError;
import org.openstreetmap.josm.tools.Logging;
import org.openstreetmap.josm.spi.preferences.Config;
import org.openstreetmap.josm.data.Preferences;
import org.openstreetmap.josm.io.OsmReader;
import org.openstreetmap.josm.io.OsmApi;
import org.openstreetmap.josm.command.Command;

/**
 * TODO

 using delimited strings in place of containers here to keep the JNI code simpler initially; will
 replace with containers later if performance dictates it
 */
public class JosmValidator
{
  /**
   * TODO
   */
  public JosmValidator()
  {
    Logging.setLogLevel(Logging.LEVEL_DEBUG);
    Preferences pref = Preferences.main();
    Config.setPreferencesInstance(pref);
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
  public String validate(String validators, String featuresXml, boolean fixFeatures)
    throws Exception
  {
    String validatedFeaturesStr = "";
    try
    {
      // will try passing features around as xml for the first draft; if too slow can try
      // an OsmMap --> OsmPrimitive conversion

      // read element xml
      Collection<OsmPrimitive> elements =
        OsmReader.parseDataSet(
          new ByteArrayInputStream(featuresXml.getBytes()), null).getAllPrimitives();

      // run the specified validation tests
      assert(validators.contains(";"));
      List<TestError> errors = runValidators(validators.split(";"), elements);

      // add validation/fix msg tags
      Collection<AbstractPrimitive> validatedElements =
        collectValidatedElements(errors, fixFeatures);

      // convert elements back to xml
      validatedFeaturesStr =
        OsmApi.getOsmApi("http://localhost").toBulkXml(validatedElements, true);
    }
    catch (Exception e)
    {
      System.out.println(e.getMessage());
      throw e;
    }

    return validatedFeaturesStr;
  }

  private List<TestError> runValidators(String[] validators, Collection<OsmPrimitive> elements)
    throws Exception
  {
    List<TestError> errors = new ArrayList<TestError>();

    for (int i = 0; i < validators.length; i++)
    {
      String validatorStr = validators[i];
      // TODO: fix
      Test validator = null;
      // ClassUtils.getPackageName(ElementFactory.class) + "." + elementType
      /*Test validationTest =
        (Test)ConstructorUtils.invokeConstructor(
          Class.forName(validatorStr),
          new Object[] { validatorStr },
          new Class<?>[] { String.class });*/

      validator.initialize();
      validator.setPartialSelection(false);
      validator.startTest(null);
      validator.visit(elements);
      validator.endTest();
      errors.addAll(validator.getErrors());
      validator.clear();
   }

   return errors;
  }

  private Collection<AbstractPrimitive> collectValidatedElements(
    List<TestError> errors, boolean fixFeatures)
  {
    Collection<AbstractPrimitive> validatedElements = new ArrayList<AbstractPrimitive>();

    for (TestError error : errors)
    {
      Collection<? extends OsmPrimitive> elementsWithErrors = error.getPrimitives();

      boolean fixSuccess = false;
      if (fixFeatures && error.isFixable())
      {
        // fix features based on error found

        Command fixCmd = error.getFix();
        fixSuccess = fixCmd.executeCommand();
        if (!fixSuccess)
        {
          System.out.println("Failure executing fix command.");
        }
      }

      for (OsmPrimitive element : elementsWithErrors)
      {
        element.put("hoot:validation", error.getMessage());
        if (fixFeatures)
        {
          String fixStatus = "false";
          if (fixSuccess)
          {
            fixStatus = "true";
          }
          element.put("hoot:validation:fixed", fixStatus);
        }
        validatedElements.add(element);
      }
    }

    return validatedElements;
  }
}
