#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Users.h"
#include "Message.h"
#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

using namespace std;

int main() {
    setlocale(LC_ALL, "");

    // ============ ЧТЕНИЕ ИЗ ФАЙЛОВ ============

// Чтение пользователей из файлов
//     
    //std::filesystem::current_path("F:\\Project_C++\\Модуль 18(Linux_Работа с файлами)\\Home_Work_18\\WorkSpace"); 
    std::filesystem::current_path("F:\\Project_C++\\Files");
    std::cout << "Current working directory: " << fs::current_path() << std::endl;  // Получить текущую рабочую директорию
    std::cout << "name.txt exists: " << (fs::exists("name.txt") ? "YES" : "NO") << std::endl;

    std::ifstream nameFile("name.txt");
    std::ifstream ageFile("age.txt");
    std::ifstream passwordFile("password.txt");

    // Устанавливаем права доступа только для владельца
    if (fs::exists("name.txt")) {
        fs::permissions("name.txt",
            fs::perms::group_all | fs::perms::others_all,
            fs::perm_options::remove);
    }
    if (fs::exists("age.txt")) {
        fs::permissions("age.txt",
            fs::perms::group_all | fs::perms::others_all,
            fs::perm_options::remove);
    }
    if (fs::exists("password.txt")) {
        fs::permissions("password.txt",
            fs::perms::group_all | fs::perms::others_all,
            fs::perm_options::remove);
    }

    if (nameFile.is_open() && ageFile.is_open() && passwordFile.is_open()) {
        //std::string name;
        //int age;
        //std::string password;

        //while (nameFile >> name && ageFile >> age && passwordFile >> password) {
        //    try {
        //        if (!users.userexists(name)) {
        //            users.adduser(name, password, age);
        //        }
        //    }
        //    catch (const std::exception& e) {
        //        std::cout << "ошибка при загрузке пользователя: " << e.what() << std::endl;
        //        continue;
        //    }
        //}

        nameFile.close();
        ageFile.close();
        passwordFile.close();
    }
    else {
        std::cout << "Файлы пользователей не найдены" << std::endl;
    }

    // Чтение сообщений из файлов
    std::ifstream senderFile("senderId.txt");
    std::ifstream receiverFile("receiverId.txt");
    std::ifstream contentFile("content.txt");

    // Устанавливаем права доступа только для владельца
    if (fs::exists("senderId.txt")) {
        fs::permissions("senderId.txt",
            fs::perms::group_all | fs::perms::others_all,
            fs::perm_options::remove);
    }
    if (fs::exists("receiverId.txt")) {
        fs::permissions("receiverId.txt",
            fs::perms::group_all | fs::perms::others_all,
            fs::perm_options::remove);
    }
    if (fs::exists("content.txt")) {
        fs::permissions("content.txt",
            fs::perms::group_all | fs::perms::others_all,
            fs::perm_options::remove);
    }

    if (senderFile.is_open() && receiverFile.is_open() && contentFile.is_open()) {
        int senderId;
        int receiverId;
        std::string content;

        while (senderFile >> senderId && receiverFile >> receiverId && contentFile >> content) {
            // Здесь нужно будет восстановить сообщение в MessageManager
            // messageManager.restoreMessage(senderId, receiverId, content);
        }

        senderFile.close();
        receiverFile.close();
        contentFile.close();
    }
    else {
        std::cout << "Файлы сообщений не найдены или не могут быть открыты" << std::endl;
    }

   
    cout << "==================================" << endl;
    cout << "Собираем базу пользователей" << endl;
    cout << "Введите количество пользователей:" << endl;
    int user_number;
    cin >> user_number;

    Users users;
    MessageManager messageManager;

    // Регистрация пользователей
    for (int i = 0; i < user_number; i++) {
        string user_name, user_password;

        cout << "Введите имя пользователя " << i + 1 << endl;
        cin >> user_name;
        cout << "Введите пароль " << i + 1 << endl;
        cin >> user_password;

        try {
            users.addUser(user_name, user_password);
            cout << "Пользователь " << user_name << " успешно зарегистрирован" << endl;
        }
        catch (const exception& e) {
            cout << "Ошибка: " << e.what() << endl;
            i--; // Повторяем ввод для этого пользователя
        }
    }

    // Основной цикл чата
    while (true) {
        cout << "==================================" << endl;
        cout << "Верификация пользователя" << endl;
        cout << "Логин (или 'exit' для выхода):" << endl;

        string current_user;
        cin >> current_user;

        if (current_user == "exit") {
            break;
        }

        if (!users.userExists(current_user)) {
            cout << "Пользователь не найден" << endl;
            continue;
        }

        cout << "Пароль:" << endl;
        string current_password;
        cin >> current_password;

        if (!users.checkPassword(current_user, current_password)) {
            cout << "Пароль неверный" << endl;
            continue;
        }

        int currentUserId = users.getUserId(current_user);
        cout << "Добро пожаловать, " << current_user << "!" << endl;

        // Показываем новые сообщения
        auto userMessages = messageManager.getMessagesForUser(currentUserId);
        if (!userMessages.empty()) {
            cout << "--- Новые сообщения ---" << endl;
            for (const auto& msg : userMessages) {
                string senderName = users.getUserName(msg.senderId);
                cout << "От " << senderName << ": " << msg.content << endl;
            }
            cout << "-----------------------" << endl;
        }
        else {
            cout << "Новых сообщений нет" << endl;
        }

        // Отправка сообщений
        cout << "Кому отправить сообщение (введите имя адресата или 'all' для групповой отправки, или 'back' для выхода):" << endl;
        string recipient;
        cin >> recipient;

        if (recipient == "back") {
            continue;
        }

        if (recipient == "all") {
            cout << "Введите сообщение для всех:" << endl;
            string message;
            cin.ignore(); // Очищаем буфер
            getline(cin, message);

            messageManager.broadcastMessage(currentUserId, message);
            cout << "Сообщение отправлено всем пользователям" << endl;
        }
        else {
            if (!users.userExists(recipient)) {
                cout << "Пользователь не найден" << endl;
                continue;
            }

            if (recipient == current_user) {
                cout << "Нельзя отправить сообщение самому себе" << endl;
                continue;
            }

            cout << "Введите сообщение для " << recipient << ":" << endl;
            string message;
            cin.ignore(); // Очищаем буфер
            getline(cin, message);

            int recipientId = users.getUserId(recipient);
            messageManager.sendMessage(currentUserId, recipientId, message);
            cout << "Сообщение отправлено пользователю " << recipient << endl;
        }
    }

    // ============ ЗАПИСЬ В ФАЙЛЫ ===========
   // Запись пользователей в файлы
    std::ofstream nameOut("name.txt");
    std::ofstream ageOut("age.txt");
    std::ofstream passwordOut("password.txt");

    if (nameOut.is_open() && ageOut.is_open() && passwordOut.is_open()) {
        // Для записи всех пользователей потребуется добавить метод в класс Users
        // Например: std::vector<User> allUsers = users.getAllUsers();
        // for (const auto& user : allUsers) {
        //     nameOut << user.name << " ";
        //     ageOut << user.age << " ";
        //     passwordOut << user.password << " ";
        // }

        nameOut.close();
        ageOut.close();
        passwordOut.close();

        // Устанавливаем права доступа только для владельца
        fs::permissions("name.txt",
            fs::perms::group_all | fs::perms::others_all,
            fs::perm_options::remove);
        fs::permissions("age.txt",
            fs::perms::group_all | fs::perms::others_all,
            fs::perm_options::remove);
        fs::permissions("password.txt",
            fs::perms::group_all | fs::perms::others_all,
            fs::perm_options::remove);
    }
    else {
        std::cout << "Не удалось открыть файлы для записи пользователей" << std::endl;
    }

    // Запись сообщений в файлы
    std::ofstream senderOut("senderId.txt");
    std::ofstream receiverOut("receiverId.txt");
    std::ofstream contentOut("content.txt");

    if (senderOut.is_open() && receiverOut.is_open() && contentOut.is_open()) {
        // Для записи всех сообщений потребуется добавить метод в класс MessageManager
        // Например: std::vector<Message> allMessages = messageManager.getAllMessages();
        // for (const auto& msg : allMessages) {
        //     senderOut << msg.senderId << " ";
        //     receiverOut << msg.receiverId << " ";
        //     contentOut << msg.content << " ";
        // }

        senderOut.close();
        receiverOut.close();
        contentOut.close();

        // Устанавливаем права доступа только для владельца
        fs::permissions("senderId.txt",
            fs::perms::group_all | fs::perms::others_all,
            fs::perm_options::remove);
        fs::permissions("receiverId.txt",
            fs::perms::group_all | fs::perms::others_all,
            fs::perm_options::remove);
        fs::permissions("content.txt",
            fs::perms::group_all | fs::perms::others_all,
            fs::perm_options::remove);
    }
    else {
        std::cout << "Не удалось открыть файлы для записи сообщений" << std::endl;
    }


    cout << "До свидания!" << endl;

    return 0;
}