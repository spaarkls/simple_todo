#ifndef __BACKEND_H__
#define __BACKEND_H__
#include <crow.h>
#include <crow/middlewares/cors.h>
#include "../TaskService/task_service.h"

class Backend {
    TaskService &ts;

public:
    Backend(TaskService& ts) : ts(ts)
        { }
    
    void setup_routes(crow::App<crow::CORSHandler>& app);
};

#endif // __BACKEND_H__