### QEagleScaler ###

QEagleScaler is a command line tool for scaling Eagle CAD files.<br>
Latest supported file format version is 8.0

QEagleScaler implements following functions:

1) Scale Eagle file up/down:

`QEagleScaler scale <source_file> <target_file> <factor>`

All PCB-related coordinates will be multiplied by `<factor>`:

`QEagleScaler scale mylibrary.lbr mylibrary.2x.lbr 0.5`<br>
`QEagleScaler scale myproject-2x.sch myproject-1x.sch 2`<br>
`QEagleScaler scale myproject-2x.brd myproject-1x.brd 2`<br>

2) Modify library extensions for scaled files:

`QEagleScaler libsuffix mylibrary.2x.lbr mylibrary.2x.lbr '' '.2x'`<br>
`QEagleScaler libsuffix myproject-1x.sch myproject-1x.sch '.2x' ''`<br>
`QEagleScaler libsuffix myproject-1x.brd myproject-1x.brd '.2x' ''`<br>

### Project maintenance ###

The workflow for a scaled-down EagleCAD project includes following steps:

1) Create scaled-down version of EagleCAD component libraries using scripts in `doc/automation`. All libraries must be in XML format with DTD version in range from 6.0 to 8.0. Put your libraries in `libs-original` directory and run `update-makefile` script, this script will create a *GNU Make* makefile for updating scaled versions of libraries, then run the `make` command.

2) Create an EagleCAD project only with scaled-down versions of libraries, **do not mix original and scaled versions**. Make sure that `[Globals]` section of `eagle.epf` does not contain unwanted libraries. If you're starting from an existing project, use `scale-down` script from `doc/automation/project-dir` to modify the project. Change the board name in script before using it.

3) Modify your DRC rules to scale and save them. See examples in `doc/automation/design-rules`.

4) Develop your project, use `update-makefile` from step (1) when you add new libraries and run `make` when you modify them.

5) When you're ready to produce GERBER files, use `scale-up` script from `doc/automation/project-dir`, load scaled-up project and load correct DRC rules. Change the board name in script before using it.

When switching scales, close project and unload all libraries e.g. by exiting EagleCAD, this will help to avoid mixing of libraries of different scales in one project.
