package hoot.services.models.review;

import hoot.services.models.osm.ElementInfo;

import javax.xml.bind.annotation.XmlRootElement;

/**
 * A request for review references
 */
@XmlRootElement
public class ReviewRefsRequest
{
	private ElementInfo[] elementInfos;
  public ElementInfo[] getElementInfos() { return elementInfos; }
  public void setElementInfos(ElementInfo[] infos) { this.elementInfos = infos; }
  
  public ReviewRefsRequest()
  {
  }
  
  public ReviewRefsRequest(final ElementInfo[] elementInfos)
  {
  	this.elementInfos = elementInfos;
  }
  
  @Override
  public String toString()
  {
  	String str = "";
  	if (elementInfos != null)
  	{
  		for (int i = 0; i < elementInfos.length; i++)
    	{
    		str += elementInfos[i].toString();
    	}
  	}
  	return str;
  }
}