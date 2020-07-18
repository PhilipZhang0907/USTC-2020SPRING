import functools

from flask import Flask, session, g
from flask import redirect
from flask import request, make_response
from flask import render_template
from flask import url_for

from db import *

# 生成一个app
app = Flask(__name__, instance_relative_config=True)
app.secret_key = 'lab3'

# 对app执行请求页面地址到函数的绑定
@app.route("/", methods=("GET", "POST"))
@app.route("/login", methods=("GET", "POST"))
def login():
    """Log in a registered user by adding the user id to the session."""
    if request.method == "POST":
        # 客户端在login页面发起的POST请求
        username = request.form["username"]
        password = request.form["password"]
        ipaddr   = request.form["ipaddr"]
        database = request.form["database"]

        db = db_login(username, "", ipaddr, database)

        if db == None:
            return render_template("login_fail.html")
        else:
            session['username'] = username
            session['password'] = ""
            session['ipaddr'] = ipaddr
            session['database'] = database

            return redirect(url_for('MainMenu'))
    else :
        # 客户端GET 请求login页面时
        return render_template("login.html")

# 请求url为host/table的页面返回结果
@app.route("/table", methods=(["GET", "POST"]))
def table():
    # 出于简单考虑，每次请求都需要连接数据库，可以尝试使用其它context保存数据库连接
    if 'username' in session:
        db = db_login(session['username'], session['password'], 
                        session['ipaddr'], session['database'])
        tabs = db_showtable(db)
        db_close(db)
    else:
        return redirect(url_for('login'))
    #
    if request.method == "POST":
        if 'clear' in request.form:
            return render_template("table.html", rows = '', dbname=session['database'])
        elif 'search' in request.form:
            return redirect(url_for('SearchCustomer'))

    else:
        return render_template("table.html", rows = tabs, dbname=session['database'])

@app.route("/MainMenu", methods=(["GET", "POST"]))
def MainMenu():
    if 'username' in session:
        db = db_login(session['username'], session['password'], 
                        session['ipaddr'], session['database'])
        tabs = db_SearchCustomer(db)
        db_close(db)
    else:
        return redirect(url_for('login'))
    # 
    if request.method == "POST":
        if 'Customer Management' in request.form:
            return redirect(url_for('CustomerMenu'))
        if 'Account Management' in request.form:
            return redirect(url_for('AccountMenu'))
        if 'Loan Management' in request.form:
            return redirect(url_for('LoanMenu'))
        if 'Business Summary' in request.form:
            return redirect(url_for('BusinessMenu'))
    else:
        return render_template("MainMenu.html")

@app.route("/CustomerMenu", methods=(["GET", "POST"]))
def CustomerMenu():
    if 'username' in session:
        db = db_login(session['username'], session['password'], 
                        session['ipaddr'], session['database'])
        tabs = db_SearchCustomer(db)
        db_close(db)
    else:
        return redirect(url_for('login'))
    #
    if request.method == "POST":
        if 'Insert' in request.form:
            return redirect(url_for('InsertCustomer'))
        if 'Delete' in request.form:
            return redirect(url_for('DeleteCustomer'))
        if 'Search' in request.form:
            return redirect(url_for('SearchCustomer'))
        if 'Update' in request.form:
            return redirect(url_for('UpdateCustomer'))
        if 'Back' in request.form:
            return redirect(url_for('MainMenu'))
    else:
        return render_template("CustomerMenu.html")

@app.route("/SearchCustomer", methods=(["GET", "POST"]))
def SearchCustomer():
    if 'username' in session:
        db = db_login(session['username'], session['password'], 
                        session['ipaddr'], session['database'])
        tabs = db_SearchCustomer(db)
        db_close(db)
    else:
        return redirect(url_for('login'))
    
    if request.method == "POST":
        if 'back' in request.form:
            return redirect(url_for('CustomerMenu'))
        elif 'contact' in request.form:
            return render_template("SearchCustomer.html", rows = tabs, dbname='Customer')

    else:
        return render_template("SearchCustomer.html", rows = tabs, dbname='Customer')

@app.route("/InsertCustomer", methods=(["GET", "POST"]))
def InsertCustomer():
    if 'username' in session:
        db = db_login(session['username'], session['password'], 
                        session['ipaddr'], session['database'])
    else:
        return redirect(url_for('login'))
    if request.method == "POST":
        cusID = request.form["cusID"]
        cusname = request.form["cusname"]
        cusphone = request.form["cusphone"]
        address = request.form["address"]
        contact_phone = request.form["contact_phone"]
        contact_name = request.form["contact_name"]
        contact_Email = request.form["contact_Email"]
        relation = request.form["relation"]
        db_InsertCustomer(db, cusID, cusname, cusphone, address, contact_phone, contact_name, contact_Email, relation)
        db_close(db)
        return redirect(url_for('CustomerMenu'))
    else:
        return render_template("InsertCustomer.html")

@app.route("/DeleteCustomer", methods=(["GET", "POST"]))
def DeleteCustomer():
    if 'username' in session:
        db = db_login(session['username'], session['password'], 
                        session['ipaddr'], session['database'])
    else:
        return redirect(url_for('login'))
    if request.method == "POST":
        cusID = request.form["cusID"]
        db_DeleteCustomer(db, cusID)
        db_close(db)
        return redirect(url_for('CustomerMenu'))
    else:
        return render_template("DeleteCustomer.html")

@app.route("/UpdateCustomer", methods=(["GET", "POST"]))
def UpdateCustomer():
    if 'username' in session:
        db = db_login(session['username'], session['password'], 
                        session['ipaddr'], session['database'])
    else:
        return redirect(url_for('login'))
    if request.method == "POST":
        cusID = request.form["cusID"]
        cusname = request.form["cusname"]
        cusphone = request.form["cusphone"]
        address = request.form["address"]
        contact_phone = request.form["contact_phone"]
        contact_name = request.form["contact_name"]
        contact_Email = request.form["contact_Email"]
        relation = request.form["relation"]
        db_UpdateCustomer(db, cusID, cusname, cusphone, address, contact_phone, contact_name, contact_Email, relation)
        db_close(db)
        return redirect(url_for('CustomerMenu'))
    else:
        return render_template("UpdateCustomer.html")


@app.route("/AccountMenu", methods=(["GET", "POST"]))
def AccountMenu():
    if 'username' in session:
        db = db_login(session['username'], session['password'], 
                        session['ipaddr'], session['database'])
        db_close(db)
    else:
        return redirect(url_for('login'))
    #
    if request.method == "POST":
        if 'Insert1' in request.form:
            return redirect(url_for('OpenSavingAcc'))
        if 'Insert2' in request.form:
            return redirect(url_for('OpenCheckingAcc'))
        if 'Delete' in request.form:
            return redirect(url_for('DeleteAcc'))
        if 'Search' in request.form:
            return redirect(url_for('SearchAcc'))
        if 'Update' in request.form:
            return redirect(url_for('UpdateAcc'))
        if 'Back' in request.form:
            return redirect(url_for('MainMenu'))
    else:
        return render_template("AccountMenu.html")

@app.route("/OpenSavingAcc", methods=(["GET", "POST"]))
def OpenSavingAcc():
    if 'username' in session:
        db = db_login(session['username'], session['password'], 
                        session['ipaddr'], session['database'])
    else:
        return redirect(url_for('login'))
    #
    if request.method == "POST":
        cusID = request.form["cusID"]
        accID = request.form["accID"]
        money = request.form["money"]
        settime = request.form["settime"]
        interest = request.form["interest"]
        saveType = request.form["saveType"]
        db_OpenSavingAcc(db, cusID, accID, money, settime, interest, saveType)
        db_close(db)
        return redirect(url_for('AccountMenu'))
    else:
        return render_template("OpenSavingAcc.html")

@app.route("/OpenCheckingAcc", methods=(["GET", "POST"]))
def OpenCheckingAcc():
    if 'username' in session:
        db = db_login(session['username'], session['password'], 
                        session['ipaddr'], session['database'])
    else:
        return redirect(url_for('login'))
    #
    if request.method == "POST":
        cusID = request.form["cusID"]
        accID = request.form["accID"]
        money = request.form["money"]
        settime = request.form["settime"]
        overdraft = request.form["overdraft"]
        db_OpenCheckingAcc(db, cusID, accID, money, settime, overdraft)
        db_close(db)
        return redirect(url_for('AccountMenu'))
    else:
        return render_template("OpenCheckingAcc.html")

@app.route("/SearchAcc", methods=(["GET", "POST"]))
def SearchAcc():
    if 'username' in session:
        db = db_login(session['username'], session['password'], 
                        session['ipaddr'], session['database'])
        tabs = db_SearchAcc(db)
        db_close(db)
    else:
        return redirect(url_for('login'))
    #
    if request.method == "POST":
        if 'back' in request.form:
            return redirect(url_for('AccountMenu'))
    else:
        return render_template("SearchAcc.html", rows = tabs, dbname='Customer')

@app.route("/DeleteAcc", methods=(["GET", "POST"]))
def DeleteAcc():
    if 'username' in session:
        db = db_login(session['username'], session['password'], 
                        session['ipaddr'], session['database'])
    else:
        return redirect(url_for('login'))
    #
    if request.method == "POST":
        accID = request.form["accID"]
        db_DeleteAcc(db, accID)
        db_close(db)
        return redirect(url_for('AccountMenu'))
    else:
        return render_template("DeleteAcc.html", tablename='Account')

@app.route("/UpdateAcc", methods=(["GET", "POST"]))
def UpdateAcc():
    if 'username' in session:
        db = db_login(session['username'], session['password'], 
                        session['ipaddr'], session['database'])
    else:
        return redirect(url_for('login'))
    #
    if request.method == "POST":
        accID = request.form["accID"]
        money = request.form["money"]
        interest = request.form["interest"]
        overdraft = request.form["overdraft"]
        db_UpdateAcc(db, accID, money, interest, overdraft)
        db_close(db)
        return redirect(url_for('AccountMenu'))
    else:
        return render_template("UpdateAcc.html")

@app.route("/LoanMenu", methods=(["GET", "POST"]))
def LoanMenu():
    if 'username' in session:
        db = db_login(session['username'], session['password'], 
                        session['ipaddr'], session['database'])
        db_close(db)
    else:
        return redirect(url_for('login'))
    #
    if request.method == "POST":
        if 'Create' in request.form:
            return redirect(url_for('CreateLoan'))
        if 'Search' in request.form:
            return redirect(url_for('SearchLoan'))
        if 'Delete' in request.form:
            return redirect(url_for('DeleteLoan'))
        if 'Pay' in request.form:
            return redirect(url_for('PayLoan'))
        if 'Back' in request.form:
            return redirect(url_for('MainMenu'))
    else:
        return render_template("LoanMenu.html")

@app.route("/CreateLoan", methods=(["GET", "POST"]))
def CreateLoan():
    if 'username' in session:
        db = db_login(session['username'], session['password'], 
                        session['ipaddr'], session['database'])
    else:
        return redirect(url_for('login'))
    #
    if request.method == "POST":
        loanID = request.form["loanID"]
        money = request.form["money"]
        cusID = request.form["cusID"]
        db_CreateLoan(db, loanID, cusID, money)
        db_close(db)
        return redirect(url_for('LoanMenu'))
    else:
        return render_template("CreateLoan.html", tablename='Loan')

@app.route("/SearchLoan", methods=(["GET", "POST"]))
def SearchLoan():
    if 'username' in session:
        db = db_login(session['username'], session['password'], 
                        session['ipaddr'], session['database'])
        tabs = db_SearchLoan(db)
        db_close(db)
    else:
        return redirect(url_for('login'))
    #
    if request.method == "POST":
        if 'back' in request.form:
            return redirect(url_for('LoanMenu'))
    else:
        return render_template("SearchLoan.html", rows = tabs, tablename='Loan')

@app.route("/DeleteLoan", methods=(["GET", "POST"]))
def DeleteLoan():
    if 'username' in session:
        db = db_login(session['username'], session['password'], 
                        session['ipaddr'], session['database'])
    else:
        return redirect(url_for('login'))
    #
    if request.method == "POST":
        loanID = request.form["loanID"]
        db_DeleteLoan(db, loanID)
        db_close(db)
        return redirect(url_for('LoanMenu'))
    else:
        return render_template("DeleteLoan.html", tablename='Loan')

@app.route("/PayLoan", methods=(["GET", "POST"]))
def PayLoan():
    if 'username' in session:
        db = db_login(session['username'], session['password'], 
                        session['ipaddr'], session['database'])
    else:
        return redirect(url_for('login'))
    #
    if request.method == "POST":
        loanID = request.form["loanID"]
        cusID = request.form["cusID"]
        money = request.form["money"]
        paytime = request.form["paytime"]
        db_PayLoan(db, loanID, cusID, money, paytime)
        db_close(db)
        return redirect(url_for('LoanMenu'))
    else:
        return render_template("PayLoan.html", tablename='Loan')

# 测试URL下返回html page
@app.route("/hello")
def hello():
    return "hello world!"

#返回不存在页面的处理
@app.errorhandler(404)
def not_found(e):
    return render_template("404.html")

if __name__ == "__main__":

    app.run(host = "0.0.0.0", debug=True)
