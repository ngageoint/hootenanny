package hoot.services.utils;

import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement
/**
 * Simple web request container for a string array
 */
public class StringsWebWrapper
{
	private String[] values;
  public String[] getValues() { return values; }
  public void setValues(String[] values) 
  { 
    this.values = values;
  } 
  
  public StringsWebWrapper()
  {
  	
  }
  
  public StringsWebWrapper(final String[] values)
  {
  	this.values = values;
  }
}
