#ifndef __DATABASE_H__
#define __DATABASE_H__
#include <vector>
#include <string>
#include <pqxx/pqxx>
#include "../models/task.h"

class DataBase {
    pqxx::connection conn;
    
public:
    DataBase(const std::string& conn_str) : conn(conn_str)
        { }

    std::vector<Task> get_all_tasks(void);
    void add_task(Task& t);
    void remove_task(int id);
    void update_task(Task& t);
    Task get_task_by_title(const std::string& title);
    std::vector<Task> get_tasks_by_status(bool completed);
    int count_tasks();
    int count_completed_tasks();
    Task get_task_by_id(int id);
};

#endif // __DATABASE_H__