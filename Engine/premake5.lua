project "Engine"
	kind "StaticLib"
    language "C++"
	cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/Intermediates/" .. outputdir .. "/%{prj.name}")

    pchheader "anpch.h"
	pchsource "src/anpch.cpp"

    flags { "NoPCH" }

    files
    {
        "Source/**.h",
        "Source/**.cpp",
    }

    defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

    includedirs
	{
		"Source",
        "../ThirdParties/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.GLM}",
        "%{IncludeDir.entt}",
    }

    links
    {
        "GLFW",
        "ImGui",
        "Glad",
    }