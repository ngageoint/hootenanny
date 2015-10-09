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
package hoot.services.validators.job;


import java.io.FileReader;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import hoot.services.HootProperties;

import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class JobFieldsValidator {
	private String _resourceName = null;
	private JSONObject _metaData = null;
	private String _metaDataPath = null;

	private static final Logger log = LoggerFactory.getLogger(JobFieldsValidator.class);

	public JobFieldsValidator(String resourceName)
	{

		_resourceName = resourceName;

		try
		{

			if (_metaDataPath ==  null)
			{
				_metaDataPath = HootProperties.getProperty("homeFolder");
				_metaDataPath += "/scripts/services_fields_metadata.json";
				_loadFieldsMetaData();
			}

		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());
		}
	}

	// validate required fields list.


	public boolean validateField( String fieldName, String fieldValue)
	{

	/*
		String resourceName = _resourceName;
		boolean isValid = true;

		Object resMeta = _metaData.get(resourceName);
		if(resMeta != null)
		{
			Object fieldMeta  = ((JSONObject)resMeta).get(fieldName);
			if(fieldMeta != null)
			{
				JSONObject field = (JSONObject)fieldMeta;

				// first check if the field is required field.
				String requiredStr = _getFieldValueStr(field, "required", "false");
				boolean required = Boolean.parseBoolean(requiredStr);
				if(required == false)
				{
					// if optional we just say valid.
					if(fieldValue ==  null || fieldValue.length() == 0)
					{
						return true;
					}
				}
				else
				{
				// When empty value then it is invalid
					if(fieldValue ==  null || fieldValue.length() == 0)
					{
						return false;
					}
				}


				// get the field type. If none specified we treat as String
				String fieldDataType = _getFieldValueStr(field, "type", "String");
				String valueRange = _getFieldValueStr(field, "value_range", null);
				String maxRangeStr = _getFieldValueStr(field, "max_value_range", null);
				String minRangeStr = _getFieldValueStr(field, "min_value_range", null);


				if(fieldDataType.equalsIgnoreCase("double") ||
						fieldDataType.equalsIgnoreCase("float"))
				{
					boolean isMinValid = _validateDoubleStr(minRangeStr, fieldValue, "<");
					boolean isMaxValid = _validateDoubleStr(maxRangeStr, fieldValue, ">");
					isValid = isMinValid && isMaxValid;

				}
				else if(fieldDataType.equalsIgnoreCase("int"))
				{
					boolean isMinValid = _validateIntStr(minRangeStr, fieldValue, "<");
					boolean isMaxValid = _validateIntStr(maxRangeStr, fieldValue, ">");

					isValid = isMinValid && isMaxValid;
				}
				else
				{
					// treat it as String
					if(valueRange != null && valueRange.length() > 0)
					{
						boolean isValInList = false;
						String[] valList = valueRange.split(",");
						for(int i=0;  i< valList.length; i++)
						{
							if(valList[i].equalsIgnoreCase(fieldValue))
							{
								isValInList = true;
								break;
							}

						}

						isValid = isValInList;
					}
					else
					{
						isValid = true;
					}
				}
			}
			else
			{
			// nothing to check with so we pass
				isValid = true;
			}

		}
		else
		{
			// nothing to check with so we pass
			isValid = true;
		}

		return isValid;*/
		return true;
	}
	private void _loadFieldsMetaData() throws Exception
	{

		FileReader fReader = new FileReader(_metaDataPath);
		JSONParser parser = new JSONParser();
		_metaData = (JSONObject)parser.parse(fReader);
		fReader.close();
	}

	public boolean validateRequiredExists(Map<String,String> fields, List<String>missingList)
	{

		String resourceName = _resourceName;
		boolean isValid = true;

		Object resMeta = _metaData.get(resourceName);
		if(resMeta != null)
		{
			// get the list of required fields
			List<String> reqList = new ArrayList<String>();
			JSONObject o = (JSONObject)resMeta;
    	Iterator iter = o.entrySet().iterator();
    	while (iter.hasNext()) {
  			Map.Entry mEntry = (Map.Entry) iter.next();
  			String key = (String)mEntry.getKey();
  			JSONObject val = (JSONObject)mEntry.getValue();

  			Object oReq = val.get("required");
  			Boolean isReq = false;
  			if(oReq != null)
  			{
  				isReq = oReq.toString().equalsIgnoreCase("true");
  			}

  			if(isReq)
  			{
  				reqList.add(key);
  			}
  		}

    	// check for empty field val
    	iter = fields.entrySet().iterator();
    	while(iter.hasNext())
    	{
    		Map.Entry mEntry = (Map.Entry) iter.next();
  			Object key = mEntry.getKey();
  			Object val = mEntry.getValue();

  			if(val == null)
  			{
  				return false;
  			}

  			if(key == null)
  			{
  				return true;
  			}

    	}


    	// now see if all required fields exists
    	for(int i=0; i<reqList.size(); i++)
    	{
    		String key = reqList.get(i);
    		if(fields.get(key) == null)
    		{
    			missingList.add(key);
    			isValid = false;
    		}
    	}

		}
		else
		{
			// nothing to check with so we pass
			isValid = true;
		}

		return isValid;

	}


}
