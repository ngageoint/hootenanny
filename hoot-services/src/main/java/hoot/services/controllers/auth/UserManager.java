package hoot.services.controllers.auth;

import java.io.IOException;

import javax.xml.parsers.ParserConfigurationException;

import org.springframework.stereotype.Service;
import org.xml.sax.SAXException;

import hoot.services.models.db.Users;

@Service
public interface UserManager {
    void upsert(String xml) throws SAXException, IOException, ParserConfigurationException, InvalidUserProfileException;
    Users parseUser(String xml) throws SAXException, IOException, ParserConfigurationException, InvalidUserProfileException;
}
