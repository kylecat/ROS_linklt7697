/*
   SBUS 測試程式(v1)
   用Serial1，在115200測試，8byte, even, 2 (別人的函式庫都是用100000，但7697無法這樣做)

   讀到的原始資料為：
   5E E2 02 13 B1 83 1E 74 00 02 20 00 00 00 1F 0B 33 9C B1 9F D2 EF F3 1E FF 43 1F 1F 03 03 FB 5E 43 9B CF 9F E8 11 1F F9 13 5E E2 02 13 B1 83 1E 74 00 02 20 00 00 00 1F 0B 33 98 B1 9F D2 EF F3 19 FF C2 1F F9 13 5E E2 02 13 B1 83 1E 74 00 02 20 00 00 00 1F 03 03 FB 5E 43 9B CF 1F E8 11 1F F9 13 5E E2
   其中1E 74 00 02 20 00 00 00 1F 為重複，但00 00 00 數量有時候會變(推測是雜訊)
   因此定位資料用0x20(結束) 及0x1f(開始)

   整理好的資料：
   [Modfy] 1F 13 B3 9C B1 9F D2 EF F3 1E FF 43 1F 1F 9B 58 E1 B2 9F D1 EF F3 1E FF 32 0F EB 4E 5E E2 02 13 B1 83 1E 74 00 02 20 00

   參考資料：
   1.Futaba SBUS：https://github.com/zendes/SBUS
   2.bolderflight SBUS：https://github.com/bolderflight/SBUS

*/

// 定位用的資料
int _d1c = 0x20; //結尾
int _d2c = 0x1F; //開頭

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(115200, SERIAL_8E2);
  Serial.println("初始化完成");
}


void loop() {

  int _buffer[100] = {0};  // 緩衝用
  int _data[50] = {0};     // 最後整理的資料

  if (Serial1.available()) {
    for (int _i = 0; _i < 100; _i++) {
      int rx = Serial1.read();
      _buffer[_i] = rx;
    }

    /* 檢查原始資料
      Serial.print("[Raw  ] ");
      show_data(_buffer,100);
    */

    int _show_count = modify_data(_buffer, _data);

    Serial.print(100 - _show_count);
    Serial.print("\t[Modfy] ");
    show_data(_data, 40);

  }

  delay(500);
}

/*
   把_buffer[100]的資料整理到_data[50]裡
*/
int modify_data(int* _buffer, int* _data)
{
  int _count = 100;

  // 整理buffer
  while ((_buffer[0] != _d1c) && (_buffer[4] != _d2c)) {

    _count --;
    if (_count < 0) break;

    for (int _i = 0; _i <  _count; _i++) {
      _buffer[_i] = _buffer[_i + 1];
    }
  }// end of while

  // 把buffer整理到data當中
  for (int _j = 0; _j <  _count; _j++) {
    _data[_j] = _buffer[_j + 4];
  }

  return _count;
}

/*
   把_data[50]裡的資料，按照給的_count長度印出來(HEX, 補0)
*/
void show_data(int* _data, int _count)
{
  for (int _i = 0; _i < _count; _i++) {
    if (_data[_i] < 16) Serial.print(0);
    Serial.print(_data[_i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}


