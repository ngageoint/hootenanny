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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.wps;

import hoot.services.HootProperties;
import hoot.services.nativeInterfaces.NativeInterfaceException;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.io.FileUtils;
import org.deegree.services.wps.Processlet;
import org.deegree.services.wps.ProcessletException;
import org.deegree.services.wps.ProcessletExecutionInfo;
import org.deegree.services.wps.ProcessletInputs;
import org.deegree.services.wps.ProcessletOutputs;
import org.deegree.services.wps.input.LiteralInput;
import org.deegree.services.wps.input.ProcessletInput;
import org.deegree.services.wps.output.ComplexOutput;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


/**
 * @author Jong Choi
 * 
 * WPS processlet for ETL operation. The operation is done using make file.
 * See hoot-services.conf for dependencies.
 * Please note that the arguments used by the make file is driven by ETLProcesslet.xml
 *
 */
public class ExportProcesslet  implements Processlet {
	
  private static final Logger log = LoggerFactory.getLogger(ExportProcesslet.class);
  private String tempOutputPath = null;
	private String delPath = null;
	
  
  /**
	 * 
	 */
	public ExportProcesslet() throws Exception {
		try
		{
			if(tempOutputPath ==  null){
				tempOutputPath = HootProperties.getProperty("tempOutputPath");
	    }
		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());
		}
		
	}
	
	/* (non-Javadoc)
	 * @see hoot.services.wps.WPSProcesslet#destroy()
	 */
	@Override
	public void destroy() {
		
		try
		{
			if(delPath != null)
			{
				File workfolder = new File(delPath);
				if(workfolder.exists() && workfolder.isDirectory())
				{
					FileUtils.deleteDirectory(workfolder);
				}
			}
		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());
		}
	}

	/* (non-Javadoc)
	 * @see hoot.services.wps.WPSProcesslet#init()
	 */
	@Override
	public void init() {
		//

	}
	
	/* (non-Javadoc)
	 * @see hoot.services.wps.WPSProcesslet#process(org.deegree.services.wps.ProcessletInputs, org.deegree.services.wps.ProcessletOutputs, org.deegree.services.wps.ProcessletExecutionInfo)
	 * 
	 * Process osm2ogr operation. For param filter see OSM2OgrProcesslet.xml
	 */
	@Override
	public void process(ProcessletInputs in, ProcessletOutputs out, ProcessletExecutionInfo info)
	  throws ProcessletException 
	{
		Map<String, String> args = parseRequestParams(in);
		
		try
		{
			String workingFolder = tempOutputPath + "/" + args.get("id") ;
			String outputFilePath = workingFolder + "/" + args.get("outputname")  + ".zip";
			String remove = args.get("removecache").toString();
			if(remove.equalsIgnoreCase("true"))
			{
				delPath = tempOutputPath + "/" + args.get("id") ;
			}
			
			File fOut = new File(outputFilePath);
	    if(!fOut.exists())
	    {
	    	throw new NativeInterfaceException("Missing output file",
	          NativeInterfaceException.HttpCode.SERVER_ERROR);
	    }
			FileInputStream fIn = null;
	    try
	    {
	    	fIn = new FileInputStream(fOut);
		    
		    org.apache.commons.io.IOUtils.copy(fIn,
		    		((ComplexOutput)out.getParameter("BinaryOutput")).getBinaryOutputStream());
	    }
	    finally
	    {
	    	try
	      {
	    		if (fIn != null)
	    		{
	    			fIn.close();
	    		}
	      }
	      catch (IOException e)
	      {
		      log.error(e.getMessage());
	      }
	    }
		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());
		}
	}
	
  protected Map<String, String> parseRequestParams(ProcessletInputs in)
  {
    Map<String, String> commandArgs = new HashMap<String,String>();
    List<ProcessletInput> params = in.getParameters();

    for (int i = 0; i < params.size(); i++)
    {
      ProcessletInput input = params.get(i);
      String id = input.getIdentifier().getCode();
      String value = ((LiteralInput) in.getParameter(id)).getValue().trim();
      try
      {
        commandArgs.put(id, value);
      }
      catch (Exception e)
      {
        log.error(e.getMessage());
      }
    }
    return commandArgs;
  }
	

}
