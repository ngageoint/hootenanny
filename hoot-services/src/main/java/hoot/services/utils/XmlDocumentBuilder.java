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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

package hoot.services.utils;

import java.io.File;
import java.io.IOException;
import java.io.StringReader;
import java.io.StringWriter;
import java.io.Writer;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.TransformerFactoryConfigurationError;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathFactory;

import org.apache.commons.io.FileUtils;
import org.apache.xml.serialize.OutputFormat;
import org.apache.xml.serialize.XMLSerializer;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;


/**
 * General XML utilities
 */
@SuppressWarnings("deprecation")
public class XmlDocumentBuilder
{
  /**
   * Creates a new XML DOM
   * 
   * @return XML document
   * @throws IOException
   */
  public static Document create() throws IOException
  {
    DocumentBuilderFactory dBF = DocumentBuilderFactory.newInstance();
    DocumentBuilder builder;
    try
    {
      builder = dBF.newDocumentBuilder();
    }
    catch (ParserConfigurationException e)
    {
      throw new IOException("Error creating document builder. (" + e.getMessage() + ")");
    }
    return builder.newDocument();
  }

  /**
   * Creates an XPATH instance for querying with
   * 
   * @return an XPATH instance
   */
  public static XPath createXPath()
  {
    XPathFactory factory = XPathFactory.newInstance();
    return factory.newXPath();
  }

  /**
   * Parses an XML string into a DOM
   * 
   * @param xml an XML string
   * @return an XML DOM
   * @throws SAXException
   * @throws IOException
   * @throws ParserConfigurationException
   */
  public static Document parse(String xml) throws SAXException, IOException,
    ParserConfigurationException
  {
    return parse(xml, true);
  }
  
  /**
   * Parses an XML string into a DOM
   * 
   * @param xml an XML string
   * @param namespaceAware determines whether namespaces are respected during the parsing
   * @return an XML DOM
   * @throws SAXException
   * @throws IOException
   * @throws ParserConfigurationException
   */
  public static Document parse(String xml, boolean namespaceAware) throws SAXException, 
    IOException, ParserConfigurationException
  {
    DocumentBuilderFactory domFactory = DocumentBuilderFactory.newInstance();
    domFactory.setNamespaceAware(namespaceAware); // never forget this!
    DocumentBuilder builder;
    builder = domFactory.newDocumentBuilder();
    
    InputSource is = new InputSource();
    is.setCharacterStream(new StringReader(xml));
    
    return builder.parse(is);
  }
  
  /**
   * Creates a DOM from file content
   * 
   * @param file file to read
   * @return an XML DOM
   * @throws ParserConfigurationException 
   * @throws IOException 
   * @throws SAXException 
   */
  public static Document parse(File file) throws SAXException, IOException, 
    ParserConfigurationException
  {
    return parse(FileUtils.readFileToString(file));
  }
  
  /**
   * Returns a string representation of an XML DOM
   * 
   * @param document an XML DOM
   * @return an XML string
   * @throws IOException
   */
  public static String toString(Document document) throws IOException
  {
    StringWriter writer = new StringWriter();
    write(document, writer);
    return writer.toString();
  }
  
  /**
   * 
   * 
   * @param node
   * @return
   * @throws TransformerFactoryConfigurationError 
   * @throws TransformerException 
   */
  public static String nodeToString(final Node node) throws TransformerFactoryConfigurationError, 
    TransformerException
  {
    StringWriter writer = new StringWriter();
    Transformer transformer = TransformerFactory.newInstance().newTransformer();
    transformer.transform(new DOMSource(node), new StreamResult(writer));
    return writer.toString();
  }

  /**
   * Writes an XML DOM to a writer
   * 
   * @param document XML DOM to write
   * @param out writer to write the XML DOM to
   * @throws IOException if unable to write the XML DOM
   */
  public static void write(Document document, Writer out) throws IOException
  {
    // happy to replace this code w/ the non-deprecated code, but I couldn't get the transformer 
    // approach to work. 
    OutputFormat format = new OutputFormat(document);
    format.setIndenting(true);
    format.setIndent(2);
    XMLSerializer serializer = new XMLSerializer(out, format);
    serializer.serialize(document);
  }
  
  /**
   * Walks the document and removes all nodes of the specified type and specified name. 
   * If name is null, then the node is removed if the type matches.
   *
   * @param node starting node
   * @param nodeType type of nodes to remove
   * @param name name of nodes to remove
   */
  public static void removeAll(Node node, final short nodeType, final String name) 
  {
    if (node.getNodeType() == nodeType && (name == null || node.getNodeName().equals(name))) 
    {
      node.getParentNode().removeChild(node);
    } 
    else 
    {
      NodeList list = node.getChildNodes();
      for (int i=0; i<list.getLength(); i++) 
      {
        removeAll(list.item(i), nodeType, name);
      }
    }
  }
}
