import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    
    shares = []
    prices = []
    companies = []
    Sum = 0
    """Show portfolio of stocks"""
    
    # Gets all the companies symbols that the user owns
    symbols = db.execute("SELECT DISTINCT Company_symbol FROM user_ex WHERE username_id = ?", session["user_id"])
    
    # Gets the price of the shares the user owns at the momment and the company name
    for symbol in symbols:
        prices.append(lookup(symbol["Company_symbol"])["price"])
        companies.append(lookup(symbol["Company_symbol"])["name"])
        
    # Gets the sum of shares of the differents companies the user owns    
    shar = db.execute(
        "SELECT SUM(shares) FROM user_ex WHERE username_id = ? GROUP BY Company_symbol ORDER BY date", session["user_id"])
    
    # Converts dic to list because of SUM error
    for share in shar:
        shares.append(share["SUM(shares)"])
    
    # Gets the total sum of money from shares       
    for share, price in zip(shares, prices):
        Sum = Sum + share * price
        
    return render_template("index.html", info=zip(symbols, shares, prices, companies), cash=db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"], Sum=Sum)
    

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        
        # Ensure symbol is valid
        if not symbol or not lookup(symbol):
            return apology("Invalid Symbol", 400)
            
        # Ensure shares is int
        try:
            int(shares)
        except:
            return apology("Invalid value for shares", 400)
        
        # Ensure share is positive integer  
        if int(shares) <= 0 or isinstance(shares, float): 
            return apology("Invalid value for shares", 400)
        
        # Transforms string into int
        shares = int(shares)
        
        # Checks if user as enough money to by the shares
        if lookup(symbol)["price"] * shares > db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]:
            return apology("You don't have enough cash to purchase this", 400)
        
        # Gets all the info fo the db   
        previous_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
        current_cash = previous_cash - shares * lookup(symbol)["price"]
        exchange = "- {:.2f}".format(shares * lookup(symbol)["price"])
        
        # Updates da db
        db.execute("UPDATE users SET cash = ? WHERE id = ?", current_cash, session["user_id"])
        db.execute("INSERT INTO user_ex (previous_cash, current_cash, exchange, Company_symbol,shares, username_id) VALUES (?,?,?,?,?,?)", 
                   previous_cash, current_cash, exchange, symbol.upper(), +shares, session["user_id"])
        return redirect("/")
        
    else:
        return render_template("buy.html")
        
    return apology("TODO")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    exchanges = db.execute("SELECT * FROM user_ex WHERE username_id = ?", session["user_id"])
    return render_template("history.html", exchanges=exchanges)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
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
    if request.method == "POST":

        symbol = request.form.get("symbol")
        
        # Ensure symbol exists
        if lookup(symbol):
            return render_template("quoted.html", dic=lookup(symbol), usd=usd)
            
        else:
            return apology("Invalid Symbol", 400)
        
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST": 
        
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        
        # Checks if password and confirmation are the same
        if password != confirmation:
            return apology("Empty username", 400)
        
        # Checks if a username and password were given
        if not username or not password or not confirmation:
            return apology("Empty username", 400)
        
        # Checks if username isnt in use
        for user in db.execute("SELECT username FROM users"):
            if db.execute("SELECT 1 FROM users WHERE username = ?", username):
                return apology("Username already exists", 400)
        
        # Add new user to the database       
        db.execute("INSERT INTO users (username, hash) VALUES (?,?)", username, generate_password_hash(password))
        
    else:
        return render_template("register.html")
        
    return redirect("/login")
    
    
@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        
        shares = request.form.get("shares")
        symbol = request.form.get("symbol")
        
        # Ensure it is an integer  
        try:
            int(shares)
        except:
            return apology("Invalid value for shares", 400)
    
        # Ensure is a positive integer
        if int(shares) <= 0 or isinstance(shares, float): 
            return apology("Invalid value for shares", 400)
        
        # Transforms string to int  
        shares = int(shares)
        
        # Ensure you can't sell more than you have
        if shares > db.execute("SELECT SUM(shares) FROM user_ex WHERE username_id = ? AND Company_symbol = ? ", session["user_id"], symbol.upper())[0]["SUM(shares)"]:
            return apology("You don't have enough shares to sell", 400)
           
        # Gets all the info to add to the db   
        previous_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
        current_cash = previous_cash + shares * lookup(symbol)["price"]
        exchange = "+ {:.2f}".format(shares * lookup(symbol)["price"])
        
        # Updates db
        db.execute("UPDATE users SET cash = ? WHERE id = ?", current_cash, session["user_id"])
        db.execute("INSERT INTO user_ex (previous_cash, current_cash, exchange, Company_symbol,shares, username_id) VALUES (?,?,?,?,?,?)", 
                   previous_cash, current_cash, exchange, symbol.upper(), -shares, session["user_id"])
        return redirect("/")
        
    else:
        # Gets all the company stocks it has a returns symbol
        symbols = db.execute("SELECT DISTINCT Company_symbol FROM user_ex WHERE username_id = ?", session["user_id"])
        return render_template("sell.html", symbols=symbols)
        

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)


@app.route("/change", methods=["GET", "POST"])
def change():
    """Change password"""
    
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        username = request.form.get("username")
        current_password = request.form.get("current_password")
        new_password = request.form.get("new_password")
        confirmation = request.form.get("confirmation")

        # Ensure username was submitted
        if not username:
            return apology("must provide username", 403)

        # Ensure password is submitted the same as previous
        elif not current_password:
            return apology("Previous password is wrong", 403)
            
        # Esure new password is equal to confirmation
        elif new_password != confirmation:
            return apology("Passwords don't match", 400)
            
        # Query database for username
        rows = db.execute("UPDATE users SET hash = ? WHERE username = ?", generate_password_hash(new_password), username)

        # Redirect user to home page
        return redirect("/login")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("change.html")