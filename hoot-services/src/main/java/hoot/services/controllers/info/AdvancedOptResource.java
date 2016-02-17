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
package hoot.services.controllers.info;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.InputStreamReader;
import java.util.Iterator;
import java.util.Map;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;

import hoot.services.HootProperties;
import hoot.services.utils.ResourceErrorHandler;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

@Path("/advancedopts")
public class AdvancedOptResource
{
	private static final Logger log = LoggerFactory.getLogger(AdvancedOptResource.class);
	private String _asciidocPath = null;
	private String _templatePath = null;
	private String _homeFolder = null;
	private String _refOverridePath = null;
	private String _horzOverridePath = null;
	private String _aveOverridePath = null;
	protected JSONObject _doc = null;
	protected JSONArray _template = null;
	protected JSONArray _refTemplate = null;
	protected JSONArray _hrzTemplate = null;
	protected JSONArray _aveTemplate = null;
	protected JSONObject _refOverride = null;
	protected JSONObject _horzOverride = null;
	protected JSONObject _aveOverride = null;

	public AdvancedOptResource()
	{
		try
		{
			_homeFolder = HootProperties.getProperty("homeFolder");
			_asciidocPath = HootProperties.getProperty("configAsciidocPath");

			File f = new File(_homeFolder + "/" + _asciidocPath);
			if (!f.exists())
			{
				throw new Exception("Missing required file: " + f.getAbsolutePath());
			}

			_templatePath = HootProperties.getProperty("advOptTemplate");
			_refOverridePath = HootProperties.getProperty("advOptRefOverride");
			_horzOverridePath = HootProperties
					.getProperty("advOptHorizontalOverride");
			_aveOverridePath = HootProperties.getProperty("advOptAverageOverride");
		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());
		}
	}

	@GET
	@Path("/getoptions")
	@Produces(MediaType.TEXT_PLAIN)
	public Response getOptions(@QueryParam("conftype") final String confType,
		@QueryParam("force") final String isForce)
	{
		FileReader fr = null;
		JSONArray template = null;
		try
		{

			// Force option should only be used to update options list by
			// administrator
			boolean doForce = false;
			if (isForce != null)
			{
				doForce = isForce.equalsIgnoreCase("true");
			}

			_getOverrides(isForce);
			if (_doc == null || doForce)
			{
				_doc = new JSONObject();
				_parseAsciidoc();
			}

			if (_doc != null)
			{
				JSONParser par = new JSONParser();
				if (confType.equalsIgnoreCase("reference"))
				{

					if (_refTemplate == null || doForce)
					{
						_refTemplate = new JSONArray();
						_refTemplate.add(_refOverride);
					}
					template = _refTemplate;

				}
				else if (confType.equalsIgnoreCase("horizontal"))
				{
					if (_hrzTemplate == null || doForce)
					{
						_hrzTemplate = new JSONArray();
						_hrzTemplate.add(_horzOverride);
					}
					template = _hrzTemplate;

				}
				else if (confType.equalsIgnoreCase("average"))
				{
					if (_aveTemplate == null || doForce)
					{
						_aveTemplate = new JSONArray();
						_aveTemplate.add(_aveOverride);
					}
					template = _aveTemplate;

				}
				else
				{
					if (_doc == null || _template == null || doForce)
					{
						fr = new FileReader(_homeFolder + "/" + _templatePath);
						_template = (JSONArray) par.parse(fr);
						_generateRule(_template, null);
					}
					template = _template;
				}
			}
			else
			{
				throw new Exception("Failed to populate asciidoc information.");
			}

		}
		catch (Exception ex)
		{
			ResourceErrorHandler.handleError(
					"Error getting advanced options: " + ex.toString(),
					Status.INTERNAL_SERVER_ERROR, log);
		}
		finally
		{
			if (fr != null)
			{
				try
				{
					fr.close();
				}
				catch (Exception ee)
				{

				}
			}
		}

		return Response.ok(template.toJSONString(), MediaType.APPLICATION_JSON)
				.build();
	}

	public void _getOverrides(final String isForce) throws Exception
	{
		FileReader frRef = null;
		FileReader frHrz = null;
		FileReader frAve = null;
		try
		{
			// Force option should only be used to update options list by
			// administrator
			boolean doForce = false;
			if (isForce != null)
			{
				doForce = isForce.equalsIgnoreCase("true");
			}
			if (_horzOverride == null || _refOverride == null || doForce)
			{
				JSONParser par = new JSONParser();
				frRef = new FileReader(_homeFolder + "/" + _refOverridePath);
				_refOverride = (JSONObject) par.parse(frRef);

				frHrz = new FileReader(_homeFolder + "/" + _horzOverridePath);
				_horzOverride = (JSONObject) par.parse(frHrz);

				frAve = new FileReader(_homeFolder + "/" + _aveOverridePath);
				_aveOverride = (JSONObject) par.parse(frAve);
			}
		}
		catch (Exception ex)
		{
			throw ex;
		}
		finally
		{
			if (frRef != null)
			{
				try
				{
					frRef.close();
				}
				catch (Exception ee)
				{
					throw ee;
				}
			}

			if (frHrz != null)
			{
				try
				{
					frHrz.close();
				}
				catch (Exception ee)
				{
					throw ee;
				}
			}

			if (frAve != null)
			{
				try
				{
					frAve.close();
				}
				catch (Exception ee)
				{
					throw ee;
				}
			}
		}
	}

	protected String _getDepKeyVal(String sDefVal)
	{
		try
		{
			int iStart = sDefVal.indexOf('{') + 1;
			int iEnd = sDefVal.indexOf('}');
			String depKey = sDefVal.substring(iStart, iEnd).trim();
			if (depKey != null && depKey.length() > 0)
			{
				// now find the dep value
				JSONObject oDep = (JSONObject) _doc.get(depKey);
				String depDefVal = oDep.get("Default Value").toString();
				return depDefVal;
			}
		}
		catch (Exception e1)
		{

		}
		return "";
	}

	@SuppressWarnings({ "unchecked", "rawtypes" })
	protected void _generateRule(JSONArray a, JSONObject p) throws Exception
	{
		// for each options in template apply the value

		for (int i = 0; i < a.size(); i++)
		{
			Object o = a.get(i);
			if (o instanceof JSONObject)
			{
				JSONObject curOpt = (JSONObject) o;
				// first check to see if there is key then apply the asciidoc val
				Object oKey = curOpt.get("hoot_key");
				if (oKey != null)
				{
					String sKey = oKey.toString();
					Object oDocKey = _doc.get(sKey);
					if (oDocKey != null)
					{
						JSONObject jDocKey = (JSONObject) oDocKey;

						Object oAttrib = jDocKey.get("Data Type");
						if (oAttrib != null)
						{
							String sDataType = oAttrib.toString().trim();
							// We can not determine list vs multilist from asciidoc so we will
							// skip for the data type
							if (!sDataType.equals("list") && sDataType.length() > 0)
							{
								curOpt.put("elem_type", sDataType);
								if (sDataType.equalsIgnoreCase("bool"))
								{
									// bool type is checkbox
									curOpt.put("elem_type", "checkbox");
								}
							}
						}

						oAttrib = jDocKey.get("Default Value");
						if (oAttrib != null)
						{
							String sDefVal = oAttrib.toString().trim();
							if (sDefVal.length() > 0)
							{
								// It is poting to other val
								if (sDefVal.startsWith("$"))
								{
									String depDefVal = _getDepKeyVal(sDefVal);
									curOpt.put("defaultvalue", depDefVal);
								}
								else
								{
									curOpt.put("defaultvalue", sDefVal);
								}
							}
						}

						oAttrib = jDocKey.get("Description");
						if (oAttrib != null)
						{
							String sDesc = oAttrib.toString().trim();
							if (sDesc.length() > 0)
							{
								curOpt.put("description", sDesc);
							}
						}

						// handle override
						oAttrib = curOpt.get("override");
						if (oAttrib != null)
						{
							JSONObject override = (JSONObject) oAttrib;

							Iterator it = override.entrySet().iterator();
							@SuppressWarnings("unused")
							boolean hasNext = true;
							while (it.hasNext())
							{
								Map.Entry pair = (Map.Entry) it.next();
								curOpt.put(pair.getKey(), pair.getValue());
							}
							// remove override element
							curOpt.remove("override");
						}

					}
				}
				else
				{
					// Second check for hoot_val and if there is one then apply descripton
					// from asciidoc
					// This can be checked from parent object
					Object oVal = curOpt.get("hoot_val");
					if (oVal != null)
					{
						String sVal = oVal.toString();
						if (p != null)
						{
							// parent always have to have hoot_key for hoot_val
							Object pKey = p.get("hoot_key");
							if (pKey != null)
							{
								// try to get default list from parent
								String sPKey = pKey.toString();
								Object oDocPKey = _doc.get(sPKey);
								if (oDocPKey != null)
								{
									JSONObject jDocPKey = (JSONObject) oDocPKey;
									Object oDocDefList = jDocPKey.get("Default List");

									if (oDocDefList != null)
									{
										JSONObject defList = (JSONObject) oDocDefList;
										Iterator it = defList.entrySet().iterator();
										boolean hasNext = true;
										while (hasNext)
										{
											Map.Entry pair = (Map.Entry) it.next();
											hasNext = it.hasNext();
											if (pair.getKey().toString().equalsIgnoreCase(sVal))
											{
												String sDefListDesc = pair.getValue().toString().trim();
												if (sDefListDesc.length() > 0)
												{
													curOpt.put("description", sDefListDesc);
													curOpt.put("defaultvalue", "true");

												}
												hasNext = false;
											}
										}
									}

									// If the parent is boolean then try to get default value
									Object oDocDataType = jDocPKey.get("Data Type");
									if (oDocDataType != null
											&& oDocDataType.toString().equalsIgnoreCase("bool"))
									{
										Object oDocDefVal = jDocPKey.get("Default Value");
										if (oDocDefVal != null)
										{
											if (oDocDefVal.toString().equalsIgnoreCase("true"))
											{
												if (sVal.equalsIgnoreCase("true"))
												{
													curOpt.put("isDefault", "true");
												}

												if (sVal.equalsIgnoreCase("false"))
												{
													curOpt.put("isDefault", "false");
												}
											}

											if (oDocDefVal.toString().equalsIgnoreCase("false"))
											{
												if (sVal.equalsIgnoreCase("true"))
												{
													curOpt.put("isDefault", "false");
												}

												if (sVal.equalsIgnoreCase("false"))
												{
													curOpt.put("isDefault", "true");
												}
											}
										}

									}
								}
							}
						}
						// handle override
						Object oAttrib = curOpt.get("override");
						if (oAttrib != null)
						{
							JSONObject override = (JSONObject) oAttrib;

							Iterator it = override.entrySet().iterator();
							while (it.hasNext())
							{
								Map.Entry pair = (Map.Entry) it.next();
								curOpt.put(pair.getKey(), pair.getValue());
							}
							// remove override element
							curOpt.remove("override");
						}
					}

				}

				// now check for members and if one exist then recurse

				Object oValMembers = curOpt.get("members");
				if (oValMembers != null && (oValMembers instanceof JSONArray))
				{
					_generateRule((JSONArray) oValMembers, curOpt);
				}

			}
			else if (o instanceof JSONArray)
			{
			}
		}

		// do recursion to visit each object and try to update value
	}

	// If line starts with "* " then attribute field
	// "Data Type:"
	// "Default Value:"

	// If line starts with "** " then list item field
	protected String _parseLine(final String line, final String curOptName)
			throws Exception
	{
		String optName = curOptName;
		// If line starts with "=== " then it is option field
		if (line.startsWith("=== "))
		{
			optName = line.substring(3).trim();
			JSONObject field = new JSONObject();
			_doc.put(optName, field);
		}
		else
		{
			Object curObject = _doc.get(optName);
			if (curObject != null)
			{
				JSONObject curOpt = (JSONObject) curObject;

				if (line.startsWith("* "))
				{
					String field = line.substring(1).trim();
					String[] parts = field.split(":");
					if (parts.length > 1)
					{
						String k = parts[0];
						String v = "";
						for (int i = 1; i < parts.length; i++)
						{
							if (v.length() > 0)
							{
								v += ":";
							}
							v += parts[i].replaceAll("`", "");
						}
						curOpt.put(k.trim(), v.trim());
					}
				}
				else if (line.startsWith("** ")) // must be list item
				{
					// Try to get default list object for current option
					String listKey = "Default List";
					Object o = curOpt.get(listKey);
					JSONObject defList = new JSONObject();
					if (o != null)
					{
						defList = (JSONObject) o;
					}
					else
					{
						curOpt.put(listKey, defList); // create new if not exist
					}

					String curLine = line.substring(2).trim();
					// try getting item description
					String[] parts = curLine.split("` - ");
					if (parts.length > 0)
					{
						String k = parts[0].replaceAll("`", "").trim();
						String v = "";
						if (parts.length > 1) // there is description
						{
							for (int i = 1; i < parts.length; i++)
							{
								if (v.length() > 0)
								{
									v += "` - ";
								}
								v += parts[i];
							}
						}
						defList.put(k.trim(), v.trim());
					}
				}
				else
					// must be description
				{
					String curLine = line.trim();
					if (curLine.length() > 0)
					{
						String k = "Description";
						Object o = curOpt.get(k);
						if (o == null)
						{
							curOpt.put(k, curLine);
						}
						else
						{
							String v = curOpt.get(k).toString() + " " + curLine;
							curOpt.put(k, v.trim());
						}
					}
				}
			}
		}

		return optName;
	}

	protected void _parseAsciidoc() throws Exception
	{
		FileInputStream fstream = null;
		InputStreamReader istream = null;
		BufferedReader br = null;

		try
		{
			// Open the file
			fstream = new FileInputStream(_homeFolder + "/" + _asciidocPath);
			istream = new InputStreamReader(fstream);
			br = new BufferedReader(istream);

			String strLine;

			String curOptName = null;
			// Read File Line By Line
			while ((strLine = br.readLine()) != null)
			{
				// Print the content on the console
				curOptName = _parseLine(strLine, curOptName);
			}
		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());
			throw ex;
		}
		finally
		{
			if (br != null)
			{
				br.close();
			}

			if (istream != null)
			{
				istream.close();
			}

			if (fstream != null)
			{
				fstream.close();
			}
		}
	}
}
