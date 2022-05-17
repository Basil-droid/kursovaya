import pyodbc
import socket
import threading
import struct

class Sql:
    def __init__(self, database="ChatCentral", server="LAPTOP-NBQ8V6E2\SQLEXPRESS"):
        self.cnxn = pyodbc.connect("DRIVER={ODBC Driver 17 for SQL Server};Server="+server+";"+"Database="+database+";"+"Trusted_Connection=yes;")

    def Manual(self, query, response=False): #шаблон запроса
        cursor = self.cnxn.cursor()
        try:
            cursor.execute(query)
            if response:
                rows = cursor.fetchall()
                return rows
        except pyodbc.ProgrammingError as error:
            print("Ошибка:\n{}".format(error))
        self.cnxn.commit()

    def InsertUser(self, login, password, name): #добавление юзера
        row = self.Manual(f"""INSERT INTO [ChatCentral].[dbo].[Users] (login, password, name) VALUES ('{login}', '{password}', '{name}')""", False)
    def GetUserByID(self, id): #поиск юзера по id
        row = self.Manual(f"""select * from users where id = {id}""", True)
        return row
    def LoginUser(self, login, password):
        row = self.Manual(f"""select * from users where login='{login}' and password = '{password}'""", True)
        return row
    def CheckLogin(self, login):
        row = self.Manual(f"""select * from users where login='{login}'""", True)
        if row:
            return False
        else:
            return True
    def GetUserByLogin(self, login):
        row = self.Manual(f"""select * from users where login='{login}'""", True)
        return row
    def GetUsersDialogs(self, id):
        rows = self.Manual(f"""select * from dialog where dialog_one_user_id={id} or dialog_two_user_id={id}""", True)
        formatedRows=[]
        for row in rows:
            if row[1] == id:
                secondId = row[2]
            else:
                secondId = row[1]
            userinf = self.GetUserByID(secondId)
            formatedRows.append([secondId, userinf[0][3]])
        return formatedRows
    def GetUsersDialog(self, idToSend, idSendler):
        rows = self.Manual(f"""select * from ChatCentral.dbo.dialog where 
(dialog_one_user_id = {idToSend} and dialog_two_user_id = {idSendler}) or
(dialog_one_user_id = {idSendler} and dialog_two_user_id = {idToSend})""", True)
        formatedRows=[]
        for row in rows:
            if row[1] == idSendler:
                secondId = row[2]
            else:
                secondId = row[1]
            userinf = self.GetUserByID(secondId)
            formatedRows.append([secondId, userinf[0][3]])
        return formatedRows
    def GetUsersMessages(self, id):
        rows = self.Manual(f"""select * from [ChatCentral].[dbo].[message] where chat_messages_fk_user_id = {id}
or  chat_messages_fk_to_user_id = {id} """,True)
        return rows
    def GetIdOfDialog(self, id1, id2):
        row = self.Manual(f"""select * from [ChatCentral].[dbo].[dialog] where (dialog_one_user_id={id1} and dialog_two_user_id = {id2})
or (dialog_one_user_id={id2} and dialog_two_user_id = {id1})""", True)
        id = row[0][0]
        return id
    def InsertNewMessage(self, messInfo):
        idFrom = messInfo[0]
        idTo = messInfo[1]
        mess = messInfo[2]
        dlgId = self.GetIdOfDialog(idTo,idFrom)

        self.Manual(f"""insert into ChatCentral.dbo.message (chat_messages_text, chat_messages_fk_dialog_id,
chat_messages_fk_user_id, chat_messages_fk_to_user_id) values
('{mess}', {dlgId}, {idFrom}, {idTo})""")
    def InsertNewDlg(self, id1, id2):
        self.Manual(f"""insert into ChatCentral.dbo.dialog (dialog_one_user_id, dialog_two_user_id) values({id1}, {id2})""")




MAXCONNECTIONS = 1000
users = []

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

sock.bind(('127.0.0.1', 5005))

sock.listen(MAXCONNECTIONS)

sql = Sql()
def SendUserForDlg(rows, conn):
    for i in rows:
        id = struct.pack("q", i[0])
        Name = i[1].encode('utf16')
        msg=bytearray(512)
        msg[0]=3
        msg[1]=0
        startId = 2
        for chars in id:
            msg[startId] = chars
            startId+=1
        startName = 22
        for chars in Name:
            msg[startName]=chars
            startName+=1
        conn.send(msg)
def SendUserMessage(rows,conn):
    for row in rows:
        print(row)
        fromId = struct.pack("q", row[3])
        toId = struct.pack("q", row[4])
        mess = row[1].encode('utf16')
        msg = bytearray(512)
        msg[0] = 3
        msg[1] = 1
        startFromId=2
        for chars in fromId:
            msg[startFromId] = chars
            startFromId+=1
        startToId=22
        for chars in toId:
            msg[startToId] = chars
            startToId+=1
        startMess=42
        for chars in mess:
            msg[startMess] = chars
            startMess+=1
        conn.send(msg)



class MessageHandler(threading.Thread):
    def run(self):
        try:
            while True:
                print('------conn------')
                print(self.conn)
                print('--------end conn-------')
                data = self.conn.recv(512)#.decode('utf16').replace('\0', '')
                message = bytearray(512)
                command = data[0]
                subcommand1 = data[1]

                if command == 1:            #Авторизация
                    if subcommand1 == 0:
                        #При авторизации получаем логин и пароль
                        login = data[2:44].decode('utf16').replace('\ucccc', '')
                        password = data[44:86].decode('utf16').replace('\ucccc', '')
                        name = data[86:128].decode('utf16').replace('\ucccc', '')
                        if sql.CheckLogin(login):
                            sql.InsertUser(login,password,name)
                            message[0]=1
                            message[1]=0
                        else:
                            message[0] = 1
                            message[1] = 1
                        self.conn.send(message)
                if command == 2: # регистрация
                    if subcommand1 == 0:
                        login = data[2:44].decode('utf16').replace('\ucccc', '')
                        password = data[44:86].decode('utf16').replace('\ucccc', '')
                        usr = sql.LoginUser(login, password)
                        if usr:
                            self.id = usr[0][0]
                            message[0] = 2
                            message[1] = 0
                            self.conn.send(message)
                            SendUserForDlg(sql.GetUsersDialogs(self.id), self.conn)
                            SendUserMessage(sql.GetUsersMessages(self.id), self.conn)
                        else:
                            message[0] = 2
                            message[1] = 1
                            self.conn.send(message)
                if command == 3: #работа пользователя с другими пользователями
                    if subcommand1 == 0: # отправка сообщения пользователю
                        toId = struct.unpack("q",data[2:10])[0]
                        mess = data[22:422].decode('utf16').replace('\ucccc', '')
                        sql.InsertNewMessage([self.id, toId, mess])
                        for user in users:
                            if not user.is_alive():
                                users.remove(user)
                            if user.id == toId or user.id == self.id:
                                SendUserMessage([[0,mess, 0, self.id, toId]], user.conn)
                    if subcommand1 == 1: # поиск одним пользователем другого
                        login = data[2:42].decode('utf16').replace('\ucccc', '')
                        row = sql.GetUserByLogin(login)
                        if row:
                            message[0]=3
                            message[1]=2
                            self.conn.send(message)
                            sql.InsertNewDlg(row[0][0], self.id)
                            for user in users:
                                if not user.is_alive():
                                    users.remove(user)
                                if user.id == row[0][0]:
                                    SendUserForDlg(sql.GetUsersDialog(self.id, user.id), user.conn)
                                if user.id == self.id:
                                    SendUserForDlg(sql.GetUsersDialog(row[0][0], self.id), user.conn) # кароч тут ошибка завтра разберись, не добавляется в бд и не отправляется клиенту

                        else:
                            message[0]=3
                            message[1]=3
                            self.conn.send(message)
        except(ConnectionResetError):
            users.remove(self)
            return


while True:
    conn, addr = sock.accept()

    print('new accept')
    a = MessageHandler()
    a.conn = conn
    a.id = -1
    a.start()
    users.append(a)
