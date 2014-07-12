#pragma once
#include "Mesh.h"
#include <memory>
class SkySphere
{
public:
    SkySphere(void);
    ~SkySphere(void);
    int Generate(void *NotUsed, int argc, char **argv, char **azColName);
    std::shared_ptr<Mesh> m;
};

