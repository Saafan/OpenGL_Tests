
$FileName = Read-Host -Prompt "What's your project's name? "

(Get-Content -Path 'OpenGL.sln') -creplace 'OpenGL', $FileName |  Out-File -encoding ASCII "OpenGL.sln"

Rename-Item -Path "OpenGL" -NewName $FileName

Rename-Item -Path "OpenGL.sln" -NewName "$FileName.sln"

Rename-Item -Path "$FileName\OpenGL.vcxproj" -NewName "$FileName.vcxproj"

Rename-Item -Path "$FileName\OpenGL.vcxproj.filters" -NewName "$FileName.vcxproj.filters"

(Get-Content -Path "$FileName\$FileName.vcxproj") -creplace 'OpenGL', $FileName |  Out-File -encoding ASCII "$FileName\$FileName.vcxproj"