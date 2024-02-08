import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

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
    if not session["user_id"]:
        return render_template("login.html")

    results = []
    user_result = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
    owning_result = db.execute(
        "SELECT * FROM portfolios WHERE id = ?", session["user_id"]
    )

    for r in owning_result:
        symbol = r["symbol"]
        shares = r["shares"]
        price = lookup(symbol)["price"]
        value = r["value"]
        results.append(
            {"symbol": symbol, "shares": shares, "price": price, "value": value}
        )

    cash = user_result[0]["cash"]
    return render_template("index.html", results=results, cash=cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    # If we clicked the BUY button
    if request.method == "POST":
        # Get data from forms
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        type = "buy"
        # Search for the symbol data on the internet
        lookup_result = lookup(symbol)

        # Check some errors
        if not symbol:
            return apology("Invalid symbol!", 400)
        elif not lookup_result:
            return apology("Symbol not found!", 400)
        elif not shares.isdigit():
            return apology("Invalid number!", 400)
        elif int(shares) <= 0:
            return apology("Shares must be more than 0!", 400)

        shares = int(shares)
        # Get price and calculate total value for N shares, and calc current time
        price = float(lookup_result["price"])
        value = shares * price
        time = datetime.now().strftime("%d/%m/%Y %H:%M:%S")

        # Get cash value from database
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = cash[0]["cash"]

        # Check if there is enough cash for buying
        if value > cash:
            return apology("Not enough cash!", 403)

        # Get data from portfolios table to check if there is already the same bought symbols
        owning_data = db.execute(
            "SELECT * FROM portfolios WHERE id = ? AND symbol = ?",
            session["user_id"],
            symbol,
        )

        # Check if there is already bought symbols
        if not owning_data:
            sql_result_1 = db.execute(
                "INSERT INTO portfolios (id, symbol, shares, price, value) VALUES(?, ?, ?, ?, ?)",
                session["user_id"],
                symbol,
                shares,
                price,
                value,
            )
        else:
            # Get new values of shares, prices, values, cash
            n_shares = owning_data[0]["shares"] + shares
            n_price = (
                (owning_data[0]["shares"] * owning_data[0]["price"]) + (shares * price)
            ) / 2
            n_value = n_shares * price
            sql_result_1 = db.execute(
                "UPDATE portfolios SET shares = ?, price = ?, value = ? WHERE id = ? AND symbol = ?",
                n_shares,
                n_price,
                n_value,
                session["user_id"],
                symbol,
            )

        # Calculate new value of cash
        n_cash = cash - value

        # Write a data into purchcases table
        sql_result_2 = db.execute(
            "INSERT INTO history (id, type, symbol, shares, price, value, time, cash) VALUES(?, ?, ?, ?, ?, ?, ?, ?)",
            session["user_id"],
            type,
            symbol,
            shares,
            price,
            value,
            time,
            n_cash,
        )

        # Check if there are SQL errors
        if not sql_result_1 or not sql_result_2:
            return apology("Unknown error with database!", 403)

        # Take cash from database account and update it value
        cash = cash - value
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash, session["user_id"])

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    if not session["user_id"]:
        return render_template("login.html")

    history_results = db.execute(
        "SELECT * FROM history WHERE id = ? ORDER BY time DESC", session["user_id"]
    )

    return render_template("history.html", results=history_results)


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
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
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
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Empty symbol!", 400)
        result = lookup(symbol)
        if not result:
            return apology("Invalid symbol", 400)
        return render_template("quoted.html", result=result)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":
        """Register user"""
        username = request.form.get("username")
        password = request.form.get("password")
        password_confirmation = request.form.get("confirmation")
        hash = generate_password_hash(password)
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        if not username:
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not password:
            return apology("must provide password", 400)

        elif password != password_confirmation:
            return apology("passwords are not the same", 400)

        elif len(rows) > 0:
            return apology("account already exist!", 400)

        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, hash)

        return render_template("login.html")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "POST":
        # Get data from forms
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        type = "sell"
        # Search for the symbol data on the internet
        lookup_result = lookup(symbol)

        # Check some errors
        if not symbol:
            return apology("Invalid symbol!", 400)
        elif not lookup_result:
            return apology("Symbol not found!", 400)
        elif not shares.isdigit():
            return apology("Invalid number!", 400)
        elif int(shares) <= 0:
            return apology("Shares must be more than 0!", 400)

        shares = int(shares)

        # Get price and calculate total value for N shares, and calc current time
        price = float(lookup_result["price"])
        value = shares * price
        time = datetime.now().strftime("%d/%m/%Y %H:%M:%S")

        # Get cash value from database
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = cash[0]["cash"]

        # Get data from portfolios table to check if there is already the same bought symbols
        owning_data = db.execute(
            "SELECT * FROM portfolios WHERE id = ? AND symbol = ?",
            session["user_id"],
            symbol,
        )

        # Check if there is already bought symbols
        if not owning_data or shares > owning_data[0]["shares"]:
            return apology("User don't have such symbols in such amount!", 400)
        else:
            # Get new values of shares, prices, values
            n_shares = owning_data[0]["shares"] - shares
            n_price = price
            n_value = n_shares * price
            sql_result_1 = db.execute(
                "UPDATE portfolios SET shares = ?, price = ?, value = ? WHERE id = ? AND symbol = ?",
                n_shares,
                n_price,
                n_value,
                session["user_id"],
                symbol,
            )

        # Calculate new value of cash
        n_cash = cash + value

        # Write a data into sellings table
        sql_result_2 = db.execute(
            "INSERT INTO history (id, type, symbol, shares, price, value, time, cash) VALUES(?, ?, ?, ?, ?, ?, ?, ?)",
            session["user_id"],
            type,
            symbol,
            shares,
            price,
            value,
            time,
            n_cash,
        )

        # Check if there are SQL errors
        if not sql_result_1 or not sql_result_2:
            return apology("Unknown error with database!", 403)

        # Take cash from database account and update it value
        cash = cash + value
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash, session["user_id"])

        # Check if row in portfolio is empty
        owning_data = db.execute(
            "SELECT * FROM portfolios WHERE id = ? AND symbol = ?",
            session["user_id"],
            symbol,
        )
        # Check if there is already bought symbols
        if owning_data[0]["shares"] == 0:
            db.execute(
                "DELETE FROM portfolios WHERE id = ? AND symbol = ?",
                session["user_id"],
                symbol,
            )

        return redirect("/")

    else:
        owning_data = db.execute(
            "SELECT * FROM portfolios WHERE id = ?", session["user_id"]
        )
        return render_template("sell.html", results=owning_data)
