#include <MatlabEngine.hpp>

class MatEngine
{

public:
  MatEngine();
  ~MatEngine();
  bool callExtract(char *buffer, double maxcutvalue);

private:
  std::unique_ptr<matlab::engine::MATLABEngine> matlabPtr;
};