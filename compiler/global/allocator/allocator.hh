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
            try 
            {
                Type* instance = new Type(std::forward<Args>(args)...);
                return instance;
            }
            catch (const std::exception& e) 
            {
                std::cout << "Allocate memory error!: " << e.what() << std::endl;
                exit(11);
            }
        }

        template<typename Type>
        static void free(Type* instance) noexcept
        {
            delete instance;
        }

    };

}