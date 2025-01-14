#ifndef SHAPEGROUP_HPP
#define SHAPEGROUP_HPP

#include <memory>
#include <vector>

#include "shape.hpp"

namespace Drawing
{    
    using ShapePtr = std::unique_ptr<Shape>;

    class ShapeGroup : public CloneableShape<ShapeGroup>
    {
        std::vector<ShapePtr> shapes_;
    public:
        constexpr static auto id = "ShapeGroup";

        using iterator = std::vector<ShapePtr>::iterator;
        using const_iterator = std::vector<ShapePtr>::const_iterator;

        ShapeGroup() = default;

        ShapeGroup(const ShapeGroup& source)
        {
            shapes_.reserve(source.shapes_.size());

            for(const auto& shp : source.shapes_)
                shapes_.push_back(shp->clone());
        }

        ShapeGroup& operator=(const ShapeGroup& s)
        {
            ShapeGroup temp(s);
            swap(temp);

            return *this;
        }

        ShapeGroup(ShapeGroup&& source) = default;
        ShapeGroup& operator=(ShapeGroup&& source) = default;

        void swap(ShapeGroup& other)
        {
            shapes_.swap(other.shapes_);
        }

        void move(int dx, int dy) override
        {
            for(const auto& shp : shapes_)
                shp->move(dx, dy);
        }

        void draw() const override
        {
            for(const auto& shp : shapes_)
                shp->draw();
        }

        void add(ShapePtr shape)
        {
            shapes_.push_back(std::move(shape));
        }

        iterator begin()
        {
            return shapes_.begin();
        }

        const_iterator begin() const
        {
            return shapes_.begin();
        }

        iterator end()
        {
            return shapes_.end();
        }

        const_iterator end() const
        {
            return shapes_.end();
        }

        size_t size() const
        {
            return shapes_.size();
        }
    };
}

#endif // SHAPEGROUP_HPP
