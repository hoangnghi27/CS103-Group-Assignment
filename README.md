# Việc cần làm  

## I. Các từ khóa và gợi ý  
1. Dễ - 10 câu  
2. Trung bình - 10 câu  
3. Khó - 5 câu  

## II. Làm việc với file lưu trữ  
### 1. Từ khóa  
a. Đọc các từ khóa từ 3 ngân hàng
### 2. Người chơi  
a. Đọc tên   
b. Đọc các từ khóa đã chơi  
c. Lưu các từ khóa vừa chơi  
### 3. Scoreboard  
a. Đọc tên  
b. Đọc điểm  
c. Lưu tên (nếu mới)  
d. Lưu điểm  

## III. Trò chơi  
### a. Bắt đầu  
#### 1. Hỏi xem người chơi đã từng chơi chưa  
  * **Rồi**: Hiện tên các người chơi trong file scoreboard kèm theo số thứ tự. Yêu cầu người chơi nhập số thứ tự của mình.  
  * **Chưa**: Yêu cầu người chơi nhập tên. Kiểm tra xem tên vừa nhập có trùng với tên nào trong file điểm không. Nếu có thì báo lỗi và yêu cầu nhập lại. Nếu không thì lưu vào file điểm với số điểm là 50.  
  
Lưu tên và điểm người chơi vào biến `name` và `score`   
  
  
#### 2. Chọn độ khó  
Lưu vào biến `dif`  
  
  
#### 3. Hiện tên và điểm số hiện tại của người chơi  
  
  
  
  
### b. Chơi
#### 1. Chọn từ khóa
  * **B1**: Xác định độ khó dựa trên biến `dif` để truy cập file ngân hàng tương ứng. Lưu các từ khóa của ngân hàng vào mảng `bank`  
  * **B2**: Đọc các từ khóa đã được chơi và lưu vào mảng `used`. Lưu số lượng từ đã chơi vào biến `num_of_used`  
  * **B3**: Nếu `num_of_used` = kích thước ngân hàng thì thông báo đã chơi hết và thoát. Nếu nhỏ hơn thì tiếp tục chơi  
  * **B4**: Chọn random một phần tử từ mảng `bank`. Duyệt qua mảng used, nếu không có từ nào trùng thì dùng, nếu có thì chọn lại
  
#### 2. Lật
