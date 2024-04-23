#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

using namespace std;
namespace fs = filesystem;

int main(int argc, char** argv)
{
    string commandStr = "msiexec /i ";
    string gaInstaller = "\"GLPI-Agent-1.7.2-x86.msi\"";
    string gaExtension = ".msi";
    string configFileName = "tag.txt";
    string inputStr = "";
    string tagStr = "";
    bool isGAInstallerExist = false;
    bool isTAGFileExist = false;

    fs::path currentDir = fs::current_path();

    for (const auto& entry : fs::directory_iterator(currentDir)) {
        if (fs::is_regular_file(entry.path())) {
            if (entry.path().extension() == gaExtension) {
                gaInstaller = entry.path().filename().string();
                cout << "GLPI Agent installer detected : " + gaInstaller << endl;
                isGAInstallerExist = true;
            }

            if (entry.path().filename() == configFileName) {
                ifstream file(entry.path());
                string line;
                while (getline(file, line)) {
                    tagStr = line;
                }
                tagStr.erase(remove(tagStr.begin(), tagStr.end(), '\n'), tagStr.cend());
                cout << "Tag file detected. TAG : " + tagStr << endl;
                isTAGFileExist = true;
                file.close();
            }
        }
    }

    if (!isGAInstallerExist) {
        cout << "Please copy past the GLPI Agent installer into the folder." << endl;
        cout << "Enter the GLPI Agent installer file name :";
        getline(cin, inputStr);
    }

    if (!isTAGFileExist) {
        cout << "Enter location Tag (press \"Enter\" to leave it blank):";
        getline(cin, tagStr);
    }

    if (inputStr.length() > 0) {
        gaInstaller = inputStr;
    }

    commandStr.append(gaInstaller);
    commandStr.append(" /norestart RUNNOW=1 ADDLOCAL=ALL ADD_FIREWALL_EXCEPTION=1 NO_SSL_CHECK=1 SERVER=\"https://helpdeskict.moh.gov.my/\"");

    tagStr.erase(remove(tagStr.begin(), tagStr.end(), '\n'), tagStr.cend());

    if (tagStr.length() > 0) {
        commandStr.append(" tag=\"" + tagStr + "\"");
    }

    cout << inputStr << endl;

    system(commandStr.c_str());
    return 0;
}