package hoot.services.controllers.auth;

import static hoot.services.models.db.QUsers.users;
import static hoot.services.utils.DbUtils.createQuery;

import java.io.IOException;
import java.sql.Timestamp;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.TimeZone;
import java.util.concurrent.ConcurrentHashMap;

import javax.xml.parsers.ParserConfigurationException;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.security.oauth.consumer.OAuthConsumerToken;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

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
            return createQuery().select(users).from(users).where(users.session_id.eq(sessionId)).fetchFirst();
        }
    }
    public void update(Users user) {
        createQuery().update(users).populate(user).where(users.id.eq(user.getId())).execute();
        if (user.getSessionId() != null) {
            userCache.put(user.getSessionId(), user);
        }
    }
    private void insert(Users user) {
        createQuery().insert(users).populate(user).execute();
        if (user.getSessionId() != null) {
            userCache.put(user.getSessionId(), user);
        }
    }
    public Users getUser(String id) {
        return getUser(Long.parseLong(id));
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
        return user;
    }
    @Override
    public void upsert(String xml, OAuthConsumerToken accessToken, String sessionId) throws SAXException, IOException, ParserConfigurationException, InvalidUserProfileException {
        Users user = this.parseUser(xml);
        user.setProviderAccessToken(accessToken);
        user.setSessionId(sessionId);
        user.setEmail(String.format("%d@hootenanny.com", user.getId()));
        if (this.getUser(user.getId()) == null) {
            this.insert(user);
        } else {
            this.update(user);
        }
    }
}
