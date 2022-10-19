#!python
from distutils.command.build import build
import sys

env = DefaultEnvironment()

if sys.platform.startswith("linux"):
    host_platform = "linux"
elif sys.platform == "darwin":
    raise ValueError(
        "We do not support OSX for now :("
    )
elif sys.platform == "win32" or sys.platform == "msys":
    host_platform = "windows"
else:
    raise ValueError(
        "Could not detect platform automatically, please specify with "
        "platform=<platform>"
    )

opts = Variables([], ARGUMENTS)

opts.Add(EnumVariable('target', "Compilation target", 'debug', ['d', 'debug', 'r', 'release']))
opts.Add(EnumVariable('platform', "Compilation platform", '', ['', 'windows', 'x11', 'linux']))
opts.Add(EnumVariable('p', "Compilation target, alias for 'platform'", '', ['', 'windows', 'x11', 'linux']))
opts.Add(BoolVariable('use_llvm', "Use the LLVM / Clang compiler", 'no'))

build_path = "build/"
build_tests_path = "build-tests/"

includes_path = "include/"
sources_path = "src/"
tests_path = "tests/"

tests_target_path = "bin/"
tests_target_name = "run"

lib_target_path = "bin/"
lib_target_name = "dialog-flow"

opts.Update(env)

if env['use_llvm']:
    env['CC'] = 'clang'
    env['CXX'] = 'clang++'

if env['p'] != '':
    env['platform'] = env['p']

if env['platform'] == '':
    print("No valid target platform selected.")
    quit()

if env['platform'] == "osx":
    raise ValueError(
        "We do not support OSX for now :("
    )


elif env['platform'] in ('x11', 'linux'):
    pass #TODO


elif env['platform'] == "windows":
    if host_platform != "windows":
        print("""warning: Building for Windows on non-Windows platform.
            \rCross-compilation is NOT supported and this will most likely error-out.""")

    lib_target_path += 'win64/'
    tests_target_path += 'win64/'

    env.Append(CPPDEFINES=["WIN32", "_WIN32", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS"])
    env.Append(CCFLAGS=["-W3", "-GR"])
    
    if env["target"] == "debug":
        env.Append(CPPDEFINES=["_DEBUG"])
        env.Append(CCFLAGS=["-EHsc", "-MDd", "-ZI"])
        env.Append(LINKFLAGS=["-DEBUG"])

        lib_target_name += ".d"
    
    else:
        env.Append(CPPDEFINES=["NDEBUG"])
        env.Append(CCFLAGS=["-O2", "-EHsc", "-MD"])
    
    lib_target_name += ".dll"
    
    env.Append(CCFLAGS=["/std:c++14"])

env.Append(CPPPATH=[includes_path])

env.VariantDir('build', 'src', duplicate=0)
env.VariantDir('build-tests', 'tests', duplicate=0)

# Building library
sources = Glob(build_path + '*.cpp')

library = env.SharedLibrary(target=build_path + lib_target_path + lib_target_name, source=sources)
Default(library)

# Builing tests
sources = Glob(build_path + '*.cpp')
sources += Glob(build_tests_path + 'run.cpp')

application = env.Program(target=build_tests_path + tests_target_path + tests_target_name, source=sources)
Default(application)

Help(opts.GenerateHelpText(env))