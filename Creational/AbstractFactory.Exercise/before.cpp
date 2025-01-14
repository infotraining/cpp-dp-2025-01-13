#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

#define MOTIF 1

enum class IconType
{
    none,
    ok,
    cancel,
    warning,
    error
};

enum class Arch
{
    Motif = 0,
    Windows = 1
};

#define ARCH Arch::Motif


class Widget
{
public:
    virtual void draw() = 0;
    virtual ~Widget() = default;
};

class Button : public Widget
{
    std::string caption_;
    IconType icon_type_;

public:
    Button(const std::string &caption, IconType icon_type)
        : caption_{caption}, icon_type_{icon_type}
    {
    }

    std::string caption() const
    {
        return caption_;
    }

    IconType icon() const
    {
        return icon_type_;
    }
};

class Menu : public Widget
{
    std::string text_;

public:
    Menu(const std::string &text)
        : text_{text}
    {
    }

    std::string text() const
    {
        return text_;
    }
};

class MotifButton : public Button
{
public:
    using Button::Button;

    void draw() override
    {
        cout << "MotifButton [ " << caption() << " ]\n";
    }
};

class MotifMenu : public Menu
{
public:
    using Menu::Menu;

    void draw() override
    {
        cout << "MotifMenu { " << text() << " }\n";
    }
};

class WindowsButton : public Button
{
public:
    using Button::Button;

    void draw() override
    {
        cout << "WindowsButton [ " << caption() << " ]\n";
    }
};

class WindowsMenu : public Menu
{
public:
    using Menu::Menu;

    void draw() override
    {
        cout << "WindowsMenu { " << text() << " }\n";
    }
};

class Window
{
    std::vector<std::unique_ptr<Widget>> widgets;

public:
    void display() const
    {
        std::cout << "######################\n";
        for (const auto &w : widgets)
            w->draw();
        std::cout << "######################\n\n";
    }

    void add_widget(std::unique_ptr<Widget> widget)
    {
        widgets.push_back(std::move(widget));
    }
};

class IWidgetFactory
{
public:
    virtual std::unique_ptr<Button> CreateButton(const std::string &caption, IconType icon_type) = 0;
    virtual std::unique_ptr<Menu> CreateMenu(const std::string &text) = 0;
    virtual ~IWidgetFactory() = default;	
};

class MotifWidgetFactory : public IWidgetFactory
{
public:
    std::unique_ptr<Button> CreateButton(const std::string &caption, IconType icon_type)
    {
        return std::make_unique<MotifButton>(caption, icon_type);
    }

    std::unique_ptr<Menu> CreateMenu(const std::string &text)
    {
        return std::make_unique<MotifMenu>(text);
    }
};

class WindowsWidgetFactory : public IWidgetFactory
{
public:
    std::unique_ptr<Button> CreateButton(const std::string &caption, IconType icon_type)
    {
        return std::make_unique<WindowsButton>(caption, icon_type);
    }

    std::unique_ptr<Menu> CreateMenu(const std::string &text)
    {
        return std::make_unique<WindowsMenu>(text);
    }
};

class WindowOne : public Window
{

public:
    WindowOne(IWidgetFactory& widgetFactory)
    {
        add_widget(widgetFactory.CreateButton("OK", IconType::ok));
        add_widget(widgetFactory.CreateMenu("File"));
    }
};

class WindowTwo : public Window
{
public:
    WindowTwo(IWidgetFactory& widgetFactory)
    {
        add_widget(widgetFactory.CreateMenu("Edit"));
        add_widget(widgetFactory.CreateButton("OK", IconType::ok));
        add_widget(widgetFactory.CreateButton("Cancel", IconType::cancel));
    }
};

static std::unique_ptr<IWidgetFactory> CreateWidgetFactory(Arch type)
{
    switch (type)
    {
    case Arch::Motif:
        return std::make_unique<MotifWidgetFactory>();
        
    case Arch::Windows:
    default:
        return std::make_unique<WindowsWidgetFactory>();
    }
}

int main(void)
{
    std::unique_ptr<IWidgetFactory> widgetFactory = CreateWidgetFactory(ARCH);
    
    WindowOne w1{*widgetFactory};
    w1.display();

    WindowTwo w2{*widgetFactory};
    w2.display();
}
