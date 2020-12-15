#include "player.h"
#include <vector>
#include <dirent.h>
#include <sys/types.h>
#include <fstream>
#include <sstream>
#include <QApplication>
#include <unistd.h>
#define GetCurrentDir getcwd

#include <iostream>
using namespace std;

vector<string> videos;
vector<string> all_videos;
vector<int> points;
int files;
int rows;
string videoFolderPath = "/home/ja/.Wideo";
string dataFolderPath = "/home/ja/project/cpp/player/points";
//string videoFolderPath = "./video";
//string dataFolderPath = "data";

void list_dir(string d)
{
    const char* path = d.c_str();
    struct dirent* entry;
    DIR* dir = opendir(path);

    if (dir == NULL)
    {
        return;
    }
    files = 0;
    while((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] != '.')
        {
            string video = entry->d_name;
            videos.push_back(video);
            files++;
        }
    }
    closedir(dir);
}

void sort_videos()
{
    string line;
    ifstream file;
    file.open(dataFolderPath + "/videos.txt");
    while(getline(file, line))
    {
        if(line.find("points: ", 0) != string::npos)
        {
            size_t namePosition = line.find("name: ");
            string name = line.substr(namePosition + 6);
            for(int i = 0; i < videos.size(); i++)
            {
                if(videos[i] == name)
                {
                    bool advance = true;
                    for(int i = 0; i < all_videos.size(); i++)
                    {
                        if(name == all_videos[i])
                        {
                            advance = false;
                            break;
                        }
                    }
                    if(advance == true)
                    {
                        size_t endPosition = line.find(";");
                        int position = endPosition - 8;
                        string number = line.substr(8, position);
                        stringstream convert(number);
                        int point = 0;
                        convert >> point;

                        all_videos.push_back(name);
                        points.push_back(point);
                        break;
                    }
                }
            }
        }
    }
    file.close();
    for(int i = 0; i < videos.size(); i++)
    {
        file.open(dataFolderPath + "/videos.txt");
        bool foundVideo = false;
        while(getline(file, line))
        {
            if(line.find(videos[i], 0) != string::npos)
            {
                foundVideo = true;
                break;
            }
        }
        if(foundVideo != true)
        {
            all_videos.push_back(videos[i]);
            points.push_back(0);
        }
        file.close();
    }
}

void set_rows()
{
    if(files % 3 == 0)
    {
        rows = files / 3;
    }
    else
    {
        rows = (files / 3) + 1;
    }
    if(rows > 10)
    {
        rows = 10;
    }
    if(rows < 3)
    {
        rows = 3;
    }
}
string get_current_dir()
{
   char buff[FILENAME_MAX];
   GetCurrentDir( buff, FILENAME_MAX );
   string current_working_dir(buff);
   return current_working_dir;
}
int main(int argc, char *argv[])
{
    string dir = get_current_dir();
    //videoFolderPath = dir + "/" + videoFolderPath;
    list_dir(videoFolderPath);
    sort_videos();
    set_rows();
    QApplication a(argc, argv);
    player w;
    w.show();
    return a.exec();
}
