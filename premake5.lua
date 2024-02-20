IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/ThirdParties/GLFW/include"
IncludeDir["ImGui"] = "%{wks.location}/ThirdParties/imgui"
IncludeDir["Glad"] = "%{wks.location}/ThirdParties/glad/include"

workspace "AN"
    architecture "x86_64"
    startproject "Editor"

    configurations
	{
		"Debug",
		"Release"
	}

    flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"

group "ThirdParties"
    include "ThirdParties/glfw"
    include "ThirdParties/imgui"
    include "ThirdParties/glad"
group ""

group "Engine"
    include "Engine"
group ""


group "Editor"
    include "Editor"
group ""