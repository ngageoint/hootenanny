
// Hadoop
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <hadoop/Pipes.hh>
#include "hadoop/TemplateFactory.hh"
#include "hadoop/StringUtils.hh"
#pragma GCC diagnostic warning "-Wunused-parameter"

// Qt
#include <QString>

// Standard
#include <iostream>

class WordCountMap: public HadoopPipes::Mapper {
public:
  WordCountMap(HadoopPipes::TaskContext& context){}
  void map(HadoopPipes::MapContext& context) {
//    std::vector<std::string> words =
//      HadoopUtils::splitString(context.getInputValue(), " ");
//    for(unsigned int i=0; i < words.size(); ++i) {
//      context.emit(words[i], "1");
//    }
    QString s = QString::fromStdString(context.getInputValue());
    s = s.replace("\n", " ");
    context.emit("-", s.toStdString());
  }
};

class WordCountReduce: public HadoopPipes::Reducer {
public:
  WordCountReduce(HadoopPipes::TaskContext& context){}
  void reduce(HadoopPipes::ReduceContext& context) {
//    int sum = 0;
//    while (context.nextValue()) {
//      sum += HadoopUtils::toInt(context.getInputValue());
//    }
    while (context.nextValue())
    {
      context.emit(context.getInputKey(), context.getInputValue());
    }
  }
};

int main(int argc, char *argv[])
{
  //std::cout << "hello world." << std::endl;
  return HadoopPipes::runTask(HadoopPipes::TemplateFactory<WordCountMap, WordCountReduce>());
}
