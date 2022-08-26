# ALLARMS-based-on-STM32-and-Arduino-Nano
Це проект безпекової системи для будинку, що побудований на базі МК STM32, яка виконує роль хаба та двох підрядних пристроїв в основі яких лежить Arduino Nano. Ви за бажанням можете продовжити цей проект та побудувати на базі нього повноцінну систему для охорони вашого маєтку.

В репозиторію ви можете побачити два файли: 
                                            1 - Motion_Cam_arduino
                                            2 - STM32_LoRa_RA_02
Ці файли містять в собі код для arduino nano та для STM32, вам не обовязково використовувати STM32f103c8t6 та arduino nano які використав я, ви можете використати інші мікроконтроллери.

В коді для STM32 є нелише той мінімум який необхідний для того, щоб працював алгоритм який я описав у відео. Там також є деякі функції які я писав для того, щоб можна було відправляти дані через хаб завдяки натисканню кнопки. Для прикладу, в файлі main.с є функція one_btn() завдяки якій ви зможете використовуючи лише одну тактову кнопку виконувати різні дії, тобто функція покликана замінити групу кнопок призначених для конкретних дій лише однією кнопкою, а результат буде залежати від кількості натискань.

Більше про бібліотеку для радіомодуля LoRa RA-02 та про його підключення до STM32 ви зможете знайти тут: https://github.com/SMotlaq/LoRa
Для Arduino Nano використовував бібліотеку LoRa.h.
                   
