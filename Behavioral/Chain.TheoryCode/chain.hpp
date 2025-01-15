#ifndef CHAIN_HPP_
#define CHAIN_HPP_

#include <iostream>
#include <memory>
#include <string>

// "Handler"
class Handler
{
    std::shared_ptr<Handler> successor_;
public:
    Handler() : successor_{nullptr} {}

    void set_successor(std::shared_ptr<Handler> successor)
    {
        successor_ = successor;
    }

    void handle_request(int request)
    {
        bool is_accepted = false;

        if (can_handle(request))
            process_request(request, is_accepted);

        if (is_accepted)
            return;

        if (successor_ != nullptr)
            successor_->handle_request(request);
    }

    virtual ~Handler() = default;
protected:
    virtual bool can_handle(int request) = 0;
    virtual void process_request(int request, bool& is_accepted) = 0;
};

// "ConcreteHandler1"
class ConcreteHandler1 : public Handler
{
protected:
    bool can_handle(int request) override
    {
        return (request >= 0) && (request < 10);
    }

    void process_request(int request, bool& is_accepted) override
    {
        std::cout << "ConcreteHandler1 handled request " << request << std::endl;
    }
};

// "ConcreteHandler2"
class ConcreteHandler2 : public Handler
{
protected:
    bool can_handle(int request) override
    {
        return (request >= 10) && (request < 20);
    }

    void process_request(int request, bool& is_accepted) override
    {
        std::cout << "ConcreteHandler2 handled request " << request << std::endl;

        is_accepted = true;
    }
};

// "ConcreteHandler3"
class ConcreteHandler3 : public Handler
{
protected:
    bool can_handle(int request) override
    {
        return (request >= 20) && (request < 30);
    }

    void process_request(int request, bool& is_accepted) override
    {
        std::cout << "ConcreteHandler3 handled request " << request << std::endl;
    }
};

#endif /*CHAIN_HPP_*/
