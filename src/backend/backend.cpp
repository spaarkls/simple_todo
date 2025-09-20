#include "backend.h"

namespace {
    crow::response handle_get_all_task(TaskService& service) {
        try {
            std::vector<Task> tasks = service.get_all_tasks();
            crow::json::wvalue res;
            int index = 1;
            for (Task& t : tasks) {
                res["tasks"][index++] = t.to_json();
            }
            return crow::response(200, res.dump());
        
        } catch (pqxx::sql_error& e) {
            return crow::response(500, e.what());
        }
    }

    crow::response handle_get_task_by_id(TaskService& service, int id) {
        crow::json::wvalue res;
        try {
            Task task = service.get_task(id);
            res["task"] = task.to_json();
            return crow::response{200, res.dump()};

        } catch (std::runtime_error& e) {
            if (std::string(e.what()) == "Id must be greater than 0") {
                return crow::response{400, "id must be greater than 0"};

            } else if (std::string(e.what()) == "Has no value with such id") {
                return crow::response{404, "such id not exist"};
            }
            return crow::response{400, e.what()};

        } catch (pqxx::sql_error& e) {
            return crow::response(500, e.what());
        }
    }

    crow::response handle_add_task(TaskService& service, const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response{400, "Error request"};
        }
        
        try {
            std::string title = (body.has("title")) ? std::string(body["title"].s()) : "";
            bool completed = body.has("completed") ? body["completed"].b() : false;
            Task t(title, completed);
            service.add_task(t);
            return crow::response{201, crow::json::wvalue{
                {"id", t.get_id()},
                {"title", t.get_title()},
                {"completed", t.is_completed()}
            }};

        } catch (std::runtime_error& e) {
            return crow::response{400, e.what()};
        
        } catch (pqxx::sql_error& e) {
            return crow::response{500, e.what()};
        }
    }

    crow::response handle_get_task_by_title(TaskService& service, const std::string& title) {
        try {
            Task t = service.get_task_by_title(title);
            crow::json::wvalue res;
            res["task"] = t.to_json();
            return crow::response{200, res.dump()};

        } catch (std::runtime_error& e) {
            return crow::response{404, e.what()};
        
        } catch (pqxx::sql_error& e) {
            return crow::response{500, e.what()};
        }
    }

    crow::response handle_delete_task_by_id(TaskService& service, int id) {
        try {
            service.remove_task(id);
            return crow::response{204};

        } catch (std::runtime_error& e) {
            if (std::string(e.what()) == "Id must be greater than 0") {
                return crow::response{400, "id must be greater than 0"};

            } else if (std::string(e.what()) == "Has no value with such id") {
                return crow::response{404, "such id not exist"};
            }
            return crow::response{400, e.what()};

        } catch (pqxx::sql_error& e) {
            return crow::response(500, e.what());
        }
    }

    crow::response handle_update_task_by_id(TaskService& service, int id, const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response{400, "Error request"};
        }

        try {
            Task exist = service.get_task(id);
            std::string title = (body.has("title")) ? std::string(body["title"].s()) : exist.get_title();
            bool completed = body.has("completed") ? body["completed"].b() : exist.is_completed();
            Task t(id, title, completed);
            service.update_task(t);
            return crow::response{200, crow::json::wvalue{
                {"id", t.get_id()},
                {"title", t.get_title()},
                {"completed", t.is_completed()}
            }};

        } catch (std::runtime_error& e) {
            if (std::string(e.what()) == "Id must be greater than 0") {
                return crow::response{400, "id must be greater than 0"};

            } else if (std::string(e.what()) == "Has no value with such id") {
                return crow::response{404, "such id not exist"};
            }
            return crow::response{400, e.what()};

        } catch (pqxx::sql_error& e) {
            return crow::response(500, e.what());
        }
    }

}

void Backend::setup_routes(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/tasks").methods("GET"_method)
    ([this](){
        return handle_get_all_task(ts);
    });

    CROW_ROUTE(app, "/tasks/id/<int>").methods("GET"_method)
    ([this](int id) {
        return handle_get_task_by_id(ts, id);
    });

    CROW_ROUTE(app, "/tasks").methods("POST"_method)
    ([this](const crow::request& req) {
        return handle_add_task(ts, req);
    });

    CROW_ROUTE(app, "/tasks/title/<str>").methods("GET"_method)
    ([this](const std::string& title){
        return handle_get_task_by_title(ts, title);
    });

    CROW_ROUTE(app, "/tasks/id/<int>").methods("DELETE"_method)
    ([this](int id) {
        return handle_delete_task_by_id(ts, id);
    });

    CROW_ROUTE(app, "/tasks/id/<int>").methods("PUT"_method)
    ([this](const crow::request& req, int id) {
        return handle_update_task_by_id(this->ts, id, req);
    });
}

