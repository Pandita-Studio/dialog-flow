# DialogFlow

This project it's aiming to create a [Godot Engine](https://github.com/godotengine/godot) addon to run [Ren'Py](https://github.com/renpy/renpy)-style dialog scripts.

```
label start:
    scene "school_bg.png"
    show isabella at center with fade
    isabella "Hello boiz!"
    hide isabella with fade(0.5)
    jump end

label end:
    scene black
```

## Why?

Because we can :)

## Why not use another Godot dialog-scripting-tool?

There are some addons that already parses a very close renpy style dialog script but most of them are written in GDScript so you could imagine than that is not to much efficent, also some times those addons implements some logical and flow control blocks, like for's and if's and I think... why? you already have a powerful script language that you can use for that (GDSCript or even C++).

So, this projects aims to create an addon written in C++ (for high eficience) for renpy style dialog scripts parsing that implements only the dialog flow logic leaving complex logical operations (like conditionals, user inputs, animation and rendering) to GDScript. The comunication between dialog-flow logic and complex game logic will be done by Godot signals, for example:

```
label start:
    show isabella # this triggers a signal 'show(character_id)'
    isabella "Hello" # this triggers a signal 'say(who, what)'
```

So, dialog logic flows inside renpy style scripts and you handles all the complex stuff inside Godot handling the triggered signals.

## Dependencies

- [SCons](https://github.com/SCons/scons) as compilation tool.
- C++ Compiler ([Microsoft Visual Studio Build Tools](https://visualstudio.microsoft.com/es/downloads/?q=build+tools) or Visual Studio recomended on windows)

You can install SCons as a python package:

```
pip install scons
```

If you will use MinGW to complie add this to SConstruct file:

```
env['tools'] = 'mingw'
```

## Build

```bash
# 1. Clone the repo
git clone https://github.com/Pandita-Studio/dialog-flow.git

# Run scons
cd dialog-flow && scons platform=windows
```

This will compile shared libs under `build/bin` and tests binaries under `build-tests/bin`.

## ToDo

- Tokenizer is under development right now (I need to add more tokens that renpy scripts support)
- Make a Parser that takes the Tokenized script and generates the OpCodes three.
- Make a kind of Virtual Machine that takes de OpCodes three and run it. This VM also acts like and intermediate between dialog flow and Godot trowing the signals called by dialog script.
- After that we have to integrate the library as a godot addon ([like this](https://github.com/Pandita-Studio/godot-addons-cpp)).

## License

See LICENSE file: [GNU/GPL v3](./LICENSE)