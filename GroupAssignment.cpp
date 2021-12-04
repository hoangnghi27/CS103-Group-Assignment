#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

#define MAX_SIZE 200


typedef string STRARRAY[MAX_SIZE];
typedef int INTARRAY[MAX_SIZE];


string name,                                                // Tên người chơi
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

STRARRAY bank,                                              // Ngân hàng từ khóa
         revealed,                                          // Các kí tự đã được mở
         used_1, used_2, used_3;                            // Các từ khóa đã chơi

INTARRAY revealed_pos;                                      // Các vị trí đã được mở


void ReadKeys(char), ReadScoreboardData(), ReadPlayerData(string),
     UpdateScoreboardData(string, int), UpdatePlayerData(string),
     CreateNewPlayerData(string),
    
     KeyPicker(), Printer(), Timer(),
    
     AskToReveal(), Revealer(),
    
     AnswerResult(), SessionResult();


int main()
{
    return 0;
}





void ReadKeys(char mode)
{
    //
}


void ReadScoreboardData()
{
    //
}


void ReadPlayerData(string name)
{
    //
}


void UpdateScoreboardData(string name, int score)
{
    //
}


void UpdatePlayerData(string name)
{
    //
}


void CreateNewPlayerData(string name)
{
    //
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