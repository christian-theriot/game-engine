#ifndef __ENGINE_SERIALIZATION_SERIALIZABLE_HPP
#define __ENGINE_SERIALIZATION_SERIALIZABLE_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

namespace Engine::Serialization
{
    class ISerializable
    {
    public:
        virtual ~ISerializable() = default;
    };
}

#endif