project "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

    outputdir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"


    targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/Intermediates/" .. outputdir .. "/%{prj.name}")

    files
    {
        "Source/**.h",
        "Source/**.cpp",
    }

    includedirs
	{
		"Source",
        "../Engine/Source"
    }

    links
	{
		"Engine"
	}