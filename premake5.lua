Libpath = "C:/Libraries"

function getFolderName(path)
    local len = #path
    for i = len, 1,-1 do
        local char = path:sub(i,i)

        if (char == '\\') or (char == '/') then
            --print(path:sub(i + 1,len))
            return path:sub(i + 1,len)
        else
            --print(char)
        end
    end
    return path
end


function hasLibararyDir()
    if (not (os.isdir(Libpath)) or (not(os.isdir(Libpath .. "include"))) or (not(os.isdir(Libpath .. "lib")))) then
        return false
    end
    return true
end

function removeEndingSlash(path)
    local char = path:sub(#path,#path)

    if char == '\\' or char == '/' then
        path = path:sub(1,#path - 1)
    end
    return path
end

function copyFolder(src, dest)
    -- removes trailing '/' if any is present
    src = removeEndingSlash(src)
    dest = removeEndingSlash(dest)

    local srcFolderName = getFolderName(src)
    --gets a list of all the files in the sorce directory
    local files = os.matchfiles(src .. "/**")
    local folders = os.matchdirs(src .. "/**")
    
    -- creates all the subfolders inside the new Folder
    for _,folderpath in pairs(folders) do
        local newFolderPath = dest .. "/" .. srcFolderName .. "/" .. path.getrelative(src,folderpath)
        
        local ok, err = os.mkdir(newFolderPath)
        if not ok then
            error(err)
        end
    end

    for _,filename in pairs(files) do
        local copyFom = filename
        --  calculate destination
        local copyTo = dest .. "/" .. srcFolderName .. "/" .. path.getrelative(src,filename)

        print(copyFom .. " -> " .. copyTo)
        
        local ok, err = os.copyfile(copyFom, copyTo )
        if not ok then
            error(err)
        end
    end

end

function createLibraryDir()
    if not(hasLibararyDir()) then
        os.mkdir(Libpath .. "/include")
        os.mkdir(Libpath .. "/lib")
        return true
    end
    return false
end

function hasLibrary(name)
    if os.isdir(Libpath .. "/include/" .. string.lower(name)) or os.isdir(Libpath .. "include/" .. string.upper(name)) then
        return true
    end

    return false
end

function getLibrary(url, outerFolder)
    local filename = path.getname(url)

    print("Downloading: " .. filename)
    http.download(url,filename)

    -- if temp folder does not exists then create it
    if not(os.isdir("temp/")) then
        os.mkdir("temp/")
    end

    print("Extracting: " .. filename)
    zip.extract(filename,"temp/")

    local src
    --include folder
    if not(outerFolder == nil) then
        src = "temp/" .. outerFolder .. "/include/"
    else
        src = "temp/include/"
    end

    local dest = Libpath
    print(src .. " -> " .. dest)
    copyFolder(src,dest)

    --lib folder
    if not(outerFolder == nil) then
        src = "temp/" .. outerFolder .. "/lib/"
    else
        src = "temp/lib/"
    end
    dest = Libpath
    print(src .. " -> " .. dest)
    copyFolder(src,dest)

    print("Filename: " .. filename)
    os.chmod(tostring(filename),"755")
    os.remove(tostring(filename))
end

function deleteFile(path)

    --makes sure files are not read only
    if os.isfile(path) then
        os.chmod(tostring(path),"755")
        os.remove(tostring(path))
    else
        print("Failed to delete file" .. path)
    end
end

function deleteFolder(path)

    --makes sure that files are not read only
    local fileNames = os.matchfiles("temp/**")

    for _,file in pairs(fileNames) do
        os.chmod(tostring(file),"755")
    end

    --removes folder
    local ok, err = os.rmdir("temp/")
    if not ok then
        error(err)
    end
end

workspace "Journal"
    configurations "Release"
    cppdialect "c++23"
    platforms "Win64"

project "MyJournal"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin/%{cfg.buildcfg}"

    includedirs "C:/Libraries/include"
    libdirs "C:/Libraries/lib"

    files {"src/**.cpp","src/**.h"}

    filter "platforms:Win64"
        system "Windows"
        architecture "x86_64"
    
    filter "configurations:Release"
        defines {"NDEBUG","SFML_STATIC","TGUI_STATIC"}
        optimize "on"
        links { "sfml-graphics-s","sfml-window-s","sfml-system-s","tgui-s","opengl32","winmm","gdi32","freetype" }

local hasTempFolder = false
if not(hasLibararyDir()) then
    print(" -- Downloading and installing Libraries -- ")
end

if not(hasLibrary("SFML")) then
    print("Downloading and Installing SFML")
    getLibrary("https://www.sfml-dev.org/files/SFML-3.0.0-windows-vc17-64-bit.zip","SFML-3.0.0")
    print("SFML downloaded")
    hasTempFolder = true
else
    print("SFML is already installed")
end
    
if not(hasLibrary("TGUI")) then
    print("Downloading and Installing TGUI")
    getLibrary("https://github.com/texus/TUI/releases/download/v1.10.0/TGUI-1.10.0-vc17-64bit-for-SFML-3.0.0.zip","TGUI-1.10")
    print("TGUI downloaded")
    hasTempFolder = true
else
    print("TGUI is already installed")
end

if not(hasLibrary("nlohmann")) then
    print("Downloading and Installing nlohmann json")
    getLibrary("https://github.com/nlohmann/json/releases/download/v3.12.0/include.zip")
    print("nlohmann json downloaded")
    hasTempFolder = true
else
    print("nlohmann json is already installed")
end

if hasTempFolder then
    deleteFolder("temp/")
end