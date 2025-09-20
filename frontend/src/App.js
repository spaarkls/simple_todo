import { useEffect, useState } from "react";
import './App.css';

export default function App() {
  const [tasks, setTasks] = useState([]);
  const [newTask, setNewTask] = useState("");

  useEffect(() => {
    fetch("/tasks")
      .then(res => res.json())
      .then(data => {
        if (data.tasks && Array.isArray(data.tasks)) {
          // отбрасываем null элементы
          setTasks(data.tasks.filter(task => task));
        } else {
          setTasks([]);
        }
      })
      .catch(err => console.error("Error find tasks", err));
  }, []);

  const fetchTaskById = (id) => {
    fetch(`/tasks/id/${id}`)
      .then(res => res.json())
      .then(task => {
        if (task && task.id) {
          setTasks(prev => {
            const filtered = prev.filter(t => t.id !== task.id);
            return [...filtered, task];
          });
        }
      })
      .catch(err => console.error(err));
  };

  const addTask = () => {
    if (!newTask.trim()) return;
    fetch("/tasks", {
      method: "POST",
      headers: {
        "Content-Type": "application/json"
      },
      body: JSON.stringify({ title: newTask.trim(), completed: false }),
    })
      .then(res => res.json())
      .then(task => {
        if (task && task.id) {
          setTasks(prev => [...prev, task]);
        } else {
          fetch("/tasks")
          .then(res => res.json())
          .then(all => {
            if (all.tasks && Array.isArray(all.tasks)) {
              setTasks(all.tasks.filter(t => t));
            }
          });
        }
        setNewTask("");
      })
      .catch(err => console.error("Error adding task", err));
  };
  
  const changeTask = (id, updatedFields) => {
    fetch(`/tasks/id/${id}`, {
      method: "PUT",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(updatedFields),
    })
    .then(res => res.json()) // теперь безопасно, сервер возвращает JSON
    .then(updatedTask => {
      if (updatedTask && updatedTask.id) {
        setTasks(prev =>
          prev.map(t => t.id === updatedTask.id ? updatedTask : t)
        );
      }
    })
    .catch(err => console.error("Error changing task", err));
  };

  const deleteTask = (id) => {
    fetch(`/tasks/id/${id}`, { method: "DELETE"})
    .then(res => {
      if (res.ok) {
        setTasks(prev => prev.filter(t => t.id !== id));
      }
    })
    .catch(err => console.error("Error deleting task", err));
  };
 
  return (
    <div className="App">
      <header className="App-header">
        <h1>Todo List</h1>
        <div style={{marginBottom: "1rem"}}>
          <input 
            type="text" 
            value={newTask} 
            onChange={e => setNewTask(e.target.value)} 
            placeholder="New task title" 
          />
          <button onClick={addTask}>Add Task</button>
        </div>
        {tasks.length === 0 ? (
          <p>Tasks not found</p>
        ) : (
          <ul>
            {tasks.map(task => (
              <li key={task.id}>
                {task.title} {task.completed ? "✅" : "❌"}
                <button onClick={() => changeTask(task.id, { completed: !task.completed })}>
                  {task.completed ? "Undo" : "Done"}
                </button>
                <button onClick={() => deleteTask(task.id)}>Delete Task</button>
              </li>
            ))}
          </ul>
        )}
      </header>
    </div>
  );
}
