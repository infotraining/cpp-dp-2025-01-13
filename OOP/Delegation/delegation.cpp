#include <algorithm>
#include <array>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <variant>
#include <vector>

struct Color
{
    uint8_t r, g, b;
};

namespace Inheritance
{
    class TextParagraph
    {
        std::string text_;
        Color color_;

    public:
        TextParagraph(std::string text, Color color)
            : text_(std::move(text))
            , color_(std::move(color))
        {
        }

        const std::string& text() const
        {
            return text_;
        }

        virtual void render(size_t line_width) const
        {
            std::cout << "[" << text() << std::setw(line_width - text().length()) << std::right << "" << "]\n";
        }

        virtual ~TextParagraph() noexcept = default;

        // other methods...
    };

    class RightAlignedTextParagraph : public TextParagraph
    {
    public:
        using TextParagraph::TextParagraph;

        void render(size_t line_width) const override
        {
            std::cout << "[" << std::setw(line_width) << std::right << text() << "]\n";
        }
    };

    class CenteredAlignedTextParagraph : public TextParagraph
    {
    public:
        using TextParagraph::TextParagraph;

        void render(size_t line_width) const override
        {
            auto pad_left = (line_width - text().length()) / 2;
            auto pad_right = line_width - text().length() - pad_left;
            std::cout << "[" << std::setw(pad_left) << "" << text() << std::setw(pad_right) << "" << "]\n";
        }
    };
} // namespace Inheritance

namespace Delegation
{
    namespace Classic
    {
        class TextAlignment
        {
        public:
            virtual std::string aligned_text(const std::string& text, size_t line_width) const = 0;
            virtual ~TextAlignment() = default;
        };

        class LeftAlignment : public TextAlignment
        {
        public:
            std::string aligned_text(const std::string& text, size_t line_width) const override
            {
                std::stringstream out_str;
                out_str << text << std::setw(line_width - text.length()) << std::right << "";
                return out_str.str();
            }
        };

        class CenterAlignment : public TextAlignment
        {
        public:
            std::string aligned_text(const std::string& text, size_t line_width) const override
            {
                std::stringstream out_str;
                auto pad_left = (line_width - text.length()) / 2;
                auto pad_right = line_width - text.length() - pad_left;
                out_str << std::setw(pad_left) << "" << text << std::setw(pad_right) << "";
                return out_str.str();
            }
        };

        class RightAlignment : public TextAlignment
        {
        public:
            std::string aligned_text(const std::string& text, size_t line_width) const override
            {
                std::stringstream out_str;
                out_str << std::setw(line_width) << std::right << text;
                return out_str.str();
            };
        };

        class TextParagraph
        {
            std::string text_;
            Color color_;
            std::unique_ptr<TextAlignment> alignment_;

        public:
            TextParagraph(std::string text, Color color, std::unique_ptr<TextAlignment> alignment = std::make_unique<LeftAlignment>())
                : text_(std::move(text))
                , color_(std::move(color))
                , alignment_{std::move(alignment)}
            {
            }

            void set_alignment(std::unique_ptr<TextAlignment> alignment)
            {
                alignment_ = std::move(alignment);
            }

            void render(size_t line_width) const
            {
                std::cout << "[" << alignment_->aligned_text(text_, line_width) << "]\n";
            }
        };
    }

    // class TextAlignment
    // {
    // public:
    //     virtual std::string aligned_text(const std::string& text, size_t line_width) const = 0;
    //     virtual ~TextAlignment() = default;
    // };

    using TextAlignment = std::function<std::string(const std::string&, size_t)>;

    std::string left_aligned_text(const std::string& text, size_t line_width)
    {
        std::stringstream out_str;
        out_str << text << std::setw(line_width - text.length()) << std::right << "";
        return out_str.str();
    }

    struct CenterAlignedText
    {
        int counter = 0;

        std::string operator()(const std::string& text, size_t line_width)
        {
            ++counter;
            std::stringstream out_str;
            auto pad_left = (line_width - text.length()) / 2;
            auto pad_right = line_width - text.length() - pad_left;
            out_str << std::setw(pad_left) << "" << text << std::setw(pad_right) << "";
            return out_str.str();
        }
    };

    auto right_aligned_text = [](const std::string& text, size_t line_width)
    {
        std::stringstream out_str;
        out_str << std::setw(line_width) << std::right << text;
        return out_str.str();
    };

    std::string tabbed_text(const std::string& text, size_t line_width)
    {
        std::stringstream out_str;
        out_str << "\t" << text << std::setw(line_width - text.length()) << std::right << "";
        return out_str.str();
    }

    class TextParagraph
    {
        std::string text_;
        Color color_;
        TextAlignment alignment_;

    public:
        TextParagraph(std::string text, Color color, TextAlignment alignment = &left_aligned_text)
            : text_(std::move(text))
            , color_(std::move(color))
            , alignment_{std::move(alignment)}
        {
        }

        void set_alignment(TextAlignment alignment)
        {
            alignment_ = std::move(alignment);
        }

        void render(size_t line_width) const
        {
            std::cout << "[" << alignment_(text_, line_width) << "]\n";
        }
    };
} // namespace Delegation

namespace AlternativeWithEnum
{
    enum class Alignment
    {
        Left,
        Center,
        Right,
        Tabbed
    };

    class TextParagraph
    {
        std::string text_;
        Color color_;
        Alignment alignment_;

    public:
        TextParagraph(std::string text, Color color, Alignment alignment = Alignment::Left)
            : text_(std::move(text))
            , color_(std::move(color))
            , alignment_{std::move(alignment)}
        {
        }

        void set_alignment(Alignment alignment)
        {
            alignment_ = std::move(alignment);
        }

        void render(size_t line_width) const
        {
            switch (alignment_)
            {
            case Alignment::Left:
                std::cout << "[" << text_ << std::setw(line_width - text_.length()) << std::right << "" << "]\n";
                break;
            case Alignment::Center:
                {
                    auto pad_left = (line_width - text_.length()) / 2;
                    auto pad_right = line_width - text_.length() - pad_left;
                    std::cout << "[" << std::setw(pad_left) << "" << text_ << std::setw(pad_right) << "" << "]\n";
                }
                break;
            case Alignment::Right:
                std::cout << "[" << std::setw(line_width) << std::right << text_ << "]\n";
                break;
            case Alignment::Tabbed:
                std::cout << "[" << "\t" << text_ << std::setw(line_width - text_.length()) << std::right << "" << "]\n";
                break;
            }
        }
    };
}

// test comment
///////////

//
void use_inheritance()
{
    using namespace Inheritance;

    TextParagraph left_aligned{"This is sample of text...", Color{0, 0, 0}};
    RightAlignedTextParagraph right_aligned{"This is sample of text...", Color{0, 0, 0}};
    CenteredAlignedTextParagraph center_aligned{"This is sample of text...", Color{0, 0, 0}};

    left_aligned.render(80);
    right_aligned.render(80);
    center_aligned.render(80);
}

void use_delegation()
{
    using namespace Delegation;

    TextParagraph text{"This is sample of text...", Color{0, 0, 0}};
    text.render(80);

    text.set_alignment(Delegation::right_aligned_text);
    text.render(80);

    text.set_alignment(Delegation::CenterAlignedText{});
    text.render(80);
}

int main()
{
    use_inheritance();

    std::cout << "\n-----------------\n\n";

    use_delegation();
}