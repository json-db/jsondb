#include "crow.h"
//#include "crow_all.h"

int main()
{
    crow::SimpleApp app;

    //define your endpoint at the root directory
    CROW_ROUTE(app, "/")([](){
        auto page = crow::mustache::load_text("fancypage.html");
        return page;
    });

    app.port(18080).multithreaded().run();
}
