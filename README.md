# minishell

## Project Overview

Minishell is a project where my group partner @sonjasonjao and I built a functional shell from scratch. The goal was to replicate core Bash behavior, handling command execution, pipes, redirections and signals.

## My Approach

- Minishell was a tricky project to start with. My friend and I began studying a lot of material and testing how Bash actually behaves with different inputs. We then divided the work by filling a Kanban board with bite-sized tasks. Before diving in, we had a short knowledge-sharing session where I walked her through my yearlier project Minitalk (signals) and she walked me through Pipex (pipes). We decided I would focus on execution and she would focus on parsing and signals. A little bit counterintuitive on purpose, but we both wanted to learn something new along the way!
- Once we started building, things clicked surpsingly fast. Early on we aligned on the struct format for parsed input, which meant I could start working on execution independently with placeholder input. Clear ownership from the very start made the collaboration super smooth!

## Learnings

- Processes & Pipes: I gained much deeper understanding of how processes work, how pipes connect them and how to redirect STDIN and STDOUT.

- Bash knowledge: I had only used a handful of commands before this, so building a shell from scratch was very informative.

- Collaboration: This project was a great project when it came to the importance of dividing work clearly so both people have ownership and constantly know the status of tasks.

## Future Directions

I'm very happy with how this project turned out! If I were to revisit it, I'd focus on cleaning up the code as well as the folder structure to make it easier to read.

## How to Run the Project

- Clone the repository and compile using the provided Makefile. Then run ./minishell to start the shell.
