#include "bitmap.hpp"
#include <algorithm>
#include <array>

using namespace std;

struct Bitmap::BitmapImpl
{
    std::vector<uint8_t> image;
    
    BitmapImpl() = default;

    // pimpl interface...
};

Bitmap::Bitmap(size_t size, char fill_char) : pimpl_{std::make_unique<BitmapImpl>()}
{
}

Bitmap::~Bitmap() = default;

void Bitmap::draw()
{
    cout << "Image: ";
    for (size_t i = 0; i < pimpl_->image.size(); ++i)
        cout << pimpl_->image[i];
    cout << endl;
}
