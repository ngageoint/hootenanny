package hoot;

public class runme
{
  static
  {
    HootHadoop.init();
  }

  public static void main(String argv[])
  {
    PbfInputSplit is = new PbfInputSplit();
    System.out.println(is.getLength());
  }
}
