#ifndef __TASK_SERVICE_H__
#define __TASK_SERVICE_H__
#include <optional>

#include "../database/database.h"

class TaskService {
    DataBase &db;

public:
    TaskService(DataBase& database) : db(database)
        { }

    std::vector<Task> get_all_tasks();
    Task get_task(int id);
    Task get_task_by_title(const std::string& title);
    void add_task(Task& task);
    void update_task(Task& task);
    void remove_task(int id);
    std::vector<Task> get_tasks_by_status(bool completed);

    int count_completed_task();
    int count_task();
    int count_active_task();
};

#endif // __TASK_SERVICE_H__