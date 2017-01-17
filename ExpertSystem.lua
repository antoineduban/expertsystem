#!lua

CodeName				= "Code"
CodeDir 				= CodeName.."/"
BinName					= "Bin"
BinDir					= BinName.."/"

RootName				= "ExpertSystem"
RootDir					= "./"
		
function DefineProject(ProjectName, ProjectDir, ProjectKind)
	
	local ProjectCodeDir = ProjectDir..CodeDir
	
	project (ProjectName)
		location (ProjectDir)
		
		configuration {}
			kind		(ProjectKind)
			language	"C++"
			
			targetdir	(RootDir..BinDir)
			
			includedirs	{ProjectCodeDir}

			files		{ProjectCodeDir.."**.h",
						 ProjectCodeDir.."**.hpp",
						 ProjectCodeDir.."**.c",
						 ProjectCodeDir.."**.cpp",
						 ProjectCodeDir.."**.tpp",
						 ProjectCodeDir.."**.inl"}

			vpaths		{["*"] = ProjectCodeDir}
			
			excludes	{ProjectCodeDir.."**/*.bak"}
			
		configuration {"Debug"}
			targetname		(ProjectName.."_d")
			flags			{"Symbols"}
			--buildoptions	{}
		configuration {"Release"}
			targetname		(ProjectName)
			flags			{"Optimize", "ExtraWarnings", "FatalWarnings"} -- Wextra Werror
			--buildoptions	{}
		
		configuration {}
end

solution (RootName)
	location (RootDir)
	configurations {"Debug", "Release"}
	configuration {"Debug"}
		defines	{"__FT_DEBUG__=1"}
	configuration {"Release"}
		defines	{"__FT_RELEASE__=1"}
	configuration {}
	
	DefineProject("ExpertSystem", RootDir, "ConsoleApp")