import pymysql

def db_login(user, passwd, server_addr, dbname):
    try:
        db = pymysql.connect(server_addr, user, passwd, dbname)
    except pymysql.err.OperationalError:
        db = None
    return db

def db_showtable(db):
    cursor = db.cursor()
    cursor.execute("show tables")
    tabs = cursor.fetchall()
    res = list()
    for tab in tabs:
        cursor.execute("select count(*) from " + tab[0])
        row_cnt = cursor.fetchone()
        res.append((tab[0], row_cnt[0]))
    cursor.close()
    return res

def db_close(db):
    if db is not None:
        db.close()

def db_SearchCustomer(db):
    cursor = db.cursor()
    cursor.execute("select * from customer")
    tabs = cursor.fetchall()
    res = list()
    for tab in tabs:
        cusID = tab[0]
        cusname = tab[1]
        cusphone= tab[2]
        res.append((cusID, cusname, cusphone))
    cursor.close()
    return res

def db_InsertCustomer(db, cusID, cusname, cusphone, address, contact_phone, contact_name, contact_Email, relation):
    cursor = db.cursor()
    try:
        sql = "INSERT INTO customer VALUES('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', null, null)" %\
                (cusID, cusname, cusphone, address, contact_phone, contact_name, contact_Email, relation)
        cursor.execute(sql)
        db.commit()
    except:
        db.rollback()
    cursor.close()

def db_DeleteCustomer(db, cusID):
    cursor = db.cursor()
    try:
        sql = "DELETE FROM customer WHERE cusID = '%s'" % (cusID)
        cursor.execute(sql)
        db.commit()
    except:
        db.rollback()
    cursor.close()

def db_UpdateCustomer(db, cusID, cusname, cusphone, address, contact_phone, contact_name, contact_Email, relation):
    cursor = db.cursor()
    sql = "UPDATE customer SET cusname = '%s', cusphone = '%s', address = '%s', contact_phont = '%s', contact_name = '%s', contact_Email='%s', relation='%s' WHERE \
        cusID = '%s'" % (cusname, cusphone, address, contact_phone, contact_name, contact_Email, relation, cusID)
    cursor.execute(sql)
    db.commit()
    cursor.close()

def db_OpenSavingAcc(db, cusID, accID, money, settime, interest, saveType):
    cursor = db.cursor()
    try:
        sql = "INSERT INTO accounts VALUES('%s', %s, '%s', '储蓄账户')" %(accID, money, settime)
        cursor.execute(sql)
        sql = "INSERT INTO saveacc VALUES('%s', %s, '%s')" %(accID, interest, saveType)
        cursor.execute(sql)
        sql = "INSERT INTO cusforacc VALUES('%s', 'Liverpool', '%s', '%s', '储蓄账户')" %(accID, cusID, settime)
        cursor.execute(sql)
        db.commit()
    except:
        db.rollback()
    cursor.close()

def db_OpenCheckingAcc(db, cusID, accID, money, settime, overdraft):
    cursor = db.cursor()
    try:
        sql = "INSERT INTO accounts VALUES('%s', %s, '%s', '支票账户')" %(accID, money, settime)
        cursor.execute(sql)
        sql = "INSERT INTO checkacc VALUES('%s', %s )" %(accID, overdraft)
        cursor.execute(sql)
        sql = "INSERT INTO cusforacc VALUES('%s', 'Liverpool', '%s', '%s', '支票账户')" %(accID, cusID, settime)
        cursor.execute(sql)
        db.commit()
    except:
        db.rollback()
    cursor.close()

def db_SearchAcc(db):
    cursor = db.cursor()
    cursor.execute("select accounts.accountID, cusforacc.cusID, accounts.accountType from accounts, cusforacc WHERE accounts.accountID = cusforacc.accountID")
    tabs = cursor.fetchall()
    res = list()
    for tab in tabs:
        accID = tab[0]
        cusID = tab[1]
        accType= tab[2]
        res.append((accID, cusID, accType))
    cursor.close()
    return res

def db_DeleteAcc(db, accID):
    cursor = db.cursor()
    try:
        sql = "DELETE FROM saveacc WHERE accountID = '%s'" %(accID)
        cursor.execute(sql)
        sql = "DELETE FROM checkacc WHERE accountID = '%s'" %(accID)
        cursor.execute(sql)
        sql = "DELETE FROM cusforacc WHERE accountID = '%s'" %(accID)
        cursor.execute(sql)
        sql = "DELETE FROM accounts WHERE accountID = '%s'" %(accID)
        cursor.execute(sql)
        db.commit()
    except:
        db.rollback()
    cursor.close()

def db_UpdateAcc(db, accID, money, interest, overdraft):
    cursor = db.cursor()
    cursor.execute("SELECT accountType from accounts WHERE accountID = '%s'" %(accID))
    Type = cursor.fetchall()
    for tab in Type:
        if(tab[0] == '储蓄账户'):
            sql = "UPDATE saveacc SET interestrate = %s WHERE accountID = '%s'" %(interest, accID)
            cursor.execute(sql)
        elif(tab[0] == '支票账户'):
            sql = "UPDATE checkacc SET overdraft = %s WHERE accountID = '%s'" %(overdraft, accID)
    sql = "UPDATE accounts SET money = %s WHERE accountID = '%s'" %(money, accID)
    cursor.execute(sql)
    db.commit()
    cursor.close()

def db_CreateLoan(db, loanID, cusID, money):
    cursor = db.cursor()
    try:
        sql = "INSERT INTO loan VALUES('%s', %s, 'Liverpool', '0')" %(loanID, money)
        cursor.execute(sql)
        sql = "INSERT INTO cusforloan VALUES('%s', '%s')" %(loanID, cusID)
        cursor.execute(sql)
        db.commit()
    except:
        db.rollback()
    cursor.close()

def db_SearchLoan(db):
    cursor = db.cursor()
    sql = "SELECT cusforloan.loanID, cusforloan.cusID, loan.money, loan.state FROM loan, cusforloan WHERE loan.loanID = cusforloan.loanID"
    cursor.execute(sql)
    tabs = cursor.fetchall()
    res = list()
    for tab in tabs:
        loanID = tab[0]
        cusID = tab[1]
        money= tab[2]
        state = tab[3]
        res.append((loanID, cusID, money, state))
    cursor.close()
    return res

def db_DeleteLoan(db, loanID):
    cursor = db.cursor()
    try:
        sql = "DELETE FROM cusforloan WHERE loanID = '%s'" %(loanID)
        cursor.execute(sql)
        sql = "DELETE FROM loan WHERE loanID = '%s'" %(loanID)
        cursor.execute(sql)
        db.commit()
    except:
        db.rollback()
    cursor.close()

def db_PayLoan(db, loanID, cusID, money, paytime):
    cursor = db.cursor()
    try:
        sql = "INSERT INTO payinfo VALUES('%s', '%s', %s, '%s')" %(loanID, cusID, money, paytime)
        cursor.execute(sql)
        db.commit()
    except:
        db.rollback()
    cursor.close()

if __name__ == "__main__":
    db = db_login("root", "", "127.0.0.1", "test")
    tabs = db_showtable(db)
    db_close(db)