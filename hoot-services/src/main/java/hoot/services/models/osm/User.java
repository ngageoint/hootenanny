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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
package hoot.services.models.osm;

import java.util.Date;

import org.w3c.dom.Document;
import org.w3c.dom.Element;

import hoot.services.controllers.auth.UserManagerImpl;
import hoot.services.models.db.Users;

/**
 * Represents the model for an OSM user
 */
public class User extends Users {

    public User(Users user) {
        setDisplayName(user.getDisplayName());
        setEmail(user.getEmail());
        setId(user.getId());

        setProviderAccessToken(user.getProviderAccessToken());
        setProviderAccessKey(user.getProviderAccessKey());
        setProviderCreatedAt(user.getProviderCreatedAt());

        setHootservicesCreatedAt(user.getHootservicesCreatedAt());
        setHootservicesLastAuthorize(user.getHootservicesLastAuthorize());
    }

    /**
     * Returns an XML representation of the user's details
     *
     * @param parentXml XML node this node should be attached under
     * @return an XML node
     */
    private Element detailsToXml(Element parentXml, long changesetsCount) {
        Document doc = parentXml.getOwnerDocument();

        Element userElement = doc.createElement("user");
        userElement.setAttribute("id", String.valueOf(getId()));
        userElement.setAttribute("display_name", getDisplayName());
        userElement.setAttribute("account_created", UserManagerImpl.DATE_FORMAT.format(new Date(getProviderCreatedAt().getTime())));
        userElement.setAttribute("account_last_authorized", UserManagerImpl.DATE_FORMAT.format(new Date(getHootservicesLastAuthorize().getTime())));

        // img element not supported
        // roles element not supported
        Element changesetsElement = doc.createElement("changesets");
        changesetsElement.setAttribute("count", String.valueOf(changesetsCount));

        // traces element not supported
        // blocks element not supported
        // home element not supported
        // description element not supported
        // languages element not supported
        // messages element not supported

        userElement.appendChild(changesetsElement);

        return userElement;
    }

    /**
     * Returns an XML representation of the user
     *
     * @param parentXml XML node this node should be attached under
     * @return an XML node
     */
    public Element toXml(Element parentXml, long changesetsCount) {
        return detailsToXml(parentXml, changesetsCount);
    }
}
