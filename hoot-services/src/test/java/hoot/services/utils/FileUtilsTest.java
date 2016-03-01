package hoot.services.utils;

import hoot.services.HootProperties;
import hoot.services.UnitTest;


import org.junit.Test;
import org.junit.experimental.categories.Category;

public class FileUtilsTest {
	@Test
	@Category(UnitTest.class)
	public void testValidateFilePath() throws Exception
	{
		String name = "mytest";
		String homeFolder = HootProperties.getProperty("homeFolder");
    String scriptFolder = homeFolder + "/" + HootProperties.getProperty("customScriptPath");
		boolean isValid = FileUtils.validateFilePath(scriptFolder, scriptFolder + "/" + name + ".js");
		org.junit.Assert.assertTrue(isValid);

	}
	
	@Test
	@Category(UnitTest.class)
	public void testValidateFilePathFail() throws Exception
	{
		String name = "../mytest";
		String homeFolder = HootProperties.getProperty("homeFolder");
    String scriptFolder = homeFolder + "/" + HootProperties.getProperty("customScriptPath");
		boolean isValid = FileUtils.validateFilePath(scriptFolder, scriptFolder + "/" + name + ".js");
		org.junit.Assert.assertTrue(!isValid);

	}
}
