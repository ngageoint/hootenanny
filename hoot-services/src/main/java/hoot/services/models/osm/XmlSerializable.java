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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.models.osm;

import java.util.Map;

import hoot.services.models.osm.Element.ElementType;


/**
 * All OSM element models that want to be serializable to/from XML should
 * implement this
 */
public interface XmlSerializable {
    void fromXml(final org.w3c.dom.Node xml) throws Exception;

    org.w3c.dom.Element toXml(final org.w3c.dom.Element parentXml, final long modifyingUserId,
            final String modifyingUserDisplayName, final boolean multiLayerUniqueElementIds, final boolean addChildren)
            throws Exception;

    org.w3c.dom.Element toChangesetResponseXml(final org.w3c.dom.Element parentXml) throws Exception;

    void setElementCache(Map<ElementType, Map<Long, Element>> parsedElementIdsToElementsByType);

    long getId() throws Exception;

    void setId(long id) throws Exception;

    long getOldId();

    void setOldId(long id);

    void setRequestChangesetId(long id);
}
