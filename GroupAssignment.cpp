#include <iostream>
#include <stdio.h>
#include <direct.h>
#include <fstream>
#include <string>
#include <cstring>
#include <windows.h>

using namespace std;


ofstream writer;
ifstream reader;


struct BANK
{
    int size = 0;
    string keys[10] = {};
    string hints[10] = {};
};

struct PLAYER
{
    string name = "";
    int score = 50,
        dif = 1;

    BANK played = {};                                       // Chứa các từ đã chơi cho mức độ đã chọn
};

struct PLAYER_LIST
{
    PLAYER players[50] = {};
    int size = 0;
};


string working_path()
{
    char temp[261];
    return (_getcwd(temp, sizeof(temp)) ? string(temp) : string(""));
}

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

BANK bank_e = {}, bank_m = {}, bank_h = {},                 // Ngân hàng từ khóa của từng mức độ
     used_e = {}, used_m = {}, used_h = {};                 // Các từ khóa đã chơi của từng mức độ

PLAYER_LIST scrbrd = {};                                    // Scoreboard

PLAYER player;                                              // Người chơi

string revealed[30] = {};                                   // Các kí tự đã được mở

int revealed_pos[30] = {};                                  // Các vị trí đã được mở


void ReadKeys(), ReadScrbrdData(), ReadPlayerData(string, PLAYER &), ReadPlayerData(string, BANK &, BANK &, BANK &),
     UpdateScrbrdData(), UpdatePlayerData(PLAYER),
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

            player = scrbrd.players[id - 1];
        }

        else
        {
            cin.clear(); cin.ignore();
            cout << endl << endl << "Vui long nhap ten cua ban: ";
            getline(cin, player.name);

            int cur_scrbrd_size = scrbrd.size;
            bool existed = false;

            for (int i = 0; i < cur_scrbrd_size; i++)
            {
                if (scrbrd.players[i].name == player.name)
                {
                    existed = true;
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
                        scrbrd.players[i] = player;
                        UpdateScrbrdData();

                        cout << endl << "Da tao moi du lieu cua nguoi choi " << player.name << endl;

                        break;
                    }

                    else break;
                }
            }

            if (existed == false)
                AddNewPlayer(player);
        }
    }

    cout << endl << "Chon do kho (1: De   2: Trung binh   3: Kho): ";
    cin >> dif;

    while (dif < 1 && 3 < dif)
    {
        cout << "Khong hop le. Vui long nhap lai: ";
        cin >> dif;
    }

    player.dif = dif;

    ReadPlayerData(player.name, player);
    cout << "Da doc du lieu cua nguoi choi " << player.name << endl;

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
                if (target.dif == 1)
                    target.played.size = stoi(content);
                
                counter++;
            }

            else if (counter == 1)
            {
                for (; elem_counter < target.played.size; elem_counter++)
                {
                    if (target.dif == 1)
                        target.played.keys[elem_counter] = content;

                    else continue;
                }

                counter++;
                elem_counter = 0;
            }


            else if (counter == 2)
            {
                if (target.dif == 2)
                    target.played.size = stoi(content);

                counter++;
            }

            else if (counter == 3)
            {
                for (; elem_counter < target.played.size; elem_counter++)
                {
                    if (target.dif == 2)
                        target.played.keys[elem_counter] = content;

                    else continue;
                }

                counter++;
                elem_counter = 0;
            }


            else if (counter == 4)
            {
                if (target.dif == 3)
                    target.played.size = stoi(content);

                counter++;
            }

            else if (counter == 5)
            {
                for (; elem_counter < target.played.size; elem_counter++)
                {
                    if (target.dif == 3)
                        target.played.keys[elem_counter] = content;

                    else continue;
                }

                break;
            }
        }

        reader.close();
    }

    else
        scrbrd.size = -1;
}

void ReadPlayerData(string name, BANK &e, BANK &m, BANK &h)
{
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
                e.size = stoi(content);

                counter++;
            }

            else if (counter == 1)
            {
                for (; elem_counter < e.size; elem_counter++)
                    e.keys[elem_counter] = content;

                counter++;
                elem_counter = 0;
            }


            else if (counter == 2)
            {
                m.size = stoi(content);

                counter++;
            }

            else if (counter == 3)
            {
                for (; elem_counter < m.size; elem_counter++)
                    m.keys[elem_counter] = content;

                counter++;
                elem_counter = 0;
            }


            else if (counter == 0)
            {
                h.size = stoi(content);

                counter++;
            }

            else if (counter == 1)
            {
                for (; elem_counter < h.size; elem_counter++)
                    h.keys[elem_counter] = content;

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


void UpdatePlayerData(PLAYER player)
{
    BANK e, m, h;

    ReadPlayerData(player.name, e, m, h);


    writer.open(dir_players_folder + player.name + ".txt");


    writer << e.size;

    for (int i = 0; i < e.size; i++)
    {
        if (player.dif == 1)
            writer << "\n" << player.played.keys[i];

        else writer << "\n" << e.keys[i];
    }


    writer << "\n" << m.size;

    for (int i = 0; i < m.size; i++)
    {
        if (player.dif == 2)
            writer << "\n" << player.played.keys[i];

        else writer << "\n" << m.keys[i];
    }


    writer << "\n" << h.size;

    for (int i = 0; i < h.size; i++)
    {
        if (player.dif == 3)
            writer << "\n" << player.played.keys[i];

        else writer << "\n" << h.keys[i];
    }


    writer.close();
}


void CreateNewPlayerData(string name)
{
    string new_dir = dir_players_folder + name + ".txt";

    ofstream new_file(new_dir);

    PLAYER new_player;
    new_player.name = name;

    UpdatePlayerData(new_player);
}


void AddNewPlayer(PLAYER player)
{
    CreateNewPlayerData(player.name);
    ReadPlayerData(player.name, player);
    AddPlayerToScrbrd(player);
    UpdateScrbrdData();

    cout << endl << "Da tao moi du lieu cua nguoi choi " << player.name << endl;
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
    PLAYER temp;

    switch (mode)
    {
        case 'i':                                                       // Increment
        {
            for (int i = 0; i < (scrbrd.size - 1); i++)
            {
                for (int j = i + 1; j < scrbrd.size; j++)
                {
                    if (scrbrd.players[j].score > scrbrd.players[j + 1].score)
                    {
                        temp = scrbrd.players[j];

                        scrbrd.players[j] = scrbrd.players[j + 1];
                        scrbrd.players[j + 1] = temp;
                    }
                }
            }

            break;
        }

        case 'd':                                                       // Decrement
        {
            for (int i = 0; i < (scrbrd.size - 1); i++)
            {
                for (int j = i + 1; j < scrbrd.size; j++)
                {
                    if (scrbrd.players[i].score < scrbrd.players[j].score)
                    {
                        temp = scrbrd.players[i];

                        scrbrd.players[i] = scrbrd.players[j];
                        scrbrd.players[j] = temp;
                    }
                }
            }

            break;
        }
    }
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