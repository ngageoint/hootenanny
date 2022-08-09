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
 * @copyright Copyright (C) 2015, 2016, 2017, 2021 Maxar (http://www.maxar.com/)
 */

// Standard Includes
#include <fstream>
#include <iostream>
#include <math.h>
using namespace std;

// CPP Unit Includes
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt Includes
#include <QDir>

#include <tgs/StreamUtils.h>
#include <tgs/SpinImage/GroundPlaneRemover.h>
#include <tgs/SpinImage/Points.h>
#include <tgs/SpinImage/SpinImage.h>
#include <tgs/SpinImage/SpinImageGenerator.h>
#include <tgs/SpinImage/SpinImageStack.h>
#include <tgs/Statistics/Random.h>

#include "../PluginFactory.h"


namespace Tgs
{
  class SpinImageGeneratorTest : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(SpinImageGeneratorTest);
    CPPUNIT_TEST(testSimple);
    CPPUNIT_TEST(testSphere);
    //CPPUNIT_TEST(testCloser);
    CPPUNIT_TEST_SUITE_END();
  public:

    double calculateMean(const std::vector<double>& v)
    {
      double sum = 0.0;
      for (unsigned int i = 0; i < v.size(); i++)
      {
        sum += v[i];
      }
      return sum / v.size();
    }

    double calculateStandardDeviation(const std::vector<double>& v)
    {
      double mean = calculateMean(v);
      double sum = 0.0;
      for (unsigned int i = 0; i < v.size(); i++)
      {
        double tmp = (v[i] - mean);
        sum += tmp * tmp;
      }
      return sqrt(sum / v.size());
    }

    vector<string> findAllObjectFiles(string obj)
    {
      QDir d("C:\\GisData\\Urgent\\Ottawa\\extracted_objects_v7\\" + QString::fromStdString(obj));
      vector<string> result;

      QStringList allDirs = d.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::NoFilter,
        QDir::Name);

      for (int i = 0; i < allDirs.size(); i++)
      {
        QDir dd(d.absolutePath() + "\\" + allDirs[i]);
        QStringList file = dd.entryList(QDir::Files);
        if (file.size() != 1)
        {
          cout << "Only expected one file." << dd.absolutePath().toStdString() << endl;
        }
        else
        {
          QString p = (dd.absolutePath() + "\\" + file[0]);
          p.replace("/", "\\");
          result.push_back(p.toStdString());
        }
      }

      return result;
    }

    void loadPointCloud(string fn, PointCloud& pointCloud)
    {
      std::fstream ins(fn.c_str(), std::fstream::in);

      if(ins.is_open())
      {
        while(!ins.eof())
        {
          char inLine[512];
          Tgs::CloudPoint cp;
          ins.getline(inLine, 512);
          std::stringstream ss;
          ss << inLine;
          ss >> cp.x >> cp.y >> cp.z >> cp.i >> cp.r >> cp.g >> cp.b;
          pointCloud.addPoint(cp);
        }
      }
      else
      {
        std::cout << "Unable to open " << fn << std::endl;
      }
    }

    void split(vector<string>& orig, int trainSize, vector<string>& training,
      int testSize, vector<string>& testing)
    {
      for (unsigned int i = 0; i < orig.size(); i++)
      {
        int r1 = Tgs::Random::instance()->generate(orig.size());
        int r2 = Tgs::Random::instance()->generate(orig.size());
        string tmp = orig[r1];
        orig[r1] = orig[r2];
        orig[r2] = tmp;
      }

      for (int j = 0; j < trainSize; j++)
      {
        training.push_back(orig[j]);
      }

      for (int j = trainSize; j < trainSize + testSize; j++)
      {
        testing.push_back(orig[j]);
      }
    }

    void savePointCloud(string fn, const PointCloud& pointCloud)
    {
      std::fstream out(fn.c_str(), std::fstream::out);

      if(out.is_open())
      {
        for (unsigned int i = 0; i < pointCloud.getPoints().size(); i++)
        {
          const Tgs::CloudPoint& cp = pointCloud.getPoints()[i];
          out.precision(16);
          out << cp.x << " " << cp.y << " " << cp.z << " "
            << cp.i << " " << cp.r << " " << cp.g << " " << cp.b << 
            " 0.000000 000 00 000" << endl; // these zeros appear to be at the end of every line
        }
      }
      else
      {
        std::cout << "Unable to open " << fn << std::endl;
      }
    }

    void testCloser()
    {
      CPPUNIT_ASSERT(false);
      vector<string> objectsOfInterest;
      objectsOfInterest.push_back("traffic_signs");
      objectsOfInterest.push_back("lamp_post");
      objectsOfInterest.push_back("tree");
      objectsOfInterest.push_back("light_standard");

      std::fstream summary("SpinImageSummary.txt", std::fstream::app);

      CPPUNIT_ASSERT(summary.is_open());

for (int TRAINING_SIZE = 2; TRAINING_SIZE < 17; TRAINING_SIZE *= 2)
{

      const int TESTING_SIZE = 20;

      vector<string> trainingType;
      vector<string> training;
      vector<string> testing;
      vector<string> testingType;

      for (unsigned int i = 0; i < objectsOfInterest.size(); i++)
      {
        vector<string> filenames = findAllObjectFiles(objectsOfInterest[i]);
        split(filenames, TRAINING_SIZE, training, TESTING_SIZE, testing);
        for (int j = 0; j < TRAINING_SIZE; j++)
        {
          trainingType.push_back(objectsOfInterest[i]);
        }
        for (int j = 0; j < TESTING_SIZE; j++)
        {
          testingType.push_back(objectsOfInterest[i]);
        }
      }

      vector<int> s;
//       s.push_back(10);
      s.push_back(15);
      //s.push_back(20);
//       s.push_back(30);

      vector<double> b;
      b.push_back(5);
      //b.push_back(10);

      const int SAMPLE_SIZE = 100;

      vector<int> ra;
      ra.push_back(SpinImageGenerator::Up);
      ra.push_back(SpinImageGenerator::Normal);

for (unsigned int rai = 0; rai < ra.size(); rai++)
for (unsigned int bi = 0; bi < b.size(); bi++)
for (unsigned int si = 0; si < s.size(); si++)
{
      vector<boost::shared_ptr<SpinImageStack> > stacks;

      SpinImageGenerator gen;
      gen.setRelativeAngle(ra[rai]);
      gen.setSize(s[si], s[si]);
      gen.setKThreshold(5);
      gen.setBounds(b[bi], b[bi]);

      for (unsigned int i = 0; i < training.size(); i++)
      {
        cout << "Training " << training[i] << endl;
        boost::shared_ptr<SpinImageStack> stack(new SpinImageStack());
        stacks.push_back(stack);
        PointCloud pc;
        pc.load(training[i]);
        GroundPlaneRemover gpr;
        gpr.removeGroundPlane(pc);
        gen.generateStack(pc, *stack);
      }

      int numCorrect = 0;
      double errorSum = 0;
      for (unsigned int i = 0; i < testing.size(); i++)
      {
        cout << "Testing " << testing[i] << endl;
//        SpinImageStack stack;
        PointCloud pc;
//         cout << "  Loading..." << endl;
        loadPointCloud(testing[i], pc);
//         cout << "  Normals/Ground Plane Removal..." << endl;
        GroundPlaneRemover gpr;
        gpr.removeGroundPlane(pc);
        //gen.generateStack(pc, stack);

        double bestScore;
//        std::map<string, double> bestScores;

        std::map<string, int> results;
        bestScore = -1e300;
        string bestMatch;
        SpinImage testImage;
//         cout << "  Comparing..." << endl;
        for (int j = 0; j < SAMPLE_SIZE; j++)
        {
          //SpinImage testImage = stack.getImage(Tgs::Random::instance()->generate(stack.getSize()));
          int randomIndex = Tgs::Random::instance()->generate(pc.getPoints().size());
          gen.generateSingleImage(pc, randomIndex, testImage);

          for (unsigned int trainingStackIdx = 0; trainingStackIdx < stacks.size(); 
            trainingStackIdx++)
          {
            std::vector<double> scores;
            const boost::shared_ptr<SpinImageStack> sig = stacks[trainingStackIdx];
            scores.reserve(sig->getSize());
  //           cout << " * comparing " << trainingType[trainingStackIdx];
            for (unsigned int imageIdx = 0; imageIdx < sig->getSize(); imageIdx++)
            {
              int idx = Tgs::Random::instance()->generate(sig->getSize());
              double score = sig->getImage(idx)->compare(testImage);
              scores.push_back(score);
              if (score > bestScore)
              {
                bestScore = score;
                bestMatch = trainingType[trainingStackIdx];
              }
  //             if (score > bestTmpScore)
  //             {
  //               bestTmpScore = score;
  //             }
            }
  //           double mu = calculateMean(scores);
  //           double sigma = calculateStandardDeviation(scores);
  // 
  //           for (unsigned int i = 0; i < scores.size(); i++)
  //           {
  //             if (scores[i] > mu + sigma * 6)
  //             {
  //               results[trainingType[trainingStackIdx]]++;
  //             }
  //           }
            //printf("mu: %g sigma: %g\n", mu, sigma);
          }

          results[bestMatch]++;
        }

        int max = -1;
        for (std::map<string, int>::iterator it = results.begin(); it != results.end(); ++it)
        {
          max = std::max(it->second, max);
        }

        if (results[testingType[i]] == max)
        {
          numCorrect++;
        }

        double error = 1.0 - (double)results[testingType[i]] / (double)SAMPLE_SIZE;
        cout << "Type: " << testingType[i] << 
          "\terror: " << error << 
          "\t" << results << endl;
//         printf("    car:       %d (%g)\n", results["car"], bestScores["car"]);
//         printf("    lamp_post: %d (%g)\n", results["lamp_post"], bestScores["lamp_post"]);
//         printf("    tree:      %d (%g)\n", results["tree"], bestScores["tree"]);
        errorSum += error * error;
      }

      summary << TRAINING_SIZE << "\t" 
        << gen.getAlphaSize() << "x" << gen.getBetaSize() << "\t" 
        << gen.getAlphaMax() << "m x " << gen.getBetaMax() << "m\t" 
        << (ra[rai] == SpinImageGenerator::Normal ? "Normal" : "Up") << "\t" 
        << errorSum / (double)testingType.size() << "\t"
        << (double)numCorrect / (double)testingType.size() << "\t"
        << "CompareAllPoints" << endl;
      summary.flush();
      printf("Training Size: %d\n", TRAINING_SIZE);
      printf("Image size: %d x %d\n", gen.getAlphaSize(), gen.getBetaSize());
      printf("Image bounds: %fm x %fm\n", gen.getAlphaMax(), gen.getBetaMax());
      printf("Normal: %s\n", ra[rai] == SpinImageGenerator::Normal ? "Normal" : "Up");
      printf("MSE: %f\n", errorSum / (double)testingType.size());
      printf("%% correct: %f\n", (double)numCorrect / (double)testingType.size());
}
} // training size
    }

    void testSimple()
    {
      PointCloud pc;
      pc.addPoint(CloudPoint(-5, 0, 0));
      pc.addPoint(CloudPoint(0, -5, 0));
      pc.addPoint(CloudPoint(0, 0, -5));
      pc.addPoint(CloudPoint(5, 0, 0));
      pc.addPoint(CloudPoint(0, 5, 0));
      pc.addPoint(CloudPoint(0, 0, 5));

      SpinImageStack stack;
      SpinImageGenerator uut;
      uut.setSize(10, 10);
      uut.setKThreshold(5);
      uut.setBounds(10, 20);
      NormalEstimator ne;
      ne.calculateNormals(pc);
      uut.generateStack(pc, stack);
  //     for (unsigned int i = 0; i < 3; i++)
  //     {
  //       float sum = 0.0;
  //       const SpinImage* image = stack.getImage(i);
  //       for (int b = 0; b < image->getBetaBins(); b++)
  //       {
  //         for (int a = 0; a < image->getAlphaBins(); a++)
  //         {
  //           float v = image->bin(a, b);
  //           printf("%6.3f  ", v);
  //           sum += v;
  //         }
  //         printf("\\n");
  //       }
  //       printf("sum: %.1f\\n", sum);
  //       printf("\\n");
  //     }
    }

    void testSphere()
    {
      int testSize = 500;
      PointCloud pc;
      // create a sphere of points, almost uniformly distributed
      for (int i = 0; i < testSize; i++)
      {
        Point3d p(Tgs::Random::instance()->generateInt() - RAND_MAX / 2.0,
                  Tgs::Random::instance()->generateInt() - RAND_MAX / 2.0,
                  Tgs::Random::instance()->generateInt() - RAND_MAX / 2.0);
        p.normalize();
        p = p * 10.0;
        pc.addPoint(CloudPoint(p.p1, p.p2, p.p3));
      }

      double expected[][10] =
      {
        {  0.028,  0.122,  0.044,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000  },
        {  0.012,  0.080,  0.132,  0.002,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000  },
        {  0.000,  0.041,  0.140,  0.005,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000  },
        {  0.000,  0.065,  0.127,  0.002,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000  },
        {  0.019,  0.114,  0.040,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000  },
        {  0.014,  0.013,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000  },
        {  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000  },
        {  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000  },
        {  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000  },
        {  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000  },
      };

      SpinImageStack stack;
      SpinImageGenerator uut;
      uut.setSize(10, 10);
      uut.setBounds(40, 40);
      NormalEstimator ne;
      ne.calculateNormals(pc);
      uut.generateStack(pc, stack);
      for (unsigned int i = 0; i < stack.getSize(); i++)
      {
        float sum = 0.0;
        float error = 0.0;
        const SpinImage* image = stack.getImage(i);
        for (int b = 0; b < image->getBetaBins(); b++)
        {
  //         printf("  { ");
          for (int a = 0; a < image->getAlphaBins(); a++)
          {
            float v = image->bin(a, b);
  //           printf("%6.3f, ", v / testSize);
            error += fabs((float)expected[b][a] - v / (float)testSize);
            sum += v;
          }
  //         printf(" }\\n");
        }
        // make sure we're in the right ball park.
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0, error, .3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(testSize - 1, sum, .01);
      }
    }
  };
}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::SpinImageGeneratorTest, PluginFactory::testName());
