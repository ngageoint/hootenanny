#ifndef CSLUTILITIES_H
#define CSLUTILITIES_H

//Std Includes
#include <string>
#include <vector>

class CslUtilities
{
public:

  /**
   * @brief readCostMatrix read a file containing the cost matrix in row major form (space separate column entries and CR for each row)
   * @param filename the name of the file
   * @return the cost matrix
   */
  static std::vector<float> readCostMatrix(std::string filename);
};

#endif // CSLUTILITIES_H
