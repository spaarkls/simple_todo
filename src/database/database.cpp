#include "database.h"

std::vector<Task> DataBase::get_all_tasks(void) {
    std::vector<Task> tasks;
    try {
        pqxx::work w(conn);
        pqxx::result res =  w.exec("SELECT id, title, completed FROM tasks");
        for (const auto& row : res) {
            tasks.emplace_back(row[0].as<int>(), row[1].as<std::string>(), row[2].as<bool>());
        }

    } catch (pqxx::sql_error& e) {
        std::cerr << e.what() << std::endl;
    }
    return tasks;
}

void DataBase::add_task(Task& t) {
    try {
        pqxx::work w(conn);
        w.exec_params(
            "INSERT INTO tasks(title, completed) VALUES($1, $2)",
            t.get_title(), t.is_completed()
        );
        w.commit();

    } catch (pqxx::sql_error& e) {
        std::cerr << e.what() << std::endl;
    }
}

void DataBase::remove_task(int id) {
    try {
        pqxx::work w(conn);
        w.exec_params("DELETE FROM tasks WHERE id = $1", id);
        w.commit();
    } catch (pqxx::sql_error& e) {
        std::cerr << e.what() << std::endl;
    }
}

void DataBase::update_task(Task& t) {
    try {
        pqxx::work w(conn);
        w.exec_params(
            "UPDATE tasks SET title = $1, completed = $2 WHERE id = $3",
            t.get_title(), t.is_completed(), t.get_id()
        );
        w.commit();
    } catch (pqxx::sql_error& e) {
        std::cerr << e.what() << std::endl;
    }
}

Task DataBase::get_task_by_id(int id) {
    try {
        pqxx::work w(conn);
        pqxx::result res = w.exec_params("SELECT id, title, completed FROM tasks WHERE id = $1", id);
        if (res.empty()) {
            throw std::runtime_error("Has no value with such id");
        }
        const auto& row = res[0];
        return Task(row[0].as<int>(), row[1].as<std::string>(), row[2].as<bool>());

    } catch (pqxx::sql_error& e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

int DataBase::count_tasks() {
    try {
        pqxx::work w(conn);
        pqxx::result res = w.exec("SELECT COUNT(*) FROM tasks");
        const auto& row = res[0];
        return row[0].as<int>();

    } catch (pqxx::sql_error& e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

int DataBase::count_completed_tasks() {
    try {
        pqxx::work w(conn);
        pqxx::result res = w.exec("SELECT COUNT(*) FROM tasks WHERE completed = true");
        const auto& row = res[0];
        return row[0].as<int>();

    } catch (pqxx::sql_error& e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

std::vector<Task> DataBase::get_tasks_by_status(bool completed) {
    std::vector<Task> tasks;
    try {
        pqxx::work w(conn);
        pqxx::result res = w.exec_params("SELECT id, title, completed FROM tasks WHERE completed = $1", completed);
        for (const auto& row : res) {
            tasks.emplace_back(row[0].as<int>(), row[1].as<std::string>(), row[2].as<bool>());
        }

    } catch (pqxx::sql_error& e) {
        std::cerr << e.what() << std::endl;
    }
    return tasks;
}

Task DataBase::get_task_by_title(const std::string& title) {
    try {
        pqxx::work w(conn);
        pqxx::result res = w.exec_params("SELECT id, title, completed FROM tasks WHERE title = $1", title);
        if (res.empty()) {
            throw std::runtime_error("No task with such title!");
        }
        auto row = res[0];
        return Task(row[0].as<int>(), row[1].as<std::string>(), row[2].as<bool>());
        
    } catch (pqxx::sql_error& e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
}