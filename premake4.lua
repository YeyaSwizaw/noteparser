solution "NoteParser"
	configurations { "debug", "release" }

project "noteparse"
	kind "ConsoleApp"
	language "C++"
	files { "src/**" }
    links { "c++abi" }
	objdir "build/obj"
	buildoptions { "-std=c++11", "-stdlib=libc++", "-lc++abi" }

	configuration "debug"
		flags { "Symbols", "ExtraWarnings" }

	configuration "release"
		flags { "Optimize" }

