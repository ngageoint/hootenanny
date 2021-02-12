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
 * @copyright Copyright (C) 2018 Maxar (http://www.maxar.com/)
 */

package hoot.services.language.opennlp;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;
import hoot.services.language.LanguageUtils;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class OpenNlpLanguageDetectorTest  
{
  private static final Logger logger = LoggerFactory.getLogger(OpenNlpLanguageDetectorTest.class);

  private OpenNlpLanguageDetector detector = null;

  public OpenNlpLanguageDetectorTest() throws Exception
  {
    detector = OpenNlpLanguageDetector.getInstance();
  }

  @Test
  @Category(UnitTest.class)
  public void testAll() throws Exception
  {
    if (!detector.isLanguageAvailable("de"))
    {
      logger.warn(
        "No OpenNLP language detection model available.  Skipping OpenNlpLanguageDetectorTest...");
      return;
    }

    Assert.assertTrue(detector.isLanguageAvailable("de"));
    Assert.assertTrue(detector.getSupportedLanguages().length > 0);
    Assert.assertEquals("German", detector.getLanguageName("de"));
    Assert.assertEquals("https://opennlp.apache.org", detector.getUrl());
    Assert.assertTrue(!detector.getDescription().isEmpty());
    Assert.assertEquals(
      "none available", LanguageUtils.confidenceToString(detector.getConfidence()));
    //logger.debug(detector.detect("wie alt bist du"));
    Assert.assertEquals("deu", detector.detect("wie alt bist du"));
  }
}
