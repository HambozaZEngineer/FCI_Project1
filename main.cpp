#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <vector>
using namespace std;

/* ------------------------ Global Variables -------------------------- */
string path;
vector<string> imageExtensions = {".png", ".jpg", ".jpeg"};
vector<string> documentExtensions = {".pdf", ".docx", ".pptx", ".xlsx"};
vector<string> textExtensions = {".md", ".txt"};
vector<string> dataExtensions = {".dat", ".json", ".csv"};
vector<string> multimediaExtensions ={".mp3", ".ogg", ".wav", ".mp4", ".mov"};
/* ------------------------ Global Variables _Finished_ -------------------------- */


/* ------------------------ Helpful Functions -------------------------- */
void CreateDirectory(string folderName)
{
    filesystem::create_directory(path + "//" + folderName);
}

bool CheckExtension(filesystem::path fileExtension, vector<string> extensionList)
{
    for(string extension : extensionList)
    {
        if(fileExtension == extension) return true;
    }

    return false;
}

void MoveFileTo(filesystem::path originalPath, string newPathDirectory)
{
    filesystem::path newPath = path + "\\" + newPathDirectory + "\\"; /* Getting the directory path */
    newPath += originalPath.filename();
    filesystem::copy(originalPath, newPath); /* Copy the file into the directory */
    filesystem::remove(originalPath); /* Remove the file in the original path */
    /* Note: Copying to the new directory and remove the old file gives an illusion of moving the file */
}
/* ------------------------ Helpful Functions _Finished_ -------------------------- */


int main()
{
    /* ------------------------ Asking user for the folder path -------------------------- */
    cout << "Please enter the folder to be organized : ";
    getline(cin, path);
    /* ------------------------ Asking user for the folder path _Finished_-------------------------- */


    /* ------------------------ Iterate over the folder and check files extensions -------------------------- */
    for (const auto& filePath : filesystem::directory_iterator(path))
    {
        if(CheckExtension(filePath.path().extension(), imageExtensions))
        {
            CreateDirectory("Imgs");
            MoveFileTo(filePath, "Imgs");
        }
        else if(CheckExtension(filePath.path().extension(), documentExtensions))
        {
            CreateDirectory("Docs");
            MoveFileTo(filePath, "Docs");
        }
        else if(CheckExtension(filePath.path().extension(), textExtensions))
        {
            CreateDirectory("Ideas");
            MoveFileTo(filePath, "Ideas");
        }
        else if(CheckExtension(filePath.path().extension(), dataExtensions))
        {
            CreateDirectory("Data");
            MoveFileTo(filePath, "Data");
        }
        else if(CheckExtension(filePath.path().extension(), multimediaExtensions))
        {
            CreateDirectory("Multimedia");
            MoveFileTo(filePath, "Multimedia");
        }
        else
        {
            CreateDirectory("Others");
            MoveFileTo(filePath, "Others");
        }
    }
    /* ------------------------ Iterate over the folder and check files extensions _Finished_ -------------------------- */
};