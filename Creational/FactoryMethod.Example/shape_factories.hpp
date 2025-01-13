#ifndef SHAPE_FACTORIES_HPP
#define SHAPE_FACTORIES_HPP

#include <functional>
#include <memory>
#include <string>
#include <typeindex>
#include <vector>

#include "shape.hpp"
#include "./shape_readers_writers/shape_reader_writer.hpp"

using ShapeCreator = std::function<std::unique_ptr<Drawing::Shape>()>;

template <typename TProduct, typename TId = std::string, typename TCreator = std::function<std::unique_ptr<TProduct>()>>
class GenericFactory
{
    std::unordered_map<TId, TCreator> creators_;
public:
    bool register_creator(const TId& id, TCreator creator)
    {
        return creators_.emplace(id, creator).second;
    }

    std::unique_ptr<TProduct> create(const TId& id)
    {
        return creators_.at(id)();
    }
};

template <typename T>
class SingletonHolder
{
    SingletonHolder() = default;
public:
    SingletonHolder(const SingletonHolder&) = delete;
    SingletonHolder& operator=(const SingletonHolder&) = delete;
 
    static T& instance()
    {
        static T unique_instance;
 
        return unique_instance;
    }
};

using ShapeFactory = GenericFactory<Drawing::Shape>;
using ShapeRWFactory = GenericFactory<Drawing::IO::ShapeReaderWriter, std::type_index>;

using SingletonShapeFactory = SingletonHolder<ShapeFactory>;
using SingletonShapeRWFactory = SingletonHolder<ShapeRWFactory>;

template <typename T>
std::type_index make_type_index()
{
    return std::type_index(typeid(T));
}

template <typename T>
std::type_index make_type_index(const T& obj)
{
    return std::type_index(typeid(obj));
}

#endif