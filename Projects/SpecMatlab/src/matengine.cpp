#include "matengine.h"

#include <MatlabDataArray.hpp>

using namespace matlab::engine;

MatEngine::MatEngine()
{   
   matlabPtr = matlab::engine::startMATLAB();
}
MatEngine::~MatEngine() = default;

bool MatEngine::callExtract(char *buffer, double maxcutvalue)
{
    matlab::data::ArrayFactory factory;
    auto path = factory.createCharArray(buffer);
    auto percent = factory.createScalar<double>(maxcutvalue);

    // set values to workspace
    matlabPtr->setVariable(convertUTF8StringToUTF16String("percentage_main"), std::move(percent));
    matlabPtr->setVariable(convertUTF8StringToUTF16String("path"), std::move(path));

    matlabPtr->eval(convertUTF8StringToUTF16String("A = extract(path, percentage_main);"));

    // Get the result
    matlab::data::TypedArray<double> const A = matlabPtr->getVariable(convertUTF8StringToUTF16String("A"));

    if (A.getNumberOfElements() > 1)
    {
        return true;
    }
    else
    {
        return false;
    }
};
