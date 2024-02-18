

workspace "AN"
    architecture "x86_64"
    startproject "Hazelnut"

    configurations
	{
		"Debug",
		"Release"
	}

group "Engine"
    include "Engine"
group ""


group "Editor"
    include "Editor"
group ""