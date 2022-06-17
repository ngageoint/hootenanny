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
 * @copyright Copyright (C) 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.auth;

import java.io.IOException;
import java.sql.Timestamp;

import javax.xml.parsers.ParserConfigurationException;

import org.springframework.security.oauth.consumer.OAuthConsumerToken;
import org.springframework.stereotype.Service;
import org.xml.sax.SAXException;

import com.fasterxml.jackson.databind.JsonNode;

import hoot.services.models.db.Users;

@Service
public interface UserManager {
    Users upsert(String xml, OAuthConsumerToken accessToken, String sessionId) throws SAXException, IOException, ParserConfigurationException, InvalidUserProfileException;
    Users upsert(JsonNode userDetailsJson, String accessToken, String sessionId) throws SAXException, IOException, ParserConfigurationException, InvalidUserProfileException;
    Users parseUser(String xml) throws SAXException, IOException, ParserConfigurationException, InvalidUserProfileException;
    Users parseUser(JsonNode userDetailsJson) throws InvalidUserProfileException;

    Timestamp parseTimestamp(String timestamp);

    Users getUserBySession(String sessionId);

    void clearCachedUser(Long userId);
}
