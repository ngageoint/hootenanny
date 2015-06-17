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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.info;

import javax.xml.bind.annotation.XmlRootElement;

/**
 * Represents details about the web services
 */
@XmlRootElement
public class ServicesDetail 
{
  @XmlRootElement
  public static class Property
  {
	public Property()
	{
		
	}
	  
	private String name;
	public String getName() { return name; }
	public void setName(String name) { this.name = name; }
	
	private String value;
	public String getValue() { return value; }
	public void setValue(String value) { this.value = value; }
  }
  
  @XmlRootElement
  public static class ServicesResource
  {
	public ServicesResource()
	{
		
	}
	  
	private String type;
	public String getType() { return type; }
	public void setType(String type) { this.type = type; }
	
	private String url;
	public String getUrl() { return url; }
	public void setUrl(String url) { this.url = url; }
  }
	
  private Property[] properties;
  public Property[] getProperties() { return properties; }
  public void setProperties(Property[] properties) { this.properties = properties; }
  
  private String classPath;
  public String getClassPath() { return classPath; }
  public void setClassPath(String classPath) { this.classPath = classPath; }
  
  private ServicesResource[] resources;
  public ServicesResource[] getResources() { return resources; }
  public void setResources(ServicesResource[] resources) { this.resources = resources; }
	
  public ServicesDetail()
  {
	  
  }
}
