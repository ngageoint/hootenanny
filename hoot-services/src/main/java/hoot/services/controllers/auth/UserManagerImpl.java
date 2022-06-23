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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.auth;

import static hoot.services.models.db.QSpringSession.springsessions;
import static hoot.services.models.db.QUsers.users;
import static hoot.services.utils.DbUtils.createQuery;

import java.io.IOException;
import java.sql.Timestamp;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import java.util.TimeZone;
import java.util.concurrent.ConcurrentHashMap;

import javax.xml.parsers.ParserConfigurationException;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import com.fasterxml.jackson.databind.JsonNode;

import hoot.services.models.db.SpringSession;
import hoot.services.models.db.Users;
import hoot.services.utils.XmlDocumentBuilder;

@Component
@Transactional(propagation = Propagation.REQUIRES_NEW) // Run inside of a new transaction.  This is intentional.
public class UserManagerImpl implements UserManager {

    private static final Logger logger = LoggerFactory.getLogger(UserManagerImpl.class);

    public static final SimpleDateFormat DATE_FORMAT = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss'Z'");

    public static final ConcurrentHashMap<String, Users> userCache = new ConcurrentHashMap<String, Users>();

    @Override
    public Users getUserBySession(String sessionId) {
        if (userCache.containsKey(sessionId)) {
            return userCache.get(sessionId);
        } else {
            SpringSession sess = createQuery()
                    .select(springsessions)
                    .from(springsessions)
                    .where(springsessions.session_id.eq(sessionId))
                    .where(springsessions.user_id.isNotNull())
                    .fetchOne();

            if(sess != null) {
                Users user = createQuery()
                        .select(users)
                        .from(users)
                        .where(users.id.eq(sess.getUser_id()))
                        .fetchOne();
                userCache.put(sessionId, user);
                return user;
            }
        }

        return null;
    }

    private void update(Users user) {
        createQuery().update(users).populate(user).where(users.id.eq(user.getId())).execute();
    }

    private void insert(Users user) {
        createQuery().insert(users).populate(user).execute();
    }

    public Users getUser(Long id) {
        return createQuery().select(users).from(users).where(users.id.eq(id)).fetchFirst();
    }

    @Override
    public Timestamp parseTimestamp(String timestamp) {

        DATE_FORMAT.setTimeZone(TimeZone.getTimeZone("UTC"));
        Date dt;
        try {
            dt = DATE_FORMAT.parse(timestamp);
            return new Timestamp(dt.getTime());
        } catch (ParseException e) {
            return null;
        }
    }

    @Override
    public Users parseUser(JsonNode userDetailsJson) throws InvalidUserProfileException {
        Users user = new Users();
        JsonNode details = userDetailsJson.get("user");

        user.setDisplayName(details.get("display_name").asText());
        user.setId(Long.parseLong(details.get("id").asText()));
        user.setProviderCreatedAt(parseTimestamp(details.get("account_created").asText()));
        user.setHootservicesLastAuthorize(new Timestamp(System.currentTimeMillis()));

        return user;
    }

    @Override
    public Users parseUser(String xml) throws SAXException, IOException, ParserConfigurationException, InvalidUserProfileException {
        Users user = new Users();
        Document doc = XmlDocumentBuilder.parse(xml);

        // Get User node:
        NodeList l = doc.getElementsByTagName("user");
        if (l.getLength() != 1) {
            throw new InvalidUserProfileException(xml);
        }
        Node userNode = l.item(0);

        // Store attributes from User node:
        Element userElement = (Element) userNode;
        user.setDisplayName(userElement.getAttribute("display_name"));
        user.setId(Long.parseLong(userElement.getAttribute("id")));
        user.setProviderCreatedAt(parseTimestamp(userElement.getAttribute("account_created")));
        user.setHootservicesLastAuthorize(new Timestamp(System.currentTimeMillis()));
        if(user.getId() <= 0L) {
            logger.warn("empty -or- invalid user profiles can result from misconfigured oauth, check hoot-services.conf::oauthProviderURL");
            throw new InvalidUserProfileException(xml);
        }
        return user;
    }

    private void attributeSessionWithUser(String sessionId, Users u) {
        long affectedRows = createQuery()
        .update(springsessions)
        .set(springsessions.user_id, u.getId())
        .where(springsessions.session_id.eq(sessionId))
        .execute();

        if (affectedRows == 0) {
            logger.warn("attributeSessionWithUser(): failed to attribute spring session with user.");
        }
    }

    private List<String> getUserSessionId(Long userId) {
        List<String> sess = createQuery()
                .select(springsessions.session_id)
                .from(springsessions)
                .where(springsessions.user_id.eq(userId))
                .fetch();
        return sess;
    }


	@Override
	public Users upsert(JsonNode userDetailsJson, String accessToken, String sessionId) throws InvalidUserProfileException {
		Users user = parseUser(userDetailsJson);
		user.setProviderAccessKey(accessToken);
        user.setEmail(String.format("%d@hootenanny", user.getId()));

        Users existingUser = this.getUser(user.getId());
        if (existingUser == null) {
            this.insert(user);
        } else {
            // look in database for possible existing privileges set
            user.setPrivileges(existingUser.getPrivileges());

            // look in database for existing user favorite adv. opts
            user.setFavoriteOpts(existingUser.getFavoriteOpts());

            this.update(user);
        }

        attributeSessionWithUser(sessionId, user);
        userCache.put(sessionId, user);
        return user;
	}


    @Override
    public void clearCachedUser(Long userId) {
        getUserSessionId(userId).forEach(id -> userCache.remove(id));
    }

}
