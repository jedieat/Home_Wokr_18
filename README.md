Программа чата дополнена функциями чтения из текстовых файлов и записи в тесктовые файлы.
Новая Информация из файлов не обрабатывается и не записывается. Происходит только считывание файла, выдача пользователю прав для работы с файлом и запись этого же файла. Для того , чтобы писалась инофрмация из классов, необходима доработка чата.
Тектовые файлы для каждого объекта свои зарезервированы под объекты класоов Uzers и Message, названы не так как в задании, чтобы синхронизироваться с предыдущими проектами чата.
    
    Uzers
    std::ifstream nameFile("name.txt");
    std::ifstream ageFile("age.txt");
    std::ifstream passwordFile("password.txt");
    
    Message
    std::ifstream senderFile("senderId.txt");
    std::ifstream receiverFile("receiverId.txt");
    std::ifstream contentFile("content.txt");

    Потом будут предпеолагается их синхронизировать с данными контейнеров
    
    string name;
    string password;
    int age;
    
    int senderId;
    int receiverId; 
    string content;

 Чтение и запись реализованы в main в начале и в конце.
