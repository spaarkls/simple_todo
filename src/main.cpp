#include <iostream>
#include <string>
#include <crow.h>
#include <crow/middlewares/cors.h>

#include "database/database.h"
#include "TaskService/task_service.h"
#include "backend/backend.h"


int main(void) {
    std::string conn;
    conn += "dbname=" + std::string(std::getenv("DB_NAME")) + 
            " user=" + std::string(std::getenv("DB_USER")) +
            " password=" + std::string(std::getenv("DB_PASS")) + 
            " host=" + std::string(std::getenv("DB_HOST")) +
            " port=" + std::string(std::getenv("DB_PORT"));

    crow::App<crow::CORSHandler> app;
    auto &cors = app.get_middleware<crow::CORSHandler>();
    cors.global()
        .origin("*")
        .methods("GET"_method, "POST"_method, "DELETE"_method, "PUT"_method);
    std::cout << conn << std::endl;
    DataBase db(conn);
    TaskService ts(db);
    Backend back(ts);

    back.setup_routes(app);

    app.port(18080).multithreaded().run();
    
    return 0;
}