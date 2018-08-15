
package hoot.services.controllers.language;

import java.io.IOException;
import java.util.List;
import java.util.Arrays;

import org.xml.sax.SAXException;

import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;
import javax.ws.rs.Consumes;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import org.springframework.stereotype.Controller;

import hoot.services.language.LanguageDetectorFactory;
import hoot.services.language.ToEnglishTranslatorFactory;
import hoot.services.language.ToEnglishTranslator;
import hoot.services.language.SupportedLanguages;
import hoot.services.language.LanguageDetector;
import hoot.services.language.LanguageDetectionConsumer;

import org.apache.commons.lang3.reflect.MethodUtils;

/*
 * 
 */
@Controller
@Path("")
public class LanguageResource 
{
  private static final Logger logger = LoggerFactory.getLogger(LanguageResource.class);

  public LanguageResource() throws Exception
  {
    //The Joshua init can take a long time, so let's do it here vs having it happen the first time a translation is made (I think).
    MethodUtils.invokeStaticMethod(Class.forName("hoot.services.language.JoshuaLanguageTranslator"), "getInstance", null);
  }

  @GET
  @Path("/supportedLanguages")
  @Produces(MediaType.APPLICATION_JSON)
  public SupportedLanguagesResponse getSupportedLangs()
  {
    try
    {
      return new SupportedLanguagesResponse(SupportedLanguages.getInstance().getSupportedLanguages());
    }
    catch (Exception e)
    {
      throw new WebApplicationException(
        e, 
        Response.status(Status.INTERNAL_SERVER_ERROR).entity("Error retrieving supported language information.  Error: " + e.getMessage())
          .build());
    }
  }

  private String toDetectResponse(LanguageDetectRequest request, String detectedLang, String detectingDetector)
  {
    JSONObject entity = new JSONObject();
    entity.put("sourceText", request.getText());
    entity.put("detectedLanguage", detectedLang);
    entity.put("detectorUsed", detectingDetector);
    return entity.toJSONString();
  }

  @POST
  @Path("/detect")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public Response detectLanguage(LanguageDetectRequest request)
  {
    try
    {
      String detectedLang = "";
      String detectingDetector = "";
      List<String> detectors = Arrays.asList(request.getDetectors());
      if (detectors.isEmpty())
      {
        //TODO: add these with reflection
        detectors.add("TikaLanguageDetector");
        detectors.add("OpenNlpLanguageDetector");
      }

      for (String detectorName : detectors)
      {
        if (detectedLang.isEmpty())
        {
          LanguageDetector detector = LanguageDetectorFactory.create(detectorName);
          detectingDetector = detectorName;
          detectedLang = detector.detect(request.getText());
        }
      }

      return Response.ok(toDetectResponse(request, detectedLang, detectingDetector)).build();
    }
    catch (Exception e)
    {
      throw new WebApplicationException(
        e, 
        Response.status(Status.INTERNAL_SERVER_ERROR)
          .entity(
            "Error detecting language with detector(s): " + String.join(",", request.getDetectors()) + "; error: " + e.getMessage() + 
            " text: " + request.getText())
          .build());
    }
  }

  private String toTranslateResponse(LanguageTranslateRequest request, String translatedText, ToEnglishTranslator translator)
  {
    JSONObject entity = new JSONObject();
    entity.put("sourceText", request.getText());
    entity.put("sourceLangCodes", request.getSourceLangCodes());
    entity.put("translatedText", translatedText);
    entity.put("translator", request.getTranslator());
    if (translator instanceof LanguageDetectionConsumer)
    {
      LanguageDetectionConsumer detectionConsumer = (LanguageDetectionConsumer)translator;
      if (!detectionConsumer.getDetectedLanguage().isEmpty())
      {
        entity.put("detectedLanguage", detectionConsumer.getDetectedLanguage());
        entity.put("detectorUsed", detectionConsumer.getDetectorUsed());
      }
    }
    entity.put("detectedLanguageOverridesSpecifiedSourceLanguages", request.getDetectedLanguageOverridesSpecifiedSourceLanguages());
    entity.put("performExhaustiveTranslationSearchWithNoDetection", request.getPerformExhaustiveTranslationSearchWithNoDetection());
    return entity.toJSONString();
  }

  @POST
  @Path("/translate")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public Response translate(LanguageTranslateRequest request) 
  {
    String translatedText = "";
    ToEnglishTranslator translator = null;
    try
    {
      translator = ToEnglishTranslatorFactory.create(request.getTranslator());

      for (int i = 0; i < request.getSourceLangCodes().length; i++)
      {
        String sourceLangCode = request.getSourceLangCodes()[i].toLowerCase();
        if (!SupportedLanguages.getInstance().isSupportedLanguage(sourceLangCode))
        {
          throw new Exception("Requested unsupported translation language: " + sourceLangCode);
        }
        else if (!translator.isLanguageAvailable(sourceLangCode))
        {
          throw new Exception("Requested unavailable translation language: " + sourceLangCode);
        }
      } 

      translator.setConfig(request);
      translatedText = translator.translate(request.getSourceLangCodes(), request.getText());
    }
    catch (Exception e)
    {
      Status status;
      //TODO: handle more exceptions
      if (e.getMessage().startsWith("No language translator available") || 
          e.getMessage().startsWith("Requested unsupported translation language"))
      {
        status = Status.BAD_REQUEST;
      }
      else
      {
        status = Status.INTERNAL_SERVER_ERROR;
      } 
      throw new WebApplicationException(
        e, 
        Response.status(status)
          .entity(
            "Error translating with translator: " + request.getTranslator() + " to language (s): " + 
            String.join(",", request.getSourceLangCodes()) + ".  Error: " + 
            e.getMessage() + "; text: " + request.getText())
          .build());
    }
    
    return Response.ok(toTranslateResponse(request, translatedText, translator)).build();
  }
}
