#include "crow.h"
//#include "crow_all.h"

int main()
{
    crow::SimpleApp app;

    //define your endpoint at the root directory
    CROW_ROUTE(app, "/<string>")([](std::string name){ // 
        auto page = crow::mustache::load("fancypage.html"); // 
        crow::mustache::context ctx ({{"person", name}}); // 
        return page.render(ctx); //
    });

    app.port(18080).multithreaded().run();
}