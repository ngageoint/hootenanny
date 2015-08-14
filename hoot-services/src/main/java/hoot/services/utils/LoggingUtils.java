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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
//package hoot.services.utils;
//
//import java.lang.reflect.InvocationTargetException;
//import java.lang.reflect.Method;
//
//import org.slf4j.Logger;
//import org.slf4j.LoggerFactory;
//
///**
// * General logging utilities
// */
//public class LoggingUtils
//{
//  public static final String ALL = "all";
//  public static final String FINE = "fine";
//  public static final String TRACE = "trace";
//  public static final String DEBUG = "debug";
//  public static final String INFO = "info";
//  public static final String WARN = "warn";
//  public static final String ERROR = "error";
//  public static final String OFF = "off";
//  
//  private static final Logger log = LoggerFactory.getLogger(LoggingUtils.class);
//  
//  public static void setLogLevel(final String className, final String level)
//  {
//    String loggerClassName = log.getClass().getSimpleName();
//    
//    if (loggerClassName.equalsIgnoreCase("SimpleLogger") ||
//         loggerClassName.equalsIgnoreCase("Slf4jLogger"))
//    {
//      // TODO:  If we update to slf4j 1.7.1+ the properties prefix is in a string constant
//      System.setProperty("org.slf4j.simpleLogger.log." + className, level);
//      log.info("Setting log level to: " + level);
//    }
//    else if (loggerClassName.equalsIgnoreCase("Log4JLoggerAdapter"))
//    {
//      try
//      {
//        Class<?> log4jLoggerFactory = Class.forName("org.apache.log4j.Logger");
//        Class<?> levelClass = Class.forName("org.apache.log4j.Level");
//        
//        Method getLogger = log4jLoggerFactory.getMethod("getLogger", String.class);
//        Object logger = getLogger.invoke(null, className);
//                
//        Method setLevel = logger.getClass().getMethod("setLevel",  levelClass);
//        
//        Object levelEnum = getLog4JLevel(level);
//        
//        setLevel.invoke(logger, levelEnum);
//        
//        log.debug("Changed log level for: {} to: {}", className, level);
//      }
//      catch (SecurityException e)
//      {
//        e.printStackTrace();
//      }
//      catch (NoSuchMethodException e)
//      {
//        e.printStackTrace();
//      }
//      catch (ClassNotFoundException e)
//      {
//        e.printStackTrace();
//      }
//      catch (IllegalArgumentException e)
//      {
//        e.printStackTrace();
//      }
//      catch (IllegalAccessException e)
//      {
//        e.printStackTrace();
//      }
//      catch (InvocationTargetException e)
//      {
//        e.printStackTrace();
//      }
//    }
//    else
//    {
//      throw new UnsupportedOperationException("Only the Log4J & slf4j SimpleLogger is supported for logging, found: " + loggerClassName);
//    }
//  }
//  
//  public static void setLogLevel(final Class<?> clazz, final String level)
//  {
//    setLogLevel(clazz.getName(), level);
//  }
//  
//  public static void setDefaultLogLevel(final String level)
//  {
//    String loggerClassName = log.getClass().getSimpleName();
//    
//    if (loggerClassName.equalsIgnoreCase("SimpleLogger"))
//    {
//      // TODO:  If we update to slf4j 1.7.1+ the properties are in a string constant
//      System.setProperty("org.slf4j.simpleLogger.defaultLogLevel", level);
//      log.info("Setting default log level to: " + level);
//    }
//    else if (loggerClassName.equalsIgnoreCase("Log4JLoggerAdapter"))
//    {
//      try
//      {
//        Class<?> log4jLoggerFactory = Class.forName("org.apache.log4j.Logger");
//        Class<?> levelClass = Class.forName("org.apache.log4j.Level");
//
//        Method getLogger = log4jLoggerFactory.getMethod("getRootLogger", null);
//        Object logger = getLogger.invoke(null);
//        Method setLevel = logger.getClass().getMethod("setLevel",  levelClass);
//        
//        Object levelEnum = getLog4JLevel(level);
//        
//        setLevel.invoke(logger, levelEnum);
//        
//        log.debug("Changed default log level to: {}", level);
//
//      }
//      catch (ClassNotFoundException e)
//      {
//        e.printStackTrace();
//      }
//      catch (SecurityException e)
//      {
//        e.printStackTrace();
//      }
//      catch (NoSuchMethodException e)
//      {
//        e.printStackTrace();
//      }
//      catch (IllegalArgumentException e)
//      {
//        e.printStackTrace();
//      }
//      catch (IllegalAccessException e)
//      {
//        e.printStackTrace();
//      }
//      catch (InvocationTargetException e)
//      {
//        e.printStackTrace();
//      }
//    }
//    else
//    {
//      throw new UnsupportedOperationException("Only the Log4J & slf4j SimpleLogger is supported for logging.  Additional loggers can easily be added");
//    }
//  }
//  
//  public static void setLoggerToStdOut()
//  {
//    setLoggerToFile("System.out");
//  }
//  public static void setLoggerToStdErr()
//  {
//    setLoggerToFile("System.err");
//  }
//  
//  public static void setLoggerToFile(final String file)
//  {
//    String loggerClass = log.getClass().getSimpleName();
//    
//    if (loggerClass.equalsIgnoreCase("SimpleLogger"))
//    {
//      // TODO:  If we update to slf4j 1.7.1+ the properties are in a string constant
//      System.setProperty("org.slf4j.simpleLogger.logFile", file);
//      log.info("Setting logging output to: " + file);
//    }
////    else if (loggerClass.equalsIgnoreCase("Log4JLoggerAdapter"))
////    {
////      
////    }
//    else
//    {
//      throw new UnsupportedOperationException("Only the slf4j SimpleLogger is supported for logging.  Additional loggers can easily be added");
//    }
//  }
//
//  public static void setLoggerFormat()
//  {
//    String loggerClass = log.getClass().getSimpleName();
//    System.out.println(log.getClass().getName());
//    if (loggerClass.equalsIgnoreCase("SimpleLogger"))
//    {
//      // TODO:  If we update to slf4j 1.7.1+ the properties are in a string constant
//      System.setProperty("org.slf4j.simpleLogger.showDateTime", "true");
//      System.setProperty("org.slf4j.simpleLogger.dateTimeFormat", "HH:mm:ss.SSS");
//      System.setProperty("org.slf4j.simpleLogger.showThreadName", "true");
//      System.setProperty("org.slf4j.simpleLogger.showLogName", "true");
//      System.setProperty("org.slf4j.simpleLogger.showShortLogName", "false");
//      System.setProperty("org.slf4j.simpleLogger.levelInBrackets", "true");
//      log.info("Setting up default output format");
//
//    }
////    else if (loggerClass.equalsIgnoreCase("Log4JLoggerAdapter"))
////    {
////      
////    }
//    else
//    {
//      throw new UnsupportedOperationException("Only the slf4j SimpleLogger is supported for logging.  Additional loggers can easily be added");
//    }
//  }
//  
//  private static Object getLog4JLevel(final String level)
//  {
//    try
//    {
//      Class<?> levelClass = Class.forName("org.apache.log4j.Level");
//      Method toLevel = levelClass.getMethod("toLevel",  String.class);
//      Object levelEnum = toLevel.invoke(null, level);
//
//      return levelEnum;
//
//    }
//    catch (ClassNotFoundException e)
//    {
//      e.printStackTrace();
//    }
//    catch (SecurityException e)
//    {
//      e.printStackTrace();
//    }
//    catch (NoSuchMethodException e)
//    {
//      e.printStackTrace();
//    }
//    catch (IllegalArgumentException e)
//    {
//      e.printStackTrace();
//    }
//    catch (IllegalAccessException e)
//    {
//      e.printStackTrace();
//    }
//    catch (InvocationTargetException e)
//    {
//      e.printStackTrace();
//    }
//               
//    return null;
//  }
//}
