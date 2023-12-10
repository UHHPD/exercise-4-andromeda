#include "Data.hh"

#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>
#include <cmath>

using namespace std;

Data::Data(const std::string& filename) {
  ifstream file(filename);

  // check if file opened correctly
  if (!file.is_open()) {
    throw std::runtime_error("Error could not open file with name: " +
                             filename);
  }

  int size = -1;
  file >> size;

  // read in data from file: m_siz+1 bin edges
  for (int i = 0; i < size + 1; ++i) {
    double edge;
    file >> edge;
    m_bins.push_back(edge);
  }
  // read in data from file: m_siz bin contents
  for (int i = 0; i < size; ++i) {
    double entries;
    file >> entries;
    m_data.push_back(entries);
  }

  // read in data from file: m_siz bin errors
  for (int i = 0; i < size; ++i) {
    double error;
    file >> error;
    m_errors.push_back(error);
  }

  // done! close the file
  file.close();

  assertSizes();
};

int Data :: checkCompatibility ( const Data & in , int n ){
  int incompatible_total = 0;
  for (int i = 0; i < m_data.size(); ++i) {
    double diff_ = abs(m_data[i] - in.measurement(i));
    double sigma = sqrt(pow(m_errors[i], 2) + pow(in.error(i), 2));
    if (diff_ > n * sigma) {
      ++incompatible_total;
    }
  }
  return incompatible_total;
}
void Data::assertSizes() { assert(m_data.size() + 1 == m_bins.size()); }
