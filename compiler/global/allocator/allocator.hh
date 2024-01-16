#pragma once

#include <iostream>
#include <exception>

namespace xtaro
{

    class Allocator
    {
    public:
        template<typename Type, typename... Args>
        static Type* alloc(Args&&... args) noexcept
        {
            Type* instance = new Type{std::forward<Args>(args)...};
            return instance;
        }

        template<typename Type>
        static void free(Type* instance) noexcept
        {
            delete instance;
        }

    };

}