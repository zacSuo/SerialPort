# SerialPort
以C#的SerialPort類別為範例，配合自己在嵌入式Linux上的需要改良。
盡可能使用C++、少用C語言為原則。
用到C語言的低階操作也儘量封裝起來，保持物件使用的安全與簡潔。

參考文件https://msdn.microsoft.com/zh-tw/library/system.io.ports.serialport(v=vs.110).aspx

Device name和Baudrate不寫死，建構時傳入

使用C++ String Send/Recv，且可接受 Binary Data
