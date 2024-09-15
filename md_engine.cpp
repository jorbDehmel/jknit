#include "md_engine.hpp"

// Knits together a series of chunks
void MDEngine::knit(const std::list<Chunk> &_chunks)
{
    bool skip_output = false;
    for (const auto &chunk : _chunks)
    {
        if (chunk.type == "TEXT")
        {
            // Just regular markdown
            for (const auto &line : chunk.lines)
            {
                target << line << '\n';
            }
        }
        else if (chunk.type == "OUTPUT")
        {
            if (skip_output)
            {
                skip_output = false;
                continue;
            }
            else if (chunk.lines.empty())
            {
                continue;
            }

            // Output chunk
            target << "```\n";
            for (const auto &line : chunk.lines)
            {
                target << line << '\n';
            }
            target << "```\n";
        }
        else
        {
            if (chunk.show_code)
            {
                // Code chunk of some sort
                target << "```" << chunk.type << '\n';
                for (const auto &line : chunk.lines)
                {
                    target << line << '\n';
                }
                target << "```\n";
            }

            if (!chunk.show_output)
            {
                skip_output = true;
            }
        }
        target << '\n';
    }
}
