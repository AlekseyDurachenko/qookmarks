Описание полей сущности "Закладка"
==================================

1. Базовые поля
---------------

* Иконка сайта (favicon - QIcon)
* Заголовок ссылки (title - QString)
* Ссылка (url - QUrl)
* Описание (description - QString)



2. Организация иерархии
-----------------------

* Список тэгов (tags - QSet<CTag>)



3. Дополнительные поля
----------------------

* Комментарий (comment - QString)
* Ключевые слова (keywords - QStringList)
* Флаг: прочитать позже (readLater - bool)
* Флаг: избранное (favorite - bool)
* Флаг: удалено (deleted - bool) -- при установке данного флага, закладка должна помещаться в корзину.
* Оценка ссылки по десятибальной шкале (rating - int, 0 - not rated)



4. Стилистические свойства
--------------------------

* Шрифт (textFont - QFont)
* Цвет текста (textColor - QColor)
* Цвет фона (textBackgroundColor - QColor)



4. Метасвойства
---------------

* Дата создания закладки (createdDateTime - QDateTime)
* Дата последнего изменения закладки (editedDateTime - QDateTime)
* Дата последнего визита (lastVisitedDateTime - QDateTime)
* Общее количество посещений (visitCount - int)



5. Проверка страницы на работоспособность
-----------------------------------------

* HTTP ответ сервера (httpResponseCode - int, 0 - никогда не проверялось)
* HTTP текст ответа сервера (httpResponseText - QString)
* Дата последней проверки (lastCheckDateTime - QDateTime)



6. Скриншоты страницы сайта
---------------------------

* Необходимо реализовать сохранение скриншотов страниц сайтов, с возможностью просмотра их истори



7. Сохраненная копия страницы сайта
-----------------------------------

* Необходимо реализовать сохранение копии страницы сайтов, с возможностью просмотра их истори
