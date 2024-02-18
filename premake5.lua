IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/ThirdParties/GLFW/include"

workspace "AN"
    architecture "x86_64"
    startproject "Hazelnut"

    configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"

group "ThirdParties"
    include "ThirdParties/glfw"
group ""

group "Engine"
    include "Engine"
group ""


group "Editor"
    include "Editor"
group ""