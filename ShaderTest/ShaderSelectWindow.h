#pragma once
#include "..\Engine\JButton.h"
#include "..\Engine\ImageBox.h"
#include "..\Engine\Win.h"
class ShaderSelectWindow :
    public Win
{
public:
    ShaderSelectWindow(void);
    ~ShaderSelectWindow(void);
    JButton *apply1, *apply2, *apply3, *apply4, *apply5;
    ImageBox *ibox;
};

