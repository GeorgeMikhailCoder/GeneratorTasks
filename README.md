# GeneratorTasks
 Project makes different variants of test from tasks given in the one file

## Применение
Программа генерирует варианты теста по входным заданиям. В папке с приложением есть файл BaseOfData, в него записываются вопросы (и ответы, при необходимости). Один вопрос занимает одну строчку. Вопросы разделены по блокам, программа при составлении теста будет брать по одному вопросу из блока, то есть в одном блоке следует писать равносильные вопросы. (если требуется взять вопросов больше, чем количество блоков, то по одинаковому количеству вопросов из блока)

## Параметры  
Количество строк - в одном варианте теста  
"Сохранить целостность блоков?" Если да, то при выборе нескольких вопросов из блока они будут расположены друг за другом. Если нет, то вопросы из одного блока будут раскиданы по тесту случайно.  
"Сохранить порядок блоков?" - возможно только при сохранении целостности. Если да, то блоки будут идти ровно в том же порядке, что и в BaseOfData. Если нет, то в случайном порядке.  

Важно: BafeOfData всугда должен заканчиваться пустой строкой.

## Пример работы:
BaseOfData:  
3^2=9  
3^3=27  
3^4=81  
3^5=243  
3^6=729  
  
4^2=16  
4^3=64  
4^4=256  
4^5=1024  
  
5^2=25  
5^3=125  
5^4=625  
  
  
Result:  
3^4=81  
4^2=16  
5^3=125  


