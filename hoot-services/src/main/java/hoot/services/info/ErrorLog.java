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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.info;

import java.io.File;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.UUID;

import org.apache.commons.lang3.StringUtils;

import hoot.services.HootProperties;
import hoot.services.controllers.info.AboutResource;

public class ErrorLog {

	private static String _errLogPath = null;
	private static String _tempOutputPath = null;
	public ErrorLog() {
		//ErrorLogPath
		if(_errLogPath == null)
		{
			try {
	      _errLogPath = HootProperties.getProperty("ErrorLogPath");
      } catch (IOException e) {

      }
		}

		if(_tempOutputPath ==  null){
	    try {
	    	_tempOutputPath = HootProperties.getProperty("tempOutputPath");
			} catch (IOException e) {

			}
    }
	}

	public String getErrorlog(long maxLength) throws Exception
	{
		RandomAccessFile randomAccessFile = null;
    try
    {
    	File file = new File(_errLogPath);
  		randomAccessFile = new RandomAccessFile(file, "r");
      StringBuilder builder = new StringBuilder();
      long length = file.length();

      long startOffset = 0;
      if(length > maxLength)
      {
      	startOffset = length - maxLength;
      }
      for(long seek = startOffset; seek < length; seek++)
      {
      	randomAccessFile.seek(seek);
      	char c = (char)randomAccessFile.read();
        builder.append(c);
      }
      return builder.toString();
    }
    finally
    {
    	if(randomAccessFile != null)
    	{
    		randomAccessFile.close();
    	}
    }
	}

	public String generateExportLog() throws Exception
	{
		String fileId = UUID.randomUUID().toString();
		String outputPath = _tempOutputPath + "/" + fileId;


		String data = "";

		AboutResource about = new AboutResource();
		VersionInfo vInfo = about.getCoreVersionInfo();
		data = "\n************ CORE VERSION INFO ***********\n";
		data += vInfo.toString();
		CoreDetail cd = about.getCoreVersionDetail();
		data += "\n************ CORE ENVIRONMENT ***********\n";
		if(cd != null)
		{
			data += StringUtils.join(cd.getEnvironmentInfo(), '\n');
		}

		data = "\n************ SERVICE VERSION INFO ***********\n";
		data += about.getServicesVersionInfo().toString();
		ServicesDetail sd = about.getServicesVersionDetail();
		if(sd != null)
		{
			data += "\n************ SERVICE DETAIL PROPERTY ***********\n";
			for( ServicesDetail.Property prop : sd.getProperties())
			{
				String str = prop.getName() + " : " + prop.getValue() + "\n";
				data += str;
			}

			data += "\n************ SERVICE DETAIL RESOURCE ***********\n";
			for( ServicesDetail.ServicesResource res : sd.getResources())
			{
				String str = res.getType() + " : " + res.getUrl() + "\n";
				data += str;
			}
		}


		data += "\n************ CATALINA LOG ***********\n";



		// 5MB Max
		int maxSize = 5000000;

		String logStr = getErrorlog(maxSize);

		RandomAccessFile raf = null;
		try
		{
		  raf = new RandomAccessFile(outputPath, "rw");
			raf.writeBytes(data + "\n" + logStr);
			return outputPath;
		}
		finally
		{
			if(raf != null)
			{
				raf.close();
			}
		}
	}
}
