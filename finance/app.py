import os
from datetime import datetime
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    stocks = db.execute("SELECT name, symbol, shares FROM holdings WHERE user_id = ?", session["user_id"])
    cash_d = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    cash = usd(cash_d[0]["cash"])
    return render_template("index.html", stocks=stocks, cash=cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # input variables & error handling
        try:
            symbol = request.form.get("symbol")
            shares = int(request.form.get("shares"))
        except:
            error = "invalid input"
            return render_template("buy.html", error=error), 400

        # no negative numbers
        if shares < 0:
            return render_template("buy.html", error="Invalid number of shares"), 400
        # look up stock
        stock = lookup(symbol)
        if stock == None:
            return render_template("buy.html", error="Invalid Stock Symbol"), 400
        # variables
        user = session["user_id"]
        name = stock["name"]
        symbol = stock["symbol"]
        price = stock["price"]
        total = price * shares
        # retrive current cash holdings
        cash_dict = db.execute("SELECT cash FROM users where id = ?", user)
        cash = cash_dict[0]["cash"]
        # calculate new cash balance
        new_cash = cash - total
        if new_cash < 0:
            return render_template("buy.html", error="Not enough cash to buy"), 400
        # record transaction
        db.execute("INSERT INTO transactions (type, user_id, symbol, shares, price, total, time) VALUES ('buy', ?, ?, ?, ?, ?,?)",
                   user, symbol, shares, price, total, datetime.now().strftime("%d/%m/%Y %H:%M:%S"))
        # update user cash balance
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, user)
        # if already holding; increase holding, else insert new holding
        holdings = db.execute("SELECT id, symbol, shares FROM holdings WHERE user_id = ?", user)
        held = False
        # check holdings if already holding stock
        for held_stock in holdings:
            if symbol == held_stock["symbol"]:
                held = held_stock
                break
        # if already holding, update shares value
        if held != False:
            new_shares = int(held["shares"]) + shares
            db.execute("UPDATE holdings SET shares = ? WHERE id = ?", new_shares, held["id"])
        # else inser new row
        else:
            db.execute("INSERT INTO holdings (user_id, symbol, shares, name) VALUES (?, ?, ?, ?)", user, symbol, shares, name)
        # render template
        message = f" Successful purchase of {shares} shares of {name} ({symbol}) at {usd(price)} per share. Total : {usd(total)}. Remaining balance = {usd(new_cash)}"
        return render_template("buy.html", cash=cash, total=total, message=message), 200

    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    history = db.execute("SELECT * FROM transactions WHERE user_id = ?", session["user_id"])
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        username = string_checker(request.form.get("username"))
        if username != False:
            rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        else:
            return apology("invalid username and/or password", 400)

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("Empty symbol", 400)
        symbol = request.form.get("symbol")
        stock = lookup(symbol)
        if stock == None:
            return render_template("quote.html", message="Invalid Symbol", display=0), 400
        else:
            name = stock["name"]
            symbol = stock["symbol"]
            price = usd(stock["price"])
            return render_template("quote.html", name=name, symbol=symbol, price=price, display=1), 200
    return render_template("quote.html", display=0), 200


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # if already logged in redirect to home
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirm = request.form.get("confirmation")
        # if any text input is empty
        if not request.form.get("username") or not request.form.get("password"):
            return apology("Must fill out all fields", 400)
        # check if any invalid characters in username
        if string_checker(username) == False:
            return apology("invalid character or phrase in username", 400)
        # if username already exists
        user = db.execute("SELECT username FROM users WHERE username = ?", username)
        if len(user) != 0:
            return apology("username already in use", 400)
        # if password doesnt match
        if password != confirm:
            return apology("passwords don't match", 400)
        # all is well, add to database
        hash = generate_password_hash(password)
        db.execute("INSERT INTO users (username, hash, cash) VALUES (?,?, 10000)", username, hash)

        # log user in
        id_dict = db.execute("SELECT id FROM users WHERE username = ?", username)
        id = id_dict[0]["id"]
        session["user_id"] = id
        # test print: print(id, flush=True)
        return redirect("/")

    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    message = ""
    valid = True
    if request.method == "POST":
        # Symbol error handling
        if not request.form.get("symbol"):
            message = "Enter Symbol"
            valid = False
        else:
            symbol = string_checker(request.form.get("symbol"))
            if symbol == False:
                message = " Invalid Symbol "
                valid = False

        # shares error handling
        if valid:
            try:
                if request.form.get("type") == 'B':
                    shares = int(request.form.get("shares2"))
                else:
                    shares = int(request.form.get("shares"))
            except:
                message = " Input Error "
                valid = False
            if shares <= 0:
                message = " Invalid no. of shares "
                valid = False

        if valid:
            # get user holdings
            holding = db.execute("SELECT id, symbol, shares FROM holdings WHERE user_id = ? AND symbol = ?",
                                 session["user_id"], symbol)
            # if more than a single holding of a stock
            if len(holding) > 1:
                message = " DB error - too many stocks in list "
                valid = False
            # if no holdings of said stock
            elif len(holding) == 0:
                message = f"You hold any {symbol}"
                valid = False
            else:
                holding = holding[0]
                current_shares = holding["shares"]
                if current_shares <= 0:
                    message = "DB error - holding present for invalid no. of shares"
                    valid = False
                if shares > current_shares:
                    message = "Can't sell more stocks than you own"
                    valid = False

        if valid:
            # get price
            stock_lookup = lookup(symbol)
            if stock_lookup == None:
                message = "stock look up failed"
                valid = False
            else:
                price = stock_lookup["price"]

        if valid:
            user = session["user_id"]
            holding_id = holding["id"]
            # get current cash balance
            user_cash = db.execute("SELECT cash FROM users WHERE id = ?", user)
            current_cash = user_cash[0]["cash"]
            # calculate new cash balance
            delta_cash = price * shares
            new_cash = current_cash + delta_cash
            # if selling all shares
            if shares == current_shares:
                # delete holding
                message = "selling all"
                db.execute("DELETE FROM holdings WHERE id = ?", holding_id)
            # if selling less than current share holdings
            else:
                # update shares in holding
                new_shares = current_shares - shares
                db.execute("UPDATE holdings SET shares = ? WHERE id = ?", new_shares, holding_id)
            # update cash balance
            db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, user)
            # record transaction
            db.execute("INSERT INTO transactions (type, user_id, symbol, shares, price, total, time) VALUES ('sell', ?, ?, ?, ?, ?,?)",
                       user, symbol, shares, price, delta_cash, datetime.now().strftime("%d/%m/%Y %H:%M:%S"))
            message = f'sold {shares} of ({symbol}) at {usd(price)} each. Total sale: {usd(delta_cash)} Remaining cash: {usd(new_cash)}'

    # render out current holdings
    error = 200
    if valid == False:
        error = 400
    stocks = db.execute("SELECT id, name, symbol, shares FROM holdings WHERE user_id = ?", session["user_id"])
    cash_d = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    cash = usd(cash_d[0]["cash"])
    return render_template("sell.html", stocks=stocks, cash=cash, message=message), error


def string_checker(s):
    stro = s.lower()
    banned = ["insert", "delete", "empty", "drop", '"', "'", ";", ":", "=", "*", "&", "^", "%", "#", "/", ";"]
    if any(p in stro for p in banned):
        return False
    else:
        return s


@app.route("/account", methods=["GET", "POST"])
@login_required
def account():
    message = ""
    account = db.execute("SELECT username, cash FROM users WHERE id = ?", session["user_id"])

    if request.method == "POST":
        if not request.form.get("old") or not request.form.get("password") or not request.form.get("confirmation"):
            message = " Complete all fields "
            return render_template("account.html", account=account[0], message=message)
        # check old password matches
        old = request.form.get("old")
        hash_d = db.execute("SELECT hash FROM users WHERE id = ?", session["user_id"])
        valid = check_password_hash(hash_d[0]["hash"], old)
        if not valid:
            message = " old password is inccorect "
            return render_template("account.html", account=account[0], message=message)
        # check new passwords match
        new = request.form.get("password")
        confirm = request.form.get("confirmation")
        if new != confirm:
            message = " new passwords don't match "
            return render_template("account.html", account=account[0], message=message)
        # swap out new hash
        db.execute("UPDATE users SET hash = ? WHERE id = ?", generate_password_hash(new), session["user_id"])
        message = "password sucessfully changed!"

    return render_template("account.html", account=account[0], message=message)