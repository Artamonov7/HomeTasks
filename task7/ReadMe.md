Чат-сервер
Описание

Управление токенами

    read_tokens(const char *str) - разбор строки на токены с поддержкой кавычек
    add_to_list(struct token_list *tlist, char *str) - добавление токена в список
    delete_token_list(struct token_list *tlist) - освобождение памяти списка токенов
    copy_str(char *str, int size) - копирование строки заданного размера

Управление клиентами

    add_client(struct client_list **list, struct client *cl) - добавление клиента в список
    delete_client(struct client *cl) - удаление клиента и закрытие сокета
    find_client(struct chat_server *cs, const char *name) - поиск клиента по имени
    finish_sessions(struct client_list **list) - удаление завершивших сессию клиентов

Приватные имена

    add_to_private_names(struct client *cl, const char *name) - добавление в список приватных контактов
    delete_private_names(struct client *cl) - очистка списка приватных контактов

Сетевые операции

    send_string(struct client *cl, const char *str) - отправка строки клиенту
    broadcast_message(struct client_list *list, const char *mesg) - рассылка всем клиентам
    accept_client(struct chat_server *cs) - принятие нового подключения
    handle_client(struct chat_server *cs, struct client *cl) - обработка данных от клиента

Обработка сообщений и команд

    handle_string(struct chat_server *cs, struct client *cl, const char *str) - диспетчеризация входящих данных
    name_handling(struct chat_server *cs, struct client *cl, const char *str) - установка имени клиента
    message_handling(struct chat_server *cs, struct client *cl, const char *str) - обработка публичного сообщения

    command_users(struct chat_server *cs, struct client *cl) - список пользователей (/users)
    command_quit(struct chat_server *cs, struct client *cl, char **argv, int argc) - выход из чата (/quit)
    command_private(struct chat_server *cs, struct client *cl, char **argv, int argc) - приватное сообщение (/private)
    command_privates(struct client *cl) - список приватных контактов (/privates)
    command_help(struct client *cl) - справка по командам (/help)

Основной цикл и инициализация

    server_cycle(struct chat_server *cs) - главный цикл обработки событий (select)
    init_server(const char *ip, unsigned short port) - инициализация сервера
    destroy_server(struct chat_server *cs) - освобождение ресурсов сервера
    main(int argc, char **argv) - точка входа: запуск сервера с IP и портом

Команды клиента

    /users - список пользователей онлайн
    /quit "сообщение" - выход из чата с опциональным сообщением
    /private никнейм "сообщение" - приватное сообщение пользователю
    /privates - список пользователей, с которыми были приватные чаты
    /help - справка по командам
