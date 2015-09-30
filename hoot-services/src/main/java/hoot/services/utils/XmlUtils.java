package hoot.services.utils;

import java.io.StringWriter;

import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.TransformerFactoryConfigurationError;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathFactory;

import org.w3c.dom.NodeList;

/**
 * Various xml utilities
 */
public class XmlUtils
{
	/**
   * Creates an XPATH instance for querying with
   * 
   * @return an XPATH instance
   */
  public static XPath createXPath()
  {
    return XPathFactory.newInstance().newXPath();
  }
  
  /**
   * Converts an XML node list to a string
   * 
   * @param nodeList
   * @return a string representation of the node list
   * @throws TransformerFactoryConfigurationError
   * @throws TransformerException
   */
  public static String nodeListToString(final NodeList nodeList) 
  	throws TransformerFactoryConfigurationError, TransformerException
  {
  	String result = "";
  	for (int i = 0; i < nodeList.getLength(); i++)
  	{
  		StringWriter sw = new StringWriter();
    	Transformer serializer = TransformerFactory.newInstance().newTransformer();
    	serializer.transform(new DOMSource(nodeList.item(i)), new StreamResult(sw));
    	result += sw.toString(); 
  	}
  	return result;
  }
}
