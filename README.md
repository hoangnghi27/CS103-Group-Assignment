# I. Các từ khóa và gợi ý  
1. Dễ - 10 câu  
2. Trung bình - 10 câu  
3. Khó - 5 câu  

# II. File lưu trữ  
## 1. Từ khóa  
a. Đọc các từ khóa từ 3 ngân hàng
## 2. Người chơi  
a. Đọc tên   
b. Đọc các từ khóa đã chơi  
c. Lưu các từ khóa vừa chơi  
## 3. Scoreboard
a. Đọc tên  
b. Đọc điểm  
c. Lưu tên (nếu mới)  
d. Lưu điểm  

# III. Trò chơi  
## a. Bắt đầu  
### 1. Hỏi xem người chơi đã từng chơi chưa  
  * **Rồi**:
    * Hiện tên các người chơi trong file scoreboard kèm theo số thứ tự
    * Yêu cầu người chơi nhập số thứ tự của mình.  
  * **Chưa**:
    * Yêu cầu người chơi nhập tên
    * Kiểm tra xem tên vừa nhập có trùng với tên nào trong file điểm không
      * Nếu có thì báo lỗi và yêu cầu nhập lại
      * Nếu không
        * Khởi tạo file có tên của người chơi
        * Cập nhật file scoreboard:
          * Tăng tổng số người chơi
          * Lưu tên người chơi mới với số điểm 
  
Lưu tên và điểm người chơi vào biến `name` và `score`   
  
  
### 2. Chọn độ khó  
Lưu vào biến `dif` với các giá trị:
  * Dế: 1
  * TB: 2
  * Khó: 3  
  
  
### 3. In ra tên và điểm số hiện tại của người chơi  
  
  
---
## b. Chơi
### 1. Chọn từ khóa
  * **B1**: Xác định độ khó dựa trên biến `dif` để truy cập file ngân hàng tương ứng. Lưu các từ khóa của ngân hàng vào mảng `bank`  
  * **B2**: Đọc các từ khóa đã được chơi và lưu vào mảng `used_1`, `used_2`, `used_3` (tương ứng với từng độ khó). Lưu số lượng từ đã chơi vào biến `num_of_used_1`, `num_of_used_2`, `num_of_used_3`.  
  * **B3**: Nếu `num_of_used == kích_thước_ngân_hàng` (kích thước các ngân hàng được lưu vào các hằng số) thì thông báo đã chơi hết và thoát. Nếu nhỏ hơn thì tiếp tục chơi  
  * **B4**:
    * Chọn random một phần tử từ mảng `bank`
    * Duyệt qua mảng used, nếu không có từ nào trùng thì dùng, nếu có thì chọn lại  
    * Sau khi chọn được
      * Lưu vào mảng `used`
      * `num_of_used++`  
      * Lưu từ khóa vào file người chơi  
  
### 2. Mở từ khóa  
  * **B1**:
    * Lưu các kí tự của từ khóa vào biến `key` kiểu string và độ dài của từ khóa vào biến `key_len`  
    * Khởi tạo mảng `revealed` (lưu các kí tự đã mở) và `revealed_pos` (lưu các vị trí đã mở) với `key_len - 1` phần tử và các phần tử là kí tự `\0`  
    * Khởi tạo các biến (gán giá trị tương ứng độ khó):
      * `PNT`: điểm nếu đoán đúng và không lật
      * `PNT_A`: điểm bị trừ khi lật các kí tự cho phép
      * `PNT_B`: điểm bị trừ khi lật 1 kí tự cấm
      * `PNT_W`: điểm bị trừ khi đoán sai  
    * Khởi tạo các biến:
      * `num_of_revealed = 0`: số kí tự đã mở  
      * `revealing_pos`: vị trí của kí tự vừa mở  
      * `allowed`: số lần mở kí tự cho phép  
      * `banned`: số lần mở kí tự cấm  
      * `round_pnt = PNT`: điểm số có được sau vòng chơi  
      * `time_left`: dùng để đếm ngược thời gian  
      * `input`: nhận đáp án người dùng nhập vào  
  * **B2**: Clear màn hình. In ra tên, điểm, gợi ý và các dấu gạch dưới  
  * **B3**:
    * `allowed > 0`: Hỏi người chơi có muốn mở kí tự cho phép không  
      * *Có*:
        * `allowed--`  
        * `round_pnt -= PNT_A`  
      * *Không*:  Qua B7
     * `allowed < 0 && banned > 0`: Hỏi người chơi có muốn mở kí tự cấm không  
       * *Có*:
         * `banned--`  
         * `round_pnt -= PNT_B`  
       * *Không*:  Qua B7
     * `allowed < 0 && banned < 0`: Qua B7
  * **B4**:
    * Clear màn hình. In ra tên, điểm, gợi ý  
    * Chọn ngẫu nhiên một số từ `0` đến `key_len - 1`. Duyệt qua mảng `revealed_pos` xem số đó đã được chọn chưa. Nếu chưa thì lưu vào vị trí `num_of_revealed` và tăng biến đó lên 1 đơn vị. Nếu rồi thì chọn lại
    * Gán số vừa chọn vào biến `revealing_pos` và gán giá trị tại vị trí đó trong mảng `key` vào vị trí tương ứng trong mảng `revealed`:
 ```c
 revealing_pos = revealed_pos[num_of_revealed - 1];
 revealed[revealing_pos] = key[revealing_pos];
 ```
   * **B5**: Chạy vòng for duyệt qua mảng `revealed`, nếu gặp kí tự `\0` thì in ra `_`, ngoài ra thì in kí tự tương ứng  
   * **B6**:
     * `allowed > 0 || banned > 0`: Quay lại B3
     * `else`: Qua B7
   * **B7**: In ra "Ban vui long doan trong 10 giay". `time_left = 10`. Mỗi giây giảm 1, khi `== 0` thì qua B8  
   * **B8**: In ra "Ban vui long nhap dap an:". `time_left = 5`.
   * **B9**:
     * `time_left > 0 && input == key`:
       * `score += round_pnt`
       * In ra "Dap an cua ban chinh xac. Xin chuc mung ban"  
       * In ra "Ban vua duoc cong them `round_pnt` diem, so diem hien tai cua ban la `score`"
     * `else`: `score -= PNT_W`
       * `time_left == 0`: In ra "Rat tiec ban khong doan duoc o chu"
       * `input != key`: In ra "Dap an cua ban khong chinh xac, dap dung la ..." 
       *  In ra "Ban vua bi tru di `PNT_W` diem, so diem hien tai cua ban la `score`"
    * **B10**: Hỏi người chơi có muốn tiếp tục
      * *Có*: Clear màn hình. Quay lại **1. B1**
      * *Không*:
        * Cập nhật điểm của người chơi trong file scoreboard
        * Khởi tạo mảng `scoreboard` và gán các điểm số trong file scoreboard vào mảng này
        * Sắp xếp mảng `scoreboard` tăng dần. Lưu số phần tử của mảng vào biến `num_of_records`
        * Duyệt qua mảng `scoreboard` và xét xem `score` sẽ nằm ở vị trí nào. Lưu vào biến `pos` 
        * In ra "Tong diem cua ban la `score`, vi tri cua ban trong bang xep hang: `pos`/'num_of_records'. Chao ban, hen gap ban lan sau!"
