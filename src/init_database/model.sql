create table tasks
( id BIGSERIAL primary key,
  title varchar not null,
  completed boolean default FALSE
);