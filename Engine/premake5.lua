project "Engine"
	kind "StaticLib"
    language "C++"
	cppdialect "C++17"
    staticruntime "off"

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
        "%{IncludeDir.GLFW}",
    }

    links
    {
        "GLFW"
    }