QT += xml

TEMPLATE = lib
win32:CONFIG += dll

TARGET=Tgs

# Include the shared settings.
include(../Configure.pri)
CONFIG -= flat

CONFIG += rtti \
    qtestlib \
    debug

DESTDIR = ../lib/

# allow the <algorithm> std::min
win32:QMAKE_CXXFLAGS += /EHsc
unix:QMAKE_CXXFLAGS += -Wno-deprecated
#unix:QT -= core
unix:QT -= gui
UI_DIR = tmp/ui
MOC_DIR = tmp/moc
release:OBJECTS_DIR = tmp/obj/release
else:OBJECTS_DIR = tmp/obj/debug
DEPENDPATH += ./src/main/cpp/
INCLUDEPATH += $${DEPENDPATH}
win32:INCLUDEPATH += $${PWD}/../../
win32 { 
    MAKEFILE = Tgs$${BRANCH_APPEND}
    debug:TARGET = $${MAKEFILE}d
    release:TARGET = $${MAKEFILE}
    DEFINES += TGS_BUILD_DLL
    DEFINES *= _CRT_SECURE_NO_WARNINGS
    PRECOMPILED_HEADER = src/TgsStable.h
    DLLDESTDIR = $${TGS_DLL_OUTPUT_DIR}
}
MISC_FILES += Tgs.pro

newmat {
HEADERS += \
    src/main/cpp/tgs/FeatureReduction/Jacobi.h \
    src/main/cpp/tgs/FeatureReduction/PrincipalComponentsAnalysis.h \

SOURCES += \
    src/main/cpp/tgs/FeatureReduction/Jacobi.cpp \
    src/main/cpp/tgs/FeatureReduction/PrincipalComponentsAnalysis.cpp \

cppunit:SOURCES += \
    src/test/cpp/tgs/FeatureReduction/PrincipalComponentsAnalysisTest.cpp \

}

stxxl:SOURCES += \
    src/main/cpp/tgs/BigContainers/Stxxl.cpp

stxxl:HEADERS += \
    src/main/cpp/tgs/BigContainers/Stxxl.h \
    src/main/cpp/tgs/BigContainers/BigMapStxxl.h \


HEADERS += src/main/cpp/tgs/HashMap.h \
    src/main/cpp/tgs/Progress.h \
    src/main/cpp/tgs/SharedPtr.h \
    src/main/cpp/tgs/StreamUtils.h \
    src/main/cpp/tgs/TgsException.h \
    src/main/cpp/tgs/TgsExport.h \
    src/main/cpp/tgs/Version.h \
    src/main/cpp/tgs/CostSensitiveLearning/CostSensitiveLearningTrainer.h \
    src/main/cpp/tgs/CostSensitiveLearning/CslOptions.h \
    src/main/cpp/tgs/CostSensitiveLearning/CslUtilities.h \
    src/main/cpp/tgs/CostSensitiveLearning/MetaCostAlgorithm.h \
    src/main/cpp/tgs/FeatureExtraction/BasicMathCalculatorNode.h \
    src/main/cpp/tgs/FeatureExtraction/CalculatorGenome.h \
    src/main/cpp/tgs/FeatureExtraction/CalculatorGenomeNode.h \
    src/main/cpp/tgs/FeatureExtraction/CfsFitnessFunction.h \
    src/main/cpp/tgs/FeatureExtraction/ConstantCalculatorNodeSource.h \
    src/main/cpp/tgs/FeatureExtraction/DataFrameCalculatorNodeSource.h \
    src/main/cpp/tgs/FeatureExtraction/FeatureScoreFitnessFunction.h \
    src/main/cpp/tgs/FeatureExtraction/FitnessFunction.h \
    src/main/cpp/tgs/FeatureExtraction/GeneticAlgorithm.h \
    src/main/cpp/tgs/FeatureExtraction/Genome.h \
    src/main/cpp/tgs/FeatureExtraction/MimicGa.h \
    src/main/cpp/tgs/FeatureExtraction/TreeGenome.h \
    src/main/cpp/tgs/FeatureExtraction/TreeGenomeNode.h \
    src/main/cpp/tgs/FeatureExtraction/VectorCalculatorNodeSource.h \
    src/main/cpp/tgs/FeatureSelection/CfsSubsetEvaluator.h \
    src/main/cpp/tgs/FeatureSelection/ConsistencySubsetEvaluator.h \
    src/main/cpp/tgs/FeatureSelection/DataFrameDiscretizer.h \
    src/main/cpp/tgs/FeatureSelection/FactorEvaluator.h \
    src/main/cpp/tgs/FeatureSelection/FactorSubsetEvaluator.h \
    src/main/cpp/tgs/FeatureSelection/FayyadMdlDiscretizer.h \
    src/main/cpp/tgs/FeatureSelection/GreedyStepwiseSearch.h \
    src/main/cpp/tgs/FeatureSelection/InformationGainCalculator.h \
    src/main/cpp/tgs/FeatureSelection/SearchMethod.h \
    src/main/cpp/tgs/FeatureSelection/SymmetricUncertaintyCalculator.h \
    src/main/cpp/tgs/Heap/JHeap.h \
    src/main/cpp/tgs/ProbablePath/ProbablePathCalculator.h \
    src/main/cpp/tgs/RandomForest/DataFrame.h \
    src/main/cpp/tgs/RandomForest/InfoGainCalculator.h \
    src/main/cpp/tgs/RandomForest/BaseRandomForest.h \
    src/main/cpp/tgs/RandomForest/RandomForest.h \
    src/main/cpp/tgs/RandomForest/BaseRandomForestManager.h \
    src/main/cpp/tgs/RandomForest/RandomForestManager.h \
    src/main/cpp/tgs/RandomForest/RandomTree.h \
    src/main/cpp/tgs/RandomForest/TreeNode.h \
    src/main/cpp/tgs/RandomForest/MissingDataHandler.h \
    src/main/cpp/tgs/RandomForest/MultithreadedRandomForest.h \
    src/main/cpp/tgs/RandomForest/MultithreadedRandomForestManager.h \
    src/main/cpp/tgs/RandomForest/RandomForestThread.h \
    src/main/cpp/tgs/RandomForest/RandomForestUtilities.h \
    src/main/cpp/tgs/RasterOps/Image.hpp \
    src/main/cpp/tgs/RasterOps/MaxChannelCombiner.h \
    src/main/cpp/tgs/RStarTree/Box.h \
    src/main/cpp/tgs/RStarTree/DistanceIterator.h \
    src/main/cpp/tgs/RStarTree/HilbertCurve.h \
    src/main/cpp/tgs/RStarTree/HilbertRTree.h \
    src/main/cpp/tgs/RStarTree/InternalRStarTreeWrapper.h \
    src/main/cpp/tgs/RStarTree/IntersectionIterator.h \
    src/main/cpp/tgs/RStarTree/Iterator.h \
    src/main/cpp/tgs/RStarTree/KnnIterator.h \
    src/main/cpp/tgs/RStarTree/KnnIteratorNd.h \
    src/main/cpp/tgs/RStarTree/MemoryPageStore.h \
    src/main/cpp/tgs/RStarTree/Page.h \
    src/main/cpp/tgs/RStarTree/PageStore.h \
    src/main/cpp/tgs/RStarTree/RStarTree.h \
    src/main/cpp/tgs/RStarTree/RStarTreeWrapper.h \
    src/main/cpp/tgs/RStarTree/RTreeNode.h \
    src/main/cpp/tgs/RStarTree/RTreeNodeStore.h \
    src/main/cpp/tgs/Statistics/Random.h \
    src/main/cpp/tgs/System/SystemInfo.h \
    src/main/cpp/tgs/System/Time.h \
    src/main/cpp/tgs/Xml/XmlHelper.h \
    src/main/cpp/tgs/Statistics/Normal.h \
    src/main/cpp/tgs/RStarTree/RStarTreePrinter.h \
    src/main/cpp/tgs/StreamUtils.hh \
    src/main/cpp/tgs/DelaunayTriangulation/geom2d.h \
    src/main/cpp/tgs/DelaunayTriangulation/DelaunayTriangulation.h \
    src/main/cpp/tgs/DisjointSet/DisjointSet.h \
    src/main/cpp/tgs/DisjointSet/DisjointSetMap.h \
    src/main/cpp/tgs/Interpolation/Interpolator.h \
    src/main/cpp/tgs/Interpolation/BaseInterpolator.h \
    src/main/cpp/tgs/Interpolation/KernelEstimationInterpolator.h \
    src/main/cpp/tgs/Optimization/NelderMead.h \
    src/main/cpp/tgs/Interpolation/IdwInterpolator.h \
    src/main/cpp/tgs/Io/StdIoDevice.h \
    src/main/cpp/tgs/Interpolation/DelaunayInterpolator.h \
    src/main/cpp/tgs/BigContainers/BigMap.h \
    src/main/cpp/tgs/BigContainers/BigMapStl.h \
    src/main/cpp/tgs/BigContainers/BasicBloomFilter.h \
    src/main/cpp/tgs/System/DisableCout.h \
    src/main/cpp/tgs/System/Timer.h \
    src/main/cpp/tgs/BigContainers/BigMapHybrid.h \
    src/main/cpp/tgs/System/DisableCerr.h \
    src/main/cpp/tgs/LruCache.h \
    src/main/cpp/tgs/Optimization/SimulatedAnnealing.h \
    src/main/cpp/tgs/Optimization/StateDescription.h \
    src/main/cpp/tgs/Optimization/VariableDescription.h \
    src/main/cpp/tgs/Optimization/State.h \
    src/main/cpp/tgs/Optimization/FitnessFunction.h

win32:HEADERS += src/main/cpp/tgs/RStarTree/FilePageStore.h \
    src/main/cpp/tgs/SpinImage/GroundPlaneRemover.h \
    src/main/cpp/tgs/SpinImage/NormalEstimator.h \
    src/main/cpp/tgs/SpinImage/Points.h \
    src/main/cpp/tgs/SpinImage/SpinImage.h \
    src/main/cpp/tgs/SpinImage/SpinImageGenerator.h \
    src/main/cpp/tgs/SpinImage/SpinImageStack.h \
    src/main/cpp/tgs/SpinImage/SurfaceMatcher.h

SOURCES += src/main/cpp/tgs/StreamUtils.cpp \
    src/main/cpp/tgs/Version.cpp \
    src/main/cpp/tgs/CostSensitiveLearning/CostSensitiveLearningTrainer.cpp \
    src/main/cpp/tgs/CostSensitiveLearning/CslOptions.cpp \
    src/main/cpp/tgs/CostSensitiveLearning/CslUtilities.cpp \
    src/main/cpp/tgs/CostSensitiveLearning/MetaCostAlgorithm.cpp \
    src/main/cpp/tgs/FeatureExtraction/BasicMathCalculatorNode.cpp \
    src/main/cpp/tgs/FeatureExtraction/CalculatorGenome.cpp \
    src/main/cpp/tgs/FeatureExtraction/CalculatorGenomeNode.cpp \
    src/main/cpp/tgs/FeatureExtraction/CfsFitnessFunction.cpp \
    src/main/cpp/tgs/FeatureExtraction/ConstantCalculatorNodeSource.cpp \
    src/main/cpp/tgs/FeatureExtraction/DataFrameCalculatorNodeSource.cpp \
    src/main/cpp/tgs/FeatureExtraction/FeatureScoreFitnessFunction.cpp \
    src/main/cpp/tgs/FeatureExtraction/GeneticAlgorithm.cpp \
    src/main/cpp/tgs/FeatureExtraction/MimicGa.cpp \
    src/main/cpp/tgs/FeatureExtraction/TreeGenome.cpp \
    src/main/cpp/tgs/FeatureExtraction/TreeGenomeNode.cpp \
    src/main/cpp/tgs/FeatureExtraction/VectorCalculatorNodeSource.cpp \
    src/main/cpp/tgs/FeatureSelection/CfsSubsetEvaluator.cpp \
    src/main/cpp/tgs/FeatureSelection/ConsistencySubsetEvaluator.cpp \
    src/main/cpp/tgs/FeatureSelection/DataFrameDiscretizer.cpp \
    src/main/cpp/tgs/FeatureSelection/FayyadMdlDiscretizer.cpp \
    src/main/cpp/tgs/FeatureSelection/GreedyStepwiseSearch.cpp \
    src/main/cpp/tgs/FeatureSelection/InformationGainCalculator.cpp \
    src/main/cpp/tgs/FeatureSelection/SymmetricUncertaintyCalculator.cpp \
    src/main/cpp/tgs/ProbablePath/ProbablePathCalculator.cpp \
    src/main/cpp/tgs/RandomForest/DataFrame.cpp \
    src/main/cpp/tgs/RandomForest/InfoGainCalculator.cpp \
    src/main/cpp/tgs/RandomForest/BaseRandomForest.cpp \
    src/main/cpp/tgs/RandomForest/RandomForest.cpp \
    src/main/cpp/tgs/RandomForest/BaseRandomForestManager.cpp \
    src/main/cpp/tgs/RandomForest/RandomForestManager.cpp \
    src/main/cpp/tgs/RandomForest/RandomTree.cpp \
    src/main/cpp/tgs/RandomForest/MissingDataHandler.cpp \
    src/main/cpp/tgs/RandomForest/MultithreadedRandomForest.cpp \
    src/main/cpp/tgs/RandomForest/MultithreadedRandomForestManager.cpp \
    src/main/cpp/tgs/RandomForest/RandomForestThread.cpp \
    src/main/cpp/tgs/RandomForest/RandomForestUtilities.cpp \
    src/main/cpp/tgs/RasterOps/MaxChannelCombiner.cpp \
    src/main/cpp/tgs/RStarTree/Box.cpp \
    src/main/cpp/tgs/RStarTree/DistanceIterator.cpp \
    src/main/cpp/tgs/RStarTree/HilbertRTree.cpp \
    src/main/cpp/tgs/RStarTree/InternalRStarTreeWrapper.cpp \
    src/main/cpp/tgs/RStarTree/IntersectionIterator.cpp \
    src/main/cpp/tgs/RStarTree/KnnIterator.cpp \
    src/main/cpp/tgs/RStarTree/KnnIteratorNd.cpp \
    src/main/cpp/tgs/RStarTree/MemoryPageStore.cpp \
    src/main/cpp/tgs/RStarTree/Page.cpp \
    src/main/cpp/tgs/RStarTree/RStarTree.cpp \
    src/main/cpp/tgs/RStarTree/RStarTreeWrapper.cpp \
    src/main/cpp/tgs/RStarTree/RTreeNode.cpp \
    src/main/cpp/tgs/RStarTree/RTreeNodeStore.cpp \
    src/main/cpp/tgs/Statistics/Random.cpp \
    src/main/cpp/tgs/System/SystemInfo.cpp \
    src/main/cpp/tgs/System/Time.cpp \
    src/main/cpp/tgs/Xml/XmlHelper.cpp \
    src/main/cpp/tgs/Statistics/Normal.cpp \
    src/main/cpp/tgs/RStarTree/RStarTreePrinter.cpp \
    src/main/cpp/tgs/DelaunayTriangulation/DelaunayTriangulation.cpp \
    src/main/cpp/tgs/DisjointSet/DisjointSet.cpp \
    src/main/cpp/tgs/Interpolation/BaseInterpolator.cpp \
    src/main/cpp/tgs/Interpolation/KernelEstimationInterpolator.cpp \
    src/main/cpp/tgs/Optimization/NelderMead.cpp \
    src/main/cpp/tgs/Interpolation/IdwInterpolator.cpp \
    src/main/cpp/tgs/Io/StdIoDevice.cpp \
    src/main/cpp/tgs/Interpolation/DelaunayInterpolator.cpp \
    src/main/cpp/tgs/BigContainers/BigMap.cpp \
    src/main/cpp/tgs/BigContainers/BasicBloomFilter.cpp \
    src/main/cpp/tgs/System/DisableCout.cpp \
    src/main/cpp/tgs/System/DisableCerr.cpp \
    src/main/cpp/tgs/Optimization/SimulatedAnnealing.cpp \
    src/main/cpp/tgs/Optimization/StateDescription.cpp \
    src/main/cpp/tgs/Optimization/VariableDescription.cpp \
    src/main/cpp/tgs/Optimization/State.cpp \

cppunit:SOURCES += \
    src/test/cpp/tgs/ProbablePath/ProbablePathCalculatorTest.cpp \
    src/test/cpp/tgs/PluginFactory.cpp \
    src/test/cpp/tgs/FeatureExtraction/CalculatorGenomeNodeTest.cpp \
    src/test/cpp/tgs/FeatureExtraction/CalculatorGenomeTest.cpp \
    src/test/cpp/tgs/FeatureExtraction/GeneticAlgorithmTest.cpp \
    src/test/cpp/tgs/Xml/XmlHelperTest.cpp \
    src/test/cpp/tgs/RStarTree/HilbertRTreeTest.cpp \
    src/test/cpp/tgs/RStarTree/RTreeNodeStoreTest.cpp \
    src/test/cpp/tgs/RStarTree/DistanceIteratorTest.cpp \
    src/test/cpp/tgs/RStarTree/RTreeNodeTest.cpp \
    src/test/cpp/tgs/RStarTree/RStarTreeTest.cpp \
    src/test/cpp/tgs/RStarTree/BoxTest.cpp \
    src/test/cpp/tgs/RStarTree/HilbertCurveTest.cpp \
    src/test/cpp/tgs/FeatureSelection/DataFrameDiscretizerTest.cpp \
    src/test/cpp/tgs/FeatureSelection/ConsistencySubsetEvaluatorTest.cpp \
    src/test/cpp/tgs/FeatureSelection/FayyadMdlDiscretizerTest.cpp \
    src/test/cpp/tgs/FeatureSelection/CfsSubsetEvaluatorTest.cpp \
    src/test/cpp/tgs/FeatureSelection/GreedyStepwiseSearchTest.cpp \
    src/test/cpp/tgs/FeatureSelection/SymmetricUncertaintyCalculatorTest.cpp \
    src/test/cpp/tgs/RandomForest/RandomForestTest.cpp \
    src/test/cpp/tgs/RandomForest/RfDataFrameTest.cpp \
    src/test/cpp/tgs/RandomForest/InfoGainCalculatorTest.cpp \
    src/test/cpp/tgs/RandomForest/RandomTreeTest.cpp \
    src/test/cpp/tgs/RStarTree/KnnIteratorNdTest.cpp \
    src/test/cpp/tgs/System/SystemInfoTest.cpp \
    src/test/cpp/tgs/DelaunayTriangulation/DelaunayTriangulationTest.cpp \
    src/test/cpp/tgs/DisjointSet/DisjointSetTest.cpp \
    src/test/cpp/tgs/DisjointSet/DisjointSetMapTest.cpp \
    src/test/cpp/tgs/Interpolation/KernelEstimationInterpolatorTest.cpp \
    src/test/cpp/tgs/Optimization/NelderMeadTest.cpp \
    src/test/cpp/tgs/Interpolation/DelaunayInterpolatorTest.cpp \
    src/test/cpp/tgs/Io/StdIoDeviceTest.cpp \
    src/test/cpp/tgs/BigContainers/BigMapTest.cpp \
    src/test/cpp/tgs/BigContainers/BasicBloomFilterTest.cpp \
    src/test/cpp/tgs/LruCacheTest.cpp \
    src/test/cpp/tgs/Optimization/SimulatedAnnealingTest.cpp

win32:SOURCES += src/main/cpp/tgs/RStarTree/FilePageStore.cpp \
    src/main/cpp/tgs/SpinImage/GroundPlaneRemover.cpp \
    src/main/cpp/tgs/SpinImage/NormalEstimator.cpp \
    src/main/cpp/tgs/SpinImage/SpinImage.cpp \
    src/main/cpp/tgs/SpinImage/SpinImageGenerator.cpp \
    src/main/cpp/tgs/SpinImage/SpinImageStack.cpp \
    src/main/cpp/tgs/SpinImage/SurfaceMatcher.cpp \
    src/test/cpp/tgs/RStarTree/PageStoreTest.cpp \
    src/test/cpp/tgs/SpinImage/SpinImageGeneratorTest.cpp \
    src/test/cpp/tgs/SpinImage/GroundPlaneRemoverTest.cpp \
    src/test/cpp/tgs/SpinImage/SpinImageStackTest.cpp \
    src/test/cpp/tgs/SpinImage/NormalEstimatorTest.cpp \
    src/test/cpp/tgs/SpinImage/SpinImageTest.cpp \

win32:INCLUDEPATH += $${THIRDPARTY_PATH}/NewMat/include

release { 
    win32::LIBS += $${THIRDPARTY_PATH}/Newmat/Lib/newmat.lib
    win32:LIBS += $${THIRDPARTY_PATH}/boost/lib/libboost_thread-vc80-mt-1_34_1.lib
}
else { 
    win32::LIBS += $${THIRDPARTY_PATH}/Newmat/Lib/newmatd.lib
    win32:LIBS += $${THIRDPARTY_PATH}/boost/lib/libboost_thread-vc80-mt-gd-1_34_1.lib
}
