import { useEffect, useState } from "react";
import './App.css';

export default function App() {
  const [tasks, setTasks] = useState([]);

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

  return (
    <div className="App">
      <header className="App-header">
        <h1>Todo List</h1>
        {tasks.length === 0 ? (
          <p>Tasks not found</p>
        ) : (
          <ul>
            {tasks.map(task => (
              <li key={task.id}>
                {task.title} {task.completed ? "✅" : "❌"}
                <button onClick={() => fetchTaskById(task.id)}>Refresh</button>
              </li>
            ))}
          </ul>
        )}
      </header>
    </div>
  );
}
