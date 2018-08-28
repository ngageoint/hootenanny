package hoot.services.controllers.auth;

import java.io.IOException;
import java.sql.Timestamp;

import javax.xml.parsers.ParserConfigurationException;

import org.springframework.security.oauth.consumer.OAuthConsumerToken;
import org.springframework.stereotype.Service;
import org.xml.sax.SAXException;

import hoot.services.models.db.Users;

@Service
public interface UserManager {
    void upsert(String xml, OAuthConsumerToken accessToken, String sessionId) throws SAXException, IOException, ParserConfigurationException, InvalidUserProfileException;
    Users parseUser(String xml) throws SAXException, IOException, ParserConfigurationException, InvalidUserProfileException;

    Timestamp parseTimestamp(String timestamp);

    Users getUserBySession(String sessionId);
}
