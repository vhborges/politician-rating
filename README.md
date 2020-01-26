# Politician-rating
A CLI-based application aiming to satisfy a personal need to register, rate and later visualize all the politicians' relevant actions, as to guide my vote decisions. The app currently allows inserting new politicians, add ratings to existing ones (that increase or decrease their rating points), updating/deleting politicians and also supports various search options.
## Build dependencies
- GNU Make
- g++ with C++17 support
- [SQLite 3 library](https://www.sqlite.org/index.html)
- [Boost.Locale library](https://www.boost.org/doc/libs/1_72_0/libs/locale/doc/html/index.html)
## Run dependencies
- A Unix-based operating system
## Building
Inside the project's root, run:
```
make
```
## Installing
```
chmod +x install.sh
sudo ./install.sh
```
## Running
Print a help message with all available subcommands and options:
```
politician --help
```
Show all subcommands and options of a particular subcommand:
```
politician <subcommand> --help
```
**Note**: Arguments may accept texts with spaces, but they must be embraced by quotation marks.
<br><br>During execution the **database file** (and all unexisting parent folders) will be created inside `$HOME/.local/share/politician/`.
### Usage examples
Insert a new politician:
```
politician register -n <name> [-p <party>] [-i <information>]
```
**Note**: a politician without party is allowed (the party will be registered as "none"), but all politicians are univocally identified by the combination (Name + Party), this means that two politicians with the same name and without party are **not** allowed.
<br><br>
Insert a new rate to existing politician:
```
politician rate -n <name> [-p <party>] -r <rating points> [-d <rating description>]
```
**Note**: the rating points must be in the range [-5, 5].
<br><br>
Show all ratings belonging to a politician:
```
politician search ratings -n <name> [-p <party>]
```
<br>Show all politicians ordered by highest to lowest rating:
```
politician search all [-r] [-f]
```
**Note**: the flag `-r` inverts the order the politicians are shown by their rating points, and the flag `-f` shows the full version incluing information and rating points.
<br><br>
Search politicians by name:
```
politician search name <name>
```
<br>Search all politicians belonging to a party:
```
politician search party <party>
```
## Debugging
Inside the project's root, run:
```
make debug
gdb debug/politician
```
