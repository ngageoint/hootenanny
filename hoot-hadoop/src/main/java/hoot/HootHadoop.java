package hoot;

public class HootHadoop
{
  private static boolean _initialized = false;
  
  public static void init()
  {
    if (_initialized == false)
    {
      System.err.println("LD_LIBRARY_PATH: " + System.getenv("LD_LIBRARY_PATH"));
      System.err.println("java.library.path: " + System.getProperty("java.library.path"));
      try
      {
        System.out.println("Loading HootHadoop");
        System.loadLibrary("HootHadoop");
        System.out.println("Loaded HootHadoop");
        _initialized = true;
      }
      catch (UnsatisfiedLinkError e)
      {
        System.err
          .println("Error linking with HootHadoop library. Is libHootHadoop.so in your LD_LIBRARY_PATH?");
        System.err.println("LD_LIBRARY_PATH: " + System.getenv("LD_LIBRARY_PATH"));
        System.err.println("java.library.path: " + System.getProperty("java.library.path"));
        throw e;
      }
    }
  }
}
