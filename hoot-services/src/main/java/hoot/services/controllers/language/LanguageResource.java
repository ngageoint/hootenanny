
package hoot.services.controllers.language;

import java.io.IOException;
import java.util.List;
import java.util.Arrays;

import org.xml.sax.SAXException;

import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;
//import javax.inject.Singleton;
import javax.ws.rs.Consumes;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import org.springframework.stereotype.Controller;

import hoot.services.language.LanguageDetectorFactory;
import hoot.services.language.ToEnglishLanguageTranslatorFactory;

/*
 * 
 */
@Controller
@Path("")
//@Singleton
public class LanguageResource 
{
  private static final Logger logger = LoggerFactory.getLogger(LanguageResource.class);

  public LanguageResource()
  {
    //Joshua init takes a long time
    //JoshuaLanguageTranslator.getInstance();
  }

  @POST
  @Path("/detect")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public Response detectLanguage(LanguageDetectRequest request) throws IOException, Exception
  {
    logger.debug(String.join(",", request.getDetectors()));
    logger.debug(request.getText());

    String detectedLang = "";
    String detectingDetector = "";
    List<String> detectors = Arrays.asList(request.getDetectors());
    if (detectors.isEmpty())
    {
      //TODO: add these with reflection
      detectors.add("TikaLanguageDetector");
      detectors.add("OpenNlpLanguageDetector");
    }

    for (String detector : detectors)
    {
      if (detectedLang.isEmpty())
      {
        detectedLang = LanguageDetectorFactory.create(detector).detect(request.getText());
      }
    }

    JSONObject entity = new JSONObject();
    entity.put("sourceText", request.getText());
    entity.put("detectedLang", detectedLang);
    entity.put("detectingDetector", detectingDetector);
    return Response.ok(entity.toJSONString()).build();
  }

  @POST
  @Path("/translate")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public Response translate(LanguageTranslateRequest request) 
  {
    logger.debug(request.getTranslator());
    logger.debug(request.getSourceLangCode());
    logger.debug(request.getText());

    String translatedText = "";
    try
    {
      translatedText = 
        ToEnglishLanguageTranslatorFactory.create(request.getTranslator()).translate(request.getSourceLangCode(), request.getText());
    }
    catch (Exception e)
    {
      throw new WebApplicationException(
      e, 
      Response.status(Status.INTERNAL_SERVER_ERROR)
        .entity(
          "Error translating with translator: " + request.getTranslator() + " to language: " + request.getSourceLangCode() + "; text: " + 
          request.getText())
        .build());
    }
    
    JSONObject entity = new JSONObject();
    entity.put("sourceText", request.getText());
    entity.put("translatedText", translatedText);
    entity.put("translator", request.getTranslator());
    return Response.ok(entity.toJSONString()).build();
  }

  /*@POST
  @Path("/translateWithDetect")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public Response translate(LanguageTranslateWithDetectRequest request) 
  {
    
  }*/
}
