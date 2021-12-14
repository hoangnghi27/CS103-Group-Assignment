#include <iostream>
#include <direct.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <cstring>
#include <windows.h>

using namespace std;


ofstream writer;
ifstream reader;

string working_path()
{
    char temp[261];
    return (_getcwd(temp, sizeof(temp)) ? string(temp) : string(""));
}

struct BANK
{
    int size = 0;
    string keys[10];
    string hints[10];
};

struct PLAYER
{
    string name = "";
    int score = 50;

    BANK played_e, played_m, played_h;                      // Chứa các từ đã chơi cho mỗi mức độ
};

struct PLAYER_LIST
{
    int size = 0;
    PLAYER players[50];
};

string dir_bank_e = working_path() + "\\saves\\banks\\de.txt",               // File câu hỏi mức độ dễ
       dir_bank_m = working_path() + "\\saves\\banks\\tb.txt",               //                     trung bình
       dir_bank_h = working_path() + "\\saves\\banks\\kho.txt",              //                     khó
       dir_scrbrd = working_path() + "\\saves\\scoreboard.txt",              // File scoreboard
       dir_players_folder = working_path() + "\\saves\\players\\",           // Folder dữ liệu người chơi

       name,                                                // Tên người chơi
       psswd,                                               // Mật khẩu người chơi
       key,                                                 // Từ khóa được chọn
       input;                                               // Kết quả người chơi nhập vào

int score,                                                  // Tổng điểm
    round_pnt,                                              // Số điểm có được sau vòng chơi
    dif,                                                    // Độ khó (1: Easy    2: Medium    3: Hard)

    PNT,                                                    // Điểm nếu đoán đúng và không lật
    PNT_A,                                                  // Điểm bị trừ khi lật các kí tự cho phép
    PNT_B,                                                  // Điểm bị trừ khi lật 1 kí tự cấm
    PNT_W,                                                  // Điểm bị trừ khi đoán sai

    num_of_revealed = 0,                                    // Số chữ cái đã được mở
    revealing_pos,                                          // Vị trí vừa được mở
    allowed,                                                // Số kí tự cho phép được mở
    banned,                                                 // Số kí tự cấm được phép mở
    used_banned = 0,                                        // Số lần mở kí tự cấm
    duration,                                               // Dùng cho Timer()
    time_left;                                              // Thời gian còn lại

BANK bank_e, bank_m, bank_h;                                // Ngân hàng từ khóa

PLAYER_LIST scrbrd;                                         // Scoreboard

PLAYER player;                                              // Người chơi
BANK played;                                                // Các từ khóa đã chơi của độ khó đã chọn

string revealed[30];                                        // Các kí tự đã được mở
int revealed_pos[30];                                       // Các vị trí đã được mở


string Encryptor(string);

bool ReadPlayerData(string);

void ReadKeys(), ReadScrbrdData(),
     UpdateScrbrdData(), UpdatePlayerData(string), UpdateSessionVars(),
     CreateNewPlayerDataFile(string), AddNewPlayer(PLAYER &, bool),
     AddPlayerToScrbrd(PLAYER), PrintScrbrd(bool), SortScrbrd(char),
     PlayerSelector(bool),
    
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
    cout << endl << "Co " << bank_e.size << " o chu De, " << bank_m.size << " o chu Trung binh, " << bank_h.size << " o chu Kho da duoc load";
    


    if (scrbrd.size == 0)
        AddNewPlayer(player, true);

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
            PlayerSelector(true);
            UpdateScrbrdData();
        }

        else
        {
            cin.clear(); cin.ignore();
            cout << endl << endl << "Vui long nhap ten cua ban (co phan biet hoa/thuong): ";
            getline(cin, player.name);

            bool existed = false;
            int cur_scrbrd_size = scrbrd.size;

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
                        cin.clear(); cin.ignore();
                        cout << "Mat khau (nhap 'quit' de thoat): ";
                        getline(cin, psswd);

                        if (psswd == "quit")
                        {
                            cout << endl << "Chao ban, hen gap lan sau!" << endl;
                            exit(0);
                        }

                        while (ReadPlayerData(player.name) == false)
                        {
                            cout << endl << "Mat khau khong chinh xac" << endl << "Vui long nhap lai (nhap 'quit' de thoat): ";
                            getline(cin, psswd);

                            if (psswd == "quit")
                            {
                                cout << endl << "Chao ban, hen gap lan sau!" << endl;
                                exit(0);
                            }

                        }

                        cout << endl << "Mat khau chinh xac. ";

                        PLAYER temp;
                        temp.name = player.name;
                        player = temp;

                        scrbrd.size -= 1;

                        AddNewPlayer(player, false);


                        break;
                    }

                    else
                    {
                        cin.clear(); cin.ignore();
                        cout << "Mat khau (nhap 'quit' de thoat): ";
                        getline(cin, psswd);

                        while (ReadPlayerData(player.name) == false)
                        {
                            cout << endl << "Mat khau khong chinh xac" << endl << "Vui long nhap lai (nhap 'quit' de thoat): ";
                            getline(cin, psswd);

                            if (psswd == "quit")
                            {
                                cout << endl << "Chao ban, hen gap lan sau!" << endl;
                                exit(0);
                            }

                        }

                        UpdateScrbrdData();

                        cout << endl << "Mat khau chinh xac. Du lieu cua nguoi choi " << player.name << " duoc doc thanh cong" << endl;

                        break;
                    }
                }
            }

            if (existed == false)
                AddNewPlayer(player, false);
        }
    }

    cout << endl << "Chon do kho (1: De   2: Trung binh   3: Kho): ";
    cin >> dif;

    while (dif < 1 || 3 < dif)
    {
        cout << "Khong hop le. Vui long nhap lai: ";
        cin >> dif;
    }

    UpdateSessionVars();

    cout << endl;

    return 0;
}





string Encryptor(string psswd)
{
    char key = 'X';
    string output = psswd;

    for (int i = 0; i < psswd.size(); i++)
        output[i] = psswd[i] ^ key;


    return output;
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

bool ReadPlayerData(string name)
{
    player.name = name;

    for (int i = 0; i < scrbrd.size; i++)
    {
        if (scrbrd.players[i].name == name)
            player.score = scrbrd.players[i].score;
    }


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
                if (Encryptor(content) != psswd)
                {
                    reader.close();
                    return false;
                }
                
                else
                    counter++;
            }

            else if (counter == 1)
            {
                player.played_e.size = stoi(content);
                counter++;
            }

            else if (counter == 2)
            {
                player.played_e.keys[elem_counter] = content;

                if (elem_counter == player.played_e.size - 1)
                {
                    counter++;
                    elem_counter = 0;
                }

                else elem_counter++;
            }


            else if (counter == 3)
            {
                player.played_m.size = stoi(content);
                counter++;
            }

            else if (counter == 4)
            {
                player.played_m.keys[elem_counter] = content;

                if (elem_counter == player.played_m.size - 1)
                {
                    counter++;
                    elem_counter = 0;
                }

                else elem_counter++;
            }


            else if (counter == 5)
            {
                player.played_h.size = stoi(content);
                counter++;
            }

            else if (counter == 6)
            {
                player.played_h.keys[elem_counter] = content;

                if (elem_counter == player.played_h.size - 1)
                    break;

                else elem_counter++;
            }
        }

        reader.close();
    }

    else
        scrbrd.size = -1;
    
    
    return true;
}


void UpdateScrbrdData()
{
    SortScrbrd('d');


    writer.open(dir_scrbrd);

    writer << scrbrd.size;

    for (int i = 0; i < scrbrd.size; i++)
        writer << "\n" << scrbrd.players[i].name << "\n" << scrbrd.players[i].score;

    writer.close();
}

void UpdatePlayerData(string name)
{
    writer.open(dir_players_folder + player.name + ".txt");

    writer << Encryptor(psswd);
    
    writer << "\n" << player.played_e.size;

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

void UpdateSessionVars()
{
    name = player.name;
    score = player.score;


    switch (dif)
    {
        case 1:
        {
            allowed = 1;
            banned = 1;
            PNT = 10;
            PNT_A = 5;
            PNT_B = 3;
            PNT_W = 2;

            for (int i = 0; i < player.played_e.size; i++)
                played.keys[i] = player.played_e.keys[i];

            break;
        }

        case 2:
        {
            allowed = 2;
            banned = 1;
            PNT = 15;
            PNT_A = 7;
            PNT_B = 5;
            PNT_W = 4;

            for (int i = 0; i < player.played_m.size; i++)
                played.keys[i] = player.played_m.keys[i];

            break;
        }

        case 3:
        {
            allowed = 3;
            banned = 2;
            PNT = 20;
            PNT_A = 10;
            PNT_B = 8;
            PNT_W = 6;

            for (int i = 0; i < player.played_h.size; i++)
                played.keys[i] = player.played_h.keys[i];

            break;
        }
    }
}


void CreateNewPlayerDataFile(string name)
{
    string new_dir = dir_players_folder + name + ".txt";

    ofstream new_file(new_dir);

    PLAYER new_player;
    new_player.name = name;

    UpdatePlayerData(new_player.name);
}

void AddNewPlayer(PLAYER &player, bool ask_name)
{
    if (ask_name)
    {
        cout << endl << endl << "Vui long nhap ten cua ban (co phan biet hoa/thuong): ";
        getline(cin, player.name);
    }

    cout << "Vui long tao mat khau (Luu y: Mat khau khi mat se khong the khoi phuc): ";
    cin.clear();
    getline(cin, psswd);

    while (psswd == "quit")
    {
        cout << "Mat khau khong hop le. Vui long chon mat khau khac: ";
        cin.clear();
        getline(cin, psswd);
    }

    CreateNewPlayerDataFile(player.name);
    AddPlayerToScrbrd(player);
    UpdateScrbrdData();

    cout << "Da luu du lieu cua nguoi choi " << player.name << endl;
}

void AddPlayerToScrbrd(PLAYER player)
{
    scrbrd.players[scrbrd.size] = player;
    scrbrd.size += 1;
}


void PrintScrbrd(bool with_scr)
{
    ReadScrbrdData();

    for (int i = 0; i < scrbrd.size; i++)
    {
        cout << endl << i + 1 << ". " << scrbrd.players[i].name;

        if (with_scr)
            cout << "\t\t\t" << scrbrd.players[i].score << " diem" << endl;
    }
}

void SortScrbrd(char mode)
{
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


void PlayerSelector(bool prompt)
{
    int id;

    if (prompt)
    {
        cout << endl << "===== Danh sach nguoi choi da luu =====" << endl;
        PrintScrbrd(true);
    }

    cout << endl << endl << "Nhap so thu tu cua ban (0 de thoat chuong trinh): ";
    cin >> id;

    while (id < 0 || scrbrd.size < id)
    {
        cout << "Khong hop le. Vui long nhap lai: ";
        cin >> id;
    }

    if (id == 0)
    {
        cout << endl << "Chao ban, hen gap lan sau!" << endl;
        exit(0);
    }

    cin.clear(); cin.ignore();
    cout << endl << "Mat khau: ";
    getline(cin, psswd);

    while (ReadPlayerData(scrbrd.players[id - 1].name) == false)
    {
        cout << endl << "Mat khau khong chinh xac" << endl << "Vui long nhap lai hoac nhan Enter de chon nguoi choi khac: ";
        getline(cin, psswd);

        if (psswd == "")
        {
            cout << endl << endl << "Nhap so thu tu cua ban (0 de thoat chuong trinh): ";
            cin >> id;

            while (id < 0 || scrbrd.size < id)
            {
                cout << "Khong hop le. Vui long nhap lai: ";
                cin >> id;
            }

            if (id == 0)
            {
                cout << endl << "Chao ban, hen gap lan sau!" << endl;
                exit(0);
            }


            cin.clear(); cin.ignore();
            cout << endl << "Mat khau: ";
            getline(cin, psswd);
            continue;
        }
    }

    cout << endl << "Mat khau chinh xac. Du lieu cua nguoi choi " << player.name << " duoc doc thanh cong" << endl;
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
    if (time_left > 0 && input == key)
    {
        score += round_pnt;
        cout << "Dap an cua ban chinh xac. Xin chuc mung ban" << endl;
        cout << "Ban vua duoc cong them " << round_pnt << ", diem hien tai cua ban la " << score;
    }

    else
    {
        score -= PNT_W;
        if (time_left == 0)
            cout << "Rat tiec ban khong hoan thanh duoc o chu";

        else if (input != key)
            cout << "Dap an cua ban khong chinh xac, dap an dung la " << key << endl;

        cout << "Ban vua bi tru di " << PNT_W << " diem, so diem hien tai cua ban la " << score;
    }
    
}


void SessionResult()
{
    UpdateScrbrdData();
    UpdatePlayerData(player.name);

    int pos;

    for (int i = 0; i < scrbrd.size; i++)
    {
        if (scrbrd.players[i].name == player.name)
        {
            pos = i;
            break;
        }
    }

    cout << "Tong diem cua ban la " << score << ", vi tri cua ban trong bang xep hang : " << pos + 1 << "/" << scrbrd.size << endl;
    cout << endl << "Chao ban, hen gap lan sau!";

    exit(0);
}