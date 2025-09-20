#ifndef __TASK_H__
#define __TASK_H__
#include <iostream>
#include <string>
#include <crow/json.h>

class Task {
    int id;
    std::string title;
    bool completed;

public:
    Task(const std::string& title, bool done = false)
        : id(0), title(title), completed(done)
        { }
    
    Task(int id, const std::string& title, bool done)
        : id(id), title(title), completed(done)
        { }

    Task() : id(0), title(""), completed(false)
        { }

    inline int get_id(void) const noexcept { return id; }

    inline const std::string get_title(void) const noexcept { return title; }

    inline bool is_completed(void) const noexcept { return completed; }

    inline void set_title(const std::string& title) {
        this->title = title;
    }

    inline void set_completed(bool c) noexcept {
        completed = c;
    }

    inline void set_id(int id) noexcept {
        this->id = id;
    }

    inline crow::json::wvalue to_json(void) const {
        crow::json::wvalue json_data;
        json_data["id"] = id;
        json_data["title"] = title;
        json_data["completed"] = completed;
        return json_data;
    }

    static inline Task from_json(const crow::json::rvalue& value) {
        return Task(value["id"].i(), value["title"].s(), value["completed"].b());
    }

};  


#endif // __TASK_H__