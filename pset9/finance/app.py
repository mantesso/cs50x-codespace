import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd


# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


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
    user_id = session["user_id"]
    user_balance = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
    stocks = db.execute(
        "SELECT symbol, SUM(shares) AS shares FROM shares WHERE user_id = ? GROUP BY symbol",
        user_id,
    )
    print(stocks)

    # Filter out stocks with 0 shares
    filtered_stocks = [stock for stock in stocks if stock["shares"] != 0]

    stock_total_value = 0
    for stock in filtered_stocks:
        stock["current_price"] = lookup(stock["symbol"])["price"]
        stock["total_value"] = stock["current_price"] * stock["shares"]
        stock_total_value += stock["total_value"]

    return render_template(
        "index.html",
        stocks=filtered_stocks,
        user_balance=user_balance,
        grand_total=stock_total_value + user_balance,
    )


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    symbol = request.form.get("symbol")
    shares = request.form.get("shares")

    if request.method == "POST":
        # Render an apology if the input is blank or the symbol does not exist
        if not symbol:
            return apology("missing symbol", 400)
        elif not shares:
            return apology("missing shares", 400)
        elif not lookup(symbol):
            return apology("invalid symbol", 400)

        # handles fractional, negative, and non-numeric shares
        try:
            int(shares)
        except ValueError:
            return apology("invalid shares", 400)

        if int(shares) <= 0:
            return apology("invalid shares", 400)

        user_id = session["user_id"]
        user_balance = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0][
            "cash"
        ]
        current_price = lookup(symbol)["price"]
        transaction_total = current_price * int(shares)

        # Render an apology if the user cannot afford the number of shares at the current price.
        if user_balance < transaction_total:
            return apology("insufficient funds", 400)

        # log purchase into shares table
        db.execute(
            "INSERT INTO shares (user_id, symbol, shares, price, transacted) VALUES(?, ?, ?, ?, CURRENT_TIMESTAMP)",
            user_id,
            symbol.upper(),
            int(shares),
            current_price,
        )

        # subtract transaction_total from users cash and update value on users table
        new_user_balance = user_balance - transaction_total
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_user_balance, user_id)

        # Redirect user to home page
        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    stocks = db.execute("SELECT * FROM shares WHERE user_id = ?", user_id)

    return render_template("history.html", stocks=stocks)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

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
    symbol = request.form.get("symbol")

    if request.method == "POST":
        # if blank ticker symbol
        if not symbol:
            return apology("symbol is required", 400)

        # handles invalid ticker symbol
        quote = lookup(symbol)
        if not quote:
            return apology("invalid ticker symbol", 400)

        # formats quoted price and render quoted template
        quote["price"] = usd(quote["price"])
        return render_template("quoted.html", quote=quote)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    username = request.form.get("username")
    password = request.form.get("password")
    confirmation = request.form.get("confirmation")

    if request.method == "POST":
        # Render an apology if either input is blank or the passwords do not match.
        if not username or not password or not confirmation:
            return apology("All fields are required")
        elif password != confirmation:
            return apology("Password and confirmation should match")

        # check if username already exists
        elif db.execute("SELECT username FROM users WHERE username = ?", username):
            return apology("Username already exists", 400)

        # register the user
        # generate hashed password
        hashed = generate_password_hash(confirmation)

        # save user in the db
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, hashed)

        # Redirect user to home page
        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]
    stocks = db.execute(
        "SELECT symbol, SUM(shares) AS shares FROM shares WHERE user_id = ? GROUP BY symbol",
        user_id,
    )
    stock_symbols = [stock["symbol"] for stock in stocks]

    if request.method == "POST":
        selling_symbol = request.form.get("symbol")
        selling_qty = request.form.get("shares")

        # Render an apology if the user fails to select a stock
        if not selling_symbol:
            return apology("failed to select stock", 400)

        # Render apology if user does not own any shares of that stock
        if selling_symbol not in stock_symbols:
            return apology("you don't own this stock", 400)

        # Render an apology if the input shares is not a positive integer
        if int(selling_qty) <= 0:
            return apology("invalid selling qty", 400)

        # Render an apology if the user does not own that many shares of the stock
        for stock in stocks:
            if stock["symbol"] == selling_symbol:
                own_qty = stock["shares"]

        if int(selling_qty) > own_qty:
            return apology("you don't have that many stocks", 400)

        # check current price for selling stock
        current_price = lookup(selling_symbol)["price"]

        # log sell into shares table
        db.execute(
            "INSERT INTO shares (user_id, symbol, shares, price, transacted) VALUES(?, ?, ?, ?, CURRENT_TIMESTAMP)",
            user_id,
            selling_symbol.upper(),
            -int(selling_qty),
            current_price,
        )

        # add sell value to user cash
        user_balance = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0][
            "cash"
        ]
        transaction_total = current_price * int(selling_qty)
        new_user_balance = user_balance + transaction_total
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_user_balance, user_id)

        # Redirect user to home page
        return redirect("/")

    return render_template("sell.html", stock_symbols=stock_symbols)
