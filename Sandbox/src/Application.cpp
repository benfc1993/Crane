#include <Crane.h>

class Sandbox : public Crane::Application
{
public:
    Sandbox() {}
    ~Sandbox() {}
};

Crane::Application *Crane::CreateApplication()
{
    return new Sandbox();
}