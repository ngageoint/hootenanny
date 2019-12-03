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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.josm;

import java.util.List;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Map;
import java.util.HashMap;
import java.util.Arrays;

import org.openstreetmap.josm.data.osm.AbstractPrimitive;
import org.openstreetmap.josm.data.osm.OsmPrimitive;
import org.openstreetmap.josm.data.osm.Node;
import org.openstreetmap.josm.data.osm.Way;
import org.openstreetmap.josm.data.osm.Relation;
import org.openstreetmap.josm.data.osm.RelationMember;
import org.openstreetmap.josm.tools.Logging;
import org.openstreetmap.josm.data.preferences.JosmBaseDirectories;
import org.openstreetmap.josm.data.preferences.JosmUrls;
import org.openstreetmap.josm.spi.preferences.Config;
import org.openstreetmap.josm.data.Preferences;
import org.openstreetmap.josm.command.PseudoCommand;
import org.openstreetmap.josm.command.Command;
import org.openstreetmap.josm.command.DeleteCommand;
import org.openstreetmap.josm.actions.DeleteAction;
import org.openstreetmap.josm.command.SequenceCommand;

/**
 * TODO
 */
public class JosmUtils
{
  /**
   * TODO
   */
  public static void initJosm(String logLevel) throws Exception
  {
    setLogLevel(logLevel);
    Logging.debug("Initializing JOSM...");
    Preferences pref = Preferences.main();
    Config.setPreferencesInstance(pref);
    Config.setBaseDirectoriesProvider(JosmBaseDirectories.getInstance());
    Config.setUrlsProvider(JosmUrls.getInstance());
    DeleteCommand.setDeletionCallback(DeleteAction.defaultDeletionCallback);
  }

  /**
   * TODO
   */
  public static void setLogLevel(String logLevel) throws Exception
  {
    switch (logLevel.toUpperCase())
    {
      case "FATAL":
        Logging.setLogLevel(Logging.LEVEL_ERROR);
        break;
      case "ERROR":
        Logging.setLogLevel(Logging.LEVEL_ERROR);
        break;
      case "WARN":
        Logging.setLogLevel(Logging.LEVEL_WARN);
        break;
      case "STATUS":
        Logging.setLogLevel(Logging.LEVEL_INFO);
        break;
      case "INFO":
        Logging.setLogLevel(Logging.LEVEL_INFO);
        break;
      case "DEBUG":
        Logging.setLogLevel(Logging.LEVEL_DEBUG);
        break;
      case "TRACE":
        Logging.setLogLevel(Logging.LEVEL_TRACE);
        break;
      default:
        throw new Exception("Invalid log level: " + logLevel);
    }
  }

 /*
  * TODO
  */
  public static String getElementMapKey(OsmPrimitive element)
  {
    //Logging.trace(
      //"key: " + element.getClass().getSimpleName() + ":" + String.valueOf(element.getUniqueId()));
    return element.getClass().getSimpleName() + ":" + String.valueOf(element.getUniqueId());
  }

  /**
   * TODO
   *
   * There's probably some utility function to do this in JOSM, just haven't found it.
   */
  public static Map<String, AbstractPrimitive> hydrate(OsmPrimitive element)
  {
    //Logging.trace("Hydrating element: " + element.toString() + "...");
    if (element instanceof Node)
    {
      return hydrate((Node)element);
    }
    else if (element instanceof Way)
    {
      return hydrate((Way)element);
    }
    else if (element instanceof Relation)
    {
      return hydrate((Relation)element);
    }
    return null;
  }

  /**
   * TODO
   */
  public static String elementsToString(Collection<? extends AbstractPrimitive> elements)
  {
    String elementsStr = "";
    for (AbstractPrimitive element : elements)
    {
      elementsStr += element.toString() + ", keys: " + element.getKeys().toString() + ";";
    }
    return elementsStr;
  }

  /**
   * TODO

     Not all of the commands implement toString()
   */
  public static String commandToString(Command command, boolean printElements)
  {
    String str = "";
    if (command instanceof SequenceCommand)
    {
      SequenceCommand seqCmd = (SequenceCommand)command;
      str += "Name: " + seqCmd.getName() + ", description: " + seqCmd.getDescriptionText();
      str += ", commands: " + Arrays.toString(seqCmd.getChildren().toArray(new PseudoCommand[]{}));
      if (printElements)
      {
        str += ", elements: " + elementsToString(seqCmd.getParticipatingPrimitives());
      }
    }
    else
    {
      str += command.toString() + ", description: " + command.getDescriptionText();
    }
    return str;
  }

  /**
   * TODO
   */
  public static List<String> getDeletedElementIds(Command command) throws Exception
  {
    if (command instanceof DeleteCommand)
    {
      return getDeletedElementIds((DeleteCommand)command);
    }
    else if (command instanceof SequenceCommand)
    {
      return getDeletedElementIds((SequenceCommand)command);
    }
    else
    {
      return new ArrayList<String>();
    }
  }

  private static List<String> getDeletedElementIds(DeleteCommand deleteCommand)
  {
    List<String> deletedElementIds = new ArrayList<String>();
    Collection<? extends OsmPrimitive> deletedElements = deleteCommand.getParticipatingPrimitives();
    for (OsmPrimitive deletedElement : deletedElements)
    {
      deletedElementIds.add(JosmUtils.getElementMapKey(deletedElement));
    }
    return deletedElementIds;
  }

  private static List<String> getDeletedElementIds(SequenceCommand seqCommand)
  {
    List<String> deletedElementIds = new ArrayList<String>();
    Collection<PseudoCommand> childCmds = seqCommand.getChildren();
    for (PseudoCommand childCmd : childCmds)
    {
      if (childCmd instanceof DeleteCommand)
      {
        deletedElementIds.addAll(JosmUtils.getDeletedElementIds((DeleteCommand)childCmd));
      }
    }
    return deletedElementIds;
  }  

  private static Map<String, AbstractPrimitive> hydrate(Node node)
  {
    // node doesn't have any children
    //Logging.trace("Hydrating node: " + node.toString() + "...");
    Map<String, AbstractPrimitive> elementWithChildren = new HashMap<String, AbstractPrimitive>();
    elementWithChildren.put(getElementMapKey(node), node);
    return elementWithChildren;
  }

  private static Map<String, AbstractPrimitive> hydrate(Way way)
  {
    //Logging.trace("Hydrating way: " + way.toString() + "...");
    Map<String, AbstractPrimitive> elementWithChildren = new HashMap<String, AbstractPrimitive>();
    elementWithChildren.put(getElementMapKey(way), way);
    List<Node> wayNodes = way.getNodes();
    //Logging.trace("wayNodes size: " + wayNodes.size());
    for (Node wayNode : wayNodes)
    {
      elementWithChildren.put(getElementMapKey(wayNode), wayNode);
    }
    //Logging.trace("elementWithChildren size: " + elementWithChildren.size());
    return elementWithChildren;
  }

  private static Map<String, AbstractPrimitive> hydrate(Relation relation)
  {
    //Logging.trace("Hydrating relation: " + relation.toString() + "...");
    Map<String, AbstractPrimitive> elementWithChildren = new HashMap<String, AbstractPrimitive>();
    elementWithChildren.put(getElementMapKey(relation), relation);
    List<RelationMember> members = relation.getMembers();
    for (RelationMember member : members)
    {
      elementWithChildren.put(getElementMapKey(member.getMember()), member.getMember());
    }
    return elementWithChildren;
  }
}
