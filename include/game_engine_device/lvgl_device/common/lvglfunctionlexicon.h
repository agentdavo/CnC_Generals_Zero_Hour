#pragma once

#include "game_engine/common/function_lexicon.h"

/**
 * LVGL-specific FunctionLexicon implementation
 * This is a stub implementation for cross-platform development
 */
class LvglFunctionLexicon : public FunctionLexicon
{
public:
    LvglFunctionLexicon();
    virtual ~LvglFunctionLexicon();

    // FunctionLexicon interface implementation
    virtual void init() override;
    virtual void reset() override;
    virtual void update() override;
};