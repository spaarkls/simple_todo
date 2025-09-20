#include "task_service.h"

std::vector<Task> TaskService::get_all_tasks(void) {
    std::vector<Task> tasks = db.get_all_tasks();
    return tasks;
}

void TaskService::add_task(Task& t) {
    if (t.get_title().empty())    
        throw std::runtime_error("Task is empty!");

    try {
        Task exist = db.get_task_by_title(t.get_title());
        throw std::runtime_error("Task is exist!");

    } catch (const std::runtime_error& e) {
        if (std::string(e.what()) == "No task with such title!") {
            db.add_task(t);
        } else {
            throw;
        }
    }
}

Task TaskService::get_task(int id) {
    if (id <= 0) {
        throw std::runtime_error("Id must be greater than 0");
    }

    return db.get_task_by_id(id);
}

Task TaskService::get_task_by_title(const std::string& title) {
    if (title.empty()) {
        throw std::runtime_error("title must contains a symbols");
    }
    
    try {
        Task t = db.get_task_by_title(title);
        return t;
           
    } catch (std::runtime_error& e) {
        throw;
    }
}

void TaskService::update_task(Task& t) {
    if (t.get_id() <= 0) {
        throw std::runtime_error("Can not update task with id = 0");
    }
    Task exist_task = db.get_task_by_id(t.get_id());
    
    if (exist_task.get_title() != t.get_title()) {
        try {
            Task conflict = db.get_task_by_title(t.get_title());
            if (conflict.get_id() != t.get_id()) {
                throw std::runtime_error("Task with such title already exist");
            }

        } catch (std::runtime_error& e) {
            if (std::string(e.what()) != "No task with such title!") {
                throw;
            }
        }

    } 

    db.update_task(t);
}

void TaskService::remove_task(int id) {
    if (id <= 0) {
        throw std::runtime_error("Can not remove task with id = 0");
    }
    Task exist = db.get_task_by_id(id);
    db.remove_task(id);   
}

std::vector<Task> TaskService::get_tasks_by_status(bool completed) {
    return db.get_tasks_by_status(completed);
}

int TaskService::count_completed_task(void) {
    return db.count_completed_tasks();
}

int TaskService::count_active_task(void) {
    return db.count_tasks() - db.count_completed_tasks();
}

int TaskService::count_task(void) {
    return db.count_tasks();
}