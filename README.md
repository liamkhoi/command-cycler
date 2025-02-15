# command-cycler

A simple and configurable command automation tool. The program simply reads and executes a sequence of commands from a configuration (`.ini`) file, providing configurable sequencing automation cycles, and also a mechanism to ensure safety on headless machines.

## What can you use this for

- Automate commands with your predefined list, **executed sequentially**
- Configurable cycles where you can set your own cooldown time between sequences
- **Headless-friendly** with a configurable exit key, features repeated key configuration as a safety mechanism
- Customizable .ini configuration.

## How a sequence is performed

1. The program reads the list of available variables of command strings from `variables.ini` under the `[Cycled commands]` section.
2. The commands are executed in the the variable number order.
3. After completing a cycle, it enters a cooldown phase.
4. The program continuously repeats the process unless being stopped.

## Example configuration (`variables.ini`)

**Note:** The program settings in this example is set to the default variables. If any values of these settings was not detected, the program would load a default configuration with the same value as this example. The program still works regardless of none commands.
```ini
[Program settings]
cooldown = 180
exitKeypresses = 3
exitKey = 'x'

[Cycled commands]
cmd1 = echo Hello, world!
cmd2 = systeminfo
#cmd3
#cmd4
#cmd5
#More commands can be written
```

## Usage

1. Download or compile the executable.
2. Place `variables.ini` in the same directory as the executable.
3. Run the program. You can stop with your predefined key sequence (for headless machines) or close the command window.

### Additionals:

- **Self-compiling:** Feel free to clone the repo and build yourself!
- You can add a **startup shortcut** and it does not require Windows login
- Please run the program as Administrator to perform administrator-required commands

## Other features

- There is a delayed console clearing for every cycle for a cooldown timer.
- The exit key mechanism ensures the program does not stop unintentionally (best use for headless machines!)

## Potential good use cases

- Automating backup commands (`rclone`, `robocopy`, etc.).
- Running periodic maintenance scripts.
- Executing remote tasks

## License

This project is licensed under the MIT License.

## Contribution

Feel free to contribute! It helps us learn more from this project.