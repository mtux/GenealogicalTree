
#include "prompthandler.h"

int main(int argc, char **argv)
{
    PromptHandler app(argc, argv);
    
    return app.EventLoop();
}
