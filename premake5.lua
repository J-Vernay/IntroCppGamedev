workspace "IntroCppGamedev"

  -- Les plateformes correspondent à quel type de PC ou console.
  platforms { "x64-windows" } --, "x64-linux" }

  -- Les configurations changent la manière dont les fichiers C++ sont compilés,
  -- par exemple en changeant l'état du préprocesseur, en demandant des optimisations,
  -- ou l'ajout de checks de debug.
  configurations { "Debug", "DebugASAN", "Release" }

  -- Quels standards utiliser pour les langages de programmation.
  cdialect "C17"
  cppdialect "C++20"

  -- Où sont générés les fichiers dy système de build (MSBuild)
  location "build"

  -- Où sont générés les binaires (exécutables et bibliothèques natives)
  targetdir "build/%{cfg.platform}/%{cfg.buildcfg}"

  -- Les #include <truc> vont aussi chercher dans le dossier src.
  includedirs { "src" }
  
  -- On demande au compilateur de nous remonter ce qu'il trouve suspect,
  -- et de provoquer une erreur de compilation dès qu'il trouve quelque chose.
  warnings "Default"
  fatalwarnings { "All" }

  -- Génère des informations de débogage, qui vont servir aux debuggers et profilers.
  symbols "On"

  -- Particularités pour Windows 64-bits
  filter "platforms:x64-windows"
    architecture "x86_64"
    defines { "_CRT_SECURE_NO_WARNINGS" }
    incrementallink "Off"
    editandcontinue "Off"
    disablewarnings { "4267", "4244", "4305", "4838" } -- Conversion de nombres
	buildoptions { "/utf-8" }
  filter {}

  filter "platforms:x64-linux"
    architecture "x86_64"
    disablewarnings { "narrowing" } -- Conversion de nombres
  filter {}

  -- Particularités quand on veut se mettre en débogage.
  filter "configurations:Debug"
    optimize "Off"
  filter {}


  -- Particularités quand on veut se mettre en débogage.
  filter "configurations:DebugASAN"
	sanitize { "Address" }
    optimize "Off"
  filter {}

  -- Particularités quand on veut faire l'exe final, optimisé et sans outils de debug.
  filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"
  filter {}
  
  characterset "Unicode"

project "Documentation"

  -- Projet qui n'implique pas la compilation (ici, c'est la documentation).
  kind "Utility"
  -- Inclut tous les fichiers qui sont dans le dossier docs.
  files { "docs/*", "docs/cours/**", "README.md", "assets/CREDITS.md" }

  filter 'files:docs/Doxyfile'
    buildcommands {
      "{CHDIR} %[docs]%",
      "%[%{!wks.location}../tools/doxygen.exe]"
    }
    buildoutputs { "docs/html/index.html" }
  filter {}

  -- On change la date de dernière modification du fichier, de telle sorte
  -- que l'on relance toujours le prebuild quand on veut build ce projet.
  postbuildcommands {
    "{TOUCH} %[docs/Doxyfile]"
  }

project "JV"
  -- Compilation des bibliothèques tierces.
  kind "StaticLib"
  -- Compile tous les fichiers qui sont dans le dossier src
  files { "src/jv/common/*", "src/jv/*" }
  
  filter "platforms:x64-windows"
    -- Et sur Windows, on veut compiler les fichiers dans le dossier x64-windows
    files { "src/jv/x64-windows/*" }
    -- On veut également utiliser les API systèmes Windows
    links { "d3d11.lib", "d3dcompiler.lib", "dxguid.lib", "Xinput.lib" }
  filter {}
  
  -- Sur Linux, on veut compiler les fichiers correspondants, et on a des dépendances
  -- sur les bibliothèques système (notamment X pour la fenêtre et GL pour les graphismes)
  filter "platforms:x64-linux"
    files { "src/dino/x64-linux/*" }
    links { "GL", "X11", "Xi", "Xcursor", "dl", "pthread", "m" }
  filter {}

project "Exo1"
  -- On fait une application graphique (en opposition à une application console = purement texte)
  kind "WindowedApp"
  files { "src/exo1/*" }
  links { "JV" }
  
project "Exo2"
  kind "WindowedApp"
  files { "src/exo2/*" }
  links { "JV" }

project "Exo4"
  kind "WindowedApp"
  files { "src/exo4/*" }
  links { "JV" }

project "Exo5"
  kind "ConsoleApp"
  files { "src/exo5/*" }

project "Exo6"
  kind "ConsoleApp"
  files { "src/exo6/*" }
  
project "Dino"
  kind "WindowedApp"
  files { "src/dino/*" }
  links { "JV" }
  