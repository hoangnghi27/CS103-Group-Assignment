#include <iostream>
#include <stdio.h>
#include <direct.h>
#include <fstream>
#include <string>
#include <cstring>
#include <windows.h>

//

using namespace std;

#define MAX_SIZE 200


ofstream writer;
ifstream reader;

string working_path()
{
    char temp[256];
    return (_getcwd(temp, sizeof(temp)) ? string(temp) : string(""));
}


typedef string STR_ARR[MAX_SIZE];
typedef int INT_ARR[MAX_SIZE];

struct BANK
{
    int size = 0;
    STR_ARR keys;
    STR_ARR hints;
};

struct PLAYER
{
    string name = "";
    int score = 50;

    BANK played_e, played_m, played_h;                      // Chứa các từ đã chơi cho mỗi mức độ
};

struct PLAYER_LIST
{
    PLAYER players[MAX_SIZE];
    int size;
};

string dir_bank_e = working_path() + "\\saves\\banks\\de.txt",               // File câu hỏi mức độ dễ
       dir_bank_m = working_path() + "\\saves\\banks\\tb.txt",               //                     trung bình
       dir_bank_h = working_path() + "\\saves\\banks\\kho.txt",              //                     khó
       dir_scrbrd = working_path() + "\\saves\\scoreboard.txt",              // File scoreboard
       dir_players_folder = working_path() + "\\saves\\players\\",           // Folder dữ liệu người chơi

       name,                                                // Tên người chơi
       key,                                                 // Từ khóa được chọn
       input;                                               // Kết quả của người chơi

int score,                                                  // Tổng điểm
    round_pnt,                                              // Số điểm có được sau vòng chơi
    dif,                                                    // Độ khó (1: Easy    2: Medium    3: Hard)
    
    key_len,                                                // Số chữ cái của từ khóa
    num_of_used_1, num_of_used_2, num_of_used_3,            // Sô từ khóa đã sử dụng

    PNT,                                                    // Điểm nếu đoán đúng và không lật
    PNT_A,                                                  // Điểm bị trừ khi lật các kí tự cho phép
    PNT_B,                                                  // Điểm bị trừ khi lật 1 kí tự cấm
    PNT_W,                                                  // Điểm bị trừ khi đoán sai

    num_of_revealed = 0,                                    // Số chữ cái đã được mở
    revealing_pos,                                          // Vị trí vừa được mở
    allowed,                                                // Số kí tự cho phép được mở
    banned,                                                 // Số lần mở kí tự cấm
    duration;                                               // Dùng cho Timer()

BANK bank_e, bank_m, bank_h,                                // Ngân hàng từ khóa
     used_e, used_m, used_h;                                // Các từ khóa đã chơi

PLAYER_LIST scrbrd;                                         // Scoreboard

PLAYER player;                                              // Người chơi

STR_ARR revealed;                                           // Các kí tự đã được mở

INT_ARR revealed_pos;                                       // Các vị trí đã được mở


void ReadKeys(), ReadScrbrdData(), ReadPlayerData(string, PLAYER &),
     UpdateScrbrdData(), UpdatePlayerData(string),
     CreateNewPlayerData(string), AddNewPlayer(PLAYER),
     AddPlayerToScrbrd(PLAYER), PrintScrbrd(bool), SortScrbrd(char),
    
     KeyPicker(), Printer(), Timer(),
    
     AskToReveal(), Revealer(),
    
     AnswerResult(), SessionResult();


int main()
{
    ReadKeys();
    ReadScrbrdData();

    cout << endl << "=============================================================";
    cout << endl << "|| CHAO MUNG BAN DEN VOI GAME MYWORDS (BY ITEC DEVELOPERS) ||";
    cout << endl << "=============================================================";
    cout << endl << endl << "Co " << bank_e.size << " o chu De, " << bank_m.size << " o chu Trung binh, " << bank_h.size << " o chu Kho da duoc load";
    


    if (scrbrd.size == 0)
    {
        cin.clear(); cin.ignore();
        cout << endl << endl << "Vui long nhap ten cua ban: ";
        getline(cin, player.name);

        AddNewPlayer(player);

        cout << "Da luu du lieu cua nguoi choi " << player.name << endl;
    }

    else
    {
        string option;
        cout << endl << endl << "Ban co muon tiep tuc choi? (Y/N): ";
        cin >> option;

        while (option != "y" && option != "Y" && option != "n" && option != "N")
        {
            cout << "Khong hop le. Vui long nhap lai: ";
            cin >> option;
        }

        if (option == "y" || option == "Y")
        {
            int id;

            cout << endl << "===== Danh sach nguoi choi da luu =====" << endl;
            PrintScrbrd(false);

            cout << endl << endl << "Nhap so thu tu cua ban: ";
            cin >> id;

            while (id < 1 || scrbrd.size < id)
            {
                cout << "Khong hop le. Vui long nhap lai: ";
                cin >> id;
            }

            ReadPlayerData(scrbrd.players[id - 1].name, player);
            cout << "Da doc du lieu cua nguoi choi " << player.name << endl;
        }

        else
        {
            cin.clear(); cin.ignore();
            cout << endl << endl << "Vui long nhap ten cua ban: ";
            getline(cin, player.name);

            int cur_scrbrd_size = scrbrd.size;

            for (int i = 0; i < cur_scrbrd_size; i++)
            {
                if (scrbrd.players[i].name == player.name)
                {
                    cout << endl << "Ten nguoi choi da co trong Scoreboard. Ban co muon tao moi? (Y/N): ";
                    cin >> option;

                    while (option != "y" && option != "Y" && option != "n" && option != "N")
                    {
                        cout << "Khong hop le. Vui long nhap lai: ";
                        cin >> option;
                    }

                    if (option == "y" || option == "Y")
                    {
                        string del_dir_s = dir_players_folder + player.name;
                        char del_dir[200];
                        strcpy_s(del_dir, (del_dir_s).c_str());

                        remove(del_dir);

                        CreateNewPlayerData(player.name);
                        ReadPlayerData(player.name, player);
                        scrbrd.players[i] = player;
                        UpdateScrbrdData();

                        cout << "Da luu moi du lieu cua nguoi choi " << player.name << endl;
                    }

                    else
                    {
                        ReadPlayerData(player.name, player);

                        cout << "Da doc du lieu cua nguoi choi " << player.name << endl;
                    }
                }

                else
                {
                    AddNewPlayer(player);

                    cout << "Da luu du lieu cua nguoi choi " << player.name << endl;
                }
            }
        }
    }

    cout << endl << "Chon do kho (1: De   2: Trung binh   3: Kho): ";
    cin >> dif;

    while (dif < 1 && 3 < dif)
    {
        cout << "Khong hop le. Vui long nhap lai: ";
        cin >> dif;
    }

    cout << endl;

    return 0;
}





void ReadKeys()
{
    string dir;


    dir = dir_bank_e;

    reader.open(dir);

    int counter = 0, elem_counter = 0;
    string content;

    if (reader.is_open())
    {
        while (getline(reader, content))
        {
            if (counter == 0)
            {
                bank_e.size = stoi(content);
                counter++;
            }

            else if (counter == 1)
            {
                bank_e.keys[elem_counter] = content;
                counter++;
            }

            else if (counter == 2)
            {
                bank_e.hints[elem_counter] = content;
                counter = 1;
                elem_counter++;
            }
        }

        reader.close();
    }

    else
        scrbrd.size = -1;


    dir = dir_bank_m;

    reader.open(dir);

    counter = 0, elem_counter = 0;

    if (reader.is_open())
    {
        while (getline(reader, content))
        {
            if (counter == 0)
            {
                bank_m.size = stoi(content);
                counter++;
            }

            else if (counter == 1)
            {
                bank_m.keys[elem_counter] = content;
                counter++;
            }

            else if (counter == 2)
            {
                bank_m.hints[elem_counter] = content;
                counter = 1;
                elem_counter++;
            }
        }

        reader.close();
    }

    else
        scrbrd.size = -1;


    dir = dir_bank_h;

    reader.open(dir);

    counter = 0, elem_counter = 0;

    if (reader.is_open())
    {
        while (getline(reader, content))
        {
            if (counter == 0)
            {
                bank_h.size = stoi(content);
                counter++;
            }

            else if (counter == 1)
            {
                bank_h.keys[elem_counter] = content;
                counter++;
            }

            else if (counter == 2)
            {
                bank_h.hints[elem_counter] = content;
                counter = 1;
                elem_counter++;
            }
        }

        reader.close();
    }

    else
        scrbrd.size = -1;
}


void ReadScrbrdData()
{
    reader.open(dir_scrbrd);

    int counter = 0, elem_counter = 0;
    string content;

    if (reader.is_open())
    {
        while (getline(reader, content))
        {
            if (counter == 0)
            {
                scrbrd.size = stoi(content);

                if (scrbrd.size == 0)
                    break;

                else counter++;
            }

            else if (counter == 1)
            {
                scrbrd.players[elem_counter].name = content;
                counter++;
            }

            else if (counter == 2)
            {
                scrbrd.players[elem_counter].score = stoi(content);
                counter = 1;
                elem_counter++;
            }
        }

        reader.close();
    }

    else
        scrbrd.size = -1;
}


void ReadPlayerData(string name, PLAYER &target)
{
    target.name = name;

    string dir = dir_players_folder + name + ".txt";


    reader.open(dir);

    int counter = 0, elem_counter = 0;
    string content;

    if (reader.is_open())
    {
        while (getline(reader, content))
        {
            if (counter == 0)
            {
                target.played_e.size = stoi(content);
                counter++;
            }

            else if (counter == 1)
            {
                for (; elem_counter < target.played_e.size; elem_counter++)
                    target.played_e.keys[elem_counter] = content;

                counter++;
                elem_counter = 0;
            }


            else if (counter == 2)
            {
                target.played_m.size = stoi(content);
                counter++;
            }

            else if (counter == 3)
            {
                for (; elem_counter < target.played_m.size; elem_counter++)
                    target.played_m.keys[elem_counter] = content;

                counter++;
                elem_counter = 0;
            }


            else if (counter == 4)
            {
                target.played_h.size = stoi(content);
                counter++;
            }

            else if (counter == 5)
            {
                for (; elem_counter < target.played_h.size; elem_counter++)
                    target.played_h.keys[elem_counter] = content;

                break;
            }
        }

        reader.close();
    }

    else
        scrbrd.size = -1;
}


void UpdateScrbrdData()
{
    //SortScrbrd('i');


    writer.open(dir_scrbrd);

    writer << scrbrd.size;

    for (int i = 0; i < scrbrd.size; i++)
        writer << "\n" << scrbrd.players[i].name << "\n" << scrbrd.players[i].score;

    writer.close();
}


void UpdatePlayerData(string name)
{
    writer.open(dir_players_folder + player.name + ".txt");


    writer << player.played_e.size;

    for (int i = 0; i < player.played_e.size; i++)
        writer << "\n" << player.played_e.keys[i];


    writer << "\n" << player.played_m.size;

    for (int i = 0; i < player.played_m.size; i++)
        writer << "\n" << player.played_m.keys[i];


    writer << "\n" << player.played_h.size;

    for (int i = 0; i < player.played_h.size; i++)
        writer << "\n" << player.played_h.keys[i];


    writer.close();
}


void CreateNewPlayerData(string name)
{
    string new_dir = dir_players_folder + name + ".txt";

    ofstream new_file(new_dir);

    PLAYER new_player;
    new_player.name = name;

    UpdatePlayerData(new_player.name);
}


void AddNewPlayer(PLAYER player)
{
    CreateNewPlayerData(player.name);
    ReadPlayerData(player.name, player);
    AddPlayerToScrbrd(player);
    UpdateScrbrdData();
}


void PrintScrbrd(bool with_scr)
{
    ReadScrbrdData();

    for (int i = 0; i < scrbrd.size; i++)
    {
        cout << endl << i + 1 << ". " << scrbrd.players[i].name;

        if (with_scr)
            cout << endl << scrbrd.players[i].score << " diem" << endl;
    }
}


void SortScrbrd(char mode)
{
    ReadScrbrdData();

    int cur_pos = 0;

    PLAYER_LIST sorted;

    sorted.size = scrbrd.size;
    sorted.size = scrbrd.size;

    for (int i = 0; i < sorted.size; i++)
        sorted.players[i] = scrbrd.players[i];

    switch (mode)
    {
        case 'i':                                                       // Increment
        {
            for (int i = 0; i < (sorted.size - 1); i++)
            {
                for (int j = i+1; j < sorted.size; j++)
                {
                    if (sorted.players[j].score > sorted.players[j + 1].score)
                    {
                        PLAYER temp = sorted.players[j];

                        sorted.players[j] = sorted.players[j + 1];
                        sorted.players[j + 1] = temp;
                    }
                }
            }

            break;
        }

        case 'd':                                                       // Decrement
        {
            for (int i = 0; i < (sorted.size - 1); i++)
            {
                for (int j = i + 1; j < sorted.size; j++)
                {
                    if (sorted.players[i].score < sorted.players[j].score)
                    {
                        PLAYER temp = sorted.players[i];

                        sorted.players[i] = sorted.players[j];
                        sorted.players[j] = temp;
                    }
                }
            }

            break;
        }
    }


    for (int i = 0; i < sorted.size; i++)
        scrbrd.players[i] = sorted.players[i];
}


void AddPlayerToScrbrd(PLAYER player)
{
    scrbrd.players[scrbrd.size] = player;
    scrbrd.size++;
}

void KeyPicker()
{
    //
}


void Printer()
{
    //
}


void Timer()
{
    //
}





void AskToReveal()
{
    //
}


void Revealer()
{
    //
}





void AnswerResult()
{
    //
}


void SessionResult()
{
    //
}