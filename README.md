# Philosophers

Philosophers is a program created as part of 42Firenze's core curriculum. This project focuses on implementing the hungry philosophers problem by using multi-threading and mutexes.

## Technologies

- **Programming Language:** the project was fully written in C.
- **Operating System:** Unix-based systems.

## Project Structure

- `philo/`: Contains the source code for the project.

## How to Use

Run the Makefile, launch the executable using 4-5 numeric arguments that will represent the parameters of the simulation.

- **Parameter 1 / number of philosophers:** Will determine the number of philosophers that will form the simulation.
- **Parameter 2 / time to die:** Will determine the ammount of time that will need to pass for a philosopher to die if it doesn't eat.
- **Parameter 3 / time to eat:** Will determine the ammount of time the philosopher takes to eat.
- **Parameter 4 / time to sleep:** Will determine the ammount of time the philosopher will sleep for after having eaten.
- **Parameter 5 / number of time the philosopher must eat:** Will determine the ammount of times the philosopher has to eat before they are sated.

Parameter 5 is optional but it will give the program a way to conclude, otherwise it will just go on forever or until a philosopher dies.
