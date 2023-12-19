#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <cmath>

#include "Data.hh"

// generic function comparing two values of some type T used later for int and
// double
template <class T>
bool testEqual(const std::string& name, T expected, T real) {
  if (expected != real) {
    std::cout << "(" << name << ": <" << expected << ">"
              << ", " << real << ") ";
    return false;
  }
  return true;
}

bool testReadingSize() {
  std::cout << "testReadingSize...";
  Data datA("testA");
  return testEqual<int>("size", 1, datA.size());
}

bool testReadingMeasurement() {
  std::cout << "testReadingMeasurement...";
  Data datA("testA");
  return testEqual("measurement", 10., datA.measurement(0));
}

bool testReadingBinEdges() {
  std::cout << "testReadingBinEdges...";
  Data datA("testA");
  return testEqual("bin low", 0., datA.binLow(0)) &&
         testEqual("bin high", 1., datA.binHigh(0));
}

bool testReadingErrors() {
  std::cout << "testReadingErrors...";
  Data datA("testA");
  return testEqual("error", 2., datA.error(0));
}

bool testCopyConstructor() {
  std::cout << "testCopyConstructor...";
  Data a("testA");
  Data b(a);
  Data c = a;  // equivalent to Data c(a)
  return testEqual("measurement", 10., b.measurement(0)) &&
         testEqual("measurement", 10., c.measurement(0));
}

void runTests() {
  std::cout << "running tests...\n";
  std::vector<std::function<bool()> > tests(
      {testReadingSize, testReadingMeasurement, testReadingBinEdges,
       testReadingErrors, testCopyConstructor});
  for (auto test : tests)
    std::cout << (test() ? " ok" : " FAILED!") << std::endl;
}

int main() {
  using namespace std;

  cout << "******************************************************" << endl;
  runTests();
  cout << "******************************************************" << endl;

  // create an object which holds data of experiment A
  std::vector<string> expname({"A", "B", "C", "D"});
  std::vector<Data> data;

  for (string expname : expname) {
    data.push_back(Data("exp_" + expname));
  }
  // here is the data from experiment A
  cout << "bin 27: from " << data[0].binLow(27) << " to " << data[0].binHigh(27)
       << endl;
  cout << "measurement of experiment A in bin 27: " << data[0].measurement(27)
       << endl;

  for (int i = 0; i < data.size(); ++i) {
    cout << "measurement of experiment " << expname[i] << " in bin 27: " << data[i].measurement(27) << endl;
  }

  // Compatibility 

  for (int i = 0; i < data.size(); i++){
    for (int j=i+1; j < data.size(); j++){
      if (i == j){
        continue;
      }
      double diff_ = abs(data[i].measurement(27) - data[j].measurement(27));
      double sigma = sqrt(pow(data[i].error(27), 2) + pow(data[j].error(27), 2));
      if (diff_ > 2 * sigma) {
        cout << expname[i] << " and " << expname[j] << " are not compatible." << endl;
      }
      else {
        cout << expname[i] << " and " << expname[j] << " are compatible." << endl;
      }
      }
    }
  
  //ex 2

  for (int i = 0; i < data.size(); ++i) {
    for (int j =  i + 1; j < data.size(); ++j) {
      if (i == j) {
        continue;
      }
      if (data[i].checkCompatibility(data[j], 2) < 2) {
        Data average = data[i].average(data[j]);
        cout << "Average of " << expname[i] << " and " << expname[j] << " is compatible with both experiments." << endl;
        cout << "average measurement of experiment ";
        for (int k = 0; k < average.size(); ++k) {
          cout << average.measurement(k);
        }
        cout << endl;
      }
      else {
        cout << "Average of " << expname[i] << " and " << expname[j] << " is not compatible with both experiments." << endl;
      }
    }
  }

  for (Data d : data) {
    cout << "chi2 of experiment " << d.name() << " " << d.chi2() << endl;
  }

  Data total_avg = data[0];
  for (int i = 1; i < data.size(); ++i) {
    total_avg = total_avg.average(data[i]);
  }

  cout << "chi2 of total average " << total_avg.chi2() << endl;

  return 0;
}
