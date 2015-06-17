
HEADERS += \
    PluginFactory.h \
    \
    FeatureSelection/GeneratedData.h \
    

SOURCES += \
    PluginFactory.cpp \
    \
    FeatureExtraction/CalculatorGenomeTest.cpp \
    FeatureExtraction/CalculatorGenomeNodeTest.cpp \
    FeatureExtraction/GeneticAlgorithmTest.cpp \
    \
    RandomForest/RfDataFrameTest.cpp \
    RandomForest/InfoGainCalculatorTest.cpp \
    RandomForest/RandomForestTest.cpp \
    RandomForest/RandomTreeTest.cpp \
    \
    RStarTree/BoxTest.cpp \
    RStarTree/DistanceIteratorTest.cpp \
    RStarTree/HilbertCurveTest.cpp \
    RStarTree/HilbertRTreeTest.cpp \
    RStarTree/KnnIteratorNd.cpp \
    RStarTree/RStarTreeTest.cpp \
    RStarTree/RTreeNodeStoreTest.cpp \
    RStarTree/RTreeNodeTest.cpp \
    \
    Xml/XmlHelperTest.cpp \

win32 {
SOURCES += \
    FeatureReduction/PrincipalComponentsAnalysisTest.cpp \
    \
    FeatureSelection/CfsSubsetEvaluatorTest.cpp \
    FeatureSelection/ConsistencySubsetEvaluatorTest.cpp \
    FeatureSelection/DataFrameDiscretizerTest.cpp \
    FeatureSelection/FayyadMdlDiscretizerTest.cpp \
    FeatureSelection/GreedyStepwiseSearchTest.cpp \
    FeatureSelection/SymmetricUncertaintyCalculatorTest.cpp \
    \
    ProbablePath/ProbablePathCalculatorTest.cpp \
    \
    RStarTree/PageStoreTest.cpp \
    \
    SpinImage/NormalEstimatorTest.cpp \
    SpinImage/SpinImageGeneratorTest.cpp \
    SpinImage/SpinImageStackTest.cpp \
    SpinImage/SpinImageTest.cpp \

}
