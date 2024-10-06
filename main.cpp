#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <windows.h>
#include <shellapi.h>
using namespace std;

/* ------------------------ Global Variables -------------------------- */
string path;
vector<string> imageExtensions = {".png", ".jpg", ".jpeg"};
vector<string> documentExtensions = {".pdf", ".docx", ".pptx", ".xlsx"};
vector<string> textExtensions = {".md", ".txt"};
vector<string> dataExtensions = {".dat", ".json", ".csv"};
vector<string> multimediaExtensions ={".mp3", ".ogg", ".wav", ".mp4", ".mov"};

map<string, string> directoryNamesHolder
{
    {"Images", "Imgs"},
    {"Documents", "Docs"},
    {"Text", "Ideas"},
    {"Data", "Data"},
    {"Multimedia", "Multimedia"},
    {"Other", "Others"}
};
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


/* ------------------------ Core Functions Prototypes -------------------------- */
void OrganizeDirectory(bool skipReadingFile);
void ModifySettings(bool defaultValues);
/* ------------------------ Core Functions Prototypes _Finished -------------------------- */


/* ------------------------ Core Functions -------------------------- */
void OrganizeDirectory(bool skipReadingFile)
{
    /* ------------------------ Reading Data -------------------------- */
    fstream csvFile("data.csv", ios::in);

    if(!csvFile.is_open())
    {
        ModifySettings(true); // Crate csv file and Modify settings with the default directory names
    }
    else
    {
        if(!skipReadingFile)
        {
            while(csvFile)
            {
                string length;
                getline(csvFile, length);

                string token;
                stringstream streamLength(length);
                int index = 1; // This index is used to check if the token is in the second raw or not
                string directoryNameValue;

                while(getline(streamLength, token, ','))
                {
                    if(index % 2 == 0) directoryNamesHolder[directoryNameValue] = token;
                    else directoryNameValue = token;
                    index++;
                    /* Simple Explination: We check if are in the first raw (odd number raw) we will save
                    the value of the token into the directory name value, which is the directoryNamesHolder.first
                    , Else if in the second raw (even number raw) we will assign the token value into the
                    directory name holder map with the key of the directory name value that we have assigned previously,
                    because the loop will always start with an odd number. :)
                    */
                }
            }
        }
        else
        {
            cout << "Skipped Reading File" << endl; /* DELETE THIS WHEN FINISH DEBUGGING */
        }
    }
    /* ------------------------ Reading Data _Finished_ -------------------------- */


    /* ------------------------ Asking user for the folder path -------------------------- */
    cin.ignore(1, '\n');
    cout << "Please enter the folder to be organized : ";
    getline(cin, path);
    /* ------------------------ Asking user for the folder path _Finished_-------------------------- */


    /* ------------------------ Iterate over the folder and check files extensions -------------------------- */
    cout << "Organizing the files..." << endl;

    for (const auto& filePath : filesystem::directory_iterator(path))
    {
        if(CheckExtension(filePath.path().extension(), imageExtensions))
        {
            CreateDirectory(directoryNamesHolder["Images"]);
            MoveFileTo(filePath, directoryNamesHolder["Images"]);
        }
        else if(CheckExtension(filePath.path().extension(), documentExtensions))
        {
            CreateDirectory(directoryNamesHolder["Documents"]);
            MoveFileTo(filePath, directoryNamesHolder["Documents"]);
        }
        else if(CheckExtension(filePath.path().extension(), textExtensions))
        {
            CreateDirectory(directoryNamesHolder["Text"]);
            MoveFileTo(filePath, directoryNamesHolder["Text"]);
        }
        else if(CheckExtension(filePath.path().extension(), dataExtensions))
        {
            CreateDirectory(directoryNamesHolder["Data"]);
            MoveFileTo(filePath, directoryNamesHolder["Data"]);
        }
        else if(CheckExtension(filePath.path().extension(), multimediaExtensions))
        {
            CreateDirectory(directoryNamesHolder["Multimedia"]);
            MoveFileTo(filePath, directoryNamesHolder["Multimedia"]);
        }
        else
        {
            CreateDirectory(directoryNamesHolder["Other"]);
            MoveFileTo(filePath, directoryNamesHolder["Other"]);
        }
    }
    /* ------------------------ Iterate over the folder and check files extensions _Finished_ -------------------------- */


    /* ------------------------ Success Message -------------------------- */
    cout << "Success..." << endl;
    ShellExecuteA(NULL, "open", path.c_str(), NULL, NULL, SW_SHOWMINIMIZED); /* Show Folder In Explorer */
     /* ------------------------ Success Message _Finished -------------------------- */
}

void ModifySettings(bool defaultValues)
{
    /* ------------------------ Taking directory names and save it in csv file -------------------------- */
    fstream csvFile("data.csv", ios::out);
    if(csvFile.is_open())
    {
        for(auto& directoryNameHolder : directoryNamesHolder)
        {
            string directoryNameValue;

            if(!defaultValues)
            {
                cout << "Enter your " << directoryNameHolder.first << " Directory's name: ";
                cin >> directoryNameValue;
                directoryNameHolder.second = directoryNameValue;
            }

            csvFile << directoryNameHolder.first << ',' << directoryNameHolder.second << endl;
        }
    }
    else
    {
        cout << "ERROR!\n";
        return;
    }
    /* ------------------------ Taking directory names and save it in csv file _Finished_ -------------------------- */

    if(!defaultValues) OrganizeDirectory(true);
    /* Note: I skipped reading the csv file when it assigns a user defined values because the directory names holder value
    are already assigned while writing the csv file
    */
}
/* ------------------------ Core Functions _Finished_ -------------------------- */

int main()
{
    /* ------------------------ Ask the user for the option to do -------------------------- */
    int option;
    cout << "Choose what you want to:" << endl;
    cout << "[0]: To modify settings" << endl;
    cout << "[1]: To organize directory" << endl;
    cout << "-> ";
    cin >> option;

    switch(option)
    {
        case 0: ModifySettings(false); break;
        case 1: OrganizeDirectory(false); break;
        default: cout << "Invaild Choice." << endl; break;
    }
    /* ------------------------ Ask the user for the option to do _Finished_ -------------------------- */
}