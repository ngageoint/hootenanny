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
 * @copyright Copyright (C) 2014, 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.info;

import java.io.File;

import hoot.services.UnitTest;

import org.junit.BeforeClass;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.experimental.categories.Category;

public class ErrorLogTest {
	@Ignore
	@BeforeClass
  public static void oneTimeSetup() {
      // one-time setup code
		// Disable due to test server is getting permission denied error when verifying errLogPath line#48
	/*	ErrorLog log = new ErrorLog();
		org.junit.Assert.assertNotNull(log._errLogPath);
		org.junit.Assert.assertTrue(log._errLogPath.length() > 0);

		File logFile = new File(log._errLogPath); 
		org.junit.Assert.assertTrue(logFile.exists()); ///<<<===================

		org.junit.Assert.assertNotNull(log._tempOutputPath);
		org.junit.Assert.assertTrue(log._tempOutputPath.length() > 0);*/
  }

	@Ignore
	@Test
	@Category(UnitTest.class)
	public void testgenerateExportLog() throws Exception
	{
		ErrorLog log = new ErrorLog();
		String path = log.generateExportLog();

		File logFile = new File(path);
		org.junit.Assert.assertTrue(logFile.exists());

	}
	@Ignore
	@Test
	@Category(UnitTest.class)
	public void testgetErrorLog() throws Exception
	{
		ErrorLog log = new ErrorLog();
		String logStr = log.getErrorlog(100);

		org.junit.Assert.assertTrue(logStr.length() <= 100);

	}
}
