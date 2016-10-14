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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.osm;

import static com.querydsl.core.group.GroupBy.groupBy;
import static hoot.services.models.db.QCurrentNodes.currentNodes;
import static hoot.services.utils.DbUtils.createQuery;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import javax.xml.transform.TransformerException;

import org.apache.xpath.XPathAPI;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;

import hoot.services.models.db.CurrentNodes;
import hoot.services.models.osm.Element;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.models.osm.ElementFactory;
import hoot.services.utils.DbUtils.EntityChangeType;


class ChangesetErrorChecker {
    private static final Logger logger = LoggerFactory.getLogger(ChangesetErrorChecker.class);

    private final Document changesetDoc;
    private final long mapId;

    ChangesetErrorChecker(Document changesetDoc, long mapId) {
        this.changesetDoc = changesetDoc;
        this.mapId = mapId;
    }

    void checkForVersionErrors() {
        logger.debug("Checking for element version errors...");

        for (ElementType elementType : ElementType.values()) {
            if (elementType != ElementType.Changeset) {
                for (EntityChangeType entityChangeType : EntityChangeType.values()) {
                    NodeList elementXmlNodes = null;
                    try {
                        elementXmlNodes = XPathAPI.selectNodeList(changesetDoc, "//osmChange/"
                                + entityChangeType.toString().toLowerCase() + "/" + elementType.toString().toLowerCase());
                    }
                    catch (TransformerException e) {
                        throw new RuntimeException("Error invoking XPathAPI!", e);
                    }

                    Map<Long, Long> elementIdsToVersionsFromChangeset = new HashMap<>();
                    for (int i = 0; i < elementXmlNodes.getLength(); i++) {
                        long id = Long.parseLong(
                                elementXmlNodes.item(i).getAttributes().getNamedItem("id").getNodeValue());
                        long parsedVersion = Long.parseLong(
                                elementXmlNodes.item(i).getAttributes().getNamedItem("version").getNodeValue());
                        if (entityChangeType == EntityChangeType.CREATE) {
                            if (parsedVersion != 0) {
                                throw new IllegalArgumentException("Invalid version: " + parsedVersion +
                                        " specified for created " + elementType + " with ID: " + id +
                                        "; expected version 0.");
                            }
                        }
                        else {
                            elementIdsToVersionsFromChangeset.put(id, parsedVersion);
                        }
                    }

                    if ((entityChangeType != EntityChangeType.CREATE)
                            && (!elementIdsToVersionsFromChangeset.isEmpty())) {
                        Element prototype = ElementFactory.create(mapId, elementType);

                        Map<Long, Long> elementIdsToVersionsFromDb = createQuery(mapId)
                                .from(prototype.getElementTable())
                                .where(prototype.getElementIdField()
                                        .in(elementIdsToVersionsFromChangeset.keySet()))
                                .transform(groupBy(prototype.getElementIdField()).as(prototype.getElementVersionField()));

                        if (!elementIdsToVersionsFromDb.equals(elementIdsToVersionsFromChangeset)) {
                            throw new IllegalArgumentException("Invalid version specified for element(s).");
                        }
                    }
                }
            }
        }
    }

    //TODO: is this check actually necessary?
    void checkForElementVisibilityErrors() {
        logger.debug("Checking for element visibility errors...");

        // if a child element is referenced and is invisible, then fail.
        // elements are created visible by default with a create changeset, which is why we can skip checking
        // negative id's (elements being created). we're also skipping checking anything in the delete
        // sections, b/c since the elements are being deleted, they're visibility no longer is important.

        for (ElementType elementType : ElementType.values()) {
            if (elementType != ElementType.Changeset) {
                Set<Long> relationMemberIds = new HashSet<>();

                // I know there's a way to use the '|' operator in the xpath
                // query to make this simpler... its just not working
                for (EntityChangeType entityChangeType : EntityChangeType.values()) {
                    if (entityChangeType != EntityChangeType.DELETE) {
                        NodeList relationMemberIdXmlNodes;
                        try {
                            relationMemberIdXmlNodes = XPathAPI.selectNodeList(changesetDoc,
                                    "//osmChange/" + entityChangeType.toString().toLowerCase()
                                            + "/relation/member[@type = \"" + elementType.toString().toLowerCase() + "\"]");
                        }
                        catch (TransformerException e) {
                            throw new RuntimeException("Error invoking XPathAPI!", e);
                        }

                        for (int i = 0; i < relationMemberIdXmlNodes.getLength(); i++) {
                            // don't need to check for empty id here, b/c
                            // previous checking would have already errored out for it
                            long id = Long.parseLong(relationMemberIdXmlNodes.item(i).getAttributes()
                                    .getNamedItem("ref").getNodeValue());

                            if (id > 0) {
                                relationMemberIds.add(id);
                            }
                        }
                    }
                }

                if (!Element.allElementsVisible(mapId, elementType, relationMemberIds)) {
                    throw new IllegalStateException(elementType + " member(s) aren't visible for relation.");
                }
            }
        }

        Set<Long> wayNodeIds = new HashSet<>();
        for (EntityChangeType entityChangeType : EntityChangeType.values()) {
            if (entityChangeType != EntityChangeType.DELETE) {
                NodeList wayNodeIdXmlNodes = null;
                try {
                    wayNodeIdXmlNodes = XPathAPI.selectNodeList(changesetDoc,
                            "//osmChange/" + entityChangeType.toString().toLowerCase() + "/way/nd");
                }
                catch (TransformerException e) {
                    throw new RuntimeException("Error invoking XPathAPI!", e);
                }

                for (int i = 0; i < wayNodeIdXmlNodes.getLength(); i++) {
                    // don't need to check for empty id here, b/c previous
                    // checking would have already errored out for it
                    long id = Long.parseLong(wayNodeIdXmlNodes.item(i).getAttributes().getNamedItem("ref").getNodeValue());

                    if (id > 0) {
                        wayNodeIds.add(id);
                    }
                }
            }
        }

        if (!Element.allElementsVisible(mapId, ElementType.Node, wayNodeIds)) {
            throw new IllegalStateException("Way node(s) aren't visible for way.");
        }
    }

    Map<Long, CurrentNodes> checkForElementExistenceErrors() {
        logger.debug("Checking for element existence errors...");

        // if an element is referenced (besides in its own create change) and doesn't exist in the db, then fail

        Map<ElementType, Set<Long>> elementTypesToElementIds = new HashMap<>();
        for (ElementType elementType : ElementType.values()) {
            if (elementType != ElementType.Changeset) {
                elementTypesToElementIds.put(elementType, new HashSet<Long>());
            }
        }

        String emptyIdErrorMsg = "Element in changeset has empty ID.";

        // check relation members
        for (ElementType elementType : ElementType.values()) {
            if (elementType != ElementType.Changeset) {
                NodeList relationMemberIdXmlNodes = null;
                try {
                    relationMemberIdXmlNodes = XPathAPI.selectNodeList(changesetDoc,
                            "//osmChange/*/relation/member[@type = '" + elementType.toString().toLowerCase() + "']");
                }
                catch (TransformerException e) {
                    throw new RuntimeException("Error invoking XPathAPI!", e);
                }

                for (int i = 0; i < relationMemberIdXmlNodes.getLength(); i++) {
                    long id;
                    try {
                        id = Long.parseLong(relationMemberIdXmlNodes.item(i).getAttributes().getNamedItem("ref").getNodeValue());
                    }
                    catch (NumberFormatException | NullPointerException e) {
                        throw new IllegalArgumentException(emptyIdErrorMsg, e);
                    }

                    if (id > 0) {
                        elementTypesToElementIds.get(elementType).add(id);
                    }
                }
            }
        }

        // check way nodes
        NodeList wayNodeIdXmlNodes = null;
        try {
            wayNodeIdXmlNodes = XPathAPI.selectNodeList(changesetDoc, "//osmChange/*/way/nd/@ref");
        }
        catch (TransformerException e) {
            throw new RuntimeException("Error calling XPathAPI!", e);
        }

        for (int i = 0; i < wayNodeIdXmlNodes.getLength(); i++) {
            long id;
            try {
                id = Long.parseLong(wayNodeIdXmlNodes.item(i).getNodeValue());
            }
            catch (NumberFormatException | NullPointerException e) {
                throw new IllegalArgumentException(emptyIdErrorMsg, e);
            }

            if (id > 0) {
                elementTypesToElementIds.get(ElementType.Node).add(id);
            }
        }

        // check top level elements
        for (EntityChangeType entityChangeType : EntityChangeType.values()) {
            if (entityChangeType != EntityChangeType.CREATE) {

                for (ElementType elementType : ElementType.values()) {
                    if (elementType != ElementType.Changeset) {
                        NodeList elementIdXmlNodes = null;
                        try {
                            elementIdXmlNodes = XPathAPI.selectNodeList(changesetDoc,
                                    "//osmChange/" + entityChangeType.toString().toLowerCase() + "/"
                                            + elementType.toString().toLowerCase() + "/@id");
                        }
                        catch (TransformerException e) {
                            throw new RuntimeException("Error calling XPathAPI!", e);
                        }

                        for (int i = 0; i < elementIdXmlNodes.getLength(); i++) {
                            long id;
                            try {
                                id = Long.parseLong(elementIdXmlNodes.item(i).getNodeValue());
                            }
                            catch (NumberFormatException | NullPointerException e) {
                                throw new IllegalArgumentException(emptyIdErrorMsg, e);
                            }
                            if (id > 0) {
                                elementTypesToElementIds.get(elementType).add(id);
                            }
                        }
                    }
                }
            }
        }

        for (ElementType elementType : ElementType.values()) {
            if (elementType != ElementType.Changeset) {
                if (!Element.allElementsExist(mapId, elementType, elementTypesToElementIds.get(elementType))) {
                    // TODO: list the id's and types of the elements that don't exist
                    throw new IllegalStateException("Element(s) being referenced don't exist.");
                }
            }
        }

        if (!elementTypesToElementIds.get(ElementType.Node).isEmpty()) {
            return createQuery(mapId)
                    .from(currentNodes)
                    .where(currentNodes.id.in(elementTypesToElementIds.get(ElementType.Node)))
                    .transform(groupBy(currentNodes.id).as(currentNodes));
        }

        return null;
    }
}
