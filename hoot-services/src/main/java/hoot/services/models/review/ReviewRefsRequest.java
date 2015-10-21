package hoot.services.models.review;

import hoot.services.models.osm.ElementInfo;

import javax.xml.bind.annotation.XmlRootElement;

/**
 * A request for review references
 */
@XmlRootElement
public class ReviewRefsRequest
{
	private ElementInfo[] queryElements;
  public ElementInfo[] getQueryElements() { return queryElements; }
  public void setQueryElements(ElementInfo[] infos) { this.queryElements = infos; }
  
  public ReviewRefsRequest()
  {
  }
  
  public ReviewRefsRequest(final ElementInfo[] elementInfos)
  {
  	this.queryElements = elementInfos;
  }
  
  @Override
  public String toString()
  {
  	String str = "";
  	if (queryElements != null)
  	{
  		for (int i = 0; i < queryElements.length; i++)
    	{
    		str += queryElements[i].toString();
    	}
  	}
  	return str;
  }
}