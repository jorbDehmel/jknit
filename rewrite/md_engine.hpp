/*
An instantiation of the JKnit engine targeting markdown.
*/

#pragma once
#include "engine.hpp"

/*
Outputs markdown, but with the output of any code chunks
inserted in their proper position.
*/
class MDEngine : public Engine
{
  public:
    MDEngine(const Settings &_s) : Engine(_s)
    {
    }

  protected:
    // Constructs a series of text/code chunks into the output
    // file via Markdown (md).
    void knit(const std::list<Chunk> &_chunks);
};
