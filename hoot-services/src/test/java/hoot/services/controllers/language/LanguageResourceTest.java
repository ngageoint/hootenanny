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
 * @copyright Copyright (C) 2018, 2021 Maxar (http://www.maxar.com/)
 */

package hoot.services.controllers.language;

import static org.mockito.Mockito.*;

import org.powermock.api.mockito.PowerMockito;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.modules.junit4.PowerMockRunner;
import org.powermock.core.classloader.annotations.PowerMockIgnore;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.junit.runner.RunWith;
import org.junit.Before;

import javax.ws.rs.NotFoundException;
import javax.ws.rs.client.Entity;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response.Status;
import javax.ws.rs.core.Response;

import java.net.URLEncoder;
import java.net.URLDecoder;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import org.json.simple.JSONObject;
import org.json.simple.JSONArray;
import org.json.simple.parser.JSONParser;

import hoot.services.UnitTest;
import hoot.services.language.joshua.JoshuaLanguageTranslator;
import hoot.services.language.tika.TikaLanguageDetector;
import hoot.services.controllers.language.LanguageTranslateRequest;
import hoot.services.language.ToEnglishTranslatorFactory;
import hoot.services.language.LanguageDetectorFactory;
import hoot.services.language.LanguageDetectionApp;
import hoot.services.language.LanguageTranslationApp;
import hoot.services.controllers.language.SupportedLanguagesRequest;
import hoot.services.controllers.language.SupportedLanguagesResponse;
import hoot.services.language.SupportedLanguage;
import hoot.services.controllers.language.LanguageDetectRequest;
import hoot.services.controllers.language.LanguageTranslateRequest;
import hoot.services.language.LanguageTestUtils;
import hoot.services.jerseyframework.HootServicesJerseyTestAbstract;
import hoot.services.language.LanguageAppInfo;
import hoot.services.language.SupportedLanguageConsumer;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

@RunWith(PowerMockRunner.class)
@PrepareForTest(
  { JoshuaLanguageTranslator.class, TikaLanguageDetector.class, 
    ToEnglishTranslatorFactory.class, LanguageDetectorFactory.class })
@PowerMockIgnore("javax.management.*")
public class LanguageResourceTest extends HootServicesJerseyTestAbstract
{
  private static final Logger logger = LoggerFactory.getLogger(LanguageResourceTest.class);

  @Test
  @Category(UnitTest.class)
  public void testGetDetectors() throws Exception 
  {
    //This has to be mocked separately within this test since there we're testing multiple types of 
    //LanguageAppInfo implementations.
    TikaLanguageDetector detector = (TikaLanguageDetector)LanguageTestUtils.mockTika();
    LanguageAppInfo appInfo = (LanguageAppInfo)detector;
    PowerMockito.when(appInfo.getDescription()).thenReturn("blah1");
    PowerMockito.when(appInfo.getUrl()).thenReturn("http://localhost/TikaLanguageDetector");

    LanguageDetectorsResponse response =
      target("toEnglishTranslation/detectors")
        .request(MediaType.APPLICATION_JSON)
        .get(LanguageDetectorsResponse.class);
    LanguageDetectionApp[] apps = response.getApps();

    //Since LanguageDetectorFactory.getSimpleClassNames is mocked, this doesn't need 
    //to be updated if new apps are added.
    Assert.assertEquals(1, apps.length);
    Assert.assertEquals("TikaLanguageDetector", apps[0].getName());
    Assert.assertEquals("blah1", apps[0].getDescription());
    Assert.assertEquals("http://localhost/TikaLanguageDetector", apps[0].getUrl());
    Assert.assertEquals(true, apps[0].getSupportsConfidence());
  }

  @Test
  @Category(UnitTest.class)
  public void testGetTranslators() throws Exception 
  {
    //This has to be mocked separately within this test since there we're testing multiple types of 
    //LanguageAppInfo implementations.
    JoshuaLanguageTranslator translator = (JoshuaLanguageTranslator)LanguageTestUtils.mockJoshua();
    LanguageAppInfo appInfo = (LanguageAppInfo)translator;
    PowerMockito.when(appInfo.getDescription()).thenReturn("blah2");
    PowerMockito.when(appInfo.getUrl()).thenReturn("http://localhost/JoshuaLanguageTranslator");

    LanguageTranslatorsResponse response =
      target("toEnglishTranslation/translators")
        .request(MediaType.APPLICATION_JSON)
        .get(LanguageTranslatorsResponse.class);
    LanguageTranslationApp[] apps = response.getApps();

    //Since ToEnglishTranslatorFactory.getSimpleClassNames is mocked, this doesn't need 
    //to be updated if new apps are added.
    Assert.assertEquals(1, apps.length);
    Assert.assertEquals("JoshuaLanguageTranslator", apps[0].getName());
    Assert.assertEquals("blah2", apps[0].getDescription());
    Assert.assertEquals("http://localhost/JoshuaLanguageTranslator", apps[0].getUrl());
  }

  @Test
  @Category(UnitTest.class)
  public void testGetDetectableLangs() throws Exception 
  {    
    //This has to be mocked separately within this test since there we're testing multiple types of 
    //SupportedLanguageConsumer implementations.
    SupportedLanguageConsumer langConsumer = (SupportedLanguageConsumer)LanguageTestUtils.mockTika();
    PowerMockito.when(langConsumer.getSupportedLanguages()).thenReturn(LanguageTestUtils.getSupportedLanguages());
    PowerMockito.when(langConsumer.isLanguageAvailable("de")).thenReturn(true);
    PowerMockito.when(langConsumer.isLanguageAvailable("es")).thenReturn(true);
 
    SupportedLanguagesRequest request = new SupportedLanguagesRequest();
    request.setApps(new String[] { "TikaLanguageDetector" });

    SupportedLanguagesResponse response =
      target("toEnglishTranslation/detectable")
        .request(MediaType.APPLICATION_JSON)
        .post(Entity.json(request), SupportedLanguagesResponse.class);
    
    SupportedLanguage[] langs = response.getLanguages();
    Assert.assertEquals(2, langs.length);
    for (int i = 0; i < langs.length; i++)
    {
      SupportedLanguage lang = langs[i];
      Assert.assertFalse(lang.getIso6391Code().isEmpty());
      Assert.assertFalse(lang.getName().isEmpty());
      Assert.assertTrue(lang.getAvailable());
    }
  }

  @Test
  @Category(UnitTest.class)
  public void testGetTranslatableLangs() throws Exception 
  {
    //This has to be mocked separately within this test since there we're testing multiple types of 
    //SupportedLanguageConsumer implementations.
    SupportedLanguageConsumer langConsumer = (SupportedLanguageConsumer)LanguageTestUtils.mockJoshua();
    PowerMockito.when(langConsumer.getSupportedLanguages()).thenReturn(LanguageTestUtils.getSupportedLanguages());
    PowerMockito.when(langConsumer.isLanguageAvailable("de")).thenReturn(true);
    PowerMockito.when(langConsumer.isLanguageAvailable("es")).thenReturn(true);

    SupportedLanguagesRequest request = new SupportedLanguagesRequest();
    request.setApps(new String[] { "JoshuaLanguageTranslator" });

    SupportedLanguagesResponse response =
      target("toEnglishTranslation/translatable")
        .request(MediaType.APPLICATION_JSON)
        .post(Entity.json(request), SupportedLanguagesResponse.class);
    
    SupportedLanguage[] langs = response.getLanguages();
    Assert.assertEquals(2, langs.length);
    for (int i = 0; i < langs.length; i++)
    {
      SupportedLanguage lang = langs[i];
      String iso6391 = lang.getIso6391Code();
      Assert.assertFalse(iso6391.isEmpty());
      Assert.assertFalse(lang.getName().isEmpty());
      if (iso6391.equals("de") || iso6391.equals("es"))
      {
        Assert.assertTrue(lang.getAvailable());
      }
      else
      {
        Assert.assertFalse(lang.getAvailable());
      }
    }
  }

  @Test
  @Category(UnitTest.class)
  public void testDetectLanguage() throws Exception 
  {
    //This has to be mocked separately within this test since there we're testing multiple types of 
    //SupportedLanguageConsumer implementations.
    SupportedLanguageConsumer langConsumer = (SupportedLanguageConsumer)LanguageTestUtils.mockTika();
    PowerMockito.when(langConsumer.getLanguageName("es")).thenReturn("Spanish");

    LanguageDetectRequest request = new LanguageDetectRequest();
    request.setDetectors(new String[] { "TikaLanguageDetector" });
    request.setText(LanguageResource.encodeText("Buenos días"));

    String response =
      target("toEnglishTranslation/detect")
        .request(MediaType.APPLICATION_JSON)
        .post(Entity.json(request), String.class);
    JSONObject responseObj = (JSONObject)(new JSONParser()).parse(response);  
  
    Assert.assertEquals(
      "Buenos días", URLDecoder.decode(responseObj.get("sourceText").toString()));
    Assert.assertEquals("es", responseObj.get("detectedLangCode").toString());
    Assert.assertEquals(
      "Spanish", URLDecoder.decode(responseObj.get("detectedLang").toString()));
    Assert.assertEquals("TikaLanguageDetector", responseObj.get("detectorUsed").toString());
    Assert.assertEquals("high", responseObj.get("detectionConfidence").toString());
  }

  @Test
  @Category(UnitTest.class)
  public void testTranslate() throws Exception 
  {
    LanguageTestUtils.mockJoshua();

    LanguageTranslateRequest request = new LanguageTranslateRequest();
    request.setDetectors(new String[] { "TikaLanguageDetector" });
    request.setText(LanguageResource.encodeText("Buenos días"));
    request.setTranslator("JoshuaLanguageTranslator");
    String[] sourceLangCodes = new String[] { "es" };
    request.setSourceLangCodes(sourceLangCodes);
    request.setDetectedLanguageOverridesSpecifiedSourceLanguages(false);
    request.setPerformExhaustiveTranslationSearchWithNoDetection(false);

    String response =
      target("toEnglishTranslation/translate")
        .request(MediaType.APPLICATION_JSON)
        .post(Entity.json(request), String.class);
    JSONObject responseObj = (JSONObject)(new JSONParser()).parse(response);

    Assert.assertEquals(
      "Buenos días", URLDecoder.decode(responseObj.get("sourceText").toString()));
    Assert.assertEquals(
      "Good morning", URLDecoder.decode(responseObj.get("translatedText").toString()));
    JSONArray responseSourceLangCodes = (JSONArray)responseObj.get("sourceLangCodes");
    Assert.assertEquals(sourceLangCodes.length, responseSourceLangCodes.size());
    Assert.assertEquals(sourceLangCodes[0], responseSourceLangCodes.get(0));
    Assert.assertEquals("JoshuaLanguageTranslator", responseObj.get("translator").toString());
    Assert.assertEquals(
      false, 
      (Boolean)responseObj.get("detectedLanguageOverridesSpecifiedSourceLanguages"));
    Assert.assertEquals(
      false, 
      (Boolean)responseObj.get("performExhaustiveTranslationSearchWithNoDetection"));
  }

  @Test
  @Category(UnitTest.class)
  public void testGetTranslateBatch() throws Exception 
  {
    LanguageTestUtils.mockJoshua();

    LanguageTranslateRequest request = new LanguageTranslateRequest();
    request.setDetectors(new String[] { "TikaLanguageDetector" });
    request.setText(LanguageResource.encodeText("Buenos días\nBuenos noches"));
    request.setTranslator("JoshuaLanguageTranslator");
    String[] sourceLangCodes = new String[] { "es" };
    request.setSourceLangCodes(sourceLangCodes);
    request.setDetectedLanguageOverridesSpecifiedSourceLanguages(false);
    request.setPerformExhaustiveTranslationSearchWithNoDetection(false);

    String response =
      target("toEnglishTranslation/translateBatch")
        .request(MediaType.APPLICATION_JSON)
        .post(Entity.json(request), String.class);
    JSONObject responseObj = (JSONObject)(new JSONParser()).parse(response);

    Assert.assertEquals(
      "Buenos días\nBuenos noches", URLDecoder.decode(responseObj.get("sourceText").toString()));
    Assert.assertEquals(
      "Good morning\nGood night", URLDecoder.decode(responseObj.get("translatedText").toString()));
    JSONArray responseSourceLangCodes = (JSONArray)responseObj.get("sourceLangCodes");
    Assert.assertEquals(sourceLangCodes.length, responseSourceLangCodes.size());
    Assert.assertEquals(sourceLangCodes[0], responseSourceLangCodes.get(0));
    Assert.assertEquals("JoshuaLanguageTranslator", responseObj.get("translator").toString());
    Assert.assertEquals(
      false, 
      (Boolean)responseObj.get("detectedLanguageOverridesSpecifiedSourceLanguages"));
    Assert.assertEquals(
      false, 
      (Boolean)responseObj.get("performExhaustiveTranslationSearchWithNoDetection"));
  }
}
